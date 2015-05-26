#pragma once

#include "aux_/eval_aux.hpp"
#include "aux_/enabler.hpp"
#include "aux_/tuple_sub.hpp"

#include "apply.hpp"

#include <sprout/tuple.hpp>

#include <cstdint>
#include <type_traits>


    template <class T>
    struct print{
        using type = T;
        char : 16;
    };
    template <class T>
    using print_t = typename print<T>::type;

namespace slash{
    namespace detail{
        template <class...>
        struct tautology : ::std::true_type{
        };

        template <class Holder, class Tuple>
        struct get_element{
        };

        template <
            class T, class Arg, class... Rest,
            typename ::std::enable_if<sizeof...(Rest) == 0>::type *& = aux_::enabler
        >
        constexpr auto apply_to_curry(T x, const ::sprout::tuple<Arg, Rest...> &args)
        {
            return x(::sprout::get<0>(args));
        }

        template <
            class T, class Arg, class... Rest,
            typename ::std::enable_if<sizeof...(Rest) != 0>::type *& = aux_::enabler
        >
        constexpr auto apply_to_curry(T x, const ::sprout::tuple<Arg, Rest...> &args)
        {
            return apply_to_curry(x(::sprout::get<0>(args)), aux_::tuple_sub<1, 1 + sizeof...(Rest)>(args));
        }

        template <class Child, class Param>
        struct call_op{
            template <class T>
            struct callee{
                using type = T;
            };

            template <class A, class B>
            struct callee<call_op<A, B>>{
                using type = typename callee<A>::type;
            };

            Child child;
            Param param;

            constexpr call_op(Child c, Param p)
                : child(c)
                , param(p)
            {
            }

            template <class Param2>
            constexpr call_op<call_op<Child, Param>, Param2>
            operator()(Param2 p) const
            {
                return {*this, p};
            }

            template <
                class... Args,
                typename std::enable_if<
                    !is_curry<
                        typename get_element<
                            typename callee<Child>::type,
                            ::sprout::tuple<Args...>
                        >::type
                    >::value
                >::type *& = aux_::enabler
            >
            constexpr auto eval(const ::sprout::tuple<Args...>  &args) const
            {
                return ::slash::detail::apply((typename callee<Child>::type){}.eval(args), make_args_tuple(args));
            }

            template <
                class... Args,
                typename std::enable_if<
                    is_curry<
                        typename get_element<
                            typename callee<Child>::type,
                            ::sprout::tuple<Args...>
                        >::type
                    >::value
                >::type *& = aux_::enabler
            >
            constexpr auto eval(const ::sprout::tuple<Args...>  &args) const
            {
                return apply_to_curry(
                    (typename callee<Child>::type){}.eval(args),
                    make_args_tuple(args)).eval(args);
            }

            /******/

            template <
                class... Args,
                typename ::std::enable_if<
                    tautology<Args...>::value
                        && !is_call_op<Child>::value
                >::type *& = aux_::enabler
            >
            constexpr auto make_args_tuple(const ::sprout::tuple<Args...> &args) const
            {
                return ::sprout::make_tuple(aux_::eval_aux(param, args));
            }

            template <
                class... Args,
                typename ::std::enable_if<
                    tautology<Args...>::value
                        && is_call_op<Child>::value
                >::type *& = aux_::enabler
            >
            constexpr auto make_args_tuple(const ::sprout::tuple<Args...> &args) const
            {
                return ::sprout::tuple_cat(child.make_args_tuple(args), ::sprout::make_tuple(aux_::eval_aux(param, args)));
            }

        private:


            /*template <class... Params, class... Args>
            constexpr auto eval_(const ::sprout::tuple<Params...>  &params, const ::sprout::tuple<Args...>  &args) const
                -> typename std::enable_if<
                    !is_evaluative<typename callee<Child>::type>::value && is_call_op<Child>::value,
                    decltype(child.eval_(::sprout::tuple_cat(::sprout::make_tuple(aux_::eval_aux(param, args)), params), args))
                >::type
            {
                return child.eval_(::sprout::tuple_cat(::sprout::make_tuple(aux_::eval_aux(param, args)), params), args);
            }

            template <class... Params, class... Args>
            constexpr auto eval_(const ::sprout::tuple<Params...>  &params, const ::sprout::tuple<Args...>  &args) const
                -> typename std::enable_if<
                    !is_evaluative<typename callee<Child>::type>::value && !is_call_op<Child>::value,
                    decltype(apply(child, ::sprout::tuple_cat(::sprout::make_tuple(aux_::eval_aux(param, args), params))))
                >::type
            {
                return apply(child, ::sprout::tuple_cat(::sprout::make_tuple(aux_::eval_aux(param, args), params)));
            }*/
        };

        template <class A, class B>
        struct is_call_op<call_op<A, B>> : ::std::true_type{
        };

        template <::std::int64_t N>
        struct placeholder{
            template <class Param>
            constexpr call_op<placeholder<N>, Param> operator()(const Param &p) const
            {
                return {*this, p};
            }

            template <class... Args>
            constexpr auto eval(const ::sprout::tuple<Args...>  &args) const
            {
                return ::sprout::get<N - 1>(args);
            }
        };

        template <::std::int64_t N, class... Args>
        struct get_element<placeholder<N>, ::sprout::tuple<Args...>>{
            using type = typename ::sprout::tuple_element<N - 1, ::sprout::tuple<Args...>>::type;
        };

        template <::std::int64_t N>
        struct is_placeholder<placeholder<N>> : ::std::true_type{
        };
    }
}
