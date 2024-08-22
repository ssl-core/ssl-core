import constants from "../config/constants";

export const fuzzyEqual = (a: number, b: number, epsilon = constants.epsilon) =>
  Math.abs(a - b) < epsilon;

export const fuzzyLessThan = (
  a: number,
  b: number,
  epsilon = constants.epsilon
) => a < b + epsilon;

export const fuzzyGreaterThan = (
  a: number,
  b: number,
  epsilon = constants.epsilon
) => a > b - epsilon;

export const fuzzyLessThanOrEqual = (
  a: number,
  b: number,
  epsilon = constants.epsilon
) => a < b + epsilon || fuzzyEqual(a, b, epsilon);

export const fuzzyGreaterThanOrEqual = (
  a: number,
  b: number,
  epsilon = constants.epsilon
) => a > b - epsilon || fuzzyEqual(a, b, epsilon);

export const fuzzyNotEqual = (
  a: number,
  b: number,
  epsilon = constants.epsilon
) => !fuzzyEqual(a, b, epsilon);
