#pragma once

#include "placeholders.hpp"
#include "apply.hpp"
#include "aux_/eval_aux.hpp"

#include <sprout/tuple.hpp>
#include <sprout/index_tuple.hpp>

#include <cstdint>
#include <type_traits>

namespace slash{
    namespace detail{
        // parameter: formal parameter
        // argument : actual argument

        template <::std::int64_t N, class Func, class... Params>
        struct curry_evaluator{
            Func func;
            ::sprout::tuple<Params...> params;

            constexpr curry_evaluator(Func f, ::sprout::tuple<Params...> p)
                : func(f)
                , params(p)
            {
            }

            template <class... Args>
            constexpr auto eval(const ::sprout::tuple<Args...> &args) const
            {
                return eval_impl(args, ::sprout::index_range<0, sizeof...(Params)>::make());
            }

        private:
            template <class... Args, ::sprout::index_t... I>
            constexpr auto eval_impl(const ::sprout::tuple<Args...> &args, ::sprout::index_tuple<I...>&&) const
            {
                return func(aux_::eval_aux(::sprout::get<I>(params), args)...);
            }
        };

        template <::std::int64_t N, class Func, class... Params>
        struct is_evaluative<curry_evaluator<N, Func, Params...>> : ::std::true_type{
        };

        template <::std::int64_t N, class Func, class... Params>
        struct curry{
            using parameters_type = ::sprout::tuple<Params...>;

            Func func;
            ::sprout::tuple<Params...> params;

            constexpr curry(Func f) : func(f)
            {
            }

            constexpr curry(Func f, parameters_type p)
                : func(f)
                , params(p)
            {
            }

            template <class Param>
            constexpr
            typename ::std::enable_if<
                N != (sizeof...(Params) + 1),
                curry<N, Func, Params..., Param>
            >::type operator()(Param p) const
            {
                return {func, ::sprout::tuple_cat(params, ::sprout::make_tuple(p))};
            }

            template <class Param>
            constexpr
            typename ::std::enable_if<
                N == (sizeof...(Params) + 1),
                curry_evaluator<N, Func, Params..., Param>
            >::type operator()(Param p) const
            {
                return {func, ::sprout::tuple_cat(params, ::sprout::make_tuple(p))};
            }

            template <class... Args>
            constexpr curry<N, Func, Params...> eval(const ::sprout::tuple<Args...> &args)
            {
                return *this;
            }
        };

        template <class Func>
        struct curry<0, Func>{
            Func func;

            constexpr curry(Func f) : func(f)
            {
            }

            template <class... Args>
            constexpr auto eval(const ::sprout::tuple<Args...> &args) const
            {
                return func();
            }
        };

        template <class Func>
        struct is_curry<curry<0, Func>> : ::std::false_type{
        };

        template <::std::int64_t N, class Func, class... Params>
        struct is_curry<curry<N, Func, Params...>> : ::std::true_type{
        };

        template <::std::int64_t N, class Func>
        constexpr curry<N, Func> fn(Func f)
        {
            return {f};
        }

        template <::std::int64_t N, class Func>
        constexpr curry<N, Func> fn(placeholder<N>, Func f)
        {
            return {f};
        }
    }
}
