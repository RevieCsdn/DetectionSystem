#include "PopUpDlg.h"

//BEGIN_EVENT_TABLE(PopUpDlg, wxFrame)
//
//END_EVENT_TABLE()
//PopUpDlg::PopUpDlg(wxWindow* parent)
//	:wxFrame(parent, -1, _("警告"), wxPoint(780, 250), wxSize(380, 150), wxCAPTION)
//{
//	m_parent = parent;
//	wxStaticBox *l_pathBox = new wxStaticBox(this, -1, _(""), wxPoint(20, 10), wxSize(450, 80));
//	wxFont l_font(15, wxSWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("微软雅黑"));
//	wxStaticText* l_text = new wxStaticText(l_pathBox, -1, _("该目录下片子为空！"), wxPoint(70, 30));
//	l_text->SetFont(l_font);
//	l_text->SetForegroundColour(*wxRED);
//	
//	this->Center();
//}
//
//PopUpDlg::~PopUpDlg()
//{
//	
//
//}

BEGIN_EVENT_TABLE(AlignFailFrm, wxFrame)
	EVT_TIMER(-1, AlignFailFrm::OnTimerHide)
END_EVENT_TABLE()
AlignFailFrm::AlignFailFrm(wxWindow *m_parent)
	: wxFrame(m_parent, -1, "", wxDefaultPosition, wxSize(300, 100), wxSTAY_ON_TOP)
{
	m_timer = new wxTimer(this, -1);
	m_stay_time = 3000;

	m_msg = new wxStaticText(this, -1, _(""), wxPoint(15, 30));
	m_msg->SetForegroundColour(*wxYELLOW);
	m_msg->SetFont(wxFont(20, wxSWISS, wxNORMAL, wxNORMAL, false));

	parent = m_parent;

	this->SetBackgroundColour(*wxRED);
	wxDisplay Screen;
	wxRect ClientRect = Screen.GetClientArea();
	this->SetPosition(wxPoint(ClientRect.width-1100, ClientRect.height-600));
}

AlignFailFrm::~AlignFailFrm()
{
	if (m_timer != nullptr)
	{
		delete m_timer;
	}
	if (m_msg != nullptr)
	{
		delete m_msg;
	}
}
	

void AlignFailFrm::OnTimerHide(wxTimerEvent &event)
{
	this->Show(false);
}

void AlignFailFrm::SetStayTime(int m_sec)
{
	m_stay_time = m_sec;
}

void AlignFailFrm::OnShowFail()
{
	m_msg->SetLabel(_("该目录下片子为空!"));
	this->Show();
	if (m_stay_time > 0)
		m_timer->StartOnce(m_stay_time);
}

