#!/bin/bash

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" > /dev/null 2>&1 && pwd )"
PROJECT_DIR=$SCRIPT_DIR/..

cd $PROJECT_DIR/Projects

echo "=== Generating Intro project ==="
mkdir -p Intro
PROJECT_FILE_PATH=Intro/BodyandSoulIntro.jucer
cp Local/BodyandSoulLocal.jucer $PROJECT_FILE_PATH

sed -i '' -e 's#../../../../SDKs/JUCE/modules#JUCE/modules#g' $PROJECT_FILE_PATH

sed -i '' -e 's#name="Body and Soul Local"#name="Body and Soul Intro"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#bundleIdentifier="com.thesoundofmerlin.BodyAndSoulLocal"#bundleIdentifier="com.thesoundofmerlin.BodyAndSoulIntro"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginName="B\&amp;S Local"#pluginName="B\&amp;S Intro"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginDesc="Body and Soul Local"#pluginDesc="Body and Soul Intro"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginCode="Tbsb"#pluginCode="Tbsi"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginAUExportPrefix="BodyAndSoulLocalAU"#pluginAUExportPrefix="BodyAndSoulIntroAU"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#aaxIdentifier="com.thesoundofmerlin.BodyAndSoulLocal"#aaxIdentifier="com.thesoundofmerlin.BodyAndSoulIntro"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#targetName="TSoM-BodyandSoulLocal"#targetName="TSoM-BodyandSoulIntro"#g' $PROJECT_FILE_PATH

echo "=== Generating Body project ==="
mkdir -p Body
PROJECT_FILE_PATH=Body/BodyandSoulBody.jucer
cp Local/BodyandSoulLocal.jucer $PROJECT_FILE_PATH

sed -i '' -e 's#../../../../SDKs/JUCE/modules#JUCE/modules#g' $PROJECT_FILE_PATH

sed -i '' -e 's#name="Body and Soul Local"#name="Body and Soul Body"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#bundleIdentifier="com.thesoundofmerlin.BodyAndSoulLocal"#bundleIdentifier="com.thesoundofmerlin.BodyAndSoulBody"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginName="B\&amp;S Local"#pluginName="B\&amp;S Body"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginDesc="Body and Soul Local"#pluginDesc="Body and Soul Body"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginCode="Tbsb"#pluginCode="Tbsb"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginAUExportPrefix="BodyAndSoulLocalAU"#pluginAUExportPrefix="BodyAndSoulBodyAU"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#aaxIdentifier="com.thesoundofmerlin.BodyAndSoulLocal"#aaxIdentifier="com.thesoundofmerlin.BodyAndSoulBody"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#targetName="TSoM-BodyandSoulLocal"#targetName="TSoM-BodyandSoulBody"#g' $PROJECT_FILE_PATH

echo "=== Generating Soul project ==="
mkdir -p Soul
PROJECT_FILE_PATH=Soul/BodyandSoulSoul.jucer
cp Local/BodyandSoulLocal.jucer $PROJECT_FILE_PATH

sed -i '' -e 's#../../../../SDKs/JUCE/modules#JUCE/modules#g' $PROJECT_FILE_PATH

sed -i '' -e 's#name="Body and Soul Local"#name="Body and Soul Soul"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#bundleIdentifier="com.thesoundofmerlin.BodyAndSoulLocal"#bundleIdentifier="com.thesoundofmerlin.BodyAndSoulSoul"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginName="B\&amp;S Local"#pluginName="B\&amp;S Soul"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginDesc="Body and Soul Local"#pluginDesc="Body and Soul Soul"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginCode="Tbsb"#pluginCode="Tbss"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginAUExportPrefix="BodyAndSoulLocalAU"#pluginAUExportPrefix="BodyAndSoulSoulAU"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#aaxIdentifier="com.thesoundofmerlin.BodyAndSoulLocal"#aaxIdentifier="com.thesoundofmerlin.BodyAndSoulSoul"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#targetName="TSoM-BodyandSoulLocal"#targetName="TSoM-BodyandSoulSoul"#g' $PROJECT_FILE_PATH

echo "=== Generating FX project ==="
mkdir -p FX
PROJECT_FILE_PATH=FX/BodyandSoulFX.jucer
cp Local/BodyandSoulLocal.jucer $PROJECT_FILE_PATH

sed -i '' -e 's#../../../../SDKs/JUCE/modules#JUCE/modules#g' $PROJECT_FILE_PATH

sed -i '' -e 's#name="Body and Soul Local"#name="Body and Soul FX"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#bundleIdentifier="com.thesoundofmerlin.BodyAndSoulLocal"#bundleIdentifier="com.thesoundofmerlin.BodyAndSoulFX"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginName="B\&amp;S Local"#pluginName="B\&amp;S FX"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginDesc="Body and Soul Local"#pluginDesc="Body and Soul FX"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginCode="Tbsb"#pluginCode="Tbsf"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#pluginAUExportPrefix="BodyAndSoulLocalAU"#pluginAUExportPrefix="BodyAndSoulFXAU"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#aaxIdentifier="com.thesoundofmerlin.BodyAndSoulLocal"#aaxIdentifier="com.thesoundofmerlin.BodyAndSoulFX"#g' $PROJECT_FILE_PATH
sed -i '' -e 's#targetName="TSoM-BodyandSoulLocal"#targetName="TSoM-BodyandSoulFX"#g' $PROJECT_FILE_PATH
