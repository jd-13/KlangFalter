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

//[Headers] You can add your own extra header files here...

#include "../DecibelScaling.h"
#include "../Parameters.h"

//[/Headers]

#include "KlangFalterEditor.h"
#include "IRComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...


template<typename T>
T SnapValue(T val, T snapValue, T sensitivity)
{
  return (::fabs(val - snapValue) < sensitivity) ? snapValue : val;
}


static juce::String FormatFrequency(float freq)
{
  if (freq < 1000.0f)
  {
    return juce::String(static_cast<int>(freq+0.5f)) + juce::String("Hz");
  }
  return juce::String(freq/1000.0f, (freq < 1500.0f) ? 2 : 1) + juce::String("kHz");
}


static juce::String FormatSeconds(double seconds)
{
  if (seconds < 1.0)
  {
    return juce::String(static_cast<int>(seconds * 1000.0)) + juce::String("ms");
  }
  return juce::String(seconds, 2) + juce::String("s");
}


//[/MiscUserDefs]

//==============================================================================
KlangFalterEditor::KlangFalterEditor (Processor& processor)
    : AudioProcessorEditor(&processor),
      _processor(processor),
      _toggleButtonLookAndFeel(new UIUtils::ToggleButtonLookAndFeel()),
      _rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()),
      _linearSliderLookAndFeel(new UIUtils::LinearSliderLookAndFeel())
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    _decibelScaleDry.reset (new DecibelScale());
    addAndMakeVisible (_decibelScaleDry.get());

    _decibelScaleDry->setBounds (584, 51, 32, 176);

    _irTabComponent.reset (new juce::TabbedComponent (juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (_irTabComponent.get());
    _irTabComponent->setTabBarDepth (0);
    _irTabComponent->addTab (TRANS ("Placeholder"), juce::Colour (0xffb0b0b6), new IRComponent(), true);
    _irTabComponent->setCurrentTabIndex (0);

    _irTabComponent->setBounds (16, 51, 462, 176);

    _levelMeterDry.reset (new LevelMeter());
    addAndMakeVisible (_levelMeterDry.get());

    _levelMeterDry->setBounds (632, 51, 12, 176);

    _dryLevelLabel.reset (new juce::Label ("DryLevelLabel",
                                           TRANS ("-inf")));
    addAndMakeVisible (_dryLevelLabel.get());
    _dryLevelLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _dryLevelLabel->setJustificationType (juce::Justification::centredRight);
    _dryLevelLabel->setEditable (false, false, false);
    _dryLevelLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _dryLevelLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _dryLevelLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _dryLevelLabel->setBounds (572, 231, 60, 24);

    _wetLevelLabel.reset (new juce::Label ("WetLevelLabel",
                                           TRANS ("-inf")));
    addAndMakeVisible (_wetLevelLabel.get());
    _wetLevelLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _wetLevelLabel->setJustificationType (juce::Justification::centredRight);
    _wetLevelLabel->setEditable (false, false, false);
    _wetLevelLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _wetLevelLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _wetLevelLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _wetLevelLabel->setBounds (656, 231, 64, 24);

    _drySlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_drySlider.get());
    _drySlider->setRange (0, 10, 0);
    _drySlider->setSliderStyle (juce::Slider::LinearVertical);
    _drySlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _drySlider->addListener (this);

    _drySlider->setBounds (612, 43, 24, 192);

    _decibelScaleOut.reset (new DecibelScale());
    addAndMakeVisible (_decibelScaleOut.get());

    _decibelScaleOut->setBounds (672, 51, 32, 176);

    _wetSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_wetSlider.get());
    _wetSlider->setRange (0, 10, 0);
    _wetSlider->setSliderStyle (juce::Slider::LinearVertical);
    _wetSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _wetSlider->addListener (this);

    _wetSlider->setBounds (700, 43, 24, 192);

    _browseButton.reset (new juce::TextButton (juce::String()));
    addAndMakeVisible (_browseButton.get());
    _browseButton->setTooltip (TRANS ("Show Browser For Impulse Response Selection"));
    _browseButton->setButtonText (TRANS ("Show Browser"));
    _browseButton->setConnectedEdges (juce::Button::ConnectedOnBottom);
    _browseButton->addListener (this);
    _browseButton->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffbcbcff));

    _browseButton->setBounds (12, 319, 736, 24);

    _irBrowserComponent.reset (new IRBrowserComponent());
    addAndMakeVisible (_irBrowserComponent.get());

    _irBrowserComponent->setBounds (12, 343, 736, 288);

    _wetButton.reset (new juce::TextButton (juce::String()));
    addAndMakeVisible (_wetButton.get());
    _wetButton->setTooltip (TRANS ("Wet Signal On/Off"));
    _wetButton->setButtonText (TRANS ("Wet"));
    _wetButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    _wetButton->addListener (this);
    _wetButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0x80bcbcbc));
    _wetButton->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffbcbcff));
    _wetButton->setColour (juce::TextButton::textColourOffId, juce::Colour (0xff202020));
    _wetButton->setColour (juce::TextButton::textColourOnId, juce::Colour (0xff202020));

    _wetButton->setBounds (684, 255, 44, 24);

    _dryButton.reset (new juce::TextButton (juce::String()));
    addAndMakeVisible (_dryButton.get());
    _dryButton->setTooltip (TRANS ("Dry Signal On/Off"));
    _dryButton->setButtonText (TRANS ("Dry"));
    _dryButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    _dryButton->addListener (this);
    _dryButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0x80bcbcbc));
    _dryButton->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffbcbcff));
    _dryButton->setColour (juce::TextButton::textColourOffId, juce::Colour (0xff202020));
    _dryButton->setColour (juce::TextButton::textColourOnId, juce::Colour (0xff202020));

    _dryButton->setBounds (596, 255, 44, 24);

    _autogainButton.reset (new juce::TextButton (juce::String()));
    addAndMakeVisible (_autogainButton.get());
    _autogainButton->setTooltip (TRANS ("Autogain On/Off"));
    _autogainButton->setButtonText (TRANS ("Autogain 0.0dB"));
    _autogainButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    _autogainButton->addListener (this);
    _autogainButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0x80bcbcbc));
    _autogainButton->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffbcbcff));
    _autogainButton->setColour (juce::TextButton::textColourOffId, juce::Colour (0xff202020));
    _autogainButton->setColour (juce::TextButton::textColourOnId, juce::Colour (0xff202020));

    _autogainButton->setBounds (596, 287, 132, 24);

    _reverseButton.reset (new juce::TextButton (juce::String()));
    addAndMakeVisible (_reverseButton.get());
    _reverseButton->setTooltip (TRANS ("Reverse Impulse Response"));
    _reverseButton->setButtonText (TRANS ("Reverse"));
    _reverseButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    _reverseButton->addListener (this);
    _reverseButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0x80bcbcbc));
    _reverseButton->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffbcbcff));
    _reverseButton->setColour (juce::TextButton::textColourOffId, juce::Colour (0xff202020));
    _reverseButton->setColour (juce::TextButton::textColourOnId, juce::Colour (0xff202020));

    _reverseButton->setBounds (20, 198, 72, 24);

    _hiFreqLabel.reset (new juce::Label (juce::String(),
                                         TRANS ("15.2kHz")));
    addAndMakeVisible (_hiFreqLabel.get());
    _hiFreqLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _hiFreqLabel->setJustificationType (juce::Justification::centred);
    _hiFreqLabel->setEditable (false, false, false);
    _hiFreqLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _hiFreqLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _hiFreqLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _hiFreqLabel->setBounds (480, 291, 52, 24);

    _hiGainLabel.reset (new juce::Label (juce::String(),
                                         TRANS ("0.0dB")));
    addAndMakeVisible (_hiGainLabel.get());
    _hiGainLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _hiGainLabel->setJustificationType (juce::Justification::centred);
    _hiGainLabel->setEditable (false, false, false);
    _hiGainLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _hiGainLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _hiGainLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _hiGainLabel->setBounds (516, 291, 52, 24);

    _hiGainHeaderLabel.reset (new juce::Label (juce::String(),
                                               TRANS ("Gain")));
    addAndMakeVisible (_hiGainHeaderLabel.get());
    _hiGainHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _hiGainHeaderLabel->setJustificationType (juce::Justification::centred);
    _hiGainHeaderLabel->setEditable (false, false, false);
    _hiGainHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _hiGainHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _hiGainHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _hiGainHeaderLabel->setBounds (516, 247, 52, 24);

    _hiFreqHeaderLabel.reset (new juce::Label (juce::String(),
                                               TRANS ("Freq")));
    addAndMakeVisible (_hiFreqHeaderLabel.get());
    _hiFreqHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _hiFreqHeaderLabel->setJustificationType (juce::Justification::centred);
    _hiFreqHeaderLabel->setEditable (false, false, false);
    _hiFreqHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _hiFreqHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _hiFreqHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _hiFreqHeaderLabel->setBounds (480, 247, 52, 24);

    _hiGainSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_hiGainSlider.get());
    _hiGainSlider->setRange (-30, 30, 0);
    _hiGainSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _hiGainSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _hiGainSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _hiGainSlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _hiGainSlider->addListener (this);

    _hiGainSlider->setBounds (524, 267, 36, 28);

    _hiFreqSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_hiFreqSlider.get());
    _hiFreqSlider->setRange (2000, 20000, 0);
    _hiFreqSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _hiFreqSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _hiFreqSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _hiFreqSlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _hiFreqSlider->addListener (this);

    _hiFreqSlider->setBounds (488, 267, 36, 28);

    _loFreqLabel.reset (new juce::Label (juce::String(),
                                         TRANS ("1234Hz")));
    addAndMakeVisible (_loFreqLabel.get());
    _loFreqLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _loFreqLabel->setJustificationType (juce::Justification::centred);
    _loFreqLabel->setEditable (false, false, false);
    _loFreqLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _loFreqLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _loFreqLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _loFreqLabel->setBounds (396, 291, 52, 24);

    _loGainLabel.reset (new juce::Label (juce::String(),
                                         TRANS ("0.0dB")));
    addAndMakeVisible (_loGainLabel.get());
    _loGainLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _loGainLabel->setJustificationType (juce::Justification::centred);
    _loGainLabel->setEditable (false, false, false);
    _loGainLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _loGainLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _loGainLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _loGainLabel->setBounds (432, 291, 52, 24);

    _loGainHeaderLabel.reset (new juce::Label (juce::String(),
                                               TRANS ("Gain")));
    addAndMakeVisible (_loGainHeaderLabel.get());
    _loGainHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _loGainHeaderLabel->setJustificationType (juce::Justification::centred);
    _loGainHeaderLabel->setEditable (false, false, false);
    _loGainHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _loGainHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _loGainHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _loGainHeaderLabel->setBounds (432, 247, 52, 24);

    _loFreqHeaderLabel.reset (new juce::Label (juce::String(),
                                               TRANS ("Freq")));
    addAndMakeVisible (_loFreqHeaderLabel.get());
    _loFreqHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _loFreqHeaderLabel->setJustificationType (juce::Justification::centred);
    _loFreqHeaderLabel->setEditable (false, false, false);
    _loFreqHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _loFreqHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _loFreqHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _loFreqHeaderLabel->setBounds (396, 247, 52, 24);

    _loGainSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_loGainSlider.get());
    _loGainSlider->setRange (-30, 30, 0);
    _loGainSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _loGainSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _loGainSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _loGainSlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _loGainSlider->addListener (this);

    _loGainSlider->setBounds (440, 267, 36, 28);

    _loFreqSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_loFreqSlider.get());
    _loFreqSlider->setRange (20, 2000, 0);
    _loFreqSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _loFreqSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _loFreqSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _loFreqSlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _loFreqSlider->addListener (this);

    _loFreqSlider->setBounds (404, 267, 36, 28);

    _levelMeterOut.reset (new LevelMeter());
    addAndMakeVisible (_levelMeterOut.get());

    _levelMeterOut->setBounds (720, 51, 12, 176);

    _levelMeterOutLabelButton.reset (new juce::TextButton (juce::String()));
    addAndMakeVisible (_levelMeterOutLabelButton.get());
    _levelMeterOutLabelButton->setTooltip (TRANS ("Switches Between Out/Wet Level Measurement"));
    _levelMeterOutLabelButton->setButtonText (TRANS ("Out"));
    _levelMeterOutLabelButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    _levelMeterOutLabelButton->addListener (this);
    _levelMeterOutLabelButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0x00bbbbff));
    _levelMeterOutLabelButton->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0x00bcbcff));
    _levelMeterOutLabelButton->setColour (juce::TextButton::textColourOffId, juce::Colour (0xffb0b0b6));
    _levelMeterOutLabelButton->setColour (juce::TextButton::textColourOnId, juce::Colour (0xffb0b0b6));

    _levelMeterOutLabelButton->setBounds (712, 31, 28, 18);

    _levelMeterDryLabel.reset (new juce::Label (juce::String(),
                                                TRANS ("Dry")));
    addAndMakeVisible (_levelMeterDryLabel.get());
    _levelMeterDryLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _levelMeterDryLabel->setJustificationType (juce::Justification::centred);
    _levelMeterDryLabel->setEditable (false, false, false);
    _levelMeterDryLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _levelMeterDryLabel->setColour (juce::TextEditor::textColourId, juce::Colour (0xffb0b0b6));
    _levelMeterDryLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _levelMeterDryLabel->setBounds (620, 27, 36, 24);

    _lowEqButton.reset (new juce::TextButton (juce::String()));
    addAndMakeVisible (_lowEqButton.get());
    _lowEqButton->setButtonText (TRANS ("Low Cut"));
    _lowEqButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    _lowEqButton->addListener (this);
    _lowEqButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0x00bbbbff));
    _lowEqButton->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0x002c2cff));
    _lowEqButton->setColour (juce::TextButton::textColourOffId, juce::Colour (0xffb0b0b6));
    _lowEqButton->setColour (juce::TextButton::textColourOnId, juce::Colour (0xffb0b0b6));

    _lowEqButton->setBounds (404, 231, 72, 24);

    _lowCutFreqLabel.reset (new juce::Label (juce::String(),
                                             TRANS ("1234Hz")));
    addAndMakeVisible (_lowCutFreqLabel.get());
    _lowCutFreqLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _lowCutFreqLabel->setJustificationType (juce::Justification::centred);
    _lowCutFreqLabel->setEditable (false, false, false);
    _lowCutFreqLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _lowCutFreqLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _lowCutFreqLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _lowCutFreqLabel->setBounds (414, 291, 52, 24);

    _lowCutFreqHeaderLabel.reset (new juce::Label (juce::String(),
                                                   TRANS ("Freq")));
    addAndMakeVisible (_lowCutFreqHeaderLabel.get());
    _lowCutFreqHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _lowCutFreqHeaderLabel->setJustificationType (juce::Justification::centred);
    _lowCutFreqHeaderLabel->setEditable (false, false, false);
    _lowCutFreqHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _lowCutFreqHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _lowCutFreqHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _lowCutFreqHeaderLabel->setBounds (414, 247, 52, 24);

    _lowCutFreqSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_lowCutFreqSlider.get());
    _lowCutFreqSlider->setRange (20, 2000, 0);
    _lowCutFreqSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _lowCutFreqSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _lowCutFreqSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _lowCutFreqSlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _lowCutFreqSlider->addListener (this);

    _lowCutFreqSlider->setBounds (422, 267, 36, 28);

    _highCutFreqLabel.reset (new juce::Label (juce::String(),
                                              TRANS ("15.2kHz")));
    addAndMakeVisible (_highCutFreqLabel.get());
    _highCutFreqLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _highCutFreqLabel->setJustificationType (juce::Justification::centred);
    _highCutFreqLabel->setEditable (false, false, false);
    _highCutFreqLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _highCutFreqLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _highCutFreqLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _highCutFreqLabel->setBounds (498, 291, 52, 24);

    _highCutFreqHeaderLabel.reset (new juce::Label (juce::String(),
                                                    TRANS ("Freq")));
    addAndMakeVisible (_highCutFreqHeaderLabel.get());
    _highCutFreqHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _highCutFreqHeaderLabel->setJustificationType (juce::Justification::centred);
    _highCutFreqHeaderLabel->setEditable (false, false, false);
    _highCutFreqHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _highCutFreqHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _highCutFreqHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _highCutFreqHeaderLabel->setBounds (498, 247, 52, 24);

    _highCutFreqSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_highCutFreqSlider.get());
    _highCutFreqSlider->setRange (2000, 20000, 0);
    _highCutFreqSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _highCutFreqSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _highCutFreqSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _highCutFreqSlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _highCutFreqSlider->addListener (this);

    _highCutFreqSlider->setBounds (506, 267, 36, 28);

    _highEqButton.reset (new juce::TextButton (juce::String()));
    addAndMakeVisible (_highEqButton.get());
    _highEqButton->setButtonText (TRANS ("High Cut"));
    _highEqButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    _highEqButton->addListener (this);
    _highEqButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0x00bbbbff));
    _highEqButton->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0x002c2cff));
    _highEqButton->setColour (juce::TextButton::textColourOffId, juce::Colour (0xffb0b0b6));
    _highEqButton->setColour (juce::TextButton::textColourOnId, juce::Colour (0xffb0b0b6));

    _highEqButton->setBounds (488, 231, 72, 24);

    _attackShapeLabel.reset (new juce::Label (juce::String(),
                                              TRANS ("1.0")));
    addAndMakeVisible (_attackShapeLabel.get());
    _attackShapeLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _attackShapeLabel->setJustificationType (juce::Justification::centred);
    _attackShapeLabel->setEditable (false, false, false);
    _attackShapeLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _attackShapeLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _attackShapeLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _attackShapeLabel->setBounds (212, 291, 52, 24);

    _endLabel.reset (new juce::Label (juce::String(),
                                      TRANS ("100%")));
    addAndMakeVisible (_endLabel.get());
    _endLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _endLabel->setJustificationType (juce::Justification::centred);
    _endLabel->setEditable (false, false, false);
    _endLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _endLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _endLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _endLabel->setBounds (76, 291, 52, 24);

    _endSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_endSlider.get());
    _endSlider->setRange (0, 1, 0.001);
    _endSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _endSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _endSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _endSlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _endSlider->addListener (this);

    _endSlider->setBounds (84, 267, 36, 28);

    _attackShapeSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_attackShapeSlider.get());
    _attackShapeSlider->setRange (0, 10, 0);
    _attackShapeSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _attackShapeSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _attackShapeSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _attackShapeSlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _attackShapeSlider->addListener (this);
    _attackShapeSlider->setSkewFactor (0.5);

    _attackShapeSlider->setBounds (220, 267, 36, 28);

    _decayShapeLabel.reset (new juce::Label (juce::String(),
                                             TRANS ("1.0")));
    addAndMakeVisible (_decayShapeLabel.get());
    _decayShapeLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _decayShapeLabel->setJustificationType (juce::Justification::centred);
    _decayShapeLabel->setEditable (false, false, false);
    _decayShapeLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _decayShapeLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _decayShapeLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _decayShapeLabel->setBounds (276, 291, 52, 24);

    _decayShapeHeaderLabel.reset (new juce::Label (juce::String(),
                                                   TRANS ("Shape")));
    addAndMakeVisible (_decayShapeHeaderLabel.get());
    _decayShapeHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _decayShapeHeaderLabel->setJustificationType (juce::Justification::centred);
    _decayShapeHeaderLabel->setEditable (false, false, false);
    _decayShapeHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _decayShapeHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _decayShapeHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _decayShapeHeaderLabel->setBounds (276, 247, 52, 24);

    _decayShapeSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_decayShapeSlider.get());
    _decayShapeSlider->setRange (0, 10, 0);
    _decayShapeSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _decayShapeSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _decayShapeSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _decayShapeSlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _decayShapeSlider->addListener (this);
    _decayShapeSlider->setSkewFactor (0.5);

    _decayShapeSlider->setBounds (284, 267, 36, 28);

    _attackShapeHeaderLabel.reset (new juce::Label (juce::String(),
                                                    TRANS ("Shape")));
    addAndMakeVisible (_attackShapeHeaderLabel.get());
    _attackShapeHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _attackShapeHeaderLabel->setJustificationType (juce::Justification::centred);
    _attackShapeHeaderLabel->setEditable (false, false, false);
    _attackShapeHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _attackShapeHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _attackShapeHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _attackShapeHeaderLabel->setBounds (212, 247, 52, 24);

    _endHeaderLabel.reset (new juce::Label (juce::String(),
                                            TRANS ("End")));
    addAndMakeVisible (_endHeaderLabel.get());
    _endHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _endHeaderLabel->setJustificationType (juce::Justification::centred);
    _endHeaderLabel->setEditable (false, false, false);
    _endHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _endHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _endHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _endHeaderLabel->setBounds (76, 247, 52, 24);

    _beginLabel.reset (new juce::Label (juce::String(),
                                        TRANS ("100%")));
    addAndMakeVisible (_beginLabel.get());
    _beginLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _beginLabel->setJustificationType (juce::Justification::centred);
    _beginLabel->setEditable (false, false, false);
    _beginLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _beginLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _beginLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _beginLabel->setBounds (40, 291, 52, 24);

    _beginSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_beginSlider.get());
    _beginSlider->setRange (0, 1, 0.001);
    _beginSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _beginSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _beginSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _beginSlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _beginSlider->addListener (this);
    _beginSlider->setSkewFactor (0.5);

    _beginSlider->setBounds (48, 267, 36, 28);

    _beginHeaderLabel.reset (new juce::Label (juce::String(),
                                              TRANS ("Begin")));
    addAndMakeVisible (_beginHeaderLabel.get());
    _beginHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _beginHeaderLabel->setJustificationType (juce::Justification::centred);
    _beginHeaderLabel->setEditable (false, false, false);
    _beginHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _beginHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _beginHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _beginHeaderLabel->setBounds (40, 247, 52, 24);

    _widthLabel.reset (new juce::Label (juce::String(),
                                        TRANS ("1.0")));
    addAndMakeVisible (_widthLabel.get());
    _widthLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _widthLabel->setJustificationType (juce::Justification::centred);
    _widthLabel->setEditable (false, false, false);
    _widthLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _widthLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _widthLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _widthLabel->setBounds (340, 291, 52, 24);

    _widthHeaderLabel.reset (new juce::Label (juce::String(),
                                              TRANS ("Width")));
    addAndMakeVisible (_widthHeaderLabel.get());
    _widthHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _widthHeaderLabel->setJustificationType (juce::Justification::centred);
    _widthHeaderLabel->setEditable (false, false, false);
    _widthHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _widthHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _widthHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _widthHeaderLabel->setBounds (340, 247, 52, 24);

    _widthSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_widthSlider.get());
    _widthSlider->setRange (0, 10, 0);
    _widthSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _widthSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _widthSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _widthSlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _widthSlider->addListener (this);
    _widthSlider->setSkewFactor (0.30102);

    _widthSlider->setBounds (348, 267, 36, 28);

    _predelayLabel.reset (new juce::Label (juce::String(),
                                           TRANS ("0ms")));
    addAndMakeVisible (_predelayLabel.get());
    _predelayLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _predelayLabel->setJustificationType (juce::Justification::centred);
    _predelayLabel->setEditable (false, false, false);
    _predelayLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _predelayLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _predelayLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _predelayLabel->setBounds (4, 291, 52, 24);

    _predelayHeaderLabel.reset (new juce::Label (juce::String(),
                                                 TRANS ("Gap")));
    addAndMakeVisible (_predelayHeaderLabel.get());
    _predelayHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _predelayHeaderLabel->setJustificationType (juce::Justification::centred);
    _predelayHeaderLabel->setEditable (false, false, false);
    _predelayHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _predelayHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _predelayHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _predelayHeaderLabel->setBounds (4, 247, 52, 24);

    _predelaySlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_predelaySlider.get());
    _predelaySlider->setRange (0, 1000, 0);
    _predelaySlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _predelaySlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _predelaySlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _predelaySlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _predelaySlider->addListener (this);

    _predelaySlider->setBounds (12, 267, 36, 28);

    _stretchLabel.reset (new juce::Label (juce::String(),
                                          TRANS ("100%")));
    addAndMakeVisible (_stretchLabel.get());
    _stretchLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _stretchLabel->setJustificationType (juce::Justification::centred);
    _stretchLabel->setEditable (false, false, false);
    _stretchLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _stretchLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _stretchLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _stretchLabel->setBounds (112, 291, 52, 24);

    _stretchHeaderLabel.reset (new juce::Label (juce::String(),
                                                TRANS ("Stretch")));
    addAndMakeVisible (_stretchHeaderLabel.get());
    _stretchHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _stretchHeaderLabel->setJustificationType (juce::Justification::centred);
    _stretchHeaderLabel->setEditable (false, false, false);
    _stretchHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _stretchHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _stretchHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _stretchHeaderLabel->setBounds (112, 247, 52, 24);

    _stretchSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_stretchSlider.get());
    _stretchSlider->setRange (0, 2, 0);
    _stretchSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _stretchSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _stretchSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _stretchSlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _stretchSlider->addListener (this);

    _stretchSlider->setBounds (120, 267, 36, 28);

    _attackHeaderLabel.reset (new juce::Label (juce::String(),
                                               TRANS ("Attack")));
    addAndMakeVisible (_attackHeaderLabel.get());
    _attackHeaderLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _attackHeaderLabel->setJustificationType (juce::Justification::centred);
    _attackHeaderLabel->setEditable (false, false, false);
    _attackHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _attackHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colour (0xffb0b0b6));
    _attackHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _attackHeaderLabel->setBounds (176, 231, 88, 24);

    _attackLengthLabel.reset (new juce::Label (juce::String(),
                                               TRANS ("0ms")));
    addAndMakeVisible (_attackLengthLabel.get());
    _attackLengthLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _attackLengthLabel->setJustificationType (juce::Justification::centred);
    _attackLengthLabel->setEditable (false, false, false);
    _attackLengthLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _attackLengthLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _attackLengthLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _attackLengthLabel->setBounds (176, 291, 52, 24);

    _attackLengthSlider.reset (new juce::Slider (juce::String()));
    addAndMakeVisible (_attackLengthSlider.get());
    _attackLengthSlider->setRange (0, 1, 0);
    _attackLengthSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    _attackLengthSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    _attackLengthSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0xffafafff));
    _attackLengthSlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xb1606060));
    _attackLengthSlider->addListener (this);
    _attackLengthSlider->setSkewFactor (0.5);

    _attackLengthSlider->setBounds (184, 267, 36, 28);

    _attackLengthHeaderLabel.reset (new juce::Label (juce::String(),
                                                     TRANS ("Length")));
    addAndMakeVisible (_attackLengthHeaderLabel.get());
    _attackLengthHeaderLabel->setFont (juce::Font (11.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _attackLengthHeaderLabel->setJustificationType (juce::Justification::centred);
    _attackLengthHeaderLabel->setEditable (false, false, false);
    _attackLengthHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _attackLengthHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    _attackLengthHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _attackLengthHeaderLabel->setBounds (176, 247, 52, 24);

    _decayHeaderLabel.reset (new juce::Label (juce::String(),
                                              TRANS ("Decay")));
    addAndMakeVisible (_decayHeaderLabel.get());
    _decayHeaderLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _decayHeaderLabel->setJustificationType (juce::Justification::centred);
    _decayHeaderLabel->setEditable (false, false, false);
    _decayHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _decayHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colour (0xffb0b0b6));
    _decayHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _decayHeaderLabel->setBounds (256, 231, 88, 24);

    _impulseResponseHeaderLabel.reset (new juce::Label (juce::String(),
                                                        TRANS ("Impulse Response")));
    addAndMakeVisible (_impulseResponseHeaderLabel.get());
    _impulseResponseHeaderLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _impulseResponseHeaderLabel->setJustificationType (juce::Justification::centred);
    _impulseResponseHeaderLabel->setEditable (false, false, false);
    _impulseResponseHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _impulseResponseHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colour (0xffb0b0b6));
    _impulseResponseHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _impulseResponseHeaderLabel->setBounds (12, 231, 144, 24);

    _stereoHeaderLabel.reset (new juce::Label (juce::String(),
                                               TRANS ("Stereo")));
    addAndMakeVisible (_stereoHeaderLabel.get());
    _stereoHeaderLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _stereoHeaderLabel->setJustificationType (juce::Justification::centred);
    _stereoHeaderLabel->setEditable (false, false, false);
    _stereoHeaderLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _stereoHeaderLabel->setColour (juce::TextEditor::textColourId, juce::Colour (0xffb0b0b6));
    _stereoHeaderLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _stereoHeaderLabel->setBounds (340, 231, 52, 24);

    _titleLabel.reset (new juce::Label ("Title Label",
                                        TRANS ("Body & Soul:")));
    addAndMakeVisible (_titleLabel.get());
    _titleLabel->setFont (juce::Font (35.30f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _titleLabel->setJustificationType (juce::Justification::centredLeft);
    _titleLabel->setEditable (false, false, false);
    _titleLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _titleLabel->setColour (juce::TextEditor::textColourId, juce::Colour (0xffb0b0b6));
    _titleLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _titleLabel->setBounds (248, 4, 195, 40);

    _subtitleLabel.reset (new juce::Label ("Subtitle Label",
                                           TRANS ("Intro")));
    addAndMakeVisible (_subtitleLabel.get());
    _subtitleLabel->setFont (juce::Font (35.30f, juce::Font::plain).withTypefaceStyle ("Regular"));
    _subtitleLabel->setJustificationType (juce::Justification::centredLeft);
    _subtitleLabel->setEditable (false, false, false);
    _subtitleLabel->setColour (juce::Label::textColourId, juce::Colour (0xffb0b0b6));
    _subtitleLabel->setColour (juce::TextEditor::textColourId, juce::Colour (0xffb0b0b6));
    _subtitleLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    _subtitleLabel->setBounds (432, 4, 79, 40);


    //[UserPreSize]
    setLookAndFeel(customLookAndFeel);

    _titleLabel->setColour(juce::Label::textColourId, UIUtils::neutralColour.withAlpha(0.5f));
    _subtitleLabel->setColour(juce::Label::textColourId, UIUtils::highlightColour);

    auto setButtonColours = [](juce::TextButton* button) {
        button->setColour(UIUtils::ToggleButtonLookAndFeel::offColour, UIUtils::neutralColour);
        button->setColour(UIUtils::ToggleButtonLookAndFeel::onColour, UIUtils::highlightColour);
    };

    _dryButton->setLookAndFeel(_toggleButtonLookAndFeel.get());
    _wetButton->setLookAndFeel(_toggleButtonLookAndFeel.get());
    _autogainButton->setLookAndFeel(_toggleButtonLookAndFeel.get());
    _reverseButton->setLookAndFeel(_toggleButtonLookAndFeel.get());
    _browseButton->setLookAndFeel(_toggleButtonLookAndFeel.get());

    setButtonColours(_dryButton.get());
    setButtonColours(_wetButton.get());
    setButtonColours(_autogainButton.get());
    setButtonColours(_reverseButton.get());

    _reverseButton->setColour(UIUtils::ToggleButtonLookAndFeel::offColour, UIUtils::backgroundColour);
    _browseButton->setColour(UIUtils::ToggleButtonLookAndFeel::offColour, UIUtils::neutralColour);
    _browseButton->setColour(UIUtils::ToggleButtonLookAndFeel::onColour, UIUtils::neutralColour);

    _browseButton->setConnectedEdges(juce::Button::ConnectedOnBottom);

    _predelaySlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _beginSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _endSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _stretchSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _attackLengthSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _attackShapeSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _decayShapeSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _widthSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _lowCutFreqSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _highCutFreqSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _loFreqSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _loGainSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _hiFreqSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _hiGainSlider->setLookAndFeel(_rotarySliderLookAndFeel.get());
    _drySlider->setLookAndFeel(_linearSliderLookAndFeel.get());
    _wetSlider->setLookAndFeel(_linearSliderLookAndFeel.get());

    auto setLinearSliderColours = [](juce::Slider* slider) {
        slider->setColour(juce::Slider::thumbColourId, UIUtils::highlightColour);
        slider->setColour(juce::Slider::trackColourId, UIUtils::neutralColour);
    };

    setLinearSliderColours(_drySlider.get());
    setLinearSliderColours(_wetSlider.get());

    //[/UserPreSize]

    setSize (760, 340);


    //[Constructor] You can add your own custom stuff here..
    _irTabComponent->clearTabs(); // Remove placeholder only used as dummy in the Jucer
    _browseButton->setClickingTogglesState(true);
    _dryButton->setClickingTogglesState(true);
    _wetButton->setClickingTogglesState(true);
    _autogainButton->setClickingTogglesState(true);
    _reverseButton->setClickingTogglesState(true);
    _levelMeterOutLabelButton->setClickingTogglesState(true);

    _lowCutFreqSlider->setRange(Parameters::EqLowCutFreq.getMinValue(), Parameters::EqLowCutFreq.getMaxValue());
    _loFreqSlider->setRange(Parameters::EqLowShelfFreq.getMinValue(), Parameters::EqLowShelfFreq.getMaxValue());
    _loGainSlider->setRange(Parameters::EqLowShelfDecibels.getMinValue(), Parameters::EqLowShelfDecibels.getMaxValue());

    _highCutFreqSlider->setRange(Parameters::EqHighCutFreq.getMinValue(), Parameters::EqHighCutFreq.getMaxValue());
    _hiFreqSlider->setRange(Parameters::EqHighShelfFreq.getMinValue(), Parameters::EqHighShelfFreq.getMaxValue());
    _hiGainSlider->setRange(Parameters::EqHighShelfDecibels.getMinValue(), Parameters::EqHighShelfDecibels.getMaxValue());

    // Double click to default
    _predelaySlider->setDoubleClickReturnValue(true, 0);
    _beginSlider->setDoubleClickReturnValue(true, 0);
    _endSlider->setDoubleClickReturnValue(true, 1);
    _stretchSlider->setDoubleClickReturnValue(true, 1);
    _attackLengthSlider->setDoubleClickReturnValue(true, 0);
    _attackShapeSlider->setDoubleClickReturnValue(true, 0);
    _decayShapeSlider->setDoubleClickReturnValue(true, 0);
    _widthSlider->setDoubleClickReturnValue(true, Parameters::StereoWidth.getDefaultValue());
    _lowCutFreqSlider->setDoubleClickReturnValue(true, Parameters::EqLowCutFreq.getDefaultValue());
    _highCutFreqSlider->setDoubleClickReturnValue(true, Parameters::EqHighCutFreq.getDefaultValue());
    _drySlider->setDoubleClickReturnValue(true, DecibelScaling::Db2Scale(Parameters::DryDecibels.getDefaultValue()));
    _wetSlider->setDoubleClickReturnValue(true, DecibelScaling::Db2Scale(Parameters::WetDecibels.getDefaultValue()));

    _processor.addNotificationListener(this);
    _processor.getSettings().addChangeListener(this);
    _irBrowserComponent->init(&_processor);
    updateUI();
    startTimer(100);
    //[/Constructor]
}

KlangFalterEditor::~KlangFalterEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    _settingsDialogWindow.deleteAndZero();
    _processor.removeNotificationListener(this);
    _processor.getSettings().removeChangeListener(this);
    //[/Destructor_pre]

    _decibelScaleDry = nullptr;
    _irTabComponent = nullptr;
    _levelMeterDry = nullptr;
    _dryLevelLabel = nullptr;
    _wetLevelLabel = nullptr;
    _drySlider = nullptr;
    _decibelScaleOut = nullptr;
    _wetSlider = nullptr;
    _browseButton = nullptr;
    _irBrowserComponent = nullptr;
    _wetButton = nullptr;
    _dryButton = nullptr;
    _autogainButton = nullptr;
    _reverseButton = nullptr;
    _hiFreqLabel = nullptr;
    _hiGainLabel = nullptr;
    _hiGainHeaderLabel = nullptr;
    _hiFreqHeaderLabel = nullptr;
    _hiGainSlider = nullptr;
    _hiFreqSlider = nullptr;
    _loFreqLabel = nullptr;
    _loGainLabel = nullptr;
    _loGainHeaderLabel = nullptr;
    _loFreqHeaderLabel = nullptr;
    _loGainSlider = nullptr;
    _loFreqSlider = nullptr;
    _levelMeterOut = nullptr;
    _levelMeterOutLabelButton = nullptr;
    _levelMeterDryLabel = nullptr;
    _lowEqButton = nullptr;
    _lowCutFreqLabel = nullptr;
    _lowCutFreqHeaderLabel = nullptr;
    _lowCutFreqSlider = nullptr;
    _highCutFreqLabel = nullptr;
    _highCutFreqHeaderLabel = nullptr;
    _highCutFreqSlider = nullptr;
    _highEqButton = nullptr;
    _attackShapeLabel = nullptr;
    _endLabel = nullptr;
    _endSlider = nullptr;
    _attackShapeSlider = nullptr;
    _decayShapeLabel = nullptr;
    _decayShapeHeaderLabel = nullptr;
    _decayShapeSlider = nullptr;
    _attackShapeHeaderLabel = nullptr;
    _endHeaderLabel = nullptr;
    _beginLabel = nullptr;
    _beginSlider = nullptr;
    _beginHeaderLabel = nullptr;
    _widthLabel = nullptr;
    _widthHeaderLabel = nullptr;
    _widthSlider = nullptr;
    _predelayLabel = nullptr;
    _predelayHeaderLabel = nullptr;
    _predelaySlider = nullptr;
    _stretchLabel = nullptr;
    _stretchHeaderLabel = nullptr;
    _stretchSlider = nullptr;
    _attackHeaderLabel = nullptr;
    _attackLengthLabel = nullptr;
    _attackLengthSlider = nullptr;
    _attackLengthHeaderLabel = nullptr;
    _decayHeaderLabel = nullptr;
    _impulseResponseHeaderLabel = nullptr;
    _stereoHeaderLabel = nullptr;
    _titleLabel = nullptr;
    _subtitleLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void KlangFalterEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff313131));

    //[UserPaint] Add your own custom painting code here..
    // constexpr int logoHeight {36};
    // const int xMid {getWidth() / 2};
    // constexpr int xOffset {200};
    // g.setColour(UIUtils::neutralColour.withAlpha(0.2f));
    // {
    //     juce::Image tomLogo(juce::ImageCache::getFromMemory(BinaryData::tom_png, BinaryData::tom_pngSize));
    //     const float aspect {tomLogo.getWidth() / static_cast<float>(tomLogo.getHeight())};
    //     const int width {static_cast<int>(logoHeight * aspect)};
    //     g.drawImage(tomLogo, xMid - xOffset - width / 2, 8, width, logoHeight, 0, 0, tomLogo.getWidth(), tomLogo.getHeight(), true);
    // }
    // {
    //     juce::Image weaLogo(juce::ImageCache::getFromMemory(BinaryData::wea_png, BinaryData::wea_pngSize));
    //     const float aspect {weaLogo.getWidth() / static_cast<float>(weaLogo.getHeight())};
    //     const int width {static_cast<int>(logoHeight * aspect)};
    //     g.drawImage(weaLogo, xMid + xOffset - width / 2, 8, width, logoHeight, 0, 0, weaLogo.getWidth(), weaLogo.getHeight(), true);
    // }

    const int xMid {531};
    {
        constexpr int logoWidth {35};
        juce::Image tomLogo(juce::ImageCache::getFromMemory(BinaryData::tom_png, BinaryData::tom_pngSize));
        const float aspect {tomLogo.getWidth() / static_cast<float>(tomLogo.getHeight())};
        const int logoHeight {static_cast<int>(logoWidth / aspect)};
        g.drawImage(tomLogo,
                    xMid - logoWidth / 2, 60, logoWidth, logoHeight,
                    0, 0, tomLogo.getWidth(), tomLogo.getHeight());
    }
    {
        constexpr int logoWidth {80};
        juce::Image weaLogo(juce::ImageCache::getFromMemory(BinaryData::wea_png, BinaryData::wea_pngSize));
        const float aspect {weaLogo.getWidth() / static_cast<float>(weaLogo.getHeight())};
        const int logoHeight {static_cast<int>(logoWidth / aspect)};
        g.drawImage(weaLogo,
                    xMid - logoWidth / 2, 170, logoWidth, logoHeight,
                    0, 0, weaLogo.getWidth(), weaLogo.getHeight());
    }

    //[/UserPaint]
}

void KlangFalterEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void KlangFalterEditor::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == _drySlider.get())
    {
        //[UserSliderCode__drySlider] -- add your slider handling code here..
        const float scale = static_cast<float>(_drySlider->getValue());
        const float decibels = SnapValue(DecibelScaling::Scale2Db(scale), 0.0f, 0.5f);
        _processor.setParameterNotifyingHost(Parameters::DryDecibels, decibels);
        //[/UserSliderCode__drySlider]
    }
    else if (sliderThatWasMoved == _wetSlider.get())
    {
        //[UserSliderCode__wetSlider] -- add your slider handling code here..
        const float scale = static_cast<float>(_wetSlider->getValue());
        const float decibels = SnapValue(DecibelScaling::Scale2Db(scale), 0.0f, 0.5f);
        _processor.setParameterNotifyingHost(Parameters::WetDecibels, decibels);
        //[/UserSliderCode__wetSlider]
    }
    else if (sliderThatWasMoved == _hiGainSlider.get())
    {
        //[UserSliderCode__hiGainSlider] -- add your slider handling code here..
        _processor.setParameterNotifyingHost(Parameters::EqHighShelfDecibels, SnapValue(static_cast<float>(_hiGainSlider->getValue()), 0.0f, 0.5f));
        //[/UserSliderCode__hiGainSlider]
    }
    else if (sliderThatWasMoved == _hiFreqSlider.get())
    {
        //[UserSliderCode__hiFreqSlider] -- add your slider handling code here..
        _processor.setParameterNotifyingHost(Parameters::EqHighShelfFreq, static_cast<float>(_hiFreqSlider->getValue()));
        //[/UserSliderCode__hiFreqSlider]
    }
    else if (sliderThatWasMoved == _loGainSlider.get())
    {
        //[UserSliderCode__loGainSlider] -- add your slider handling code here..
        _processor.setParameterNotifyingHost(Parameters::EqLowShelfDecibels, SnapValue(static_cast<float>(_loGainSlider->getValue()), 0.0f, 0.5f));
        //[/UserSliderCode__loGainSlider]
    }
    else if (sliderThatWasMoved == _loFreqSlider.get())
    {
        //[UserSliderCode__loFreqSlider] -- add your slider handling code here..
        _processor.setParameterNotifyingHost(Parameters::EqLowShelfFreq, static_cast<float>(_loFreqSlider->getValue()));
        //[/UserSliderCode__loFreqSlider]
    }
    else if (sliderThatWasMoved == _lowCutFreqSlider.get())
    {
        //[UserSliderCode__lowCutFreqSlider] -- add your slider handling code here..
        _processor.setParameterNotifyingHost(Parameters::EqLowCutFreq, static_cast<float>(_lowCutFreqSlider->getValue()));
        //[/UserSliderCode__lowCutFreqSlider]
    }
    else if (sliderThatWasMoved == _highCutFreqSlider.get())
    {
        //[UserSliderCode__highCutFreqSlider] -- add your slider handling code here..
        _processor.setParameterNotifyingHost(Parameters::EqHighCutFreq, static_cast<float>(_highCutFreqSlider->getValue()));
        //[/UserSliderCode__highCutFreqSlider]
    }
    else if (sliderThatWasMoved == _endSlider.get())
    {
        //[UserSliderCode__endSlider] -- add your slider handling code here..
        _processor.setIREnd(_endSlider->getValue());
        //[/UserSliderCode__endSlider]
    }
    else if (sliderThatWasMoved == _attackShapeSlider.get())
    {
        //[UserSliderCode__attackShapeSlider] -- add your slider handling code here..
        _processor.setAttackShape(_attackShapeSlider->getValue());
        //[/UserSliderCode__attackShapeSlider]
    }
    else if (sliderThatWasMoved == _decayShapeSlider.get())
    {
        //[UserSliderCode__decayShapeSlider] -- add your slider handling code here..
        _processor.setDecayShape(_decayShapeSlider->getValue());
        //[/UserSliderCode__decayShapeSlider]
    }
    else if (sliderThatWasMoved == _beginSlider.get())
    {
        //[UserSliderCode__beginSlider] -- add your slider handling code here..
        _processor.setIRBegin(_beginSlider->getValue());
        //[/UserSliderCode__beginSlider]
    }
    else if (sliderThatWasMoved == _widthSlider.get())
    {
        //[UserSliderCode__widthSlider] -- add your slider handling code here..
        _processor.setParameterNotifyingHost(Parameters::StereoWidth, SnapValue(static_cast<float>(_widthSlider->getValue()), 1.0f, 0.05f));
        //[/UserSliderCode__widthSlider]
    }
    else if (sliderThatWasMoved == _predelaySlider.get())
    {
        //[UserSliderCode__predelaySlider] -- add your slider handling code here..
        _processor.setPredelayMs(_predelaySlider->getValue());
        //[/UserSliderCode__predelaySlider]
    }
    else if (sliderThatWasMoved == _stretchSlider.get())
    {
        //[UserSliderCode__stretchSlider] -- add your slider handling code here..
        double sliderVal = _stretchSlider->getValue();
        if (::fabs(sliderVal-1.0) < 0.025)
        {
          sliderVal = 1.0;
          _stretchSlider->setValue(1.0, juce::dontSendNotification);
        }
        _processor.setStretch(sliderVal);
        //[/UserSliderCode__stretchSlider]
    }
    else if (sliderThatWasMoved == _attackLengthSlider.get())
    {
        //[UserSliderCode__attackLengthSlider] -- add your slider handling code here..
        _processor.setAttackLength(_attackLengthSlider->getValue());
        //[/UserSliderCode__attackLengthSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void KlangFalterEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]

    const File presetDirectory = File::getCurrentWorkingDirectory();

    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == _browseButton.get())
    {
        //[UserButtonCode__browseButton] -- add your button handler code here..
        int browserHeight = 0;
        juce::String browseButtonText;
        if (_browseButton->getToggleState())
        {
          browserHeight = 300;
          browseButtonText = juce::String("Hide Browser");
        }
        else
        {
          browserHeight = 0;
          browseButtonText = juce::String("Show Browser");
        }
        setBounds(getBounds().withHeight(_browseButton->getY() + _browseButton->getHeight() + browserHeight));
        _irBrowserComponent->setBounds(_irBrowserComponent->getBounds().withHeight(browserHeight - 10));
        _browseButton->setButtonText(browseButtonText);
        //[/UserButtonCode__browseButton]
    }
    else if (buttonThatWasClicked == _wetButton.get())
    {
        //[UserButtonCode__wetButton] -- add your button handler code here..
        _processor.setParameterNotifyingHost(Parameters::WetOn, _wetButton->getToggleState());
        //[/UserButtonCode__wetButton]
    }
    else if (buttonThatWasClicked == _dryButton.get())
    {
        //[UserButtonCode__dryButton] -- add your button handler code here..
        _processor.setParameterNotifyingHost(Parameters::DryOn, _dryButton->getToggleState());
        //[/UserButtonCode__dryButton]
    }
    else if (buttonThatWasClicked == _autogainButton.get())
    {
        //[UserButtonCode__autogainButton] -- add your button handler code here..
        _processor.setParameterNotifyingHost(Parameters::AutoGainOn, _autogainButton->getToggleState());
        //[/UserButtonCode__autogainButton]
    }
    else if (buttonThatWasClicked == _reverseButton.get())
    {
        //[UserButtonCode__reverseButton] -- add your button handler code here..
        _processor.setReverse(_reverseButton->getToggleState());
        //[/UserButtonCode__reverseButton]
    }
    else if (buttonThatWasClicked == _levelMeterOutLabelButton.get())
    {
        //[UserButtonCode__levelMeterOutLabelButton] -- add your button handler code here..
        _processor.getSettings().setResultLevelMeterDisplay(_levelMeterOutLabelButton->getToggleState() ? Settings::Out : Settings::Wet);
        //[/UserButtonCode__levelMeterOutLabelButton]
    }
    else if (buttonThatWasClicked == _lowEqButton.get())
    {
        //[UserButtonCode__lowEqButton] -- add your button handler code here..
        const Parameters::EqType lowEqType = static_cast<Parameters::EqType>(_processor.getParameter(Parameters::EqLowType));
        _processor.setParameterNotifyingHost(Parameters::EqLowType, static_cast<int>((lowEqType == Parameters::Cut) ? Parameters::Shelf : Parameters::Cut));
        //[/UserButtonCode__lowEqButton]
    }
    else if (buttonThatWasClicked == _highEqButton.get())
    {
        //[UserButtonCode__highEqButton] -- add your button handler code here..
        const Parameters::EqType highEqType = static_cast<Parameters::EqType>(_processor.getParameter(Parameters::EqHighType));
        _processor.setParameterNotifyingHost(Parameters::EqHighType, static_cast<int>((highEqType == Parameters::Cut) ? Parameters::Shelf : Parameters::Cut));
        //[/UserButtonCode__highEqButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void KlangFalterEditor::updateUI()
{
  const bool irAvailable = _processor.irAvailable();
  const size_t numInputChannels = static_cast<size_t>(std::min(_processor.getTotalNumInputChannels(), 2));
  const size_t numOutputChannels = static_cast<size_t>(std::min(_processor.getTotalNumOutputChannels(), 2));
  {
    const double stretch = _processor.getStretch();
    _stretchSlider->setEnabled(irAvailable);
    _stretchSlider->setRange(0.5, 1.5);
    _stretchSlider->setValue(stretch, juce::dontSendNotification);
    _stretchLabel->setText(String(static_cast<int>(100.0*stretch)) + String("%"), juce::sendNotification);
  }
  {
    const float db = _processor.getParameter(Parameters::DryDecibels);
    const float scale = DecibelScaling::Db2Scale(db);
    _drySlider->setEnabled(true);
    _drySlider->setRange(0.0, 1.0);
    _drySlider->setValue(scale, juce::dontSendNotification);
    _dryLevelLabel->setText(DecibelScaling::DecibelString(db), juce::sendNotification);
    _dryButton->setToggleState(_processor.getParameter(Parameters::DryOn), juce::dontSendNotification);
  }
  {
    const float db = _processor.getParameter(Parameters::WetDecibels);
    const float scale = DecibelScaling::Db2Scale(db);
    _wetSlider->setEnabled(true);
    _wetSlider->setRange(0.0, 1.0);
    _wetSlider->setValue(scale, juce::dontSendNotification);
    _wetLevelLabel->setText(DecibelScaling::DecibelString(db), juce::sendNotification);
    _wetButton->setToggleState(_processor.getParameter(Parameters::WetOn), juce::dontSendNotification);
  }
  {
    _reverseButton->setEnabled(true);
    _reverseButton->setToggleState(_processor.getReverse(), juce::dontSendNotification);
  }
  {
    const double irBegin = _processor.getIRBegin();
    _beginSlider->setEnabled(irAvailable);
    _beginSlider->setValue(irBegin, juce::dontSendNotification);
    _beginLabel->setText(juce::String(100.0 * irBegin, 1) + juce::String("%"), juce::sendNotification);
  }
  {
    const double irEnd = _processor.getIREnd();
    _endSlider->setEnabled(irAvailable);
    _endSlider->setValue(irEnd, juce::dontSendNotification);
    _endLabel->setText(juce::String(100.0 * irEnd, 1) + juce::String("%"), juce::sendNotification);
  }
  {
    const double predelayMs = _processor.getPredelayMs();
    _predelaySlider->setValue(predelayMs);
    _predelaySlider->setEnabled(irAvailable);
    _predelayLabel->setText(FormatSeconds(predelayMs / 1000.0), juce::sendNotification);
  }
  {
    const double attackLength = _processor.getAttackLength();
    _attackLengthSlider->setValue(attackLength);
    _attackLengthSlider->setEnabled(irAvailable);
    _attackLengthLabel->setText(juce::String(100.0 * attackLength, 1)+juce::String("%"), juce::sendNotification);
  }
  {
    const double attackShape = _processor.getAttackShape();
    _attackShapeSlider->setValue(attackShape);
    _attackShapeSlider->setEnabled(irAvailable);
    _attackShapeLabel->setText((attackShape < 0.0001) ? juce::String("Neutral") : juce::String(attackShape, 2), juce::sendNotification);
  }
  {
    const double decayShape = _processor.getDecayShape();
    _decayShapeSlider->setValue(decayShape);
    _decayShapeSlider->setEnabled(irAvailable);
    _decayShapeLabel->setText((decayShape < 0.0001) ? juce::String("Neutral") : juce::String(decayShape, 2), juce::sendNotification);
  }
  {
    const float autoGainDecibels = _processor.getParameter(Parameters::AutoGainDecibels);
    const bool autoGainOn = _processor.getParameter(Parameters::AutoGainOn);
    const juce::String autoGainText = DecibelScaling::DecibelString(autoGainDecibels);
    _autogainButton->setButtonText(juce::String("Autogain ") + autoGainText);
    _autogainButton->setToggleState(autoGainOn, juce::dontSendNotification);
  }
  {
    Parameters::EqType lowEqType = static_cast<Parameters::EqType>(_processor.getParameter(Parameters::EqLowType));
    const bool loEqEnabled = irAvailable;
    const float cutFreq = _processor.getParameter(Parameters::EqLowCutFreq);
    const float shelfFreq = _processor.getParameter(Parameters::EqLowShelfFreq);
    const float shelfGainDb = _processor.getParameter(Parameters::EqLowShelfDecibels);
    _lowEqButton->setButtonText(lowEqType == Parameters::Shelf ? juce::String("Low Shelf") : juce::String("Low Cut"));
    _lowCutFreqHeaderLabel->setVisible(lowEqType == Parameters::Cut);
    _lowCutFreqLabel->setVisible(lowEqType == Parameters::Cut);
    _lowCutFreqLabel->setText((::fabs(cutFreq-Parameters::EqLowCutFreq.getMinValue()) > 0.0001f) ? FormatFrequency(cutFreq) : juce::String("Off"), juce::sendNotification);
    _lowCutFreqSlider->setVisible(lowEqType == Parameters::Cut);
    _lowCutFreqSlider->setEnabled(loEqEnabled);
    _lowCutFreqSlider->setValue(cutFreq, juce::dontSendNotification);
    _loFreqHeaderLabel->setVisible(lowEqType == Parameters::Shelf);
    _loFreqLabel->setVisible(lowEqType == Parameters::Shelf);
    _loFreqLabel->setText(FormatFrequency(shelfFreq), juce::sendNotification);
    _loFreqSlider->setVisible(lowEqType == Parameters::Shelf);
    _loFreqSlider->setEnabled(loEqEnabled);
    _loFreqSlider->setValue(shelfFreq, juce::dontSendNotification);
    _loGainHeaderLabel->setVisible(lowEqType == Parameters::Shelf);
    _loGainLabel->setVisible(lowEqType == Parameters::Shelf);
    _loGainLabel->setText(DecibelScaling::DecibelString(shelfGainDb), juce::sendNotification);
    _loGainSlider->setVisible(lowEqType == Parameters::Shelf);
    _loGainSlider->setEnabled(loEqEnabled);
    _loGainSlider->setValue(shelfGainDb, juce::dontSendNotification);
  }
  {
    Parameters::EqType highEqType = static_cast<Parameters::EqType>(_processor.getParameter(Parameters::EqHighType));
    const bool hiEqEnabled = irAvailable;
    const float cutFreq = _processor.getParameter(Parameters::EqHighCutFreq);
    const float shelfFreq = _processor.getParameter(Parameters::EqHighShelfFreq);
    const float shelfGainDb = _processor.getParameter(Parameters::EqHighShelfDecibels);
    _highEqButton->setButtonText(highEqType == Parameters::Shelf ? juce::String("High Shelf") : juce::String("High Cut"));
    _highCutFreqHeaderLabel->setVisible(highEqType == Parameters::Cut);
    _highCutFreqLabel->setVisible(highEqType == Parameters::Cut);
    _highCutFreqLabel->setText((::fabs(cutFreq-Parameters::EqHighCutFreq.getMaxValue()) > 0.0001f) ? FormatFrequency(cutFreq) : juce::String("Off"), juce::sendNotification);
    _highCutFreqSlider->setVisible(highEqType == Parameters::Cut);
    _highCutFreqSlider->setEnabled(hiEqEnabled);
    _highCutFreqSlider->setValue(cutFreq, juce::dontSendNotification);
    _hiFreqHeaderLabel->setVisible(highEqType == Parameters::Shelf);
    _hiFreqLabel->setVisible(highEqType == Parameters::Shelf);
    _hiFreqLabel->setText(FormatFrequency(shelfFreq), juce::sendNotification);
    _hiFreqSlider->setVisible(highEqType == Parameters::Shelf);
    _hiFreqSlider->setEnabled(hiEqEnabled);
    _hiFreqSlider->setValue(shelfFreq, juce::dontSendNotification);
    _hiGainHeaderLabel->setVisible(highEqType == Parameters::Shelf);
    _hiGainLabel->setVisible(highEqType == Parameters::Shelf);
    _hiGainLabel->setText(DecibelScaling::DecibelString(shelfGainDb), juce::sendNotification);
    _hiGainSlider->setVisible(highEqType == Parameters::Shelf);
    _hiGainSlider->setEnabled(hiEqEnabled);
    _hiGainSlider->setValue(shelfGainDb, juce::dontSendNotification);
  }
  {
    _widthHeaderLabel->setVisible(numOutputChannels >= 2);
    _widthSlider->setEnabled(irAvailable);
    _widthSlider->setVisible(numOutputChannels >= 2);
    _widthLabel->setVisible(numOutputChannels >= 2);
    const float stereoWidth = _processor.getParameter(Parameters::StereoWidth);
    _widthSlider->setValue(stereoWidth, juce::dontSendNotification);
    _widthLabel->setText((::fabs(1.0f-stereoWidth) < 0.001) ? juce::String("Neutral") : juce::String(stereoWidth, 2), juce::sendNotification);
  }
  {
    _levelMeterDry->setChannelCount(numInputChannels);
    _levelMeterOut->setChannelCount(numOutputChannels);
    Settings::ResultLevelMeterDisplay resultDisplay = _processor.getSettings().getResultLevelMeterDisplay();
    _levelMeterOutLabelButton->setToggleState(resultDisplay == Settings::Out, juce::dontSendNotification);
    _levelMeterOutLabelButton->setButtonText((resultDisplay == Settings::Out) ? juce::String("Out") : juce::String("Wet"));
  }
  {
    const size_t numTabs = static_cast<size_t>(_irTabComponent->getNumTabs());
    if (numTabs > numInputChannels * numOutputChannels || numTabs != _irComponents.size())
    {
      _irTabComponent->clearTabs();
      _irComponents.clear();
    }
    for (size_t input=0; input<numInputChannels; ++input)
    {
      for (size_t output=0; output<numOutputChannels; ++output)
      {
        if (_irComponents.find(std::make_pair(input, output)) == _irComponents.end())
        {
          IRAgent* agent = _processor.getAgent(input, output);
          jassert(agent);
          if (agent)
          {
            IRComponent* irComponent = new IRComponent();
            irComponent->init(_processor.getAgent(input, output));
            _irTabComponent->addTab(juce::String(static_cast<int>(input+1)) + juce::String("-") + juce::String(static_cast<int>(output+1)),
                                    juce::Colour(0xffb0b0b6),
                                    irComponent,
                                    true);
            _irComponents.insert(std::make_pair(std::make_pair(input, output), irComponent));
          }
        }
      }
    }
  }
}


void KlangFalterEditor::changeListenerCallback(ChangeBroadcaster* source)
{
  if (source)
  {
    updateUI();
  }
}


void KlangFalterEditor::changeNotification()
{
  updateUI();
}


void KlangFalterEditor::timerCallback()
{
  Settings::ResultLevelMeterDisplay resultDisplay = _processor.getSettings().getResultLevelMeterDisplay();
  _levelMeterDry->setLevel(0, _processor.getLevelDry(0));
  _levelMeterDry->setLevel(1, _processor.getLevelDry(1));
  _levelMeterOut->setLevel(0, (resultDisplay == Settings::Out) ? _processor.getLevelOut(0) : _processor.getLevelWet(0));
  _levelMeterOut->setLevel(1, (resultDisplay == Settings::Out) ? _processor.getLevelOut(1) : _processor.getLevelWet(1));
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="KlangFalterEditor" componentName=""
                 parentClasses="public AudioProcessorEditor, public ChangeNotifier::Listener, public ChangeListener, public Timer"
                 constructorParams="Processor&amp; processor" variableInitialisers="AudioProcessorEditor(&amp;processor),&#10;_processor(processor),&#10;_toggleButtonLookAndFeel(new UIUtils::ToggleButtonLookAndFeel()),&#10;_rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()),&#10;_linearSliderLookAndFeel(new UIUtils::LinearSliderLookAndFeel())"
                 snapPixels="4" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="760" initialHeight="340">
  <BACKGROUND backgroundColour="ff313131"/>
  <GENERICCOMPONENT name="" id="6dd7ac2ee661b784" memberName="_decibelScaleDry" virtualName=""
                    explicitFocusOrder="0" pos="584 51 32 176" class="DecibelScale"
                    params=""/>
  <TABBEDCOMPONENT name="IRTabComponent" id="697fc3546f1ab7f1" memberName="_irTabComponent"
                   virtualName="" explicitFocusOrder="0" pos="16 51 462 176" orientation="top"
                   tabBarDepth="0" initialTab="0">
    <TAB name="Placeholder" colour="ffb0b0b6" useJucerComp="1" contentClassName=""
         constructorParams="" jucerComponentFile="IRComponent.cpp"/>
  </TABBEDCOMPONENT>
  <GENERICCOMPONENT name="" id="93270230a2db62e0" memberName="_levelMeterDry" virtualName=""
                    explicitFocusOrder="0" pos="632 51 12 176" class="LevelMeter"
                    params=""/>
  <LABEL name="DryLevelLabel" id="892bd8ba7f961215" memberName="_dryLevelLabel"
         virtualName="" explicitFocusOrder="0" pos="572 231 60 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="-inf" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="WetLevelLabel" id="3469fbc38286d2b6" memberName="_wetLevelLabel"
         virtualName="" explicitFocusOrder="0" pos="656 231 64 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="-inf" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="" id="3694f3553dea94b" memberName="_drySlider" virtualName=""
          explicitFocusOrder="0" pos="612 43 24 192" min="0.0" max="10.0"
          int="0.0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <GENERICCOMPONENT name="" id="f3824f7df1d2ea95" memberName="_decibelScaleOut" virtualName=""
                    explicitFocusOrder="0" pos="672 51 32 176" class="DecibelScale"
                    params=""/>
  <SLIDER name="" id="e50054d828347fbd" memberName="_wetSlider" virtualName=""
          explicitFocusOrder="0" pos="700 43 24 192" min="0.0" max="10.0"
          int="0.0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TEXTBUTTON name="" id="e5cc4d9d88fb6d29" memberName="_browseButton" virtualName=""
              explicitFocusOrder="0" pos="12 319 736 24" tooltip="Show Browser For Impulse Response Selection"
              bgColOn="ffbcbcff" buttonText="Show Browser" connectedEdges="8"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="5388ff2994f22af6" memberName="_irBrowserComponent"
                    virtualName="" explicitFocusOrder="0" pos="12 343 736 288" class="IRBrowserComponent"
                    params=""/>
  <TEXTBUTTON name="" id="c0b279e2bae7030e" memberName="_wetButton" virtualName=""
              explicitFocusOrder="0" pos="684 255 44 24" tooltip="Wet Signal On/Off"
              bgColOff="80bcbcbc" bgColOn="ffbcbcff" textCol="ff202020" textColOn="ff202020"
              buttonText="Wet" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="499237d463b07642" memberName="_dryButton" virtualName=""
              explicitFocusOrder="0" pos="596 255 44 24" tooltip="Dry Signal On/Off"
              bgColOff="80bcbcbc" bgColOn="ffbcbcff" textCol="ff202020" textColOn="ff202020"
              buttonText="Dry" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="f25a3c5b0535fcca" memberName="_autogainButton" virtualName=""
              explicitFocusOrder="0" pos="596 287 132 24" tooltip="Autogain On/Off"
              bgColOff="80bcbcbc" bgColOn="ffbcbcff" textCol="ff202020" textColOn="ff202020"
              buttonText="Autogain 0.0dB" connectedEdges="3" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="" id="fcb6829f0d6fc21f" memberName="_reverseButton" virtualName=""
              explicitFocusOrder="0" pos="20 198 72 24" tooltip="Reverse Impulse Response"
              bgColOff="80bcbcbc" bgColOn="ffbcbcff" textCol="ff202020" textColOn="ff202020"
              buttonText="Reverse" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="841738a894cf241a" memberName="_hiFreqLabel" virtualName=""
         explicitFocusOrder="0" pos="480 291 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="15.2kHz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="fc2c2b94ce457ca1" memberName="_hiGainLabel" virtualName=""
         explicitFocusOrder="0" pos="516 291 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="0.0dB" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="56744cc537b91ad6" memberName="_hiGainHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="516 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="Gain" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="71caf1a3b5a498dd" memberName="_hiFreqHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="480 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="Freq" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="" id="1c3776d0e6cbce1d" memberName="_hiGainSlider" virtualName=""
          explicitFocusOrder="0" pos="524 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="-30.0" max="30.0" int="0.0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="" id="702db07b37c24f93" memberName="_hiFreqSlider" virtualName=""
          explicitFocusOrder="0" pos="488 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="2000.0" max="20000.0" int="0.0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="" id="8b28430d938b39ca" memberName="_loFreqLabel" virtualName=""
         explicitFocusOrder="0" pos="396 291 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="1234Hz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="390bab67dc140d90" memberName="_loGainLabel" virtualName=""
         explicitFocusOrder="0" pos="432 291 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="0.0dB" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="8d9e4adc7538b7dc" memberName="_loGainHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="432 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="Gain" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="f3b3523aee42340f" memberName="_loFreqHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="396 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="Freq" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="" id="a3dc7342caaa661f" memberName="_loGainSlider" virtualName=""
          explicitFocusOrder="0" pos="440 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="-30.0" max="30.0" int="0.0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="" id="313e885756edaea8" memberName="_loFreqSlider" virtualName=""
          explicitFocusOrder="0" pos="404 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="20.0" max="2000.0" int="0.0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <GENERICCOMPONENT name="" id="e4867bf99a47726a" memberName="_levelMeterOut" virtualName=""
                    explicitFocusOrder="0" pos="720 51 12 176" class="LevelMeter"
                    params=""/>
  <TEXTBUTTON name="" id="f5ed3d758ad2c3f4" memberName="_levelMeterOutLabelButton"
              virtualName="" explicitFocusOrder="0" pos="712 31 28 18" tooltip="Switches Between Out/Wet Level Measurement"
              bgColOff="bbbbff" bgColOn="bcbcff" textCol="ffb0b0b6" textColOn="ffb0b0b6"
              buttonText="Out" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="55e5b719a217b777" memberName="_levelMeterDryLabel"
         virtualName="" explicitFocusOrder="0" pos="620 27 36 24" textCol="ffb0b0b6"
         edTextCol="ffb0b0b6" edBkgCol="0" labelText="Dry" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="91aebb1e5cd3b857" memberName="_lowEqButton" virtualName=""
              explicitFocusOrder="0" pos="404 231 72 24" bgColOff="bbbbff"
              bgColOn="2c2cff" textCol="ffb0b0b6" textColOn="ffb0b0b6" buttonText="Low Cut"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="3c3de25483a2083a" memberName="_lowCutFreqLabel" virtualName=""
         explicitFocusOrder="0" pos="414 291 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="1234Hz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="e7ff4fcd0be82eec" memberName="_lowCutFreqHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="414 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="Freq" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="" id="939a6b5201207a68" memberName="_lowCutFreqSlider"
          virtualName="" explicitFocusOrder="0" pos="422 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="20.0" max="2000.0" int="0.0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="" id="54b3891e3f360022" memberName="_highCutFreqLabel"
         virtualName="" explicitFocusOrder="0" pos="498 291 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="15.2kHz" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="e765fe8c17a454cf" memberName="_highCutFreqHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="498 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="Freq" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="" id="b5ba6600a0fbff4e" memberName="_highCutFreqSlider"
          virtualName="" explicitFocusOrder="0" pos="506 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="2000.0" max="20000.0" int="0.0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TEXTBUTTON name="" id="e0f66dc6348e3991" memberName="_highEqButton" virtualName=""
              explicitFocusOrder="0" pos="488 231 72 24" bgColOff="bbbbff"
              bgColOn="2c2cff" textCol="ffb0b0b6" textColOn="ffb0b0b6" buttonText="High Cut"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="bd223d64f25070f1" memberName="_attackShapeLabel"
         virtualName="" explicitFocusOrder="0" pos="212 291 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="1.0" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="ee8d936af3b4da1e" memberName="_endLabel" virtualName=""
         explicitFocusOrder="0" pos="76 291 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="100%" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="" id="69b1c81fb4f7c601" memberName="_endSlider" virtualName=""
          explicitFocusOrder="0" pos="84 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="0.0" max="1.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="" id="4212ad3906b4822c" memberName="_attackShapeSlider"
          virtualName="" explicitFocusOrder="0" pos="220 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="0.0" max="10.0" int="0.0" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.5" needsCallback="1"/>
  <LABEL name="" id="31ae76799c1691c8" memberName="_decayShapeLabel" virtualName=""
         explicitFocusOrder="0" pos="276 291 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="1.0" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="23dd6aa127e1d22c" memberName="_decayShapeHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="276 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="Shape" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="" id="ec6cfdb461a4ddf" memberName="_decayShapeSlider" virtualName=""
          explicitFocusOrder="0" pos="284 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="0.0" max="10.0" int="0.0" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.5" needsCallback="1"/>
  <LABEL name="" id="77a4289ec7918eef" memberName="_attackShapeHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="212 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="Shape" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="f09d4e48c9272494" memberName="_endHeaderLabel" virtualName=""
         explicitFocusOrder="0" pos="76 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="End" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="83e84ce51a3ad2f0" memberName="_beginLabel" virtualName=""
         explicitFocusOrder="0" pos="40 291 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="100%" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="" id="abd56bd3093af749" memberName="_beginSlider" virtualName=""
          explicitFocusOrder="0" pos="48 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="0.0" max="1.0" int="0.001" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.5" needsCallback="1"/>
  <LABEL name="" id="8a8a2c2daac6a39b" memberName="_beginHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="40 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="Begin" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="90fac88de886d96b" memberName="_widthLabel" virtualName=""
         explicitFocusOrder="0" pos="340 291 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="1.0" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="3476a26a68685b9e" memberName="_widthHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="340 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="Width" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="" id="31fdb25b044eaf7f" memberName="_widthSlider" virtualName=""
          explicitFocusOrder="0" pos="348 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="0.0" max="10.0" int="0.0" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.30102" needsCallback="1"/>
  <LABEL name="" id="d8fd174283fc5f04" memberName="_predelayLabel" virtualName=""
         explicitFocusOrder="0" pos="4 291 52 24" textCol="ffb0b0b6" edTextCol="ff000000"
         edBkgCol="0" labelText="0ms" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="11.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="c0f7ab9477e45174" memberName="_predelayHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="4 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="Gap" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="" id="b8aaa21b836b98e0" memberName="_predelaySlider" virtualName=""
          explicitFocusOrder="0" pos="12 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="0.0" max="1000.0" int="0.0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="" id="52624fc555056069" memberName="_stretchLabel" virtualName=""
         explicitFocusOrder="0" pos="112 291 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="100%" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="5a11a2fc858b9666" memberName="_stretchHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="112 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="Stretch" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="" id="6c84a40c942d5b18" memberName="_stretchSlider" virtualName=""
          explicitFocusOrder="0" pos="120 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="0.0" max="2.0" int="0.0" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="" id="4dd28ec5626fb754" memberName="_attackHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="176 231 88 24" textCol="ffb0b0b6"
         edTextCol="ffb0b0b6" edBkgCol="0" labelText="Attack" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="a08f22558873cfa8" memberName="_attackLengthLabel"
         virtualName="" explicitFocusOrder="0" pos="176 291 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="0ms" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="" id="cfcacb5123869692" memberName="_attackLengthSlider"
          virtualName="" explicitFocusOrder="0" pos="184 267 36 28" thumbcol="ffafafff"
          rotarysliderfill="b1606060" min="0.0" max="1.0" int="0.0" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.5" needsCallback="1"/>
  <LABEL name="" id="c1b73cae5819a5a1" memberName="_attackLengthHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="176 247 52 24" textCol="ffb0b0b6"
         edTextCol="ff000000" edBkgCol="0" labelText="Length" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="11.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="9ac1cf0b8251e7f3" memberName="_decayHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="256 231 88 24" textCol="ffb0b0b6"
         edTextCol="ffb0b0b6" edBkgCol="0" labelText="Decay" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="" id="1932084a508f975f" memberName="_impulseResponseHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="12 231 144 24" textCol="ffb0b0b6"
         edTextCol="ffb0b0b6" edBkgCol="0" labelText="Impulse Response"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="" id="3deee43dc79846d0" memberName="_stereoHeaderLabel"
         virtualName="" explicitFocusOrder="0" pos="340 231 52 24" textCol="ffb0b0b6"
         edTextCol="ffb0b0b6" edBkgCol="0" labelText="Stereo" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="Title Label" id="f44017b03503c913" memberName="_titleLabel"
         virtualName="" explicitFocusOrder="0" pos="248 4 195 40" textCol="ffb0b0b6"
         edTextCol="ffb0b0b6" edBkgCol="0" labelText="Body &amp; Soul:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="35.3" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="Subtitle Label" id="4df8702c7583ab78" memberName="_subtitleLabel"
         virtualName="" explicitFocusOrder="0" pos="432 4 79 40" textCol="ffb0b0b6"
         edTextCol="ffb0b0b6" edBkgCol="0" labelText="Intro" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="35.3" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

