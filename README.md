# LightningSearch

Copyright (C) 2025 Jacob L. Farnsworth

Have you found yourself growing tired of the default search function on your favorite OS? Are you sick of getting results for Web searches, product ads, or AI summaries, when all you want is to just search for a few files on your computer? Or maybe you're tired of being unable to do more complex search queries, such as by file size, creation date, or filename prefixes, without a Ph.D. in Bash?

If any of that sounds familiar, then look no further than LightningSearch - a platform-independent search tool designed to be simple, fast, and versatile, while also being suitable for more advanced use. LightningSearch takes its job of searching seriously so that you can spend less time fighting with your search tool, and more time using your computer the way you want.

LightningSearch is maintained by faculty and students of Gymnasieingenjörsprogrammet-TE4 at NTI Gymnasiet Karlstad as an educational resource in professional-grade software development.

## Features

* Filter system which supports various filter types. You can search by file name or contents. Searches are configurable with options for full or partial matches, as well as case sensitivity.
* Support for searching by regular expression.
* Effective use of multithreading to achieve fast search times on multicore systems. LightningSearch isn't afraid to use available computing resources, and doesn't depend on any indexing services.

## Upcoming Features

* More filters such as file type, size, or creation date.
* Ability to save search results in a convenient format, such as CSV.
* Settings to control aspects such as memory usage or number of worker threads.
* Ability to combine multiple filters with logical OR/AND rules.
* ... and much more!

## Setup

### Windows

If you don't have it already, download and install MSYS2:

https://www.msys2.org/#installation

Run the following commands to install mingw-w64 and Qt5:

```
pacman -Syu
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-qt5
```

Ensure that the mingw-w64 bin directory and the Qt bin directory are added to your system's path:

```
C:\msys64\mingw64\bin
C:\msys64\mingw64\share\qt5\plugins
```

## Compiling

### Windows

1. In Visual Studio Code, make sure the CMake extension is installed.
2. In the CMake extension, under Configure, choose the mingw-w64 toolchain: `GCC x86_64-w64-mingw32`
3. Click Delete Cache and Reconfigure.
4. Click Build (gear icon in the bottom left corner).

### Linux

Run the following commands to build the project:

```
mkdir -p build && cd build
cmake ..
make
```

To run unit tests:

```
make test
```


## Contributing

* `docs/planning.md` for an overview of the project and the repository structure.
* `docs/style.md` for coding style guide. Remember to follow the style guide while writing code!

## License

LightningSearch is licensed under the terms of the GNU General Public License, version 2. See the file `LICENSE` for the full license text.