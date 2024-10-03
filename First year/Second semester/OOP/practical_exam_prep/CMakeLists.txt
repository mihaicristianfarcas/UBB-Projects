cmake_minimum_required(VERSION 3.26)
project(seminar7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.7.0\\mingw_64\\lib\\cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6Widgets REQUIRED)

add_executable(seminar7 main.cpp
        chat_window.cpp
        chat_window.h
        chat.ui
        observer.h
        subject.cpp
        subject.h
        chat_session.cpp
        chat_session.h
        user_message.cpp
        user_message.h
        message.cpp
        message.h
        user.cpp
        user.h
)

target_link_libraries(seminar7 Qt6::Widgets)

