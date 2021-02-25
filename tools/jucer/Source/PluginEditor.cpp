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
#include "PluginView.h"

RoboverbAudioProcessorEditor::RoboverbAudioProcessorEditor (RoboverbAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setLookAndFeel (&lookAndFeel);
    setOpaque (true);
    view.reset (new PluginView());
    addAndMakeVisible (view.get());
    view->stabilizeComponents (p.getState());
    setSize (view->getWidth(), view->getHeight());
    startTimer (40);
}

RoboverbAudioProcessorEditor::~RoboverbAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
    stopTimer();
}

void RoboverbAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);
}

void RoboverbAudioProcessorEditor::resized()
{

}

void RoboverbAudioProcessorEditor::timerCallback()
{
    view->setSphereValue (processor.getRMS());
}
