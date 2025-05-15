#pragma once

#include "JuceHeader.h"

#include "signalsmith-stretch.h"
#include "RichterLFO/RichterLFO.h"


class ChorusVoice {
public:
    ChorusVoice();
    ~ChorusVoice() = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioSampleBuffer& buffer, double bpm, double timeInSeconds);
    void setFrequency(float freq) { _lfo.setFreq(freq); }
    void setDepth(float depth) { _lfo.setDepth(depth); }
    void setPhaseOffset(float offset) { _lfo.setManualPhase(offset); }
    void reset();

private:
    WECore::Richter::RichterLFO _lfo;
    signalsmith::stretch::SignalsmithStretch<float> _stretch;
    juce::AudioSampleBuffer _stretchInputBuffer;
};

class Chorus {
public:
    Chorus();
    ~Chorus() = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void processBlock(juce::AudioSampleBuffer& buffer, double bpm, double timeInSeconds);
    void reset();

    void setWetGain(double wetGain) { _wetGain = wetGain; }
    void setFrequency(float freq);
    void setDepth(float depth);
    void setWidth(float width) { _rightVoice.setPhaseOffset(width * 180.0f); }

private:
    double _wetGain;

    ChorusVoice _leftVoice;
    ChorusVoice _rightVoice;
    juce::AudioSampleBuffer _dryBuffer;
};
