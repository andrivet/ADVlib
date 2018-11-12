
#include "ADVcallback.h"
#include "catch.hpp"

using namespace adv;

using MyCallback = Callback<int(*)(int, int)>;

namespace
{
    struct Class1
    {
        static int static_method(int p1, int p2)
        {
            b = p1;
            return p2;
        }

        int member_method(int p1, int p2)
        {
            a = p1;
            return p2;
        }

        int this_method(int p1, int p2)
        {
            this->a = p1;
            return p2;
        }

        int a = 0;
        static int b;
    };

    int Class1::b = 0;

    int s = 0;
    int function1(int p1, int p2) { s = p1; return p2; }
}

SCENARIO("Callbacks (int, two parameters) can be constructed in various ways", "[callback]")
{
    s = 0;
    WHEN("A callback is constructed from a function")
    {
        MyCallback cb{function1};
        THEN("It can be called and it executes")
        {
            CHECK(cb(42, 1) == 1);
            CHECK(s == 42);
        }
    }
    WHEN("A callback is constructed from a lambda without capture")
    {
        MyCallback cb{[](int p1, int p2){ s = p1; return p2; }};
        THEN("It can be called and it executes")
        {
            CHECK(cb(43, 2) == 2);
            CHECK(s == 43);
        }
    }
    WHEN("A callback is constructed from a lambda with capture by value")
    {
        int a = 6;
        MyCallback cb{[a](int p1, int p2){ s = a + p1; return a + p2;}};
        THEN("It can be called and it executes")
        {
            CHECK(cb(44, 3) == 3 + 6);
            CHECK(s == 44 + 6);
        }
    }
    WHEN("A callback is constructed from a lambda with capture by reference")
    {
        int a = 6;
        MyCallback cb{[&a](int p1, int p2){ s = p1; a = 2; return p2; }};
        THEN("It can be called and it executes")
        {
            CHECK(cb(45, 4) == 4);
            CHECK(s == 45);
            CHECK(a == 2);
        }
    }
    WHEN("A callback is constructed from a lambda with this capture")
    {
        Class1 c1;
        THEN("It can be called and it executes")
        {
            CHECK(c1.this_method(46, 5) == 5);
            CHECK(c1.a == 46);
        }
    }
    WHEN("A callback is constructed from a static method")
    {
        MyCallback cb{Class1::static_method};
        THEN("It can be called and it executes")
        {
            CHECK(cb(49, 6) == 6);
            CHECK(Class1::b == 49);
        }
    }
    WHEN("A callback is constructed from a instance method")
    {
        Class1 c1;
        MyCallback cb{c1, &Class1::member_method};
        THEN("It can be called and it executes")
        {
            CHECK(cb(50, 7) == 7);
            CHECK(c1.a == 50);
        }
    }
}

SCENARIO("Callbacks (int, two parameters) can be assigned in various ways", "[callback]")
{
    s = 0;
    GIVEN("An empty callback")
    {
        MyCallback cb{};
        THEN("It can be called, it executes and does nothing")
        {
            CHECK(cb(51, 8) == 0);
            CHECK(s == 0);
        }
        WHEN("It is assigned to function")
        {
            cb = MyCallback{function1};
            THEN("It can be called and it executes")
            {
                CHECK(cb(52, 9) == 9);
                CHECK(s == 52);
            }
        }
        WHEN("It is assigned a lambda without capture")
        {
            cb = MyCallback{[](int p1, int p2){ s = p1; return p2; }};
            THEN("It can be called and it executes")
            {
                CHECK(cb(53, 10) == 10);
                CHECK(s == 53);
            }
        }
        WHEN("It is assigned a lambda with capture by value")
        {
            int a = 42;
            cb = MyCallback{[a](int p1, int p2){ s = a + p1; return a + p2; }};
            THEN("It can be called and it executes")
            {
                CHECK(cb(54, 11) == 42 + 11);
                CHECK(s == 42 + 54);
            }
        }
        WHEN("It is assigned a lambda with capture by reference")
        {
            int a = 42;
            cb = MyCallback{[&a](int p1, int p2){ s = p1; a = 105; return p2; }};
            THEN("It can be called and it executes")
            {
                CHECK(cb(55, 12) == 12);
                CHECK(s == 55);
                CHECK(a == 105);
            }
        }
        WHEN("It is assigned a static method")
        {
            cb = MyCallback{Class1::static_method};
            THEN("It can be called and it executes")
            {
                CHECK(cb(56, 13) == 13);
                CHECK(Class1::b == 56);
            }
        }
        WHEN("It is assigned an instance method")
        {
            Class1 c1;
            cb = MyCallback{c1, &Class1::member_method};
            THEN("It can be called and it executes")
            {
                CHECK(cb(57, 14) == 14);
                CHECK(c1.a == 57);
            }
        }
        WHEN("It is assign a null pointer")
        {
            cb = nullptr;
            THEN("It can be called and it executes")
            {
                CHECK(cb(58, 15) == 0);
            }
            THEN("It is false")
            {
                CHECK_FALSE(cb);
            }
            THEN("Is does nothing")
            {
                CHECK(s == 0);
            }
        }
        WHEN("It is assigned to another null callback")
        {
            auto cb2 = MyCallback{};
            cb = cb2;
            THEN("The original callback is false")
            {
                CHECK_FALSE(cb2);
            }
            THEN("The new callback is false")
            {
                CHECK_FALSE(cb);
            }
        }
        WHEN("It is assigned to another non-null callback")
        {
            auto cb2 = MyCallback{Class1::static_method};
            cb = cb2;
            THEN("The original callback is true")
            {
                CHECK(cb2);
            }
            THEN("The new callback is true")
            {
                CHECK(cb);
            }
        }
    }
}
