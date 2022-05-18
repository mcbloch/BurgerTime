#!/usr/bin/env bash

set -euo pipefail

echo "Prepare in silence.."

# Check that every string in a given array is callable as a command.
#
# Arguments:
#   A series of commands to check.
function check() {
  for i in "$@"; do
    command -v "$i" &>/dev/null || {
      echo >&2 "error: $i is not installed"
      exit 1
    }
  done
}

check "g++" "conan"

echo "All dependencies are installed."

export BUILD_FOLDER=cmake-build-debug
mkdir -p "$BUILD_FOLDER"

# Uncomment to use the local folder as conan cache. Default is user-wide
# export CONAN_USER_HOME="$(pwd)/conan-tmp"
# mkdir -p "$CONAN_USER_HOME"

cd $BUILD_FOLDER || exit

# echo "conan install "
# conan install .. --build=libmikmod --build=sdl_mixer # 1&>/dev/null
echo "cmake"
cmake .. # 1&>/dev/null
echo "make"
make # 1&>/dev/null

echo "Prepare successful"
exit 0
