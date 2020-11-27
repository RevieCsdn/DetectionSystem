// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� BUMPDETECTOR_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// BUMPDETECTOR_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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

/*IC����ȱ�ݼ�⣨��ǿ�棩
�����롿 *img_data:�ɼ�����ͼ������
�����롿 imgWidth���ɼ�����ͼ��Ŀ�
�����롿 imgHeight���ɼ�����ͼ��ĸ�
�����롿 xc������ֵxc
�����롿 dw��ȱ��Ȩ��
�����롿 ow��ԭʼȨ�أ�һ������Ϊ1
�����롿 sigma���ֲ���ֵ����ֵ������©�������������
�����롿 divideSize���ֲ���ֵ����ֵ
�����롿 mergeLength��ȱ�������ںϳ��ȣ��Խ��߳��ȣ�
�����롿 filterThres : ȱ�ݹ�����ֵ
�����롿 filterLength��ȱ�ݹ��˳ߴ磨����Խ�ߣ��ٶ�Խ����

������� *defectNum��ȱ�ݵ�����
������� **err_rect������double���͵Ŀ�ָ��ĵ�ַ��
�����⵽��ȱ�ݵ����Ͻ�Rect���꣬��ȣ��߶�
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


/*IC����ȱ�ݼ��
�����롿 srcImage������Mat���͵�ͼƬ
�����롿 xc������ֵxc
�����롿 dw��ȱ��Ȩ��
�����롿 ow��ԭʼȨ�أ�һ������Ϊ1
�����롿 sigma���ֲ���ֵ����ֵ������©�������������
�����롿divideSize���ֲ���ֵ����ֵ
�������errorRect��ȱ������
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