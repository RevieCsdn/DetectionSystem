// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� FOREIGNMATERIALDETECTOR_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// FOREIGNMATERIALDETECTOR_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef FOREIGNMATERIALDETECTOR_EXPORTS
#define FOREIGNMATERIALDETECTOR_API __declspec(dllexport)
#else
#define FOREIGNMATERIALDETECTOR_API __declspec(dllimport)
#endif
// ��Ҫʹ�õĿ���� //
#include <iostream>
#include <fstream>
#include<io.h>
#include<string>
#include<vector>
#include "opencv2\opencv.hpp"
#include <iostream>


/********************************************************
*|-�����Ѵ������										*
*-------------------------------------------------------*
*|-�������б���										*
*	|-��1��mark_image��mark���λ��						*
*	|-��2��mark_x��mark���Ͻ�x����						*
*	|-��3��mark_y��mark���Ͻ�y����						*
*	|-��4��src_img�������ͼ��							*
*	|-��5��template_img��ģ��ͼ��						*
*	|-��6��binary_value��ȱ�ݷָ���ֵ					*
*	|-��7��median_blur_size:��ֵ�˲���С������Ϊ������	*
*	|-��8��result_rect��ȱ��λ��						*
*	|-��9��cuda_flag��cuda�����Ƿ�����Ĭ�ϲ�����		*
*********************************************************/
FOREIGNMATERIALDETECTOR_API int ForeignMaterialDetector
	(cv::Mat mark_img, float mark_x, float mark_y,
	const cv::Mat src_img, cv::Mat template_img,
	int binary_value, int median_blur_size,
	std::vector<cv::Rect> &result_rects);