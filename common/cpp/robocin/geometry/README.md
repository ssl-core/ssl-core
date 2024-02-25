# geometry

A collection of geometric classes.

## Table of Contents
- [geometry](#geometry)
  - [Table of Contents](#table-of-contents)
  - [`Point2D`](#point2d)
    - [Member Types](#member-types)
    - [Deduction Guides](#deduction-guides)
  - [`Point3D`](#point3d)
    - [Member Types](#member-types-1)
    - [Deduction Guides](#deduction-guides-1)

## [`Point2D`](point2d.h)

The `Point2D` templated struct represents a 2-dimensional point (vector) in a Cartesian coordinate system. It provides various methods and operators for manipulating and performing calculations with 2d-points.

### Member Types

- `value_type`: The type of the point's coordinates;

### Deduction Guides

The following deduction guides are provided:

- `Point2D() -> Point2D<double>`: Constructs a `Point2D` with `double` coordinates when no arguments are provided;
- `Point2D(T, U) -> Point2D<std::common_type_t<T, U>>`: This deduction guide ensures that when constructing a `Point2D`
  with different coordinate types `T` and `U`, the resulting `Point2D` type will have a coordinate type that is the
  common type of `T` and `U`;

## [`Point3D`](point3d.h)

The `Point3D` templated struct represents a 3-dimensional point (vector) in a Cartesian coordinate system. It provides various methods and operators for manipulating and performing calculations with 3d-points.

### Member Types

- `value_type`: The type of the point's coordinates;

### Deduction Guides

The following deduction guides are provided:

- `Point3D() -> Point3D<double>`: Constructs a `Point3D` with `double` coordinates when no arguments are provided;
- `Point3D(T, U, V) -> Point3D<std::common_type_t<T, U, V>>`: This deduction guide ensures that when constructing a `Point3D`
  with different coordinate types `T`, `U` and `V`, the resulting `Point3D` type will have a coordinate type that is the
  common type of `T`, `U` and `V`;
