#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <list>
#include <iostream>
#include <wx/timer.h>
#include <wx/display.h>

using namespace std;

//class PopUpDlg :public wxFrame
//{
//	DECLARE_EVENT_TABLE()
//public:
//	PopUpDlg(wxWindow* parent);
//	~PopUpDlg();
//
//private:
//
//};

class AlignFailFrm : public wxFrame
{
private:
	DECLARE_EVENT_TABLE();

	wxWindow    * parent;
	wxTimer     * m_timer;
	int         m_stay_time;

	wxStaticText    * m_msg;

public:
	AlignFailFrm(wxWindow *parent);
	~AlignFailFrm();

	void        OnTimerHide(wxTimerEvent &event);
	void        SetStayTime(int m_sec);
	void        OnShowFail();

};