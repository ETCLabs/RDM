# RDM                                                               {#mainpage}

## Introduction

RDM is a lightweight library that provides type definitions and message parsing
utilities for the Remote Device Management (RDM) protocol, defined in ANSI
E1.20. More information about RDM can be found [here](http://www.rdmprotocol.org).

To jump right into the documentation, check out the [Modules Overview](@ref rdm).

## Building RDM for Your Platform

RDM is built with [CMake](https://cmake.org), a popular cross-platform build
system generator. CMake can also be used to include RDM as a dependency to
other projects, i.e. using the `add_subdirectory()` command.

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
   You can optionally specify your build system with the `-G` option;
   otherwise, CMake will choose a system-appropriate default. Use `cmake --help`
   to see all available options.
4. Use CMake to invoke the generated build system to build the RDM library and
   unit tests:
   ```
   $ cmake --build .
   ```
   If you are generating IDE project files, you can use CMake to open the
   projects in the IDE:
   ```
   $ cmake --open .
   ```
5. To run the unit tests after building:
   ```
   # On Windows
   > .\test\[Build_Configuration]\test_rdm.exe

   # Elsewhere
   $ ./test/test_rdm
   ```
   Or, if you are generating IDE project files, simply run the test_rdm
   project from your IDE.

Alternatively, if you don't want to use CMake, your project can simply build in
the RDM sources directly using the src/ and include/ directories.

## Dependencies

### EtcPal

RDM depends on the ETC's Platform Abstraction Layer (EtcPal) so that it can be
platform-neutral. By default, EtcPal is included as a submodule in the
`external` directory. The CMake configuration will automatically update and
pull the submodule before building.

If you want to develop EtcPal simultaneously alongside RDM, simply clone the
EtcPal repository at the same level as the RDM repository, like so:
```
|- RDM/
|- EtcPal/
```

The RDM CMakeLists.txt file contains directives to try to find EtcPal at that
location before using the submodule.
