#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "nanodet_openvino.h"

struct object_rect {
    int x;
    int y;
    int width;
    int height;
};

int resize_uniform(cv::Mat&, cv::Mat&, cv::Size, object_rect&);

void draw_bboxes(const cv::Mat&, const std::vector<BoxInfo>&, object_rect);

// std::vector<float> &draw_bboxes2(const cv::Mat&, const std::vector<BoxInfo>&, object_rect);

