#include <iostream>
#include <string>

#include "slash/arg.hpp"
#include "slash/lambda.hpp"

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

int main()
{
    using namespace slash::literal;
    using namespace slash;

    constexpr auto s = (lambda(3_), 1_(3_)(2_(3_)));
    constexpr auto k = (lambda(2_), 1_);
    constexpr auto i = (lambda(1_), 1_);

    constexpr auto succ = (lambda(3_), 2_(1_(2_, 3_)));
    constexpr auto add = (lambda(4_), 1_(3_, 2_(3_, 4_)));

    constexpr auto pred = (lambda(6_), 4_(1_(5_))(2_(6_))(3_))((lambda(3_), 3_(2_(1_))), k, i);

    constexpr auto _0 = (lambda(2_), 2_);

    std::cout << type_name(s(k)(k)) << std::endl;
    std::cout << s(k)(k)(1) << std::endl;

    std::cout << succ(succ(_0))([](int n){return n + 1;})(0) << std::endl;
    std::cout << add(succ(_0))(succ(succ(_0)))([](int n){return n + 1;})(0) << std::endl;
    std::cout << pred(pred(succ(succ(_0))))([](int n){return n + 1;})(0) << std::endl;
}
