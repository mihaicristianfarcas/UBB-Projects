cmake_minimum_required(VERSION 3.26)
project(t1_Forquosh_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(t1_Forquosh_1
        domain.cpp
        domain.h
        repository.cpp
        repository.h
        service.cpp
        service.h
        start.cpp
        ui.cpp
        ui.h
        tests.cpp)
