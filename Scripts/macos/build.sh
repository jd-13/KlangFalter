#!/bin/bash

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" > /dev/null 2>&1 && pwd )"

echo "=== Cloning WE-Core ==="
WECORE_HOME=$SCRIPT_DIR/../../WECore
git clone --recurse-submodules https://github.com/jd-13/WE-Core $WECORE_HOME

cd $WECORE_HOME
echo "=== Using WE-Core $(git log --pretty=format:'%h' -n 1) ==="
cd -

echo "=== Cloning linear ==="
LINEAR_HOME=$SCRIPT_DIR/../../linear
git clone https://github.com/Signalsmith-Audio/linear $LINEAR_HOME

cd $LINEAR_HOME
echo "=== Using linear $(git log --pretty=format:'%h' -n 1) ==="
cd -


echo "=== Cloning Stretch ==="
STRETCH_HOME=$SCRIPT_DIR/../../signalsmith-stretch
git clone https://github.com/Signalsmith-Audio/signalsmith-stretch $STRETCH_HOME

cd $STRETCH_HOME
echo "=== Using stretch $(git log --pretty=format:'%h' -n 1) ==="
cd -

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

echo "=== Starting VST builds ==="
xcodebuild -version
xcodebuild -project "$PROJECT_DIR/Projects/Intro/Builds/MacOSX/Body and Soul Intro.xcodeproj" -scheme "Body and Soul Intro - All" -configuration Release
# xcodebuild -project "$PROJECT_DIR/Projects/Body/Builds/MacOSX/Body and Soul Body.xcodeproj" -scheme "Body and Soul Body - All" -configuration Release
# xcodebuild -project "$PROJECT_DIR/Projects/Soul/Builds/MacOSX/Body and Soul Soul.xcodeproj" -scheme "Body and Soul Soul - All" -configuration Release
# xcodebuild -project "$PROJECT_DIR/Projects/FX/Builds/MacOSX/Body and Soul FX.xcodeproj" -scheme "Body and Soul FX - All" -configuration Release

echo "=== Starting CLAP builds ==="
buildClap() {
     cmake -Bbuild-clap -GXcode -DCMAKE_BUILD_TYPE=Release
     cmake --build build-clap --config Release
}

cd $PROJECT_DIR/Projects/Intro && buildClap
# cd $PROJECT_DIR/Projects/Body && buildClap
# cd $PROJECT_DIR/Projects/Soul && buildClap
cd $PROJECT_DIR

echo "=== Collecting artefacts ==="
collectVST() {
     OUTPUT_DIR=$1
     VARIANT=$2
     PLUGIN_NAME=TSoM-BodyandSoul$VARIANT.vst3

     cp -r ~/Library/Audio/Plug-Ins/VST3/$PLUGIN_NAME $OUTPUT_DIR
     mkdir -p $OUTPUT_DIR/$PLUGIN_NAME/Contents/Resources/IRs
     cp -r $PROJECT_DIR/IRs/$VARIANT/* $OUTPUT_DIR/$PLUGIN_NAME/Contents/Resources/IRs
     cp $PROJECT_DIR/Themes/$VARIANT.json $OUTPUT_DIR/$PLUGIN_NAME/Contents/Resources/theme.json
}

collectAU() {
     OUTPUT_DIR=$1
     VARIANT=$2
     PLUGIN_NAME=TSoM-BodyandSoul$VARIANT.component

     cp -r ~/Library/Audio/Plug-Ins/Components/$PLUGIN_NAME $OUTPUT_DIR
     mkdir -p $OUTPUT_DIR/$PLUGIN_NAME/Contents/Resources/IRs
     cp -r $PROJECT_DIR/IRs/$VARIANT/* $OUTPUT_DIR/$PLUGIN_NAME/Contents/Resources/IRs
     cp $PROJECT_DIR/Themes/$VARIANT.json $OUTPUT_DIR/$PLUGIN_NAME/Contents/Resources/theme.json
}

collectCLAP() {
     OUTPUT_DIR=$1
     VARIANT=$2
     PLUGIN_NAME=TSoM-BodyandSoul$VARIANT.clap

     cp -r $PROJECT_DIR/Projects/$VARIANT/build-clap/TSoM-BodyandSoul${VARIANT}_artefacts/Release/$PLUGIN_NAME $OUTPUT_DIR
     mkdir -p $OUTPUT_DIR/$PLUGIN_NAME/Contents/Resources/IRs
     cp -r $PROJECT_DIR/IRs/$VARIANT/* $OUTPUT_DIR/$PLUGIN_NAME/Contents/Resources/IRs
     cp $PROJECT_DIR/Themes/$VARIANT.json $OUTPUT_DIR/$PLUGIN_NAME/Contents/Resources/theme.json
}

mkdir -p $SCRIPT_DIR/dist/BodyandSoulIntro
collectVST $SCRIPT_DIR/dist/BodyandSoulIntro Intro
collectAU $SCRIPT_DIR/dist/BodyandSoulIntro Intro
collectCLAP $SCRIPT_DIR/dist/BodyandSoulIntro Intro

# mkdir -p $SCRIPT_DIR/dist/BodyandSoul
# collectVST $SCRIPT_DIR/dist/BodyandSoul Body
# collectAU $SCRIPT_DIR/dist/BodyandSoul Body
# collectCLAP $SCRIPT_DIR/dist/BodyandSoul Body

# collectVST $SCRIPT_DIR/dist/BodyandSoul Soul
# collectAU $SCRIPT_DIR/dist/BodyandSoul Soul
# collectCLAP $SCRIPT_DIR/dist/BodyandSoul Soul

# collectVST $SCRIPT_DIR/dist/BodyandSoul FX
# collectCLAP $SCRIPT_DIR/dist/BodyandSoul FX

