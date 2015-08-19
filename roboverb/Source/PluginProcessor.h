/*
  PluginProcessor.h - This file is part of Roboverb

  Roboverb: robotic voice and ambience audio effect
  Copyright (C) 2015  Kushview, LLC.  All rights reserved.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "Roboverb.h"

class RoboverbAudioProcessor  : public AudioProcessor,
                                public ValueTree::Listener
{
public:
    RoboverbAudioProcessor();
    ~RoboverbAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    int getNumParameters() override;
    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const String getParameterName (int index) override;
    const String getParameterText (int index) override;

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void updateParameters (Roboverb::Parameters& params);
    ValueTree getState() const { return state; }

    float getRMS() const { return rmsValue.get(); }

private:
    const int numParameters;
    const int numKnobs;
    ValueTree state;
    Roboverb::Parameters params;
    Roboverb verb;

    Atomic<float> rmsValue;

    void updateState();

    virtual void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged,
                                           const Identifier& property);
    virtual void valueTreeChildAdded (ValueTree& parentTree,
                                      ValueTree& childWhichHasBeenAdded) {}
    virtual void valueTreeChildRemoved (ValueTree& parentTree,
                                        ValueTree& childWhichHasBeenRemoved,
                                        int indexFromWhichChildWasRemoved) {}
    virtual void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved,
                                             int oldIndex, int newIndex) {}
    virtual void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged) {}
    virtual void valueTreeRedirected (ValueTree& treeWhichHasBeenChanged){ }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RoboverbAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
