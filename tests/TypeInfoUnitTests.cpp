//          Copyright Malcolm Noyes 2013-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "catch.hpp"

#include "any_bridge.hpp"
#include <typeinfo>

namespace TypeInfoUnitTests
{
    class A {};

    TEST_CASE("Require int types give same id", "[types]")
    {
        typedef int InterfaceClass; // doesn't need a real class for this test
        REQUIRE(typeid(int) == typeid(const int&));
        REQUIRE(any_bridge::type_info<InterfaceClass>::type_id<int>() == any_bridge::type_info<InterfaceClass>::type_id<const int&>());
    }

    TEST_CASE("Require class types give same id", "[types]")
    {
        typedef int InterfaceClass; // doesn't need a real class for this test
        REQUIRE(typeid(A) == typeid(const A&));
        REQUIRE(any_bridge::type_info<InterfaceClass>::type_id<A>() == any_bridge::type_info<InterfaceClass>::type_id<const A&>());
        REQUIRE(any_bridge::type_info<InterfaceClass>::type_id<A*>() == any_bridge::type_info<InterfaceClass>::type_id<const A*>());
    }

    TEST_CASE("Require different types do not give same id", "[types]")
    {
        typedef int InterfaceClass; // doesn't need a real class for this test
        REQUIRE(typeid(A) != typeid(const int&));
        REQUIRE(any_bridge::type_info<InterfaceClass>::type_id<A>() != any_bridge::type_info<InterfaceClass>::type_id<const int&>());

        REQUIRE(typeid(int) != typeid(const A&));
        REQUIRE(any_bridge::type_info<InterfaceClass>::type_id<int>() != any_bridge::type_info<InterfaceClass>::type_id<const A&>());
    }

    TEST_CASE("Require different types are not equivalent", "[types]")
    {
        typedef int InterfaceClass; // doesn't need a real class for this test
        /* !(w1 < w2)        // it's not true that w1 < w2
            &&               //and
           !(w2 < w1)        //it's not true that w2 < w1
        */
        bool equivalent = !(any_bridge::type_info<InterfaceClass>::type_id<int>() < any_bridge::type_info<InterfaceClass>::type_id<double>()) &&
                                !(any_bridge::type_info<InterfaceClass>::type_id<double>() < any_bridge::type_info<InterfaceClass>::type_id<int>());
        REQUIRE(!equivalent);

        equivalent = !(any_bridge::type_info<InterfaceClass>::type_id<A>() < any_bridge::type_info<InterfaceClass>::type_id<double>()) &&
                                !(any_bridge::type_info<InterfaceClass>::type_id<double>() < any_bridge::type_info<InterfaceClass>::type_id<A>());
        REQUIRE(!equivalent);

        equivalent = !(any_bridge::type_info<InterfaceClass>::type_id<A>() < any_bridge::type_info<InterfaceClass>::type_id<A*>()) &&
                                !(any_bridge::type_info<InterfaceClass>::type_id<A*>() < any_bridge::type_info<InterfaceClass>::type_id<A>());
        REQUIRE(!equivalent);
    }

    TEST_CASE("Require same types are equivalent", "[types]")
    {
        typedef int InterfaceClass; // doesn't need a real class for this test
        bool equivalent = !(any_bridge::type_info<InterfaceClass>::type_id<int>() < any_bridge::type_info<InterfaceClass>::type_id<int>()) &&
                                !(any_bridge::type_info<InterfaceClass>::type_id<int>() < any_bridge::type_info<InterfaceClass>::type_id<int>());
        REQUIRE(equivalent);

        equivalent = !(any_bridge::type_info<InterfaceClass>::type_id<A>() < any_bridge::type_info<InterfaceClass>::type_id<A>()) &&
                                !(any_bridge::type_info<InterfaceClass>::type_id<A>() < any_bridge::type_info<InterfaceClass>::type_id<A>());
        REQUIRE(equivalent);

        equivalent = !(any_bridge::type_info<InterfaceClass>::type_id<A*>() < any_bridge::type_info<InterfaceClass>::type_id<A*>()) &&
                                !(any_bridge::type_info<InterfaceClass>::type_id<A*>() < any_bridge::type_info<InterfaceClass>::type_id<A*>());
        REQUIRE(equivalent);
    }
}
