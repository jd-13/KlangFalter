#!/bin/bash

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" > /dev/null 2>&1 && pwd )"

echo "=== Downloading JUCE ==="
JUCE_VERSION=7.0.9
wget https://github.com/juce-framework/JUCE/releases/download/$JUCE_VERSION/juce-$JUCE_VERSION-osx.zip \
     -P $SCRIPT_DIR/../..
unzip $SCRIPT_DIR/../../juce-$JUCE_VERSION-osx.zip

echo "=== Setting module paths ==="
PROJECT_DIR=$SCRIPT_DIR/../..
sed -i '' -e 's#../../../SDKs/JUCE/modules#JUCE/modules#g' $PROJECT_DIR/KlangFalter.jucer

echo "=== Generating project ==="
JUCE_HOME=$SCRIPT_DIR/../../JUCE
$JUCE_HOME/Projucer.app/Contents/MacOS/Projucer --resave $PROJECT_DIR/KlangFalter.jucer

echo "=== Starting KlangFalter build ==="
cd $PROJECT_DIR/Builds/MacOSX
xcodebuild -version
xcodebuild -project "Body and Soul Intro.xcodeproj" -scheme "Body and Soul Intro - All" -configuration Release

echo "=== Collecting artefacts ==="
mkdir -p $SCRIPT_DIR/dist

cp -r ~/Library/Audio/Plug-Ins/VST3/TSoM-BodyandSoulIntro.vst3 $SCRIPT_DIR/dist
cp -r ~/Library/Audio/Plug-Ins/Components/TSoM-BodyandSoulIntro.component $SCRIPT_DIR/dist

echo "=== Copying IRs ==="
mkdir -p $SCRIPT_DIR/dist/TSoM-BodyandSoulIntro.vst3/Contents/Resources/IRs
cp $PROJECT_DIR/IRs/* $SCRIPT_DIR/dist/TSoM-BodyandSoulIntro.vst3/Contents/Resources/IRs
mkdir -p $SCRIPT_DIR/dist/TSoM-BodyandSoulIntro.component/Contents/Resources/IRs
cp $PROJECT_DIR/IRs/* $SCRIPT_DIR/dist/TSoM-BodyandSoulIntro.component/Contents/Resources/IRs

echo "=== Copying theme ==="
cp $PROJECT_DIR/Themes/$THEME_FILE $SCRIPT_DIR/dist/TSoM-BodyandSoulIntro.vst3/Contents/Resources/theme.json
cp $PROJECT_DIR/Themes/$THEME_FILE $SCRIPT_DIR/dist/TSoM-BodyandSoulIntro.component/Contents/Resources/theme.json
