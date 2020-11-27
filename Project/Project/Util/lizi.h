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
#define THRESHOLD_FPC         100   //FPC��С�Ա���ֵ
#define THRESHOLD_FPCH        140   //FPC�ϴ�Ա���ֵ
#define THRESHOLD_ICH         130   //IC�ϴ�Ա���ֵ
#define THRESHOLD_IC          95    //IC��С�Ա���ֵ
#define X_SIDE                15    //ICƬһ����
#define Y_SIDE                130   //ICƬһ��߶�
//#define AREA_TH               50

using namespace std;
using namespace cv;

#ifdef _DEBUG
    #define __DEBUG_LIZI__
#endif

/*���ͼ�����Ƿ����������ӵ����������Լ�ǿ��
  �����롿 *img_data���������ӻҶ�ͼ�������
  �����롿 img_w����������ͼ��Ŀ�
  �����롿 *img_h����������ͼ��ĸ�
  �����롿 *oimg_data������ԭͼ����
  �����롿 oimg_w������ԭͼ�Ŀ�
  �����롿 oimg_h������ԭͼ�ĸ�
  ������/����� &blob_num: ����long long���͵����ã����������⵽��������Ŀ,��ʼ��Ϊ0
  ������/����� **blobs_det: ����double���͵Ŀ�ָ��ĵ�ַ�����������⵽�����ӵ���������
                            1���������ĵ�x����
                            2���������ĵ�y����
  �����롿 MilSystem��MIL_ID���͵�Milϵͳ��������
  �����롿 th_blob���ж����ӵ���ֵ
  �����롿 area_blob���ж�������С�������ֵ
  �����롿 err_area�����Ӽ����������������ֵ
  ������/����� flag:�ж�����ΪOK����NG
  ������� bool���͵�����������Ϊtrue�����ʾ�ɹ����������Ӽ�⣬�����ʾ��������δ�������Ӽ��
*/
LIZI_API bool BoolDetection(unsigned char *img_data, int img_w, int img_h, unsigned char *oimg_data, int oimg_w, int oimg_h, long long &blob_num, double **blobs_det, MIL_ID MilSystem, int th_blob, int area_blob, int err_area, bool &flag);
#endif //__LIZI_H__