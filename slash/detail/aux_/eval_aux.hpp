#pragma once

#include <sprout/tuple.hpp>
#include <sprout/index_tuple.hpp>

#include <cstdint>
#include <type_traits>

namespace slash{
    namespace detail{
        template <class T>
        struct is_placeholder : ::std::false_type{
        };

        template <class T>
        struct is_call_op : ::std::false_type{
        };

        template <class T>
        struct is_evaluative
            : ::std::conditional<
                is_placeholder<T>::value || is_call_op<T>::value,
                ::std::true_type,
                ::std::false_type
            >::type
        {
        };

        template <class T>
        struct is_curry : ::std::false_type{
        };

        namespace aux_{
            template <class T, class... Args>
            constexpr auto
            eval_aux(const T &x, const ::sprout::tuple<Args...> &args)
                -> typename ::std::enable_if<
                    ::slash::detail::is_evaluative<T>::value,
                    decltype(x.eval(args))
                >::type
            {
                return x.eval(args);
            }

            template <class T, class... Args>
            constexpr auto
            eval_aux(const T &x, const ::sprout::tuple<Args...> &args)
                -> typename ::std::enable_if<
                    !::slash::detail::is_evaluative<T>::value,
                    T
                >::type
            {
                return x;
            }
        }
    }
}