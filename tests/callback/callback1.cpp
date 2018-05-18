
#include "ADVcallback.h"
#include "catch.hpp"

using namespace andrivet;

using MyCallback = Callback<void(*)()>;

namespace
{
    struct Class1
    {
        static void static_method()
        {
            b = 42;
        }

        void member_method()
        {
            a = 42;
        }

        void this_method()
        {
            MyCallback cb{[this]{ this->a = 42; }};
            cb();
        }

        int a = 0;
        static int b;
    };

    int Class1::b = 0;

    int s = 0;
    void function1() { s = 42; }
}

SCENARIO("Callbacks (void, no parameters) can be constructed in various ways", "[callback]")
{
    s = 0;
    WHEN("A callback is constructed from a function")
    {
        MyCallback cb{function1};
        THEN("It can be called and it executes")
        {
            cb();
            CHECK(s == 42);
        }
    }
    WHEN("A callback is constructed from a lambda without capture")
    {
        MyCallback cb{[]{ s = 42; }};
        THEN("It can be called and it executes")
        {
            cb();
            CHECK(s == 42);
        }
    }
    WHEN("A callback is constructed from a lambda with capture by value")
    {
        int a = 6;
        MyCallback cb{[a]{ s = a; }};
        THEN("It can be called and it executes")
        {
            cb();
            CHECK(s == 6);
        }
    }
    WHEN("A callback is constructed from a lambda with capture by reference")
    {
        int a = 6;
        MyCallback cb{[&a]{ s = a; a = 2; }};
        THEN("It can be called and it executes")
        {
            cb();
            CHECK(s == 6);
            CHECK(a == 2);
        }
    }
    WHEN("A callback is constructed from a lambda with this capture")
    {
        Class1 c1;
        THEN("It can be called and it executes")
        {
            c1.this_method();
            CHECK(c1.a == 42);
        }
    }
    WHEN("A callback is constructed from a static method")
    {
        MyCallback cb{Class1::static_method};
        THEN("It can be called and it executes")
        {
            cb();
            CHECK(Class1::b == 42);
        }
    }
    WHEN("A callback is constructed from a instance method")
    {
        Class1 c1;
        MyCallback cb{c1, &Class1::member_method};
        THEN("It can be called and it executes")
        {
            cb();
            CHECK(c1.a == 42);
        }
    }
}

SCENARIO("Callbacks (void, no parameters) can be assigned in various ways", "[callback]")
{
    s = 0;
    GIVEN("An empty callback")
    {
        MyCallback cb{};
        THEN("It can be called, it executes and does nothing")
        {
            cb();
        }
        WHEN("It is assigned to function")
        {
            cb = MyCallback{function1};
            THEN("It can be called and it executes")
            {
                cb();
                CHECK(s == 42);
            }
        }
        WHEN("It is assigned a lambda without capture")
        {
            cb = MyCallback{[]{ s = 42; }};
            THEN("It can be called and it executes")
            {
                cb();
                CHECK(s == 42);
            }
        }
        WHEN("It is assigned a lambda with capture by value")
        {
            int a = 42;
            cb = MyCallback{[a]{ s = a; }};
            THEN("It can be called and it executes")
            {
                cb();
                CHECK(s == 42);
            }
        }
        WHEN("It is assigned a lambda with capture by reference")
        {
            int a = 42;
            cb = MyCallback{[&a]{ s = a; a = 43; }};
            THEN("It can be called and it executes")
            {
                cb();
                CHECK(s == 42);
                CHECK(a == 43);
            }
        }
        WHEN("It is assigned a static method")
        {
            cb = MyCallback{Class1::static_method};
            THEN("It can be called and it executes")
            {
                cb();
                CHECK(Class1::b == 42);
            }
        }
        WHEN("It is assigned an instance method")
        {
            Class1 c1;
            cb = MyCallback{c1, &Class1::member_method};
            THEN("It can be called and it executes")
            {
                cb();
                CHECK(c1.a == 42);
            }
        }
        WHEN("It is assign a null pointer")
        {
            cb = nullptr;
            THEN("It can be called and it executes")
            {
                cb();
            }
            THEN("It is false")
            {
                CHECK_FALSE(cb);
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
        }
        WHEN("It is assigned to another non-null callback")
        {
            auto cb2 = MyCallback{Class1::static_method};
            cb = cb2;
            THEN("The original callback is true")
            {
                CHECK(cb2);
            }
        }
    }
}
