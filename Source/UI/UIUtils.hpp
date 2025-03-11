#pragma once

#include <JuceHeader.h>
#include "../IRType.hpp"
#include "Theme.hpp"

namespace UIUtils {
    const juce::Colour neutralColour(0xffb0b0b6);
    const juce::Colour backgroundColour(0xff313131);
    const juce::Colour complementaryColour(0xff645e9d);

    class ToggleButtonLookAndFeel : public juce::LookAndFeel_V2 {
    public:
        ToggleButtonLookAndFeel(std::function<juce::Colour()> getOnColour,
                                std::function<juce::Colour()> getOffColour) :
            _getOnColour(getOnColour),
            _getOffColour(getOffColour) {}

        void drawButtonBackground(juce::Graphics& g,
                                  juce::Button& button,
                                  const juce::Colour& backgroundColour,
                                  bool isMouseOverButton,
                                  bool isButtonDown) override;

        void drawButtonText(juce::Graphics& g,
                            juce::TextButton& textButton,
                            bool isMouseOverButton,
                            bool isButtonDown) override;

    private:
        std::function<juce::Colour()> _getOnColour;
        std::function<juce::Colour()> _getOffColour;
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
        RotarySliderLookAndFeel(Theme& theme) : _theme(theme) {}

        void drawRotarySlider(juce::Graphics& g,
                              int x,
                              int y,
                              int width,
                              int height,
                              float sliderPosProportional,
                              float rotaryStartAngle,
                              float rotaryEndAngle,
                              juce::Slider &slider) override;

    private:
        Theme& _theme;
    };

    class LinearSliderLookAndFeel : public juce::LookAndFeel_V2 {
    public:
        LinearSliderLookAndFeel(Theme& theme) : _theme(theme) {}

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

    private:
        Theme& _theme;
    };

    class FileTreeLookAndFeel : public juce::LookAndFeel_V2 {
    public:
        FileTreeLookAndFeel(Theme& theme) : _theme(theme) {}

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
    private:
        Theme& _theme;
    };
}
