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

#include "CustomLookAndFeel.h"
#include "DecibelScale.h"
#include "IRBrowserComponent.h"
#include "IRComponent.h"
#include "LevelMeter.h"
#include "../Processor.h"
#include "UIUtils.hpp"
#include "SliderGroupComponents.hpp"
#include "SaveLoadComponent.hpp"
#include "Logo.hpp"
#include "Title.hpp"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class KlangFalterEditor  : public AudioProcessorEditor,
                           public ChangeNotifier::Listener,
                           public ChangeListener,
                           public Timer,
                           public juce::Slider::Listener,
                           public juce::Button::Listener
{
public:
    //==============================================================================
    KlangFalterEditor (Processor& processor);
    ~KlangFalterEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void updateUI();
    virtual void changeListenerCallback (ChangeBroadcaster* source);
    virtual void changeNotification();
    virtual void timerCallback();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    SharedResourcePointer<CustomLookAndFeel> customLookAndFeel;
    std::unique_ptr<UIUtils::ToggleButtonLookAndFeel> _toggleButtonLookAndFeel;
    std::unique_ptr<UIUtils::SimpleButtonLookAndFeel> _simpleButtonLookAndFeel;
    std::unique_ptr<UIUtils::RotarySliderLookAndFeel> _rotarySliderLookAndFeel;
    std::unique_ptr<UIUtils::LinearSliderLookAndFeel> _linearSliderLookAndFeel;
    Processor& _processor;
    juce::Component::SafePointer<juce::DialogWindow> _settingsDialogWindow;
    std::map<std::pair<size_t, size_t>, IRComponent*> _irComponents;
    std::unique_ptr<IRSliderGroup> _irSliderGroup;
    std::unique_ptr<TuneSliderGroup> _tuneSliderGroup;
    std::unique_ptr<AttackSliderGroup> _attackSliderGroup;
    std::unique_ptr<DecaySliderGroup> _decaySliderGroup;
    std::unique_ptr<StereoSliderGroup> _stereoSliderGroup;
    std::unique_ptr<LowEqSliderGroup> _lowEqSliderGroup;
    std::unique_ptr<HighEqSliderGroup> _highEqSliderGroup;
    // std::unique_ptr<ShimmerSliderGroup> _shimmerSliderGroup;
    std::unique_ptr<ChorusSliderGroup> _chorusSliderGroup;
    std::unique_ptr<SaveLoadComponent> _saveLoadComponent;
    std::unique_ptr<Title> _title;
    std::unique_ptr<Logo> _tomLogo;
    std::unique_ptr<Logo> _weaLogo;
    std::unique_ptr<juce::ComponentBoundsConstrainer> _constrainer;
    UIUtils::Theme _theme;

    void _updateIRBrowserOpen(bool isOpen);
    int scaled(float value) const;
    float scaledFloat(float value) const;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<DecibelScale> _decibelScaleDry;
    std::unique_ptr<juce::TabbedComponent> _irTabComponent;
    std::unique_ptr<LevelMeter> _levelMeterDry;
    std::unique_ptr<juce::Label> _dryLevelLabel;
    std::unique_ptr<juce::Label> _wetLevelLabel;
    std::unique_ptr<juce::Slider> _drySlider;
    std::unique_ptr<DecibelScale> _decibelScaleOut;
    std::unique_ptr<juce::Slider> _wetSlider;
    std::unique_ptr<juce::TextButton> _browseButton;
    std::unique_ptr<IRBrowserComponent> _irBrowserComponent;
    std::unique_ptr<juce::TextButton> _wetButton;
    std::unique_ptr<juce::TextButton> _dryButton;
    std::unique_ptr<juce::TextButton> _autogainButton;
    std::unique_ptr<LevelMeter> _levelMeterOut;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KlangFalterEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

