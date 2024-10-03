cmake_minimum_required(VERSION 3.26)
project(oop_a4_5_Forquosh)

set(CMAKE_CXX_STANDARD 14)

add_executable(oop_a4_5_Forquosh
        UI/ui_admin.cpp
        UI/ui_user.cpp
        start.cpp
        Services/services_admin.cpp
        Services/services_user.cpp
        Domain/Dog.h
        Domain/Dog.cpp
        Repository/repository.h
        Repository/repository.cpp
        Services/services.h
        Tests/tests.h
        Tests/tests_repository.cpp
        Tests/tests_domain.cpp
        Tests/tests_services_admin.cpp
        UI/ui.h
        Tests/tests_services_user.cpp)
