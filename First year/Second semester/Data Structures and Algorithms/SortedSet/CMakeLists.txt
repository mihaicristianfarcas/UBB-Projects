cmake_minimum_required(VERSION 3.26)
project(SortedSet)

set(CMAKE_CXX_STANDARD 17)

add_executable(SortedSet
        App.cpp
        ExtendedTest.cpp
        ShortTest.cpp
        SortedSet.cpp
        SortedSetIterator.cpp)
