#include "DetectionSetDlg.h"

BEGIN_EVENT_TABLE(DetectionSetDlg, wxDialog)
	EVT_BUTTON(ID_BTN_OK, DetectionSetDlg::OnBtnOK)
	EVT_BUTTON(ID_BTN_NO, DetectionSetDlg::OnBtnNO)
	EVT_BUTTON(ID_BTN_PATH, DetectionSetDlg::OnBtnPath)
	EVT_BUTTON(ID_BTN_PATH_TWO, DetectionSetDlg::OnBtnPathCsv)
	EVT_BUTTON(ID_BTN_PATH_SAVE_IMG, DetectionSetDlg::OnBtnPathSaveImg)
	EVT_BUTTON(ID_BTN_PATH_EXCEL, DetectionSetDlg::OnBtnPathExcel)
END_EVENT_TABLE()

DetectionSetDlg::DetectionSetDlg(wxWindow* parent, wxWindowID id /* = -1 */, const wxString& title /* = "自动检测设置" */, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxCAPTION */)
	:wxDialog(parent, id, title, pos, size, style), m_wxs_ip(""), m_i_port(0), m_wxs_path(""), m_wxs_path_csv(""), m_wxs_path_save_img(""), m_wxs_path_excel(""),m_i_single(0)
{
	m_frame = parent;
}	


DetectionSetDlg::~DetectionSetDlg()
{
}

void DetectionSetDlg::CreateGUIcontrols()
{
	wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);
	wxArrayString single_strings;
	single_strings.Add(wxT("1载台"));
	single_strings.Add(wxT("2载台"));

	m_lab_ip = new wxStaticText(this, -1, _("检测地址IP："));
	m_edit_ip = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(250, 25));
	m_lab_port = new wxStaticText(this, -1, _("端口号："));
	m_edit_port = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(250, 25));
	m_lab_path = new wxStaticText(this, -1, _("获取路径："));
	m_edit_path = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(250, 25));
	m_lab_path_csv = new wxStaticText(this, -1, _("csv路径："));
	m_edit_path_csv = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(250, 25));
	m_lab_read_img_path = new wxStaticText(this, -1, _("图片保存路径："));
	m_edit_read_img_path = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(250, 25));
	m_lab_excel_path = new wxStaticText(this, -1, _("报表保存路径："));
	m_edit_excel_path = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(250, 25));
	m_lab_single = new wxStaticText(this, -1, "载台信号：");
	m_radiobtn_single = new wxRadioBox(this, -1, "", wxPoint(0, 0), wxSize(250, 50), single_strings, 1, wxRA_SPECIFY_ROWS);

	m_btn_path = new wxButton(this, ID_BTN_PATH, _("选择地址"));
	m_btn_path_csv = new wxButton(this, ID_BTN_PATH_TWO, _("选择地址"));
	m_btn_path_save_img = new wxButton(this, ID_BTN_PATH_SAVE_IMG, _("选择地址"));
	m_btn_path_excel = new wxButton(this, ID_BTN_PATH_EXCEL, _("选择地址"));

	m_edit_ip->SetValue(m_wxs_ip);
	wxString temp;
	temp << m_i_port;
	m_edit_port->SetValue(temp);
	m_edit_path->SetValue(m_wxs_path);
	m_edit_path_csv->SetValue(m_wxs_path_csv);
	m_edit_read_img_path->SetValue(m_wxs_path_save_img);
	m_edit_excel_path->SetValue(m_wxs_path_excel);
	m_radiobtn_single->SetSelection(m_i_single);
	
	wxBoxSizer *lab_sizer = new wxBoxSizer(wxVERTICAL);
	lab_sizer->Add(m_lab_ip, 0, wxALL | wxALIGN_LEFT, 5);
	lab_sizer->Add(1, 6, 0, 0);
	lab_sizer->Add(m_lab_port, 0, wxALL | wxALIGN_LEFT, 5);
	lab_sizer->Add(1, 6, 0, 0);
	lab_sizer->Add(m_lab_path, 0, wxALL | wxALIGN_LEFT, 5);
	lab_sizer->Add(1, 6, 0, 0);
	lab_sizer->Add(m_lab_path_csv, 0, wxALL | wxALIGN_LEFT, 5);
	lab_sizer->Add(1, 6, 0, 0);
	lab_sizer->Add(m_lab_read_img_path, 0, wxALL | wxALIGN_LEFT, 5);
	lab_sizer->Add(1, 6, 0, 0);
	lab_sizer->Add(m_lab_excel_path, 0, wxALL | wxALIGN_LEFT, 5);
	lab_sizer->Add(1, 18, 0, 0);
	lab_sizer->Add(m_lab_single, 0, wxALL | wxALIGN_LEFT, 5);

	wxBoxSizer *edit_sizer = new wxBoxSizer(wxVERTICAL);
	edit_sizer->Add(1, 8, 0, 0);
	edit_sizer->Add(m_edit_ip, 0, wxALL | wxALIGN_CENTER, 5);
	edit_sizer->Add(m_edit_port, 0, wxALL | wxALIGN_CENTER, 5);
	edit_sizer->Add(m_edit_path, 0, wxALL | wxALIGN_CENTER, 5);
	edit_sizer->Add(m_edit_path_csv, 0, wxALL | wxALIGN_CENTER, 5);
	edit_sizer->Add(m_edit_read_img_path, 0, wxALL | wxALIGN_CENTER, 5);
	edit_sizer->Add(m_edit_excel_path, 0, wxALL | wxALIGN_CENTER, 5);
	edit_sizer->Add(m_radiobtn_single, 0, wxALL | wxALIGN_CENTER, 5);

	wxBoxSizer *choosebtn_sizer = new wxBoxSizer(wxVERTICAL);
	choosebtn_sizer->Add(1, 20, 0, 0);
	choosebtn_sizer->Add(m_btn_path, 0, wxALL | wxALIGN_CENTER, 2);
//	choosebtn_sizer->Add(1, 8, 0, 0);
	choosebtn_sizer->Add(m_btn_path_csv, 0, wxALL | wxALIGN_CENTER, 2);
	choosebtn_sizer->Add(m_btn_path_save_img, 0, wxALL | wxALIGN_CENTER, 2);
	choosebtn_sizer->Add(m_btn_path_excel, 0, wxALL | wxALIGN_CENTER, 2);

	m_btn_OK = new wxButton(this, ID_BTN_OK, _("确定"));
	m_btn_NO = new wxButton(this, ID_BTN_NO, _("返回"));

	wxBoxSizer *btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	btn_sizer->Add(m_btn_OK, 0, wxALL | wxALIGN_CENTER, 5);
	btn_sizer->Add(m_btn_NO, 0, wxALL | wxALIGN_CENTER, 5);

	wxBoxSizer *hor_sizer = new wxBoxSizer(wxHORIZONTAL);
	hor_sizer->Add(lab_sizer, 0, wxALL | wxALIGN_CENTER, 5);
	hor_sizer->Add(edit_sizer, 0, wxALL | wxALIGN_CENTER, 5);
	hor_sizer->Add(choosebtn_sizer, 0, wxALL | wxALIGN_CENTER, 5);

	top->Add(hor_sizer, 0, wxALL | wxALIGN_CENTER, 5);
	top->Add(btn_sizer, 0, wxALL | wxALIGN_CENTER, 5);

	this->SetSizer(top);

	this->Fit();

	this->Center();
}

void DetectionSetDlg::OnBtnOK(wxCommandEvent &)
{
	m_wxs_ip = m_edit_ip->GetValue();
	wxString temp_port = m_edit_port->GetValue();
	m_i_port = atoi(temp_port.c_str());
	m_i_single = m_radiobtn_single->GetSelection();

	EndModal(1);
}

void DetectionSetDlg::OnBtnNO(wxCommandEvent &)
{
	EndModal(0);
}

void DetectionSetDlg::OnBtnPath(wxCommandEvent &)
{
	wxDirDialog dlg(this, _("设置检测路径"), "", wxDD_NEW_DIR_BUTTON);
	if (wxID_OK == dlg.ShowModal())
	{
		m_wxs_path = dlg.GetPath();
		m_edit_path->SetValue(m_wxs_path);
	}
	else
	{
		dlg.Destroy();
		return;
	}
	dlg.Destroy();
}

void DetectionSetDlg::OnBtnPathCsv(wxCommandEvent &)
{
	wxDirDialog dlg(this, _("设置csv路径"), "", wxDD_NEW_DIR_BUTTON);
	if (wxID_OK == dlg.ShowModal())
	{
		m_wxs_path_csv = dlg.GetPath();
		m_edit_path_csv->SetValue(m_wxs_path_csv);
	}
	else
	{
		dlg.Destroy();
		return;
	}
	dlg.Destroy();
}

void DetectionSetDlg::OnBtnPathSaveImg(wxCommandEvent &)
{
	wxDirDialog dlg(this, _("设置图片保存路径"), "", wxDD_NEW_DIR_BUTTON);
	if (wxID_OK == dlg.ShowModal())
	{
		m_wxs_path_save_img = dlg.GetPath();
		m_edit_read_img_path->SetValue(m_wxs_path_save_img);
	}
	else
	{
		dlg.Destroy();
		return;
	}
	dlg.Destroy();
}

void DetectionSetDlg::OnBtnPathExcel(wxCommandEvent &)
{
	wxDirDialog dlg(this, _("设置Excel保存路径"), "", wxDD_NEW_DIR_BUTTON);
	if (wxID_OK == dlg.ShowModal())
	{
		m_wxs_path_excel = dlg.GetPath();
		m_edit_excel_path->SetValue(m_wxs_path_excel);
	}
	else
	{
		dlg.Destroy();
		return;
	}
	dlg.Destroy();
}

//==================================================================================================

BEGIN_EVENT_TABLE(ParameterSetDig, wxDialog)
	EVT_BUTTON(ID_BTN_OK, ParameterSetDig::OnBtnOK)
	EVT_BUTTON(ID_BTN_NO, ParameterSetDig::OnBtnNO)
END_EVENT_TABLE()

ParameterSetDig::ParameterSetDig(wxWindow* parent, wxWindowID id /* = -1 */, const wxString& title /* = "算法参数设置" */, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxCAPTION */)
	:wxDialog(parent, id, title, pos, size, style), m_i_ic_binaryValue(0), m_i_ic_backValue(0), m_i_ic_medianBlurSize(0)
{
	m_frame = parent;
}

ParameterSetDig::~ParameterSetDig()
{

}

void ParameterSetDig::CreateGUIcontrols()
{
	wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);
	m_ic_lab_binaryValue = new wxStaticText(this, -1, _("前景缺陷分割阈值："));
	m_ic_edit_binaryValue = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(100, 25));
	m_ic_lab_backValue = new wxStaticText(this, -1, _("背景缺陷分割阈值："));
	m_ic_edit_backValue = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(100, 25));
	m_ic_lab_medianBlurSize = new wxStaticText(this, -1, _("误报过滤尺寸："));
	m_ic_edit_medianBlurSize = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(100, 25));

	wxString temp;
	temp = "";
	temp << m_i_ic_binaryValue;
	m_ic_edit_binaryValue->SetValue(temp);
	temp = "";
	temp << m_i_ic_backValue;
	m_ic_edit_backValue->SetValue(temp);
	temp = "";
	temp << m_i_ic_medianBlurSize;
	m_ic_edit_medianBlurSize->SetValue(temp);
	
	wxBoxSizer *lab_ic_sizer = new wxBoxSizer(wxVERTICAL);
	lab_ic_sizer->Add(1, 10, 0, 0);
	lab_ic_sizer->Add(m_ic_lab_binaryValue, 0, wxALL | wxALIGN_LEFT, 5);
	lab_ic_sizer->Add(1, 8, 0, 0);
	lab_ic_sizer->Add(m_ic_lab_backValue, 0, wxALL | wxALIGN_LEFT, 5);
	lab_ic_sizer->Add(1, 8, 0, 0);
	lab_ic_sizer->Add(m_ic_lab_medianBlurSize, 0, wxALL | wxALIGN_LEFT, 5);

	wxBoxSizer *edit_ic_sizer = new wxBoxSizer(wxVERTICAL);
	edit_ic_sizer->Add(1, 8, 0, 0);
	edit_ic_sizer->Add(m_ic_edit_binaryValue, 0, wxALL | wxALIGN_CENTER, 5);
	edit_ic_sizer->Add(m_ic_edit_backValue, 0, wxALL | wxALIGN_CENTER, 5);
	edit_ic_sizer->Add(m_ic_edit_medianBlurSize, 0, wxALL | wxALIGN_CENTER, 5);

	m_btn_OK = new wxButton(this, ID_BTN_OK, _("确定"));
	m_btn_NO = new wxButton(this, ID_BTN_NO, _("返回"));

	wxBoxSizer *btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	btn_sizer->Add(m_btn_OK, 0, wxALL | wxALIGN_CENTER, 5);
	btn_sizer->Add(m_btn_NO, 0, wxALL | wxALIGN_CENTER, 5);

	wxBoxSizer *hor_ic_sizer = new wxBoxSizer(wxHORIZONTAL);
	hor_ic_sizer->Add(lab_ic_sizer, 0, wxALL | wxALIGN_CENTER, 5);
	hor_ic_sizer->Add(edit_ic_sizer, 0, wxALL | wxALIGN_CENTER, 5);

	wxStaticBox *box_ic = new wxStaticBox(this, -1, _("IC算法参数:"));
	wxStaticBoxSizer *box_ic_sizer = new wxStaticBoxSizer(box_ic, wxVERTICAL);
	box_ic_sizer->Add(hor_ic_sizer,wxALL | wxEXPAND, 5);

	wxBoxSizer *parameter_sizer = new wxBoxSizer(wxHORIZONTAL);
	parameter_sizer->Add(box_ic_sizer, 0, wxALL | wxEXPAND, 5);

	top->Add(parameter_sizer, 0, wxALL | wxALIGN_CENTER, 5);
	top->Add(btn_sizer, 0, wxALL | wxALIGN_CENTER, 5);

	this->SetSizer(top);

	this->Fit();

	this->Center();
}

void ParameterSetDig::OnBtnOK(wxCommandEvent &)
{
	wxString temp = "";

	temp = m_ic_edit_binaryValue->GetValue();
	m_i_ic_binaryValue = atoi(temp.c_str());

	temp = m_ic_edit_backValue->GetValue();
	m_i_ic_backValue = atoi(temp.c_str());

	temp = m_ic_edit_medianBlurSize->GetValue();
	m_i_ic_medianBlurSize = atoi(temp.c_str());

	EndModal(1);
}

void ParameterSetDig::OnBtnNO(wxCommandEvent &)
{
	EndModal(0);
}

//==================================================================================================

BEGIN_EVENT_TABLE(AddrSetDig, wxDialog)
	EVT_BUTTON(ID_BTN_OK, AddrSetDig::OnBtnOK)
	EVT_BUTTON(ID_BTN_NO, AddrSetDig::OnBtnNO)
END_EVENT_TABLE()

AddrSetDig::AddrSetDig(wxWindow* parent, wxWindowID id /* = -1 */, const wxString& title /* = "地址参数设置" */, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxCAPTION */)
	:wxDialog(parent, id, title, pos, size, style), m_i_read_addr(0), m_wxs_read_type(""), m_i_write_OK_addr(0), m_wxs_write_OK_type(""), m_i_write_NG_addr(0), m_wxs_write_NG_type("")
{
	m_frame = parent;
}

AddrSetDig::~AddrSetDig()
{
	
}

void AddrSetDig::CreateGUIcontrols()
{
	wxBoxSizer *sizer_top = new wxBoxSizer(wxVERTICAL);

	m_lab_read_addr = new wxStaticText(this, -1, _("socket读取地址：     "));
	m_edit_read_addr = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(100, 25));
	wxArrayString read_type;
	read_type.Add(_("D"));
	read_type.Add(_("M"));
	m_radiobox_read_type = new wxRadioBox(this, -1, _("socket读取地址类型"), wxPoint(0, 0), wxSize(70, 80), read_type, 1, wxRA_SPECIFY_COLS);

	wxBoxSizer *read_addr_sizer = new wxBoxSizer(wxHORIZONTAL);
	read_addr_sizer->Add(m_lab_read_addr, 0, wxALL | wxALIGN_CENTER, 5);
	read_addr_sizer->Add(m_edit_read_addr, 0, wxALL | wxALIGN_CENTER, 5);
	read_addr_sizer->Add(m_radiobox_read_type, 0, wxALL | wxALIGN_CENTER, 5);

	m_lab_write_OK_addr = new wxStaticText(this, -1, _("socket写入OK地址："));
	m_edit_write_OK_addr = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(100, 25));
	wxArrayString write_OK_type;
	write_OK_type.Add(_("D"));
	write_OK_type.Add(_("M"));
	m_radiobox_write_OK_type = new wxRadioBox(this, -1, _("socket写入OK地址类型"), wxPoint(0, 0), wxSize(70, 80), write_OK_type, 1, wxRA_SPECIFY_COLS);

	m_lab_write_NG_addr = new wxStaticText(this, -1, _("socket写入NG地址："));
	m_edit_write_NG_addr = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(100, 25));
	wxArrayString write_NG_type;
	write_NG_type.Add(_("D"));
	write_NG_type.Add(_("M"));
	m_radiobox_write_NG_type = new wxRadioBox(this, -1, _("socket写入NG地址类型"), wxPoint(0, 0), wxSize(70, 80), write_NG_type, 1, wxRA_SPECIFY_COLS);

	wxBoxSizer *write_addr_OK_sizer = new wxBoxSizer(wxHORIZONTAL);
	write_addr_OK_sizer->Add(m_lab_write_OK_addr, 0, wxALL | wxALIGN_CENTER, 5);
	write_addr_OK_sizer->Add(m_edit_write_OK_addr, 0, wxALL | wxALIGN_CENTER, 5);
	write_addr_OK_sizer->Add(m_radiobox_write_OK_type, 0, wxALL | wxALIGN_CENTER, 5);

	wxBoxSizer *write_addr_NG_sizer = new wxBoxSizer(wxHORIZONTAL);
	write_addr_NG_sizer->Add(m_lab_write_NG_addr, 0, wxALL | wxALIGN_CENTER, 5);
	write_addr_NG_sizer->Add(m_edit_write_NG_addr, 0, wxALL | wxALIGN_CENTER, 5);
	write_addr_NG_sizer->Add(m_radiobox_write_NG_type, 0, wxALL | wxALIGN_CENTER, 5);

	wxStaticBox *sb_read = new wxStaticBox(this, -1, "读", wxPoint(0, 0));
	wxStaticBoxSizer *sizer_read = new wxStaticBoxSizer(sb_read, wxHORIZONTAL);
	sizer_read->Add(read_addr_sizer, 0, wxALL | wxALIGN_CENTER, 5);

	wxStaticBox *sb_write = new wxStaticBox(this, -1, "写", wxPoint(0, 0));
	wxStaticBoxSizer *sizer_write = new wxStaticBoxSizer(sb_write, wxVERTICAL);
	sizer_write->Add(write_addr_OK_sizer, 0, wxALL | wxALIGN_CENTER, 5);
	sizer_write->Add(write_addr_NG_sizer, 0, wxALL | wxALIGN_CENTER, 5);

	wxString temp;
	temp = "";
	temp << m_i_read_addr;
	m_edit_read_addr->SetValue(temp);
	temp = "";
	temp << m_i_write_OK_addr;
	m_edit_write_OK_addr->SetValue(temp);
	temp = "";
	temp << m_i_write_NG_addr;
	m_edit_write_NG_addr->SetValue(temp);

	if (m_wxs_read_type == "D")
	{
		m_radiobox_read_type->SetSelection(0);
	}
	else
	{
		m_radiobox_read_type->SetSelection(1);
	}
	if (m_wxs_write_OK_type == "D")
	{
		m_radiobox_write_OK_type->SetSelection(0);
	}
	else
	{
		m_radiobox_write_OK_type->SetSelection(1);
	}
	if (m_wxs_write_NG_type == "D")
	{
		m_radiobox_write_NG_type->SetSelection(0);
	}
	else
	{
		m_radiobox_write_NG_type->SetSelection(1);
	}

	m_btn_OK = new wxButton(this, ID_BTN_OK, _("确定"));
	m_btn_NO = new wxButton(this, ID_BTN_NO, _("返回"));

	wxBoxSizer *btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	btn_sizer->Add(m_btn_OK, 0, wxALL | wxALIGN_CENTER, 5);
	btn_sizer->Add(m_btn_NO, 0, wxALL | wxALIGN_CENTER, 5);

	sizer_top->Add(sizer_read, 0, wxALL | wxALIGN_CENTER, 5);
	sizer_top->Add(sizer_write, 0, wxALL | wxALIGN_CENTER, 5);
	sizer_top->Add(btn_sizer, 0, wxALL | wxALIGN_CENTER, 5);

	this->SetSizer(sizer_top);

	this->Fit();

	this->Center();
}

void AddrSetDig::OnBtnOK(wxCommandEvent &)
{
	wxString temp = "";

	temp = m_edit_read_addr->GetValue();
	m_i_read_addr = atoi(temp.c_str());

	temp = m_edit_write_OK_addr->GetValue();
	m_i_write_OK_addr = atoi(temp.c_str());

	temp = m_edit_write_NG_addr->GetValue();
	m_i_write_NG_addr = atoi(temp.c_str());

	temp = m_radiobox_read_type->GetStringSelection();
	m_wxs_read_type = temp;

	temp = m_radiobox_write_OK_type->GetStringSelection();
	m_wxs_write_OK_type = temp;

	temp = m_radiobox_write_NG_type->GetStringSelection();
	m_wxs_write_NG_type = temp;

	EndModal(1);
}

void AddrSetDig::OnBtnNO(wxCommandEvent &)
{
	EndModal(0);
}

//================================================================

BEGIN_EVENT_TABLE(SetAIDlg, wxDialog)
	EVT_BUTTON(ID_BTN_OK, SetAIDlg::OnBtnOK)
	EVT_BUTTON(ID_BTN_CANCEL, SetAIDlg::OnBtnCancel)
END_EVENT_TABLE()

SetAIDlg::SetAIDlg(wxWindow* parent, wxWindowID id /* = -1 */, const wxString& title /* = "AI设置" */, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxCAPTION */)
	:wxDialog(parent, id, title, pos, size, style, title), m_wxs_ai_path("")
{
	m_parent = parent;
}

SetAIDlg::~SetAIDlg()
{

}

void SetAIDlg::CreateGUIcontrols()
{
	wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);

	m_lab_is_use_ai = new wxStaticText(this, -1, _("是否使用AI："));
	m_check_is_use_ai = new wxCheckBox(this, -1, "");
	m_lab_ai_path = new wxStaticText(this, -1, _("AI路径："));
	m_edit_ai_path = new wxTextCtrl(this, -1, "", wxPoint(0, 0), wxSize(250, 25));

	m_check_is_use_ai->SetValue(m_b_is_use_ai);
	m_edit_ai_path->SetLabel(m_wxs_ai_path);

	wxBoxSizer *lab_box = new wxBoxSizer(wxVERTICAL);

	lab_box->Add(m_lab_is_use_ai, 0, wxALL | wxEXPAND, 5);
	lab_box->Add(1, 8, 0, 0);
	lab_box->Add(m_lab_ai_path, 0, wxALL | wxEXPAND, 5);

	wxBoxSizer *edit_box = new wxBoxSizer(wxVERTICAL);
	edit_box->Add(1, 5, 0, 0);
	edit_box->Add(m_check_is_use_ai, 0, wxALL | wxEXPAND, 5);
	edit_box->Add(1, 8, 0, 0);
	edit_box->Add(m_edit_ai_path, 0, wxALL | wxEXPAND, 5);

	m_btn_OK = new wxButton(this, ID_BTN_OK, _("确定"));
	m_btn_Cancel = new wxButton(this, ID_BTN_CANCEL, _("返回"));

	wxBoxSizer *btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	btn_sizer->Add(m_btn_OK, 0, wxALL | wxALIGN_CENTER, 5);
	btn_sizer->Add(m_btn_Cancel, 0, wxALL | wxALIGN_CENTER, 5);

	wxBoxSizer *hor_box = new wxBoxSizer(wxHORIZONTAL);
	hor_box->Add(lab_box, 0, wxALL | wxALIGN_CENTER, 5);
	hor_box->Add(edit_box, 0, wxALL | wxALIGN_CENTER, 5);

	top->Add(hor_box, 0, wxALL | wxALIGN_CENTER, 5);
	top->Add(btn_sizer, 0, wxALL | wxALIGN_CENTER, 5);

	this->SetSizer(top);

	this->Fit();

	this->Center();
}

void SetAIDlg::OnBtnOK(wxCommandEvent &)
{
	m_b_is_use_ai = m_check_is_use_ai->GetValue();
	m_wxs_ai_path = m_edit_ai_path->GetValue();

	EndModal(1);
}

void SetAIDlg::OnBtnCancel(wxCommandEvent &)
{
	EndModal(0);
}
//====================================================================================

BEGIN_EVENT_TABLE(ModelListCtrl, wxListCtrl)

END_EVENT_TABLE()

ModelListCtrl::ModelListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxLC_REPORT | wxLC_SORT_ASCENDING */)
	:wxListCtrl(parent, id, pos, size, style)
{
	m_frame = parent;
}
//==========================================================================================================
BEGIN_EVENT_TABLE(ModelPanel, wxPanel)
	
END_EVENT_TABLE()

ModelPanel::ModelPanel(wxWindow *parent)
	:wxPanel(parent, -1, wxDefaultPosition, wxSize(200, 250), wxBORDER_NONE)
{
	m_frame = parent;
	m_list_recipe_name.clear();
	CreateGui();
}

ModelPanel::~ModelPanel()
{
	m_list_recipe_name.clear();
}

void ModelPanel::CreateGui()
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

void ModelPanel::AddData(list<Image_Properties> list_data)
{
	list<Image_Properties> list_add_data;
	list_add_data.assign(list_data.begin(), list_data.end());
	m_modellistctrl->DeleteAllItems();

	int testi = 0;

	m_list_recipe_name.clear();
	for (list<Image_Properties>::iterator it = list_add_data.begin(); it != list_add_data.end(); it++)
	{
		wxArrayString cols_name;
		wxString wxs_num = "";
		wxs_num << it->image_point.size();
		cols_name.Add(it->image_name);
		cols_name.Add(wxs_num);

		m_list_recipe_name.push_back(it->image_name);

		long tmp = m_modellistctrl->InsertItem(testi, cols_name[0], 2);
		m_modellistctrl->SetItemData(tmp, testi);

		for (unsigned int i = 1; i < cols_name.size(); i++)
		{
			m_modellistctrl->SetItem(tmp, i, cols_name[i]);
		}

		testi++;
	}

	m_modellistctrl->RefreshItem(testi);
}

void ModelPanel::DeleteAllModel()
{
	m_modellistctrl->DeleteAllItems();
}

BEGIN_EVENT_TABLE(ModelPanelWindow, wxDialog)
	EVT_BUTTON(ID_BTN_CLEAR, ModelPanelWindow::OnBtnClear)
	EVT_BUTTON(ID_BIN_SAVE, ModelPanelWindow::OnBtnSave)
END_EVENT_TABLE()

ModelPanelWindow::ModelPanelWindow(wxWindow* parent)
	:wxDialog(parent, -1, _("模板"), wxPoint(850, 250), wxSize(800, 500), wxCAPTION)
{
	m_parent = parent;

	wxBoxSizer *global_sizer = new wxBoxSizer(wxVERTICAL);

	m_modelpanel = new ModelPanel(this);
	m_btn_clear = new wxButton(this, ID_BTN_CLEAR, _("清空配方"), wxDefaultPosition, wxSize(100, 30));
	m_btn_save = new wxButton(this, ID_BIN_SAVE, _("保存配方"), wxDefaultPosition, wxSize(100, 30));

	wxBoxSizer *btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	btn_sizer->Add(m_btn_clear, 0, wxALL | wxEXPAND, 2);
	btn_sizer->Add(m_btn_save, 0, wxALL | wxEXPAND, 2);

	global_sizer->Add(m_modelpanel, 0, wxALL | wxEXPAND, 2);
	global_sizer->Add(btn_sizer, 0, wxALL | wxEXPAND, 2);

	this->SetSizer(global_sizer);
	this->Fit();
}

ModelPanelWindow::~ModelPanelWindow()
{
	if (m_modelpanel)
	{
		delete m_modelpanel;
		m_modelpanel = NULL;
	}
}

void ModelPanelWindow::OnBtnClear(wxCommandEvent &)
{
	wxQueueEvent(m_parent->GetEventHandler(),
		new wxCommandEvent(wxEVT_BUTTON, ID_BTN_CLEAR));
}

void ModelPanelWindow::OnBtnSave(wxCommandEvent &)
{
	wxQueueEvent(m_parent->GetEventHandler(),
		new wxCommandEvent(wxEVT_BUTTON, ID_BIN_SAVE));
}

void ModelPanelWindow::DeleteAllModel()
{
	m_modelpanel->DeleteAllModel();
}