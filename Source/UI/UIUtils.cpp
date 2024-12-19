#include "UIUtils.hpp"

namespace {
    constexpr long double LONG_PI {3.14159265358979323846264338327950288};
    constexpr double DOUBLE_PI {static_cast<double>(LONG_PI)};

    constexpr long double LONG_TAU {2 * LONG_PI};
    constexpr double DOUBLE_TAU {static_cast<double>(LONG_TAU)};

    float scaled(int currentParentWidth, float value) {
        constexpr float NOMINAL_WIDTH {760};
        return currentParentWidth / (NOMINAL_WIDTH / value);
    }
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
                              static_cast<float>(cornerSize),
                              static_cast<float>(cornerSize),
                              true,
                              true,
                              !button.isConnectedOnBottom(),
                              !button.isConnectedOnBottom());

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

        g.setFont(g.getCurrentFont().withHeight(scaled(textButton.getParentWidth(), 14.0f)));

        constexpr int MARGIN {0};
        g.drawFittedText(textButton.getButtonText(),
                        MARGIN,
                        0,
                        textButton.getWidth() - 2 * MARGIN,
                        textButton.getHeight(),
                        juce::Justification::centred,
                        0);
    }

    void SimpleButtonLookAndFeel::drawButtonText(juce::Graphics& g,
                                                 juce::TextButton& textButton,
                                                 bool isMouseOverButton,
                                                 bool isButtonDown) {
        g.setColour(textButton.findColour(juce::TextButton::textColourOnId));
        g.setFont(g.getCurrentFont().withHeight(scaled(textButton.getTopLevelComponent()->getWidth(), 14.0f)));

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

        if (slider.isEnabled()) {
            g.setColour(highlightColour);
        } else {
            g.setColour(neutralColour.withAlpha(0.2f));
        }

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

    void LinearSliderLookAndFeel::drawLinearSlider(juce::Graphics& g,
                                                   int x,
                                                   int y,
                                                   int width,
                                                   int height,
                                                   float sliderPos,
                                                   float minSliderPos,
                                                   float maxSliderPos,
                                                   const juce::Slider::SliderStyle style,
                                                   juce::Slider& slider) {
        drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }

    void LinearSliderLookAndFeel::drawLinearSliderThumb(juce::Graphics& g,
                                                        int x,
                                                        int y,
                                                        int width,
                                                        int height,
                                                        float sliderPos,
                                                        float minSliderPos,
                                                        float maxSliderPos,
                                                        const juce::Slider::SliderStyle style,
                                                        juce::Slider& slider) {
        constexpr int THUMB_RADIUS {4};

        g.setColour(slider.findColour(juce::Slider::thumbColourId));

        if (style == juce::Slider::LinearHorizontal) {
            // Horizontal thumb
            g.fillEllipse(sliderPos - THUMB_RADIUS,
                          y + height / 2 - THUMB_RADIUS,
                          2 * THUMB_RADIUS,
                          2 * THUMB_RADIUS);
        } else {
            // Vertical thumb
            g.fillEllipse(x + width / 2 - THUMB_RADIUS,
                          sliderPos - THUMB_RADIUS,
                          2 * THUMB_RADIUS,
                          2 * THUMB_RADIUS);
        }
    }

    void LinearSliderLookAndFeel::drawLinearSliderBackground(juce::Graphics& g,
                                                             int x,
                                                             int y,
                                                             int width,
                                                             int height,
                                                             float sliderPos,
                                                             float minSliderPos,
                                                             float maxSliderPos,
                                                             const juce::Slider::SliderStyle style,
                                                             juce::Slider& slider) {
        constexpr int WIDTH {2};

        g.setColour(slider.findColour(juce::Slider::trackColourId));

        if (style == juce::Slider::LinearHorizontal) {
            // Horizontal slider
            g.fillRect(x, y + height / 2 - WIDTH / 2, width, WIDTH);
        } else {
            // Vertical slider
            g.fillRect(x + width / 2 - WIDTH / 2, y, WIDTH, height);
        }
    }

    // Copied from LookAndFeel_V2 with modifications
    void FileTreeLookAndFeel::drawFileBrowserRow(juce::Graphics& g,
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
                                                 juce::DirectoryContentsDisplayComponent& dcc) {
        auto fileListComp = dynamic_cast<Component*> (&dcc);

        if (isItemSelected)
            g.fillAll (fileListComp != nullptr ? fileListComp->findColour (DirectoryContentsDisplayComponent::highlightColourId)
                                            : findColour (DirectoryContentsDisplayComponent::highlightColourId));

        const int x = 32;
        g.setColour (Colours::black);

        if (icon != nullptr && icon->isValid())
        {
            g.drawImageWithin (*icon, 2, 2, x - 4, height - 4,
                            RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
                            false);
        }
        else
        {
            if (auto* d = isDirectory ? getDefaultFolderImage()
                                    : getDefaultDocumentFileImage())
                d->drawWithin (g, Rectangle<float> (2.0f, 2.0f, x - 4.0f, (float) height - 4.0f),
                            RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
        }

        if (isItemSelected)
            g.setColour (fileListComp != nullptr ? fileListComp->findColour (DirectoryContentsDisplayComponent::highlightedTextColourId)
                                                : findColour (DirectoryContentsDisplayComponent::highlightedTextColourId));
        else
            g.setColour (fileListComp != nullptr ? fileListComp->findColour (DirectoryContentsDisplayComponent::textColourId)
                                                : findColour (DirectoryContentsDisplayComponent::textColourId));

        g.setFont ((float) height * 0.7f);

        g.drawFittedText (filename,
                            x, 0, width - x, height,
                            Justification::centredLeft, 1);
    }
}
