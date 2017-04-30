#!/usr/bin/env bash

set -eu

mkdir -p build

meson . build --warnlevel 3 --werror --buildtype=release
