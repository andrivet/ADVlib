
#include "ADVunique_ptr.h"
#include "catch.hpp"

using namespace adv;

namespace
{
    struct A
    {
        A() : b(false) { }
        A(int, double&, char&&, void*) : b(true) { }
        bool b;
    };
}

SCENARIO("A unique_ptr can be created with make_unique", "[unique_ptr]")
{
    GIVEN("A unique_ptr created with make_unique and parameters")
    {
        int i = 42;
        double d = 43.0;
        char c = 'A';
        unique_ptr<A> a = make_unique<A>(i, d, move(c), nullptr);
        THEN("The unique_ptr is not null")
        {
            REQUIRE(a != nullptr);
            REQUIRE(a.get() != nullptr);
        }
        THEN("The member of the unique_ptr are accessible and have the right value")
        {
            REQUIRE(a->b);
        }
    }
    GIVEN("A unique_ptr created with make_unique and no parameters")
    {
        unique_ptr<A> a = make_unique<A>();
        THEN("The unique_ptr is not null")
        {
            REQUIRE(a != nullptr);
            REQUIRE(a.get() != nullptr);
        }
        THEN("The member of the unique_ptr are accessible and have the right value")
        {
            REQUIRE_FALSE(a->b);
        }
    }
}
