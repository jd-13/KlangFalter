// ==================================================================================
// Copyright (c) 2012 HiFi-LoFi
//
// This is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// ==================================================================================

#include "IRBrowserComponent.h"

#include "../IRAgent.h"
#include "../Settings.h"
#include "UIUtils.hpp"
#include "../IRFileUtils.hpp"

IRBrowserComponent::IRBrowserComponent() :
  juce::Component(),
  _timeSliceThread(),
  _fileFilter(),
  _directoryContent(),
  _fileTreeComponent(),
  _infoLabel(),
  _processor(nullptr),
  _fileTreeLookAndFeel(new UIUtils::FileTreeLookAndFeel())
{
}


IRBrowserComponent::~IRBrowserComponent()
{
  if (_processor)
  {
    _processor->getSettings().removeChangeListener(this);
  }

  _processor = nullptr;
  _fileTreeComponent = nullptr;
  _directoryContent = nullptr;
  _fileFilter = nullptr;
  _timeSliceThread = nullptr;
  _fileTreeLookAndFeel = nullptr;
}


void IRBrowserComponent::init(Processor* processor, UIUtils::Theme theme)
{
  _processor = processor;
  _theme = theme;

  Settings* settings = _processor ? &(_processor->getSettings()) : nullptr;
  if (settings)
  {
    settings->addChangeListener(this);
  }

  if (!_timeSliceThread)
  {
    _timeSliceThread.reset(new juce::TimeSliceThread("IRBrowserThread"));
    _timeSliceThread->startThread();
  }

  juce::AudioFormatManager formatManager;
  formatManager.registerBasicFormats();
  _fileFilter.reset(new juce::WildcardFileFilter(formatManager.getWildcardForAllFormats(),
                                                                      "*",
                                                                      "Audio Files"));

  _directoryContent.reset(new juce::DirectoryContentsList(_fileFilter.get(), *_timeSliceThread));
  _directoryContent->setDirectory(settings ? settings->getImpulseResponseDirectory() : juce::File(), true, true);

  _fileTreeComponent.reset(new juce::FileTreeComponent(*_directoryContent));
  _fileTreeComponent->addListener(this);
  _fileTreeComponent->setColour(juce::TreeView::backgroundColourId, _theme.neutral);
  _fileTreeComponent->setColour(juce::TreeView::linesColourId, _theme.background);
  _fileTreeComponent->setColour(juce::DirectoryContentsDisplayComponent::highlightColourId, _theme.irBrowserHighlight.withAlpha(0.5f));
  _fileTreeComponent->setColour(juce::DirectoryContentsDisplayComponent::textColourId, _theme.background);
  _fileTreeComponent->setLookAndFeel(_fileTreeLookAndFeel.get());
  addAndMakeVisible(_fileTreeComponent.get());

  _infoLabel.reset(new juce::Label());
  _infoLabel->setColour(juce::Label::backgroundColourId, _theme.neutral);
  _infoLabel->setColour(juce::Label::textColourId, _theme.background);
  addAndMakeVisible(_infoLabel.get());

  updateLayout();
}


void IRBrowserComponent::updateLayout()
{
  auto scaled = [&](float value) {
    constexpr float NOMINAL_WIDTH {736};
    return getWidth() / (NOMINAL_WIDTH / value);
  };

  if (_fileTreeComponent && _infoLabel)
  {
    const int width = getWidth();
    const int height = getHeight();

    const int treeWidth = std::min(static_cast<int>(0.75 * static_cast<double>(width)), static_cast<int>(width - scaled(280))) - scaled(2);
    const int treeHeight = height - scaled(2);
    const int infoMargin = scaled(8);
    const int infoX = treeWidth + infoMargin;
    const int infoWidth = width - (infoX + infoMargin);
    const int infoHeight = height - (2 * infoMargin);

    _fileTreeComponent->setBounds(1, 1, treeWidth, treeHeight);
    _infoLabel->setBounds(infoX, infoMargin, infoWidth, infoHeight);

    _fileTreeComponent->setItemHeight(scaled(22));

    auto setFontHeight = [](juce::Label* label, float height) {
        label->setFont(label->getFont().withHeight(height));
    };

    setFontHeight(_infoLabel.get(), scaled(15.0f));
  }
}


void IRBrowserComponent::paint(juce::Graphics& g)
{
  if (_fileTreeComponent && _infoLabel)
  {
    const int width = getWidth();
    const int height = getHeight();

    g.setColour(_theme.neutral);
    g.fillRect(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));

    g.setColour(juce::Colours::grey);
    g.drawRect(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
    g.drawVerticalLine(_fileTreeComponent->getX()+_fileTreeComponent->getWidth(), 0.0f, static_cast<float>(height-1));
  }
}


void IRBrowserComponent::resized()
{
  updateLayout();
}


void IRBrowserComponent::selectionChanged()
{
  if (_infoLabel)
  {
    juce::String infoText;

    const juce::File file = _fileTreeComponent ? _fileTreeComponent->getSelectedFile() : juce::File();

    if (!file.isDirectory() && _processor)
    {
      size_t channelCount = 0;
      size_t sampleCount = 0;
      double sampleRate = 0.0;
      if (readAudioFileInfo(file, channelCount, sampleCount, sampleRate))
      {
        infoText += juce::String("Name: ") + file.getFileName();
        infoText += juce::String("\n");
        infoText += juce::String("\nSamples: ") + juce::String(static_cast<int>(sampleCount));
        if (sampleRate > 0.00001)
        {
          const double seconds = static_cast<double>(sampleCount) / sampleRate;
          infoText += juce::String("\nDuration: ") + juce::String(seconds, 2) + juce::String("s");
        }
        infoText += juce::String("\nChannels: ") + juce::String(static_cast<int>(channelCount));
        infoText += juce::String("\nSample Rate: ") + juce::String(static_cast<int>(sampleRate)) + juce::String("Hz");

        if (_processor->getTotalNumInputChannels() >= 2 && _processor->getTotalNumOutputChannels() >= 2)
        {
          const TrueStereoPairs trueStereoPairs = findTrueStereoPairs(file, sampleCount, sampleRate);
          for (size_t i=0; i<trueStereoPairs.size(); ++i)
          {
            if (trueStereoPairs[i].first != file && trueStereoPairs[i].first.existsAsFile())
            {
              infoText += juce::String("\n");
              infoText += juce::String("\nFile Pair For True-Stereo:");
              infoText += juce::String("\n - ") + file.getFileName();
              infoText += juce::String("\n - ") + trueStereoPairs[i].first.getFileName();
              break;
            }
          }
        }
      }
      else
      {
        infoText += juce::String("\n\nError!\n\nNo information available.");
      }
    }

    _infoLabel->setJustificationType(juce::Justification(juce::Justification::topLeft));
    _infoLabel->setText(infoText, juce::sendNotification);
  }
}


void IRBrowserComponent::fileClicked(const File&, const MouseEvent&)
{
}


void IRBrowserComponent::fileDoubleClicked(const File &file)
{
  if (_processor != nullptr) {
    _processor->selectIR(file);
  }
}


void IRBrowserComponent::browserRootChanged(const File&)
{
}


void IRBrowserComponent::changeListenerCallback(juce::ChangeBroadcaster*)
{
  if (_directoryContent && _processor)
  {
    _directoryContent->setDirectory(_processor->getSettings().getImpulseResponseDirectory(), true, true);
  }
}
