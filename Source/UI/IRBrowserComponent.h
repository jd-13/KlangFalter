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

#ifndef _IRBROWSERCOMPONENT_H
#define _IRBROWSERCOMPONENT_H


#include "JuceHeader.h"

#include "../Processor.h"
#include "../Settings.h"
#include "UIUtils.hpp"


class IRBrowserComponent : public juce::Component,
                           public juce::FileBrowserListener,
                           public juce::ChangeListener
{
public:
  IRBrowserComponent();
  virtual ~IRBrowserComponent();

  virtual void init(Processor* processor, UIUtils::Theme theme);
  virtual void updateLayout();

  virtual void paint(juce::Graphics& g);
  virtual void resized();

  virtual void selectionChanged();
  virtual void fileClicked(const juce::File &file, const juce::MouseEvent &e);
 	virtual void fileDoubleClicked(const juce::File &file);
 	virtual void browserRootChanged(const juce::File &newRoot);

  virtual void changeListenerCallback(juce::ChangeBroadcaster* source);

private:
  std::unique_ptr<juce::TimeSliceThread> _timeSliceThread;
  std::unique_ptr<juce::FileFilter> _fileFilter;
  std::unique_ptr<juce::DirectoryContentsList> _directoryContent;
  std::unique_ptr<juce::FileTreeComponent> _fileTreeComponent;
  std::unique_ptr<juce::Label> _infoLabel;
  Processor* _processor;
  std::unique_ptr<UIUtils::FileTreeLookAndFeel> _fileTreeLookAndFeel;
  UIUtils::Theme _theme;

  IRBrowserComponent(const IRBrowserComponent&);
  IRBrowserComponent& operator=(const IRBrowserComponent&);
};

#endif // Header guard
