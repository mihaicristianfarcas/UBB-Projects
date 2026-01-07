cmake_minimum_required(VERSION 3.10)
project(HoughTransform)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Find MPI
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Common source files
set(COMMON_SOURCES
    src/common/image_processor.cpp
    src/common/hough_common.cpp
    src/common/hough_line.cpp
)

# Threaded version
add_executable(hough_threaded
    src/threaded/main_threaded.cpp
    src/threaded/parallel_processor.cpp
    src/threaded/hough_line_threaded.cpp
    ${COMMON_SOURCES}
)
target_link_libraries(hough_threaded ${OpenCV_LIBS} pthread)
target_compile_options(hough_threaded PRIVATE -pthread)

# MPI version
add_executable(hough_mpi
    src/mpi/main_mpi.cpp
    src/mpi/parallel_processor.cpp
    src/mpi/hough_line_mpi.cpp
    ${COMMON_SOURCES}
)
target_link_libraries(hough_mpi ${OpenCV_LIBS} MPI::MPI_CXX)
target_compile_options(hough_mpi PRIVATE ${MPI_COMPILE_FLAGS})

# Create output directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/output)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/test_images)

