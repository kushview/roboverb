/*
    This file is part of Roboverb

    Copyright (C) 2015-2019  Kushview, LLC.  All rights reserved.

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

#include "PluginProcessor.h"
#include "PluginEditor.h"


RoboverbAudioProcessor::RoboverbAudioProcessor()
    : numParameters (Roboverb::numParameters - 1 + 12),
      numKnobs (Roboverb::numParameters - 1),
      state ("roboverb")
{
    updateState();
    state.addListener (this);
}

RoboverbAudioProcessor::~RoboverbAudioProcessor()
{
    state.removeListener(this);
}


const String RoboverbAudioProcessor::getName() const { return JucePlugin_Name; }
int RoboverbAudioProcessor::getNumParameters()
{
    // This version of roberb doesn't use FreezeMode. Thankfully
    // this is the last parameter hence -1
    // +12 is for the number of switches
    return numParameters;
}

float RoboverbAudioProcessor::getParameter (int index)
{
    if (index >= numKnobs && index < numParameters)
    {
        const int filterIndex = index - numKnobs;

        if (filterIndex < 8) {
            return verb.toggledCombFloat(filterIndex);
        } else {
            return verb.toggledAllPassFloat(filterIndex - 8);
        }
    }
    else
    {
        const Roboverb::Parameters& p (verb.getParameters());
        switch (index)
        {
            case Roboverb::RoomSize:    return p.roomSize; break;
            case Roboverb::Damping:     return p.damping; break;
            case Roboverb::WetLevel:    return p.wetLevel; break;
            case Roboverb::DryLevel:    return p.dryLevel; break;
            case Roboverb::Width:       return p.width; break;
            case Roboverb::FreezeMode:  return p.freezeMode; break;
        }
    }

    return 0.0f;
}

void RoboverbAudioProcessor::setParameter (int index, float newValue)
{
    if (index >= numKnobs && index < numParameters)
    {
        ScopedLock lock (getCallbackLock());
        const bool isSet = (newValue < 0.5) ? false : true;
        const int filterIndex = index - numKnobs;
        if (filterIndex < 8) {
            verb.setCombToggle (filterIndex, isSet);
        } else {
            verb.setAllPassToggle (filterIndex - 8, isSet);
        }
    }
    else
    {
        switch (index)
        {
            case Roboverb::RoomSize: params.roomSize = newValue; break;
            case Roboverb::Damping: params.damping = newValue; break;
            case Roboverb::WetLevel: params.wetLevel = newValue; break;
            case Roboverb::DryLevel: params.dryLevel = newValue; break;
            case Roboverb::Width:  params.width = newValue; break;
            case Roboverb::FreezeMode: params.freezeMode = newValue; break;
        }

        ScopedLock lock (getCallbackLock());
        if (params != verb.getParameters())
            verb.setParameters (params);
    }
}

const String RoboverbAudioProcessor::getParameterName (int index)
{
    if (index >= numKnobs && index < numParameters) {
        const int filterIndex = index - numKnobs;
        if (filterIndex < 8) {
            return String ("Comb Filter ") + String(filterIndex + 1);
        } else {
            return String ("AllPass Filter ") + String(filterIndex - 8 + 1);
        }
    }

    switch (index)
    {
        case Roboverb::RoomSize:   return "Room size"; break;
        case Roboverb::Damping:    return "Damping";  break;
        case Roboverb::WetLevel:   return "Wet level";  break;
        case Roboverb::DryLevel:   return "Dry level";  break;
        case Roboverb::Width:      return "Width";  break;
        case Roboverb::FreezeMode: return "Freeze mode";  break;
    }

    return String();
}

const String RoboverbAudioProcessor::getParameterText (int index)
{
    if (index >= numKnobs && index < numParameters) {
        getParameterName (index - numKnobs);
    }

    switch (index)
    {
        case Roboverb::RoomSize:   return "Room size (text)"; break;
        case Roboverb::Damping:    return "Damping (text)";  break;
        case Roboverb::WetLevel:   return "Wet level (text)";  break;
        case Roboverb::DryLevel:   return "Dry level (text)";  break;
        case Roboverb::Width:      return "Width (text)";  break;
        case Roboverb::FreezeMode: return "Freeze mode (text)";  break;
    }

    return String();
}

const String RoboverbAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String RoboverbAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool RoboverbAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool RoboverbAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool RoboverbAudioProcessor::acceptsMidi() const { return false; }
bool RoboverbAudioProcessor::producesMidi() const { return false; }
bool RoboverbAudioProcessor::silenceInProducesSilenceOut() const { return false; }
double RoboverbAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int RoboverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RoboverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RoboverbAudioProcessor::setCurrentProgram (int index)
{
}

const String RoboverbAudioProcessor::getProgramName (int index)
{
    return String();
}

void RoboverbAudioProcessor::changeProgramName (int index, const String& newName)
{
}


void RoboverbAudioProcessor::prepareToPlay (double sampleRate, int /*samplesPerBlock*/)
{
    verb.reset();
    verb.setSampleRate (sampleRate);
}

void RoboverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void RoboverbAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (buffer.getNumChannels() >= 2)
    {
        verb.processStereo (buffer.getWritePointer(0), buffer.getWritePointer(1),
                            buffer.getNumSamples());
        rmsValue.set ((buffer.getRMSLevel(0, 0, buffer.getNumSamples()) +
                       buffer.getRMSLevel(1, 0, buffer.getNumSamples())) * 0.5f);
    }
    else if (buffer.getNumChannels() == 1)
    {
        verb.processMono (buffer.getWritePointer (0),
                          buffer.getNumSamples());
        rmsValue.set (buffer.getRMSLevel (0, 0, buffer.getNumSamples()));
    }
}


bool RoboverbAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* RoboverbAudioProcessor::createEditor()
{
    updateState();
    return new RoboverbAudioProcessorEditor (*this);
}


void RoboverbAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    updateState();
    MemoryOutputStream stream (destData, false);
    state.writeToStream (stream);
}

void RoboverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
   #if ! KV_PLUGINS_DEMO
    MemoryInputStream stream (data, (size_t) sizeInBytes, false);
    ValueTree nextState = ValueTree::readFromStream (stream);
    if (nextState.isValid())
        state.copyPropertiesFrom (nextState, nullptr);
   #endif
}

void RoboverbAudioProcessor::updateState()
{
    state.removeListener (this);
    BigInteger combs, allpasses;
    {
        combs.setRange (0, 8, false);
        allpasses.setRange(0, 4, false);

        ScopedLock sl (getCallbackLock());
        params = verb.getParameters();
        verb.getEnablement (combs, allpasses);
    }

    state.setProperty (Tags::roomSize, params.roomSize, nullptr);
    state.setProperty (Tags::damping, params.damping, nullptr);
    state.setProperty (Tags::wetLevel, params.wetLevel, nullptr);
    state.setProperty (Tags::dryLevel, params.dryLevel, nullptr);
    state.setProperty (Tags::width, params.width, nullptr);
    state.setProperty (Tags::freezeMode, params.freezeMode, nullptr);
    state.setProperty (Tags::enabledAllPasses, allpasses.toString(2), nullptr);
    state.setProperty (Tags::enabledCombs, combs.toString(2), nullptr);
    state.addListener(this);
}

void RoboverbAudioProcessor::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
    const var& value (tree.getProperty (property));

    if (property == Tags::roomSize)
    {
        setParameter (Roboverb::RoomSize, (float) value);
    }
    else if (property == Tags::damping)
    {
        setParameter (Roboverb::Damping, (float) value);
    }
    else if (property == Tags::dryLevel)
    {
        setParameter (Roboverb::DryLevel, (float) value);
    }
    else if (property == Tags::wetLevel)
    {
        setParameter(Roboverb::WetLevel, (float) value);
    }
    else if (property == Tags::freezeMode)
    {
        setParameter(Roboverb::FreezeMode, (float) value);
    }
    else if (property == Tags::width)
    {
        setParameter(Roboverb::Width, (float) value);
    }
    else if (property == Tags::enabledCombs)
    {
        BigInteger enabled;
        enabled.parseString (value.toString(), 2);
        ScopedLock sl (getCallbackLock());
        verb.swapEnabledCombs (enabled);
    }
    else if (property == Tags::enabledAllPasses)
    {
        BigInteger enabled;
        enabled.parseString (value.toString(), 2);
        ScopedLock sl (getCallbackLock());
        verb.swapEnabledAllPasses (enabled);
    }
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RoboverbAudioProcessor();
}
