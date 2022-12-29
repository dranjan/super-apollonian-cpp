#!/usr/bin/env bash

set -eu

pushd build
ninja
mkdir -p output
time ./main output/apollonian.png
popd
