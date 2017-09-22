#! /usr/bin/env bash
BASEDIR=$(dirname "$(dirname "$(realpath "$0")")")
TARGET="src/buildidstring"
BUILD_ID="$(git rev-parse --abbrev-ref HEAD)-$(git rev-parse --short HEAD)"
cd "$BASEDIR"
echo "Updating \"$TARGET\" with build ID \"$BUILD_ID\"."
echo "\"${BUILD_ID//\"/\\\"}\"" > "./$TARGET"
