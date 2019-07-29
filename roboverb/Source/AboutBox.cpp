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
//[/Headers]

#include "AboutBox.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AboutBox::AboutBox ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    pluginLabel.reset (new Label ("pluginLabel",
                                  TRANS("Roboverb")));
    addAndMakeVisible (pluginLabel.get());
    pluginLabel->setFont (Font (24.00f, Font::plain).withTypefaceStyle ("Regular"));
    pluginLabel->setJustificationType (Justification::centred);
    pluginLabel->setEditable (false, false, false);
    pluginLabel->setColour (Label::textColourId, Colour (0xffe5e5e5));
    pluginLabel->setColour (TextEditor::textColourId, Colours::black);
    pluginLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    versionLabel.reset (new Label ("versionLabel",
                                   TRANS("v1.0.2")));
    addAndMakeVisible (versionLabel.get());
    versionLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    versionLabel->setJustificationType (Justification::centred);
    versionLabel->setEditable (false, false, false);
    versionLabel->setColour (Label::textColourId, Colour (0xffe4e4e4));
    versionLabel->setColour (TextEditor::textColourId, Colours::black);
    versionLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    linkButton.reset (new HyperlinkButton (TRANS("kushview.net"),
                                           URL ("http://kushview.net")));
    addAndMakeVisible (linkButton.get());
    linkButton->setTooltip (TRANS("http://kushview.net"));
    linkButton->setButtonText (TRANS("kushview.net"));
    linkButton->setColour (HyperlinkButton::textColourId, Colour (0xcc3677ba));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (300, 176);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

AboutBox::~AboutBox()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    pluginLabel = nullptr;
    versionLabel = nullptr;
    linkButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AboutBox::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xc3101010));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AboutBox::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    pluginLabel->setBounds ((getWidth() / 2) + -155, (getHeight() / 2) + -49, 312, 32);
    versionLabel->setBounds ((getWidth() / 2) + -101, (getHeight() / 2) + -15, 192, 24);
    linkButton->setBounds ((getWidth() / 2) + -76, (getHeight() / 2) + 9, 150, 26);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void AboutBox::mouseDown (const MouseEvent& ev)
{
    if (isVisible())
        setVisible (false);
}

void AboutBox::setPluginName (const String& name)
{
    pluginLabel->setText ((KV_PLUGINS_DEMO) ? name + String(" (demo)") : name,
                          dontSendNotification);
}

void AboutBox::setPluginVersion (const String& versionString) {
    versionLabel->setText (versionString, dontSendNotification);
}

void AboutBox::setPluginUrl (const String& text, const String& url)
{
    linkButton.reset (new HyperlinkButton (text, URL(url)));
    addAndMakeVisible (linkButton.get());
    linkButton->setColour (HyperlinkButton::textColourId, Colour (0xcc3677ba));
    resized();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AboutBox" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="300" initialHeight="176">
  <BACKGROUND backgroundColour="c3101010"/>
  <LABEL name="pluginLabel" id="6fdd824b8f8a4ee0" memberName="pluginLabel"
         virtualName="" explicitFocusOrder="0" pos="-155C -49C 312 32"
         textCol="ffe5e5e5" edTextCol="ff000000" edBkgCol="0" labelText="Roboverb"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="2.4e1" kerning="0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="versionLabel" id="68d7814acd2571b3" memberName="versionLabel"
         virtualName="" explicitFocusOrder="0" pos="-101C -15C 192 24"
         textCol="ffe4e4e4" edTextCol="ff000000" edBkgCol="0" labelText="v1.0.2"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="1.5e1" kerning="0" bold="0"
         italic="0" justification="36"/>
  <HYPERLINKBUTTON name="linkButton" id="1f700d0405340eb4" memberName="linkButton"
                   virtualName="" explicitFocusOrder="0" pos="-76C 9C 150 26" tooltip="http://kushview.net"
                   textCol="cc3677ba" buttonText="kushview.net" connectedEdges="0"
                   needsCallback="0" radioGroupId="0" url="http://kushview.net"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

