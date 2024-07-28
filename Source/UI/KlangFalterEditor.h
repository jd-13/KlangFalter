/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.9

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
    std::unique_ptr<UIUtils::RotarySliderLookAndFeel> _rotarySliderLookAndFeel;
    Processor& _processor;
    juce::Component::SafePointer<juce::DialogWindow> _settingsDialogWindow;
    std::map<std::pair<size_t, size_t>, IRComponent*> _irComponents;
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
    std::unique_ptr<juce::TextButton> _reverseButton;
    std::unique_ptr<juce::Label> _hiFreqLabel;
    std::unique_ptr<juce::Label> _hiGainLabel;
    std::unique_ptr<juce::Label> _hiGainHeaderLabel;
    std::unique_ptr<juce::Label> _hiFreqHeaderLabel;
    std::unique_ptr<juce::Slider> _hiGainSlider;
    std::unique_ptr<juce::Slider> _hiFreqSlider;
    std::unique_ptr<juce::Label> _loFreqLabel;
    std::unique_ptr<juce::Label> _loGainLabel;
    std::unique_ptr<juce::Label> _loGainHeaderLabel;
    std::unique_ptr<juce::Label> _loFreqHeaderLabel;
    std::unique_ptr<juce::Slider> _loGainSlider;
    std::unique_ptr<juce::Slider> _loFreqSlider;
    std::unique_ptr<LevelMeter> _levelMeterOut;
    std::unique_ptr<juce::TextButton> _levelMeterOutLabelButton;
    std::unique_ptr<juce::Label> _levelMeterDryLabel;
    std::unique_ptr<juce::TextButton> _lowEqButton;
    std::unique_ptr<juce::Label> _lowCutFreqLabel;
    std::unique_ptr<juce::Label> _lowCutFreqHeaderLabel;
    std::unique_ptr<juce::Slider> _lowCutFreqSlider;
    std::unique_ptr<juce::Label> _highCutFreqLabel;
    std::unique_ptr<juce::Label> _highCutFreqHeaderLabel;
    std::unique_ptr<juce::Slider> _highCutFreqSlider;
    std::unique_ptr<juce::TextButton> _highEqButton;
    std::unique_ptr<juce::Label> _attackShapeLabel;
    std::unique_ptr<juce::Label> _endLabel;
    std::unique_ptr<juce::Slider> _endSlider;
    std::unique_ptr<juce::Slider> _attackShapeSlider;
    std::unique_ptr<juce::Label> _decayShapeLabel;
    std::unique_ptr<juce::Label> _decayShapeHeaderLabel;
    std::unique_ptr<juce::Slider> _decayShapeSlider;
    std::unique_ptr<juce::Label> _attackShapeHeaderLabel;
    std::unique_ptr<juce::Label> _endHeaderLabel;
    std::unique_ptr<juce::Label> _beginLabel;
    std::unique_ptr<juce::Slider> _beginSlider;
    std::unique_ptr<juce::Label> _beginHeaderLabel;
    std::unique_ptr<juce::Label> _widthLabel;
    std::unique_ptr<juce::Label> _widthHeaderLabel;
    std::unique_ptr<juce::Slider> _widthSlider;
    std::unique_ptr<juce::Label> _predelayLabel;
    std::unique_ptr<juce::Label> _predelayHeaderLabel;
    std::unique_ptr<juce::Slider> _predelaySlider;
    std::unique_ptr<juce::Label> _stretchLabel;
    std::unique_ptr<juce::Label> _stretchHeaderLabel;
    std::unique_ptr<juce::Slider> _stretchSlider;
    std::unique_ptr<juce::Label> _attackHeaderLabel;
    std::unique_ptr<juce::Label> _attackLengthLabel;
    std::unique_ptr<juce::Slider> _attackLengthSlider;
    std::unique_ptr<juce::Label> _attackLengthHeaderLabel;
    std::unique_ptr<juce::Label> _decayHeaderLabel;
    std::unique_ptr<juce::Label> _impulseResponseHeaderLabel;
    std::unique_ptr<juce::Label> _stereoHeaderLabel;
    std::unique_ptr<juce::Label> _titleLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KlangFalterEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

