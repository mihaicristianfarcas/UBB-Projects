cmake_minimum_required(VERSION 3.26)
project(a2_3_Forquosh C)

set(CMAKE_C_STANDARD 11)

add_executable(a2_3_Forquosh
        domain/domain.h
        domain/domain.c
        start.c
        ui/ui.c
        services/services.c
        repository/repository.c
        ui/ui.c
        services/services.h
        repository/repository.h
        tests/tests_domain.c
        tests/tests_repository.c
        tests/tests_service.c
        tests/tests.h)
