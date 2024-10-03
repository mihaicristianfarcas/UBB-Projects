cmake_minimum_required(VERSION 3.26)
project(oop_a6_7_Forquosh)

set(CMAKE_CXX_STANDARD 14)

include_directories(Domain)
include_directories(Repository)
include_directories(Services)
include_directories(Tests)
include_directories(UI)

add_executable(oop_a6_7_Forquosh
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
)
