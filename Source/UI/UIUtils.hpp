#pragma once

#include <JuceHeader.h>

namespace UIUtils {
    const juce::Colour highlightColour(0xfffcf64e);
    const juce::Colour neutralColour(0xffb0b0b6);
    const juce::Colour backgroundColour(0xff313131);
    const juce::Colour complementaryColour(0xff645e9d);

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
}