#!/usr/bin/env sh

BUILD_FOLDER=cmake-build

mkdir -p $BUILD_FOLDER
cd $BUILD_FOLDER || exit

./bin/burgertime

