#pragma once

#include <opencv2/opencv.hpp>

namespace lane_detector
{
  /// <summary>
  /// Structure for lane detection options
  /// </summary>
  typedef struct options_s {

    // if true, displays FPS on window near title
    bool display_fps = false;

    // if true, displays the frames on separate window
    bool display_frames = false;

    // how many ms to wait between displaying each frame
    // 0 means wait forever
    //
    // NOTE: it is NOT guaranteed to wait this exact amount, only ATLEAST this amount
    //       reasoning - because of threads that opencv uses to process frames
    uint64_t ms_between_frames = 1uLL;

  } options;

  /// <summary>
  /// Replaces internal options with provided ones
  /// </summary>
  void set_options(options opt);

  /// <summary>
  /// Opens video file to process frames
  /// </summary>
  /// <param name="video_path"></param>
  /// <returns>true if ok, false if error</returns>
  bool open_video(const char* video_path);

  /// <summary>
  /// Processes a single frame from video, and returns processed frame via parameter.
  /// </summary>
  /// <param name="[output]">Output for processed frame. If nullptr, frame is not saved.</param>
  /// <returns>true if frame is saved to [output], false if [output] is nullptr</returns>
  bool process_frame(cv::Mat* output);

  /// <summary>
  /// Checks if there are any frames left
  /// </summary>
  /// <returns>true if no more frames, false if there are frames</returns>
  bool finished();

}
