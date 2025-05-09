#pragma once

#include "JuceHeader.h"

#include "signalsmith-stretch.h"
#include "RichterLFO/RichterLFO.h"

class Chorus {
public:
    Chorus();
    ~Chorus() = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void reset();

    void processBlock(juce::AudioSampleBuffer& buffer, double bpm, double timeInSeconds);

    void setWetGain(double wetGain) { _wetGain = wetGain; }
    double getWetGain() const { return _wetGain; }

    void setFrequency(float freq) { _lfo.setFreq(freq); }
    float getFrequency() { return _lfo.getFreq(); }

    void setDepth(float depth) { _lfo.setDepth(depth); }
    float getDepth() { return _lfo.getDepth(); }

private:
    double _wetGain;

    WECore::Richter::RichterLFO _lfo;
    signalsmith::stretch::SignalsmithStretch<float> _stretch;
    juce::AudioSampleBuffer _dryBuffer;
    juce::AudioSampleBuffer _stretchInputBuffer;
};
