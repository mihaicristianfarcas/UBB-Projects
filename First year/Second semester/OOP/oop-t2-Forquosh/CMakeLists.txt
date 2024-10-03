cmake_minimum_required(VERSION 3.26)
project(oop_t2_Forquosh)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(oop_t2_Forquosh
        main.cpp
        ui.h
        ui.cpp
        service.cpp
        house.cpp
        house.h
        block.cpp
        block.h
        service.h
        building.cpp
        building.h)
