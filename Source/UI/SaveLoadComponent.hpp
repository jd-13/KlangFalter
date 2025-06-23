#pragma once

#include <JuceHeader.h>

#include "../Processor.h"
#include "UIUtils.hpp"

class SaveLoadComponent : public juce::Component {
public:
    SaveLoadComponent(Processor& processor, UIUtils::Theme& theme);
    ~SaveLoadComponent() override;

    void resized() override;

    void refresh();

private:
    std::unique_ptr<UIUtils::ToggleButtonLookAndFeel> _toggleButtonLookAndFeel;
    std::unique_ptr<juce::TextButton> _resetButton;
    std::unique_ptr<juce::TextButton> _saveButton;
    std::unique_ptr<juce::TextButton> _loadButton;
    std::unique_ptr<juce::Label> _nameLabel;

    std::unique_ptr<juce::FileChooser> _fileChooser;

    Processor& _processor;
    UIUtils::Theme& _theme;

    void _onSaveToFile(juce::File file);
    void _onLoadFromFile(juce::File file);
};