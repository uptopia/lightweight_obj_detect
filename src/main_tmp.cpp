#include <iostream>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "nanodet_openvino.h"
#include "tool.h"
#include <openvino_test/bbox.h>
#include <openvino_test/bboxes.h>

static const std::string OPENCV_WINDOW = "Image window";
auto detector = NanoDet("nanodet.xml");

openvino_test::bbox bb;


class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    // image_sub_ = it_.subscribe("/camera/color/image_raw_workspace", 1,
    //   &ImageConverter::imageCb, this);
    image_sub_ = it_.subscribe("/usb_cam/image_raw", 1,
      &ImageConverter::imageCb, this);
    // image_pub_ = it_.advertise("/image_converter/output_video", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  std::vector<float> bb_v;

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

    object_rect effect_roi;
    cv::Mat resized_img;
    resize_uniform(cv_ptr->image, resized_img, cv::Size(416, 416), effect_roi);

    auto results = detector.detect(resized_img, 0.4, 0.5);
    draw_bboxes(cv_ptr->image, results, effect_roi);

    for(int n=0;n<3;n++)
    {
      std::cout << bb_v[n] << "\n";
    }
    // cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    // cv::imshow("resized_img", resized_img);
    cv::waitKey(1);
    // Output modified video stream
    // image_pub_.publish(cv_ptr->toImageMsg());
  }
  void msg_publish(const cv::Mat& bgr, const std::vector<BoxInfo>& bboxes, object_rect effect_roi)
  {
    static const char* class_names[] = { "screw", "screw_n", "motor", "terminal+", "terminal-"};

    cv::Mat image = bgr.clone();
    int src_w = image.cols;
    int src_h = image.rows;
    int dst_w = effect_roi.width;
    int dst_h = effect_roi.height;
    float width_ratio = (float)src_w / (float)dst_w;
    float height_ratio = (float)src_h / (float)dst_h;
    float xmin, ymin, xmax, ymax;


  }

};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");

  bb.xmin = 0;
  bb.ymin = 20;
  bb.xmax = 20;
  bb.ymax = 30;

  std::cout << bb; 

  ImageConverter ic;
  ros::spin();
  return 0;
}