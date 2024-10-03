cmake_minimum_required(VERSION 3.26)
project(FixedCapBiMap)

set(CMAKE_CXX_STANDARD 17)

add_executable(FixedCapBiMap
        ExtendedTest.cpp
        FixedCapBiMap.cpp
        FixedCapBiMapIterator.cpp
        ShortTest.cpp
        App.cpp
        FixedCapBiMapIterator.h)
