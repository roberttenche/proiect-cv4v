#include "lane_detector.hpp"

#include <iostream>
#include <memory>
#include <string>

/// PRIVATE
namespace lane_detector
{
  namespace _private
  {
    static std::chrono::steady_clock::time_point start_time;
    static uint64_t frame_count = 0uLL;

    static std::string title("lane_detector");
    
    static cv::VideoCapture cap;
    static options opt;

    static bool apply_processing(cv::Mat* output)
    {
      if (cap.isOpened() == false)
      {
        output = nullptr;
        return false;
      }

      cv::Mat raw_frame;

      cap.read(raw_frame);

      if (raw_frame.empty())
      {
        throw std::exception("blank frame");
      }

      cv::Mat processed_frame;

      cv::cvtColor(raw_frame, processed_frame, cv::COLOR_RGB2GRAY);

      cv::pyrDown(processed_frame, processed_frame);

      cv::GaussianBlur(processed_frame, processed_frame, cv::Size(3, 3), 0);

      cv::Canny(processed_frame, processed_frame, 100.0, 170.0);

      cv::pyrUp(processed_frame, processed_frame);

      if (opt.display_fps == true) {
 
        frame_count++;

        auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);

        if (elapsed.count() > 1u)
        {
          std::stringstream ss;
          ss << title << " [" << frame_count << " FPS]";
          cv::setWindowTitle(title, ss.str());

          frame_count = 0;
          start_time = current_time;
        }

        if (opt.display_frames == true)
        {
          cv::imshow(title, processed_frame);
          if (cv::waitKey(1u) >= 0)
            return false;
          else
            return true;
        }
      }
    
      if (output == nullptr) {
        return false;
      }

      *output = processed_frame;

      return true;

    }

  }
}

/// PUBLIC
namespace lane_detector
{
  void set_options(options Opt)
  {
    _private::opt = Opt;
  }

  bool finished()
  {
    return not _private::cap.isOpened();
  }

  bool open_video(const char* video_path)
  {
    return _private::cap.open(video_path);
  }

  bool get_frame(cv::Mat* output)
  {
    return lane_detector::_private::apply_processing(output);
  }

  bool init()
  {
    _private::start_time = std::chrono::high_resolution_clock::now();
    return true;
  }

}
