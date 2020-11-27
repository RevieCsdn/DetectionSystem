#pragma once

#include <iostream>
#include <fstream>
#include <list>
#include <wx/wx.h>
#include <math.h>

using namespace std;

class RoiRect
{
public:
    RoiRect() {x_ = 0; y_ = 0; w_ = 0; h_ = 0;}
    RoiRect(int x, int y, int w, int h)
        :x_(x), y_(y), w_(w), h_(h){}
    RoiRect(const RoiRect &rect){x_ = rect.x_; y_ = rect.y_; w_ = rect.w_; h_ = rect.h_;}
    void setData(int x, int y, int w, int h)
    {
        x_ = x; 
        y_ = y; 
        w_ = w; 
        h_ = h;
    }
    void operator=(const RoiRect &rect){x_ = rect.x_; y_ = rect.y_; w_ = rect.w_; h_ = rect.h_;}
    int getX(){return x_;}
    int getY(){return y_;}
    int getWidth(){return w_;}
    int getHeight(){return h_;}
private:	
    int x_;
    int y_;
    int w_;
    int h_;

};

class SplitRect
{
public:
    SplitRect(){data_ =NULL; is_need_delete =false;}
    SplitRect(const SplitRect &rect){data_ = rect.data_; roirect_ = rect.roirect_;}
    void setData(BYTE * data){data_ = data;}
    BYTE *getData(){return data_;}
    void copyData(BYTE *data ,int size)
    {
        data_ =new BYTE[size];
        memcpy(data_,data,size);
    }
    void setRoi(RoiRect &rect){roirect_ = rect;}
    void SetNeedDelete(bool is_need){is_need_delete = is_need;}
    RoiRect &getRoi(){return roirect_;}
private:
    BYTE *data_;
    RoiRect roirect_;
    bool is_need_delete;

};

class SingleImage
{
public:
    SingleImage(int v_img_size,int v_pool_size);
    ~SingleImage(void);

    void         PushImageBuf(BYTE *v_buf);
    BYTE*        GetImageBufAll();
    BYTE*        GetCurlImageBuf();
    BYTE*        GetImageBuf(int num);
	void         GetImageBufSum(int num,BYTE ** v_buf);
    void         SetImageBufPoolSize(int v_size);
    void         SetImageSize(int v_size);
    void         SetImageNum(int v_num);
    int          GetImageNum();

    int          GetAiImageSideNum();
    int          GetSelectImageNum();
    BYTE*        GetErrPosImage(int x_x,int y_y);
    BYTE*        GetErrPosImage(int v_num);
    void         InitBuf();
	void         List2Buf();

    void         SetSingImageStatu(bool is_busy);
    bool         GetSingImageStatu();

    list<SplitRect>*	Buf2List();
    list<SplitRect>*    GetSplitRect();
private:
    BYTE                *img_buf_pool;
    BYTE                *img_buf_temp_pool;
	BYTE                *img_buf_sum_temp;
    BYTE                *img_buf_err;
    int                 offset_img_buf;
    int                 offset_w_img_buf;
    int                 size_of_img_buf_pool;
    int                 size_of_img_buf;
    int                 num_of_img;
    int                 num_of_ai_img_size;
    int                 m_select_num;

    int                 img_buf_side_length;
    list<SplitRect>     list_img_buf;

    bool                m_is_busy;
};

class ImagePool
{
public:
    ImagePool(int v_img_size,int v_pool_size ,int num_img);
    ~ImagePool();

    static ImagePool     *Instance(int v_img_size,int v_pool_size ,int num_img);
    static ImagePool     *GetInstance();
    SingleImage*          GetSingleImage(int v_num);
    SingleImage*          GetFreeSingleImage();
    void                  ResetPool();
private:
    static  ImagePool *    _instance;
    list<SingleImage*>     list_single_img;
    wxMutex                *m_mutex;
};