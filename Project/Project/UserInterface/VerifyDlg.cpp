#include "VerifyDlg.h"

BEGIN_EVENT_TABLE(ImageFrame, wxFrame)
	EVT_PAINT(ImageFrame::OnPaint)
	EVT_LEFT_DOWN(ImageFrame::OnMouseDown)
END_EVENT_TABLE()
ImageFrame::ImageFrame(wxWindow *v_parent)
	: wxFrame(v_parent, -1, "", wxDefaultPosition, wxSize(300, 300), wxSTAY_ON_TOP)
{
	m_image = wxBitmap(wxImage(300, 300, true));
	parent = v_parent;
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

ImageFrame::~ImageFrame()
{
}

void ImageFrame::OnPaint(wxPaintEvent&)
{
	wxAutoBufferedPaintDC dc1(this);
	PrepareDC(dc1);
	dc1.SetBackground(wxBrush(parent->GetBackgroundColour()));
	dc1.Clear();
	dc1.DrawBitmap(m_image, 0, 0);
}

void ImageFrame::SetImage(wxImage& img, wxRect fa_rect)
{
	m_image = wxBitmap(img);

	wxMemoryDC dc;
	wxPen pen(wxColour(255, 0, 0), 2, wxSOLID);
	dc.SelectObject(m_image);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.SetPen(pen);
	dc.DrawRoundedRectangle(fa_rect, 10);
	dc.SelectObject(wxNullBitmap);

	this->SetSize(m_image.GetWidth(), m_image.GetHeight());
}

void ImageFrame::OnMouseDown(wxMouseEvent&)
{
	wxString msg = "用户点击放大后的图片";
	MyLog::LogUser(msg.mb_str());

	this->Destroy();

	msg = "该放大图片已关闭";
	MyLog::LogMessage(msg.mb_str());
}

BEGIN_EVENT_TABLE(ResultPanel, wxScrolledWindow)
	EVT_PAINT(ResultPanel::OnPaint)
	EVT_KEY_DOWN(ResultPanel::OnKeyDown)
END_EVENT_TABLE()
ResultPanel::ResultPanel(wxWindow* parent, wxWindowID id,
	const wxPoint& pos, const wxSize& size, long style)
	: wxScrolledWindow(parent, id, pos, size, style, wxT("cmpdisView"))
{
	bg_color = parent->GetBackgroundColour();
	SetVirtualSize(100, 100);
	SetScrollRate(1, 1);
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	wxImage img;
	img.Create(100, 100, 1);
	img.SetRGB(wxRect(0, 0, 100, 100), bg_color.Red(),
		bg_color.Green(), bg_color.Blue());
	image_to_draw = wxBitmap(img);

	virt_width = 0;
	virt_height = 0;
}

void ResultPanel::OnPaint(wxPaintEvent&)
{
	SetVirtualSize(virt_width, virt_height);
	wxAutoBufferedPaintDC dc1(this);
	PrepareDC(dc1);
	wxBrush bg_brush = wxBrush(bg_color);
	dc1.SetBackground(bg_brush);
	dc1.Clear();
	dc1.DrawBitmap(image_to_draw, 0, 0);
}

void ResultPanel::SetResultSize(int w, int h)
{
	virt_width = w;
	virt_height = h;
	wxImage img;
	img.Create(w, h, 1);
	img.SetRGB(wxRect(0, 0, w, h), bg_color.Red(),
		bg_color.Green(), bg_color.Blue());
	image_to_draw = wxBitmap(img);
	SetVirtualSize(virt_width, virt_height);
	Refresh(false);
	Update();
}

void ResultPanel::OnKeyDown(wxKeyEvent &)
{
	
}

IMPLEMENT_DYNAMIC_CLASS(TestResultView, wxScrolledWindow)
BEGIN_EVENT_TABLE(TestResultView, wxScrolledWindow)
	EVT_PAINT(TestResultView::OnPaint)
	EVT_LEFT_DOWN(TestResultView::OnMouseDown)
	EVT_LEFT_DCLICK(TestResultView::OnMouseDClick)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_IMAGECLICKEVENT)

TestResultView::TestResultView(wxWindow* parent, wxWindowID id,
	const wxPoint& pos, const wxSize& size, long style)
	: wxScrolledWindow(parent, id, pos, size, style, _T("TestResultView"))
{
	bg_color = parent->GetBackgroundColour();
	wxImage img;
	img.Create(300, 300, 1);
	img.SetRGB(wxRect(0, 0, 100, 100), bg_color.Red(),
		bg_color.Green(), bg_color.Blue());
	image = img;
	image_to_draw = wxBitmap(img);

	move_type = 0;
	zoom = 1.0;
	begin_point = wxPoint(0, 0);
	SetVirtualSize(image_to_draw.GetWidth(), image_to_draw.GetHeight());
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	SetOwnBackgroundColour(wxColour(255, 255, 255));

	ratio_x = 1.0;
	ratio_y = 1.0;
	frame = NULL;
	damage_len = 0;
}

bool TestResultView::SetImage(wxImage& img, wxRect fa_rect /* = wxRect(0, 0, 1, 1) */, double len /* = 0 */, wxRect ng_rect /* = wxRect(0, 0, 1, 1) */)
{
	damage_len = len;

	image_to_draw = wxBitmap(img);
	image = img;
	fail_rst = fa_rect;
	m_ng_rect = ng_rect;
	Refresh(false);
	Update();
	return true;
}

void  TestResultView::DrawResults(wxRect fail_rects)
{
	fail_rst = fail_rects;
	Refresh(false);
	Update();
}

wxImage TestResultView::GetImage()
{
	return image_to_draw.ConvertToImage();
}


void TestResultView::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxMemoryDC temp_dc;
	wxSize sz = GetClientSize();
	float w_ratio = image.GetWidth() * 1.0 / sz.x;
	float h_ratio = image.GetHeight() * 1.0 / sz.y;
	image_to_draw = wxBitmap(image.Scale(sz.x, sz.y));
	zoom = 1.0;

	int w = image_to_draw.GetWidth();
	int h = image_to_draw.GetHeight();
	begin_point = wxPoint(0, 0);

	if (w < sz.GetWidth() || h < sz.GetHeight())
		begin_point = wxPoint(sz.GetWidth() / 2 - w / 2, sz.GetHeight() / 2 - h / 2);

	if (w < sz.x)
		w = sz.x;

	if (h < sz.y)
		h = sz.y;

	SetVirtualSize(w, h);
	__buffer = wxBitmap(w, h);
	temp_dc.SelectObject(__buffer);

	if (!image_to_draw.IsOk())
		return;
	wxBrush bg_brush = wxBrush(bg_color);
	temp_dc.SetBackground(bg_brush);
	temp_dc.Clear();
	temp_dc.DrawBitmap(image_to_draw, begin_point.x, begin_point.y);
	wxBrush brush(*wxLIGHT_GREY, wxTRANSPARENT);
	wxPen pen(wxColour(255, 0, 0), 2, wxSOLID);
	temp_dc.SetBrush(brush);

	temp_dc.SetPen(pen);

	int x = (int)(m_ng_rect.x / w_ratio) + begin_point.x;
	int y = (int)(m_ng_rect.y / h_ratio) + begin_point.y;
	int w2 = (int)(m_ng_rect.GetWidth() / w_ratio);
	int h2 = (int)(m_ng_rect.GetHeight() / h_ratio);

	temp_dc.DrawRectangle(wxRect(x, y, w2, h2));
	temp_dc.SelectObject(wxNullBitmap);

	if (!__buffer.IsOk())
	{
		cout << "TestResultView::OnPaint image invalid" << endl;
		wxLogError(wxString::Format("%s(%d): image invalid", __FILE__, __LINE__));
		return;
	}
	wxAutoBufferedPaintDC dc1(this);
	PrepareDC(dc1);
	wxBrush brush1 = wxBrush(bg_color);
	dc1.SetBackground(brush1);
	dc1.Clear();
	dc1.DrawBitmap(__buffer, 0, 0);
}

void TestResultView::OnMouseDown(wxMouseEvent &)
{
	if (frame == NULL)
		return;

	ImageClickEvent evt(wxEVT_IMAGECLICKEVENT, GetId());
	evt.SetPosition(pos_x, pos_y);
	evt.SetEventObject(frame);
	frame->GetEventHandler()->AddPendingEvent(evt);
}

void TestResultView::SetPosition(double x, double y)
{
	pos_x = x;
	pos_y = y;
}

void TestResultView::SetFrame(wxWindow *f)
{
	frame = f;
}

void TestResultView::OnMouseDClick(wxMouseEvent &)
{
	wxString msg = "用户双击某张缩略图， 正在放大显示";
	MyLog::LogUser(msg.mb_str());

	ImageFrame *image_f = new ImageFrame(frame);
	image_f->SetImage(image, m_ng_rect);
	image_f->Show(true);

	msg = "已显示";
	MyLog::LogMessage(msg.mb_str());
}

BEGIN_EVENT_TABLE(VerifyDlg, wxDialog)
	EVT_BUTTON(ID_BTN_MAP, VerifyDlg::OnBtnShowImage)
	EVT_BUTTON(ID_BTN_OK, VerifyDlg::OnBtnOK)
	EVT_BUTTON(ID_BTN_NG, VerifyDlg::OnBtnNG)
	EVT_BUTTON(ID_BTN_NEXT, VerifyDlg::OnBtnNextClick)
	EVT_BUTTON(ID_BTN_PREV, VerifyDlg::OnBtnPrevClick)
	EVT_CLOSE(VerifyDlg::OnClose)
END_EVENT_TABLE()

int VerifyDlg::VERIFY_IMAGE_SIZE = 200;
int VerifyDlg::VERIFY_ROWS = 3;
int VerifyDlg::VERIFY_COLS = 6;

VerifyDlg::VerifyDlg(wxWindow *parent)
	: wxDialog(parent, -1, "", wxPoint(850, 20), wxSize(300, 100), VERIFYDLG_STYLE)
{
	m_frame = parent;

	m_bri_num = 0;
	m_cur_index = 0;
	m_treated = false;
	m_err_code = "";
	m_small_black_num = 0;
	m_thin_num = 0;
	m_fat_num = 0;
	m_ok_wait_time = -1;
	m_demo_model = false;
	m_num4cur_page = 0;
	CreateGUIControls();
}

VerifyDlg::~VerifyDlg()
{

}

void VerifyDlg::OnClose(wxCloseEvent &)
{
	this->Show(false);
}

void VerifyDlg::CreateGUIControls()
{
	m_global_sizer = new wxBoxSizer(wxVERTICAL);
	m_panel = new wxPanel(this, -1);
	m_label_result = new wxStaticText(m_panel, -1, wxT("结果"), wxPoint(5, 5));
	wxFont f_font(30, wxSWISS, wxNORMAL, wxBOLD, false, wxT("宋体"));
	m_label_result->SetFont(f_font);
	m_label_result->SetForegroundColour(wxColour(255, 0, 0));
	
	m_aa_left = new wxTextCtrl(m_panel, -1, "", wxPoint(0, 0), wxSize(100, 8));
	m_aa_left->SetBackgroundColour(*wxLIGHT_GREY);
	m_aa_right = new wxTextCtrl(m_panel, -1, "", wxPoint(0, 0), wxSize(100, 8));
	m_aa_right->SetBackgroundColour(*wxLIGHT_GREY);
	wxBoxSizer *sss = new wxBoxSizer(wxHORIZONTAL);
	sss->Add(m_aa_left, 0, wxALIGN_CENTER, 5);
	sss->AddSpacer(50);
	sss->Add(m_aa_right, 0, wxALIGN_CENTER, 5);

	wxStaticLine * st_line1 = new wxStaticLine(m_panel, -1, wxPoint(144, 150), wxSize(272, -1), wxLI_HORIZONTAL);

	wxStaticBox * result_box = new wxStaticBox(m_panel, -1, _("结果"));
	wxStaticBoxSizer * result_sizer = new wxStaticBoxSizer(result_box, wxVERTICAL);

	/////////////////////// center result sum
	wxPanel  * center_panel = new wxPanel(m_panel, -1);
	wxFont font(14, wxSWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("微软雅黑"));

	wxStaticText * label_fail_sum = new wxStaticText(center_panel, -1, _("错误统计："));
	wxStaticText * label_image_name = new wxStaticText(center_panel, -1, _("图片名："));
	wxStaticText * label_inspect_time = new wxStaticText(center_panel, -1, _("检测时间："));

	label_fail_sum->SetFont(font);
	label_image_name->SetFont(font);
	label_inspect_time->SetFont(font);

	m_machine_sn = new wxStaticText(center_panel, -1, "");
	m_insp_time = new wxStaticText(center_panel, -1, "");
	m_fail_sum = new wxStaticText(center_panel, -1, "");

	m_machine_sn->SetFont(font);
	m_insp_time->SetFont(font);
	m_fail_sum->SetFont(font);

	wxBoxSizer * center_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * center_left_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * l_sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * r_sizer = new wxBoxSizer(wxVERTICAL);
	l_sizer->Add(label_image_name, 0, wxALL, 2);
	l_sizer->Add(label_inspect_time, 0, wxALL, 2);
	l_sizer->Add(label_fail_sum, 0, wxALL, 2);

	r_sizer->Add(m_machine_sn, 0, wxALL, 2);
	r_sizer->Add(m_insp_time, 0, wxALL, 2);
	r_sizer->Add(m_fail_sum, 0, wxALL, 2);

	center_left_sizer->Add(l_sizer, 0, wxEXPAND | wxALL, 1);
	center_left_sizer->Add(r_sizer, 0, wxEXPAND | wxALL, 1);
	center_sizer->Add(center_left_sizer, 0, wxALL, 2);
	center_panel->SetSizer(center_sizer);

	/////////////////// line panel 
	m_rs_panel = new ResultPanel(m_panel, -1, wxPoint(97, 279),
		wxSize(550, 300));
	m_line_rst_sizer = new wxGridSizer(0, 6, wxSize(1, 1));
	m_rs_panel->SetSizer(m_line_rst_sizer);

	wxStaticLine * st_line2 = new wxStaticLine(m_panel, -1, wxPoint(144, 150), wxSize(272, -1), wxLI_HORIZONTAL);
	m_btn_map = new wxButton(m_panel, ID_BTN_MAP, _("查看原图"), wxPoint(0, 0), wxSize(120, 50));
	m_btn_ok = new wxButton(m_panel, ID_BTN_OK, _("OK"), wxPoint(0, 0), wxSize(120, 50));
	m_btn_ng = new wxButton(m_panel, ID_BTN_NG, _("NG"), wxPoint(0, 0), wxSize(120, 50));
	m_btn_prev = new wxButton(m_panel, ID_BTN_PREV, _("前一页"), wxPoint(0, 0), wxSize(120, 50));
	m_btn_next = new wxButton(m_panel, ID_BTN_NEXT, _("后一页"), wxPoint(0, 0), wxSize(120, 50));

	m_btn_map->SetFont(font);
	m_btn_ok->SetFont(font);
	m_btn_ng->SetFont(font);
	m_btn_prev->SetFont(font);
	m_btn_next->SetFont(font);

	wxBoxSizer *btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	btn_sizer->Add(m_btn_map, 0, wxALIGN_CENTER, 5);
	btn_sizer->AddSpacer(10);
	btn_sizer->Add(m_btn_ok, 0, wxALIGN_CENTER, 5);
	btn_sizer->AddSpacer(10);
	btn_sizer->Add(m_btn_ng, 0, wxALIGN_CENTER, 5);
	btn_sizer->AddSpacer(10);
	btn_sizer->Add(m_btn_prev, 0, wxALIGN_CENTER, 5);
	btn_sizer->AddSpacer(10);
	btn_sizer->Add(m_btn_next, 0, wxALIGN_CENTER, 5);

	result_sizer->Add(m_rs_panel, 1, wxEXPAND | wxALL, 5);
	wxBoxSizer * center_sizer0 = new wxBoxSizer(wxHORIZONTAL);
	center_sizer0->Add(center_panel, 1, wxEXPAND | wxALL, 1);
	center_sizer0->Add(btn_sizer, 0, wxEXPAND, 1);
	center_sizer0->AddSpacer(15);

	m_global_sizer->Add(m_label_result, 0, wxALIGN_CENTER | wxALL, 2);
	m_global_sizer->Add(sss, 0, wxALIGN_CENTER | wxALL, 2);
	m_global_sizer->Add(st_line1, 0, wxEXPAND | wxALL, 1);
	m_global_sizer->Add(center_sizer0, 0, wxALL | wxEXPAND, 1);
	m_global_sizer->Add(st_line2, 0, wxEXPAND | wxALL, 1);
	m_global_sizer->Add(result_sizer, 1, wxEXPAND | wxALL, 1);

	m_panel->SetSizer(m_global_sizer);
	m_rs_panel->SetFocus();

	SetTitle(_("缺陷结果"));
	SetIcon(wxNullIcon);
	this->SetSize(200, 200, 900, 700);

	this->Center();
}

void VerifyDlg::SetTestResult(const list<s_verify>& fail_msg, int brdge_num)
{
	m_last_flag = false;
	m_start_verity_time = GetTickCount();

	m_fail_sum->SetLabel(wxString::Format("%d  ( 0 / 0 页 )", (int)fail_msg.size()));
	if (fail_msg.size() >= 600)
		m_fail_sum->SetLabel(m_fail_sum->GetLabel() + _("Too many error. Can not show all."));

	if (fail_msg.size() == 0) {
		m_label_result->SetForegroundColour(*wxGREEN);
		m_label_result->SetLabel(_("Pass"));
	}
	else {
		m_label_result->SetForegroundColour(*wxRED);
		m_label_result->SetLabel(_("Fail"));
	}

	m_true_error_list.clear();
	m_fail_msg.clear();
	if (fail_msg.size() == 0)
	{
		m_btn_prev->Enable(false);
		m_btn_next->Enable(false);

		SetSize(wxSize(1100, 400));
		Center();
		return;
	}

	m_fail_msg.assign(fail_msg.begin(), fail_msg.end());
	m_bri_num = brdge_num;

	m_cur_index = 0;

	ShowNextErrMsg();
}
void VerifyDlg::ShowNextErrMsg()
{
	if (m_cur_index == 0)
		m_btn_prev->Enable(false);
	else
		m_btn_prev->Enable(true);

	if (m_cur_index > m_fail_msg.size())
	{
		m_last_flag = true;
		m_btn_next->Enable(false);
		return;
	}

	m_start_verity_page = GetTickCount();

	int err_num = m_fail_msg.size();
	int cur_page = m_cur_index / (VERIFY_ROWS * VERIFY_COLS) + 1;
	int all_page = (err_num + VERIFY_ROWS * VERIFY_COLS - 1) / (VERIFY_ROWS * VERIFY_COLS);
	m_fail_sum->SetLabel(wxString::Format("%d  ( %d / %d 页 )", err_num, cur_page, all_page));
	if (err_num >= 600)
		m_fail_sum->SetLabel(m_fail_sum->GetLabel() + _("Too many error. Can not show all."));

	m_num4cur_page = 0;
	int i = 0;
	int ii_index = i + 1 + m_cur_index;
	list<s_verify>::iterator iter = m_fail_msg.begin();
	try {
		advance(iter, m_cur_index);
		m_line_rst_sizer->Clear(true);
		m_line_rst_sizer->SetCols(VERIFY_COLS);

		while (iter != m_fail_msg.end() && i < VERIFY_ROWS * VERIFY_COLS/*18*/)
		{
			wxString label_str = "";
			if (ii_index <= m_bri_num)
				label_str = wxString::Format("[%d] %s", ii_index, iter->m_pos_code.c_str());
			else
				label_str = wxString::Format("[%d] %s", ii_index, iter->m_pos_code.c_str());

			wxCheckBox * label = new wxCheckBox(m_rs_panel, -1, label_str);
			label->SetValue(false);
			for (list<int>::iterator it = m_true_error_list.begin(); it != m_true_error_list.end(); ++it)
			{
				if (*it == ii_index)
				{
					label->SetValue(true);
					label->SetBackgroundColour(*wxRED);
					break;
				}
			}

			wxString len_type = "Point";
			if (iter->m_len > 3.0) len_type = "Line";
			label->SetToolTip(wxString::Format("%04d-%s.jpg;&%d,%s,%s,%.1f;", ii_index, iter->m_pos_code.c_str(), ii_index, iter->m_pos_code.c_str(), len_type, iter->m_len));
			label->Connect(wxEVT_CHECKBOX, wxCommandEventHandler(VerifyDlg::OnCheckboxClick));
			wxFont font(14, wxSWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("微软雅黑"));
			label->SetFont(font);

			TestResultView * tmp = new TestResultView(m_rs_panel, -1, wxPoint(10, 10), wxSize(VERIFY_IMAGE_SIZE, VERIFY_IMAGE_SIZE));
			tmp->SetImage(iter->m_img, iter->m_rect, iter->m_len, iter->m_ng_rect);
			tmp->SetPosition(iter->m_pos_x, iter->m_pos_y);
			tmp->SetFrame(m_frame);
			tmp->SetToolTip(wxString::Format("Pos(%.2f, %.2f), length(%.2f)", iter->m_pos_x, iter->m_pos_y, iter->m_len));

			wxBoxSizer * w_sizer = new wxBoxSizer(wxVERTICAL);
			w_sizer->Add(tmp, 1, wxALL | wxEXPAND, 1);
			w_sizer->Add(label, 0, wxLEFT, 45);
			m_line_rst_sizer->Add(w_sizer, 1, wxEXPAND, 1);

			++i;
			++iter;
			++ii_index;
			++m_num4cur_page;
		}
	}
	catch (...) {
	}

	if (iter == m_fail_msg.end())
	{
		m_last_flag = true;
		m_btn_next->Enable(false);
	}
	else
		m_btn_next->Enable(true);

	m_line_rst_sizer->Layout();
	/*unsigned*/ int cols = VERIFY_COLS/*6*/;
	int rows = i / cols;
	if (i < cols)
	{
		cols = i;
		rows = 1;
	}
	else if (i > cols * rows)
		++rows;

	m_rs_panel->SetResultSize(cols * (VERIFY_IMAGE_SIZE + 10), rows * (VERIFY_IMAGE_SIZE + 20));
	m_rs_panel->SetFocus();

	this->SetSize(1350, 250 + rows * (VERIFY_IMAGE_SIZE + 20) + 20);
	Center();
	
}

void VerifyDlg::OnCheckboxClick(wxCommandEvent& event)
{
	wxCheckBox * label = (wxCheckBox *)event.GetEventObject();
	VerifyDlg * self = (VerifyDlg *)label->GetParent()->GetParent()->GetParent();

	wxString pre = "[";
	wxString suf = "]";
	wxString lab_str = label->GetLabel();
	pre = lab_str.substr(lab_str.find(pre) + pre.length(), lab_str.find(suf) - lab_str.find(pre) - pre.length());
	int cur_in = wxAtoi(pre);

	if (label->GetValue())
	{
		self->svr_aoi_data.Add(label->GetToolTipText());
		label->SetBackgroundColour(*wxRED);
		self->m_true_error_list.push_back(cur_in);
	}
	else
	{
		self->svr_aoi_data.Remove(label->GetToolTipText());
		label->SetBackgroundColour(label->GetParent()->GetBackgroundColour());
		for (list<int>::iterator it = self->m_true_error_list.begin(); it != self->m_true_error_list.end();)
		{
			if (*it == cur_in)
				it = self->m_true_error_list.erase(it);
			else
				++it;
		}
	}

	event.Skip();
}

void VerifyDlg::SetUseTime(wxString ins_time)
{
	m_use_time = ins_time;
	m_insp_time->SetLabel(m_use_time);
}
void VerifyDlg::SetMachineSn(wxString mac_sn)
{
	m_board_sn = mac_sn;
	m_machine_sn->SetLabel(mac_sn);
}

void VerifyDlg::SetWorkSide(int v_side)
{
	m_work_side = v_side;
	if (v_side > 0)
	{
		m_aa_left->SetBackgroundColour(*wxLIGHT_GREY);
		m_aa_right->SetBackgroundColour(*wxRED);
		return;
	}

	m_aa_left->SetBackgroundColour(*wxGREEN);
	m_aa_right->SetBackgroundColour(*wxLIGHT_GREY);
}

void VerifyDlg::SetInsTime(time_t t)
{
	m_ins_time = t;
}

void VerifyDlg::OnBtnNextClick(wxCommandEvent &)
{
	wxString msg = "用户按下下一页";
	MyLog::LogUser(msg.mb_str());

	if (!m_btn_next->IsEnabled())
		return;

	DWORD need_time = 0 * m_num4cur_page;//VERIFY_ROWS * VERIFY_COLS;
	if (GetTickCount() - m_start_verity_page < need_time)
	{
		cout << "VerifyDlg>> Please verify after request time." << endl;
		return;
	}

	if (m_cur_index + VERIFY_ROWS * VERIFY_COLS/*18*/ > m_fail_msg.size())
	{
		m_btn_next->Enable(false);
		return;
	}
	m_cur_index += VERIFY_ROWS * VERIFY_COLS/*18*/;
	ShowNextErrMsg();
}

void VerifyDlg::OnBtnPrevClick(wxCommandEvent &)
{
	wxString msg = "用户按下上一页";
	MyLog::LogUser(msg.mb_str());

	if (!m_btn_prev->IsEnabled())
		return;

	if (m_cur_index > VERIFY_ROWS * VERIFY_COLS - 1/*17*/)
	{
		m_cur_index -= VERIFY_ROWS * VERIFY_COLS/*18*/;
		if (m_cur_index < 0)
			m_cur_index = 0;
		ShowNextErrMsg();
	}
}
//查看原图
void VerifyDlg::OnBtnShowImage(wxCommandEvent &)
{
	wxString msg = "用户按下查看原图";
	MyLog::LogUser(msg.mb_str());

	if (!m_show_map_path.empty())
	{
		string sPicturePath = m_show_map_path.c_str();
		string sPicturePara = "shimgvw.dll ImageView_Fullscreen " + sPicturePath;

		ShellExecute(NULL, NULL, _("rundll32.exe"), _(sPicturePara), _("C:\\WINDOWS\\system32"), SW_SHOW);

		msg = wxString::Format("已打开图片，路径为：%s", sPicturePara);
		MyLog::LogMessage(msg.mb_str());
	}

}

void VerifyDlg::OnBtnOK(wxCommandEvent &)
{
	wxString msg = "用户按下OK按钮";
	MyLog::LogUser(msg.mb_str());

}

void VerifyDlg::OnBtnNG(wxCommandEvent &)
{
	wxString msg = "用户按下NG按钮";
	MyLog::LogUser(msg.mb_str());

}