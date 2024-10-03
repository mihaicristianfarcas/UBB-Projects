cmake_minimum_required(VERSION 3.26)
project(oop_t3_Forquosh)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.7.0\\mingw_64\\lib\\cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6Widgets REQUIRED)

add_executable(oop_t3_Forquosh
        main.cpp
        gui.cpp
        gui.h
        gui.ui
        service.cpp
        service.h
        repository.cpp
        repository.h
        domain.cpp
        domain.h)

target_link_libraries(oop_t3_Forquosh Qt6::Widgets)