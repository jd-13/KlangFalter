#pragma once

#include <JuceHeader.h>

#include "../Processor.h"
#include "UIUtils.hpp"

class IRSliderGroup : public juce::Component {
public:
    IRSliderGroup(Processor& processor);
    virtual ~IRSliderGroup();

    void resized() override;

    void onUpdate(bool enableSliders);

private:
    Processor& _processor;

    std::unique_ptr<UIUtils::RotarySliderLookAndFeel> _rotarySliderLookAndFeel;

    std::unique_ptr<juce::Label> _impulseResponseHeaderLabel;

    std::unique_ptr<juce::Label> _predelayHeaderLabel;
    std::unique_ptr<juce::Slider> _predelaySlider;
    std::unique_ptr<juce::Label> _predelayLabel;

    std::unique_ptr<juce::Label> _beginHeaderLabel;
    std::unique_ptr<juce::Slider> _beginSlider;
    std::unique_ptr<juce::Label> _beginLabel;

    std::unique_ptr<juce::Label> _endHeaderLabel;
    std::unique_ptr<juce::Slider> _endSlider;
    std::unique_ptr<juce::Label> _endLabel;

    std::unique_ptr<juce::Label> _stretchHeaderLabel;
    std::unique_ptr<juce::Slider> _stretchSlider;
    std::unique_ptr<juce::Label> _stretchLabel;
};

class AttackSliderGroup : public juce::Component {
public:
    AttackSliderGroup(Processor& processor);
    virtual ~AttackSliderGroup();

    void resized() override;

    void onUpdate(bool enableSliders);

private:
    Processor& _processor;

    std::unique_ptr<UIUtils::RotarySliderLookAndFeel> _rotarySliderLookAndFeel;

    std::unique_ptr<juce::Label> _attackHeaderLabel;

    std::unique_ptr<juce::Label> _attackShapeHeaderLabel;
    std::unique_ptr<juce::Slider> _attackShapeSlider;
    std::unique_ptr<juce::Label> _attackShapeLabel;

    std::unique_ptr<juce::Label> _attackLengthHeaderLabel;
    std::unique_ptr<juce::Slider> _attackLengthSlider;
    std::unique_ptr<juce::Label> _attackLengthLabel;
};

class DecaySliderGroup : public juce::Component {
public:
    DecaySliderGroup(Processor& processor);
    virtual ~DecaySliderGroup();

    void resized() override;

    void onUpdate(bool enableSliders);

private:
    Processor& _processor;

    std::unique_ptr<UIUtils::RotarySliderLookAndFeel> _rotarySliderLookAndFeel;

    std::unique_ptr<juce::Label> _decayHeaderLabel;

    std::unique_ptr<juce::Label> _decayShapeHeaderLabel;
    std::unique_ptr<juce::Slider> _decayShapeSlider;
    std::unique_ptr<juce::Label> _decayShapeLabel;
};

class StereoSliderGroup : public juce::Component {
public:
    StereoSliderGroup(Processor& processor);
    virtual ~StereoSliderGroup();

    void resized() override;

    void onUpdate(bool enableSliders, int numOutputChannels);

private:
    Processor& _processor;

    std::unique_ptr<UIUtils::RotarySliderLookAndFeel> _rotarySliderLookAndFeel;

    std::unique_ptr<juce::Label> _stereoHeaderLabel;

    std::unique_ptr<juce::Label> _widthHeaderLabel;
    std::unique_ptr<juce::Slider> _widthSlider;
    std::unique_ptr<juce::Label> _widthLabel;
};
