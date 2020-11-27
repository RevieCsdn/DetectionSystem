// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 BUMPDETECTOR_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// BUMPDETECTOR_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef BUMPDETECTOR_EXPORTS
#define BUMPDETECTOR_API __declspec(dllexport)
#else
#define BUMPDETECTOR_API __declspec(dllimport)
#endif
#include <windows.h>
#include "AlgorithmLog.h"
#include <list>
#include <iostream>
#include <vector>
#include "opencv2\opencv.hpp"
#include <list>
#define JUNQIANG 0
#define HEWEI 1

/*IC异物缺陷检测（俊强版）
【输入】 *img_data:采集到的图像数据
【输入】 imgWidth：采集到的图像的宽
【输入】 imgHeight：采集到的图像的高
【输入】 xc：截至值xc
【输入】 dw：缺陷权重
【输入】 ow：原始权重，一般设置为1
【输入】 sigma：局部二值化阈值，出现漏检的情况，请调低
【输入】 divideSize：局部二值化阈值
【输入】 mergeLength：缺陷区域融合长度（对角线长度）
【输入】 filterThres : 缺陷过滤阈值
【输入】 filterLength：缺陷过滤尺寸（设置越高，速度越慢）

【输出】 *defectNum：缺陷的数量
【输出】 **err_rect：输入double类型的空指针的地址，
输出检测到的缺陷的左上角Rect坐标，宽度，高度
*/
#if JUNQIANG
BUMPDETECTOR_API bool bumpDetector(unsigned char *imgData, int imgWidth, int imgHeight,
	float xc,
	float dw,
	int ow,
	int delta,
	int divideSize,
	int mergeLength,
	double filterThres,
	int filterSize,
	int *defectNum,
	double **err_rect
);
#endif


/*IC异物缺陷检测
【输入】 srcImage：输入Mat类型的图片
【输入】 xc：截至值xc
【输入】 dw：缺陷权重
【输入】 ow：原始权重，一般设置为1
【输入】 sigma：局部二值化阈值，出现漏检的情况，请调低
【输入】divideSize：局部二值化阈值
【输出】errorRect：缺陷区域
*/
#if HEWEI
BUMPDETECTOR_API int bumpDetector(
	cv::Mat srcImage,
	float xc,
	float dw,
	int ow,
	int delta,
	int divideSize,
	int mergeLength,
	double filterThres,
	int filterSize,
	std::vector<cv::Rect> &errorRect
	);
#endif