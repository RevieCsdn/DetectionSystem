#pragma once
#include <wx/wx.h>
#include "../Util/PreDefine.h"

class ControlPanel :
	public wxPanel
{
	DECLARE_EVENT_TABLE()
public:
	ControlPanel(wxWindow* parent);
	virtual ~ControlPanel();

	void SetBtnStatus(bool flag);
	void SetConnectBtnStatus(bool flag);
	void SetSelfMotionBtnStatus(bool flag);
	void SetAIBtnStatus(int flag);

public:
	enum
	{
		ID_BTN_CONNECT,
		ID_BTN_CONNECT_AI,
		ID_BTN_DETECTION,
		ID_BTN_SELF_MOTION,
	};
private:
	wxWindow	*m_frame;

	wxButton	*m_btn_connect;
	wxButton	*m_btn_connect_ai;
	wxButton	*m_btn_detection;
	wxButton	*m_btn_self_motion;
};

