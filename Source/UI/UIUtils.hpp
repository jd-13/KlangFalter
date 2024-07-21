#pragma once

#include <JuceHeader.h>

namespace UIUtils {
    const juce::Colour highlightColour(0xfffcf64e);
    const juce::Colour neutralColour(0xffb0b0b6);
    const juce::Colour backgroundColour(0xff313131);

    class ToggleButtonLookAndFeel : public juce::LookAndFeel_V2 {
    public:
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
}