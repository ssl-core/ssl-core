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
  - [Links](#links)

## About

The `ssl-core` project is organized as a language-agnostic monorepo package-by-feature microservices. Top-level directories contains its own `README.md` and are individually responsible for describing its purpose.

## System Requirements

All the code developed was done in [Visual Studio Code](https://code.visualstudio.com) with development tied to a [devcontainer](https://code.visualstudio.com/docs/devcontainers/containers), making it platform-independent.

## Codemap

* [`experiments`](experiments/README.md)

  Contains experiments and prototypes for conceptual validation.
* [`vision-be`](vision-be/README.md)

  Contains the vision microservice.
* [`common`](common/README.md)

  Contains common code and utilities used across the project.

## How to Contribute

TODO.

## License

The `ssl-core` repository is licensed under the terms of the MIT license. See [LICENSE](LICENSE) for more information.

## Links

TODO.