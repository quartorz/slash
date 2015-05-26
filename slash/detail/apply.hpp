#pragma once

#include <sprout/tuple.hpp>
#include <sprout/index_tuple.hpp>

namespace slash{
    namespace detail{
        template <class Func, class... Args, ::sprout::index_t... I>
        constexpr auto apply_impl(Func func, ::sprout::tuple<Args...> args, ::sprout::index_tuple<I...>)
        {
            return func(::sprout::get<I>(args)...);
        }

        template <class Func, class... Args>
        constexpr auto apply(Func func, const ::sprout::tuple<Args...> &args)
        {
            return apply_impl(func, args, ::sprout::index_range<0, sizeof...(Args)>::make());
        }
    }
}
