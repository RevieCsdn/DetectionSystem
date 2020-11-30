#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/dcbuffer.h>
#include <wx/menu.h>
#include <list>
#include <iostream>
#include <string>
#include <wx/dir.h>
#include <map>
#include "VerifyDlg.h"
#include "../MyLog.h"

using namespace std;

class MyListCtrl : public wxListCtrl
{
	DECLARE_EVENT_TABLE()
public:
	MyListCtrl(wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxLC_REPORT | wxLC_EDIT_LABELS);
	virtual ~MyListCtrl() {};


private:
	wxWindow	*m_frame;
};

class ListPanel :
	public wxPanel
{
	DECLARE_EVENT_TABLE()

public:
	ListPanel(wxWindow *parent);
	virtual ~ListPanel();

	void AddData(wxString image_name, wxString image_result, wxString lizi_result, wxString image_NGnum, wxString image_path, wxString image_time);
	void OnMouseLeftDouble(wxListEvent &event);
	void OnMouseRightClick(wxListEvent &event);
	void OpenPhotoFile(wxCommandEvent &);
	void SetMapRedRect(map<string, list<wxRect>> map_red_rect);
	void ModifyLiziResults(wxString file_name, bool result);

	void GetOkNgRadioData();
	void ClearListData();
	inline void SetAIFlag(bool flag)
	{
		m_AI_flag = flag;
	};
	inline int GetOKnum()
	{
		return m_OK_num;
	};
	inline int GetNGnum()
	{
		return m_NG_num;
	};
	inline double GetRadionum()
	{
		return m_radio_num;
	};
	inline wxString GetSavePathImg()
	{
		return m_save_path_img;
	};
	inline void SetSavePathImg(wxString path)
	{
		m_save_path_img = path;
	};
	inline void SetRowIndex(int row)
	{
		m_listRowIndex = row;
	};
private:
	enum
	{
		ID_LISTCTRL,
		ID_OPEN_FILE,
	};

private:
	wxWindow	*m_frame;
	MyListCtrl	*m_mylistctrl;
	VerifyDlg	*m_verify;
	wxListItem	m_itemid;
	map<string, list<wxRect>> m_map_red_rect;

	bool		m_AI_flag;

	int			m_OK_num;
	int			m_NG_num;
	double		m_radio_num;

	wxString	m_save_path_img;
	int m_listRowIndex;

};

