#pragma once

#include <JuceHeader.h>

namespace UIUtils {

    struct Theme {
        juce::String productName;
        juce::Colour background;
        juce::Colour neutral;
        juce::Colour highlight;
        juce::Colour complementary;
        juce::Colour waveformContainerBackground;
        juce::Colour waveformContainerNeutral;
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
}
