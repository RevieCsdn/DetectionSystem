// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 CURL_AI_CLIENT_LIB_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// CURL_AI_CLIENT_LIB_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
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
    // posturl: 服务器url
    // ot: 接收超时时间，单位s
    curl_interface(const char * posturl, long ot);
    ~curl_interface();

    static void     curl_init();

    // 发送jpeg类型图片
    // filename: 图片名字
    // img_buffer: jpeg格式图片数据
    // size: 图片数据大小，单位字节
    // content: 服务器返回的内容，如果没收到数据返回NULL指针
    // 执行成功返回true，反之false
    bool            post_jpeg_type(char *filename, BYTE *img_buffer, unsigned int size, char **content);

    // 发送灰度图片原数据
    // filename: 图片名字
    // img_buffer: 灰度图片数据
    // img_w: 图片宽度，单位像素
    // img_h: 图片高度，单位像素
    // content: 服务器返回的内容，如果没收到数据返回NULL指针
    // 执行成功返回true，反之false
    bool            post_gray_raw_type(char *filename, BYTE *img_buffer, int img_w, int img_h, char **content);

    // 获取错误代码
    // 返回错误代码
    int             get_code_res();

    // 连接检测
    // 连接成功返回true，反之false
    bool            is_connect();

private:
    static bool bInit;
    CURL        *curl;
    CURLcode    code_res;
};