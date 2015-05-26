#pragma once

#include <sprout/tuple.hpp>

namespace slash{
    template <class T, class... Args>
    constexpr auto eval(const T &x, Args... args)
    {
        return x.eval(::sprout::make_tuple(args...));
    }
}
