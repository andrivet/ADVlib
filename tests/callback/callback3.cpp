
#include "ADVcallback.h"
#include "catch.hpp"

using namespace andrivet;

using BackgroundTask = Callback<void(*)()>;

namespace
{
    struct Task
    {
        void set_background_task(const BackgroundTask& task, unsigned int delta = 500) { background_task_ = task; }
        void clear_background_task() { background_task_ = nullptr; }
        void execute_background_task() { background_task_(); }
        bool has_background_task() const { return bool(background_task_); }

    private:
        BackgroundTask background_task_;
    };


    Task task0;

    struct Class1
    {
        void test1() { n_ = 1; }
        void test2() { n_ = 2; task0.clear_background_task(); }

        int n_ = 0;
    };

    struct Class2
    {
        void prepare(const BackgroundTask& task)
        {
            task0.set_background_task(task);
        }

        void step(const BackgroundTask& task)
        {
            task0.set_background_task(task);
        }

        void step1()
        {
            n_ = 1;
            step(BackgroundTask(this, &Class2::step2));
        }

        void step2()
        {
            n_ = 2;
        }

        void start()
        {
            prepare(BackgroundTask(this, &Class2::step1));
        }

        int n_ = 0;
    };

}

SCENARIO("Callbacks can be copied", "[callback]")
{
    GIVEN("A task constructed and a callback set")
    {
        Task task;
        Class1 o;
        task.set_background_task(BackgroundTask{&o, &Class1::test1});

        THEN("It is not empty") REQUIRE(task.has_background_task());
        THEN("It has still the initial value") REQUIRE(o.n_ == 0);

        WHEN("It is executed")
        {
            task.execute_background_task();
            THEN("It is not empty after execution") REQUIRE(task.has_background_task());
            THEN("Its value has been updated") REQUIRE(o.n_ == 1);
        }
    }
    GIVEN("A global task is constructed and a callback set")
    {
        Class1 o;
        task0.set_background_task(BackgroundTask{&o, &Class1::test2});

        THEN("It is not empty") REQUIRE(task0.has_background_task());
        THEN("It has still the initial value") REQUIRE(o.n_ == 0);

        WHEN("It executes")
        {
            THEN("It is not empty after execution") REQUIRE(task0.has_background_task());
            task0.execute_background_task();
            THEN("It is empty after execution") REQUIRE_FALSE(task0.has_background_task());
        }
    }
    GIVEN("A sequence of tasks is started")
    {
        Class2 o3;
        o3.start();

        THEN("Is has the initial value") REQUIRE(o3.n_ == 0);

        WHEN("It executes")
        {
            task0.execute_background_task();
            THEN("It has the correct value") REQUIRE(o3.n_ == 1);
            WHEN("It executes the next step")
            {
                task0.execute_background_task();
                THEN("It has the correct value") REQUIRE(o3.n_ == 2);
            }
        }
    }
}
