#include "NoTestPanel.h"

//BEGIN_EVENT_TABLE(ModelListCtrl, wxListCtrl)
//
//END_EVENT_TABLE()
//
//ModelListCtrl::ModelListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxLC_REPORT | wxLC_SORT_ASCENDING */)
//	:wxListCtrl(parent, id, pos, size, style)
//{
//	m_frame = parent;
//}
//==========================================================================================================
BEGIN_EVENT_TABLE(NoTestPanel, wxPanel)
	
END_EVENT_TABLE()

NoTestPanel::NoTestPanel(wxWindow *parent)
	:wxPanel(parent, -1, wxDefaultPosition, wxSize(200, 250), wxBORDER_NONE)
{
	m_frame = parent;
	m_recipeNameVec.clear();
	CreateGui();
}

NoTestPanel::~NoTestPanel()
{
	m_recipeNameVec.clear();
}

void NoTestPanel::CreateGui()
{
	wxBoxSizer *global_sizer = new wxBoxSizer(wxVERTICAL);

	m_modellistctrl = new ModelListCtrl(this, ID_LISTCTRL, wxPoint(128, 134), wxSize(121, 97), wxLC_REPORT | wxLC_SORT_ASCENDING);
	wxFont font(14, wxSWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("微软雅黑"));

	wxArrayString cols_name;
	cols_name.Add(_("模板名"));
	cols_name.Add(_("Mark数量"));

	for (unsigned int i = 0; i < cols_name.size(); i++)
	{
		m_modellistctrl->InsertColumn(i, cols_name[i]);
	}

	m_modellistctrl->SetColumnWidth(0, 100);
	m_modellistctrl->SetColumnWidth(1, 100);
	m_modellistctrl->SetFont(font);

	global_sizer->Add(m_modellistctrl, 1, wxALL | wxEXPAND, 0);
	this->SetSizer(global_sizer);

}

void NoTestPanel::AddData(vector<NoTestImage_t>& noTestVec)
{
	vector<NoTestImage_t> l_noTestVec;
	l_noTestVec.assign(noTestVec.begin(), noTestVec.end());
	m_modellistctrl->DeleteAllItems();

	int l_index = 0;

	m_recipeNameVec.clear();
	for (NoTestImage_t noTest: l_noTestVec)
	{
		wxArrayString l_colsName;
		wxString l_num = "";
		l_num << noTest.RectPointVec.size();
		l_colsName.Add(noTest.ImageName);
		l_colsName.Add(l_num);

		m_recipeNameVec.push_back(noTest.ImageName);

		long l_item = m_modellistctrl->InsertItem(l_index, l_colsName[0], 2);
		m_modellistctrl->SetItemData(l_item, l_index);

		for (unsigned int i = 1; i < l_colsName.size(); i++)
		{
			m_modellistctrl->SetItem(l_item, i, l_colsName[i]);
		}

		l_index++;
	}

	m_modellistctrl->RefreshItem(l_index);
}

void NoTestPanel::DeleteAllModel()
{
	m_modellistctrl->DeleteAllItems();
}

BEGIN_EVENT_TABLE(NoTestDlg, wxDialog)
	EVT_BUTTON(ID_BTN_NOTESTCLEAR, NoTestDlg::OnBtnClear)
	EVT_BUTTON(ID_BIN_NOTESTSAVE, NoTestDlg::OnBtnSave)
END_EVENT_TABLE()

NoTestDlg::NoTestDlg(wxWindow* parent)
	:wxDialog(parent, -1, _("不检测模板"), wxPoint(850, 250), wxSize(800, 500), wxCAPTION)
{
	m_parent = parent;

	wxBoxSizer *global_sizer = new wxBoxSizer(wxVERTICAL);

	m_modelpanel = new NoTestPanel(this);
	m_btn_clear = new wxButton(this, ID_BTN_NOTESTCLEAR, _("清空配方"), wxDefaultPosition, wxSize(100, 30));
	m_btn_save = new wxButton(this, ID_BIN_NOTESTSAVE, _("保存配方"), wxDefaultPosition, wxSize(100, 30));

	wxBoxSizer *btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	btn_sizer->Add(m_btn_clear, 0, wxALL | wxEXPAND, 2);
	btn_sizer->Add(m_btn_save, 0, wxALL | wxEXPAND, 2);

	global_sizer->Add(m_modelpanel, 0, wxALL | wxEXPAND, 2);
	global_sizer->Add(btn_sizer, 0, wxALL | wxEXPAND, 2);

	this->SetSizer(global_sizer);
	this->Fit();
}

NoTestDlg::~NoTestDlg()
{
	if (m_modelpanel)
	{
		delete m_modelpanel;
		m_modelpanel = NULL;
	}
}

void NoTestDlg::OnBtnClear(wxCommandEvent &)
{
	wxQueueEvent(m_parent->GetEventHandler(),
		new wxCommandEvent(wxEVT_BUTTON, ID_BTN_NOTESTCLEAR));
}

void NoTestDlg::OnBtnSave(wxCommandEvent &)
{
	wxQueueEvent(m_parent->GetEventHandler(),
		new wxCommandEvent(wxEVT_BUTTON, ID_BIN_NOTESTSAVE));
}

void NoTestDlg::DeleteAllModel()
{
	m_modelpanel->DeleteAllModel();
}