cmake_minimum_required(VERSION 3.26)
project(oop_e_913_916_Forquosh)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.7.0\\mingw_64\\lib\\cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6Widgets REQUIRED)

add_executable(oop_e_913_916_Forquosh
        main.cpp
        domain.h
        observer.h
        subject.cpp
        subject.h
        repository.cpp
        repository.h
        session.cpp
        session.h
        domain.cpp
        couriergui.cpp
        couriergui.h
        couriergui.ui
        companygui.cpp
        companygui.h
        companygui.ui)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)
