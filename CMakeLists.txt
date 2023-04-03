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
)

# target_link_directories(${PROJECT_NAME} PRIVATE
#   lib-src/opencv/opencv-4.7.0-prebuilt
# )

# User built / pre-compiled libraries
target_link_libraries(${PROJECT_NAME}
  ${OpenCV_LIBS}
  # opencv_calib3d470d
  # opencv_core470d
  # opencv_dnn470d
  # opencv_features2d470d
  # opencv_flann470d
  # opencv_gapi470d
  # opencv_highgui470d
  # opencv_imgcodecs470d
  # opencv_imgproc470d
  # opencv_ml470d
  # opencv_objdetect470d
  # opencv_photo470d
  # opencv_stitching470d
  # opencv_video470d
  # opencv_videoio470d
)

# System libraries
# target_link_libraries(${PROJECT_NAME}
# )
