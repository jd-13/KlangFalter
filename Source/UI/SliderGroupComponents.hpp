#pragma once

#include <JuceHeader.h>

#include "../Processor.h"
#include "UIUtils.hpp"

class IRSliderGroup : public juce::Component {
public:
    IRSliderGroup(Processor& processor, UIUtils::Theme theme);
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
};

class TuneSliderGroup : public juce::Component {
public:
    TuneSliderGroup(Processor& processor, UIUtils::Theme theme);
    virtual ~TuneSliderGroup();

    void resized() override;

    void onUpdate(bool enableSliders);

private:
    Processor& _processor;

    std::unique_ptr<UIUtils::RotarySliderLookAndFeel> _rotarySliderLookAndFeel;

    std::unique_ptr<juce::Label> _tuneHeaderLabel;

    std::unique_ptr<juce::Label> _amountHeaderLabel;
    std::unique_ptr<juce::Slider> _amountSlider;
    std::unique_ptr<juce::Label> _amountLabel;
};

class AttackSliderGroup : public juce::Component {
public:
    AttackSliderGroup(Processor& processor, UIUtils::Theme theme);
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
    DecaySliderGroup(Processor& processor, UIUtils::Theme theme);
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
    StereoSliderGroup(Processor& processor, UIUtils::Theme theme);
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

class LowEqSliderGroup : public juce::Component {
public:
    LowEqSliderGroup(Processor& processor, UIUtils::Theme theme);
    virtual ~LowEqSliderGroup();

    void resized() override;

    void onUpdate(bool enableSliders);

private:
    Processor& _processor;

    std::unique_ptr<UIUtils::RotarySliderLookAndFeel> _rotarySliderLookAndFeel;
    std::unique_ptr<UIUtils::SimpleButtonLookAndFeel> _simpleButtonLookAndFeel;

    std::unique_ptr<juce::TextButton> _lowEqButton;

    std::unique_ptr<juce::Label> _lowCutFreqHeaderLabel;
    std::unique_ptr<juce::Slider> _lowCutFreqSlider;
    std::unique_ptr<juce::Label> _lowCutFreqLabel;

    std::unique_ptr<juce::Label> _loFreqHeaderLabel;
    std::unique_ptr<juce::Slider> _loFreqSlider;
    std::unique_ptr<juce::Label> _loFreqLabel;

    std::unique_ptr<juce::Label> _loGainHeaderLabel;
    std::unique_ptr<juce::Slider> _loGainSlider;
    std::unique_ptr<juce::Label> _loGainLabel;
};

class HighEqSliderGroup : public juce::Component {
public:
    HighEqSliderGroup(Processor& processor, UIUtils::Theme theme);
    virtual ~HighEqSliderGroup();

    void resized() override;

    void onUpdate(bool enableSliders);

private:
    Processor& _processor;

    std::unique_ptr<UIUtils::RotarySliderLookAndFeel> _rotarySliderLookAndFeel;
    std::unique_ptr<UIUtils::SimpleButtonLookAndFeel> _simpleButtonLookAndFeel;

    std::unique_ptr<juce::TextButton> _highEqButton;

    std::unique_ptr<juce::Label> _highCutFreqHeaderLabel;
    std::unique_ptr<juce::Slider> _highCutFreqSlider;
    std::unique_ptr<juce::Label> _highCutFreqLabel;

    std::unique_ptr<juce::Label> _hiFreqHeaderLabel;
    std::unique_ptr<juce::Slider> _hiFreqSlider;
    std::unique_ptr<juce::Label> _hiFreqLabel;

    std::unique_ptr<juce::Label> _hiGainHeaderLabel;
    std::unique_ptr<juce::Slider> _hiGainSlider;
    std::unique_ptr<juce::Label> _hiGainLabel;
};

// class ShimmerSliderGroup : public juce::Component {
// public:
//     ShimmerSliderGroup(Processor& processor, UIUtils::Theme theme);
//     virtual ~ShimmerSliderGroup();

//     void resized() override;

//     void onUpdate(bool enableSliders);

// private:
//     Processor& _processor;

//     std::unique_ptr<UIUtils::RotarySliderLookAndFeel> _rotarySliderLookAndFeel;

//     std::unique_ptr<juce::Label> _shimmerHeaderLabel;

//     std::unique_ptr<juce::Label> _shimmerWetGainHeaderLabel;
//     std::unique_ptr<juce::Slider> _shimmerWetGainSlider;
//     std::unique_ptr<juce::Label> _shimmerWetGainLabel;

//     std::unique_ptr<juce::Label> _shimmerFeedbackHeaderLabel;
//     std::unique_ptr<juce::Slider> _shimmerFeedbackSlider;
//     std::unique_ptr<juce::Label> _shimmerFeedbackLabel;
// };

class ChorusSliderGroup : public juce::Component {
public:
    ChorusSliderGroup(Processor& processor, UIUtils::Theme theme);
    virtual ~ChorusSliderGroup();

    void resized() override;

    void onUpdate(bool enableSliders, int numOutputChannels);

private:
    Processor& _processor;

    std::unique_ptr<UIUtils::RotarySliderLookAndFeel> _rotarySliderLookAndFeel;

    std::unique_ptr<juce::Label> _chorusHeaderLabel;

    std::unique_ptr<juce::Label> _chorusWetGainHeaderLabel;
    std::unique_ptr<juce::Slider> _chorusWetGainSlider;
    std::unique_ptr<juce::Label> _chorusWetGainLabel;

    std::unique_ptr<juce::Label> _chorusFrequencyHeaderLabel;
    std::unique_ptr<juce::Slider> _chorusFrequencySlider;
    std::unique_ptr<juce::Label> _chorusFrequencyLabel;

    std::unique_ptr<juce::Label> _chorusDepthHeaderLabel;
    std::unique_ptr<juce::Slider> _chorusDepthSlider;
    std::unique_ptr<juce::Label> _chorusDepthLabel;
};
