/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.1.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "AboutBox.h"
class SphereScope;
class ToggleSwitch;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PluginView  : public Component,
                    private ValueTree::Listener,
                    public ButtonListener,
                    public SliderListener
{
public:
    //==============================================================================
    PluginView ();
    ~PluginView();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void stabilizeComponents (ValueTree pluginState);
    void setSphereValue (const float val);
    void mouseDown (const MouseEvent& ev) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AboutBox about;
    ValueTree pluginState;
    BigInteger combs, allpasses;
    Array<Button*> combButtons, allPassButtons;

    friend class ValueTree::Listener;
    virtual void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;
    virtual void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override {}
    virtual void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override {}
    virtual void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override {}
    virtual void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged) override { }
    virtual void valueTreeRedirected (ValueTree& treeWhichHasBeenChanged) override { }
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<SphereScope> sphere;
    ScopedPointer<ToggleSwitch> comb4;
    ScopedPointer<ToggleButton> frozen;
    ScopedPointer<Label> roomSizeLabel;
    ScopedPointer<Label> dampingLabel;
    ScopedPointer<Label> wetLabel;
    ScopedPointer<Label> dryLabel;
    ScopedPointer<Label> widthLabel;
    ScopedPointer<ToggleSwitch> comb1;
    ScopedPointer<ToggleSwitch> comb2;
    ScopedPointer<ToggleSwitch> comb5;
    ScopedPointer<ToggleSwitch> comb6;
    ScopedPointer<ToggleSwitch> comb3;
    ScopedPointer<ToggleSwitch> comb7;
    ScopedPointer<ToggleSwitch> comb8;
    ScopedPointer<ToggleSwitch> allpass1;
    ScopedPointer<ToggleSwitch> allpass2;
    ScopedPointer<ToggleSwitch> allpass3;
    ScopedPointer<ToggleSwitch> allpass4;
    ScopedPointer<kv::SkinDial> width;
    ScopedPointer<kv::SkinDial> damping;
    ScopedPointer<kv::SkinDial> roomSize;
    ScopedPointer<kv::SkinDial> wetLevel;
    ScopedPointer<kv::SkinDial> dryLevel;
    ScopedPointer<Label> label;
    ScopedPointer<Label> label3;
    ScopedPointer<TextButton> helpButton;
    ScopedPointer<Drawable> drawable1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginView)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
