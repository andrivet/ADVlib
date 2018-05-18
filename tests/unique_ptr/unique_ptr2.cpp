
#include "ADVunique_ptr.h"
#include "catch.hpp"

// https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/testsuite/20_util/unique_ptr

using namespace andrivet;

namespace
{
    struct A { int i = 42; virtual ~A() = default; };
    struct B : A { int j = 43; };
    struct A_pointer { explicit operator A*() const { return nullptr; } };
}

SCENARIO("A unique_ptr can be converted to volatile or const", "[unique_ptr]")
{
    WHEN("A const unique_ptr is constructed from a unique_ptr")
    {
        unique_ptr<const A> cA(new A);
        THEN("The const unique_ptr is not null")
        {
            REQUIRE(cA != nullptr);
        }
        THEN("The const unique_ptr is true")
        {
            REQUIRE(cA);
        }
    }
    WHEN("A volatile unique_ptr is constructed from a unique_ptr")
    {
        unique_ptr<volatile A> vA(new A);
        THEN("The volatile unique_ptr is not null")
        {
            REQUIRE(vA != nullptr);
        }
        THEN("The volatile unique_ptr is true")
        {
            REQUIRE(vA);
        }
    }
    WHEN("A const volatile unique_ptr is constructed from a unique_ptr")
    {
        unique_ptr<const volatile A> cvA(new A);
        THEN("The const volatile unique_ptr is not null")
        {
            REQUIRE(cvA != nullptr);
        }
        THEN("The const volatile unique_ptr is true")
        {
            REQUIRE(cvA);
        }
    }
}

SCENARIO("A unique_ptr of a derived class can be converted to a volatile or const of the base class", "[unique_ptr]")
{
    WHEN("A const unique_ptr is constructed from a unique_ptr of the base class")
    {
        unique_ptr<const A> cB(new B);
        THEN("The const unique_ptr is not null")
        {
            REQUIRE(cB != nullptr);
        }
        THEN("The const unique_ptr is true")
        {
            REQUIRE(cB);
        }
    }
    WHEN("A volatile unique_ptr is constructed from a unique_ptr of the base class")
    {
        unique_ptr<volatile A> vB(new B);
        THEN("The volatile unique_ptr is not null")
        {
            REQUIRE(vB != nullptr);
        }
        THEN("The volatile unique_ptr is true")
        {
            REQUIRE(vB);
        }
    }
    WHEN("A const volatile unique_ptr is constructed from a unique_ptr of the base class")
    {
        unique_ptr<const volatile A> cvB(new B);
        THEN("The const volatile unique_ptr is not null")
        {
            REQUIRE(cvB != nullptr);
        }
        THEN("The const volatile unique_ptr is true")
        {
            REQUIRE(cvB);
        }
    }
}

SCENARIO("A unique_ptr can be moved to a volatile or const", "[unique_ptr]")
{
    GIVEN("Three unique_ptr created with new")
    {
        unique_ptr<A> upA1(new A);
        unique_ptr<A> upA2(new A);
        unique_ptr<A> upA3(new A);

        WHEN("A unique_ptr is moved into a const unique_ptr")
        {
            unique_ptr<const A> cA(move(upA1));
            THEN("The unique_ptr is no null")
            {
                REQUIRE(cA);
            }
            THEN("The field of the unique_ptr is accessible and has the expected value")
            {
                CHECK(cA->i == 42);
            }
        }
        WHEN("A unique_ptr is moved into a volatile unique_ptr")
        {
            unique_ptr<volatile A> vA(move(upA2));
            THEN("The unique_ptr is no null")
            {
                REQUIRE(vA);
            }
            THEN("The field of the unique_ptr is accessible and has the expected value")
            {
                CHECK(vA->i == 42);
            }
        }
        WHEN("A unique_ptr is moved into a const volatile unique_ptr")
        {
            unique_ptr<const volatile A> cvA(move(upA3));
            THEN("The unique_ptr is no null")
            {
                REQUIRE(cvA);
            }
            THEN("The field of the unique_ptr is accessible and has the expected value")
            {
                CHECK(cvA->i == 42);
            }
        }
    }
}

SCENARIO("A unique_ptr of a derived class can be moved to a volatile or const of the base class", "[unique_ptr]")
{
    GIVEN("Three unique_ptr created with new")
    {
        unique_ptr<B> upB1(new B);
        unique_ptr<B> upB2(new B);
        unique_ptr<B> upB3(new B);

        WHEN("A unique_ptr is moved into a const unique_ptr")
        {
            unique_ptr<const A> cA(move(upB1));
            THEN("The unique_ptr is no null")
            {
                REQUIRE(cA);
            }
            THEN("The field of the unique_ptr is accessible and has the expected value")
            {
                CHECK(cA->i == 42);
            }
        }
        WHEN("A unique_ptr is moved into a volatile unique_ptr")
        {
            unique_ptr<volatile A> vA(move(upB2));
            THEN("The unique_ptr is no null")
            {
                REQUIRE(vA);
            }
            THEN("The field of the unique_ptr is accessible and has the expected value")
            {
                CHECK(vA->i == 42);
            }
        }
        WHEN("A unique_ptr is moved into a const volatile unique_ptr")
        {
            unique_ptr<const volatile A> cvA(move(upB3));
            THEN("The unique_ptr is no null")
            {
                REQUIRE(cvA);
            }
            THEN("The field of the unique_ptr is accessible and has the expected value")
            {
                CHECK(cvA->i == 42);
            }
        }
    }
}
