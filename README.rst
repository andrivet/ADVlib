ADVcallback
===========

**ADVcallback** is a simple callback library for C++ 11 when the `C++ Standard Library`_ is not an option or when function_ is not appropriate.

.. _`C++ Standard Library`: https://en.wikipedia.org/wiki/C%2B%2B_Standard_Library

.. _`Standard Template Library`: https://en.wikipedia.org/wiki/Standard_Template_Library

.. _function:  http://www.cplusplus.com/reference/functional/function/?kw=function

.. _functional: http://www.cplusplus.com/reference/functional/

Introduction
============

Most of the time, implementing callbacks is not a problem with C++ thanks to function_ and functional_ header. However, there are circumstances when function_ is not an option. For example:

* When the Standard Library is not available. It is in particular the case with AVR (and with IoT in general).

* When function_ is too complex to implement callbacks

When the Standard Library is unavailable
========================================

When using a C++ 11 (or later) compiler, the Standard Library is most of the time availaible, but not always. This is in particular a problem when developping for Arduino (and AVR in general).
There are several reasons for that. One of them is that the Standard Library is heavy and it is not optimized for embedded systems with, for example, only 2048 bytes of RAM (as it is the case of the `Arduino Uno`_).

There some partial implementations of the STL (`Standard Template Library`_), a subset of the Standard Library. These implementations do implement algoritms and entities such as vector (most of the time, in a pre-C++11 way). But they lack C++ 11 entities such as unique_ptr_.

When developping `ADVi3++`_, I face these limitations many times and thus decided to create a minimal implementation of what I need.

.. _`Arduino Uno`: https://store.arduino.cc/usa/arduino-uno-rev3

.. _unique_ptr: http://www.cplusplus.com/reference/memory/unique_ptr/?kw=unique_ptr

.. _`ADVi3++`: https://github.com/andrivet/ADVi3pp-Marlin

Example of STL for embedded systems:

* https://cxx.uclibc.org/index.html

* https://github.com/mike-matera/ArduinoSTL

When std::function is too complex
=================================

The template ``std::function`` is a general-purpose polymorphic function wrapper. It can store plain functions, functors, lambda expressions, static members. It can also store pointers to members and pointers to data members. For example:

::

    std::function<void()> display42 = []{ std::cout << 42 << cout::endl; };
    display42();

However in the case of members, it is bind to the instance (the ``this`` pointer). As a consequence, the signature of the function is changed. Let's assume we have defined the following class:

::

    struct Num
    {
        explicit Num(int n): n_{n} {}
        int add(int i) { return n_ + i; }
        int n_ = 0; 
    };

The signature of `add`` is the following:

::

    int(Num::*)(int)

We declare a function initialized with the member function ``add`` this way:

::

    std::function<int(const Num&, int)> f{&Num::add};

And we call it this way:

::

    Num n{0};
    f(n, 1);

In other terms, the signature of the ``function`` is ``std::function<int(const Num&, int)>`` and we declare and use separately the instance.

Sometimes, this is not what we want. We want to declare at the same time both the instance and the member function, store them and calls them later. The function has to be polymorphic and store either a plain function, a functor, a static member or an instance member. For all these cases, the signature has to be the same. In this case:

::

    std::function<int(int)>

This is possible to achieve with ``std::function`` by using ``bind`` and placeholders:

::

    Num n{0};
    std::function<int(int)> f2{std::bind(&Num::add, &n2, _1)};

    f(1);

And this is exactly what I needed for callbacks inside `ADVi3++`_. However, as you may have guessed, std::function is very heavy and complex. I wanted something similar but less complex. And this is why I implemented **ADVcallback**.

ADVcallback
===========

To make it clear that my callbacks are different from std::function, I have named them ``Callback``. All the entities are declared inside a namespace name ``andrivet`` (and not ``std``).

Callbacks are used this way:

::

    using MyCallback = Callback<int(int)>;

    Num n{0};
    MyCallback callback{n, &Num::add};
    callback(1);

    callback = MyCallback{[](int i){ return i; }};
    callback(2);

    int b = 8;
    callback = MyCallback([b](int i){ return b + i; }};
    callback(3);

Unit Tests
==========

The project constains unit tests for both ``unique_ptr`` and ``Callback``. They are located inside the ``tests`` directory and give various example of how to use ``unique_ptr`` and ``Callback``.

Copyright
=========

Copyright © 2018 Sebastien Andrivet

License
=======

.. image:: https://www.gnu.org/graphics/gplv3-127x51.png

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.

