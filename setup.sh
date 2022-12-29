#!/usr/bin/env bash

set -euxo pipefail

mkdir -p build

meson . build --warnlevel 3 --werror --buildtype=release
