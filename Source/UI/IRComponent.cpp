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
IRComponent::IRComponent (UIUtils::Theme theme) : _simpleButtonLookAndFeel(new UIUtils::SimpleButtonLookAndFeel())
{
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

    _loadButton->setBounds (80, 148, 378, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (462, 176);


    //[Constructor] You can add your own custom stuff here..
    _irAgent = nullptr;
    _loadButton->setLookAndFeel(_simpleButtonLookAndFeel.get());
    //[/Constructor]
}

IRComponent::~IRComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    IRComponent::init(nullptr);
    //[/Destructor_pre]

    _waveformComponent = nullptr;
    _loadButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
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
    availableArea.removeFromLeft(scaled(76));
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
      const Processor& processor = _irAgent->getProcessor();
      const unsigned fileSampleCount = static_cast<unsigned>(_irAgent->getFileSampleCount());
      const double fileSampleRate = _irAgent->getFileSampleRate();
      const double fileSeconds = static_cast<double>(fileSampleCount) / fileSampleRate;
      const unsigned fileChannelCount = static_cast<unsigned>(_irAgent->getFileChannelCount());
      fileLabelText = file.getFileName() + String(", ") + String(fileChannelCount) + String(" Channels, ") + String(fileSeconds, 2) + String(" s");

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


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="IRComponent" componentName="IRComponent"
                 parentClasses="public Component, public ChangeNotifier::Listener"
                 constructorParams="UIUtils::Theme colours" variableInitialisers=""
                 snapPixels="4" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="462" initialHeight="176">
  <BACKGROUND backgroundColour="ffb0b0b6"/>
  <GENERICCOMPONENT name="WaveformComponent" id="c9f33b0ee0917f49" memberName="_waveformComponent"
                    virtualName="" explicitFocusOrder="0" pos="4 4 454 140" class="WaveformComponent"
                    params="colours"/>
  <TEXTBUTTON name="LoadButton" id="5798b8525a699c54" memberName="_loadButton"
              virtualName="" explicitFocusOrder="0" pos="80 148 378 24" tooltip="Current impulse response information"
              bgColOff="bbbbff" bgColOn="4444ff" textCol="ff202020" textColOn="ff202020"
              buttonText="No Impulse Response" connectedEdges="3" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

