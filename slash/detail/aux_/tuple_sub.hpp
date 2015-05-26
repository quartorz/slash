#pragma once

#include <sprout/tuple.hpp>
#include <sprout/index_tuple.hpp>

namespace slash{
    namespace detail{
        namespace aux_{
            template <class... T, ::sprout::index_t... I>
            constexpr auto tuple_sub_impl(const ::sprout::tuple<T...> &x, ::sprout::index_tuple<I...>&&)
            {
                return ::sprout::make_tuple(::sprout::get<I>(x)...);
            }

            template <::sprout::index_t Begin, ::sprout::index_t End, class... T>
            constexpr auto tuple_sub(const ::sprout::tuple<T...> &x)
            {
                return tuple_sub_impl(x, ::sprout::index_range<Begin, End>::make());
            }
        }
    }
}
