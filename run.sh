#!/usr/bin/env bash

set -eu

pushd build
ninja
mkdir -p output
#time bin/main output/apollonian.png
time bin/test_image_buffer output/test.png
popd
