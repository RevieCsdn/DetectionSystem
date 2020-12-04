#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <list>
#include <iostream>
#include "./Refreshwindow.h"
#include "./DetectionSetDlg.h"
using namespace std;


//class ModelListCtrl : public wxListCtrl
//{
//	DECLARE_EVENT_TABLE()
//public:
//	ModelListCtrl(wxWindow* parent,
//		wxWindowID id,
//		const wxPoint& pos = wxDefaultPosition,
//		const wxSize& size = wxDefaultSize,
//		long style = wxLC_REPORT | wxLC_EDIT_LABELS);
//	virtual ~ModelListCtrl() {};
//
//private:
//	wxWindow	*m_frame;
//};

class NoTestPanel :
	public wxPanel
{
	DECLARE_EVENT_TABLE()

public:
	NoTestPanel(wxWindow *parent);
	virtual ~NoTestPanel();

	void CreateGui();
	void AddData(vector<NoTestImage_t>& noTestVec);
	void DeleteAllModel();
	inline vector<string> GetVecRecipe()
	{
		return m_recipeNameVec;
	};
private:
	enum
	{
		ID_LISTCTRL,
	};

private:
	wxWindow	*m_frame;
	ModelListCtrl	*m_modellistctrl;
	wxListItem	m_itemid;

	vector<string> m_recipeNameVec;
};

class NoTestDlg :public wxDialog
{
	DECLARE_EVENT_TABLE()
public:
	NoTestDlg(wxWindow* parent);
	~NoTestDlg();

	void DeleteAllModel();
	void OnBtnClear(wxCommandEvent &);
	void OnBtnSave(wxCommandEvent &);
	inline NoTestPanel* GetModelPanel()
	{
		return m_modelpanel;
	};
	//inline list<string> GetListRecipe()
	//{
	//	return m_modelpanel->GetListRecipe();
	//};

	enum 
	{
		ID_BTN_NOTESTCLEAR = 10010,
		ID_BIN_NOTESTSAVE,
	};
private:
	wxWindow	*m_parent;
	NoTestPanel	*m_modelpanel;
	wxButton	*m_btn_clear;
	wxButton	*m_btn_save;
};