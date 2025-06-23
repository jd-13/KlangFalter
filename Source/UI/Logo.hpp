#pragma once

#include <JuceHeader.h>

class Logo : public juce::Component {
public:
    Logo(juce::Image logo) : _logo(logo) {}

    ~Logo() = default;

    void paint(juce::Graphics& g) override {
        g.drawImageWithin(_logo,
                          0, 0, getWidth(), getHeight(),
                          RectanglePlacement::centred);
    }

private:
    juce::Image _logo;
};
