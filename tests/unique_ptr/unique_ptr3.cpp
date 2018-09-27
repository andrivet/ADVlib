
#include "ADVunique_ptr.h"
#include "catch.hpp"

using namespace adv;

namespace
{
    struct A
    {
        A() { ++ctor_count; }
        virtual ~A() { ++dtor_count; }
        static long ctor_count;
        static long dtor_count;
    };
    long A::ctor_count = 0;
    long A::dtor_count = 0;

    struct B : A
    {
        B() { ++ctor_count; }
        ~B() override { ++dtor_count; }
        static long ctor_count;
        static long dtor_count;
    };
    long B::ctor_count = 0;
    long B::dtor_count = 0;

    struct reset_count_struct
    {
        ~reset_count_struct()
        {
            A::ctor_count = 0;
            A::dtor_count = 0;
            B::ctor_count = 0;
            B::dtor_count = 0;
        }
    };
}

SCENARIO("The number of construction and destruction can be counted", "[unique_ptr]")
{
    GIVEN("A unique_ptr to a base class")
    {
        unique_ptr<A> A_default;
        WHEN("The unique_ptr is initialized")
        {
            THEN("it is null")
            {
                CHECK(A_default == nullptr);
                CHECK(A_default.get() == nullptr);
            }
        }
        WHEN("The counters are reset")
        {
            reset_count_struct reset;
            THEN("The counters of the base class are zero")
            {
                CHECK(A::ctor_count == 0);
                CHECK(A::dtor_count == 0);
            }
            THEN("The counters of the derived class are zero")
            {
                CHECK(B::ctor_count == 0);
                CHECK(B::dtor_count == 0);
            }
            WHEN("An unique_ptr is created from a const null pointer")
            {
                A* const null = nullptr;
                unique_ptr<A> p1(null);
                THEN("The unique_ptr is null")
                {
                    CHECK(p1 == nullptr);
                    CHECK(p1.get() == nullptr);
                }
                THEN("The number of constructions of the base class is 0")
                {
                    CHECK(A::ctor_count == 0);
                }
                THEN("The number of destructions of the base class is 0")
                {
                    CHECK(A::dtor_count == 0);
                }
                THEN("The number of constructions of the derived class is 0")
                {
                    CHECK(B::ctor_count == 0);
                }
                THEN("The number of destructions of the base class is 0")
                {
                    CHECK(B::dtor_count == 0);
                }            }
            WHEN("A unique_ptr of the base class is created from a new base class")
            {
                unique_ptr<A> A_from_A(new A);
                THEN("The unique_ptr is not null")
                {
                    CHECK(A_from_A != nullptr);
                    CHECK(A_from_A.get() != nullptr);
                }
                THEN("The number of constructions of the base class is 1")
                {
                    CHECK(A::ctor_count == 1);
                }
                THEN("The number of destructions of the base class is 0")
                {
                    CHECK(A::dtor_count == 0);
                }
                THEN("The number of constructions of the derived class is 0")
                {
                    CHECK(B::ctor_count == 0);
                }
                THEN("The number of destructions of the base class is 0")
                {
                    CHECK(B::dtor_count == 0);
                }
            }
            WHEN("A unique_ptr of the base class is created with a new derived class")
            {
                unique_ptr<A> A_from_B(new B);
                THEN("The unique_ptr is not null")
                {
                    CHECK(A_from_B != nullptr);
                    CHECK(A_from_B.get() != nullptr);
                }
                THEN("The number of constructions of the base class is 1")
                {
                    CHECK(A::ctor_count == 1);
                }
                THEN("The number of destructions of the base class is 0")
                {
                    CHECK(A::dtor_count == 0);
                }
                THEN("The number of constructions of the derived class is 1")
                {
                    CHECK(B::ctor_count == 1);
                }
                THEN("The number of destructions of the base class is 0")
                {
                    CHECK(B::dtor_count == 0);
                }
            }
        }
    }
}

