
#include <juce_audio_processors/juce_audio_processors.h>
#include "roboverb.hpp"
#include "ui/pluginview.hpp"

using namespace juce;

namespace roboverb {

class Editor : public juce::AudioProcessorEditor {
public:
    Editor (AudioProcessor& p) : juce::AudioProcessorEditor (p) {
        addAndMakeVisible (_view);
        setSize (_view.getWidth(), _view.getHeight());
    }

    ~Editor() {}

    void resized() override {
        _view.setTopLeftPosition (0, 0);
    }

    void paint (juce::Graphics& g) override {
        g.fillAll (juce::Colours::black);
    }

private:
    PluginView _view;
};

class Processor : public juce::AudioProcessor {
    Roboverb _verb;

public:
    Processor() : juce::AudioProcessor() {}
    ~Processor() override {}

    //==============================================================================
    const String getName() const override { return "Roboverb"; }
    // virtual StringArray getAlternateDisplayNames() const;

    //==============================================================================

    void prepareToPlay (double sampleRate, int) override {
        _verb.reset();
        _verb.setSampleRate (sampleRate);
    }

    void releaseResources() override {}

    void memoryWarningReceived() override { jassertfalse; }

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override {
        _verb.processStereo (buffer.getWritePointer (0),
                             buffer.getWritePointer (1),
                             buffer.getWritePointer (0),
                             buffer.getWritePointer (1),
                             buffer.getNumSamples());
    }

    void processBlock (AudioBuffer<double>& buffer, MidiBuffer& midiMessages) override {
        AudioProcessor::processBlock (buffer, midiMessages);
    }
#if 0
    
    // virtual void processBlockBypassed (AudioBuffer<float>& buffer, MidiBuffer& midiMessages);
    // virtual void processBlockBypassed (AudioBuffer<double>& buffer, MidiBuffer& midiMessages);
#endif

#if 0
    //==============================================================================
    virtual bool canAddBus (bool isInput) const;
    virtual bool canRemoveBus (bool isInput) const;
#endif
    //==============================================================================
    // virtual bool supportsDoublePrecisionProcessing() const;

    //==============================================================================
    double getTailLengthSeconds() const override { return 0.0; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }

    //==============================================================================
    void reset() override { _verb.reset(); }

    //==============================================================================
    bool hasEditor() const override { return true; }
    AudioProcessorEditor* createEditor() override {
        return new Editor (*this);
    }

    //==============================================================================
    int getNumPrograms() override { return 1; }

    /** Returns the number of the currently active program. */
    int getCurrentProgram() override { return 0; }

    /** Called by the host to change the current program. */
    void setCurrentProgram (int index) override { juce::ignoreUnused (index); }

    /** Must return the name of a given program. */
    const String getProgramName (int index) override {
        juce::ignoreUnused (index);
        return "Default";
    }

    /** Called by the host to rename a program. */
    void changeProgramName (int index, const String& newName) override {
        juce::ignoreUnused (index, newName);
    }

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override {
        juce::ignoreUnused (destData);
    }

    void setStateInformation (const void* data, int sizeInBytes) override {
        juce::ignoreUnused (data, sizeInBytes);
    }

#if 0
    /** The host will call this method if it wants to save the state of just the processor's
        current program.

        Unlike getStateInformation, this should only return the current program's state.

        Not all hosts support this, and if you don't implement it, the base class
        method just calls getStateInformation() instead. If you do implement it, be
        sure to also implement getCurrentProgramStateInformation.

        @see getStateInformation, setCurrentProgramStateInformation
    */
    virtual void getCurrentProgramStateInformation (juce::MemoryBlock& destData);

    /** The host will call this method if it wants to restore the state of just the processor's
        current program.

        Not all hosts support this, and if you don't implement it, the base class
        method just calls setStateInformation() instead. If you do implement it, be
        sure to also implement getCurrentProgramStateInformation.

        @see setStateInformation, getCurrentProgramStateInformation
    */
    virtual void setCurrentProgramStateInformation (const void* data, int sizeInBytes);

    /** This method is called when the total number of input or output channels is changed. */
    virtual void numChannelsChanged();

    /** This method is called when the number of buses is changed. */
    virtual void numBusesChanged();

    /** This method is called when the layout of the audio processor changes. */
    virtual void processorLayoutsChanged();

    //==============================================================================
    /** Adds a listener that will be called when an aspect of this processor changes. */
    virtual void addListener (AudioProcessorListener* newListener);

    /** Removes a previously added listener. */
    virtual void removeListener (AudioProcessorListener* listenerToRemove);

    //==============================================================================
    /** Tells the processor to use this playhead object.
        The processor will not take ownership of the object, so the caller must delete it when
        it is no longer being used.
    */
    virtual void setPlayHead (AudioPlayHead* newPlayHead);

    //==============================================================================
    /** AAX plug-ins need to report a unique "plug-in id" for every audio layout
        configuration that your AudioProcessor supports on the main bus. Override this
        function if you want your AudioProcessor to use a custom "plug-in id" (for example
        to stay backward compatible with older versions of JUCE).

        The default implementation will compute a unique integer from the input and output
        layout and add this value to the 4 character code 'jcaa' (for native AAX) or 'jyaa'
        (for AudioSuite plug-ins).
    */
    virtual int32 getAAXPluginIDForMainBusConfig (const AudioChannelSet& mainInputLayout,
                                                  const AudioChannelSet& mainOutputLayout,
                                                  bool idForAudioSuite) const;

    virtual CurveData getResponseCurve (CurveData::Type /*curveType*/) const      { return {}; }

    /** Informs the AudioProcessor that track properties such as the track's name or
        colour has been changed.

        If you are hosting this AudioProcessor then use this method to inform the
        AudioProcessor about which track the AudioProcessor is loaded on. This method
        may only be called on the message thread.

        If you are implementing an AudioProcessor then you can override this callback
        to do something useful with the track properties such as changing the colour
        of your AudioProcessor's editor. It's entirely up to the host when and how
        often this callback will be called.

        The default implementation of this callback will do nothing.
    */
    virtual void updateTrackProperties (const TrackProperties& properties);
#endif

protected:
#if 0
    /** Callback to query if the AudioProcessor supports a specific layout.

        This callback is called when the host probes the supported bus layouts via
        the checkBusesLayoutSupported method. You should override this callback if you
        would like to limit the layouts that your AudioProcessor supports. The default
        implementation will accept any layout. JUCE does basic sanity checks so that
        the provided layouts parameter will have the same number of buses as your
        AudioProcessor.

        @see checkBusesLayoutSupported
    */
    virtual bool isBusesLayoutSupported (const BusesLayout&) const          { return true; }

    /** Callback to check if a certain bus layout can now be applied.

        Most subclasses will not need to override this method and should instead
        override the isBusesLayoutSupported callback to reject certain layout changes.

        This callback is called when the user requests a layout change. It will only be
        called if processing of the AudioProcessor has been stopped by a previous call to
        releaseResources or after the construction of the AudioProcessor. It will be called
        just before the actual layout change. By returning false you will abort the layout
        change and setBusesLayout will return false indicating that the layout change
        was not successful.

        The default implementation will simply call isBusesLayoutSupported.

        You only need to override this method if there is a chance that your AudioProcessor
        may not accept a layout although you have previously claimed to support it via the
        isBusesLayoutSupported callback. This can occur if your AudioProcessor's supported
        layouts depend on other plug-in parameters which may have changed since the last
        call to isBusesLayoutSupported, such as the format of an audio file which can be
        selected by the user in the AudioProcessor's editor. This callback gives the
        AudioProcessor a last chance to reject a layout if conditions have changed as it
        is always called just before the actual layout change.

        As it is never called while the AudioProcessor is processing audio, it can also
        be used for AudioProcessors which wrap other plug-in formats to apply the current
        layout to the underlying plug-in. This callback gives such AudioProcessors a
        chance to reject the layout change should an error occur with the underlying plug-in
        during the layout change.

        @see isBusesLayoutSupported, setBusesLayout
    */
    virtual bool canApplyBusesLayout (const BusesLayout& layouts) const     { return isBusesLayoutSupported (layouts); }

    /** This method will be called when a new bus layout needs to be applied.

        Most subclasses will not need to override this method and should just use the default
        implementation.
    */
    virtual bool applyBusLayouts (const BusesLayout& layouts);

    /** Callback to query if adding/removing buses currently possible.

        This callback is called when the host calls addBus or removeBus.
        Similar to canApplyBusesLayout, this callback is only called while
        the AudioProcessor is stopped and gives the processor a last
        chance to reject a requested bus change. It can also be used to apply
        the bus count change to an underlying wrapped plug-in.

        When adding a bus, isAddingBuses will be true and the plug-in is
        expected to fill out outNewBusProperties with the properties of the
        bus which will be created just after the successful return of this callback.

        Implementations of AudioProcessor will rarely need to override this
        method. Only override this method if your processor supports adding
        and removing buses and if it needs more fine grain control over the
        naming of new buses or may reject bus number changes although canAddBus
        or canRemoveBus returned true.

        The default implementation will return false if canAddBus/canRemoveBus
        returns false (the default behavior). Otherwise, this method returns
        "Input #busIndex" for input buses and "Output #busIndex" for output buses
        where busIndex is the index for newly created buses. The default layout
        in this case will be the layout of the previous bus of the same direction.
    */
    virtual bool canApplyBusCountChange (bool isInput, bool isAddingBuses,
                                         BusProperties& outNewBusProperties);
#endif
};

} // namespace roboverb

JUCE_PUBLIC_FUNCTION
juce::AudioProcessor* createPluginFilter() {
    return new roboverb::Processor();
}
