#!/usr/bin/env bash

set -euxo pipefail

if [[ ! -e ./build/build.ninja ]]; then
    ./setup.sh
fi

time ninja -C build
