#ifdef FOREIGNMATERIALDETECTOR_EXPORTS
#define FOREIGNMATERIALDETECTOR_API __declspec(dllexport)
#else
#define FOREIGNMATERIALDETECTOR_API __declspec(dllimport)
#endif
/* ��Ҫʹ�õĿ���� */
#include<string>
#include<vector>
#include <iostream>
#include "opencv2\opencv.hpp"

//typedef struct RectPoint {
//	cv::Point LeftUpPoint;
//	cv::Point RightDownPoint;
//}RectPoint_t;

/********************************************************/
/*|-�����Ѵ������										*/
/*------------------------------------------------------*/
/*|-�������б���										*/
/*	|-��1��mark_image��mark���λ��						*/
/*	|-��2��mark_x��mark���Ͻ�x����						*/
/*	|-��3��mark_y��mark���Ͻ�y����						*/
/*	|-��4��src_img�������ͼ��							*/
/*	|-��5��temp_img��ģ��ͼ��							*/
/*	|-��6��binary_value��ȱ�ݷָ���ֵ					*/
/*	|-��7��median_blur_size:��ֵ�˲���С������Ϊ������	*/
/*	|-��8��												*/
/*	|-��9��result_rect��ȱ��λ��						*/
/*********************************************************/
FOREIGNMATERIALDETECTOR_API int ForeignMaterialDetector(cv::Mat mark_img, float mark_x, float mark_y,
														const cv::Mat src_img, cv::Mat template_img,
														int binary_value, int median_blur_size,
														std::vector<RectPoint_t> no_detect_area,
														std::vector<cv::Rect> &result_rects);
