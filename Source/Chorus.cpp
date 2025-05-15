#include "Chorus.hpp"

namespace {
    constexpr float SHIFT_RANGE_SEMITONES {1.0f};
}

ChorusVoice::ChorusVoice() : _stretchInputBuffer(1, 0) {
    _lfo.setBypassSwitch(true);
}

void ChorusVoice::prepareToPlay(double sampleRate, int samplesPerBlock) {
    _stretch.presetCheaper(1, sampleRate);

    _lfo.setSampleRate(sampleRate);
    _stretchInputBuffer.setSize(1, samplesPerBlock);
}

void ChorusVoice::processBlock(juce::AudioSampleBuffer& buffer, double bpm, double timeInSeconds) {
    _lfo.prepareForNextBuffer(bpm, timeInSeconds);

    // Perform the pitch shift
    _stretchInputBuffer.makeCopyOf(buffer, true);

    _stretch.setTransposeSemitones(_lfo.getLastOutput() * SHIFT_RANGE_SEMITONES);
    _stretch.process(_stretchInputBuffer.getArrayOfReadPointers(), _stretchInputBuffer.getNumSamples(), buffer.getArrayOfWritePointers(), buffer.getNumSamples());

    // Advance the LFO
    for (int sampleIndex {0}; sampleIndex < buffer.getNumSamples(); sampleIndex++) {
        _lfo.getNextOutput(0);
    }
}

void ChorusVoice::reset() {
    _lfo.reset();
    _stretchInputBuffer.clear();
}

Chorus::Chorus() :
        _wetGain(0.0f),
        _dryBuffer(1, 0) {
}

void Chorus::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels) {
    _dryBuffer.setSize(numChannels, samplesPerBlock);
    _leftVoice.prepareToPlay(sampleRate, samplesPerBlock);
    _rightVoice.prepareToPlay(sampleRate, samplesPerBlock);
}

void Chorus::processBlock(juce::AudioSampleBuffer& buffer, double bpm, double timeInSeconds) {
    _dryBuffer.makeCopyOf(buffer, true);

    juce::AudioSampleBuffer leftBuffer(buffer.getArrayOfWritePointers(), 1, buffer.getNumSamples());
    _leftVoice.processBlock(leftBuffer, bpm, timeInSeconds);

    if (buffer.getNumChannels() > 1) {
        juce::AudioSampleBuffer rightBuffer(buffer.getArrayOfWritePointers() + 1, 1, buffer.getNumSamples());
        _rightVoice.processBlock(rightBuffer, bpm, timeInSeconds);
    }

    // Apply the gain to the output
    for (int channel {0}; channel < buffer.getNumChannels(); channel++) {
        juce::FloatVectorOperations::multiply(buffer.getWritePointer(channel), _wetGain, buffer.getNumSamples());
    }

    // Add the original signal to the output
    for (int channel {0}; channel < buffer.getNumChannels(); channel++) {
        juce::FloatVectorOperations::add(buffer.getWritePointer(channel), _dryBuffer.getReadPointer(channel), buffer.getNumSamples());
    }
}

void Chorus::reset() {
    _dryBuffer.clear();
    _leftVoice.reset();
    _rightVoice.reset();
}

void Chorus::setFrequency(float freq) {
    _leftVoice.setFrequency(freq);
    _rightVoice.setFrequency(freq);
}
void Chorus::setDepth(float depth) {
    _leftVoice.setDepth(depth);
    _rightVoice.setDepth(depth);
}
