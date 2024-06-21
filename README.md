[![RobôCIn](https://img.shields.io/badge/🇧🇷-RobôCIn-009B3A)](https://robocin.com.br)
[![Issues](https://img.shields.io/github/issues/robocin/ssl-core)](https://github.com/robocin/ssl-core/issues)
[![Buf: Lint and Formatting Checker](https://github.com/robocin/ssl-core/actions/workflows/buf-lint-and-formatting-checker.yaml/badge.svg?branch=main)](https://github.com/robocin/ssl-core/actions/workflows/buf-lint-and-formatting-checker.yaml?query=branch%3Amain)
[![C/C++: clang-format](https://github.com/robocin/ssl-core/actions/workflows/cpp-clang-format.yaml/badge.svg?branch=main)](https://github.com/robocin/ssl-core/actions/workflows/cpp-clang-format.yaml?query=branch%3Amain)
[![Pull Requests](https://img.shields.io/github/issues-pr/robocin/ssl-core)](https://github.com/robocin/ssl-core/pulls)
[![C++](https://img.shields.io/badge/C%2B%2B-23%2B-darkblue.svg)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/CMake-3.29%2B-blue.svg)](https://cmake.org/cmake/help/latest/release/3.29.html)
[![VS Code Dev Containers](https://img.shields.io/static/v1?label=VS+Code&message=Dev+Containers&logo=visualstudiocode&color=007ACC&labelColor=2C2C32&logoColor=007ACC)](https://code.visualstudio.com/docs/devcontainers/containers)
[![License: MIT](https://img.shields.io/badge/License-MIT-orange.svg)](LICENSE)

# ssl-core

This repository contains RobôCIn's next yet-to-be-developed software for the RoboCup Small Size Soccer.

Its conception consists of a long-term project that tries to apply the concept of microservices to robotics, taking the utmost care to meet the latency and resource management requirements that exist in the category, exploring the single responsibility principle so that each service has an isolated purpose.

## Table of Contents

- [ssl-core](#ssl-core)
  - [Table of Contents](#table-of-contents)
  - [About](#about)
  - [System Requirements](#system-requirements)
  - [Codemap](#codemap)
  - [How to Contribute](#how-to-contribute)
  - [License](#license)
  - [Acknowledgements](#acknowledgements)

## About

The `ssl-core` project is organized as a language-agnostic monorepo package-by-feature microservices. Top-level directories contains its own `README.md` and are individually responsible for describing its purpose.

## System Requirements

All the code developed was done in [Visual Studio Code](https://code.visualstudio.com) with development tied to a [devcontainer](https://code.visualstudio.com/docs/devcontainers/containers), making it platform-independent.

## Codemap

* [`experiments`](experiments/README.md)

  Contains experiments and prototypes for conceptual validation.

* [`perception`](perception/README.md)

  Contains the perception microservice.

* [`referee-ms`](referee-ms/README.md)

  Contains the referee microservice.

* [`app-shell`](app-shell/README.md)

  Contains the application shell for mounting microfrontends.

* [`common`](common/README.md)

  Contains common code and utilities used across the project.

* [`.cmake`](.cmake/README.md)

  Contains common [CMake](https://cmake.org) rules and utilities used across the project.

* [`protocols`](protocols/README.md)

  Contains the [Protocol Buffers](https://developers.google.com/protocol-buffers) definitions used across the project.

* [`docs`](docs/README.md)

  Contains documentation pages as guides, tutorials, and other useful information.

* [`containers`](containers/README.md)

  Contains common [Docker](https://www.docker.com) files and scripts used across the project.

## How to Contribute

There are many ways in which you can participate in this project, for example:

* [Reporting bugs, suggesting new features](https://github.com/robocin/ssl-core/issues) and contributing to the validation while they are being checked in
* Reviewing our [pull requests](https://github.com/robocin/ssl-core/pulls)
* Reviewing our documentation within this repository and making pull requests for anything from typos to additional new content

If you are interested in fixing issues and contributing directly to the code base,
please see [How to Contribute](docs/how-to-contribute.md).

## License

The `ssl-core` repository is licensed under the terms of the MIT license. See [LICENSE](LICENSE) for more information.

## Acknowledgements

We would like to express our gratitude to the following open-source projects that have inspired and influenced us in the development of `ssl-core`:

* [`abseil-cpp`](https://github.com/abseil/abseil-cpp), from Google
* [`vscode`](https://github.com/microsoft/vscode), from Microsoft
* [`Sumatra`](https://github.com/TIGERs-Mannheim/Sumatra), from TIGERs Mannheim
