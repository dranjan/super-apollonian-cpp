#!/usr/bin/env bash

set -euxo pipefail

if [[ ! -e build/build.ninja ]]; then
    mkdir -p build
    meson . build --warnlevel 3 --werror --buildtype=release
fi

time ninja -C build
