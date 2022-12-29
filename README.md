# Fancy Apollonian Gasket Rendering

## Prerequisites

- `meson` build system (`meson` on Ubuntu)
- `ninja` build tool (`ninja-build` on Ubuntu)
- C++ compiler
- `cairomm` (`libcairomm-1.0-dev` on Ubuntu)

This code has only been tested on Linux, but it would probably work
on other OSes with some minor modifications.

## Execution

To create the image, run `./run.sh` from the Git root directory.
This will recompile the code, if necesssary, and generate the image.
You can find the image at `./build/apollonian.png`.

## Tweaking and customization

For easy customization, look at the `main` function in `src/main.cpp`.
