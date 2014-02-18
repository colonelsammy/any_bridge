//          Copyright Malcolm Noyes 2013-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ANY_BRIDGE_CALL_WITH_HPP_INCLUDED
#define ANY_BRIDGE_CALL_WITH_HPP_INCLUDED

#include "member_function_traits.hpp"

namespace any_bridge
{
    // interface forwarding, allow up to 10 params
    template <typename ObjectType, typename Function, int Arity>
    struct call_with;

    template <typename ObjectType, typename Function>
    struct call_with<ObjectType, Function, 0>
    {
        call_with(ObjectType* obj, Function f)
            : content(obj), fn(f)
        {}
        // return result type without additional function call if no params...
        operator typename member_function_traits<Function>::result_type ()
        {
            return (content->*fn)();
        }
        ObjectType* content;
        Function fn;
    };

    template <typename ObjectType, typename Function>
    struct call_with<ObjectType, Function, 1>
    {
        call_with(ObjectType* obj, Function f)
            : content(obj), fn(f)
        {}
        typename member_function_traits<Function>::result_type with(
            typename member_function_traits<Function>::arg1_type t1)
        {
            return (content->*fn)(t1);
        }
        ObjectType* content;
        Function fn;
    };

    template <typename ObjectType, typename Function>
    struct call_with<ObjectType, Function, 2>
    {
        call_with(ObjectType* obj, Function f)
            : content(obj), fn(f)
        {}
        typename member_function_traits<Function>::result_type with(
            typename member_function_traits<Function>::arg1_type t1,
            typename member_function_traits<Function>::arg2_type t2)
        {
            return (content->*fn)(t1, t2);
        }
        ObjectType* content;
        Function fn;
    };

    template <typename ObjectType, typename Function>
    struct call_with<ObjectType, Function, 3>
    {
        call_with(ObjectType* obj, Function f)
            : content(obj), fn(f)
        {}
        typename member_function_traits<Function>::result_type with(
            typename member_function_traits<Function>::arg1_type t1,
            typename member_function_traits<Function>::arg2_type t2,
            typename member_function_traits<Function>::arg3_type t3)
        {
            return (content->*fn)(t1, t2, t3);
        }
        ObjectType* content;
        Function fn;
    };

    template <typename ObjectType, typename Function>
    struct call_with<ObjectType, Function, 4>
    {
        call_with(ObjectType* obj, Function f)
            : content(obj), fn(f)
        {}
        typename member_function_traits<Function>::result_type with(
            typename member_function_traits<Function>::arg1_type t1,
            typename member_function_traits<Function>::arg2_type t2,
            typename member_function_traits<Function>::arg3_type t3,
            typename member_function_traits<Function>::arg4_type t4)
        {
            return (content->*fn)(t1, t2, t3, t4);
        }
        ObjectType* content;
        Function fn;
    };

    template <typename ObjectType, typename Function>
    struct call_with<ObjectType, Function, 5>
    {
        call_with(ObjectType* obj, Function f)
            : content(obj), fn(f)
        {}
        typename member_function_traits<Function>::result_type with(
            typename member_function_traits<Function>::arg1_type t1,
            typename member_function_traits<Function>::arg2_type t2,
            typename member_function_traits<Function>::arg3_type t3,
            typename member_function_traits<Function>::arg4_type t4,
            typename member_function_traits<Function>::arg5_type t5)
        {
            return (content->*fn)(t1, t2, t3, t4, t5);
        }
        ObjectType* content;
        Function fn;
    };

    template <typename ObjectType, typename Function>
    struct call_with<ObjectType, Function, 6>
    {
        call_with(ObjectType* obj, Function f)
            : content(obj), fn(f)
        {}
        typename member_function_traits<Function>::result_type with(
            typename member_function_traits<Function>::arg1_type t1,
            typename member_function_traits<Function>::arg2_type t2,
            typename member_function_traits<Function>::arg3_type t3,
            typename member_function_traits<Function>::arg4_type t4,
            typename member_function_traits<Function>::arg5_type t5,
            typename member_function_traits<Function>::arg6_type t6)
        {
            return (content->*fn)(t1, t2, t3, t4, t5, t6);
        }
        ObjectType* content;
        Function fn;
    };

    template <typename ObjectType, typename Function>
    struct call_with<ObjectType, Function, 7>
    {
        call_with(ObjectType* obj, Function f)
            : content(obj), fn(f)
        {}
        typename member_function_traits<Function>::result_type with(
            typename member_function_traits<Function>::arg1_type t1,
            typename member_function_traits<Function>::arg2_type t2,
            typename member_function_traits<Function>::arg3_type t3,
            typename member_function_traits<Function>::arg4_type t4,
            typename member_function_traits<Function>::arg5_type t5,
            typename member_function_traits<Function>::arg6_type t6,
            typename member_function_traits<Function>::arg7_type t7)
        {
            return (content->*fn)(t1, t2, t3, t4, t5, t6, t7);
        }
        ObjectType* content;
        Function fn;
    };

    template <typename ObjectType, typename Function>
    struct call_with<ObjectType, Function, 8>
    {
        call_with(ObjectType* obj, Function f)
            : content(obj), fn(f)
        {}
        typename member_function_traits<Function>::result_type with(
            typename member_function_traits<Function>::arg1_type t1,
            typename member_function_traits<Function>::arg2_type t2,
            typename member_function_traits<Function>::arg3_type t3,
            typename member_function_traits<Function>::arg4_type t4,
            typename member_function_traits<Function>::arg5_type t5,
            typename member_function_traits<Function>::arg6_type t6,
            typename member_function_traits<Function>::arg7_type t7,
            typename member_function_traits<Function>::arg8_type t8)
        {
            return (content->*fn)(t1, t2, t3, t4, t5, t6, t7, t8);
        }
        ObjectType* content;
        Function fn;
    };

    template <typename ObjectType, typename Function>
    struct call_with<ObjectType, Function, 9>
    {
        call_with(ObjectType* obj, Function f)
            : content(obj), fn(f)
        {}
        typename member_function_traits<Function>::result_type with(
            typename member_function_traits<Function>::arg1_type t1,
            typename member_function_traits<Function>::arg2_type t2,
            typename member_function_traits<Function>::arg3_type t3,
            typename member_function_traits<Function>::arg4_type t4,
            typename member_function_traits<Function>::arg5_type t5,
            typename member_function_traits<Function>::arg6_type t6,
            typename member_function_traits<Function>::arg7_type t7,
            typename member_function_traits<Function>::arg8_type t8,
            typename member_function_traits<Function>::arg9_type t9)
        {
            return (content->*fn)(t1, t2, t3, t4, t5, t6, t7, t8, t9);
        }
        ObjectType* content;
        Function fn;
    };

    template <typename ObjectType, typename Function>
    struct call_with<ObjectType, Function, 10>
    {
        call_with(ObjectType* obj, Function f)
            : content(obj), fn(f)
        {}
        typename member_function_traits<Function>::result_type with(
            typename member_function_traits<Function>::arg1_type t1,
            typename member_function_traits<Function>::arg2_type t2,
            typename member_function_traits<Function>::arg3_type t3,
            typename member_function_traits<Function>::arg4_type t4,
            typename member_function_traits<Function>::arg5_type t5,
            typename member_function_traits<Function>::arg6_type t6,
            typename member_function_traits<Function>::arg7_type t7,
            typename member_function_traits<Function>::arg8_type t8,
            typename member_function_traits<Function>::arg9_type t9,
            typename member_function_traits<Function>::arg10_type t10)
        {
            return (content->*fn)(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
        }
        ObjectType* content;
        Function fn;
    };
}

#endif // ANY_BRIDGE_CALL_WITH_HPP_INCLUDED
