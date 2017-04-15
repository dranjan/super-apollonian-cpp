#!/usr/bin/env bash

set -eu

scons
time build/bin/main
