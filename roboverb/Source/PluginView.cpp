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

//[Headers] You can add your own extra header files here...
#include "Roboverb.h"
#include "SphereScope.h"
//[/Headers]

#include "PluginView.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
PluginView::PluginView ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    combs.setRange (0, 8, false);
    allpasses.setRange (0, 4, false);
    //[/Constructor_pre]

    sphere.reset (new SphereScope());
    addAndMakeVisible (sphere.get());
    sphere->setName ("sphere");

    sphere->setBounds (130, 18, 96, 94);

    comb4.reset (new ToggleSwitch ("comb4"));
    addAndMakeVisible (comb4.get());
    comb4->setButtonText (String());
    comb4->addListener (this);

    comb4->setBounds (318, 50, 40, 40);

    frozen.reset (new ToggleButton ("frozen"));
    addAndMakeVisible (frozen.get());
    frozen->setButtonText (TRANS("Frozen"));
    frozen->addListener (this);
    frozen->setColour (ToggleButton::textColourId, Colour (0xffe4e4e4));

    frozen->setBounds (176, 176, 104, 24);

    roomSizeLabel.reset (new Label ("roomSizeLabel",
                                    TRANS("Room Size")));
    addAndMakeVisible (roomSizeLabel.get());
    roomSizeLabel->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    roomSizeLabel->setJustificationType (Justification::centred);
    roomSizeLabel->setEditable (false, false, false);
    roomSizeLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    roomSizeLabel->setColour (TextEditor::textColourId, Colours::black);
    roomSizeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    roomSizeLabel->setBounds (13, 159, 56, 24);

    dampingLabel.reset (new Label ("dampingLabel",
                                   TRANS("Damping")));
    addAndMakeVisible (dampingLabel.get());
    dampingLabel->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    dampingLabel->setJustificationType (Justification::centred);
    dampingLabel->setEditable (false, false, false);
    dampingLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    dampingLabel->setColour (TextEditor::textColourId, Colours::black);
    dampingLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    dampingLabel->setBounds (72, 159, 56, 24);

    wetLabel.reset (new Label ("wetLabel",
                               TRANS("Wet")));
    addAndMakeVisible (wetLabel.get());
    wetLabel->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    wetLabel->setJustificationType (Justification::centred);
    wetLabel->setEditable (false, false, false);
    wetLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    wetLabel->setColour (TextEditor::textColourId, Colours::black);
    wetLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    wetLabel->setBounds (14, 94, 56, 24);

    dryLabel.reset (new Label ("dryLabel",
                               TRANS("Dry")));
    addAndMakeVisible (dryLabel.get());
    dryLabel->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    dryLabel->setJustificationType (Justification::centred);
    dryLabel->setEditable (false, false, false);
    dryLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    dryLabel->setColour (TextEditor::textColourId, Colours::black);
    dryLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    dryLabel->setBounds (64, 94, 72, 24);

    widthLabel.reset (new Label ("widthLabel",
                                 TRANS("Width")));
    addAndMakeVisible (widthLabel.get());
    widthLabel->setTooltip (TRANS("Width"));
    widthLabel->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    widthLabel->setJustificationType (Justification::centred);
    widthLabel->setEditable (false, false, false);
    widthLabel->setColour (Label::textColourId, Colour (0xe4dfddaf));
    widthLabel->setColour (TextEditor::textColourId, Colours::black);
    widthLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    widthLabel->setBounds (131, 159, 56, 24);

    comb1.reset (new ToggleSwitch ("comb1"));
    addAndMakeVisible (comb1.get());
    comb1->setButtonText (String());
    comb1->addListener (this);

    comb1->setBounds (219, 50, 40, 40);

    comb2.reset (new ToggleSwitch ("comb2"));
    addAndMakeVisible (comb2.get());
    comb2->setButtonText (String());
    comb2->addListener (this);

    comb2->setBounds (252, 50, 40, 40);

    comb5.reset (new ToggleSwitch ("comb5"));
    addAndMakeVisible (comb5.get());
    comb5->setButtonText (String());
    comb5->addListener (this);

    comb5->setBounds (219, 87, 40, 40);

    comb6.reset (new ToggleSwitch ("comb6"));
    addAndMakeVisible (comb6.get());
    comb6->setButtonText (String());
    comb6->addListener (this);

    comb6->setBounds (252, 87, 40, 40);

    comb3.reset (new ToggleSwitch ("comb3"));
    addAndMakeVisible (comb3.get());
    comb3->setButtonText (String());
    comb3->addListener (this);

    comb3->setBounds (285, 50, 40, 40);

    comb7.reset (new ToggleSwitch ("comb7"));
    addAndMakeVisible (comb7.get());
    comb7->setButtonText (String());
    comb7->addListener (this);

    comb7->setBounds (285, 87, 40, 40);

    comb8.reset (new ToggleSwitch ("comb8"));
    addAndMakeVisible (comb8.get());
    comb8->setButtonText (String());
    comb8->addListener (this);

    comb8->setBounds (318, 87, 40, 40);

    allpass1.reset (new ToggleSwitch ("allpass1"));
    addAndMakeVisible (allpass1.get());
    allpass1->setButtonText (String());
    allpass1->addListener (this);

    allpass1->setBounds (219, 124, 40, 40);

    allpass2.reset (new ToggleSwitch ("allpass2"));
    addAndMakeVisible (allpass2.get());
    allpass2->setButtonText (String());
    allpass2->addListener (this);

    allpass2->setBounds (252, 124, 40, 40);

    allpass3.reset (new ToggleSwitch ("allpass3"));
    addAndMakeVisible (allpass3.get());
    allpass3->setButtonText (String());
    allpass3->addListener (this);

    allpass3->setBounds (285, 124, 40, 40);

    allpass4.reset (new ToggleSwitch ("allpass4"));
    addAndMakeVisible (allpass4.get());
    allpass4->setButtonText (String());
    allpass4->addListener (this);

    allpass4->setBounds (318, 124, 40, 40);

    label.reset (new Label ("new label",
                            TRANS("Chambers\n")));
    addAndMakeVisible (label.get());
    label->setFont (Font (12.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (Label::textColourId, Colour (0xe4dfddaf));
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (221, 159, 136, 24);

    label3.reset (new Label ("new label",
                             TRANS("ROBOVERB")));
    addAndMakeVisible (label3.get());
    label3->setFont (Font (16.00f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredRight);
    label3->setEditable (false, false, false);
    label3->setColour (Label::textColourId, Colour (0xd9dfddaf));
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label3->setBounds (270, 20, 78, 24);

    helpButton.reset (new TextButton ("helpButton"));
    addAndMakeVisible (helpButton.get());
    helpButton->setButtonText (TRANS("?"));
    helpButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    helpButton->addListener (this);
    helpButton->setColour (TextButton::buttonColourId, Colour (0xe1111111));
    helpButton->setColour (TextButton::buttonOnColourId, Colour (0xff111111));
    helpButton->setColour (TextButton::textColourOffId, Colour (0xe7e5e5e5));
    helpButton->setColour (TextButton::textColourOnId, Colour (0xdee5e5e5));

    helpButton->setBounds (9, 23, 18, 18);

    wetLevel.reset (new kv::SkinDial ("wetLevel"));
    addAndMakeVisible (wetLevel.get());
    wetLevel->setRange (0, 1, 0);
    wetLevel->setSliderStyle (Slider::RotaryVerticalDrag);
    wetLevel->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    wetLevel->addListener (this);

    wetLevel->setBounds (14, 46, 56, 56);

    dryLevel.reset (new kv::SkinDial ("dryLevel"));
    addAndMakeVisible (dryLevel.get());
    dryLevel->setRange (0, 1, 0);
    dryLevel->setSliderStyle (Slider::RotaryVerticalDrag);
    dryLevel->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    dryLevel->addListener (this);

    dryLevel->setBounds (73, 46, 56, 56);

    roomSize.reset (new kv::SkinDial ("roomSize"));
    addAndMakeVisible (roomSize.get());
    roomSize->setRange (0, 1, 0);
    roomSize->setSliderStyle (Slider::RotaryVerticalDrag);
    roomSize->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    roomSize->addListener (this);

    roomSize->setBounds (13, 111, 56, 56);

    damping.reset (new kv::SkinDial ("damping"));
    addAndMakeVisible (damping.get());
    damping->setRange (0, 1, 0);
    damping->setSliderStyle (Slider::RotaryVerticalDrag);
    damping->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    damping->addListener (this);

    damping->setBounds (72, 111, 56, 56);

    width.reset (new kv::SkinDial ("width"));
    addAndMakeVisible (width.get());
    width->setRange (0, 1, 0);
    width->setSliderStyle (Slider::RotaryVerticalDrag);
    width->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    width->addListener (this);

    width->setBounds (131, 111, 56, 56);

    drawable1.reset (Drawable::createFromImageData (BinaryData::roboverb_bg_jpg, BinaryData::roboverb_bg_jpgSize));

    //[UserPreSize]
    combButtons.add (comb1.get());
    combButtons.add (comb2.get());
    combButtons.add (comb3.get());
    combButtons.add (comb4.get());
    combButtons.add (comb5.get());
    combButtons.add (comb6.get());
    combButtons.add (comb7.get());
    combButtons.add (comb8.get());
    allPassButtons.add (allpass1.get());
    allPassButtons.add (allpass2.get());
    allPassButtons.add (allpass3.get());
    allPassButtons.add (allpass4.get());

    frozen->setVisible (false);

    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        if (kv::SkinDial* sd = dynamic_cast<kv::SkinDial*> (
            getChildComponent(i)))
        {
            sd->setScale (2);
            sd->setImage (ImageCache::getFromMemory(BinaryData::red_knob_png, BinaryData::red_knob_pngSize), false);
        }
    }
    //[/UserPreSize]

    setSize (360, 210);


    //[Constructor] You can add your own custom stuff here..
    about.setPluginName (JucePlugin_Name);
    about.setPluginVersion (JucePlugin_VersionString);
    about.setPluginUrl ("Kushview, LLC", "https://kushview.net/products/roboverb");
    pluginState.addListener (this);
    //[/Constructor]
}

PluginView::~PluginView()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pluginState.removeListener(this);
    combButtons.clear();
    allPassButtons.clear();
    //[/Destructor_pre]

    sphere = nullptr;
    comb4 = nullptr;
    frozen = nullptr;
    roomSizeLabel = nullptr;
    dampingLabel = nullptr;
    wetLabel = nullptr;
    dryLabel = nullptr;
    widthLabel = nullptr;
    comb1 = nullptr;
    comb2 = nullptr;
    comb5 = nullptr;
    comb6 = nullptr;
    comb3 = nullptr;
    comb7 = nullptr;
    comb8 = nullptr;
    allpass1 = nullptr;
    allpass2 = nullptr;
    allpass3 = nullptr;
    allpass4 = nullptr;
    label = nullptr;
    label3 = nullptr;
    helpButton = nullptr;
    wetLevel = nullptr;
    dryLevel = nullptr;
    roomSize = nullptr;
    damping = nullptr;
    width = nullptr;
    drawable1 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PluginView::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff2d2b24));

    {
        int x = 0, y = 0, width = proportionOfWidth (1.0000f), height = proportionOfHeight (1.0000f);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable1 != 0);
        if (drawable1 != 0)
            drawable1->drawWithin (g, Rectangle<float> (x, y, width, height),
                                   RectanglePlacement::stretchToFit, 1.000f);
    }

    {
        int x = 0, y = 0, width = proportionOfWidth (1.0000f), height = proportionOfHeight (1.0000f);
        Colour fillColour1 = Colour (0xa35c0000), fillColour2 = Colour (0xaa401818);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (ColourGradient (fillColour1,
                                       56.0f - 0.0f + x,
                                       static_cast<float> (-56) - 0.0f + y,
                                       fillColour2,
                                       424.0f - 0.0f + x,
                                       224.0f - 0.0f + y,
                                       true));
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PluginView::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PluginView::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    if (combButtons.contains(buttonThatWasClicked))
    {
        combs.setBit(combButtons.indexOf(buttonThatWasClicked),
                     buttonThatWasClicked->getToggleState());
        pluginState.setProperty("enabledCombs", combs.toString(2), nullptr);
        return;
    }
    else if (allPassButtons.contains(buttonThatWasClicked))
    {
        allpasses.setBit (allPassButtons.indexOf (buttonThatWasClicked),
                          buttonThatWasClicked->getToggleState());
        pluginState.setProperty("enabledAllPasses", allpasses.toString(2), nullptr);
        return;
    }
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == comb4.get())
    {
        //[UserButtonCode_comb4] -- add your button handler code here..
        //[/UserButtonCode_comb4]
    }
    else if (buttonThatWasClicked == frozen.get())
    {
        //[UserButtonCode_frozen] -- add your button handler code here..
        //[/UserButtonCode_frozen]
    }
    else if (buttonThatWasClicked == comb1.get())
    {
        //[UserButtonCode_comb1] -- add your button handler code here..
        //[/UserButtonCode_comb1]
    }
    else if (buttonThatWasClicked == comb2.get())
    {
        //[UserButtonCode_comb2] -- add your button handler code here..
        //[/UserButtonCode_comb2]
    }
    else if (buttonThatWasClicked == comb5.get())
    {
        //[UserButtonCode_comb5] -- add your button handler code here..
        //[/UserButtonCode_comb5]
    }
    else if (buttonThatWasClicked == comb6.get())
    {
        //[UserButtonCode_comb6] -- add your button handler code here..
        //[/UserButtonCode_comb6]
    }
    else if (buttonThatWasClicked == comb3.get())
    {
        //[UserButtonCode_comb3] -- add your button handler code here..
        //[/UserButtonCode_comb3]
    }
    else if (buttonThatWasClicked == comb7.get())
    {
        //[UserButtonCode_comb7] -- add your button handler code here..
        //[/UserButtonCode_comb7]
    }
    else if (buttonThatWasClicked == comb8.get())
    {
        //[UserButtonCode_comb8] -- add your button handler code here..
        //[/UserButtonCode_comb8]
    }
    else if (buttonThatWasClicked == allpass1.get())
    {
        //[UserButtonCode_allpass1] -- add your button handler code here..
        //[/UserButtonCode_allpass1]
    }
    else if (buttonThatWasClicked == allpass2.get())
    {
        //[UserButtonCode_allpass2] -- add your button handler code here..
        //[/UserButtonCode_allpass2]
    }
    else if (buttonThatWasClicked == allpass3.get())
    {
        //[UserButtonCode_allpass3] -- add your button handler code here..
        //[/UserButtonCode_allpass3]
    }
    else if (buttonThatWasClicked == allpass4.get())
    {
        //[UserButtonCode_allpass4] -- add your button handler code here..
        //[/UserButtonCode_allpass4]
    }
    else if (buttonThatWasClicked == helpButton.get())
    {
        //[UserButtonCode_helpButton] -- add your button handler code here..
        if (! about.isVisible()) {
            addAndMakeVisible(about);
            about.setVisible(true);
            about.centreWithSize(about.getWidth(), about.getHeight());
        } else {
            removeChildComponent(&about);
            about.setVisible(false);
        }
        //[/UserButtonCode_helpButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void PluginView::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == wetLevel.get())
    {
        //[UserSliderCode_wetLevel] -- add your slider handling code here..
        //[/UserSliderCode_wetLevel]
    }
    else if (sliderThatWasMoved == dryLevel.get())
    {
        //[UserSliderCode_dryLevel] -- add your slider handling code here..
        //[/UserSliderCode_dryLevel]
    }
    else if (sliderThatWasMoved == roomSize.get())
    {
        //[UserSliderCode_roomSize] -- add your slider handling code here..
        //[/UserSliderCode_roomSize]
    }
    else if (sliderThatWasMoved == damping.get())
    {
        //[UserSliderCode_damping] -- add your slider handling code here..
        //[/UserSliderCode_damping]
    }
    else if (sliderThatWasMoved == width.get())
    {
        //[UserSliderCode_width] -- add your slider handling code here..
        //[/UserSliderCode_width]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void PluginView::stabilizeComponents (ValueTree newPluginState)
{
    pluginState = newPluginState;

    roomSize->setValue(pluginState.getProperty("roomSize"), dontSendNotification);
    roomSize->getValueObject().referTo(
        pluginState.getPropertyAsValue ("roomSize", nullptr));

    dryLevel->setValue(pluginState.getProperty("dryLevel"), dontSendNotification);
    dryLevel->getValueObject().referTo(
        pluginState.getPropertyAsValue ("dryLevel", nullptr));

    wetLevel->setValue(pluginState.getProperty("wetLevel"), dontSendNotification);
    wetLevel->getValueObject().referTo(
        pluginState.getPropertyAsValue ("wetLevel", nullptr));

    width->setValue(pluginState.getProperty("width"), dontSendNotification);
    width->getValueObject().referTo(
        pluginState.getPropertyAsValue ("width", nullptr));

    damping->setValue(pluginState.getProperty("damping"), dontSendNotification);
    damping->getValueObject().referTo(
        pluginState.getPropertyAsValue ("damping", nullptr));


    combs.parseString(pluginState.getProperty(Tags::enabledCombs).toString(), 2);
    allpasses.parseString(pluginState.getProperty(Tags::enabledAllPasses).toString(), 2);

    for (int i = 0; i < combButtons.size(); ++i)
        combButtons.getUnchecked(i)->setToggleState(combs[i], dontSendNotification);
    for (int i = 0; i < allPassButtons.size(); ++i)
        allPassButtons.getUnchecked(i)->setToggleState(allpasses[i], dontSendNotification);

}

void PluginView::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
    if (tree != pluginState)
        return;

    const var& value (tree.getProperty (property));
    if (property == Tags::enabledCombs)
    {
        combs.parseString (value.toString(), 2);
        for (int i = 0; i < combButtons.size(); ++i)
            combButtons.getUnchecked(i)->setToggleState(combs[i], dontSendNotification);
    }
    else if (property == Tags::enabledAllPasses)
    {
        allpasses.parseString(value.toString(), 2);
        for (int i = 0; i < allPassButtons.size(); ++i)
            allPassButtons.getUnchecked(i)->setToggleState(allpasses[i], dontSendNotification);
    }
}

void PluginView::setSphereValue (const float val)
{
    sphere->setValue (val);
    sphere->repaint();
}

void PluginView::mouseDown (const MouseEvent& ev)
{
    if (about.isVisible())
        about.setVisible(false);
    Component::mouseDown (ev);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PluginView" componentName=""
                 parentClasses="public Component, private ValueTree::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="360"
                 initialHeight="210">
  <BACKGROUND backgroundColour="ff2d2b24">
    <IMAGE pos="0 0 100% 100%" resource="BinaryData::roboverb_bg_jpg" opacity="1"
           mode="0"/>
    <RECT pos="0 0 100% 100%" fill=" radial: 56 -56, 424 224, 0=a35c0000, 1=aa401818"
          hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="sphere" id="4672cf09df25d92f" memberName="sphere" virtualName="SphereScope"
                    explicitFocusOrder="0" pos="130 18 96 94" class="Component" params=""/>
  <TOGGLEBUTTON name="comb4" id="1b56d74e200d200a" memberName="comb4" virtualName="ToggleSwitch"
                explicitFocusOrder="0" pos="318 50 40 40" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="frozen" id="b5b45bb641f23158" memberName="frozen" virtualName=""
                explicitFocusOrder="0" pos="176 176 104 24" txtcol="ffe4e4e4"
                buttonText="Frozen" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="roomSizeLabel" id="9c584537021a4c92" memberName="roomSizeLabel"
         virtualName="" explicitFocusOrder="0" pos="13 159 56 24" textCol="e4dfddaf"
         edTextCol="ff000000" edBkgCol="0" labelText="Room Size" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.2e1" kerning="0" bold="0" italic="0" justification="36"/>
  <LABEL name="dampingLabel" id="102f11db8649f0a8" memberName="dampingLabel"
         virtualName="" explicitFocusOrder="0" pos="72 159 56 24" textCol="e4dfddaf"
         edTextCol="ff000000" edBkgCol="0" labelText="Damping" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.2e1" kerning="0" bold="0" italic="0" justification="36"/>
  <LABEL name="wetLabel" id="193cc8bfd3ef648e" memberName="wetLabel" virtualName=""
         explicitFocusOrder="0" pos="14 94 56 24" textCol="e4dfddaf" edTextCol="ff000000"
         edBkgCol="0" labelText="Wet" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="1.2e1"
         kerning="0" bold="0" italic="0" justification="36"/>
  <LABEL name="dryLabel" id="8f9c5d2033dcb149" memberName="dryLabel" virtualName=""
         explicitFocusOrder="0" pos="64 94 72 24" textCol="e4dfddaf" edTextCol="ff000000"
         edBkgCol="0" labelText="Dry" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="1.2e1"
         kerning="0" bold="0" italic="0" justification="36"/>
  <LABEL name="widthLabel" id="b5e5129090968ba2" memberName="widthLabel"
         virtualName="" explicitFocusOrder="0" pos="131 159 56 24" tooltip="Width"
         textCol="e4dfddaf" edTextCol="ff000000" edBkgCol="0" labelText="Width"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.2e1" kerning="0" bold="0"
         italic="0" justification="36"/>
  <TOGGLEBUTTON name="comb1" id="10f369ee64dc606a" memberName="comb1" virtualName="ToggleSwitch"
                explicitFocusOrder="0" pos="219 50 40 40" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="comb2" id="3764a52bf0c0c270" memberName="comb2" virtualName="ToggleSwitch"
                explicitFocusOrder="0" pos="252 50 40 40" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="comb5" id="84eff46ba18183d3" memberName="comb5" virtualName="ToggleSwitch"
                explicitFocusOrder="0" pos="219 87 40 40" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="comb6" id="51298beb77515717" memberName="comb6" virtualName="ToggleSwitch"
                explicitFocusOrder="0" pos="252 87 40 40" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="comb3" id="ce5ceb453a90039b" memberName="comb3" virtualName="ToggleSwitch"
                explicitFocusOrder="0" pos="285 50 40 40" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="comb7" id="bded4b7757f9b1f0" memberName="comb7" virtualName="ToggleSwitch"
                explicitFocusOrder="0" pos="285 87 40 40" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="comb8" id="31ca9d756f9bb291" memberName="comb8" virtualName="ToggleSwitch"
                explicitFocusOrder="0" pos="318 87 40 40" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="allpass1" id="2254b724ccf48cf5" memberName="allpass1" virtualName="ToggleSwitch"
                explicitFocusOrder="0" pos="219 124 40 40" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="allpass2" id="3ab1aa0ce31d54e7" memberName="allpass2" virtualName="ToggleSwitch"
                explicitFocusOrder="0" pos="252 124 40 40" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="allpass3" id="b38aa8a508e947" memberName="allpass3" virtualName="ToggleSwitch"
                explicitFocusOrder="0" pos="285 124 40 40" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="allpass4" id="6573319772fb73a6" memberName="allpass4" virtualName="ToggleSwitch"
                explicitFocusOrder="0" pos="318 124 40 40" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="eb1801d07e80ba9d" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="221 159 136 24" textCol="e4dfddaf"
         edTextCol="ff000000" edBkgCol="0" labelText="Chambers&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.2e1" kerning="0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="3d2124da91d9b2f0" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="270 20 78 24" textCol="d9dfddaf"
         edTextCol="ff000000" edBkgCol="0" labelText="ROBOVERB" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.6e1" kerning="0" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="helpButton" id="545fc9f3f2bf952a" memberName="helpButton"
              virtualName="" explicitFocusOrder="0" pos="9 23 18 18" bgColOff="e1111111"
              bgColOn="ff111111" textCol="e7e5e5e5" textColOn="dee5e5e5" buttonText="?"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="wetLevel" id="60f92ab9097a3a68" memberName="wetLevel" virtualName="kv::SkinDial"
          explicitFocusOrder="0" pos="14 46 56 56" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="dryLevel" id="71c4babfc54096a3" memberName="dryLevel" virtualName="kv::SkinDial"
          explicitFocusOrder="0" pos="73 46 56 56" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="roomSize" id="4818250426324d72" memberName="roomSize" virtualName="kv::SkinDial"
          explicitFocusOrder="0" pos="13 111 56 56" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="damping" id="36e7b175ad3e7b34" memberName="damping" virtualName="kv::SkinDial"
          explicitFocusOrder="0" pos="72 111 56 56" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="width" id="eb11bcc99f28a586" memberName="width" virtualName="kv::SkinDial"
          explicitFocusOrder="0" pos="131 111 56 56" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

