#pragma once

#include <tuple>
#include <cstdint>

namespace slash{
    namespace detail{
        template <class Func, class... ArgTypes>
        struct call_op{
            template <typename... Args>
            constexpr auto eval(const std::tuple<Args...> &t) const
                -> decltype(Func().eval(t)(ArgTypes().eval(t)...))
            {
                return Func().eval(t)(ArgTypes().eval(t)...);
            }

            template <class... Args>
            constexpr call_op<call_op<Func, ArgTypes...>, Args...> operator()(Args...) const
            {
                return {};
            }
        };

        template <std::int64_t N>
        struct arg{
            template <typename... Args>
            constexpr auto eval(const std::tuple<Args...> &t) const
                -> typename std::tuple_element<N - 1, std::tuple<Args...>>::type
            {
                return std::get<N - 1>(t);
            }

            template <class... Args>
            constexpr call_op<arg<N>, Args...> operator()(Args...) const
            {
                return {};
            }
        };

        template <typename T>
        struct is_placeholder{
            static constexpr bool value = false;
        };

        template <std::int64_t N>
        struct is_placeholder<arg<N>>{
            static constexpr bool value = true;
        };
    }
}