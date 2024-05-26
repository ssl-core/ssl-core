# [`ssl-core`](https://github.com/robocin/ssl-core) Protocols

The protocols directory is a central hub dedicated to storing and managing protocols that are intended to be shared across multiple projects. It serves as a reliable source of the latest versions of the protocols definitions, providing an organized directory structure that categorizes and documents each protobuf file, making it simple for developers to navigate and understand the available data structures.

By centralizing protocols in a dedicated folder, it promotes code reuse, modularity, and efficient development practices. Any updates or modifications to the shared protocols can be made in a controlled manner, ensuring compatibility across multiple repositories that depend on these protocols.

## About

The protocols contained in this repository are Protocol Buffers (protobuf).

### Protobufs

[Protobufs](https://protobuf.dev) are a language-agnostic data serialization format developed by Google that allows  efficient and structured communication between different software components.

Protocol Buffers offer numerous advantages, such as efficient serialization, platform independence, and language interoperability. By using Protocol Buffers, you can easily serialize, deserialize, and exchange structured data between different systems or languages.

## Codemap

> **Note**: All messages are inside packages that represent their paths from the root of the project to where they are.

- [`protocols/third_party`](protocols/third_party/README.md): Contains third-party protobufs that are used across the project.
- [`protocols/common`](protocols/common/README.md): Contains common protobufs that are used across the project.
- [`protocols/perception`](protocols/perception/README.md): Contains the perception service output protobufs.