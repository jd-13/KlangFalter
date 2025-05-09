#include "Chorus.hpp"

Chorus::Chorus() :
        _wetGain(0.0f),
        _dryBuffer(1, 0),
        _stretchInputBuffer(1, 0) {
    _lfo.setBypassSwitch(true);
}

void Chorus::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels) {
    _stretch.presetCheaper(numChannels, sampleRate);

    _dryBuffer.setSize(numChannels, samplesPerBlock);
    _lfo.setSampleRate(sampleRate);
    _stretchInputBuffer.setSize(numChannels, samplesPerBlock);
}

void Chorus::processBlock(juce::AudioSampleBuffer& buffer, double bpm, double timeInSeconds) {
    _lfo.prepareForNextBuffer(bpm, timeInSeconds);

    _dryBuffer.makeCopyOf(buffer, true);
    _stretchInputBuffer.makeCopyOf(buffer, true);

    _stretch.setTransposeSemitones(_lfo.getLastOutput() * 1.0f);

    _stretch.process(_stretchInputBuffer.getArrayOfReadPointers(), _stretchInputBuffer.getNumSamples(), buffer.getArrayOfWritePointers(), buffer.getNumSamples());

    // Apply the gain to the output
    for (int channel {0}; channel < buffer.getNumChannels(); channel++) {
        juce::FloatVectorOperations::multiply(buffer.getWritePointer(channel), _wetGain, buffer.getNumSamples());
    }

    // Add the original signal to the output
    for (int channel {0}; channel < buffer.getNumChannels(); channel++) {
        juce::FloatVectorOperations::add(buffer.getWritePointer(channel), _dryBuffer.getReadPointer(channel), buffer.getNumSamples());
    }

    // Advance the LFO
    for (int sampleIndex {0}; sampleIndex < buffer.getNumSamples(); sampleIndex++) {
        _lfo.getNextOutput(0);
    }
}

void Chorus::reset() {
    _lfo.reset();
    _dryBuffer.clear();
    _stretchInputBuffer.clear();
}
