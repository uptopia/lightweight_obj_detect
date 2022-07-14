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
#include <lightweight_obj_detect/bbox.h>
#include <lightweight_obj_detect/bboxes.h>

static const std::string OPENCV_WINDOW = "Image window";
auto detector = NanoDet("nanodet.xml");
bool draw = true;

class ImageConverter
{
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;

    ros::Publisher pub_other_objs = nh_.advertise<lightweight_obj_detect::bboxes>("/Det_other_objs_bboxes", 1000);
    ros::Publisher pub_motors = nh_.advertise<lightweight_obj_detect::bboxes>("/Det_motor_bboxes", 1000);

public:
    ImageConverter()
    : it_(nh_)
    {
        image_sub_ = it_.subscribe("/usb_cam/image_raw", 1, &ImageConverter::imageCb, this);

    }

    ~ImageConverter()
    {
        // cv::destroyWindow(OPENCV_WINDOW);
        cv::destroyAllWindows();
    }

    void imageCb(const sensor_msgs::ImageConstPtr& msg)
    {
        cv_bridge::CvImagePtr cv_ptr;
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

        object_rect effect_roi;
        cv::Mat resized_img;
        resize_uniform(cv_ptr->image, resized_img, cv::Size(416, 416), effect_roi);

        auto results = detector.detect(resized_img, 0.4, 0.5);

        if(draw==true)draw_bboxes(cv_ptr->image, results, effect_roi);
        this->msg_publish(cv_ptr->image, results, effect_roi);

        // cv::imshow(OPENCV_WINDOW, cv_ptr->image);
        // cv::imshow("resized_img", resized_img);
        cv::waitKey(1);
    }

    void msg_publish(const cv::Mat& bgr, const std::vector<BoxInfo>& bboxes, object_rect effect_roi)
    {   
        lightweight_obj_detect::bboxes bbs_other_objs;
        lightweight_obj_detect::bboxes bbs_motors;

        static const char* class_names[] = { "screw", "screw_n", "motor", "terminal+", "terminal-"};

        cv::Mat image = bgr.clone();
        int src_w = image.cols;
        int src_h = image.rows;
        int dst_w = effect_roi.width;
        int dst_h = effect_roi.height;
        float width_ratio = (float)src_w / (float)dst_w;
        float height_ratio = (float)src_h / (float)dst_h;
        float xmin, ymin, xmax, ymax;

        for (size_t i = 0; i < bboxes.size(); i++)
        {   
            lightweight_obj_detect::bbox bb;
            const BoxInfo& bbox = bboxes[i];
            xmin = (bbox.x1 - effect_roi.x) * width_ratio;
            ymin = (bbox.y1 - effect_roi.y) * height_ratio;
            xmax = (bbox.x2 - effect_roi.x) * width_ratio;
            ymax = (bbox.y2 - effect_roi.y) * height_ratio;

            // printf("%f %f %f %f %f %s\n",xmin,ymin,xmax,ymax,bbox.score, class_names[bbox.label]);
            bb.xmin = int(xmin);
            bb.ymin = int(ymin);
            bb.xmax = int(xmax);
            bb.ymax = int(ymax);
            bb.score = bbox.score;
            bb.object_name = class_names[bbox.label];

            if(bbox.label == 2)
            {   
                // std::cout << "motor\n";
                bbs_motors.bboxes.push_back(bb);
            }
            else
            {
                // std::cout << "others\n";
                bbs_other_objs.bboxes.push_back(bb);
            }
        }
        pub_motors.publish(bbs_motors);
        pub_other_objs.publish(bbs_other_objs);
        // std::cout << "---------\n";
    }

};


int main(int argc, char** argv)
{
  ros::init(argc, argv, "Det_Node");
  ImageConverter ic;
  ros::spin();
  return 0;
}
