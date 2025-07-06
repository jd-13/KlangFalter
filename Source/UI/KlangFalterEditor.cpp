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
      _simpleButtonLookAndFeel(new UIUtils::SimpleButtonLookAndFeel()),
      _rotarySliderLookAndFeel(new UIUtils::RotarySliderLookAndFeel()),
      _linearSliderLookAndFeel(new UIUtils::LinearSliderLookAndFeel()),
      _constrainer(new juce::ComponentBoundsConstrainer())
{
    //[Constructor_pre] You can add your own custom stuff here..
    _theme = UIUtils::LoadTheme();
    //[/Constructor_pre]

    _decibelScaleDry.reset(new DecibelScale());
    addAndMakeVisible(_decibelScaleDry.get());

    _irTabComponent.reset(new juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible(_irTabComponent.get());
    _irTabComponent->setTabBarDepth(0);
    _irTabComponent->addTab(TRANS ("Placeholder"), juce::Colour(0xffb0b0b6), new IRComponent(_theme, _processor), true);
    _irTabComponent->setCurrentTabIndex(0);
    _irTabComponent->setColour(juce::TabbedComponent::backgroundColourId, _theme.waveformContainerBackground);

    _levelMeterDry.reset(new LevelMeter(_theme));
    addAndMakeVisible(_levelMeterDry.get());

    _dryLevelLabel.reset(new juce::Label("DryLevelLabel", TRANS("-inf")));
    addAndMakeVisible(_dryLevelLabel.get());
    _dryLevelLabel->setFont(juce::Font (11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _dryLevelLabel->setJustificationType(juce::Justification::centredRight);
    _dryLevelLabel->setEditable(false, false, false);
    _dryLevelLabel->setColour(juce::Label::textColourId, _theme.neutral);

    _wetLevelLabel.reset(new juce::Label("WetLevelLabel", TRANS("-inf")));
    addAndMakeVisible(_wetLevelLabel.get());
    _wetLevelLabel->setFont(juce::Font(11.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _wetLevelLabel->setJustificationType(juce::Justification::centredRight);
    _wetLevelLabel->setEditable(false, false, false);
    _wetLevelLabel->setColour(juce::Label::textColourId, _theme.neutral);

    auto setLinearSliderColours = [&](juce::Slider* slider) {
        slider->setColour(juce::Slider::thumbColourId, _theme.highlight);
        slider->setColour(juce::Slider::trackColourId, _theme.neutral);
    };

    _drySlider.reset(new juce::Slider(juce::String()));
    addAndMakeVisible(_drySlider.get());
    _drySlider->setRange(0, 10, 0);
    _drySlider->setSliderStyle(juce::Slider::LinearVertical);
    _drySlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _drySlider->addListener(this);
    _drySlider->setDoubleClickReturnValue(true, DecibelScaling::Db2Scale(Parameters::DryDecibels.getDefaultValue()));
    _drySlider->setLookAndFeel(_linearSliderLookAndFeel.get());
    setLinearSliderColours(_drySlider.get());

    _decibelScaleOut.reset(new DecibelScale());
    addAndMakeVisible(_decibelScaleOut.get());

    _wetSlider.reset(new juce::Slider (juce::String()));
    addAndMakeVisible(_wetSlider.get());
    _wetSlider->setRange(0, 10, 0);
    _wetSlider->setSliderStyle(juce::Slider::LinearVertical);
    _wetSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    _wetSlider->addListener(this);
    _wetSlider->setDoubleClickReturnValue(true, DecibelScaling::Db2Scale(Parameters::WetDecibels.getDefaultValue()));
    _wetSlider->setLookAndFeel(_linearSliderLookAndFeel.get());
    setLinearSliderColours(_wetSlider.get());

    _browseButton.reset(new juce::TextButton(juce::String()));
    addAndMakeVisible(_browseButton.get());
    _browseButton->setTooltip(TRANS("Show Browser For Impulse Response Selection"));
    _browseButton->setButtonText(TRANS("Show Browser"));
    _browseButton->setConnectedEdges(juce::Button::ConnectedOnBottom);
    _browseButton->addListener(this);
    _browseButton->setColour(UIUtils::ToggleButtonLookAndFeel::offColour, _theme.neutral);
    _browseButton->setColour(UIUtils::ToggleButtonLookAndFeel::onColour, _theme.neutral);
    _browseButton->setLookAndFeel(_toggleButtonLookAndFeel.get());
    _browseButton->setClickingTogglesState(true);
    _browseButton->setConnectedEdges(juce::Button::ConnectedOnBottom);

    _irBrowserComponent.reset(new IRBrowserComponent());
    addAndMakeVisible(_irBrowserComponent.get());

    auto setButtonColours = [&](juce::TextButton* button) {
        button->setColour(UIUtils::ToggleButtonLookAndFeel::offColour, _theme.neutral);
        button->setColour(UIUtils::ToggleButtonLookAndFeel::onColour, _theme.highlight);
    };

    _wetButton.reset(new juce::TextButton(juce::String()));
    addAndMakeVisible(_wetButton.get());
    _wetButton->setTooltip(TRANS("Wet Signal On/Off"));
    _wetButton->setButtonText(TRANS("Wet"));
    _wetButton->addListener(this);
    setButtonColours(_wetButton.get());
    _wetButton->setLookAndFeel(_toggleButtonLookAndFeel.get());
    _wetButton->setClickingTogglesState(true);

    _dryButton.reset(new juce::TextButton(juce::String()));
    addAndMakeVisible(_dryButton.get());
    _dryButton->setTooltip(TRANS("Dry Signal On/Off"));
    _dryButton->setButtonText(TRANS("Dry"));
    _dryButton->addListener(this);
    setButtonColours(_dryButton.get());
    _dryButton->setLookAndFeel(_toggleButtonLookAndFeel.get());
    _dryButton->setClickingTogglesState(true);

    _autogainButton.reset(new juce::TextButton(juce::String()));
    addAndMakeVisible(_autogainButton.get());
    _autogainButton->setTooltip(TRANS("Autogain On/Off"));
    _autogainButton->setButtonText(TRANS("Autogain 0.0dB"));
    _autogainButton->addListener(this);
    setButtonColours(_autogainButton.get());
    _autogainButton->setLookAndFeel(_toggleButtonLookAndFeel.get());
    _autogainButton->setClickingTogglesState(true);

    _levelMeterOut.reset(new LevelMeter(_theme));
    addAndMakeVisible(_levelMeterOut.get());

    //[UserPreSize]
    _rotarySliderLookAndFeel->theme = _theme;
    _irSliderGroup.reset(new IRSliderGroup(_processor, _theme));
    addAndMakeVisible(_irSliderGroup.get());

    _tuneSliderGroup.reset(new TuneSliderGroup(_processor, _theme));
    addAndMakeVisible(_tuneSliderGroup.get());

    _attackSliderGroup.reset(new AttackSliderGroup(_processor, _theme));
    addAndMakeVisible(_attackSliderGroup.get());

    _decaySliderGroup.reset(new DecaySliderGroup(_processor, _theme));
    addAndMakeVisible(_decaySliderGroup.get());

    _stereoSliderGroup.reset(new StereoSliderGroup(_processor, _theme));
    addAndMakeVisible(_stereoSliderGroup.get());

    _lowEqSliderGroup.reset(new LowEqSliderGroup(_processor, _theme));
    addAndMakeVisible(_lowEqSliderGroup.get());

    _highEqSliderGroup.reset(new HighEqSliderGroup(_processor, _theme));
    addAndMakeVisible(_highEqSliderGroup.get());

    // _shimmerSliderGroup.reset(new ShimmerSliderGroup(_processor, _theme));
    // addAndMakeVisible(_shimmerSliderGroup.get());

    _chorusSliderGroup.reset(new ChorusSliderGroup(_processor, _theme));
    addAndMakeVisible(_chorusSliderGroup.get());

    _saveLoadComponent.reset(new SaveLoadComponent(_processor, _theme));
    addAndMakeVisible(_saveLoadComponent.get());

    _tomLogo.reset(new Logo(juce::ImageCache::getFromMemory(BinaryData::tom_png, BinaryData::tom_pngSize)));
    addAndMakeVisible(_tomLogo.get());

    _weaLogo.reset(new Logo(juce::ImageCache::getFromMemory(BinaryData::wea_png, BinaryData::wea_pngSize)));
    addAndMakeVisible(_weaLogo.get());

    customLookAndFeel->theme = _theme;
    setLookAndFeel(customLookAndFeel);

    _title.reset(new Title(_theme, getAudioProcessor()->wrapperType));
    addAndMakeVisible(_title.get());
    //[/UserPreSize]

    const juce::Rectangle<int> bounds = _processor.getUIBounds();
    setSize(bounds.getWidth(), bounds.getHeight());

    //[Constructor] You can add your own custom stuff here..
    setResizable(true, true);
    _constrainer->setFixedAspectRatio(UIUtils::NOMINAL_WIDTH / UIUtils::NOMINAL_HEIGHT);
    _constrainer->setMinimumWidth(UIUtils::NOMINAL_WIDTH);
    setConstrainer(_constrainer.get());

    _irTabComponent->clearTabs(); // Remove placeholder only used as dummy in the Jucer

    _processor.addNotificationListener(this);
    _processor.getSettings().addChangeListener(this);
    _irBrowserComponent->init(&_processor, _theme);
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
    _tuneSliderGroup = nullptr;
    _attackSliderGroup = nullptr;
    _decaySliderGroup = nullptr;
    _stereoSliderGroup = nullptr;
    _lowEqSliderGroup = nullptr;
    _highEqSliderGroup = nullptr;
    // _shimmerSliderGroup = nullptr;
    _chorusSliderGroup = nullptr;
    _saveLoadComponent = nullptr;

    _processor.setUIBounds(getBounds(), true);
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
    _levelMeterOut = nullptr;
    _title = nullptr;


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
    g.fillAll(juce::Colour(_theme.background));
    //[/UserPaint]
}

void KlangFalterEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    auto setFontHeight = [](juce::Label* label, float height) {
        label->setFont(label->getFont().withHeight(height));
    };

    juce::Rectangle<int> availableArea = getLocalBounds();

    // Title row
    {
        const int TITLE_ROW_HEIGHT {scaled(61)};
        const int TITLE_WIDTH {scaled(250)};

        juce::Rectangle<int> titleRow = availableArea.removeFromTop(TITLE_ROW_HEIGHT);
        titleRow.reduce(scaled(10), scaled(10));

        _title->setBounds(titleRow.removeFromRight(TITLE_WIDTH).reduced(0, scaled(5)));

        _saveLoadComponent->setBounds(titleRow);
    }

    // Imager row
    const int METERS_TOTAL_WIDTH {scaled(160)};
    {
        const int IMAGER_ROW_HEIGHT {scaled(166)};
        const int IMAGER_ROW_MARGIN {scaled(16)};
        juce::Rectangle<int> imagerRow = availableArea.removeFromTop(IMAGER_ROW_HEIGHT);
        imagerRow.reduce(IMAGER_ROW_MARGIN, 0);

        juce::Rectangle<int> metersArea = imagerRow.removeFromLeft(METERS_TOTAL_WIDTH);
        metersArea.reduce(0, scaled(8));
        metersArea.removeFromLeft(scaled(4));
        metersArea.removeFromRight(scaled(14));

        auto getMeterSliderBounds = [&](juce::Rectangle<int> bounds, const juce::Rectangle<int>& metersArea) {
            return bounds.withBottomY(metersArea.getBottom() - scaled(5)).withHeight(metersArea.getHeight() + scaled(10));
        };

        const int METER_WIDTH {scaled(12)};
        const float METER_SLIDER_WIDTH {METER_WIDTH * 2.0f};
        const float METER_SCALE_WIDTH {METER_WIDTH * 2.4f};

        _decibelScaleDry->setBounds(metersArea.removeFromLeft(METER_SCALE_WIDTH));
        _drySlider->setBounds(getMeterSliderBounds(metersArea.removeFromLeft(METER_SLIDER_WIDTH), metersArea));
        _levelMeterDry->setBounds(metersArea.removeFromLeft(METER_WIDTH));

        _levelMeterOut->setBounds(metersArea.removeFromRight(METER_WIDTH));
        _wetSlider->setBounds(getMeterSliderBounds(metersArea.removeFromRight(METER_SLIDER_WIDTH), metersArea));
        _decibelScaleOut->setBounds(metersArea.removeFromRight(METER_SCALE_WIDTH));

        _irTabComponent->setBounds(imagerRow);
    }

    // Sliders row
    juce::Rectangle<int> slidersTopRow = availableArea.removeFromTop(scaled(89));
    juce::Rectangle<int> slidersBottomRow = availableArea.removeFromTop(scaled(89));

    // Gain buttons
    {
        juce::Rectangle<int> gainButtonsArea = slidersTopRow.removeFromLeft(METERS_TOTAL_WIDTH);
        juce::Rectangle<int> gainButtonLabelsRow = gainButtonsArea.removeFromTop(scaled(24));

        const int LEVEL_LABEL_WIDTH {scaled(60)};
        const int LEVEL_LABEL_OFFSET {scaled(4)};
        _dryLevelLabel->setBounds(_drySlider->getRight() - LEVEL_LABEL_WIDTH + LEVEL_LABEL_OFFSET, gainButtonLabelsRow.getY(), LEVEL_LABEL_WIDTH, gainButtonLabelsRow.getHeight());
        _wetLevelLabel->setBounds(_wetSlider->getRight() - LEVEL_LABEL_WIDTH + LEVEL_LABEL_OFFSET, gainButtonLabelsRow.getY(), LEVEL_LABEL_WIDTH, gainButtonLabelsRow.getHeight());

        setFontHeight(_dryLevelLabel.get(), scaledFloat(11.0f));
        setFontHeight(_wetLevelLabel.get(), scaledFloat(11.0f));

        const int BUTTON_WIDTH {scaled(44)};
        const int BUTTON_HEIGHT {scaled(24)};
        const int ROW_MARGIN {scaled(8)};
        const int BUTTON_OFFSET {scaled(2)};
        _dryButton->setBounds(_drySlider->getBounds().getCentreX() - BUTTON_WIDTH / 2 - BUTTON_OFFSET, gainButtonsArea.getY(), BUTTON_WIDTH, BUTTON_HEIGHT);
        _wetButton->setBounds(_wetSlider->getBounds().getCentreX() - BUTTON_WIDTH / 2 - BUTTON_OFFSET, gainButtonsArea.getY(), BUTTON_WIDTH, BUTTON_HEIGHT);

        _autogainButton->setBounds(_dryButton->getX(), _dryButton->getBottom() + ROW_MARGIN, _wetButton->getRight() - _dryButton->getX(), BUTTON_HEIGHT);
    }

    // Logos
    {
        juce::Rectangle<int> logoArea = slidersBottomRow.removeFromLeft(METERS_TOTAL_WIDTH);
        logoArea.reduce(0, scaled(16));
        logoArea.removeFromLeft(scaled(25));
        logoArea.removeFromRight(scaled(10));

        _tomLogo->setBounds(logoArea.removeFromLeft(scaled(60)));
        _weaLogo->setBounds(logoArea.removeFromRight(scaled(60)));
    }

    slidersTopRow.removeFromTop(scaled(4));
    slidersTopRow.removeFromBottom(scaled(2));
    slidersTopRow.removeFromLeft(scaled(20));
    slidersTopRow.removeFromRight(scaled(20));

    slidersBottomRow.removeFromTop(scaled(2));
    slidersBottomRow.removeFromBottom(scaled(4));
    slidersBottomRow.removeFromLeft(scaled(20));
    slidersBottomRow.removeFromRight(scaled(20));

    // Sliders
    {
        const juce::FlexItem::Margin marginRight(0, scaled(38), 0, 0);
        // Top row
        {
            juce::FlexBox flexBox;
            flexBox.flexDirection = juce::FlexBox::Direction::row;
            flexBox.flexWrap = juce::FlexBox::Wrap::wrap;
            flexBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
            flexBox.alignContent = juce::FlexBox::AlignContent::center;

            flexBox.items.add(juce::FlexItem(*_irSliderGroup.get()).withMinWidth(scaled(108)).withMinHeight(slidersTopRow.getHeight()).withMargin(marginRight));
            flexBox.items.add(juce::FlexItem(*_tuneSliderGroup.get()).withMinWidth(scaled(52)).withMinHeight(slidersTopRow.getHeight()).withMargin(marginRight));
            flexBox.items.add(juce::FlexItem(*_attackSliderGroup.get()).withMinWidth(scaled(88)).withMinHeight(slidersTopRow.getHeight()).withMargin(marginRight));
            flexBox.items.add(juce::FlexItem(*_decaySliderGroup.get()).withMinWidth(scaled(52)).withMinHeight(slidersTopRow.getHeight()));
            flexBox.performLayout(slidersTopRow.toFloat());
        }

        // Bottom row
        {
            juce::FlexBox flexBox;
            flexBox.flexDirection = juce::FlexBox::Direction::row;
            flexBox.flexWrap = juce::FlexBox::Wrap::wrap;
            flexBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
            flexBox.alignContent = juce::FlexBox::AlignContent::center;

            flexBox.items.add(juce::FlexItem(*_chorusSliderGroup.get()).withMinWidth(scaled(108)).withMinHeight(slidersBottomRow.getHeight()).withMargin(marginRight));
            flexBox.items.add(juce::FlexItem(*_stereoSliderGroup.get()).withMinWidth(scaled(52)).withMinHeight(slidersBottomRow.getHeight()).withMargin(marginRight));
            flexBox.items.add(juce::FlexItem(*_lowEqSliderGroup.get()).withMinWidth(scaled(88)).withMinHeight(slidersBottomRow.getHeight()));
            flexBox.items.add(juce::FlexItem(*_highEqSliderGroup.get()).withMinWidth(scaled(88)).withMinHeight(slidersBottomRow.getHeight()));
            // flexBox.items.add(juce::FlexItem(*_shimmerSliderGroup.get()).withMinWidth(scaled(72)).withMinHeight(slidersTopRow.getHeight()).withMargin(marginRight));
            flexBox.performLayout(slidersBottomRow.toFloat());
        }
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
        const juce::Rectangle<int> browseButtonArea = availableArea.removeFromTop(IR_BROWSER_BUTTON_HEIGHT)
            .withTrimmedLeft(IR_BROWSER_MARGIN)
            .withTrimmedRight(IR_BROWSER_MARGIN);
        _browseButton->setBounds(browseButtonArea);
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
  _tuneSliderGroup->onUpdate(irAvailable);
  _attackSliderGroup->onUpdate(irAvailable);
  _decaySliderGroup->onUpdate(irAvailable);
  _stereoSliderGroup->onUpdate(irAvailable, numOutputChannels);
  _lowEqSliderGroup->onUpdate(irAvailable);
  _highEqSliderGroup->onUpdate(irAvailable);
//   _shimmerSliderGroup->onUpdate(irAvailable);
  _chorusSliderGroup->onUpdate(irAvailable, numOutputChannels);
  _saveLoadComponent->refresh();
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
    const float autoGainDecibels = _processor.getParameter(Parameters::AutoGainDecibels);
    const bool autoGainOn = _processor.getParameter(Parameters::AutoGainOn);
    const juce::String autoGainText = DecibelScaling::DecibelString(autoGainDecibels);
    _autogainButton->setButtonText(juce::String("Autogain ") + autoGainText);
    _autogainButton->setToggleState(autoGainOn, juce::dontSendNotification);
  }
  {
    _levelMeterDry->setChannelCount(numInputChannels);
    _levelMeterOut->setChannelCount(numOutputChannels);
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
            IRComponent* irComponent = new IRComponent(_theme, _processor);
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
  _levelMeterDry->setLevel(0, _processor.getLevelDry(0));
  _levelMeterDry->setLevel(1, _processor.getLevelDry(1));
  _levelMeterOut->setLevel(0, _processor.getLevelWet(0));
  _levelMeterOut->setLevel(1, _processor.getLevelWet(1));
}

void KlangFalterEditor::_updateIRBrowserOpen(bool isOpen) {
    if (isOpen) {
        _constrainer->setFixedAspectRatio(UIUtils::NOMINAL_WIDTH / (UIUtils::NOMINAL_HEIGHT + IR_BROWSER_AREA_HEIGHT));
        _browseButton->setButtonText("Hide IR Browser");
    } else {
        _constrainer->setFixedAspectRatio(UIUtils::NOMINAL_WIDTH / UIUtils::NOMINAL_HEIGHT);
        _browseButton->setButtonText("Show IR Browser");
    }

    // Make sure state is always consistent
    _processor.setIrBrowserOpen(isOpen);
    _browseButton->setToggleState(isOpen, juce::dontSendNotification);

    const int newHeight = getWidth() / _constrainer->getFixedAspectRatio();
    setBounds(getBounds().withHeight(newHeight));

}

int KlangFalterEditor::scaled(float value) const {
    // All measurements based on getWidth(), as getHeight() changes if the browser is open
    return static_cast<int>(getWidth() / (UIUtils::NOMINAL_WIDTH / value));
}

float KlangFalterEditor::scaledFloat(float value) const {
    // All measurements based on getWidth(), as getHeight() changes if the browser is open
    return getWidth() / (UIUtils::NOMINAL_WIDTH / value);
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
         constructorParams="_theme" jucerComponentFile="IRComponent.cpp"/>
  </TABBEDCOMPONENT>
  <GENERICCOMPONENT name="" id="93270230a2db62e0" memberName="_levelMeterDry" virtualName=""
                    explicitFocusOrder="0" pos="632 51 12 176" class="LevelMeter"
                    params="_theme"/>
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
                    params="_theme"/>
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

