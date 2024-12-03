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

## Documentation

This code compiles to one program that outputs one PNG image. It's
not very complex, so I recommend just diving into the code. For a
little bit more context on some aspects of the design, the
[HACKING.md](./HACKING.md) file may be useful in parallel.

## Copyright

Copyright Darsh Ranjan.

This software is released under the GNU General Public License,
version 3. See the file [COPYING](./COPYING) for details.
