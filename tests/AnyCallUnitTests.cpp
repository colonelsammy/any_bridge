//          Copyright Malcolm Noyes 2013-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "catch.hpp"
#include "any_bridge.hpp"

namespace ab = any_bridge;

namespace
{
    struct A
    {
        explicit A(int t)
            : v(t)
        {}
        int value() const {return v;}
        int v;
    };
    struct TestCallInterface
    {
        virtual ~TestCallInterface() {}
        virtual int f0() const = 0;
        virtual int f1(int) const = 0;
        virtual int f2(int t1, double t2) const = 0;
        virtual int f3(int t1, double t2, int t3) const = 0;
        virtual int f4(int t1, double t2, int t3, int t4) const = 0;
        virtual int f5(int t1, double t2, int t3, int t4, int t5) const = 0;
        virtual int f6(int t1, double t2, int t3, int t4, int t5, const int& t6) const = 0;
        virtual int f7(int t1, double t2, int t3, int t4, int t5, const int& t6, int& t7) const = 0;
        virtual int f8(int t1, double t2, int t3, int t4, int t5, const int& t6, int& t7, const char* t8) const = 0;
        virtual int f9(int t1, double t2, int t3, int t4, int t5, const int& t6, int& t7, const char* t8, A t9) const = 0;
        virtual int f10(int t1, double t2, int t3, int t4, int t5, const int& t6, int& t7, const char* t8, A t9, A& t10) const = 0;
    };
}

namespace any_bridge
{
    //
    // forwarder<any<...> > doesn't *need* any methods defined....
    // (unless you want to use them...)
    //
    template <>
    class forwarder<any<interfaces<TestCallInterface> > >
    {
    public:
        // no methods
    };

    template <typename Derived, typename Base, typename ValueType>
    class value_type_operations : public Base
    {
    public:
        virtual int f0() const { return static_cast<const Derived*>(this)->held;}
        virtual int f1(int t1) const {return static_cast<const Derived*>(this)->held + t1;}
        virtual int f2(int t1, double t2) const {return static_cast<const Derived*>(this)->held + t1 + static_cast<int>(t2);}
        virtual int f3(int t1, double t2, int t3) const {return static_cast<const Derived*>(this)->held + t1 + static_cast<int>(t2) + t3;}
        virtual int f4(int t1, double t2, int t3, int t4) const
        {
            return static_cast<const Derived*>(this)->held + t1 + static_cast<int>(t2) + t3 + t4;
        }
        virtual int f5(int t1, double t2, int t3, int t4, int t5) const
        {
            return static_cast<const Derived*>(this)->held + t1 + static_cast<int>(t2) + t3 + t4 + t5;
        }
        virtual int f6(int t1, double t2, int t3, int t4, int t5, const int& t6) const
        {
            return static_cast<const Derived*>(this)->held + t1 + static_cast<int>(t2) + t3 + t4 + t5 + t6;
        }
        virtual int f7(int t1, double t2, int t3, int t4, int t5, const int& t6, int& t7) const
        {
            int result = static_cast<const Derived*>(this)->held + t1 + static_cast<int>(t2) + t3 + t4 + t5 + t6 + t7;
            t7 = -7;
            return result;
        }
        virtual int f8(int t1, double t2, int t3, int t4, int t5, const int& t6, int& t7, const char* t8) const
        {
            int t8v = atoi(t8);
            int result = static_cast<const Derived*>(this)->held + t1 + static_cast<int>(t2) + t3 + t4 + t5 + t6 + t7 + t8v;
            t7 = -8;
            return result;
        }
        virtual int f9(int t1, double t2, int t3, int t4, int t5, const int& t6, int& t7, const char* t8, A t9) const
        {
            int t8v = atoi(t8);
            int result = static_cast<const Derived*>(this)->held + t1 + static_cast<int>(t2) + t3 + t4 + t5 + t6 + t7 + t8v + t9.value();
            t7 = -9;
            return result;
        }
        virtual int f10(int t1, double t2, int t3, int t4, int t5, const int& t6, int& t7, const char* t8, A t9, A& t10) const
        {
            int t8v = atoi(t8);
            int result = static_cast<const Derived*>(this)->held + t1 + static_cast<int>(t2) + t3 + t4 + t5 + t6 + t7 + t8v + t9.value() + t10.value();
            t7 = -10;
            t10.v = -10;
            return result;
        }
    };
}

namespace AnyCallUnitTests
{
    TEST_CASE("Require call zero params calls f0", "[call]")
    {
        ab::any<ab::interfaces<TestCallInterface> > a(1);
        REQUIRE(a.call(&TestCallInterface::f0) == 1);
    }

    TEST_CASE("Require call one param calls f1", "[call]")
    {
        ab::any<ab::interfaces<TestCallInterface> > a(1);
        REQUIRE(a.call(&TestCallInterface::f1).with(2) == 3);
    }

    TEST_CASE("Require call two params calls f2", "[call]")
    {
        ab::any<ab::interfaces<TestCallInterface> > a(1);
        REQUIRE(a.call(&TestCallInterface::f2).with(2, 3.0) == 6);
    }

    TEST_CASE("Require call 3 params calls f3", "[call]")
    {
        ab::any<ab::interfaces<TestCallInterface> > a(1);
        REQUIRE(a.call(&TestCallInterface::f3).with(2, 3.0, 5) == 11);
    }

    TEST_CASE("Require call 4 params calls f4", "[call]")
    {
        ab::any<ab::interfaces<TestCallInterface> > a(1);
        REQUIRE(a.call(&TestCallInterface::f4).with(2, 3.0, 5, 7) == 18);
    }

    TEST_CASE("Require call 5 params calls f5", "[call]")
    {
        ab::any<ab::interfaces<TestCallInterface> > a(1);
        REQUIRE(a.call(&TestCallInterface::f5).with(2, 3.0, 5, 7, 11) == 29);
    }

    TEST_CASE("Require call 6 params calls f6", "[call]")
    {
        ab::any<ab::interfaces<TestCallInterface> > a(1);
        REQUIRE(a.call(&TestCallInterface::f6).with(2, 3.0, 5, 7, 11, 13) == 42);
    }

    TEST_CASE("Require call 7 params calls f7", "[call]")
    {
        ab::any<ab::interfaces<TestCallInterface> > a(1);
        int t7 = 17;
        REQUIRE(a.call(&TestCallInterface::f7).with(2, 3.0, 5, 7, 11, 13, t7) == 59);
        REQUIRE(t7 == -7);
    }

    TEST_CASE("Require call 8 params calls f8", "[call]")
    {
        ab::any<ab::interfaces<TestCallInterface> > a(1);
        int t7 = 17;
        REQUIRE(a.call(&TestCallInterface::f8).with(2, 3.0, 5, 7, 11, 13, t7, "19") == 78);
        REQUIRE(t7 == -8);
    }

    TEST_CASE("Require call 9 params calls f9", "[call]")
    {
        ab::any<ab::interfaces<TestCallInterface> > a(1);
        int t7 = 17;
        REQUIRE(a.call(&TestCallInterface::f9).with(2, 3.0, 5, 7, 11, 13, t7, "19", A(23)) == 101);
        REQUIRE(t7 == -9);
    }

    TEST_CASE("Require call 10 params calls f10", "[call]")
    {
        ab::any<ab::interfaces<TestCallInterface> > a(1);
        int t7 = 17;
        A t10(29);
        REQUIRE(a.call(&TestCallInterface::f10).with(2, 3.0, 5, 7, 11, 13, t7, "19", A(23), t10) == 130);
        REQUIRE(t7 == -10);
        REQUIRE(t10.value() == -10);
    }
}
