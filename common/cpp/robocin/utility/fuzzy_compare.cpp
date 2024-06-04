#include "robocin/utility/fuzzy_compare.h"

namespace robocin {

template class FuzzyIsZero<float>;
template class FuzzyIsZero<double>;
template class FuzzyIsZero<long double>;

template class FuzzyEqualTo<float>;
template class FuzzyEqualTo<double>;
template class FuzzyEqualTo<long double>;

template class FuzzyNotEqualTo<float>;
template class FuzzyNotEqualTo<double>;
template class FuzzyNotEqualTo<long double>;

template class FuzzyThreeWay<float>;
template class FuzzyThreeWay<double>;
template class FuzzyThreeWay<long double>;

template class FuzzyLess<float>;
template class FuzzyLess<double>;
template class FuzzyLess<long double>;

template class FuzzyLessEqual<float>;
template class FuzzyLessEqual<double>;
template class FuzzyLessEqual<long double>;

template class FuzzyGreater<float>;
template class FuzzyGreater<double>;
template class FuzzyGreater<long double>;

template class FuzzyGreaterEqual<float>;
template class FuzzyGreaterEqual<double>;
template class FuzzyGreaterEqual<long double>;

} // namespace robocin
