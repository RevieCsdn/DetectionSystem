#ifndef __LIZI_H__
#define __LIZI_H__
#ifdef LIZI_EXPORTS
#define LIZI_API __declspec(dllexport)
#else
#define LIZI_API __declspec(dllimport)
#endif

#include<mil.h>
#include <list>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

#define EDGE_TH               160
#define SIZE_LEDGE            10
#define SIZE_SLEDGE           2
#define SIZE_SUEDGE           3
#define THRESHOLD_FPC         100   //FPC较小对比阈值
#define THRESHOLD_FPCH        140   //FPC较大对比阈值
#define THRESHOLD_ICH         130   //IC较大对比阈值
#define THRESHOLD_IC          95    //IC较小对比阈值
#define X_SIDE                15    //IC片一般宽度
#define Y_SIDE                130   //IC片一般高度
//#define AREA_TH               50

using namespace std;
using namespace cv;

#ifdef _DEBUG
    #define __DEBUG_LIZI__
#endif

/*检测图像中是否存在异物，粒子的重心坐标以及强度
  【输入】 *img_data：带框粒子灰度图像的数据
  【输入】 img_w：带框粒子图像的宽
  【输入】 *img_h：带框粒子图像的高
  【输入】 *oimg_data：粒子原图数据
  【输入】 oimg_w：粒子原图的宽
  【输入】 oimg_h：粒子原图的高
  【输入/输出】 &blob_num: 输入long long类型的引用，它将输出检测到的粒子数目,初始化为0
  【输入/输出】 **blobs_det: 输入double类型的空指针的地址，它将输出检测到的粒子的重心坐标
                            1、粒子重心的x坐标
                            2、粒子重心的y坐标
  【输入】 MilSystem：MIL_ID类型的Mil系统关联变量
  【输入】 th_blob：判断粒子的阈值
  【输入】 area_blob：判断粒子最小面积的阈值
  【输入】 err_area：粒子检测区域的异物面积阈值
  【输入/输出】 flag:判断粒子为OK还是NG
  【输出】 bool类型的输出，若输出为true，则表示成功进行了粒子检测，否则表示存在异物未进入粒子检测
*/
LIZI_API bool BoolDetection(unsigned char *img_data, int img_w, int img_h, unsigned char *oimg_data, int oimg_w, int oimg_h, long long &blob_num, double **blobs_det, MIL_ID MilSystem, int th_blob, int area_blob, int err_area, bool &flag);
#endif //__LIZI_H__