
#include "ADVunique_ptr.h"
#include "catch.hpp"

using namespace adv;

namespace
{
    struct A { int field = 42; };
    unique_ptr<A> factory() { return unique_ptr<A>(new A()); }
}

SCENARIO("A unique_ptr can be created", "[unique_ptr]")
{
    GIVEN("A unique_ptr created with new")
    {
        unique_ptr<A> ptr1{new A()};
        THEN("The unique_ptr is not null")
        {
            REQUIRE(ptr1 != nullptr);
        }
        THEN("The unique_ptr is true")
        {
            REQUIRE(ptr1);
        }
        THEN("The field of the unique_ptr can be accessed and has the expected value")
        {
            CHECK(ptr1->field == 42);
        }
    }

    GIVEN("A unique_ptr created with a factory")
    {
        unique_ptr<A> ptr1 = factory();
        THEN("The unique_ptr is not null")
        {
            REQUIRE(ptr1 != nullptr);
        }
        THEN("The unique_ptr is true")
        {
            REQUIRE(ptr1);
        }
        THEN("The field of the unique_ptr can be accessed and has the expected value")
        {
            CHECK(ptr1->field == 42);
        }
    }
}

SCENARIO("A unique_ptr can be moved", "[unique_ptr]")
{
    GIVEN("A unique_ptr created from a factory")
    {
        unique_ptr<A> ptr1 = factory();
        WHEN("The unique_ptr is moved")
        {
            unique_ptr<A> ptr2 = move(ptr1);
            THEN("The unique_ptr is not null")
            {
                REQUIRE(ptr2 != nullptr);
            }
            THEN("The unique_ptr is true")
            {
                REQUIRE(ptr2);
            }
            THEN("The field of the unique_ptr can be accessed and has the expected value")
            {
                CHECK(ptr2->field == 42);
            }
            THEN("The original unique_ptr is null")
            {
                CHECK(ptr1 == nullptr);
                CHECK(ptr1.get() == nullptr);
            }
            THEN("The original unique_ptr is false")
            {
                CHECK_FALSE(ptr1);
            }
        }
    }
}
