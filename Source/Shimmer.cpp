#include "Shimmer.hpp"

Shimmer::Shimmer() :
        _wetGain(0.0f),
        _feedback(0.0f),
        _delayLine(22050),
        _dryBuffer(1, 0),
        _stretchInputBuffer(1, 0) {
    _delayLine.setDelay(22050);
    _stretch.setTransposeSemitones(12);
}

void Shimmer::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels) {
    juce::dsp::ProcessSpec spec {sampleRate, static_cast<juce::uint32>(samplesPerBlock), static_cast<uint32>(numChannels)};
    _delayLine.prepare(spec);

    _stretch.presetCheaper(numChannels, sampleRate);

    _dryBuffer.setSize(numChannels, samplesPerBlock);
    _stretchInputBuffer.setSize(numChannels, samplesPerBlock);
}

void Shimmer::processBlock(juce::AudioSampleBuffer& buffer) {
    _dryBuffer.makeCopyOf(buffer, true);
    _stretchInputBuffer.makeCopyOf(buffer, true);

    // Add delay output to the buffer
    // TODO this won't work properly for mono in stereo out
    for (int channel {0}; channel < _stretchInputBuffer.getNumChannels(); channel++) {
        for (int sampleIndex {0}; sampleIndex < _stretchInputBuffer.getNumSamples(); sampleIndex++) {
            _stretchInputBuffer.getWritePointer(channel)[sampleIndex] += _delayLine.popSample(channel);
        }
    }

    _stretch.process(_stretchInputBuffer.getArrayOfReadPointers(), _stretchInputBuffer.getNumSamples(), buffer.getArrayOfWritePointers(), buffer.getNumSamples());

    // Store the output in the delay line
    for (int channel {0}; channel < buffer.getNumChannels(); channel++) {
        for (int sampleIndex {0}; sampleIndex < buffer.getNumSamples(); sampleIndex++) {
            _delayLine.pushSample(channel, buffer.getReadPointer(channel)[sampleIndex] * _feedback);
        }
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

void Shimmer::reset() {
    _delayLine.reset();
    _stretch.reset();
    _dryBuffer.clear();
    _stretchInputBuffer.clear();
}
