# Changelog
All notable changes to the RDM library will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

Note: This is a pre-release version. This version changelog is not exhaustive.

### Added
- C++ types for RDM commands, responses, NACK reasons, and UIDs:
  + `rdm/cpp/message.h`
  + `rdm/cpp/message_types/*`
  + `rdm/cpp/uid.h`

### Changed
- RDM_UID_CMP() renamed to rdm_uid_compare()
- Message packing and unpacking functions moved to rdm/message.h and renamed. RDM controller and
  responder modules are reserved for eventual addition of RDM controller and responder logic.

## [0.1.0] - 2019-05-29
### Added
- This CHANGELOG file.
- Initial library modules, tests and documentation.

[Unreleased]: https://github.com/ETCLabs/RDM/compare/master...develop
[0.1.0]: https://github.com/ETCLabs/RDM/releases/tag/v0.1.0
