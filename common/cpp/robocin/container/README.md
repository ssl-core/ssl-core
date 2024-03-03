# container

This module provides the implementation of adaptors for standard containers.

## Table of Contents

- [container](#container)
  - [Table of Contents](#table-of-contents)
  - [`collection`](#collection)
  - [`span`](#span)

## [`collection`](collection.h)

The [collection](collection.h) header provides the `Collection` class template, which is a base class for implementing adaptors for standard containers.

- `Collection`: base class for implementing adaptors for standard containers.
- `ArrayCollection`: adaptor for `std::array`.

## [`span`](span.h)

The [span](span.h) header provides the `Span` class template, which is an adaptor for `std::span`.
