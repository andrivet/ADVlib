
#include "ADVcrtp.h"
#include "catch.hpp"

using namespace adv;

namespace
{
    // a member function without parameter

    template<typename Self>
    struct A: Crtp<Self, A>
    {
        int something() { return this->self().do_something(); }

    protected:
        int do_something() { return 0xA000; }
    };

    struct A1: A<A1>
    {
    private:
        int do_something() { return 0xA100; }
        friend A; // Since do_something is private
    };

    struct A2: A<A2>
    {
    };

    // a member function with a parameter

    template<typename Self>
    struct B: Crtp<Self, B>
    {
        int something(int p) { return this->self().do_something(p); }

    protected:
        int do_something(int p) { return 0xB000 + p; }
    };

    struct B1: B<B1>
    {
        int do_something(int p) { return 0xB100 + p; }
    };

    struct B2: B<B2>
    {
    };

    // a member function without returning a value

    template<typename Self>
    struct C: Crtp<Self, C>
    {
        void something() { this->self().do_something(); }
        int get_value() const { return value_; }

    protected:
        void do_something() { value_ = 0xC000; }

        int value_ = 0;
    };

    struct C1: C<C1>
    {
        void do_something() { value_ = 0xC100; }
    };

    struct C2: C<C2>
    {
    };

    // a member function calling its parent

    template<typename Self>
    struct D: Crtp<Self, D>
    {
        void something() { this->self().do_something(); }
        int get_value() const { return value_; }

    protected:
        void do_something() { value_ = 0xD000; }

        int value_ = 0;
    };

    struct D1: D<D1>
    {
        void do_something() { Parent::do_something(); value_ += 2; }
    };

    struct D2: D<D2>
    {
    };

    // a const member function without parameter

    template<typename Self>
    struct E: Crtp<Self, E>
    {
        int something() const { return this->self().do_something(); }

    protected:
        int do_something() const { return 0xE000; }
    };

    struct E1: E<E1>
    {
    private:;
        friend E;
        int do_something() const { return 0xE100; }
    };

    struct E2: E<E2>
    {
    };

    // Call a member function in a deep hierarchy

    template<typename Self>
    struct F: Crtp<Self, F>
    {
        int something() { return this->self().do_something(); }

    protected:
        int do_something() { return 0xF000; }
    };

    template<typename Derived>
    struct F1: F<Derived>
    {
    private:
        friend F<Derived>;
        int do_something() { return 0xF100; }
    };

    struct FF1: F1<FF1>
    {
    private:
        friend F;
        int do_something() { return 0xFF10; }
    };

    // Call a member function in the middle of a deep hierarchy

    template<typename Self>
    struct G: Crtp<Self, G>
    {
        int something() { return this->self().do_something(); }
    };

    template<typename Derived>
    struct G1: G<Derived>
    {
    private:
        friend G<Derived>;
        int do_something() { return 0x1100; }
    };

    struct GG1: G1<GG1>
    {
    };
}

SCENARIO("Call a member function without parameter", "[poly]")
{
    GIVEN("A Child defining the member function can be constructed")
    {
        A1 child1{};
        WHEN("The member function is called")
        {
            auto r = child1.something();
            THEN("The member function of the Child was called")
            {
                REQUIRE(r == 0xA100);
            }
        }
    }

    GIVEN("A Child not defining the member function can be constructed")
    {
        A2 child2{};
        WHEN("The member function is called")
        {
            auto r = child2.something();
            THEN("The member function of the Parent was called")
            {
                REQUIRE(r == 0xA000);
            }
        }
    }
}

SCENARIO("Call a member function with a parameter", "[poly]")
{
    GIVEN("A Child defining the member function can be constructed")
    {
        B1 child1{};
        WHEN("The member function is called")
        {
            auto r = child1.something(1);
            THEN("The member function of the Child was called")
            {
                REQUIRE(r == 0xB101);
            }
        }
    }

    GIVEN("A Child not defining the member function can be constructed")
    {
        B2 child2{};
        WHEN("The member function is called")
        {
            auto r = child2.something(1);
            THEN("The member function of the Parent was called")
            {
                REQUIRE(r == 0xB001);
            }
        }
    }
}

SCENARIO("Call a member function without returning a value", "[poly]")
{
    GIVEN("A Child defining the member function can be constructed")
    {
        C1 child1{};
        WHEN("The member function is called")
        {
            child1.something();
            THEN("The member function of the Child was called")
            {
                REQUIRE(child1.get_value() == 0xC100);
            }
        }
    }

    GIVEN("A Child not defining the member function can be constructed")
    {
        C2 child2{};
        WHEN("The member function is called")
        {
            child2.something();
            THEN("The member function of the Parent was called")
            {
                REQUIRE(child2.get_value() == 0xC000);
            }
        }
    }
}

SCENARIO("Call a member function calling its parent", "[poly]")
{
    GIVEN("A Child defining the member function can be constructed")
    {
        D1 child1{};
        WHEN("The member function is called")
        {
            child1.something();
            THEN("The member function of the Child was called and also the member function of the parent")
            {
                REQUIRE(child1.get_value() == 0xD002);
            }
        }
    }

    GIVEN("A Child not defining the member function can be constructed")
    {
        D2 child2{};
        WHEN("The member function is called")
        {
            child2.something();
            THEN("The member function of the Parent was called")
            {
                REQUIRE(child2.get_value() == 0xD000);
            }
        }
    }
}

SCENARIO("Call a const member function without parameter", "[poly]")
{
    GIVEN("A Child defining the member function can be constructed")
    {
        E1 child1{};
        WHEN("The member function is called")
        {
            auto r = child1.something();
            THEN("The member function of the Child was called")
            {
                REQUIRE(r == 0xE100);
            }
        }
    }

    GIVEN("A Child not defining the member function can be constructed")
    {
        E2 child2{};
        WHEN("The member function is called")
        {
            auto r = child2.something();
            THEN("The member function of the Parent was called")
            {
                REQUIRE(r == 0xE000);
            }
        }
    }
}

SCENARIO("Call a member function in a deep hierarchy", "[poly]")
{
    GIVEN("A Child of a Child defining the member function can be constructed")
    {
        FF1 child1{};
        WHEN("The member function is called")
        {
            auto r = child1.something();
            THEN("The member function of the Child was called")
            {
                REQUIRE(r == 0xFF10);
            }
        }
    }
}

SCENARIO("Call a member function in the middle of a deep hierarchy", "[poly]")
{
    GIVEN("A Child of a Child can be constructed")
    {
        GG1 child1{};
        WHEN("The member function is called")
        {
            auto r = child1.something();
            THEN("The member function of the Child was called")
            {
                REQUIRE(r == 0x1100);
            }
        }
    }
}
