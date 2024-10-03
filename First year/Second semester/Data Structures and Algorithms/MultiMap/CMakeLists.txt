cmake_minimum_required(VERSION 3.26)
project(MultiMap)

set(CMAKE_CXX_STANDARD 17)

add_executable(MultiMap
        App.cpp
        ExtendedTest.cpp
        ExtendedTest.h
        MultiMap.cpp
        MultiMap.h
        MultiMapIterator.cpp
        MultiMapIterator.h
        ShortTest.cpp
        ShortTest.h)
