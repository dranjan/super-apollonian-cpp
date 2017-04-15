#!/usr/bin/env bash

set -eu

scons

mkdir -p build/output

time build/bin/main build/output/apollonian.png
