#include <lane_detector.hpp>

int main()
{
  lane_detector::open_video("C:\\Users\\teenc\\Desktop\\5h.mp4");

  lane_detector::options opt;

  opt.display_fps = true;
  opt.display_frames = true;

  lane_detector::set_options(opt);

  while(not lane_detector::finished())
  {
    lane_detector::get_frame(nullptr);
  }

  return 0;
}
