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
#include "../IRAgent.h"
#include "../Settings.h"
//[/Headers]

#include "IRComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
IRComponent::IRComponent (UIUtils::Theme theme, Processor& processor)
        : _simpleButtonLookAndFeel(new UIUtils::SimpleButtonLookAndFeel()),
          _toggleButtonLookAndFeel(new UIUtils::ToggleButtonLookAndFeel()),
          _processor(processor) {
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("IRComponent");
    _waveformComponent.reset (new WaveformComponent (theme));
    addAndMakeVisible (_waveformComponent.get());
    _waveformComponent->setName ("WaveformComponent");

    _waveformComponent->setBounds (4, 4, 454, 140);

    _loadButton.reset (new juce::TextButton ("LoadButton"));
    addAndMakeVisible (_loadButton.get());
    _loadButton->setTooltip (TRANS ("Current impulse response information"));
    _loadButton->setButtonText (TRANS ("No Impulse Response"));
    _loadButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    _loadButton->addListener (this);
    _loadButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0x00bbbbff));
    _loadButton->setColour (juce::TextButton::buttonOnColourId, juce::Colour (0x004444ff));
    _loadButton->setColour (juce::TextButton::textColourOffId, juce::Colour (0xff202020));
    _loadButton->setColour (juce::TextButton::textColourOnId, juce::Colour (0xff202020));
    _loadButton->setLookAndFeel(_simpleButtonLookAndFeel.get());

    _directionButtons.reset(new UIUtils::IRDirectionButtons(
      [&]() { _processor.setReverse(false); },
      [&]() { _processor.setReverse(true); },
      [&]() { return !_processor.getReverse(); },
      [&]() { return _processor.getReverse(); },
      theme
    ));
    addAndMakeVisible(_directionButtons.get());

    _scaleUnitButtons.reset(new UIUtils::ScaleUnitButtons(
      [&]() {
        Settings& settings = _irAgent->getProcessor().getSettings();
        settings.setTimelineUnit(Settings::Seconds);
        _waveformComponent->repaint();
       },
      [&]() {
        Settings& settings = _irAgent->getProcessor().getSettings();
        settings.setTimelineUnit(Settings::Beats);
        _waveformComponent->repaint();
      },
      [&]() { return _processor.getSettings().getTimelineUnit() == Settings::Seconds; },
      [&]() { return _processor.getSettings().getTimelineUnit() == Settings::Beats; },
      theme
    ));
    addAndMakeVisible(_scaleUnitButtons.get());
    //[UserPreSize]
    //[/UserPreSize]

    setSize (462, 176);


    //[Constructor] You can add your own custom stuff here..
    _irAgent = nullptr;
    //[/Constructor]
}

IRComponent::~IRComponent()
{
    IRComponent::init(nullptr);

    _waveformComponent = nullptr;
    _loadButton = nullptr;
    _directionButtons = nullptr;
    _scaleUnitButtons = nullptr;
}

//==============================================================================
void IRComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffb0b0b6));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void IRComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    auto scaled = [&](float value) {
        constexpr float NOMINAL_WIDTH {462};
        return static_cast<int>(getWidth() / (NOMINAL_WIDTH / value));
    };

    juce::Rectangle<int> availableArea = getLocalBounds();
    availableArea.reduce(scaled(4), scaled(4));

    _waveformComponent->setBounds(availableArea.removeFromTop(scaled(140)));
    availableArea.removeFromTop(scaled(4));
    _directionButtons->setBounds(availableArea.removeFromLeft(scaled(72)));
    availableArea.removeFromLeft(scaled(4));
    _scaleUnitButtons->setBounds(availableArea.removeFromLeft(scaled(72)));
    availableArea.removeFromLeft(scaled(4));
    _loadButton->setBounds(availableArea);
    //[/UserResized]
}

void IRComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == _loadButton.get())
    {
        //[UserButtonCode__loadButton] -- add your button handler code here..
        //[/UserButtonCode__loadButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void IRComponent::init(IRAgent* irAgent)
{
  if (_irAgent)
  {
    _irAgent->removeNotificationListener(this);
    _irAgent->getProcessor().removeNotificationListener(this);
    _irAgent = nullptr;
  }

  if (irAgent)
  {
    _irAgent = irAgent;
    _irAgent->addNotificationListener(this);
    _irAgent->getProcessor().addNotificationListener(this);
  }
  irChanged();
}


void IRComponent::irChanged()
{
  String fileLabelText("No File Loaded");

  juce::String toolTip("No Impulse Response");

  double sampleRate = 0.0;
  size_t samplesPerPx = 0;

  if (_irAgent)
  {
    const File file = _irAgent->getFile();
    if (file.existsAsFile())
    {
      fileLabelText = file.getFileName();

      const Processor& processor = _irAgent->getProcessor();
      sampleRate = processor.getSampleRate();

      constexpr int WAVEFORM_NOMINAL_WIDTH {452};
      samplesPerPx = static_cast<size_t>(1.6 * (processor.getMaxFileDuration()+1.0) * sampleRate) / WAVEFORM_NOMINAL_WIDTH;
    }
  }
  _waveformComponent->init(_irAgent, sampleRate, samplesPerPx);
  _loadButton->setButtonText(fileLabelText);
}


void IRComponent::changeNotification()
{
  irChanged();
}
