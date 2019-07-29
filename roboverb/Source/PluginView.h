/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

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
                    public Button::Listener,
                    public Slider::Listener
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
    std::unique_ptr<SphereScope> sphere;
    std::unique_ptr<ToggleSwitch> comb4;
    std::unique_ptr<ToggleButton> frozen;
    std::unique_ptr<Label> roomSizeLabel;
    std::unique_ptr<Label> dampingLabel;
    std::unique_ptr<Label> wetLabel;
    std::unique_ptr<Label> dryLabel;
    std::unique_ptr<Label> widthLabel;
    std::unique_ptr<ToggleSwitch> comb1;
    std::unique_ptr<ToggleSwitch> comb2;
    std::unique_ptr<ToggleSwitch> comb5;
    std::unique_ptr<ToggleSwitch> comb6;
    std::unique_ptr<ToggleSwitch> comb3;
    std::unique_ptr<ToggleSwitch> comb7;
    std::unique_ptr<ToggleSwitch> comb8;
    std::unique_ptr<ToggleSwitch> allpass1;
    std::unique_ptr<ToggleSwitch> allpass2;
    std::unique_ptr<ToggleSwitch> allpass3;
    std::unique_ptr<ToggleSwitch> allpass4;
    std::unique_ptr<kv::SkinDial> width;
    std::unique_ptr<kv::SkinDial> damping;
    std::unique_ptr<kv::SkinDial> roomSize;
    std::unique_ptr<kv::SkinDial> wetLevel;
    std::unique_ptr<kv::SkinDial> dryLevel;
    std::unique_ptr<Label> label;
    std::unique_ptr<Label> label3;
    std::unique_ptr<TextButton> helpButton;
    std::unique_ptr<Drawable> drawable1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginView)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

