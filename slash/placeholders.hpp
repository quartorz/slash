#pragma once

#include "detail/placeholders.hpp"

#include <cstdint>
#include <type_traits>

namespace slash{
    using detail::placeholder;

    namespace literals{
        namespace detail{
            constexpr ::std::int64_t to_int(::std::int64_t n)
            {
                return ('0' <= n && n <= '9')
                    ? n - '0'
                    : (n & 0xf) + 9;
            }

            constexpr ::std::int64_t concat(::std::int64_t base, ::std::int64_t n)
            {
                return n;
            }

            template <typename... Rest>
            constexpr ::std::int64_t concat(::std::int64_t base, ::std::int64_t a, ::std::int64_t b, Rest... rest)
            {
                return concat(base, a * base + b, rest...);
            }

            template <char A, char B, char... Rest>
            constexpr typename ::std::enable_if<
                A == '0' && (B == 'x' || B == 'X'),
                ::slash::detail::placeholder<concat(16, to_int(Rest)...)>
            >::type parse()
            {
                return {};
            }

            template <char A, char B, char... Rest>
            constexpr typename ::std::enable_if<
                A == '0' && !(B == 'x' || B == 'X'),
                ::slash::detail::placeholder<concat(8, to_int(B), to_int(Rest)...)>
            >::type parse()
            {
                return {};
            }

            template <char A>
            constexpr typename ::std::enable_if<
                A == '0',
                ::slash::detail::placeholder<0>
            >::type parse()
            {
                return {};
            }

            template <char A, char... Rest>
            constexpr typename ::std::enable_if<
                A != '0',
                ::slash::detail::placeholder<concat(10, to_int(A), to_int(Rest)...)>
            >::type parse()
            {
                return {};
            }
        }

        template <char... Chars>
        constexpr decltype(detail::parse<Chars...>()) operator"" _()
        {
            return {};
        }
    }
}
