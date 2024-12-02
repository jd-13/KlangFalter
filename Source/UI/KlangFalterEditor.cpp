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



namespace {
    constexpr int IR_BROWSER_AREA_HEIGHT {300};
}


//[/MiscUserDefs]

//==============================================================================
KlangFalterEditor::KlangFalterEditor (Processor& processor)
    : AudioProcessorEditor(&processor),
      _processor(processor),
      _toggleButtonLookAndFeel(new UIUtils::ToggleButtonLookAndFeel()),
      _rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()),
      _linearSliderLookAndFeel(new UIUtils::LinearSliderLookAndFeel()),
      _constrainer(new juce::ComponentBoundsConstrainer())
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

    _creditsButton.reset (new juce::TextButton (juce::String()));
    addAndMakeVisible (_creditsButton.get());
    _creditsButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0x00bbbbff));
    _creditsButton->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0x002c2cff));
    _creditsButton->setColour (juce::TextButton::textColourOffId, juce::Colour (0xffb0b0b6));
    _creditsButton->setColour (juce::TextButton::textColourOnId, juce::Colour (0xffb0b0b6));

    _creditsButton->setBounds (244, 4, 268, 40);

    _resetButton.reset (new juce::TextButton (juce::String()));
    addAndMakeVisible (_resetButton.get());
    _resetButton->setTooltip (TRANS ("Reset all parameters"));
    _resetButton->setButtonText (TRANS ("Reset"));
    _resetButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0x80bcbcbc));
    _resetButton->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xffbcbcff));
    _resetButton->setColour (juce::TextButton::textColourOffId, juce::Colour (0xff202020));
    _resetButton->setColour (juce::TextButton::textColourOnId, juce::Colour (0xff202020));

    _resetButton->setBounds (16, 14, 52, 24);


    //[UserPreSize]
    _irSliderGroup.reset(new IRSliderGroup(_processor));
    addAndMakeVisible(_irSliderGroup.get());

    _attackSliderGroup.reset(new AttackSliderGroup(_processor));
    addAndMakeVisible(_attackSliderGroup.get());

    _decaySliderGroup.reset(new DecaySliderGroup(_processor));
    addAndMakeVisible(_decaySliderGroup.get());

    _stereoSliderGroup.reset(new StereoSliderGroup(_processor));
    addAndMakeVisible(_stereoSliderGroup.get());

    _lowEqSliderGroup.reset(new LowEqSliderGroup(_processor));
    addAndMakeVisible(_lowEqSliderGroup.get());

    _highEqSliderGroup.reset(new HighEqSliderGroup(_processor));
    addAndMakeVisible(_highEqSliderGroup.get());

    setLookAndFeel(customLookAndFeel);

    _creditsWindowOptions.reset(new juce::DialogWindow::LaunchOptions());
    _creditsWindowOptions->dialogTitle = "Credits";
    _creditsWindowOptions->dialogBackgroundColour = UIUtils::backgroundColour;
    _creditsWindowOptions->componentToCentreAround = this;
    _creditsWindowOptions->useNativeTitleBar = false;

    _creditsButton->onClick = [&]() {
        const juce::CharPointer_UTF8 credits(
            "Concept and IR design: The Sound of Merlin\n"
            "Programming: White Elephant Audio\n"
            "Support: info@thesoundofmerlin.com\n"
            "\n"
            "Body & Soul: Intro is open source software (GPLv3) and is based on Klangfalter from HiFi-LoFi.\n"
            "\n"
            "This convolution reverb was conceived to be used as a module in White Elephant Audio's plugin host Syndicate but can also be used separately from it.\n"
            "\n"
            "A special thank you goes to Eirik Gr\xc3\xb8nner from Demningen Studios in Norway for his wonderful suggestions and support.");

        _creditsWindowOptions->content.set(new juce::Label("Credits", credits), true);
        juce::DialogWindow* window = _creditsWindowOptions->launchAsync();
        window->centreWithSize(500, 300);
    };

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
    _resetButton->setLookAndFeel(_toggleButtonLookAndFeel.get());

    setButtonColours(_dryButton.get());
    setButtonColours(_wetButton.get());
    setButtonColours(_autogainButton.get());
    setButtonColours(_reverseButton.get());

    _reverseButton->setColour(UIUtils::ToggleButtonLookAndFeel::offColour, UIUtils::backgroundColour);
    _browseButton->setColour(UIUtils::ToggleButtonLookAndFeel::offColour, UIUtils::neutralColour);
    _browseButton->setColour(UIUtils::ToggleButtonLookAndFeel::onColour, UIUtils::neutralColour);

    _browseButton->setConnectedEdges(juce::Button::ConnectedOnBottom);

    _resetButton->setColour(UIUtils::ToggleButtonLookAndFeel::offColour, UIUtils::neutralColour);
    _resetButton->setColour(UIUtils::ToggleButtonLookAndFeel::onColour, UIUtils::neutralColour);

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
    setResizable(true, true);
    _constrainer->setFixedAspectRatio(760.0 / 340.0);
    _constrainer->setMinimumWidth(760);
    setConstrainer(_constrainer.get());

    _irTabComponent->clearTabs(); // Remove placeholder only used as dummy in the Jucer
    _browseButton->setClickingTogglesState(true);
    _dryButton->setClickingTogglesState(true);
    _wetButton->setClickingTogglesState(true);
    _autogainButton->setClickingTogglesState(true);
    _reverseButton->setClickingTogglesState(true);
    _levelMeterOutLabelButton->setClickingTogglesState(true);


    // Double click to default
    _drySlider->setDoubleClickReturnValue(true, DecibelScaling::Db2Scale(Parameters::DryDecibels.getDefaultValue()));
    _wetSlider->setDoubleClickReturnValue(true, DecibelScaling::Db2Scale(Parameters::WetDecibels.getDefaultValue()));

    _resetButton->onClick = [&]() {
        _processor.setPredelayMs(0.0);
        _processor.setIRBegin(0.0);
        _processor.setIREnd(1.0);
        _processor.setStretch(1.0);
        _processor.setAttackLength(0.0);
        _processor.setAttackShape(0.0);
        _processor.setDecayShape(0.0);
        _processor.setParameterNotifyingHost(Parameters::StereoWidth, Parameters::StereoWidth.getDefaultValue());

        _processor.setReverse(false);

        _processor.setParameterNotifyingHost(Parameters::EqLowType, Parameters::EqLowType.getDefaultValue());
        _processor.setParameterNotifyingHost(Parameters::EqLowCutFreq, Parameters::EqLowCutFreq.getDefaultValue());
        _processor.setParameterNotifyingHost(Parameters::EqLowShelfFreq, Parameters::EqLowShelfFreq.getDefaultValue());
        _processor.setParameterNotifyingHost(Parameters::EqLowShelfDecibels, Parameters::EqLowShelfDecibels.getDefaultValue());

        _processor.setParameterNotifyingHost(Parameters::EqHighType, Parameters::EqHighType.getDefaultValue());
        _processor.setParameterNotifyingHost(Parameters::EqHighCutFreq, Parameters::EqHighCutFreq.getDefaultValue());
        _processor.setParameterNotifyingHost(Parameters::EqHighShelfFreq, Parameters::EqHighShelfFreq.getDefaultValue());
        _processor.setParameterNotifyingHost(Parameters::EqHighShelfDecibels, Parameters::EqHighShelfDecibels.getDefaultValue());
    };

    _processor.addNotificationListener(this);
    _processor.getSettings().addChangeListener(this);
    _irBrowserComponent->init(&_processor);
    updateUI();
    startTimer(100);
    _updateIRBrowserOpen(_processor.getIrBrowserOpen());
    //[/Constructor]
}

KlangFalterEditor::~KlangFalterEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    _settingsDialogWindow.deleteAndZero();
    _processor.removeNotificationListener(this);
    _processor.getSettings().removeChangeListener(this);
    setConstrainer(nullptr);
    _irSliderGroup = nullptr;
    _attackSliderGroup = nullptr;
    _decaySliderGroup = nullptr;
    _stereoSliderGroup = nullptr;
    _lowEqSliderGroup = nullptr;
    _highEqSliderGroup = nullptr;
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
    _levelMeterOut = nullptr;
    _levelMeterOutLabelButton = nullptr;
    _levelMeterDryLabel = nullptr;
    _titleLabel = nullptr;
    _subtitleLabel = nullptr;
    _creditsButton = nullptr;
    _resetButton = nullptr;


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

    const int logoAreaWidth {_decibelScaleDry->getX() - _irTabComponent->getRight()};
    const int xMid {_irTabComponent->getRight() + logoAreaWidth / 2};
    {
        const int logoWidth {scaled(35)};
        juce::Image tomLogo(juce::ImageCache::getFromMemory(BinaryData::tom_png, BinaryData::tom_pngSize));
        const float aspect {tomLogo.getWidth() / static_cast<float>(tomLogo.getHeight())};
        const int logoHeight {static_cast<int>(logoWidth / aspect)};
        g.drawImage(tomLogo,
                    xMid - logoWidth / 2, scaled(60), logoWidth, logoHeight,
                    0, 0, tomLogo.getWidth(), tomLogo.getHeight());
    }
    {
        const int logoWidth {scaled(80)};
        juce::Image weaLogo(juce::ImageCache::getFromMemory(BinaryData::wea_png, BinaryData::wea_pngSize));
        const float aspect {weaLogo.getWidth() / static_cast<float>(weaLogo.getHeight())};
        const int logoHeight {static_cast<int>(logoWidth / aspect)};
        g.drawImage(weaLogo,
                    xMid - logoWidth / 2, scaled(170), logoWidth, logoHeight,
                    0, 0, weaLogo.getWidth(), weaLogo.getHeight());
    }

    //[/UserPaint]
}

void KlangFalterEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    juce::Rectangle<int> availableArea = getLocalBounds();

    // Title row
    {
        const int TITLE_ROW_HEIGHT {scaled(51)};
        const int TITLE_TEXT_HEIGHT {scaled(40)};
        juce::Rectangle<int> titleRow = availableArea.removeFromTop(TITLE_ROW_HEIGHT).withTrimmedTop(scaled(4));

        const int titleRowYCentre {availableArea.getWidth() / 2};
        const int titleTextWidth {_titleLabel->getFont().getStringWidth(_titleLabel->getText())};
        const int titleTextTotalWidth {
            titleTextWidth + _subtitleLabel->getFont().getStringWidth(_subtitleLabel->getText())
        };

        _titleLabel->setBounds(
            titleRowYCentre - titleTextTotalWidth / 2,
            scaled(4),
            titleTextWidth,
            TITLE_TEXT_HEIGHT);

        _subtitleLabel->setBounds(
            titleRowYCentre - titleTextTotalWidth / 2 + titleTextWidth,
            scaled(4),
            titleTextTotalWidth - titleTextWidth,
            TITLE_TEXT_HEIGHT);

        _creditsButton->setBounds(
            _titleLabel->getX(),
            _titleLabel->getY(),
            titleTextTotalWidth,
            TITLE_TEXT_HEIGHT);

        _resetButton->setBounds(scaled(16), scaled(14), scaled(52), scaled(24));
    }

    // Imager row
    const int METERS_TOTAL_WIDTH {scaled(160)};
    {
        const int IMAGER_ROW_HEIGHT {scaled(176)};
        const int IMAGER_ROW_MARGIN {scaled(16)};
        juce::Rectangle<int> imagerRow = availableArea.removeFromTop(IMAGER_ROW_HEIGHT);
        imagerRow.reduce(IMAGER_ROW_MARGIN, 0);

        juce::Rectangle<int> metersArea = imagerRow.removeFromRight(METERS_TOTAL_WIDTH);

        auto getMeterSliderBounds = [&](juce::Rectangle<int> bounds, const juce::Rectangle<int>& metersArea) {
            return bounds.withBottomY(metersArea.getBottom() - scaled(8)).withHeight(metersArea.getHeight() + scaled(16));
        };

        juce::Rectangle<int> meterButtonsArea = metersArea.removeFromTop(scaled(18));

        const int METER_WIDTH {scaled(12)};
        const int METER_SLIDER_WIDTH {METER_WIDTH * 2};
        const int METER_SCALE_WIDTH {METER_WIDTH * 3};
        _levelMeterOut->setBounds(metersArea.removeFromRight(METER_WIDTH));
        _wetSlider->setBounds(getMeterSliderBounds(metersArea.removeFromRight(METER_SLIDER_WIDTH), metersArea));
        _decibelScaleOut->setBounds(metersArea.removeFromRight(METER_SCALE_WIDTH));

        _decibelScaleDry->setBounds(metersArea.removeFromLeft(METER_SCALE_WIDTH));
        _drySlider->setBounds(getMeterSliderBounds(metersArea.removeFromLeft(METER_SLIDER_WIDTH), metersArea));
        _levelMeterDry->setBounds(metersArea.removeFromLeft(METER_WIDTH));

        auto positionMeterButton = [&](juce::Component* button, const std::unique_ptr<LevelMeter>& meter) {
            const int BUTTON_WIDTH {scaled(28)};
            const int xPos {meter->getX() + meter->getWidth() / 2 - BUTTON_WIDTH / 2};
            button->setBounds(xPos, meterButtonsArea.getY(), BUTTON_WIDTH, meterButtonsArea.getHeight());
        };
        positionMeterButton(_levelMeterDryLabel.get(), _levelMeterDry);
        positionMeterButton(_levelMeterOutLabelButton.get(), _levelMeterOut);

        imagerRow.removeFromRight(scaled(106));
        _irTabComponent->setBounds(imagerRow);

        _reverseButton->setBounds(scaled(20), scaled(198), scaled(72), scaled(24));
    }

    // IR Browser
    {
        const int IR_BROWSER_MARGIN {scaled(12)};

        if (_processor.getIrBrowserOpen()) {
            const juce::Rectangle<int> irBrowserArea = availableArea.removeFromBottom(scaled(IR_BROWSER_AREA_HEIGHT))
                .withTrimmedBottom(IR_BROWSER_MARGIN)
                .withTrimmedLeft(IR_BROWSER_MARGIN)
                .withTrimmedRight(IR_BROWSER_MARGIN);

            _irBrowserComponent->setBounds(irBrowserArea);
        } else {
            _irBrowserComponent->setBounds(availableArea.removeFromBottom(0));
        }

        const int IR_BROWSER_BUTTON_HEIGHT {scaled(24)};
        availableArea.removeFromBottom(scaled(1));

        const juce::Rectangle<int> browseButtonArea = availableArea.removeFromBottom(IR_BROWSER_BUTTON_HEIGHT)
            .withTrimmedLeft(IR_BROWSER_MARGIN)
            .withTrimmedRight(IR_BROWSER_MARGIN);
        _browseButton->setBounds(browseButtonArea);
    }

    // Sliders row

    // Gain buttons
    {
        juce::Rectangle<int> gainButtonsArea = availableArea.removeFromRight(METERS_TOTAL_WIDTH);
        juce::Rectangle<int> gainButtonLabelsRow = gainButtonsArea.removeFromTop(scaled(24));

        const int LEVEL_LABEL_WIDTH {scaled(60)};
        _dryLevelLabel->setBounds(_drySlider->getRight() - LEVEL_LABEL_WIDTH, gainButtonLabelsRow.getY(), LEVEL_LABEL_WIDTH, gainButtonLabelsRow.getHeight());
        _wetLevelLabel->setBounds(_wetSlider->getRight() - LEVEL_LABEL_WIDTH, gainButtonLabelsRow.getY(), LEVEL_LABEL_WIDTH, gainButtonLabelsRow.getHeight());

        juce::FlexBox flexBox;
        flexBox.flexDirection = juce::FlexBox::Direction::row;
        flexBox.flexWrap = juce::FlexBox::Wrap::wrap;
        flexBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
        flexBox.alignContent = juce::FlexBox::AlignContent::flexStart;

        const juce::FlexItem::Margin marginRight(0, scaled(16), 0, 0);
        const juce::FlexItem::Margin marginTop(scaled(8), 0, 0, 0);
        flexBox.items.add(juce::FlexItem(*_dryButton.get()).withMinWidth(scaled(44)).withMinHeight(scaled(24)).withMargin(marginRight));
        flexBox.items.add(juce::FlexItem(*_wetButton.get()).withMinWidth(scaled(44)).withMinHeight(scaled(24)));
        flexBox.items.add(juce::FlexItem(*_autogainButton.get()).withMinWidth(scaled(132)).withMinHeight(scaled(24)).withMargin(marginTop));
        flexBox.performLayout(gainButtonsArea.toFloat());
    }

    availableArea.removeFromTop(scaled(4));
    availableArea.removeFromBottom(scaled(4));
    availableArea.removeFromLeft(scaled(12));

    // Sliders
    {
        const int SPACE_WIDTH {scaled(20)};
        _irSliderGroup->setBounds(availableArea.removeFromLeft(scaled(144)));
        availableArea.removeFromLeft(SPACE_WIDTH);

        _attackSliderGroup->setBounds(availableArea.removeFromLeft(scaled(88)));
        availableArea.removeFromLeft(SPACE_WIDTH);

        _decaySliderGroup->setBounds(availableArea.removeFromLeft(scaled(52)));
        availableArea.removeFromLeft(SPACE_WIDTH);

        _stereoSliderGroup->setBounds(availableArea.removeFromLeft(scaled(52)));
        availableArea.removeFromLeft(SPACE_WIDTH);

        _lowEqSliderGroup->setBounds(availableArea.removeFromLeft(scaled(72)));
        availableArea.removeFromLeft(SPACE_WIDTH);

        _highEqSliderGroup->setBounds(availableArea.removeFromLeft(scaled(72)));
    }

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
        _updateIRBrowserOpen(_browseButton->getToggleState());
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

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void KlangFalterEditor::updateUI()
{
  const bool irAvailable = _processor.irAvailable();
  const size_t numInputChannels = static_cast<size_t>(std::min(_processor.getTotalNumInputChannels(), 2));
  const size_t numOutputChannels = static_cast<size_t>(std::min(_processor.getTotalNumOutputChannels(), 2));
  _irSliderGroup->onUpdate(irAvailable);
  _attackSliderGroup->onUpdate(irAvailable);
  _decaySliderGroup->onUpdate(irAvailable);
  _stereoSliderGroup->onUpdate(irAvailable, numOutputChannels);
  _lowEqSliderGroup->onUpdate(irAvailable);
  _highEqSliderGroup->onUpdate(irAvailable);
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
    const float autoGainDecibels = _processor.getParameter(Parameters::AutoGainDecibels);
    const bool autoGainOn = _processor.getParameter(Parameters::AutoGainOn);
    const juce::String autoGainText = DecibelScaling::DecibelString(autoGainDecibels);
    _autogainButton->setButtonText(juce::String("Autogain ") + autoGainText);
    _autogainButton->setToggleState(autoGainOn, juce::dontSendNotification);
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

void KlangFalterEditor::_updateIRBrowserOpen(bool isOpen) {
    int browserHeight = 0;
    juce::String browseButtonText;
    if (isOpen)
    {
        browserHeight = IR_BROWSER_AREA_HEIGHT + 1;
        browseButtonText = juce::String("Hide Browser");
    }
    else
    {
        browserHeight = 0;
        browseButtonText = juce::String("Show Browser");
    }

    // Make sure state is always consistent
    _processor.setIrBrowserOpen(isOpen);
    _browseButton->setToggleState(isOpen, juce::dontSendNotification);

    _constrainer->setFixedAspectRatio(760.0 / (340.0 + browserHeight));

    const int newHeight = getWidth() / _constrainer->getFixedAspectRatio();
    setBounds(getBounds().withHeight(newHeight));

    _browseButton->setButtonText(browseButtonText);
}

int KlangFalterEditor::scaled(float value) const {
    // All measurements based on getWidth(), as getHeight() changes if the browser is open
    constexpr float NOMINAL_WIDTH {760};
    return static_cast<int>(getWidth() / (NOMINAL_WIDTH / value));
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
  <TEXTBUTTON name="" id="4a427657c2a2a228" memberName="_creditsButton" virtualName=""
              explicitFocusOrder="0" pos="244 4 268 40" bgColOff="bbbbff" bgColOn="2c2cff"
              textCol="ffb0b0b6" textColOn="ffb0b0b6" buttonText="" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="" id="44f8c20ca7250560" memberName="_resetButton" virtualName=""
              explicitFocusOrder="0" pos="16 14 52 24" tooltip="Reset all parameters"
              bgColOff="80bcbcbc" bgColOn="ffbcbcff" textCol="ff202020" textColOn="ff202020"
              buttonText="Reset" connectedEdges="0" needsCallback="0" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

