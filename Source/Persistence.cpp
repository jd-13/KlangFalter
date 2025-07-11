// ==================================================================================
// Copyright (c) 2012 HiFi-LoFi
//
// This is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// ==================================================================================

#include "Persistence.h"

#include "IRAgent.h"
#include "Parameters.h"

#include <algorithm>
#include <list>
#include <map>


namespace Internal
{

  struct IRAgentConfiguration
  {
    IRAgent* _irAgent;
    String _file;
    int _fileChannel;
    double _stretch;
  };


  static juce::String EqType2String(Parameters::EqType eqType)
  {
    if (eqType == Parameters::Cut)
    {
      return juce::String("Cut");
    }
    if (eqType == Parameters::Shelf)
    {
      return juce::String("Shelf");
    }
    return juce::String();
  }

  static Parameters::EqType String2EqType(const juce::String& eqTypeString)
  {
    if (eqTypeString == juce::String("Cut"))
    {
      return Parameters::Cut;
    }
    if (eqTypeString == juce::String("Shelf"))
    {
      return Parameters::Shelf;
    }
    return Parameters::Cut;
  }

} // End of namespace internal



XmlElement* SaveState(const File& irDirectory, Processor& processor)
{
  ScopedPointer<XmlElement> convolutionElement(new XmlElement("Convolution"));
  convolutionElement->setAttribute("pluginVersion", juce::String(ProjectInfo::versionString));
  convolutionElement->setAttribute("wetOn", processor.getParameter(Parameters::WetOn));
  convolutionElement->setAttribute("wetDecibels", processor.getParameter(Parameters::WetDecibels));
  convolutionElement->setAttribute("dryOn", processor.getParameter(Parameters::DryOn));
  convolutionElement->setAttribute("dryDecibels", processor.getParameter(Parameters::DryDecibels));
  convolutionElement->setAttribute("autoGainOn", processor.getParameter(Parameters::AutoGainOn));
  convolutionElement->setAttribute("eqLowType", Internal::EqType2String(Parameters::EqType(processor.getParameter(Parameters::EqLowType))));
  convolutionElement->setAttribute("eqLowCutFreq", processor.getParameter(Parameters::EqLowCutFreq));
  convolutionElement->setAttribute("eqLowShelfFreq", processor.getParameter(Parameters::EqLowShelfFreq));
  convolutionElement->setAttribute("eqLowShelfDecibels", processor.getParameter(Parameters::EqLowShelfDecibels));
  convolutionElement->setAttribute("eqHighType", Internal::EqType2String(Parameters::EqType(processor.getParameter(Parameters::EqHighType))));
  convolutionElement->setAttribute("eqHighCutFreq", processor.getParameter(Parameters::EqHighCutFreq));
  convolutionElement->setAttribute("eqHighShelfFreq", processor.getParameter(Parameters::EqHighShelfFreq));
  convolutionElement->setAttribute("eqHighShelfDecibels", processor.getParameter(Parameters::EqHighShelfDecibels));
  convolutionElement->setAttribute("irBegin", processor.getIRBegin());
  convolutionElement->setAttribute("irEnd", processor.getIREnd());
  convolutionElement->setAttribute("stretch", processor.getStretch());
  convolutionElement->setAttribute("predelayMs", processor.getPredelayMs());
  convolutionElement->setAttribute("attackLength", processor.getAttackLength());
  convolutionElement->setAttribute("attackShape", processor.getAttackShape());
  convolutionElement->setAttribute("decayShape", processor.getDecayShape());
  convolutionElement->setAttribute("stereoWidth", processor.getParameter(Parameters::StereoWidth));
  // convolutionElement->setAttribute("shimmerWetGain", processor.getParameter(Parameters::ShimmerWetGain));
  // convolutionElement->setAttribute("shimmerFeedback", processor.getParameter(Parameters::ShimmerFeedback));
  convolutionElement->setAttribute("chorusWetGain", processor.getParameter(Parameters::ChorusWetGain));
  convolutionElement->setAttribute("chorusFrequency", processor.getParameter(Parameters::ChorusFrequency));
  convolutionElement->setAttribute("chorusDepth", processor.getParameter(Parameters::ChorusDepth));
  convolutionElement->setAttribute("reverse", processor.getReverse());
  convolutionElement->setAttribute("irBrowserOpen", processor.getIrBrowserOpen());
  convolutionElement->setAttribute("uiBounds", processor.getUIBounds().toString());
  convolutionElement->setAttribute("presetName", processor.getPresetName());

  // IRs
  auto irAgents = processor.getAgents();
  for (auto it=irAgents.begin(); it!=irAgents.end(); ++it)
  {
    IRAgent* irAgent = (*it);
    if (!irAgent)
    {
      continue;
    }

    const File irFile = irAgent->getFile();
    if (!irFile.existsAsFile())
    {
      continue;
    }

    XmlElement* irElement = new XmlElement("ImpulseResponse");
    irElement->setAttribute("input", static_cast<int>(irAgent->getInputChannel()));
    irElement->setAttribute("output", static_cast<int>(irAgent->getOutputChannel()));
    irElement->setAttribute("file", irFile.getRelativePathFrom(irDirectory));
    irElement->setAttribute("fileChannel", static_cast<int>(irAgent->getFileChannel()));
    convolutionElement->addChildElement(irElement);
  }

  return convolutionElement.release();
}




bool LoadState(const File& irDirectory, XmlElement& element, Processor& processor)
{
  if (element.getTagName() != "Convolution")
  {
    return false;
  }

  // Phase 1: Load all data
  bool wetOn = element.getBoolAttribute("wetOn", Parameters::WetOn.getDefaultValue());
  double wetDecibels = element.getDoubleAttribute("wetDecibels", Parameters::WetDecibels.getDefaultValue());
  bool dryOn = element.getBoolAttribute("dryOn", Parameters::DryOn.getDefaultValue());
  double dryDecibels = element.getDoubleAttribute("dryDecibels", Parameters::DryDecibels.getDefaultValue());
  bool autoGainOn = element.getBoolAttribute("autoGainOn", Parameters::AutoGainOn.getDefaultValue());
  double irBegin = element.getDoubleAttribute("irBegin", 0.0);
  double irEnd = element.getDoubleAttribute("irEnd", 1.0);
  double stretch = element.getDoubleAttribute("stretch", 1.0);
  double predelayMs = element.getDoubleAttribute("predelayMs", 0.0);
  double attackLength = element.getDoubleAttribute("attackLength", 0.0);
  double attackShape = element.getDoubleAttribute("attackShape", 0.0);
  double decayShape = element.getDoubleAttribute("decayShape", 0.0);
  double stereoWidth = element.getDoubleAttribute("stereoWidth", Parameters::StereoWidth.getDefaultValue());
  // double shimmerWetGain = element.getDoubleAttribute("shimmerWetGain", Parameters::ShimmerWetGain.getDefaultValue());
  // double shimmerFeedback = element.getDoubleAttribute("shimmerFeedback", Parameters::ShimmerFeedback.getDefaultValue());
  double chorusWetGain = element.getDoubleAttribute("chorusWetGain", Parameters::ChorusWetGain.getDefaultValue());
  double chorusFrequency = element.getDoubleAttribute("chorusFrequency", Parameters::ChorusFrequency.getDefaultValue());
  double chorusDepth = element.getDoubleAttribute("chorusDepth", Parameters::ChorusDepth.getDefaultValue());
  bool reverse = element.getBoolAttribute("reverse", false);
  const bool irBrowserOpen = element.getBoolAttribute("irBrowserOpen", false);
  const juce::String presetName = element.getStringAttribute("presetName", juce::String());

  if (element.hasAttribute("uiBounds")) {
    processor.setUIBounds(juce::Rectangle<int>::fromString(element.getStringAttribute("uiBounds")), false);
  }

  Parameters::EqType eqLoType = Internal::String2EqType(element.getStringAttribute("eqLowType", juce::String()));
  double eqLoCutFreq = element.getDoubleAttribute("eqLowCutFreq", Parameters::EqLowCutFreq.getDefaultValue());
  double eqLoShelfFreq = element.getDoubleAttribute("eqLowShelfFreq", Parameters::EqLowShelfFreq.getDefaultValue());
  double eqLoShelfDecibels = element.getDoubleAttribute("eqLowShelfDecibels", Parameters::EqLowShelfDecibels.getDefaultValue());

  Parameters::EqType eqHiType = Internal::String2EqType(element.getStringAttribute("eqHighType", juce::String()));
  double eqHiCutFreq = element.getDoubleAttribute("eqHighCutFreq", Parameters::EqHighCutFreq.getDefaultValue());
  double eqHiShelfFreq = element.getDoubleAttribute("eqHighShelfFreq", Parameters::EqHighShelfFreq.getDefaultValue());
  double eqHiShelfDecibels = element.getDoubleAttribute("eqHighShelfDecibels", Parameters::EqHighShelfDecibels.getDefaultValue());

  // IRs
  std::vector<Internal::IRAgentConfiguration> irConfigurations;
  forEachXmlChildElementWithTagName (element, irElement, "ImpulseResponse")
  {
    const int inputChannel = irElement->getIntAttribute("input", -1);
    const int outputChannel = irElement->getIntAttribute("output", -1);
    if (inputChannel == -1 || outputChannel == -1)
    {
      return false;
    }

    IRAgent* irAgent = processor.getAgent(inputChannel, outputChannel);
    if (!irAgent)
    {
      return false;
    }
    Internal::IRAgentConfiguration configuration;
    configuration._irAgent = irAgent;
    configuration._file = irElement->getStringAttribute("file", {});
    configuration._fileChannel = irElement->getIntAttribute("fileChannel", -1);
    irConfigurations.push_back(configuration);
  }

  // Phase 2: Restore the state
  processor.clearConvolvers();
  processor.setParameterNotifyingHost(Parameters::WetOn, wetOn);
  processor.setParameterNotifyingHost(Parameters::WetDecibels, static_cast<float>(wetDecibels));
  processor.setParameterNotifyingHost(Parameters::DryOn, dryOn);
  processor.setParameterNotifyingHost(Parameters::DryDecibels, static_cast<float>(dryDecibels));
  processor.setParameterNotifyingHost(Parameters::AutoGainOn, autoGainOn);
  processor.setParameterNotifyingHost(Parameters::EqLowType, static_cast<int>(eqLoType));
  processor.setParameterNotifyingHost(Parameters::EqLowCutFreq, static_cast<float>(eqLoCutFreq));
  processor.setParameterNotifyingHost(Parameters::EqLowShelfFreq, static_cast<float>(eqLoShelfFreq));
  processor.setParameterNotifyingHost(Parameters::EqLowShelfDecibels, static_cast<float>(eqLoShelfDecibels));
  processor.setParameterNotifyingHost(Parameters::EqHighType, static_cast<int>(eqHiType));
  processor.setParameterNotifyingHost(Parameters::EqHighCutFreq, static_cast<float>(eqHiCutFreq));
  processor.setParameterNotifyingHost(Parameters::EqHighShelfFreq, static_cast<float>(eqHiShelfFreq));
  processor.setParameterNotifyingHost(Parameters::EqHighShelfDecibels, static_cast<float>(eqHiShelfDecibels));
  processor.setParameterNotifyingHost(Parameters::StereoWidth, static_cast<float>(stereoWidth));
  // processor.setParameterNotifyingHost(Parameters::ShimmerWetGain, static_cast<float>(shimmerWetGain));
  // processor.setParameterNotifyingHost(Parameters::ShimmerFeedback, static_cast<float>(shimmerFeedback));
  processor.setParameterNotifyingHost(Parameters::ChorusWetGain, static_cast<float>(chorusWetGain));
  processor.setParameterNotifyingHost(Parameters::ChorusFrequency, static_cast<float>(chorusFrequency));
  processor.setParameterNotifyingHost(Parameters::ChorusDepth, static_cast<float>(chorusDepth));
  processor.setIRBegin(irBegin);
  processor.setIREnd(irEnd);
  processor.setPredelayMs(predelayMs);
  processor.setAttackLength(attackLength);
  processor.setAttackShape(attackShape);
  processor.setDecayShape(decayShape);
  processor.setStretch(stretch);
  processor.setReverse(reverse);
  processor.setIrBrowserOpen(irBrowserOpen);
  processor.setPresetName(presetName);
  for (auto it=irConfigurations.begin(); it!=irConfigurations.end(); ++it)
  {
    IRAgent* irAgent = it->_irAgent;
    const File irFile = irDirectory.getChildFile(it->_file);
    irAgent->setFile(irFile, it->_fileChannel);
  }
  return true;
}
