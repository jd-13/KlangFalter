#pragma once

#include "JuceHeader.h"

#include "signalsmith-stretch.h"

class Shimmer {
public:
    Shimmer();
    ~Shimmer() = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void reset();

    void processBlock(juce::AudioSampleBuffer& buffer);

    void setWetGain(double wetGain) { _wetGain = wetGain; }
    double getWetGain() const { return _wetGain; }

    void setFeedback(double feedback) { _feedback = feedback; }
    double getFeedback() const { return _feedback; }

private:
    double _wetGain;
    double _feedback;

    juce::dsp::DelayLine<float> _delayLine;
    signalsmith::stretch::SignalsmithStretch<float> _stretch;
    juce::AudioSampleBuffer _dryBuffer;
    juce::AudioSampleBuffer _stretchInputBuffer;
};