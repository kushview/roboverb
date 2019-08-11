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
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AboutBox  : public Component
{
public:
    //==============================================================================
    AboutBox ();
    ~AboutBox();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void mouseDown (const MouseEvent& ev) override;
    void setPluginName (const String& name);
    void setPluginVersion (const String& versionString);
    void setPluginUrl (const String& text, const String& url);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    String pluginUrl;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Label> pluginLabel;
    std::unique_ptr<Label> versionLabel;
    std::unique_ptr<HyperlinkButton> linkButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AboutBox)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

