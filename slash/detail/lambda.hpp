#pragma once

#include "curry.hpp"
#include "placeholders.hpp"

#include <sprout/tuple.hpp>
#include <sprout/index_tuple.hpp>

#include <cstdint>

namespace slash{
    namespace detail{
        template <::std::int64_t N, class Expr>
        struct lambda_{
            Expr expr;

            constexpr lambda_(Expr e) : expr(e)
            {
            }

            template <class... Args>
            constexpr auto operator()(Args... args) const
            {
                return expr.eval(::sprout::make_tuple(args...));
            }
        };

        template <::std::int64_t N, class Expr, ::sprout::index_t... I>
        constexpr
        curry_evaluator<
            N, lambda_<N, Expr>,
            placeholder<I>...
        >
        lambda_impl(const Expr &expr, ::sprout::index_tuple<I...>&&)
        {
            return {lambda_<N, Expr>(expr), ::sprout::make_tuple(placeholder<I>()...)};
        }

        template <::std::int64_t N, class Expr>
        constexpr auto lambda(const Expr &expr)
        {
            return lambda_<N, Expr>(expr);
            //return fn<N>(lambda_<N, Expr>(expr));
            //return lambda_impl<N>(expr, ::sprout::index_range<1, N + 1>::make());
        }
    }
}
