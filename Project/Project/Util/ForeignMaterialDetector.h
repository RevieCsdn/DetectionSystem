#ifdef FOREIGNMATERIALDETECTOR_EXPORTS
#define FOREIGNMATERIALDETECTOR_API __declspec(dllexport)
#else
#define FOREIGNMATERIALDETECTOR_API __declspec(dllimport)
#endif
/* 需要使用的库调用 */
#include<string>
#include<vector>
#include <iostream>
#include "opencv2\opencv.hpp"

//typedef struct RectPoint {
//	cv::Point LeftUpPoint;
//	cv::Point RightDownPoint;
//}RectPoint_t;

/********************************************************/
/*|-厦门友达异物检										*/
/*------------------------------------------------------*/
/*|-【参数列表】：										*/
/*	|-【1】mark_image：mark标记位置						*/
/*	|-【2】mark_x：mark左上角x坐标						*/
/*	|-【3】mark_y：mark左上角y坐标						*/
/*	|-【4】src_img：待检测图像							*/
/*	|-【5】temp_img：模板图像							*/
/*	|-【6】binary_value：缺陷分割阈值					*/
/*	|-【7】median_blur_size:中值滤波大小（必须为奇数）	*/
/*	|-【8】												*/
/*	|-【9】result_rect：缺陷位置						*/
/*********************************************************/
FOREIGNMATERIALDETECTOR_API int ForeignMaterialDetector(cv::Mat mark_img, float mark_x, float mark_y,
														const cv::Mat src_img, cv::Mat template_img,
														int binary_value, int median_blur_size,
														std::vector<RectPoint_t> no_detect_area,
														std::vector<cv::Rect> &result_rects);
