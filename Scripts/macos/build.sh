#!/bin/bash

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" > /dev/null 2>&1 && pwd )"

echo "=== Downloading JUCE ==="
JUCE_VERSION=8.0.4
wget https://github.com/juce-framework/JUCE/releases/download/$JUCE_VERSION/juce-$JUCE_VERSION-osx.zip \
     -P $SCRIPT_DIR/../..
unzip $SCRIPT_DIR/../../juce-$JUCE_VERSION-osx.zip

echo "=== Generating projects ==="
PROJECT_DIR=$SCRIPT_DIR/../..
JUCE_HOME=$SCRIPT_DIR/../../JUCE
$JUCE_HOME/Projucer.app/Contents/MacOS/Projucer --resave $PROJECT_DIR/Projects/Intro/BodyandSoulIntro.jucer
$JUCE_HOME/Projucer.app/Contents/MacOS/Projucer --resave $PROJECT_DIR/Projects/Body/BodyandSoulBody.jucer
$JUCE_HOME/Projucer.app/Contents/MacOS/Projucer --resave $PROJECT_DIR/Projects/Soul/BodyandSoulSoul.jucer
# $JUCE_HOME/Projucer.app/Contents/MacOS/Projucer --resave $PROJECT_DIR/Projects/FX/BodyandSoulFX.jucer

echo "=== Starting builds ==="
xcodebuild -version
xcodebuild -project "$PROJECT_DIR/Projects/Intro/Builds/MacOSX/Body and Soul Intro.xcodeproj" -scheme "Body and Soul Intro - All" -configuration Release
xcodebuild -project "$PROJECT_DIR/Projects/Body/Builds/MacOSX/Body and Soul Body.xcodeproj" -scheme "Body and Soul Body - All" -configuration Release
xcodebuild -project "$PROJECT_DIR/Projects/Soul/Builds/MacOSX/Body and Soul Soul.xcodeproj" -scheme "Body and Soul Soul - All" -configuration Release
# xcodebuild -project "$PROJECT_DIR/Projects/FX/Builds/MacOSX/Body and Soul FX.xcodeproj" -scheme "Body and Soul FX - All" -configuration Release

echo "=== Collecting artefacts ==="
mkdir -p $SCRIPT_DIR/dist/BodyandSoulIntro

cp -r ~/Library/Audio/Plug-Ins/VST3/TSoM-BodyandSoulIntro.vst3 $SCRIPT_DIR/dist/BodyandSoulIntro
mkdir -p $SCRIPT_DIR/dist/BodyandSoulIntro/TSoM-BodyandSoulIntro.vst3/Contents/Resources/IRs
cp -r $PROJECT_DIR/IRs/Intro/* $SCRIPT_DIR/dist/BodyandSoulIntro/TSoM-BodyandSoulIntro.vst3/Contents/Resources/IRs
cp $PROJECT_DIR/Themes/Intro.json $SCRIPT_DIR/dist/BodyandSoulIntro/TSoM-BodyandSoulIntro.vst3/Contents/Resources/theme.json

mkdir -p $SCRIPT_DIR/dist/BodyandSoul

cp -r ~/Library/Audio/Plug-Ins/VST3/TSoM-BodyandSoulBody.vst3 $SCRIPT_DIR/dist/BodyandSoul
mkdir -p $SCRIPT_DIR/dist/BodyandSoul/TSoM-BodyandSoulBody.vst3/Contents/Resources/IRs
cp -r $PROJECT_DIR/IRs/Body/* $SCRIPT_DIR/dist/BodyandSoul/TSoM-BodyandSoulBody.vst3/Contents/Resources/IRs
cp $PROJECT_DIR/Themes/Body.json $SCRIPT_DIR/dist/BodyandSoul/TSoM-BodyandSoulBody.vst3/Contents/Resources/theme.json

cp -r ~/Library/Audio/Plug-Ins/VST3/TSoM-BodyandSoulSoul.vst3 $SCRIPT_DIR/dist/BodyandSoul
mkdir -p $SCRIPT_DIR/dist/BodyandSoul/TSoM-BodyandSoulSoul.vst3/Contents/Resources/IRs
cp -r $PROJECT_DIR/IRs/Soul/* $SCRIPT_DIR/dist/BodyandSoul/TSoM-BodyandSoulSoul.vst3/Contents/Resources/IRs
cp $PROJECT_DIR/Themes/Soul.json $SCRIPT_DIR/dist/BodyandSoul/TSoM-BodyandSoulSoul.vst3/Contents/Resources/theme.json

# cp -r ~/Library/Audio/Plug-Ins/VST3/TSoM-BodyandSoulFX.vst3 $SCRIPT_DIR/dist/BodyandSoul
# mkdir -p $SCRIPT_DIR/dist/BodyandSoul/TSoM-BodyandSoulFX.vst3/Contents/Resources/IRs
# cp -r $PROJECT_DIR/IRs/FX/* $SCRIPT_DIR/dist/BodyandSoul/TSoM-BodyandSoulFX.vst3/Contents/Resources/IRs
# cp $PROJECT_DIR/Themes/FX.json $SCRIPT_DIR/dist/BodyandSoul/TSoM-BodyandSoulFX.vst3/Contents/Resources/theme.json
