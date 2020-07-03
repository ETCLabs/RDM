# Contributing to RDM

Thank you for your interest in contributing to the RDM project!

## Pull requests

Thanks for your interest in contributing code to the RDM library!

### Building and Debugging the Library

Check out the relevant [docs page](https://etclabs.github.io/RDM/docs/head) for how to build RDM.

When configuring with CMake, you will want to define `RDM_BUILD_TESTS=ON` so that you can check
that the unit tests pass as you modify the RDM library. You can run the unit test executables using
CTest by just typing `ctest` in the build directory (or `ctest -C [configuration]` for a
multi-config CMake generator).

### Before Opening a Pull Request

* Make sure the unit tests pass
* Add unit tests if applicable for any regressions or new features you have added to the library
* Format the code you've touched using clang-format (CMake creates a convenient target `reformat_all`
  which runs clang-format on all of the RDM sources if it is available on your PATH)

## Reporting issues

### Check to make sure your issue isn't already known

If you have identified a reproducible bug or missing feature, please do the following before
opening an issue:

* Make sure the bug or feature is not covered as a known issue in the README or documentation.
* Make sure the bug or feature is not covered in an existing open issue.

### Write a comprehensive bug report

A good bug report includes the following:

* Which library code modules(s) you were using and the library's version
* A set of steps to reproduce the issue, in order
* What you expected to see, and what happened instead
* If the bug has occured in code you wrote that uses the RDM library, please provide code snippets
  and try to reduce to a minimal reproducible example.
* Any logging output that was produced when the issue occurred
