cmake_minimum_required(VERSION 3.24.2)

project(CV4V-PROJECT LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES
  appl/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# OpenCV settings
# https://docs.opencv.org/4.x/db/d05/tutorial_config_reference.html
set(BUILD_PERF_TESTS OFF)
set(BUILD_TESTS OFF)
set(BUILD_DOCS OFF)
set(WITH_CUDA OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_SHARED_LIBS OFF)
set(OPENCV_GENERATE_PKGCONFIG OFF)
set(ENABLE_PIC OFF)
set(BUILD_opencv_apps OFF)
set(INSTALL_CREATE_DISTRIB ON)


# Start building
add_subdirectory(lib-src)

target_include_directories(${PROJECT_NAME} PRIVATE
  lib-src/opencv/include
)

target_link_directories(${PROJECT_NAME} PRIVATE
  build/lib-src/glfw/src
  build/lib-src/glew
)

# User built / pre-compiled libraries
target_link_libraries(${PROJECT_NAME}
  glfw3
  glew32
)

# System libraries
target_link_libraries(${PROJECT_NAME}
  opengl32
)
