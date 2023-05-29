#include "lane_detector.hpp"

#include <iostream>
#include <memory>
#include <string>

// Private function prototypes
static void handle_options(const cv::Mat& current_frame);
static void apply_image_processing(cv::Mat* input, cv::Mat* output);
static void mask_region_of_intereset(cv::Mat* input, cv::Mat* output);


// Private variables
static std::chrono::steady_clock::time_point start_time = std::chrono::high_resolution_clock::now();
static uint64_t frame_count = 0uLL;

static std::string title("lane_detector");

static cv::VideoCapture cap;
static lane_detector::options opt;

static bool apply_processing(cv::Mat* output)
{
  bool ret_value = false;

  if (cap.isOpened() == false)
  {
    ret_value = false;
    return ret_value;
  }

  cv::Mat raw_frame;

  cap.read(raw_frame);

  if (raw_frame.empty())
  {
    throw std::exception("blank frame");
  }

  cv::Mat processed_frame;

  apply_image_processing(&raw_frame, &processed_frame);

  handle_options(processed_frame);

  if (output != nullptr)
  {
    *output = processed_frame;
    ret_value = true;
  }

  return ret_value;

}



static void apply_image_processing(cv::Mat* input, cv::Mat* output)
{
  cv::Mat processed_frame = *input;

  cv::Mat hsvImage;
  cv::cvtColor(*input, hsvImage, cv::COLOR_BGR2HSV);

  cv::Scalar lowerWhite = cv::Scalar(0, 0, 0);  // Adjust the threshold as needed
  cv::Scalar upperWhite = cv::Scalar(255, 50, 255);  // Adjust the threshold as needed

  cv::Mat whiteMask;
  cv::inRange(hsvImage, lowerWhite, upperWhite, whiteMask);

  cv::Mat whitePixels;
  cv::bitwise_and(*input, *input, whitePixels, whiteMask);

  for (int y = 0; y < whitePixels.rows; ++y) {
    for (int x = 0; x < whitePixels.cols; ++x) {
      cv::Vec3b& pixel = whitePixels.at<cv::Vec3b>(y, x);
      // Increase the brightness of the white pixels
      pixel[2] = cv::saturate_cast<uchar>(pixel[2] * 1.5);  // Adjust the factor as needed
    }
  }

  cv::Mat result;
  cv::bitwise_or(whitePixels, *input, result);


  //cv::cvtColor(processed_frame, processed_frame, cv::COLOR_RGB2GRAY);

  //cv::pyrDown(processed_frame, processed_frame);

  //cv::GaussianBlur(processed_frame, processed_frame, cv::Size(5, 5), 0);

  //cv::Canny(processed_frame, processed_frame, 150.0, 170.0);

  //mask_region_of_intereset(&processed_frame, &processed_frame);

  //cv::pyrUp(processed_frame, processed_frame);

  *output = result;
}

static void mask_region_of_intereset(cv::Mat* input, cv::Mat* output)
{
  cv::Size inpSize = input->size();
  cv::Mat maskedImage;

  cv::Mat mask(input->size(), CV_8UC1, cv::Scalar::all(0));

  // rough cutout
  int roiPointX = 0; 
  int roiPointY = inpSize.height / 2;
  int roiPointW = inpSize.width;
  int roiPointH = inpSize.height / 2;

  // cutout pixels going in opposite direction
  // example: topCut cuts pixels starting from top going to bottom
  int topCut = 45;
  int leftCut = 0;
  int bottomCut = 0;
  int rightCut = 120;

  cv::Rect roiRect(
    roiPointX + leftCut,
    roiPointY + topCut - bottomCut,
    roiPointW - rightCut,
    roiPointH - topCut
  );

  cv::Mat roi(mask, roiRect);
  roi.setTo(cv::Scalar(255)); // Set the ROI region to white

  input->copyTo(maskedImage, mask);

  *output = maskedImage;

}

static void handle_options(const cv::Mat& current_frame)
{
  if (opt.display_frames == true)
  {
    if (opt.display_fps == true)
    {
      frame_count++;

      auto current_time = std::chrono::high_resolution_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);

      // atleast 1 second passed
      if (elapsed.count() > 1u)
      {
        std::stringstream ss;
        ss << title << " [" << frame_count << " FPS]";
        cv::setWindowTitle(title, ss.str());

        frame_count = 0;
        start_time = current_time;
      }
    }

    cv::imshow(title, current_frame);

    // wait 1 ms for key press
    if (cv::waitKey(1u) != -1)
    {
      cap.release();
    }
  }
}

/// PUBLIC
namespace lane_detector
{

void set_options(options Opt)
{
  opt = Opt;
}

bool finished()
{
  return not cap.isOpened();
}

bool open_video(const char* video_path)
{
  return cap.open(video_path);
}

bool process_frame(cv::Mat* output)
{
  return apply_processing(output);
}

} // namespace lane_detector
