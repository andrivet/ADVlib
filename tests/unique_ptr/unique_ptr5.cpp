
#include "ADVunique_ptr.h"
#include "catch.hpp"

using namespace andrivet;

namespace
{
    struct A
    {
        A() : b(false) { }
        A(int, double&, char&&, void*) : b(true) { }
        bool b;
    };
}

SCENARIO("Unique_ptrs can be compared", "[unique_ptr]")
{
    GIVEN("Two null unique_ptr")
    {
        unique_ptr<A> p1;
        unique_ptr<A> p2;
        THEN("They are equal")
        {
            CHECK(p1 == p2);
        }
        THEN("They are not different")
        {
            CHECK_FALSE(p1 != p2);
        }
        THEN("The first one is not less than the second one")
        {
            CHECK_FALSE(p1 < p2);
        }
        THEN("The first one is not greater than the second one")
        {
            CHECK_FALSE(p1 > p2);
        }
        THEN("The first one is less or equal to the second one")
        {
            CHECK(p1 <= p2);
        }
        THEN("The first one is greater or equal to the second one")
        {
            CHECK(p1 >= p2);
        }
    }

    GIVEN("A null unique_ptr and a non-null unique_ptr")
    {
        unique_ptr<A> p1;
        unique_ptr<A> p2(new A);
        THEN("They are not equal")
        {
            CHECK_FALSE(p1 == p2);
        }
        THEN("They are different")
        {
            CHECK(p1 != p2);
        }
        THEN("The first one is less or greater than the second one")
        {
            CHECK(((p1 < p2) || (p1 > p2)));
        }
    }

    GIVEN("Two null unique_ptr p1 and p3, and a non-null unique_ptr p2")
    {
        unique_ptr<A> p1;
        unique_ptr<A> p2(new A);
        unique_ptr<A> p3;
        WHEN("The second null unique_ptr p3 is swapped with the non-null unique_ptr p2")
        {
            swap(p3, p2);
            THEN("p1 and p3 are different")
            {
                CHECK(p1 != p3);
            }
            THEN("p2 and p3 are different")
            {
                CHECK(p2 != p3);
            }
            THEN("p1 and p1 are equal")
            {
                CHECK(p1 == p2);
            }
        }
    }
}
