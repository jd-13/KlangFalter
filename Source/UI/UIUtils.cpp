#include "UIUtils.hpp"

namespace {
    constexpr long double LONG_PI {3.14159265358979323846264338327950288};
    constexpr double DOUBLE_PI {static_cast<double>(LONG_PI)};

    constexpr long double LONG_TAU {2 * LONG_PI};
    constexpr double DOUBLE_TAU {static_cast<double>(LONG_TAU)};
}

namespace UIUtils {
    void ToggleButtonLookAndFeel::drawButtonBackground(juce::Graphics& g,
                                                       juce::Button& button,
                                                       const juce::Colour& backgroundColour,
                                                       bool isMouseOverButton,
                                                       bool isButtonDown) {
        const int width {button.getWidth()};
        const int height {button.getHeight()};

        constexpr float indent {2.0f};
        const int cornerSize {juce::jmin(juce::roundToInt(width * 0.4f),
                                         juce::roundToInt(height * 0.4f))};

        juce::Path p;
        juce::PathStrokeType pStroke(1);

        if (button.getToggleState()) {
            g.setColour(button.findColour(onColour));
        } else {
            g.setColour(button.findColour(offColour));
        }

        p.addRoundedRectangle(indent,
                              indent,
                              width - 2 * indent,
                              height - 2 * indent,
                              static_cast<float>(cornerSize));

        g.strokePath(p, pStroke);
    }

    void ToggleButtonLookAndFeel::drawButtonText(juce::Graphics& g,
                                                 juce::TextButton& textButton,
                                                 bool isMouseOverButton,
                                                 bool isButtonDown) {
        if (textButton.getToggleState()) {
            g.setColour(textButton.findColour(onColour));
        } else {
            g.setColour(textButton.findColour(offColour));
        }

        constexpr int MARGIN {0};
        g.drawFittedText(textButton.getButtonText(),
                        MARGIN,
                        0,
                        textButton.getWidth() - 2 * MARGIN,
                        textButton.getHeight(),
                        juce::Justification::centred,
                        0);
    }

    void RotarySliderLookAndFeel::drawRotarySlider(juce::Graphics& g,
                                                   int /*x*/,
                                                   int /*y*/,
                                                   int width,
                                                   int height,
                                                   float /*sliderPosProportional*/,
                                                   float /*rotaryStartAngle*/,
                                                   float /*rotaryEndAngle*/,
                                                   juce::Slider &slider) {

        // Calculate useful constants
        constexpr double arcGap {DOUBLE_TAU / 4};
        constexpr double rangeOfMotion {DOUBLE_TAU - arcGap};

        const double sliderNormalisedValue {slider.valueToProportionOfLength(slider.getValue())};
        const double arcEndPoint {sliderNormalisedValue * rangeOfMotion + arcGap / 2};

        constexpr int margin {2};
        juce::Rectangle<int> area = slider.getBounds();
        area.reduce(margin, margin);
        const int diameter {std::min(area.getWidth(), area.getHeight())};

        g.setColour(highlightColour);

        juce::Path p;

        // Draw inner ring
        constexpr int arcSpacing {3};
        p.addCentredArc(width / 2,
                        height / 2,
                        diameter / 2 - arcSpacing,
                        diameter / 2 - arcSpacing,
                        DOUBLE_PI,
                        arcGap / 2,
                        DOUBLE_TAU - (arcGap / 2),
                        true);

        g.strokePath(p, juce::PathStrokeType(0.7f));

        // Draw outer ring
        p.clear();
        p.addCentredArc(width / 2,
                        height / 2,
                        diameter / 2,
                        diameter / 2,
                        DOUBLE_PI,
                        arcGap / 2,
                        arcEndPoint,
                        true);
        g.strokePath(p, juce::PathStrokeType(3.0f));
    }
}
