#pragma once
#include <wx/wx.h>
#include <iostream>
#include <list>
#include <wx/dcbuffer.h>
#include <wx/statline.h>
#include <Windows.h>
#include <shellapi.h>
#include <wx/pen.h>
#include "UIEvent.h"
#include "../MyLog.h"

using namespace std;

#undef VERIFYDLG_STYLE
#define VERIFYDLG_STYLE  wxCAPTION |wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX| wxRESIZE_BORDER | wxMINIMIZE_BOX// wxSTAY_ON_TOP| 

struct s_verify
{
	wxImage m_img;
	wxRect	m_ng_rect;
	wxRect  m_rect;
	string  m_pos_code;
	double  m_len;
	string  m_line_name;
	int     m_err_num;
	int     m_type;
	double  m_pos_x;
	double  m_pos_y;
};

class  ResultPanel : public wxScrolledWindow
{
private:
	DECLARE_EVENT_TABLE();
	int virt_width;
	int virt_height;
	wxImage  image;
	wxBitmap image_to_draw;
	wxColor bg_color;

public:
	ResultPanel(wxWindow* parent, wxWindowID id = -1,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = wxBORDER);
	virtual ~ResultPanel() {};
	void SetResultSize(int w, int h);

private:
	void OnKeyDown(wxKeyEvent &);
	void OnPaint(wxPaintEvent&);

};

class ImageFrame : public wxFrame
{
private:
	DECLARE_EVENT_TABLE();

	wxWindow    * parent;
	wxBitmap    m_image;

public:
	ImageFrame(wxWindow *parent);
	~ImageFrame();

	void        OnPaint(wxPaintEvent& event);
	void        OnMouseDown(wxMouseEvent&);
	void        SetImage(wxImage& img, wxRect fa_rect);
};

class	TestResultView : public wxScrolledWindow
{
protected:
	wxBitmap    image_to_draw;
	wxBitmap    __buffer;
	wxImage     image;
	wxPoint     mouse_position;
	wxCursor    pre_cursor;
	int         move_type;
	double      zoom;
	wxPoint     begin_point;
	wxColor     bg_color;
	wxRect      fail_rst;
	double      ratio_x;
	double      ratio_y;
	double      pos_x;
	double      pos_y;
	wxWindow    *frame;
	double      damage_len;
	wxRect      m_ng_rect;
public:
	TestResultView() {}
	TestResultView(wxWindow* parent, wxWindowID id,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = wxBORDER_NONE);
	virtual ~TestResultView() {};

	
	void    OnPaint(wxPaintEvent&);
	bool    SetImage(wxImage& img, wxRect fa_rect = wxRect(0, 0, 1, 1), double len = 0, wxRect ng_rect = wxRect(0, 0, 1, 1));
	wxImage GetImage();
	void    DrawResults(wxRect fail_rects);
	void    SetPosition(double x, double y);
	void    OnMouseDown(wxMouseEvent &);
	void    SetFrame(wxWindow *f);
	void    OnMouseDClick(wxMouseEvent &);

private:
	DECLARE_DYNAMIC_CLASS(TestResultView)
	DECLARE_EVENT_TABLE()
};

class VerifyDlg :
	public wxDialog
{
	DECLARE_EVENT_TABLE()
public:
	VerifyDlg(wxWindow *parent);
	virtual ~VerifyDlg();

	void CreateGUIControls();
 	void SetTestResult(const list<s_verify>& fail_msg, int brdge_num);
 	void SetUseTime(wxString ins_time);
 	void SetMachineSn(wxString mac_sn);
 	void SetWorkSide(int v_side);
 	void SetInsTime(time_t t);
	void ShowNextErrMsg();
	void OnCheckboxClick(wxCommandEvent& event);
	void OnBtnNextClick(wxCommandEvent &);
	void OnBtnPrevClick(wxCommandEvent &);
	void OnBtnShowImage(wxCommandEvent &);
	void OnBtnOK(wxCommandEvent &);
	void OnBtnNG(wxCommandEvent &);
	void OnClose(wxCloseEvent &);

	inline void SetShowImagePath(wxString path)
	{
		m_show_map_path = path;
	};
private:
	wxWindow		*m_frame;
	wxBoxSizer		*m_global_sizer;
	wxPanel			*m_panel;
	wxStaticText	*m_label_result;
	wxTextCtrl      *m_aa_left;
	wxTextCtrl      *m_aa_right;
	wxStaticText    *m_machine_sn;
	wxStaticText    *m_insp_time;
	wxStaticText    *m_fail_sum;
	wxGridSizer     *m_line_rst_sizer;

	wxButton		*m_btn_prev;
	wxButton        *m_btn_next;
 	wxButton        *m_btn_map;
	wxButton        *m_btn_ok;
	wxButton        *m_btn_ng;
	ResultPanel     *m_rs_panel;
	//==========================================================
	int             m_bri_num;
	int             m_cur_index;
	list<s_verify>  m_fail_msg;
	int             m_work_side;
	wxString        m_board_sn;
	wxString         m_use_time;
	time_t          m_ins_time;
	int             m_count;
	wxString        m_ftp_add;
	wxString		m_err_code;
	bool			m_treated;
	int             m_ok_wait_time;
	DWORD           m_start_verity_time;
	DWORD           m_start_verity_page;
	bool            m_demo_model;
	int             m_result_id;
	int             m_small_black_num;
	int             m_thin_num;
	int             m_fat_num;
	bool            m_last_flag;
	list<int>       m_true_error_list;
	int             m_num_tradition;
	int             m_num4cur_page;
	wxString		m_show_map_path;

	static int      VERIFY_COLS;
	static int      VERIFY_ROWS;
	static int      VERIFY_IMAGE_SIZE;
public:
	enum
	{
		ID_BTN_MAP,
		ID_BTN_OK,
		ID_BTN_NG,
		ID_BTN_PREV,
		ID_BTN_NEXT,
	};
	wxArrayString svr_aoi_data;
};

