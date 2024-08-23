# Fancy Apollonian Gasket Rendering

## Prerequisites

- `meson` build system
- `ninja` build tool
- C++ compiler
- `cairomm`

This code has only been tested on Linux, but it would probably work
on other OSes with some minor modifications.

On Arch Linux:

    pacman -S meson ninja gcc cairomm

## Execution

To create the image, run `./run.sh` from the Git root directory.
This will recompile the code, if necesssary, and generate the image.
You can find the image at `./build/apollonian.png`.

## Tweaking and customization

For easy customization, look at the `main` function in `src/main.cpp`.
