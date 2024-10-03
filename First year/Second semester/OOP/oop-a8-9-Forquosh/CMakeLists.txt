cmake_minimum_required(VERSION 3.26)
project(oop_a8_9_Forquosh)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.7.0\\mingw_64\\lib\\cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6Widgets REQUIRED)

include_directories(Domain)
include_directories(Repository)
include_directories(Services)
include_directories(Tests)
include_directories(UI)

add_executable(oop_a8_9_Forquosh
        Domain/Dog.cpp
        Domain/Dog.h
        Repository/repository.cpp
        Repository/repository.h
        Services/services.h
        Services/services_admin.cpp
        Services/services_user.cpp
        Tests/tests.h
        Tests/tests_domain.cpp
        Tests/tests_repository.cpp
        Tests/tests_services_admin.cpp
        Tests/tests_services_user.cpp
        UI/ui.h
        UI/ui_admin.cpp
        UI/ui_user.cpp
        start.cpp
        gui.h
        gui.cpp)

target_link_libraries(oop_a8_9_Forquosh Qt6::Widgets)
