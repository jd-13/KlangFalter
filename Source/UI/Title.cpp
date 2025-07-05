#include "Title.hpp"

namespace {
    juce::String buildVersionString(juce::AudioProcessor::WrapperType pluginFormat) {
        juce::String versionString = JucePlugin_Name;
        versionString += " ";
        versionString += JucePlugin_VersionString;

        // Format
        versionString += " ";
        versionString += juce::AudioProcessor::getWrapperTypeDescription(pluginFormat);

        // OS
        versionString += " ";
#if _WIN32
        versionString += "Win";
#elif __APPLE__
        versionString += "macOS";
#elif __linux__
        versionString += "Linux";
#else
    #error "Unknown OS"
#endif

        // Arch
        versionString += " ";
#if defined(__x86_64__) || defined(_M_AMD64)
        versionString += "x86_64";
#elif defined(__aarch64__) || defined(_M_ARM64)
        versionString += "arm64";
#else
    #error "Unknown arch"
#endif

        return versionString;
    }
}

Title::Title(UIUtils::Theme& theme, juce::AudioProcessor::WrapperType pluginFormat) : _theme(theme) {
    _titleLabel.reset(new juce::Label("Title Label", TRANS("Body & Soul:")));
    addAndMakeVisible(_titleLabel.get());
    _titleLabel->setFont(juce::Font(35.30f, juce::Font::plain).withTypefaceStyle("Regular"));
    _titleLabel->setJustificationType(juce::Justification::centred);
    _titleLabel->setEditable(false, false, false);
    _titleLabel->setColour(juce::Label::textColourId, _theme.neutral.withAlpha(0.5f));

    _subtitleLabel.reset(new juce::Label("Subtitle Label", TRANS("Intro")));
    addAndMakeVisible(_subtitleLabel.get());
    _subtitleLabel->setFont(juce::Font(35.30f, juce::Font::plain).withTypefaceStyle("Regular"));
    _subtitleLabel->setJustificationType(juce::Justification::centred);
    _subtitleLabel->setEditable(false, false, false);
    _subtitleLabel->setColour(juce::Label::textColourId, _theme.subtitle);
    _subtitleLabel->setText(_theme.productName, juce::dontSendNotification);

    _creditsButton.reset(new juce::TextButton(juce::String()));
    addAndMakeVisible(_creditsButton.get());
    _creditsButton->setColour(juce::TextButton::buttonColourId, juce::Colour(0x00bbbbff));
    _creditsButton->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0x002c2cff));
    _creditsButton->setColour(juce::TextButton::textColourOffId, juce::Colour(0xffb0b0b6));
    _creditsButton->setColour(juce::TextButton::textColourOnId, juce::Colour(0xffb0b0b6));

    _creditsWindowOptions.reset(new juce::DialogWindow::LaunchOptions());
    _creditsWindowOptions->dialogTitle = "Credits";
    _creditsWindowOptions->dialogBackgroundColour = _theme.background;
    _creditsWindowOptions->componentToCentreAround = this;
    _creditsWindowOptions->useNativeTitleBar = false;

    _versionString = buildVersionString(pluginFormat);

    _creditsButton->onClick = [&]() {
        const juce::String credits(juce::CharPointer_UTF8(
            "Concept and IR design: The Sound of Merlin\n"
            "Programming: White Elephant Audio\n"
            "Support: info@thesoundofmerlin.com\n"
            "\n"
            "Our Body & Soul plugins are open source software (GPLv3) and are based on Klangfalter from HiFi-LoFi.\n"
            "\n"
            "This convolution reverb was conceived to be used as a module in White Elephant Audio's plugin host Syndicate but can also be used separately from it.\n"
            "\n"
            "A special thank you goes to Eirik Gr\xc3\xb8nner from Demningen Studios in Norway for his wonderful suggestions and support.\n"
            "\n"));

        _creditsWindowOptions->content.set(new juce::Label("Credits", credits + _versionString), true);
        juce::DialogWindow* window = _creditsWindowOptions->launchAsync();
        window->centreWithSize(500, 300);
    };
}

void Title::resized() {
    juce::Rectangle<int> availableArea = getLocalBounds();

    const juce::Font titleFont = _titleLabel->getFont().withHeight(std::floor(availableArea.getHeight()));
    _titleLabel->setFont(titleFont);
    _subtitleLabel->setFont(titleFont);

    juce::FlexBox flexBox;
    flexBox.flexDirection = juce::FlexBox::Direction::row;
    flexBox.flexWrap = juce::FlexBox::Wrap::wrap;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
    flexBox.alignContent = juce::FlexBox::AlignContent::center;

    const int titleTextWidth {_titleLabel->getFont().getStringWidth(_titleLabel->getText())};
    const int subtitleTextWidth {_subtitleLabel->getFont().getStringWidth(_subtitleLabel->getText())};
    flexBox.items.add(juce::FlexItem(*_titleLabel.get()).withMinWidth(titleTextWidth).withMinHeight(getHeight()));
    flexBox.items.add(juce::FlexItem(*_subtitleLabel.get()).withMinWidth(subtitleTextWidth).withMinHeight(getHeight()));
    flexBox.performLayout(availableArea.toFloat());

    _creditsButton->setBounds(availableArea);
}
