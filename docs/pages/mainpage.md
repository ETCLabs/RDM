# RDM                                                                                   {#mainpage}

## Introduction

RDM is a lightweight library that provides type definitions and message parsing utilities for the
Remote Device Management (RDM) protocol, defined in ANSI E1.20. More information about RDM can be
found [here](http://www.rdmprotocol.org).

\htmlonly
To jump right into the documentation, check out the <a href="modules.html">Modules Overview</a>.

\endhtmlonly
 
## Platforms

RDM supports all platforms targeted by [EtcPal](https://github.com/ETCLabs/EtcPal), including
Microsoft Windows, macOS, Linux and several embedded RTOS platforms.

## Dependencies

### EtcPal

RDM depends on the ETC's Platform Abstraction Layer (EtcPal) so that it can be platform-neutral. By
default, EtcPal is included as a submodule in the `external` directory. The CMake configuration
will automatically update and pull the submodule before building.

See the [documentation for EtcPal](https://etclabs.github.io/EtcPal).

## Building RDM for Your Platform

RDM is built with [CMake](https://cmake.org), a popular cross-platform build system generator.
CMake can also be used to include RDM as a dependency to other projects, i.e. using the
`add_subdirectory()` command.

To configure and build RDM on its own using CMake, follow these steps:

1. [Download and install](https://cmake.org/download/) CMake version 3.3 or higher.
2. Create a directory in your location of choice to hold your build projects or
   Makefiles:
   ```
   $ mkdir build
   $ cd build
   ```
   This directory can be inside or outside the RDM repository.
3. Run CMake to configure the RDM project:
   ```
   $ cmake path/to/RDM/root
   ```
   You can optionally specify your build system with the `-G` option; otherwise, CMake will choose
   a system-appropriate default. Use `cmake --help` to see all available options.
4. Use CMake to invoke the generated build system to build the RDM library and any extras you have
   enabled:
   ```
   $ cmake --build .
   ```
   If you are generating IDE project files, you can use CMake to open the projects in the IDE:
   ```
   $ cmake --open .
   ```
5. Use CMake's installation target to install the built binaries and headers. This usually shows up
   as another project called "INSTALL" inside an IDE or a target called "install" (e.g.
   `make install` for a Makefile generator). You can also do it manually from the command line in
   the build directory:
   ```
   $ cmake -P cmake_install.cmake
   ```

Alternatively, if you don't want to use CMake, your project can simply build in the RDM sources
directly using the src/ and include/ directories.
