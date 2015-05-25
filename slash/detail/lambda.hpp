#pragma once

#include "arg.hpp"

#include <tuple>
#include <type_traits>

namespace slash{
    namespace detail{
        template <std::int64_t N>
        struct lambda_{
            static constexpr std::int64_t value = N;
        };

        template <std::int64_t N>
        constexpr lambda_<N> lambda(arg<N> n)
        {
            return {};
        }

        template <class Lambda, class Expr, class... Args>
        struct partial_evaluator{
            Expr expr;
            std::tuple<Args...> args;

            constexpr partial_evaluator(Expr e, std::tuple<Args...> a)
                : expr(e)
                , args(a)
            {
            }

            template <typename... Rest, typename = typename std::enable_if<(sizeof...(Args) + sizeof...(Rest)) == Lambda::value>::type>
            constexpr auto operator()(Rest... rest) const
                -> decltype(expr.eval(std::tuple_cat(args, std::make_tuple(std::forward<Rest>(rest)...))))
            {
                return expr.eval(std::tuple_cat(args, std::make_tuple(std::forward<Rest>(rest)...)));
            }

            template <typename... Rest, typename = typename std::enable_if<(sizeof...(Args) + sizeof...(Rest)) != Lambda::value>::type>
            constexpr auto operator()(Rest... rest) const
                -> partial_evaluator<Lambda, Expr, Args..., Rest...>
            {
                static_assert((sizeof...(Args) + sizeof...(Rest)) < Lambda::value, "too many arguments");
                return {expr, std::tuple_cat(args, std::make_tuple(std::forward<Rest>(rest)...))};
            }
        };

        template <class Lambda, class Expr>
        struct evaluator{
            Expr expr;

            constexpr evaluator(Expr e) : expr(e)
            {
            }

            template <typename... Args, typename = typename std::enable_if<sizeof...(Args) == Lambda::value>::type>
            constexpr auto operator()(Args... args) const
                -> decltype(expr.eval(std::make_tuple(std::forward<Args>(args)...)))
            {
                return expr.eval(std::make_tuple(std::forward<Args>(args)...));
            }

            template <typename... Args, typename = typename std::enable_if<sizeof...(Args) != Lambda::value>::type>
            constexpr auto operator()(Args... args) const
                -> partial_evaluator<Lambda, Expr, Args...>
            {
                static_assert(sizeof...(Args) < Lambda::value, "too many arguments");
                return {expr, std::make_tuple(std::forward<Args>(args)...)};
            }
        };

        template <std::int64_t N, class Expr>
        constexpr evaluator<lambda_<N>, Expr> operator,(lambda_<N>, Expr e)
        {
            return {e};
        }
    }
}