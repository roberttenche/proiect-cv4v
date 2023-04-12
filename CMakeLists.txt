cmake_minimum_required(VERSION 3.24.2)

project(CV4V-PROJECT LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

find_package( OpenCV REQUIRED )

add_subdirectory(
  appl
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${OpenCV_INCLUDE_DIRS}
  appl/lane_detector
)

target_link_libraries(${PROJECT_NAME}
  ${OpenCV_LIBS}
  lane_detector
)
