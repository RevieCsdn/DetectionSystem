#ifdef FOREIGNMATERIALDETECTOR_EXPORTS
#define FOREIGNMATERIALDETECTOR_API __declspec(dllexport)
#else
#define FOREIGNMATERIALDETECTOR_API __declspec(dllimport)
#endif
/* ��Ҫʹ�õĿ���� */
#pragma once
#include "opencv2\opencv.hpp"
#include<string>
#include<vector>
#include <iostream>

using std::string;


typedef struct RectPoint 
{
	cv::Point LeftUpPoint;
	cv::Point RightDownPoint;
}RectPoint_t;

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
FOREIGNMATERIALDETECTOR_API int ForeignMaterialDetector(
	const cv::Mat &mark_img, const int &mark_x, const int &mark_y,
	const cv::Mat &src_img, const cv::Mat &template_img,
	const int &binary_value, const int &median_blur_size,
	const std::vector<RectPoint> &no_detect_area,
	std::vector<cv::Rect> &result_rects,
	const string &image_name = "no image name");
