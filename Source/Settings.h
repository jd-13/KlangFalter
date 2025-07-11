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

#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "JuceHeader.h"


class Settings
{
public:
  Settings();
  virtual ~Settings();

  void addChangeListener(juce::ChangeListener* listener);
  void removeChangeListener(juce::ChangeListener* listener);

  juce::File getImpulseResponseDirectory();

  enum TimelineUnit
  {
    Seconds,
    Beats
  };
  TimelineUnit getTimelineUnit();
  void setTimelineUnit(TimelineUnit timelineUnit);

  juce::Rectangle<int> getDefaultUIBounds();
  void setDefaultUIBounds(const juce::Rectangle<int>& bounds);

private:
  juce::ApplicationProperties _properties;

  Settings(const Settings&);
  Settings& operator=(const Settings&);
};


#endif // Header guard
