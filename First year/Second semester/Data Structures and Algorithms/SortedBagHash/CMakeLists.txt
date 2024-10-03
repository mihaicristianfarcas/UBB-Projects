cmake_minimum_required(VERSION 3.26)
project(SortedBagHash)

set(CMAKE_CXX_STANDARD 17)

add_executable(SortedBagHash
        SortedBagIterator.h
        SortedBagIterator.cpp
        SortedBag.h
        SortedBag.cpp
        ShortTest.h
        ShortTest.cpp
        ExtendedTest.h
        ExtendedTest.cpp
        App.cpp)
