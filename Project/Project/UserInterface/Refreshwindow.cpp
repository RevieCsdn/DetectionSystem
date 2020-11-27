#include "Refreshwindow.h"

BEGIN_EVENT_TABLE(AffirmDialog, wxDialog)
	EVT_BUTTON(ID_BTN_OK, AffirmDialog::OnBtnYes)
	EVT_BUTTON(ID_BTN_NO, AffirmDialog::OnBtnNo)
END_EVENT_TABLE()

AffirmDialog::AffirmDialog(wxWindow *frame, int x, int y)
	:wxDialog(frame, -1, "", wxPoint(x, y), wxSize(102, 32), wxSTAY_ON_TOP)
{
	this->CreateGuiControls();
}

AffirmDialog::~AffirmDialog()
{

}

void AffirmDialog::CreateGuiControls()
{
	m_panel = new wxPanel(this, -1, wxPoint(-1, -1), wxSize(100, 30));

	m_btn_no = new wxButton(m_panel, ID_BTN_NO, _("取消"), wxPoint(0, 0), wxSize(50, 30));
	m_btn_yes = new wxButton(m_panel, ID_BTN_OK, _("完成"), wxPoint(50, 0), wxSize(50, 30));
}

void AffirmDialog::OnBtnYes(wxCommandEvent &event)
{
	EndModal(1);
}

void AffirmDialog::OnBtnNo(wxCommandEvent &event)
{
	EndModal(0);
}

IMPLEMENT_DYNAMIC_CLASS(Refreshwindow, wxScrolledWindow)
BEGIN_EVENT_TABLE(Refreshwindow, wxScrolledWindow)
	EVT_PAINT(Refreshwindow::OnPaint)
	EVT_LEFT_DOWN(Refreshwindow::OnMouseLeftDown)
	EVT_LEFT_UP(Refreshwindow::OnMouseLeftUp)
	EVT_MOTION(Refreshwindow::OnMouseMove)
	EVT_RIGHT_UP(Refreshwindow::OnMouseRightUp)
END_EVENT_TABLE()

Refreshwindow::Refreshwindow(wxWindow* parent, int MyScrolled_Width, int MyScrolled_Height, wxWindowID id /* = -1 */, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */)
	:wxScrolledWindow(parent, id, pos, size, wxSUNKEN_BORDER)
{
	this->SetScrollRate(1, 1);
	this->SetFocus();

	wxInitAllImageHandlers();
	m_frame = parent;
	m_vector_draw_rect.clear();
	m_vector_wx_screenshot_rect.clear();

	m_MyScrolled_Width = MyScrolled_Width;
	m_MyScrolled_Height = MyScrolled_Height;
	m_zoomX = m_MyScrolled_Width;
	m_zoomY = m_MyScrolled_Height;

	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	m_mutex = new wxMutex();

	m_image_buffer = wxImage("./Icon/watch.jpg", wxBITMAP_TYPE_JPEG);
	m_init_image_buffer = m_image_buffer.Copy();
	m_image_buffer.Rescale(m_zoomX, m_zoomY);
	m_matWidth = m_image_buffer.GetWidth();
	m_matHeight = m_image_buffer.GetHeight();

	m_bitmap_buffer = wxBitmap(m_image_buffer);
	wxBufferedDC dc(NULL, m_bitmap_buffer, wxBUFFER_VIRTUAL_AREA);
	dc.DrawBitmap(m_image_buffer, wxPoint(0, 0));
	this->SetBackgroundColour(*wxWHITE);

	m_is_draw_rect = false;
	m_iStartX = 0;
	m_iStartY = 0;
	m_iEndX = 0;
	m_iEndY = 0;
	m_iSquare_X = 0;
	m_iSquare_Y = 0;
	m_affirmdialog = NULL;
	m_save_image_properties_model = {};

	m_draw_pen_state = 0;
}

Refreshwindow::~Refreshwindow()
{
	m_vector_draw_rect.clear();
	m_vector_wx_screenshot_rect.clear();

	if (m_mutex)
		delete m_mutex;

}

void Refreshwindow::OnPaint(wxPaintEvent &)
{
	wxMutexLocker lock(*m_mutex);

	if (!m_bitmap_buffer.IsOk())
	{
		wxLogError("refresh window(OnPaint): m_buffer not ok");
		return;
	}

	while (GLOBAL_LOCKED)
	{
		Sleep(5);
	}
	GLOBAL_LOCKED = true;
	wxAutoBufferedPaintDC dc(this);
	PrepareDC(dc);
	
	if (m_bitmap_buffer.IsOk())
	{
		dc.SetPen(*wxWHITE_PEN);
		dc.SetBrush(*wxWHITE_BRUSH);
		dc.DrawRectangle(0, 0, m_MyScrolled_Width, m_MyScrolled_Height);
		dc.DrawBitmap(m_bitmap_buffer, 0, m_MyScrolled_Height / 2 - m_bitmap_buffer.GetHeight() / 2 - 10);
	}
	GLOBAL_LOCKED = false;
}

void Refreshwindow::SetBitmapBuf(cv::Mat &image_img)
{
	m_temp_image = image_img;
// 	cv::Mat rgbMat;
// 	cv::cvtColor(m_temp_image, rgbMat, cv::COLOR_GRAY2RGB);

// 	m_image_buffer.Clear();
// 	m_init_image_buffer.Clear();
// //	wxImage wx_image_img;
// // 	m_image_buffer.SetData(rgbMat.data, m_matWidth, m_matHeight, true);
// // 	m_init_image_buffer.SetData(rgbMat.data, m_matWidth, m_matHeight, true);					//存原图
// 	BYTE * temp = new BYTE[m_temp_image.cols * m_temp_image.rows];
// 	memcpy(temp, m_temp_image.data, m_temp_image.cols * m_temp_image.rows);
// 	m_image_buffer = wxImage(m_temp_image.cols, m_temp_image.rows);
// 	m_image_buffer.SetData(temp);
// 
// 	m_init_image_buffer = wxImage(m_temp_image.cols, m_temp_image.rows);
// 	BYTE * temp2 = new BYTE[m_temp_image.cols * m_temp_image.rows];
// 	memcpy(temp2, m_temp_image.data, m_temp_image.cols * m_temp_image.rows);
// 	m_init_image_buffer.SetData(temp2);

	cv::Mat cv_temp_image;
	cv::resize(image_img, cv_temp_image, cv::Size(m_zoomX, m_zoomY), 0.0, 0.0, cv::INTER_AREA);	//缩放

	cv::Mat CvrgbMat;
	cv::cvtColor(cv_temp_image, CvrgbMat, cv::COLOR_GRAY2RGB);

	wxImage wx_show_image_img;
	wx_show_image_img.SetData(CvrgbMat.data, m_zoomX, m_zoomY, true);

	m_image_buffer = wx_show_image_img.Copy();

	m_bitmap_buffer = wxBitmap(wx_show_image_img);
}
//画结果
void Refreshwindow::DrawResult(vector<cv::Rect> result, cv::Mat image_data)
{
	m_vector_draw_rect = result;

	cv::cvtColor(image_data, image_data, cv::COLOR_GRAY2BGR);
	cv::Rect allRect(0, 0, image_data.cols, image_data.rows);

	m_vector_wx_screenshot_rect.clear();
	for (vector<cv::Rect>::iterator it = m_vector_draw_rect.begin(); it != m_vector_draw_rect.end(); it++)
	{
		it->x = it->x - it->width/2- 112;
		it->y = it->y - it->height / 2 - 112;
		it->width = 224;
		it->height = 224;
		//x截图边界判断
		if (it->x < 0)
		{
			it->x = 0;
		}
		else if (it->x + it->width > image_data.cols-1)
		{
			it->x = image_data.cols - it->width;
		}
		//y截图边界判断
		if (it->y < 0)
		{
			it->y = 0;
		}
		else if(it->y + it->height > image_data.rows-1)
		{
			it->y = image_data.rows - it->height;
		}

		wxRect temp_rect = wxRect(it->x, it->y, it->width, it->height);
		m_vector_wx_screenshot_rect.push_back(temp_rect);
	}
	m_paint_finish_image = image_data;

	cv::Mat cv_temp_image;
	cv::resize(image_data, cv_temp_image, cv::Size(m_zoomX, m_zoomY), 0.0, 0.0, cv::INTER_AREA);	//缩放
	
	cv::Mat CvrgbMat;
	cv::cvtColor(cv_temp_image, CvrgbMat, cv::COLOR_BGR2RGB);

	wxImage m_image_buffer_temp;
	m_image_buffer_temp.SetData(CvrgbMat.data, m_zoomX, m_zoomY,true);

	m_bitmap_buffer = wxBitmap(m_image_buffer_temp);

	this->Refresh();
}

void Refreshwindow::OnMouseLeftDown(wxMouseEvent &event)
{
	if (m_is_draw_rect)
	{
		int ViewStartX = 0;
		int	ViewStarty = 0;
		this->GetViewStart(&ViewStartX, &ViewStarty);

		if (event.GetPosition().y < m_MyScrolled_Height / 2 - m_bitmap_buffer.GetHeight() / 2 - 10 || event.GetPosition().y > m_MyScrolled_Height / 2 - m_bitmap_buffer.GetHeight() / 2 - 10 + m_bitmap_buffer.GetHeight())
		{
			return;
		}

		m_iStartX = event.GetPosition().x/* + ViewStartX*/;
		m_iStartY = event.GetPosition().y - m_MyScrolled_Height / 2 + m_bitmap_buffer.GetHeight() / 2 + 10/* + ViewStarty*/;
	}
}

void Refreshwindow::OnMouseLeftUp(wxMouseEvent &event)
{
	if (m_is_draw_rect)
	{
		if (1 == m_draw_pen_state)
		{
			int ViewStartX = 0;
			int ViewStartY = 0;
			this->GetViewStart(&ViewStartX, &ViewStartY);
			if (m_iEndX >= m_iStartX && m_iEndY >= m_iStartY)
			{
				m_iEndX = m_iEndX;
				m_iEndY = m_iEndY;
			}
			else if (m_iEndX > m_iStartX && m_iEndY <= m_iStartY)
			{
				int sx, sy, ex, ey;
				sx = m_iStartX;
				sy = m_iStartY;
				ex = m_iEndX;
				ey = m_iEndY;

				m_iEndY = sy;
				m_iStartY = ey;
				m_iEndX = ex;
			}
			else if (m_iEndX <= m_iStartX && m_iEndY > m_iStartY)
			{
				int sx, sy, ex, ey;
				sx = m_iStartX;
				sy = m_iStartY;
				ex = m_iEndX;
				ey = m_iEndY;

				m_iEndX = sx;
				m_iStartX = ex;
				m_iEndY = ey;
			}
			else
			{
				int sx, sy, ex, ey;
				sx = m_iStartX;
				sy = m_iStartY;
				ex = m_iEndX;
				ey = m_iEndY;

				m_iEndX = sx;
				m_iEndY = sy;
				m_iStartX = ex;
				m_iStartY = ey;
			}

			int temp_affirmdialog_x = m_iEndX + 11;
			int temp_affirmdialog_y = m_MyScrolled_Height + m_iEndY + 77 + m_MyScrolled_Height / 2 - m_bitmap_buffer.GetHeight() / 2 - 10;
			if (temp_affirmdialog_x + 102 > m_MyScrolled_Width)
			{
				temp_affirmdialog_x = m_MyScrolled_Width - 102;
			}

			if (event.GetPosition().y < m_MyScrolled_Height / 2 - m_bitmap_buffer.GetHeight() / 2 - 10 || event.GetPosition().y > m_MyScrolled_Height / 2 - m_bitmap_buffer.GetHeight() / 2 - 10 + m_bitmap_buffer.GetHeight())
			{
				return;
			}

			m_affirmdialog = new AffirmDialog(m_frame, temp_affirmdialog_x, temp_affirmdialog_y);
			if (0 == m_affirmdialog->ShowModal())
			{
				m_bitmap_buffer = wxBitmap(m_image_buffer);
			}
			else
			{
				this->OnDrawAffirmRect(wxPen(wxT("BLUE"), 1), *wxGREEN_BRUSH);

				m_iStartX = this->CoordinateTransformation_W(m_iStartX, m_zoomX);
				m_iStartY = this->CoordinateTransformation_H(m_iStartY, m_zoomY);
				m_iEndX = this->CoordinateTransformation_W(m_iEndX, m_zoomX);
				m_iEndY = this->CoordinateTransformation_H(m_iEndY, m_zoomY);

				cv::Mat temp_mark_pic = m_temp_image(cv::Rect(m_iStartX, m_iStartY, m_iEndX - m_iStartX, m_iEndY - m_iStartY));
				cv::Mat temp_model_pic = m_temp_image;

				wxString temp_image_type = "";
				temp_image_type = strstr(m_image_path.c_str(), "]");
				if (NULL != temp_image_type)
				{
					temp_image_type = temp_image_type.substr(1, 4);
				}
				else
				{
					wxMessageBox("该图不能作为模板", "Warning");
					return;
				}

				m_save_image_properties_model.image_name = temp_image_type;
				m_save_image_properties_model.image_mark.clear();
				m_save_image_properties_model.image_mark.push_back(temp_mark_pic);
				m_save_image_properties_model.image_point.clear();
				m_save_image_properties_model.image_point.push_back(cv::Point(m_iStartX, m_iStartY));
				m_save_image_properties_model.Model_image = temp_model_pic;

				wxQueueEvent(m_frame->GetEventHandler(),
					new wxCommandEvent(wxEVT_BUTTON, ID_REFRESH_ADD_MODEL));

				// 			cv::imwrite("mark.jpg", m_save_image_properties_model.image_mark);
				// 			cv::imwrite("image.jpg", m_save_image_properties_model.Model_image);
			}
		}
		else if(2 == m_draw_pen_state)
		{
			int ViewStartX = 0;
			int	ViewStarty = 0;
			this->GetViewStart(&ViewStartX, &ViewStarty);

			if (event.GetPosition().y < m_MyScrolled_Height / 2 - m_bitmap_buffer.GetHeight() / 2 - 10 || event.GetPosition().y > m_MyScrolled_Height / 2 - m_bitmap_buffer.GetHeight() / 2 - 10 + m_bitmap_buffer.GetHeight())
			{
				return;
			}

			m_iSquare_X = event.GetPosition().x - 8/* + ViewStartX*/;
			m_iSquare_Y = event.GetPosition().y - m_MyScrolled_Height / 2 + m_bitmap_buffer.GetHeight() / 2 + 10 - 8/* + ViewStarty*/;

			wxMemoryDC dc;
			dc.Clear();

			m_bitmap_buffer = wxBitmap(m_image_buffer);
			dc.SelectObject(m_bitmap_buffer);

			dc.SetPen(wxPen(wxT("RED"), 1));
			dc.SetBrush(*wxTRANSPARENT_BRUSH);

			dc.DrawRectangle(wxRect(m_iSquare_X, m_iSquare_Y, 16, 16));
			m_drawrectdata = wxRect(m_iSquare_X, m_iSquare_Y, 16, 16);

			dc.SelectObject(wxNullBitmap);
		}
	}
	this->Refresh(false);
}

void Refreshwindow::OnMouseMove(wxMouseEvent &event)
{
	if (event.Dragging() && m_is_draw_rect)//Dragging 拖动事件
	{
		if (1 == m_draw_pen_state)
		{
			wxMemoryDC dc;
			dc.Clear();

			m_bitmap_buffer = wxBitmap(m_image_buffer);
			dc.SelectObject(m_bitmap_buffer);

			dc.SetPen(wxPen(wxT("RED"), 1));
			dc.SetBrush(*wxTRANSPARENT_BRUSH);

			int ViewStartX;
			int ViewStarty;
			this->GetViewStart(&ViewStartX, &ViewStarty);

			m_iEndX = event.GetPosition().x;
			m_iEndY = event.GetPosition().y - m_MyScrolled_Height / 2 + m_bitmap_buffer.GetHeight() / 2 + 10;

			int rect_X = 0;
			int rect_Y = 0;
			int rect_W = 0;
			int rect_H = 0;

			if (m_iEndX >= m_iStartX && m_iEndY >= m_iStartY)
			{
				rect_X = m_iStartX;
				rect_Y = m_iStartY;
				rect_W = m_iEndX - m_iStartX;
				rect_H = m_iEndY - m_iStartY;
			}
			else if (m_iEndX > m_iStartX && m_iEndY <= m_iStartY)
			{
				rect_X = m_iStartX;
				rect_Y = m_iEndY;
				rect_W = m_iEndX - m_iStartX;
				rect_H = m_iStartY - m_iEndY;
			}
			else if (m_iEndX <= m_iStartX && m_iEndY > m_iStartY)
			{
				rect_X = m_iEndX;
				rect_Y = m_iStartY;
				rect_W = m_iStartX - m_iEndX;
				rect_H = m_iEndY - m_iStartY;
			}
			else
			{
				rect_X = m_iEndX;
				rect_Y = m_iEndY;
				rect_W = m_iStartX - m_iEndX;
				rect_H = m_iStartY - m_iEndY;
			}
			dc.DrawRectangle(wxRect(rect_X, rect_Y, rect_W, rect_H));
			m_drawrectdata = wxRect(rect_X, rect_Y, rect_W, rect_H);

			dc.SelectObject(wxNullBitmap);

			this->Refresh(false);
		}
		else if (2 == m_draw_pen_state)
		{
			int temp_iSquare_X = event.GetPosition().x - 8;
			int temp_iSquare_Y = event.GetPosition().y - m_MyScrolled_Height / 2 + m_bitmap_buffer.GetHeight() / 2 + 10 - 8;
			if (temp_iSquare_X >= m_iSquare_X && temp_iSquare_X <= m_iSquare_X + 16 && temp_iSquare_Y >= m_iSquare_Y && temp_iSquare_Y <= m_iSquare_Y + 16)
			{
				m_iSquare_X = temp_iSquare_X;
				m_iSquare_Y = temp_iSquare_Y;

				wxMemoryDC dc;
				dc.Clear();

				m_bitmap_buffer = wxBitmap(m_image_buffer);
				dc.SelectObject(m_bitmap_buffer);

				dc.SetPen(wxPen(wxT("RED"), 1));
				dc.SetBrush(*wxTRANSPARENT_BRUSH);

				dc.DrawRectangle(wxRect(temp_iSquare_X, temp_iSquare_Y, 16, 16));
				m_drawrectdata = wxRect(temp_iSquare_X, temp_iSquare_Y, 16, 16);

				dc.SelectObject(wxNullBitmap);

				this->Refresh(false);
			}
		}
	}
}

void Refreshwindow::OnMouseRightUp(wxMouseEvent &event)
{
	if (m_is_draw_rect)
	{
		if (2 == m_draw_pen_state)
		{
			int temp_iSquare_X = event.GetPosition().x;
			int temp_iSquare_Y = event.GetPosition().y - m_MyScrolled_Height / 2 + m_bitmap_buffer.GetHeight() / 2 + 10;
			if (temp_iSquare_X >= m_iSquare_X && temp_iSquare_X <= m_iSquare_X + 16 && temp_iSquare_Y >= m_iSquare_Y && temp_iSquare_Y <= m_iSquare_Y + 16)
			{
				int temp_affirmdialog_x = m_iSquare_X + 16 + 11;
				int temp_affirmdialog_y = m_MyScrolled_Height + m_iSquare_Y + 16 + 77 + m_MyScrolled_Height / 2 - m_bitmap_buffer.GetHeight() / 2 - 10;
				if (temp_affirmdialog_x + 102 > m_MyScrolled_Width)
				{
					temp_affirmdialog_x = m_MyScrolled_Width - 102;
				}

				if (event.GetPosition().y < m_MyScrolled_Height / 2 - m_bitmap_buffer.GetHeight() / 2 - 10 || event.GetPosition().y > m_MyScrolled_Height / 2 - m_bitmap_buffer.GetHeight() / 2 - 10 + m_bitmap_buffer.GetHeight())
				{
					return;
				}

				m_affirmdialog = new AffirmDialog(m_frame, temp_affirmdialog_x, temp_affirmdialog_y);
				if (0 == m_affirmdialog->ShowModal())
				{
					m_bitmap_buffer = wxBitmap(m_image_buffer);
				}
				else
				{
					this->OnDrawAffirmRect(wxPen(wxT("BLUE"), 1), *wxGREEN_BRUSH);

					int i_temp_Start_X = this->CoordinateTransformation_W(m_iSquare_X, m_zoomX);
					int i_temp_Start_Y = this->CoordinateTransformation_H(m_iSquare_Y, m_zoomY);
					int i_temp_End_X = this->CoordinateTransformation_W(m_iSquare_X + 16, m_zoomX);
					int i_temp_End_Y = this->CoordinateTransformation_H(m_iSquare_Y + 16, m_zoomY);

					cv::Mat temp_mark_pic = m_temp_image(cv::Rect(i_temp_Start_X, i_temp_Start_Y, i_temp_End_X - i_temp_Start_X, i_temp_End_Y - i_temp_Start_Y));
					cv::Mat temp_model_pic = m_temp_image;

					wxString temp_image_type = "";
					temp_image_type = strstr(m_image_path.c_str(), "]");
					if (NULL != temp_image_type)
					{
						temp_image_type = temp_image_type.substr(1, 4);
					}
					else
					{
						wxMessageBox("该图不能作为模板", "Warning");
						return;
					}

					m_save_image_properties_model.image_name = temp_image_type;
					m_save_image_properties_model.image_mark.clear();
					m_save_image_properties_model.image_mark.push_back(temp_mark_pic);
					cv::imwrite("save_mark.jpg", temp_mark_pic);
					m_save_image_properties_model.image_point.clear();
					m_save_image_properties_model.image_point.push_back(cv::Point(i_temp_Start_X, i_temp_Start_Y));
					m_save_image_properties_model.Model_image = temp_model_pic;

					wxQueueEvent(m_frame->GetEventHandler(),
						new wxCommandEvent(wxEVT_BUTTON, ID_REFRESH_ADD_MODEL));
				}

				this->Refresh(false);
			}
		}
	}
}

//确认后的矩形框
void Refreshwindow::OnDrawAffirmRect(wxPen pen, wxBrush brush)
{
	wxMemoryDC dc;
	dc.Clear();

	m_bitmap_buffer = wxBitmap(m_image_buffer);
	dc.SelectObject(m_bitmap_buffer);

	dc.SetPen(pen);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.DrawRectangle(m_drawrectdata);

	dc.SelectObject(wxNullBitmap);

	m_image_buffer = m_bitmap_buffer.ConvertToImage();
	m_image_buffer.Rescale(m_zoomX, m_zoomY);
	m_bitmap_buffer = wxBitmap(m_image_buffer);

	this->Refresh();
}

double Refreshwindow::CoordinateTransformation_W(double x, double screen_x)
{
	double RealWidth = m_matWidth;
	if (abs(RealWidth) < 1e-6)
	{
		return 0.0;
	}
	return  x / (double)screen_x * RealWidth;
}

double Refreshwindow::CoordinateTransformation_H(double y, double screen_y)
{
	double RealHeight = m_matHeight;
	if (abs(RealHeight) < 1e-6)
	{
		return 0.0;
	}
	return  y / (double)screen_y * RealHeight;
}