cmake_minimum_required(VERSION 3.10)
project(DSM_Lab8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find MPI
find_package(MPI REQUIRED)

# Add executable
add_executable(dsm_main
    main.cpp
    dsm.cpp
)

# Include MPI headers
target_include_directories(dsm_main PRIVATE ${MPI_CXX_INCLUDE_PATH})

# Link MPI libraries
target_link_libraries(dsm_main PRIVATE MPI::MPI_CXX)

# Enable threading
find_package(Threads REQUIRED)
target_link_libraries(dsm_main PRIVATE Threads::Threads)

# Compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(dsm_main PRIVATE -Wall -Wextra -pedantic)
endif()

# Print MPI info
message(STATUS "MPI found: ${MPI_CXX_FOUND}")
message(STATUS "MPI include path: ${MPI_CXX_INCLUDE_PATH}")
message(STATUS "MPI libraries: ${MPI_CXX_LIBRARIES}")
