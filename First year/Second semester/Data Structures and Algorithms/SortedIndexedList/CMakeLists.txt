cmake_minimum_required(VERSION 3.26)
project(SortedIndexedList)

set(CMAKE_CXX_STANDARD 17)

add_executable(SortedIndexedList
        SortedIndexedList.cpp
        SortedIndexedList.h
        ShortTest.h
        ShortTest.cpp
        ListIterator.h
        ListIterator.cpp
        ExtendedTest.h
        ExtendedTest.cpp
        App.cpp)
