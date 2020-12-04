#pragma once
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "../Util/PreDefine.h"
#include <list>

using namespace std;

#define NOTESTRECT 3

typedef struct Imageproperties
{
	string	image_name;
	vector<cv::Mat> image_mark;
// 	int		image_start_x;
// 	int		image_start_y;
	vector<cv::Point> image_point;
	cv::Mat Model_image;
}Image_Properties;

typedef struct RectPoint
{
	cv::Point LeftUpPoint;
	cv::Point RightDownPoint;

}RectPoint_t;
typedef struct NoTestImage
{
	string ImageName;
	vector<RectPoint_t> RectPointVec;
	//vector<cv::Mat> NoTestRect;
	//vector<cv::Point> LeftUpPoint;
	//vector<cv::Point> RightDownPoint;
	//cv::Mat ModelImage;

}NoTestImage_t;

class AffirmDialog :public wxDialog
{
	DECLARE_EVENT_TABLE()
public:
	AffirmDialog(wxWindow *frame, int x, int y);
	virtual ~AffirmDialog();

	void CreateGuiControls();
private:
	enum
	{
		ID_BTN_OK,
		ID_BTN_NO,
	};
private:
	wxPanel		*m_panel;
	wxButton	*m_btn_yes;
	wxButton	*m_btn_no;
private:
	void OnBtnYes(wxCommandEvent &event);
	void OnBtnNo(wxCommandEvent &event);
};

class Refreshwindow :
	public wxScrolledWindow
{
	DECLARE_DYNAMIC_CLASS(Refreshwindow)
	DECLARE_EVENT_TABLE()
protected:
	Refreshwindow() {};
public:
	Refreshwindow(wxWindow* parent, int MyScrolled_Width, int MyScrolled_Height, wxWindowID id = -1,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~Refreshwindow();

	void OnPaint(wxPaintEvent&);
	void SetBitmapBuf(cv::Mat &image_img);

	void DrawResult(vector<cv::Rect> result, cv::Mat image_data);
	void OnDrawAffirmRect(wxPen pen, wxBrush brush);
	
	//坐标转换函数
	double CoordinateTransformation_W(double x, double screen_x);
	double CoordinateTransformation_H(double y, double screen_y);

	inline int GetWindowWidth()
	{
		return m_MyScrolled_Width;
	};
	inline int GetWindowHeight()
	{
		return m_MyScrolled_Height;
	};
	inline void SetMat_Width_Height(int width, int height)
	{
		m_matWidth = width;
		m_matHeight = height;
	};
	inline void SetZoom(int zoomX, int zoomY) 
	{
		m_zoomX = zoomX;
		m_zoomY = zoomY;
	};
	inline list<wxRect> GetwxRectList()
	{
		return m_vector_wx_screenshot_rect;
	};
	inline cv::Mat GetPaintFinImage()
	{
		return m_paint_finish_image;
	};
	inline void SetIsDrawRect(bool flag)
	{
		m_is_draw_rect = flag;
	};
	inline void SetImagePath(string str)
	{
		m_image_path = str.c_str();
	};
	inline Image_Properties GetImageModel()
	{
		return m_save_image_properties_model;
	};
	inline NoTestImage_t GetNoTestImage()
	{
		return m_noTestImage;
	};

	enum
	{
		ID_REFRESH_ADD_MODEL = 300,
		ID_NOTESTRECT_MODEL,
	};
	inline void SetDrawPenState(int val)
	{
		m_draw_pen_state = val;
	};
private:
	wxWindow		*m_frame;
	wxImage			m_image_buffer;
	wxImage			m_init_image_buffer;
	wxBitmap		m_bitmap_buffer;
	cv::Mat			m_temp_image;
	cv::Mat			m_paint_finish_image;

	int				m_MyScrolled_Width;
	int				m_MyScrolled_Height;
	int				m_zoomX;
	int				m_zoomY;
	int				m_matWidth;
	int				m_matHeight;

	wxMutex			*m_mutex;
	vector<cv::Rect> m_vector_draw_rect;
	list<wxRect>	m_vector_wx_screenshot_rect;

	bool			m_is_draw_rect;
	int				m_iStartX;
	int				m_iStartY;
	int				m_iEndX;
	int				m_iEndY;
	int				m_iSquare_X;
	int				m_iSquare_Y;
	AffirmDialog	*m_affirmdialog;
	wxRect			m_drawrectdata;
	string			m_image_path;
	Image_Properties m_save_image_properties_model;
	NoTestImage_t m_noTestImage;

	int				m_draw_pen_state;	//1是随便画 2是正方形
private:
	void OnMouseLeftDown(wxMouseEvent &event);
	void OnMouseLeftUp(wxMouseEvent &event);
	void OnMouseMove(wxMouseEvent &event);
	void OnMouseRightUp(wxMouseEvent &event);
};

