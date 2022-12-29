#!/usr/bin/env bash

set -euxo pipefail

if [[ ! -e ./build/build.ninja ]]; then
    ./setup.sh
fi

pushd build
ninja
mkdir -p output
time ./main output/apollonian.png
popd
