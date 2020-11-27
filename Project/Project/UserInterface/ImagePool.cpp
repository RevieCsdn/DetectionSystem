#include "ImagePool.h"


SingleImage::SingleImage(int v_img_size,int v_pool_size)
{
    size_of_img_buf_pool =v_pool_size;
    size_of_img_buf =v_img_size;
    img_buf_pool=NULL;
    img_buf_temp_pool =NULL;
	img_buf_sum_temp =NULL;
    img_buf_err =NULL;
    num_of_ai_img_size =0;
    m_select_num =0;
    m_is_busy =false;
    num_of_img =0;
    InitBuf();
}


SingleImage::~SingleImage(void)
{
    if (img_buf_pool)
        delete img_buf_pool;
}

void SingleImage::PushImageBuf( BYTE *v_buf )
{
    if (!v_buf)   
        return;
    BYTE * pool_temp =img_buf_pool;
    pool_temp+=offset_w_img_buf*(size_of_img_buf*size_of_img_buf);
    memcpy(pool_temp,v_buf ,size_of_img_buf*size_of_img_buf);
    offset_w_img_buf++;
}


BYTE* SingleImage::GetImageBufAll()
{
    return img_buf_pool;
}

BYTE* SingleImage::GetCurlImageBuf()
{
    BYTE * pool_temp =img_buf_pool;
    pool_temp +=size_of_img_buf*size_of_img_buf*offset_img_buf;
    return pool_temp;
}

BYTE* SingleImage::GetImageBuf( int num )
{
//     if (img_buf_temp_pool)
//         delete []img_buf_temp_pool;
// 	img_buf_temp_pool=NULL;
// 	img_buf_temp_pool =new BYTE[num*size_of_img_buf*size_of_img_buf];

 
    int  sqrt_num =sqrt(num*1.0);
    int col_offset =0;
    int row_offset =0;
    BYTE * buf =img_buf_pool;
    BYTE * buf_temp =img_buf_temp_pool;
    for (int row =0; row <sqrt_num ;row++)
    {
        col_offset=0;
        row_offset=row*size_of_img_buf*size_of_img_buf*sqrt_num;
        for (int sin_row =0 ;sin_row <size_of_img_buf ;sin_row++)
        {
            for (int col =0 ;col <sqrt_num ;col++)
            {
                
                buf =(img_buf_pool+col_offset+row_offset)+col*size_of_img_buf*size_of_img_buf;
                memcpy(buf_temp,buf,size_of_img_buf);
                buf_temp+=size_of_img_buf;
                
            }
            col_offset+=size_of_img_buf;
        }
    }
    return img_buf_temp_pool;
}

void SingleImage::SetImageBufPoolSize( int v_size )
{
    size_of_img_buf_pool =v_size;
}

void SingleImage::SetImageSize(int v_size)
{
    size_of_img_buf =v_size;
}

void SingleImage::SetImageNum( int v_num )
{
    num_of_img =v_num;

    double num_temp_d =sqrt(num_of_img*1.0);
    int num_temp_int =num_temp_d;
    num_of_ai_img_size = num_of_img>(num_temp_int*num_temp_int) ? num_temp_int+1:num_temp_int;
    cout<<"ImagePool :"<<num_of_ai_img_size<<endl;
}

int SingleImage::GetImageNum()
{
    return num_of_img;
}

int SingleImage::GetAiImageSideNum()
{
    return num_of_ai_img_size;
}

int SingleImage::GetSelectImageNum()
{
    return m_select_num;
}

BYTE* SingleImage::GetErrPosImage( int x_x,int y_y )
{
    if (img_buf_err)
        delete img_buf_err;
    img_buf_err =new BYTE[size_of_img_buf*size_of_img_buf];
    m_select_num = x_x* num_of_ai_img_size +y_y;
//    cout<<"jpg num:"<<select_num<<endl;
    BYTE * img_temp =img_buf_pool+m_select_num*size_of_img_buf*size_of_img_buf;

    memcpy(img_buf_err,img_temp,size_of_img_buf*size_of_img_buf);
    return img_buf_err;
}

BYTE* SingleImage::GetErrPosImage( int v_num )
{
    if (img_buf_err)
        delete img_buf_err;
    img_buf_err =new BYTE[size_of_img_buf*size_of_img_buf];
    m_select_num = v_num;
    //    cout<<"jpg num:"<<select_num<<endl;
    BYTE * img_temp =img_buf_pool+m_select_num*size_of_img_buf*size_of_img_buf;

    memcpy(img_buf_err,img_temp,size_of_img_buf*size_of_img_buf);
    return img_buf_err;
}

void SingleImage::InitBuf()
{
    offset_img_buf = 0;
    offset_w_img_buf =0;
    num_of_ai_img_size =0;
    m_select_num =0;
    if (img_buf_pool)
        memset(img_buf_pool,0,size_of_img_buf_pool*size_of_img_buf*size_of_img_buf);
    else
    {
        img_buf_pool =new BYTE[size_of_img_buf_pool*size_of_img_buf*size_of_img_buf];
        memset(img_buf_pool,0,size_of_img_buf_pool*size_of_img_buf*size_of_img_buf);
    }
    if (img_buf_temp_pool)
        memset(img_buf_temp_pool,0,size_of_img_buf_pool*size_of_img_buf*size_of_img_buf);
    else
    {
        img_buf_temp_pool =new BYTE[size_of_img_buf_pool*size_of_img_buf*size_of_img_buf];
        memset(img_buf_temp_pool,0,size_of_img_buf_pool*size_of_img_buf*size_of_img_buf);
    }

}

void   SingleImage:: List2Buf()
{
	list<SplitRect> ::iterator it =list_img_buf.begin();
	BYTE* img_buf = NULL;
    img_buf= img_buf_pool;
	for(it ; it!=list_img_buf.end() ;it++ )
	{
		memcpy(img_buf,it->getData(),size_of_img_buf*size_of_img_buf);
		img_buf += size_of_img_buf*size_of_img_buf;
	}
}

void SingleImage::SetSingImageStatu( bool is_busy )
{
    m_is_busy =is_busy;
    if (!m_is_busy)
        this->InitBuf();
}

bool SingleImage::GetSingImageStatu()
{
    return m_is_busy;
}

std::list<SplitRect>* SingleImage::Buf2List()
{
    list_img_buf.clear();
    BYTE* img_buf =img_buf_pool;
    int temp_offset =0;
    for (int i=0;i<num_of_img ;i++)
    {
        SplitRect split_rect ;
        split_rect.setData(img_buf);
        list_img_buf.push_back(split_rect);
        img_buf += size_of_img_buf*size_of_img_buf;
    }
    return &list_img_buf;
}

std::list<SplitRect>* SingleImage::GetSplitRect()
{
    return &list_img_buf;
}

void SingleImage::GetImageBufSum( int num,BYTE ** v_buf )
{
	int  sqrt_num =sqrt(num*1.0);
	int col_offset =0;
	int row_offset =0;
	BYTE * buf =img_buf_pool;
	BYTE * buf_temp =img_buf_temp_pool;
	for (int row =0; row <sqrt_num ;row++)
	{
		col_offset=0;
		row_offset=row*size_of_img_buf*size_of_img_buf*sqrt_num;
		for (int sin_row =0 ;sin_row <size_of_img_buf ;sin_row++)
		{
			for (int col =0 ;col <sqrt_num ;col++)
			{

				buf =(img_buf_pool+col_offset+row_offset)+col*size_of_img_buf*size_of_img_buf;
				memcpy(buf_temp,buf,size_of_img_buf);
				buf_temp+=size_of_img_buf;

			}
			col_offset+=size_of_img_buf;
		}
	}

	memcpy(*v_buf,img_buf_temp_pool,num*size_of_img_buf*size_of_img_buf);
}

/**************************************************************************************************************************************/
ImagePool * ImagePool::_instance = NULL;
ImagePool::ImagePool( int v_img_size,int v_pool_size ,int num_img )
{
    for (int i=0 ; i<num_img ;i++)
    {
        SingleImage * s_img =new SingleImage(v_img_size,v_pool_size);
        list_single_img.push_back(s_img);
    }
    m_mutex =new wxMutex();
}

ImagePool::~ImagePool()
{
    list_single_img.clear();
    if (m_mutex)
        delete m_mutex;
}

ImagePool*ImagePool :: Instance(int v_img_size,int v_pool_size ,int num_img)
{
    if(_instance==0){
        _instance = new ImagePool(v_img_size,v_pool_size ,num_img);
    }

    return _instance;
}

ImagePool * ImagePool::GetInstance()
{
    return _instance;
}

SingleImage *ImagePool::GetSingleImage( int v_num )
{
    list<SingleImage*> ::iterator it =list_single_img.begin();
    advance(it,v_num);
    return *it;
}

SingleImage* ImagePool::GetFreeSingleImage()
{
    m_mutex->Lock();
    list<SingleImage*> ::iterator it =list_single_img.begin();
    for (it ;it != list_single_img.end() ;it++)
    {
        if (!(*it)->GetSingImageStatu())
        {
            (*it)->SetSingImageStatu(true);
            m_mutex->Unlock();
            return (*it);
        }
    }
    m_mutex->Unlock();
    return NULL;
}

void ImagePool::ResetPool()
{
    list<SingleImage*> ::iterator it =list_single_img.begin();
    for (it ;it != list_single_img.end() ;it++)
    {
        (*it)->InitBuf();
    }
}
