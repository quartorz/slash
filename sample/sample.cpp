#include <iostream>

#include "slash/curry.hpp"
#include "slash/placeholders.hpp"
#include "slash/lambda.hpp"
#include "slash/eval.hpp"

#include <typeinfo>

#if defined(_MSC_VER)
template <class T>
const char *type_name(T x)
{
    return typeid(x).name();
}
#else
#include <cxxabi.h>

template <class T>
const char *type_name(T x)
{
    return abi::__cxa_demangle(typeid(x).name(), nullptr, nullptr, nullptr);
}
#endif

int inc(int n)
{
    return n + 1;
}

int main()
{
    using namespace slash;
    using namespace slash::literals;

    constexpr auto s = lambda<3>(1_(3_)(2_(3_)));
    constexpr auto k = lambda<2>(1_);
    constexpr auto i = lambda<1>(1_);

    constexpr auto S = fn<3>(s);
    constexpr auto K = fn<2>(k);
    constexpr auto I = fn<1>(i);

    constexpr auto succ = lambda<3>(2_(1_(2_)(3_)));
    constexpr auto Succ = fn<3>(succ);

    constexpr auto _0 = fn<2>(lambda<2>(2_));
    constexpr auto _1 = fn<2>(lambda<2>(Succ(_0)(1_)(2_)));

    constexpr auto f = lambda<1>(i(1_));
    constexpr auto g = lambda<1>(S(K)(K)(1_));

    std::cout << g(10) << std::endl;
    std::cout << lambda<2>(_1(1_)(2_))(fn<1>(inc), 0) << std::endl;
}
