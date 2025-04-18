#!/bin/bash

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" > /dev/null 2>&1 && pwd )"
PROJECT_DIR=$SCRIPT_DIR/..

cd $PROJECT_DIR/Projects

echo "=== Hashing existing projects ==="
INTRO_HASH=$(shasum -a 256 Intro/BodyandSoulIntro.jucer | awk '{print $1}')
BODY_HASH=$(shasum -a 256 Body/BodyandSoulBody.jucer | awk '{print $1}')
SOUL_HASH=$(shasum -a 256 Soul/BodyandSoulSoul.jucer | awk '{print $1}')
FX_HASH=$(shasum -a 256 FX/BodyandSoulFX.jucer | awk '{print $1}')

echo "=== Generating projects ==="
cd -
$SCRIPT_DIR/generate-projects.sh

echo "=== Hashing new projects ==="
cd $PROJECT_DIR/Projects
NEW_INTRO_HASH=$(shasum -a 256 Intro/BodyandSoulIntro.jucer | awk '{print $1}')
NEW_BODY_HASH=$(shasum -a 256 Body/BodyandSoulBody.jucer | awk '{print $1}')
NEW_SOUL_HASH=$(shasum -a 256 Soul/BodyandSoulSoul.jucer | awk '{print $1}')
NEW_FX_HASH=$(shasum -a 256 FX/BodyandSoulFX.jucer | awk '{print $1}')

HAS_CHANGED=0
if [ "$INTRO_HASH" != "$NEW_INTRO_HASH" ]; then
    echo "Intro project needs to be regenerated"
    HAS_CHANGED=1
fi

if [ "$BODY_HASH" != "$NEW_BODY_HASH" ]; then
    echo "Body project needs to be regenerated"
    HAS_CHANGED=1
fi

if [ "$SOUL_HASH" != "$NEW_SOUL_HASH" ]; then
    echo "Soul project needs to be regenerated"
    HAS_CHANGED=1
fi

if [ "$FX_HASH" != "$NEW_FX_HASH" ]; then
    echo "FX project needs to be regenerated"
    HAS_CHANGED=1
fi

if [ $HAS_CHANGED -eq 1 ]; then
    # Error, projects have changed
    exit 1
fi