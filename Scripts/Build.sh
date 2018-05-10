#!/usr/bin/env bash

set -e

tput setaf 3; tput bold; echo "Installing Yotta"
pip install --user yotta
tput setaf 3; tput bold; echo "Setting Yotta Target"
yotta target bbc-microbit-classic-gcc

tput setaf 3; tput bold; echo "Building Project"
VERBOSE=1 yotta build
tput setaf 3; tput bold; echo "Copying Output"
cp build/bbc-microbit-classic-gcc/Source/microbit-js-remote-combined.hex GH-Pages/microbit-js.hex 
