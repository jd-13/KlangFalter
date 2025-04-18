#include "IRFileUtils.hpp"

namespace {
    juce::File checkMatchingTrueStereoFile(
            const juce::String& fileNameBody,
            const juce::String& fileNameExt,
            const juce::File& directory,
            const juce::String& pattern,
            const juce::String& replacement,
            const size_t sampleCount,
            const double sampleRate) {

        std::vector<juce::String> candidateNames;
        if (fileNameBody.startsWith(pattern)) {
            candidateNames.push_back(replacement + fileNameBody.substring(pattern.length(), fileNameBody.length()) + fileNameExt);
        }

        if (fileNameBody.endsWith(pattern)) {
            candidateNames.push_back(fileNameBody.substring(0, fileNameBody.length()-pattern.length()) + replacement + fileNameExt);
        }

        for (size_t i=0; i<candidateNames.size(); ++i) {
            const juce::String& candidateName = candidateNames[i];
            if (directory.getNumberOfChildFiles(juce::File::findFiles|juce::File::ignoreHiddenFiles, candidateName) == 1) {
                const juce::File candidateFile = directory.getChildFile(candidateName);
                size_t candidateChannelCount = 0;
                size_t candidateSampleCount = 0;
                double candidateSampleRate = 0.0;
                const bool fileInfoSuccess = readAudioFileInfo(candidateFile, candidateChannelCount, candidateSampleCount, candidateSampleRate);

                if (fileInfoSuccess &&
                        candidateChannelCount == 2 &&
                        candidateSampleCount == sampleCount &&
                        ::fabs(candidateSampleRate - sampleRate) < 0.000001) {
                    return candidateFile;
                }
            }
        }

        return juce::File();
    }
}

bool readAudioFileInfo(const juce::File& file, size_t& channelCount, size_t& sampleCount, double& sampleRate) {
  juce::AudioFormatManager formatManager;
  formatManager.registerBasicFormats();
  std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
  if (reader)
  {
    channelCount = static_cast<size_t>(reader->numChannels);
    sampleCount = static_cast<size_t>(reader->lengthInSamples);
    sampleRate = reader->sampleRate;
    return true;
  }
  channelCount = 0;
  sampleCount = 0;
  sampleRate = 0.0;
  return false;
}


TrueStereoPairs findTrueStereoPairs(const juce::File& file, size_t sampleCount, double sampleRate) {
  if (!file.existsAsFile() || file.isDirectory())
  {
    return TrueStereoPairs();
  }

  const juce::File directory = file.getParentDirectory();
  if (!file.existsAsFile() || file.isDirectory())
  {
    return TrueStereoPairs();
  }

  const juce::String fileNameBody = file.getFileNameWithoutExtension();
  const juce::String fileNameExt = file.getFileExtension();

  // Left => Right
  static std::vector<std::pair<juce::String, juce::String> > pairsLeft;
  if (pairsLeft.empty())
  {
    pairsLeft.push_back(std::make_pair(juce::String("L"),    juce::String("R")));
    pairsLeft.push_back(std::make_pair(juce::String("l"),    juce::String("r")));
    pairsLeft.push_back(std::make_pair(juce::String("Left"), juce::String("Right")));
    pairsLeft.push_back(std::make_pair(juce::String("left"), juce::String("right")));
    pairsLeft.push_back(std::make_pair(juce::String("LEFT"), juce::String("RIGHT")));
  }
  for (size_t i=0; i<pairsLeft.size(); ++i)
  {
    const juce::File matchingFile = checkMatchingTrueStereoFile(fileNameBody,
                                                                fileNameExt,
                                                                directory,
                                                                pairsLeft[i].first,
                                                                pairsLeft[i].second,
                                                                sampleCount,
                                                                sampleRate);
    if (matchingFile.existsAsFile())
    {
      TrueStereoPairs trueStereoPairs;
      trueStereoPairs.push_back(std::make_pair(file, 0));
      trueStereoPairs.push_back(std::make_pair(file, 1));
      trueStereoPairs.push_back(std::make_pair(matchingFile, 0));
      trueStereoPairs.push_back(std::make_pair(matchingFile, 1));
      return trueStereoPairs;
    }
  }

  static std::vector<std::pair<juce::String, juce::String> > pairsRight;
  if (pairsRight.empty())
  {
    pairsRight.push_back(std::make_pair(juce::String("R"),     juce::String("L")));
    pairsRight.push_back(std::make_pair(juce::String("r"),     juce::String("l")));
    pairsRight.push_back(std::make_pair(juce::String("Right"), juce::String("Left")));
    pairsRight.push_back(std::make_pair(juce::String("right"), juce::String("left")));
    pairsRight.push_back(std::make_pair(juce::String("RIGHT"), juce::String("LEFT")));
  }
  for (size_t i=0; i<pairsRight.size(); ++i)
  {
    const juce::File matchingFile = checkMatchingTrueStereoFile(fileNameBody,
                                                                fileNameExt,
                                                                directory,
                                                                pairsRight[i].first,
                                                                pairsRight[i].second,
                                                                sampleCount,
                                                                sampleRate);
    if (matchingFile.existsAsFile())
    {
      TrueStereoPairs trueStereoPairs;
      trueStereoPairs.push_back(std::make_pair(matchingFile, 0));
      trueStereoPairs.push_back(std::make_pair(matchingFile, 1));
      trueStereoPairs.push_back(std::make_pair(file, 0));
      trueStereoPairs.push_back(std::make_pair(file, 1));
      return trueStereoPairs;
    }
  }

  return TrueStereoPairs();
}