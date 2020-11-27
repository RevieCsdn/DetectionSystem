// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 FOREIGNMATERIALDETECTOR_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// FOREIGNMATERIALDETECTOR_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef FOREIGNMATERIALDETECTOR_EXPORTS
#define FOREIGNMATERIALDETECTOR_API __declspec(dllexport)
#else
#define FOREIGNMATERIALDETECTOR_API __declspec(dllimport)
#endif
// 需要使用的库调用 //
#include <iostream>
#include <fstream>
#include<io.h>
#include<string>
#include<vector>
#include "opencv2\opencv.hpp"
#include <iostream>


/********************************************************
*|-厦门友达异物检										*
*-------------------------------------------------------*
*|-【参数列表】：										*
*	|-【1】mark_image：mark标记位置						*
*	|-【2】mark_x：mark左上角x坐标						*
*	|-【3】mark_y：mark左上角y坐标						*
*	|-【4】src_img：待检测图像							*
*	|-【5】template_img：模板图像						*
*	|-【6】binary_value：缺陷分割阈值					*
*	|-【7】median_blur_size:中值滤波大小（必须为奇数）	*
*	|-【8】result_rect：缺陷位置						*
*	|-【9】cuda_flag：cuda加速是否开启，默认不开启		*
*********************************************************/
FOREIGNMATERIALDETECTOR_API int ForeignMaterialDetector
	(cv::Mat mark_img, float mark_x, float mark_y,
	const cv::Mat src_img, cv::Mat template_img,
	int binary_value, int median_blur_size,
	std::vector<cv::Rect> &result_rects);