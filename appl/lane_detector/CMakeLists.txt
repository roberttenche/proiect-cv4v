add_library(lane_detector STATIC
  lane_detector.cpp
  lane_detector.hpp
)

target_include_directories(lane_detector PRIVATE
  ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(lane_detector PRIVATE
  ${OpenCV_LIBS}
)