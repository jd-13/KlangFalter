#pragma once

#include <JuceHeader.h>

bool readAudioFileInfo(const juce::File& file, size_t& channelCount, size_t& sampleCount, double& sampleRate);

typedef std::vector<std::pair<juce::File, size_t> > TrueStereoPairs;
TrueStereoPairs findTrueStereoPairs(const juce::File& file, size_t sampleCount, double sampleRate);
