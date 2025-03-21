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

#include "CustomLookAndFeel.h"

#include "../DecibelScaling.h"


namespace {
  float scaled(int currentRootWidth, float value) {
    constexpr float NOMINAL_WIDTH {760};
    return currentRootWidth / (NOMINAL_WIDTH / value);
  }
}

CustomLookAndFeel::CustomLookAndFeel() :
  juce::LookAndFeel_V3()
{
}


// ==============================================
// Waveform
// ==============================================
juce::Colour CustomLookAndFeel::getWaveformColour() const
{
  return theme.waveform;
}


juce::Colour CustomLookAndFeel::getWaveformBackgroundColour() const
{
  return juce::Colour(0.0f, 0.0f, 0.0f, 0.0f);
}


// ==============================================
// Envelope
// ==============================================

juce::Colour CustomLookAndFeel::getEnvelopeRestrictionColour() const
{
  return getEnvelopeNodeColour(false).withAlpha(48.0f/255.0f);
}


juce::Colour CustomLookAndFeel::getEnvelopeNodeColour(bool highlighted) const
{
  return highlighted ? juce::Colour(0x40, 0x40, 0xFF) : theme.background.withAlpha(0.5f);
}


// ==============================================
// Scales
// ==============================================

juce::Font CustomLookAndFeel::getScaleFont(int currentParentWidth) const
{
  juce::Font font;
  font.setHeight(scaled(currentParentWidth, 0.7f * font.getHeight()));
  return font;
}


juce::Colour CustomLookAndFeel::getScaleColour() const
{
  return theme.neutral;
}

juce::Colour CustomLookAndFeel::getWaveformScaleColour() const
{
  return theme.waveformContainerNeutral;
}



juce::Colour CustomLookAndFeel::getLevelColourMinusInfDb() const
{
  return theme.meters;
}


juce::Colour CustomLookAndFeel::getLevelColourMinus40Db() const
{
  return theme.meters;
}


juce::Colour CustomLookAndFeel::getLevelColourZeroDb() const
{
  return theme.meters;
}


juce::Colour CustomLookAndFeel::getLevelColourClipping() const
{
  return juce::Colour(255, 64, 64);
}
