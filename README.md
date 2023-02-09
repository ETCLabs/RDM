# RDM

A helper library for the ANSI E1.20 (RDM) protocol.

*RDM* is an ANSI standard for remotely configuring devices which are controlled via
[DMX512](https://en.wikipedia.org/wiki/DMX512). More information about RDM can be found
[here](http://www.rdmprotocol.org).

This C/C++ library is intended to be a very lightweight RDM implementation.

**Current Features:**

* RDM type definitions and message formats
* Functions for building and parsing RDM messages

**Planned Features:**

* Implementation of controller logic for RDM discovery
* Implementation of RDM responder logic
* RDM parameter data packing and unpacking functions

## About this ETCLabs Project

RDM is official, open-source software developed by ETC employees and is designed to interact with
ETC products. For challenges using, integrating, compiling, or modifying this software, we
encourage posting on the [issues page](https://github.com/ETCLabs/RDM/issues) of this project.

Before posting an issue or opening a pull request, please read the
[contribution guidelines](./CONTRIBUTING.md).

## Quality Gates

### Code Reviews

* At least 2 developers must approve all code changes made before they can be merged into the integration branch.
* API and major functionality reviews typically include application developers as well.

### Automated Testing

* Some unit testing is provided.

### Automated Static Analysis

* Treating warnings as errors is enabled on all platforms.
* Adding Clang Tidy (in phases) is on the TODO list. Once implemented, refer to
.clang-tidy to see which rulesets have been added.

### Automated Style Checking

* Clang format is enabled – currently this follows the style guidelines established for our libraries,
 and it may be updated from time to time. See .clang-format for more details.
* Non-conformance to .clang-format will result in pipeline failures.  The code is not automatically re-formatted.

### Continuous Integration

* A GitLab CI pipeline is being used to run builds and tests that enforce all supported quality gates for all merge
requests, and for generating new library builds from main. See .gitlab-ci.yml for details.

### Automated Dynamic Analysis

* ASAN is currently being used when running all automated tests on Linux to catch various memory errors during runtime.

## Documentation

For instructions on building the RDM library, please see the
[documentation](https://etclabs.github.io/RDMDocs).

## Revision Control

RDM development is using Git for revision control.

## License

RDM is licensed under the Apache License 2.0. RDM also incorporates some third-party software with different license terms, disclosed in ThirdPartySoftware.txt in the directory containing this
README file.
