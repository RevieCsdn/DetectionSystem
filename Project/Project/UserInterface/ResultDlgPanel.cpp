#include "ResultDlgPanel.h"

BEGIN_EVENT_TABLE(ResultDlgPanel, wxPanel)

END_EVENT_TABLE()

ResultDlgPanel::ResultDlgPanel(wxWindow* parent, Profile* file)
	:wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE)
{
	m_frame = parent;

	m_path = "Ini.ini";
	m_proFile = file;
	//m_proFile = Profile::Instance();
	//m_proFile->loadIni(m_path);

	wxColour lab_color = wxColour(0, 255, 0);
	wxFont font(30, wxSWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Î¢ÈíÑÅºÚ"));
	wxFont font_edit(40, wxSWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Î¢ÈíÑÅºÚ"));

	wxBoxSizer *top_sizer = new wxBoxSizer(wxVERTICAL);

	m_lab_ok = new wxStaticText(this, -1, _("OK"), wxDefaultPosition, wxDefaultSize);
	m_lab_ok->SetFont(font);
	m_lab_ok->SetForegroundColour(lab_color);
	m_edit_ok = new wxTextCtrl(this, -1, "0", wxDefaultPosition, wxSize(200, m_frame->GetSize().y / 14), wxTE_CENTRE);
	m_edit_ok->SetFont(font_edit);
	m_edit_ok->SetEditable(false);
	m_edit_ok->SetBackgroundColour(*wxYELLOW);

	m_lab_ng = new wxStaticText(this, -1, _("NG"), wxDefaultPosition, wxDefaultSize);
	m_lab_ng->SetFont(font);
	lab_color = wxColour(255, 0, 0);
	m_lab_ng->SetForegroundColour(lab_color);
	m_edit_ng = new wxTextCtrl(this, -1, "0", wxDefaultPosition, wxSize(200, m_frame->GetSize().y / 14), wxTE_CENTRE);
	m_edit_ng->SetFont(font_edit);
	m_edit_ng->SetEditable(false);
	m_edit_ng->SetBackgroundColour(*wxYELLOW);

	m_lab_ratio = new wxStaticText(this, -1, _("Ratio"), wxDefaultPosition, wxDefaultSize);
	m_lab_ratio->SetFont(font);
	lab_color = wxColour(0, 0, 255);
	m_lab_ratio->SetForegroundColour(lab_color);
	m_edit_ratio = new wxTextCtrl(this, -1, "0", wxDefaultPosition, wxSize(200, m_frame->GetSize().y / 14), wxTE_CENTRE);
	m_edit_ratio->SetFont(font_edit);
	m_edit_ratio->SetEditable(false);
	m_edit_ratio->SetBackgroundColour(*wxYELLOW);

	top_sizer->Add(m_lab_ok, 0, wxALL | wxALIGN_CENTER, 10);
	top_sizer->Add(m_edit_ok, 0, wxALL | wxALIGN_CENTER, 10);
	top_sizer->Add(m_lab_ng, 0, wxALL | wxALIGN_CENTER, 10);
	top_sizer->Add(m_edit_ng, 0, wxALL | wxALIGN_CENTER, 8);
	top_sizer->Add(m_lab_ratio, 0, wxALL | wxALIGN_CENTER, 8);
	top_sizer->Add(m_edit_ratio, 0, wxALL | wxALIGN_CENTER, 5);

	this->SetSizer(top_sizer);
}


ResultDlgPanel::~ResultDlgPanel()
{


}

void ResultDlgPanel::SetEditOK(int num)
{
	m_proFile->SetOkNum(num);
	m_proFile->SaveIni(m_path);
	wxString str = "";
	str << num;
	m_edit_ok->SetLabel(str);
}

void ResultDlgPanel::SetEditNG(int num)
{
	m_proFile->SetNgNum(num);
	m_proFile->SaveIni(m_path);
	wxString str = "";
	str << num;
	m_edit_ng->SetLabel(str);
}

void ResultDlgPanel::SetEditRatio(float num)
{
	m_proFile->SetRatio(num);
	m_proFile->SaveIni(m_path);
	wxString str = "";
	float f_num = num * 100.0;
	str = wxString::Format("%.1f%%", f_num);
	m_edit_ratio->SetLabel(str);
}


