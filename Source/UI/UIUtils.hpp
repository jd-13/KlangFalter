#pragma once

#include <JuceHeader.h>

namespace UIUtils {
    inline constexpr float NOMINAL_WIDTH {654};
    inline constexpr float NOMINAL_HEIGHT {431};

    struct Theme {
        juce::String productName;
        juce::String presetExtension;
        juce::Colour background;
        juce::Colour neutral;
        juce::Colour highlight;
        juce::Colour waveformContainerBackground;
        juce::Colour waveformContainerNeutral;
        juce::Colour subtitle;
        juce::Colour meters;
        juce::Colour waveform;
        juce::Colour irBrowserHighlight;
    };

    Theme LoadTheme();

    class ToggleButtonLookAndFeel : public juce::LookAndFeel_V2 {
    public:
        enum ColourIds {
            onColour,
            offColour
        };

        void drawButtonBackground(juce::Graphics& g,
                                  juce::Button& button,
                                  const juce::Colour& backgroundColour,
                                  bool isMouseOverButton,
                                  bool isButtonDown) override;

        void drawButtonText(juce::Graphics& g,
                            juce::TextButton& textButton,
                            bool isMouseOverButton,
                            bool isButtonDown) override;
    };

    class SimpleButtonLookAndFeel : public juce::LookAndFeel_V2 {
    public:
        void drawButtonText(juce::Graphics& g,
                            juce::TextButton& textButton,
                            bool isMouseOverButton,
                            bool isButtonDown) override;
    };

    class RotarySliderLookAndFeel : public juce::LookAndFeel_V2 {
    public:
        Theme theme;

        void drawRotarySlider(juce::Graphics& g,
                              int x,
                              int y,
                              int width,
                              int height,
                              float sliderPosProportional,
                              float rotaryStartAngle,
                              float rotaryEndAngle,
                              juce::Slider &slider) override;
    };

    class LinearSliderLookAndFeel : public juce::LookAndFeel_V2 {
       void drawLinearSlider(juce::Graphics& g,
                             int x,
                             int y,
                             int width,
                             int height,
                             float sliderPos,
                             float minSliderPos,
                             float maxSliderPos,
                             const juce::Slider::SliderStyle style,
                             juce::Slider& slider) override;

        void drawLinearSliderThumb(juce::Graphics& g,
                                   int x,
                                   int y,
                                   int width,
                                   int height,
                                   float sliderPos,
                                   float minSliderPos,
                                   float maxSliderPos,
                                   const juce::Slider::SliderStyle style,
                                   juce::Slider& slider) override;

        void drawLinearSliderBackground(juce::Graphics& g,
                                        int x,
                                        int y,
                                        int width,
                                        int height,
                                        float sliderPos,
                                        float minSliderPos,
                                        float maxSliderPos,
                                        const juce::Slider::SliderStyle style,
                                        juce::Slider& slider) override;
    };

    class FileTreeLookAndFeel : public juce::LookAndFeel_V2 {
    public:
        void drawFileBrowserRow(juce::Graphics& g,
                                int width,
                                int height,
                                const juce::File& file,
                                const juce::String& filename,
                                juce::Image* icon,
                                const juce::String& fileSizeDescription,
                                const juce::String& fileTimeDescription,
                                bool isDirectory,
                                bool isItemSelected,
                                int itemIndex,
                                juce::DirectoryContentsDisplayComponent& dcc) override;
    };

    class IRDirectionButtons : public juce::Component {
    public:
        IRDirectionButtons(std::function<void()> onForwardClick,
                           std::function<void()> onReverseClick,
                           std::function<bool()> getForwardState,
                           std::function<bool()> getReverseState,
                           Theme& theme);
        virtual ~IRDirectionButtons() override;

        void resized() override;

    private:
        std::function<void()> _onForwardClick;
        std::function<void()> _onReverseClick;

        UIUtils::ToggleButtonLookAndFeel _buttonLookAndFeel;

        std::unique_ptr<juce::TextButton> _forwardButton;
        std::unique_ptr<juce::TextButton> _reverseButton;
    };

    class ScaleUnitButtons : public juce::Component {
    public:
        ScaleUnitButtons(std::function<void()> onSecClick,
                           std::function<void()> onBPMClick,
                           std::function<bool()> getSecState,
                           std::function<bool()> getBPMState,
                           Theme& theme);
        virtual ~ScaleUnitButtons() override;

        void resized() override;

    private:
        std::function<void()> _onSecClick;
        std::function<void()> _onBPMClick;

        UIUtils::ToggleButtonLookAndFeel _buttonLookAndFeel;

        std::unique_ptr<juce::TextButton> _secButton;
        std::unique_ptr<juce::TextButton> _BPMButton;
    };
}
