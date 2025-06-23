#include "SaveLoadComponent.hpp"

#include "../Parameters.h"

namespace {
    float scaled(float width, float value) {
        return width / ((UIUtils::NOMINAL_WIDTH - 260) / value);
    };
}

SaveLoadComponent::SaveLoadComponent(Processor& processor, UIUtils::Theme& theme) :
        _toggleButtonLookAndFeel(new UIUtils::ToggleButtonLookAndFeel()),
_processor(processor), _theme(theme) {
    _resetButton.reset(new juce::TextButton(juce::String()));
    addAndMakeVisible(_resetButton.get());
    _resetButton->setTooltip(TRANS("Reset all parameters"));
    _resetButton->setButtonText(TRANS("Init"));
    _resetButton->setColour(UIUtils::ToggleButtonLookAndFeel::offColour, _theme.neutral);
    _resetButton->setColour(UIUtils::ToggleButtonLookAndFeel::onColour, _theme.neutral);
    _resetButton->setLookAndFeel(_toggleButtonLookAndFeel.get());
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

        // _processor.setParameterNotifyingHost(Parameters::ShimmerWetGain, Parameters::ShimmerWetGain.getDefaultValue());
        // _processor.setParameterNotifyingHost(Parameters::ShimmerFeedback, Parameters::ShimmerFeedback.getDefaultValue());

        _processor.setParameterNotifyingHost(Parameters::ChorusWetGain, Parameters::ChorusWetGain.getDefaultValue());
        _processor.setParameterNotifyingHost(Parameters::ChorusFrequency, Parameters::ChorusFrequency.getDefaultValue());
        _processor.setParameterNotifyingHost(Parameters::ChorusDepth, Parameters::ChorusDepth.getDefaultValue());
    };

    _saveButton.reset(new juce::TextButton(juce::String()));
    addAndMakeVisible(_saveButton.get());
    _saveButton->setTooltip(TRANS("Save all parameters to a preset file"));
    _saveButton->setButtonText(TRANS("Save"));
    _saveButton->setColour(UIUtils::ToggleButtonLookAndFeel::offColour, _theme.neutral);
    _saveButton->setColour(UIUtils::ToggleButtonLookAndFeel::onColour, _theme.neutral);
    _saveButton->setLookAndFeel(_toggleButtonLookAndFeel.get());
    _saveButton->onClick = [&]() {
        const int flags {juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::warnAboutOverwriting};
        _fileChooser.reset(new juce::FileChooser("Save Body and Soul Preset", juce::File(), "*.bas"));
        _fileChooser->launchAsync(flags, [&](const juce::FileChooser& chooser) {
            _onSaveToFile(chooser.getResult());
        });
    };

    _loadButton.reset(new juce::TextButton(juce::String()));
    addAndMakeVisible(_loadButton.get());
    _loadButton->setTooltip(TRANS("Load parameters from a preset file"));
    _loadButton->setButtonText(TRANS("Load"));
    _loadButton->setColour(UIUtils::ToggleButtonLookAndFeel::offColour, _theme.neutral);
    _loadButton->setColour(UIUtils::ToggleButtonLookAndFeel::onColour, _theme.neutral);
    _loadButton->setLookAndFeel(_toggleButtonLookAndFeel.get());
    _loadButton->onClick = [&]() {
        const int flags {juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::openMode};
        _fileChooser.reset(new juce::FileChooser("Load Body and Soul Preset", juce::File(), "*.bas"));
        _fileChooser->launchAsync(flags, [&](const juce::FileChooser& chooser) {
            _onLoadFromFile(chooser.getResult());
        });
    };

    _nameLabel.reset(new juce::Label("Name Label", "test preset name"));
    addAndMakeVisible(_nameLabel.get());
    _nameLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    _nameLabel->setJustificationType(juce::Justification::centredLeft);
    _nameLabel->setEditable(false, false, false);
    _nameLabel->setColour(juce::Label::textColourId, _theme.subtitle);
    _nameLabel->toBack();

}

SaveLoadComponent::~SaveLoadComponent() {
}

void SaveLoadComponent::resized() {
    juce::Rectangle<int> availableArea = getLocalBounds();

    juce::FlexBox flexBox;
    flexBox.flexDirection = juce::FlexBox::Direction::row;
    flexBox.flexWrap = juce::FlexBox::Wrap::wrap;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    flexBox.alignContent = juce::FlexBox::AlignContent::center;

    const int BUTTON_SPACE_WIDTH {static_cast<int>(scaled(getWidth(), 10))};
    const juce::FlexItem::Margin marginRight(0, BUTTON_SPACE_WIDTH, 0, 0);

    const int BUTTON_WIDTH {static_cast<int>(scaled(getWidth(), 52))};
    const int BUTTON_HEIGHT {static_cast<int>(scaled(getWidth(), 24))};

    flexBox.items.add(juce::FlexItem(*_resetButton.get()).withMinWidth(BUTTON_WIDTH).withMinHeight(BUTTON_HEIGHT).withMargin(marginRight));
    flexBox.items.add(juce::FlexItem(*_saveButton.get()).withMinWidth(BUTTON_WIDTH).withMinHeight(BUTTON_HEIGHT).withMargin(marginRight));
    flexBox.items.add(juce::FlexItem(*_loadButton.get()).withMinWidth(BUTTON_WIDTH).withMinHeight(BUTTON_HEIGHT).withMargin(marginRight));
    flexBox.items.add(juce::FlexItem(*_nameLabel.get()).withFlex(1).withMinHeight(BUTTON_HEIGHT));
    flexBox.performLayout(availableArea.toFloat());

    _nameLabel->setFont(_nameLabel->getFont().withHeight(_nameLabel->getHeight() * 0.8));
}

void SaveLoadComponent::_onSaveToFile(juce::File file) {
    std::unique_ptr<juce::XmlElement> element = _processor.writeToXml();
    element->writeTo(file);
}

void SaveLoadComponent::_onLoadFromFile(juce::File file) {
    if (file.existsAsFile()) {
        std::unique_ptr<juce::XmlElement> element = juce::XmlDocument::parse(file);
        if (element != nullptr) {
            _processor.restoreFromXml(std::move(element));
        }
    }
}
