#include "ControlPanel.h"

BEGIN_EVENT_TABLE(ControlPanel, wxPanel)

END_EVENT_TABLE()

ControlPanel::ControlPanel(wxWindow* parent)
	:wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE)
{
	m_frame = parent;

	wxColor btn_color = wxColor(125, 255, 125);
	wxFont font(20, wxSWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("微软雅黑"));

	wxBoxSizer *global_sizer = new wxBoxSizer(wxVERTICAL);

	m_btn_connect = new wxButton(this, ID_BTN_CONNECT, _("连接PLC"), wxDefaultPosition, wxSize(300, m_frame->GetSize().y / 10));
	m_btn_connect->SetFont(font);
	m_btn_connect_ai = new wxButton(this, ID_BTN_CONNECT_AI, _("连接AI"), wxDefaultPosition, wxSize(300, m_frame->GetSize().y / 10));
	m_btn_connect_ai->SetFont(font);
	m_btn_detection = new wxButton(this, ID_BTN_DETECTION, _("检测"), wxDefaultPosition, wxSize(300, m_frame->GetSize().y / 10));
	m_btn_detection->SetFont(font);
	m_btn_self_motion = new wxButton(this, ID_BTN_SELF_MOTION, _("自动检测"), wxDefaultPosition, wxSize(300, m_frame->GetSize().y / 10));
	m_btn_self_motion->SetFont(font);

	m_btn_connect->SetBackgroundColour(btn_color);
	
 	m_btn_detection->Enable(false);
	if (PLAN_ONE)
	{
		m_btn_self_motion->Enable(false);
	}
	else
	{
		m_btn_connect->Enable(false);
		m_btn_connect->SetBackgroundColour(wxColor(204, 204, 204));
		m_btn_self_motion->SetBackgroundColour(wxColor(125, 255, 125));
	}
 
	m_btn_connect_ai->SetBackgroundColour(btn_color);

	global_sizer->Add(m_btn_connect, 0, wxALL | wxEXPAND, 10);
	global_sizer->Add(m_btn_connect_ai, 0, wxALL | wxEXPAND, 10);
	global_sizer->Add(m_btn_detection, 0, wxALL | wxEXPAND, 10);
	global_sizer->Add(m_btn_self_motion, 0, wxALL | wxEXPAND, 10);

	this->SetSizer(global_sizer);
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::SetBtnStatus(bool flag)
{
	if (flag)
	{
		m_btn_connect->Enable(true);
		m_btn_connect->SetLabel("断开");
		m_btn_connect->SetBackgroundColour(wxColor(255, 0, 0));

		m_btn_detection->Enable(false);
//		m_btn_detection->SetBackgroundColour(wxColor(125, 255, 125));

		m_btn_self_motion->Enable(true);
		m_btn_self_motion->SetLabel("自动检测");
		m_btn_self_motion->SetBackgroundColour(wxColor(125, 255, 125));//绿色
	}
	else
	{
		m_btn_connect->Enable(false);
		m_btn_connect->SetBackgroundColour(wxColor(204, 204, 204));//灰色

		m_btn_detection->Enable(false);
		m_btn_detection->SetBackgroundColour(wxColor(204, 204, 204));

		m_btn_self_motion->Enable(false);
		m_btn_self_motion->SetBackgroundColour(wxColor(204, 204, 204));
	}
}

void ControlPanel::SetConnectBtnStatus(bool flag)
{
	if (flag)
	{
		m_btn_connect->Enable(true);
		m_btn_connect->SetLabel("连接PLC");
		m_btn_connect->SetBackgroundColour(wxColor(125, 255, 125));

		m_btn_detection->Enable(false);
		m_btn_detection->SetBackgroundColour(wxColor(204, 204, 204));

		m_btn_self_motion->Enable(false);
		m_btn_self_motion->SetBackgroundColour(wxColor(204, 204, 204));
	}
	else
	{
		m_btn_connect->Enable(false);
		m_btn_connect->SetBackgroundColour(wxColor(204, 204, 204));
	}
}

void ControlPanel::SetSelfMotionBtnStatus(bool flag)
{
	if (flag)
	{
		m_btn_connect->Enable(false);
		
		m_btn_detection->Enable(false);
//		m_btn_detection->SetBackgroundColour(wxColor(204, 204, 204));

		m_btn_self_motion->Enable(true);
		m_btn_self_motion->SetLabel("停止");
		m_btn_self_motion->SetBackgroundColour(wxColor(255, 0, 0));//红色
	}
	else
	{
		if (PLAN_ONE)
		{
			m_btn_connect->Enable(true);
			m_btn_connect->SetBackgroundColour(wxColor(255, 0, 0));
		}

		m_btn_self_motion->Enable(true);
		m_btn_self_motion->SetLabel("自动检测");
		m_btn_self_motion->SetBackgroundColour(wxColor(125, 255, 125));//绿色
	}
}

void ControlPanel::SetAIBtnStatus(int flag)
{
	switch (flag)
	{
	case 0:
		m_btn_connect_ai->SetLabel("连接AI");
		m_btn_connect_ai->SetBackgroundColour(wxColor(125, 255, 125));
		break;
	case 1:
		m_btn_connect_ai->SetLabel("AI已连接");
		m_btn_connect_ai->SetBackgroundColour(*wxYELLOW);
		break;
	default:
		m_btn_connect_ai->SetLabel("AI连接失败");
		m_btn_connect_ai->SetBackgroundColour(*wxRED);
		break;
	}
}

