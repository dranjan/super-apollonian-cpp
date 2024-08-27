#!/usr/bin/env bash

set -euxo pipefail

meson setup build . --warnlevel 3 --werror --buildtype=release --reconfigure
time meson compile -C build
