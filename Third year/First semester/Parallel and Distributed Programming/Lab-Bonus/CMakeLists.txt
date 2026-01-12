cmake_minimum_required(VERSION 3.20)
project(PolynomialMultiplicationMetal LANGUAGES CXX OBJCXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OBJCXX_STANDARD 20)
set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)

# Find required frameworks
find_library(METAL_FRAMEWORK Metal REQUIRED)
find_library(FOUNDATION_FRAMEWORK Foundation REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/polynomial.cpp
    src/cpu_multiplier.cpp
    src/metal_multiplier.mm
)

# Create executable
add_executable(polymul_metal ${SOURCES})

# Include directories
target_include_directories(polymul_metal PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

# Link frameworks
target_link_libraries(polymul_metal PRIVATE
    ${METAL_FRAMEWORK}
    ${FOUNDATION_FRAMEWORK}
)

# Set working directory for shader loading
set_target_properties(polymul_metal PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Copy shaders directory for runtime compilation
add_custom_command(TARGET polymul_metal POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_BINARY_DIR}/shaders
    COMMENT "Copying shader sources for runtime compilation"
)

# Compiler warnings
target_compile_options(polymul_metal PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O2
)

# Enable ARC for Objective-C++
set_source_files_properties(src/metal_multiplier.mm PROPERTIES
    COMPILE_FLAGS "-fobjc-arc"
)

# Print build info
message(STATUS "Metal Framework: ${METAL_FRAMEWORK}")
message(STATUS "Foundation Framework: ${FOUNDATION_FRAMEWORK}")
message(STATUS "Metal shaders will be compiled at runtime from: ${CMAKE_SOURCE_DIR}/shaders/")
