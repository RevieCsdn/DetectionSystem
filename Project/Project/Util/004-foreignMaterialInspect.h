#ifdef MY004FOREIGNMATERIALINSPECT_EXPORTS
#define MY004FOREIGNMATERIALINSPECT_API __declspec(dllexport)
#else
#define MY004FOREIGNMATERIALINSPECT_API __declspec(dllimport)
#endif
#include "opencv2/opencv.hpp"
#include <iostream>

MY004FOREIGNMATERIALINSPECT_API int foreignMaterialInspect(
	cv::Mat markIMG, int markX, int markY,
	cv::Mat srcIMG, cv::Mat tempIMG,
	int binaryValue, int medianBlurSize, std::vector<cv::Rect> &resultRect);
