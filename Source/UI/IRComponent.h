/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.12

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"

#include "WaveformComponent.h"
#include "../IRAgent.h"
#include "../Processor.h"
#include "UIUtils.hpp"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class IRComponent  : public Component,
                     public ChangeNotifier::Listener,
                     public juce::Button::Listener
{
public:
    //==============================================================================
    IRComponent(UIUtils::Theme theme, Processor& processor);
    ~IRComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void init(IRAgent* irAgent);
    void irChanged();
    virtual void changeNotification();

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    IRAgent* _irAgent;
    std::unique_ptr<UIUtils::SimpleButtonLookAndFeel> _simpleButtonLookAndFeel;
    std::unique_ptr<UIUtils::ToggleButtonLookAndFeel> _toggleButtonLookAndFeel;
    Processor& _processor;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<WaveformComponent> _waveformComponent;
    std::unique_ptr<juce::TextButton> _loadButton;
    std::unique_ptr<juce::TextButton> _reverseButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

