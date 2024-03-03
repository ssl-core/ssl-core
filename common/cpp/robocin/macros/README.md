# macros

This directory contains a collection of macros that are used throughout the project.

## Table of Contents

- [macros](#macros)
  - [Table of Contents](#table-of-contents)
  - [`builder`](#builder)

## [`builder`](builder.h)

The [builder](builder.h) header provides a set of macros for building classes with a fluent interface.

- `BuildableWith`: define a class as buildable with a specific builder (a nested class). It provides functionality for converting an instance of the class to its builder:

  - `toBuilder() &&`: Converts an instance of the class to its builder using move semantics;
  - `toBuilder() const&`: Converts a const instance of the class to its builder;
  - Usage:
    ```cpp
    class Buildable final {
      BuildableWith(Builder);
      // ...
    };
    ```

- `BuilderOf`: define a builder for a buildable class. It provides a static method from to create a builder from an instance of the buildable class and methods to build the final object:

  - `from(T&& instance)`: Creates a builder from an instance of the buildable class;
  - `build() &&`: Builds the final object using move semantics;
  - `build() const&`: Builds the final object from a const instance;
  - Usage:
    ```cpp
    class Buildable::Builder final {
      BuilderOf(Buildable);
      // ...
    };
    ```

- `Getter`: generate getter methods for a class member. It provides methods to retrieve the value, check if it exists, and get an optional value for the specified member:

  - `get##field() const`: Retrieves the value of the specified member;
  - `is##field() const`: Checks if the specified boolean member is `true` (if applicable);
  - `has##field() const`: Checks if the optional member has a value (if applicable);
  - `getOptional##field() const`: Retrieves the optional value of the specified member (if applicable);
  - Usage:
    ```cpp
    // ...
    Getter(Integers, std::vector<int>);
    // ...
    ```

- `Setter`: generate setter methods for a class member. It provides methods to set the value of the specified member:

  - `set##name(const type& cref_value)`: Sets the value of the specified member with a const reference.
  - `set##name(type&& rvalue)`: Sets the value of the specified member with an rvalue reference.
  - `set##name(T&& xvalue)`: Sets the value of the specified member with a generic value.
  - Usage:
    ```cpp
    // ...
    Setter(Integers);
    // ...
    ```

> **Note**: All setter methods return a reference to the instance of the class to allow chaining.

- `Adder`: generate methods for adding elements to a container-type class member. It supports both lvalue and rvalue references for adding elements, and add methods to clear the container.
  - `add##name(const type& cref_value)`: Adds an element to the container with a const reference.
  - `add##name(type&& rvalue)`: Adds an element to the container with an rvalue reference.
  - `add##name(Args&&... args)`: Emplaces elements into the container.
  - `clear##name##s()`: Clears the container.
  - Usage:
    ```cpp
    // ...
    Adder(Integer); // singular, without 's'.
    // ...
    ```

> **Note**: All adder methods return a reference to the instance of the class to allow chaining.
