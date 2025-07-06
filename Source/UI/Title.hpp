#pragma once

#include <JuceHeader.h>

#include "UIUtils.hpp"

class Title : public juce::Component {
public:
    Title(UIUtils::Theme& theme, juce::AudioProcessor::WrapperType pluginFormat);
    ~Title() = default;

    void resized() override;

private:
    UIUtils::Theme& _theme;
    std::unique_ptr<juce::Label> _titleLabel;
    std::unique_ptr<juce::Label> _subtitleLabel;
    std::unique_ptr<juce::TextButton> _creditsButton;
    std::unique_ptr<juce::DialogWindow::LaunchOptions> _creditsWindowOptions;
    juce::String _versionString;
};
