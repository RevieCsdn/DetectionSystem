// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� CURL_AI_CLIENT_LIB_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// CURL_AI_CLIENT_LIB_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#pragma once
#ifdef CURL_AI_CLIENT_LIB_EXPORTS
#define CURL_AI_CLIENT_LIB_API __declspec(dllexport)
#else
#define CURL_AI_CLIENT_LIB_API __declspec(dllimport)
#endif

//#define NDEBUG
#define BUILDING_LIBCURL
//#define USE_OPENSSL

#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <curl\curl.h>

using namespace std;

//extern enum CURLcode;
//extern struct CURL;

typedef struct {
    double Pos[5];
} CURL_AI_CLIENT_LIB_API Pos_type;

typedef struct {
    vector<Pos_type> Pos;
    string label;
} CURL_AI_CLIENT_LIB_API predictions_type;

typedef struct {
    string id;
    vector<predictions_type> predictions;
    bool succes;
} CURL_AI_CLIENT_LIB_API ai_result_type;

CURL_AI_CLIENT_LIB_API bool json_resolver(char *indata, ai_result_type &outdata);


class CURL_AI_CLIENT_LIB_API curl_interface
{
public:
    // posturl: ������url
    // ot: ���ճ�ʱʱ�䣬��λs
    curl_interface(const char * posturl, long ot);
    ~curl_interface();

    static void     curl_init();

    // ����jpeg����ͼƬ
    // filename: ͼƬ����
    // img_buffer: jpeg��ʽͼƬ����
    // size: ͼƬ���ݴ�С����λ�ֽ�
    // content: ���������ص����ݣ����û�յ����ݷ���NULLָ��
    // ִ�гɹ�����true����֮false
    bool            post_jpeg_type(char *filename, BYTE *img_buffer, unsigned int size, char **content);

    // ���ͻҶ�ͼƬԭ����
    // filename: ͼƬ����
    // img_buffer: �Ҷ�ͼƬ����
    // img_w: ͼƬ��ȣ���λ����
    // img_h: ͼƬ�߶ȣ���λ����
    // content: ���������ص����ݣ����û�յ����ݷ���NULLָ��
    // ִ�гɹ�����true����֮false
    bool            post_gray_raw_type(char *filename, BYTE *img_buffer, int img_w, int img_h, char **content);

    // ��ȡ�������
    // ���ش������
    int             get_code_res();

    // ���Ӽ��
    // ���ӳɹ�����true����֮false
    bool            is_connect();

private:
    static bool bInit;
    CURL        *curl;
    CURLcode    code_res;
};