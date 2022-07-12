#include "tool.h"

int resize_uniform(cv::Mat& src, cv::Mat& dst, cv::Size dst_size, object_rect& effect_area)
{
    int w = src.cols;
    int h = src.rows;
    int dst_w = dst_size.width;
    int dst_h = dst_size.height;
    //std::cout << "src: (" << h << ", " << w << ")" << std::endl;
    dst = cv::Mat(cv::Size(dst_w, dst_h), CV_8UC3, cv::Scalar(0));

    float ratio_src = w * 1.0 / h;
    float ratio_dst = dst_w * 1.0 / dst_h;

    int tmp_w = 0;
    int tmp_h = 0;
    if (ratio_src > ratio_dst) {
        tmp_w = dst_w;
        tmp_h = floor((dst_w * 1.0 / w) * h);
    }
    else if (ratio_src < ratio_dst) {
        tmp_h = dst_h;
        tmp_w = floor((dst_h * 1.0 / h) * w);
    }
    else {
        cv::resize(src, dst, dst_size);
        effect_area.x = 0;
        effect_area.y = 0;
        effect_area.width = dst_w;
        effect_area.height = dst_h;
        return 0;
    }

    //std::cout << "tmp: (" << tmp_h << ", " << tmp_w << ")" << std::endl;
    cv::Mat tmp;
    cv::resize(src, tmp, cv::Size(tmp_w, tmp_h));

    if (tmp_w != dst_w) {
        int index_w = floor((dst_w - tmp_w) / 2.0);
        //std::cout << "index_w: " << index_w << std::endl;
        for (int i = 0; i < dst_h; i++) {
            memcpy(dst.data + i * dst_w * 3 + index_w * 3, tmp.data + i * tmp_w * 3, tmp_w * 3);
        }
        effect_area.x = index_w;
        effect_area.y = 0;
        effect_area.width = tmp_w;
        effect_area.height = tmp_h;
    }
    else if (tmp_h != dst_h) {
        int index_h = floor((dst_h - tmp_h) / 2.0);
        //std::cout << "index_h: " << index_h << std::endl;
        memcpy(dst.data + index_h * dst_w * 3, tmp.data, tmp_w * tmp_h * 3);
        effect_area.x = 0;
        effect_area.y = index_h;
        effect_area.width = tmp_w;
        effect_area.height = tmp_h;
    }
    else {
        printf("error\n");
    }
    //cv::imshow("dst", dst);
    //cv::waitKey(0);
    return 0;
}

const int color_list[5][3] =
{
    //{255 ,255 ,255}, //bg
    {216 , 82 , 24},
    {236 ,176 , 31},
    {125 , 46 ,141},
    {118 ,171 , 47},
    { 76 ,189 ,237},
    
};

void draw_bboxes(const cv::Mat& bgr, const std::vector<BoxInfo>& bboxes, object_rect effect_roi)
{
    static const char* class_names[] = { "screw", "screw_n", "motor", "terminal+", "terminal-"};

    cv::Mat image = bgr.clone();
    int src_w = image.cols;
    int src_h = image.rows;
    int dst_w = effect_roi.width;
    int dst_h = effect_roi.height;
    float width_ratio = (float)src_w / (float)dst_w;
    float height_ratio = (float)src_h / (float)dst_h;
    int xmin, ymin, xmax, ymax;
    float bb[6];
    for (size_t i = 0; i < bboxes.size(); i++)
    {
        const BoxInfo& bbox = bboxes[i];
        cv::Scalar color = cv::Scalar(color_list[bbox.label][0], color_list[bbox.label][1], color_list[bbox.label][2]);
        //fprintf(stderr, "%d = %.5f at %.2f %.2f %.2f %.2f\n", bbox.label, bbox.score,
        //    bbox.x1, bbox.y1, bbox.x2, bbox.y2);

        cv::rectangle(image, cv::Rect(cv::Point((bbox.x1 - effect_roi.x) * width_ratio, (bbox.y1 - effect_roi.y) * height_ratio),
                                      cv::Point((bbox.x2 - effect_roi.x) * width_ratio, (bbox.y2 - effect_roi.y) * height_ratio)), color);
        

        // cv::circle(image, cv::Point((bbox.x1 - effect_roi.x) * width_ratio, (bbox.y1 - effect_roi.y) * height_ratio), 1, cv::Scalar(0, 255, 0), cv::FILLED);
        // cv::circle(image, cv::Point((bbox.x2 - effect_roi.x) * width_ratio, (bbox.y2 - effect_roi.y) * height_ratio), 1, cv::Scalar(0, 255, 0), cv::FILLED);

        xmin = (bbox.x1 - effect_roi.x) * width_ratio;
        ymin = (bbox.y1 - effect_roi.y) * height_ratio;
        xmax = (bbox.x2 - effect_roi.x) * width_ratio;
        ymax = (bbox.y2 - effect_roi.y) * height_ratio;

        char text[256];
        sprintf(text, "%s %.1f%%", class_names[bbox.label], bbox.score * 100);

        int baseLine = 0;
        cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.4, 1, &baseLine);

        int x = (bbox.x1 - effect_roi.x) * width_ratio;
        int y = (bbox.y1 - effect_roi.y) * height_ratio - label_size.height - baseLine;
        if (y < 0)
            y = 0;
        if (x + label_size.width > image.cols)
            x = image.cols - label_size.width;

        cv::rectangle(image, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),
            color, -1);

        cv::putText(image, text, cv::Point(x, y + label_size.height),
            cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255));
    }
    
    // std::cout << image.size() << std::endl;
    cv::imshow("image", image);
}


// std::vector<float> &draw_bboxes2(const cv::Mat& bgr, const std::vector<BoxInfo>& bboxes, object_rect effect_roi)
// {
//     static const char* class_names[] = { "screw", "screw_n", "motor", "terminal+", "terminal-"};

//     cv::Mat image = bgr.clone();
//     int src_w = image.cols;
//     int src_h = image.rows;
//     int dst_w = effect_roi.width;
//     int dst_h = effect_roi.height;
//     float width_ratio = (float)src_w / (float)dst_w;
//     float height_ratio = (float)src_h / (float)dst_h;
//     float xmin, ymin, xmax, ymax;
//     // float bb[6];
    
//     for (size_t i = 0; i < bboxes.size(); i++)
//     {
//         const BoxInfo& bbox = bboxes[i];
//         BoxInfo bb;
//         cv::Scalar color = cv::Scalar(color_list[bbox.label][0], color_list[bbox.label][1], color_list[bbox.label][2]);
//         //fprintf(stderr, "%d = %.5f at %.2f %.2f %.2f %.2f\n", bbox.label, bbox.score,
//         //    bbox.x1, bbox.y1, bbox.x2, bbox.y2);

//         cv::rectangle(image, cv::Rect(cv::Point((bbox.x1 - effect_roi.x) * width_ratio, (bbox.y1 - effect_roi.y) * height_ratio),
//                                       cv::Point((bbox.x2 - effect_roi.x) * width_ratio, (bbox.y2 - effect_roi.y) * height_ratio)), color);
        

//         // cv::circle(image, cv::Point((bbox.x1 - effect_roi.x) * width_ratio, (bbox.y1 - effect_roi.y) * height_ratio), 1, cv::Scalar(0, 255, 0), cv::FILLED);
//         // cv::circle(image, cv::Point((bbox.x2 - effect_roi.x) * width_ratio, (bbox.y2 - effect_roi.y) * height_ratio), 1, cv::Scalar(0, 255, 0), cv::FILLED);

//         xmin = (bbox.x1 - effect_roi.x) * width_ratio;
//         ymin = (bbox.y1 - effect_roi.y) * height_ratio;
//         xmax = (bbox.x2 - effect_roi.x) * width_ratio;
//         ymax = (bbox.y2 - effect_roi.y) * height_ratio;

//         char text[256];
//         sprintf(text, "%s %.1f%%", class_names[bbox.label], bbox.score * 100);

//         int baseLine = 0;
//         cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.4, 1, &baseLine);

//         int x = (bbox.x1 - effect_roi.x) * width_ratio;
//         int y = (bbox.y1 - effect_roi.y) * height_ratio - label_size.height - baseLine;
//         if (y < 0)
//             y = 0;
//         if (x + label_size.width > image.cols)
//             x = image.cols - label_size.width;

//         cv::rectangle(image, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),
//             color, -1);

//         cv::putText(image, text, cv::Point(x, y + label_size.height),
//             cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255));

//         // bb_v.push_back(xmin);
//         // bb_v.push_back(ymin);
//         // bb_v.push_back(xmax);
//         // bb_v.push_back(ymax);
//         // bb_v.push_back(bbox.score);
//         // bb_v.push_back(bbox.label);

//         bb.x1 = xmin;
//         bb.y1 = ymin;
//         bb.x2 = xmax;
//         bb.y2 = ymax;
//         bb.score = 
//     }
    
//     // std::cout << image.size() << std::endl;
//     cv::imshow("image", image);

//     return bb_v;
// }