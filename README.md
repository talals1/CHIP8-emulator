CHIP-8 Emulator
==============
This is a simple interpreter/emulator for the [CHIP-8 programming language](https://en.wikipedia.org/wiki/CHIP-8). Its unfinished and a bit rough around the edges since this is supposed to be more of a learning experience for me. If you would like a fully-featured CHIP-8 emulator, I'd suggest looking through the many similar project located on Github.

## Compiling
-------------
Since I primarily use VSCode on Windows and love the ease of Makefiles, I compile on WSL via `make` which uses the mingw32 package in order to get an `exe`. Are there better ways of doing it? Yep!

For Linux users, do `make linux` (which uses standard `gcc`).

## Resources
------------
I primarily used [Cowgod's CHIP-8 technical reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#00EE) in implementing this emulator.
