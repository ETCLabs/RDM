# RDM

[![Build Status](https://dev.azure.com/ETCLabs/RDM/_apis/build/status/ETCLabs.RDM?branchName=develop)](https://dev.azure.com/ETCLabs/RDM/_build/latest?definitionId=3&branchName=develop)

A helper library for the ANSI E1.20 (RDM) protocol.

*RDM* is an ANSI standard for remotely configuring devices which are controlled via
[DMX512](https://en.wikipedia.org/wiki/DMX512). More information about RDM can be found
[here](http://www.rdmprotocol.org).

This C-language library is intended to be a very lightweight RDM implementation.

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

## Building

For instructions on building the RDM library, please see the
[documentation](https://etclabs.github.io/RDM).
