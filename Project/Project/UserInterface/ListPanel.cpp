#include "ListPanel.h"

BEGIN_EVENT_TABLE(MyListCtrl, wxListCtrl)

END_EVENT_TABLE()

MyListCtrl::MyListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxLC_REPORT | wxLC_SORT_ASCENDING */)
	:wxListCtrl(parent, id, pos, size, style)
{
	m_frame = parent;
}
//==========================================================================================================
BEGIN_EVENT_TABLE(ListPanel, wxPanel)
	EVT_LIST_ITEM_ACTIVATED(ID_LISTCTRL, ListPanel::OnMouseLeftDouble)
	//EVT_LIST_ITEM_RIGHT_CLICK(ID_LISTCTRL, ListPanel::OnMouseRightClick)
	//EVT_MENU(ID_OPEN_FILE, ListPanel::OpenPhotoFile)
END_EVENT_TABLE()

ListPanel::ListPanel(wxWindow *parent)
	:wxPanel(parent, -1, wxDefaultPosition, wxSize(400, 200), wxBORDER_NONE)
{
	m_frame = parent;
	m_verify = NULL;
	m_map_red_rect.clear();
	m_AI_flag = false;

	m_OK_num = 0;
	m_NG_num = 0;
	m_radio_num = 0;

	m_save_path_img = "";
	m_listRowIndex = 0;
	wxBoxSizer *global_sizer = new wxBoxSizer(wxVERTICAL);

	m_mylistctrl = new MyListCtrl(this, ID_LISTCTRL, wxPoint(128, 134), wxSize(121, 97), wxLC_REPORT | wxLC_SORT_ASCENDING);
	wxFont font(14, wxSWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("微软雅黑"));

	wxArrayString cols_name;
#ifdef __LIZI__
	cols_name.Add(_("序号"));
	cols_name.Add(_("图片名"));
	cols_name.Add(_("异物结果"));
	cols_name.Add(_("异物数量"));
	cols_name.Add(_("粒子结果"));
	cols_name.Add(_("检测后图片路径"));
	cols_name.Add(_("信号发送时间"));
#else
	cols_name.Add(_("序号"));
	cols_name.Add(_("图片名"));
	cols_name.Add(_("异物结果"));
	cols_name.Add(_("异物数量"));
	cols_name.Add(_("检测后图片路径"));
	cols_name.Add(_("信号发送时间"));
#endif // __LIZI__
	for (unsigned int i = 0; i < cols_name.size(); i++)
	{
		m_mylistctrl->InsertColumn(i, cols_name[i]);
	}
#ifdef __LIZI__
	m_mylistctrl->SetColumnWidth(0, 60);
	m_mylistctrl->SetColumnWidth(1, 140);
	m_mylistctrl->SetColumnWidth(2, 100);
	m_mylistctrl->SetColumnWidth(3, 100);
	m_mylistctrl->SetColumnWidth(4, 90);
	m_mylistctrl->SetColumnWidth(5, 820);
	m_mylistctrl->SetColumnWidth(6, 210);
#else
	m_mylistctrl->SetColumnWidth(0, 60);
	m_mylistctrl->SetColumnWidth(1, 230);
	m_mylistctrl->SetColumnWidth(2, 100);
	m_mylistctrl->SetColumnWidth(3, 100);
	m_mylistctrl->SetColumnWidth(4, 810);
	m_mylistctrl->SetColumnWidth(5, 210);
#endif // __LIZi__
	m_mylistctrl->SetFont(font);
	
	global_sizer->Add(m_mylistctrl, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(global_sizer);
}

ListPanel::~ListPanel()
{
	m_map_red_rect.clear();
}

void ListPanel::AddData(wxString image_name, wxString image_result, wxString lizi_result, wxString image_NGnum, wxString image_path, wxString image_time)
{
	//static int testi = 0;
	wxArrayString cols_name;
	wxString str = "";
	str << m_listRowIndex;
#ifdef __LIZI__
	cols_name.Add(str);
	cols_name.Add(image_name);
	cols_name.Add(image_result);
	cols_name.Add(image_NGnum);
	cols_name.Add(lizi_result);
	cols_name.Add(image_path);
	cols_name.Add(image_time);
#else
	cols_name.Add(str);
	cols_name.Add(image_name);
	cols_name.Add(image_result);
	cols_name.Add(image_NGnum);
	cols_name.Add(image_path);
	cols_name.Add(image_time);
#endif
	
	m_mylistctrl->InsertItem(m_listRowIndex, "dd");
	m_mylistctrl->SetItemData(m_listRowIndex, m_listRowIndex);
	wxFont font(14, wxSWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("微软雅黑"));
	m_mylistctrl->SetFont(font);

	for (unsigned int i = 0; i < cols_name.size(); i++)
	{
		m_mylistctrl->SetItem(m_listRowIndex, i, cols_name[i]);
	}

#ifdef __LIZI__
	if ("NG" == image_result || "NG" == lizi_result)
	{
		m_mylistctrl->SetItemTextColour(testi, wxColor(255, 0, 0));
	}
#else
	if (strstr(image_result.c_str(), "NG"))
	{
		m_mylistctrl->SetItemTextColour(m_listRowIndex, wxColor(255, 0, 0));
	}
#endif


	m_listRowIndex++;

	wxString msg;
#ifdef __LIZI__
	msg = wxString::Format("添加检测信息：    图片名：%s， 异物结果：%s， 粒子结果：%s， 异物数量： %s， 检测后图片所在路径：%s， 检测时间：%s", image_name, image_result, lizi_result, image_NGnum, image_path, image_time);
#else
	msg = wxString::Format("添加检测信息：    图片名：%s， 异物结果：%s， 异物数量： %s， 检测后图片所在路径：%s， 检测时间：%s", image_name, image_result, image_NGnum, image_path, image_time);
#endif // __LIZI__

	MyLog::LogMessage(msg.mb_str());
}

void ListPanel::ModifyLiziResults(wxString file_name, bool result)
{
	int line_num = m_mylistctrl->GetItemCount();
	long list_item = m_mylistctrl->GetTopItem();

	for (int i = 0; i < line_num; i++)
	{
		wxString str_data = m_mylistctrl->GetItemText(list_item, 5);
		if (str_data == file_name)
		{
			wxString lizi_result = m_mylistctrl->GetItemText(list_item, 4);
			if ("NG" == lizi_result)
			{
				m_mylistctrl->SetItem(list_item, 4, "NG");

				wxString str_yiwu_result = m_mylistctrl->GetItemText(list_item, 2);
				wxString str_lizi_result = m_mylistctrl->GetItemText(list_item, 4);
				if (str_yiwu_result == "NG" || str_lizi_result == "NG")
				{
					m_mylistctrl->SetItemTextColour(list_item, wxColor(255, 0, 0));
				}

				break;
			}
			else
			{
				if (!result)
				{
					m_mylistctrl->SetItem(list_item, 4, "NG");
				}
				else
				{
					m_mylistctrl->SetItem(list_item, 4, "OK");
				}

				wxString str_yiwu_result = m_mylistctrl->GetItemText(list_item, 2);
				wxString str_lizi_result = m_mylistctrl->GetItemText(list_item, 4);
				if (str_yiwu_result == "NG" || str_lizi_result == "NG")
				{
					m_mylistctrl->SetItemTextColour(list_item, wxColor(255, 0, 0));
				}
			}
		}
		list_item = m_mylistctrl->GetNextItem(list_item);
	}
}

void ListPanel::GetOkNgRadioData()
{
	int line_num = m_mylistctrl->GetItemCount();
	long list_item = m_mylistctrl->GetTopItem();

	m_OK_num = 0;
	m_NG_num = 0;
	m_radio_num = 0;

	for (int i = 0; i < line_num; i++)
	{
		wxString yiwu_result = m_mylistctrl->GetItemText(list_item, 2);
		if ("NG" == yiwu_result)
		{
			m_NG_num++;
		}
		else
		{
			m_OK_num++;
		}
	}

	m_radio_num = (float)m_NG_num / ((float)m_OK_num + (float)m_NG_num);
}

void ListPanel::ClearListData()
{
	m_mylistctrl->DeleteAllItems();
}

void ListPanel::OnMouseLeftDouble(wxListEvent &event)
{
	wxString msg = "用户双击检测信息行，正在显示详细检测信息";
	MyLog::LogUser(msg.mb_str());

	wxListItem wli_item = event.GetItem();
	wxString temp_image_name = m_mylistctrl->GetItemText(wli_item, 1);
#ifdef __LIZI__
	wxString temp_image_path = m_mylistctrl->GetItemText(wli_item, 5);
#else
	wxString temp_image_path = m_mylistctrl->GetItemText(wli_item, 4);
#endif // __LIZI__
	wxString temp_Find_path;

	map<string, list<wxRect>>::iterator iter;
	map<string, list<wxRect>> temp_map;
	temp_map = m_map_red_rect;
	string temp_str = temp_image_name.c_str();
	iter = temp_map.find(temp_str);

	char *t;
	string t_s_image_name = temp_image_name.c_str();
	t = (char *)malloc((t_s_image_name.length() + 1) * sizeof(char*));
	strcpy_s(t, t_s_image_name.length() + 1, t_s_image_name.c_str());
	char *p = strrchr(t, '\\');
	string s_t;
	string s_tt;
	if (*p != NULL)
	{
		s_t = p;
#ifdef __STATE_ELICITATION__
		temp_Find_path = temp_image_path.SubString(0, temp_image_path.size() - s_t.size());
#else
		s_tt = s_t;
		int s_t_num = s_tt.size();
		s_tt = s_tt.substr(1, s_t_num - 5);
#endif // __STATE_ELICITATION__
	}
	else
	{	
		msg = "用户双击检测信息行----末位不为\\";
		MyLog::LogWarning(msg.c_str());
	}
#ifndef __STATE_ELICITATION__
		
	wxString wxs_now_time = m_mylistctrl->GetItemText(wli_item, 5);
	wxs_now_time = wxs_now_time.substr(0, 4) + wxs_now_time.substr(5, 2) + wxs_now_time(8, 2);

	s_tt += "-HasBeenTesting";
	wxString directory_name = "";
	directory_name = temp_image_name.substr(0, temp_image_name.find_last_of('\\'));
	directory_name = directory_name.substr(directory_name.find_last_of('\\') + 1);
	wxString l_temp = s_tt.c_str();
	int l_index1 = l_temp.Find("-");
	wxString l_temp1 = l_temp.SubString(0,l_index1-1);

	if (m_AI_flag)
		temp_Find_path = m_save_path_img + "\\Result_Image\\AI_Img\\" + wxs_now_time + "\\" + directory_name + "\\" + s_tt.c_str();
	else
		temp_Find_path = m_save_path_img + "\\Result_Image\\Arithmetic_Img\\" + wxs_now_time + "\\" + directory_name + "\\" + s_tt.c_str();

	wxString l_mgs = wxString::Format("图片所在目录:%s", temp_Find_path);
	MyLog::LogMessage(msg.c_str());
#endif // __STATE_ELICITATION__

	wxArrayString img_list;
	wxDir::GetAllFiles(temp_Find_path, &img_list, wxT("*.jpg"), wxDIR_FILES);

	list<s_verify> list_data;
	int i = 1;

	list<wxRect>::iterator it;
	if (iter != temp_map.end())
	{
		it = iter->second.begin();
	}

	s_t = s_t.substr(1, 13);
	for (size_t i_img = 0; i_img < img_list.size(); i_img++)
	{
		wxString temp_find_image_name = img_list[i_img].substr(temp_Find_path.size(), img_list[i_img].size());
		if (!strstr(temp_find_image_name, s_t.c_str()))
		{
			if (m_AI_flag)
			{
				if (strstr(temp_find_image_name, "AI-"))
				{
					s_verify sy;
					sy.m_pos_code = temp_find_image_name;
					sy.m_rect = wxRect(100 * i, 100 * i, 500, 500);
					sy.m_img = wxImage(img_list[i_img]);
					if (iter != temp_map.end())
					{
						sy.m_ng_rect = wxRect(it->x, it->y, it->width, it->height);
					}
					else
					{
						sy.m_ng_rect = wxRect(0, 0, 0, 0);
					}
					sy.m_pos_x = 0;
					sy.m_err_num = 1;
					sy.m_len = 0;
					sy.m_line_name = "X10101";
					sy.m_pos_y = 0;
					sy.m_type = 1;

					i++;
					it++;

					list_data.push_back(sy);
				}
			}
			else
			{
				if (strstr(temp_find_image_name, "yw-"))
				{
					s_verify sy;
					sy.m_pos_code = temp_find_image_name;
					sy.m_rect = wxRect(100 * i, 100 * i, 500, 500);
					sy.m_img = wxImage(img_list[i_img]);
					if (iter != temp_map.end())
					{
						sy.m_ng_rect = wxRect(it->x, it->y, it->width, it->height);
					}
					sy.m_pos_x = 0;
					sy.m_err_num = 1;
					sy.m_len = 0;
					sy.m_line_name = "X10101";
					sy.m_pos_y = 0;
					sy.m_type = 1;

					i++;
					it++;

					list_data.push_back(sy);
				}
			}
		}
	}

	if (m_verify)
	{
		delete m_verify;
		m_verify = NULL;
	}
	temp_Find_path += s_t.c_str();

	m_verify = new VerifyDlg(m_frame);
	m_verify->SetShowImagePath(temp_image_path);
	m_verify->SetTestResult(list_data, 0);
#ifdef __LIZI__
	m_verify->SetUseTime(m_mylistctrl->GetItemText(wli_item, 6));
#else
	m_verify->SetUseTime(m_mylistctrl->GetItemText(wli_item, 5));
#endif // __LIZI__
	m_verify->SetMachineSn(temp_image_name);
	m_verify->SetWorkSide(list_data.size());
	m_verify->SetInsTime(time(NULL));
	m_verify->Show();

	msg = wxString::Format("已打开图片：%s的详细检测信息", temp_image_name);
	MyLog::LogMessage(msg.mb_str());
}
//右键菜单
void ListPanel::OnMouseRightClick(wxListEvent &event)
{
	m_itemid = event.GetItem();

	wxPoint point = event.GetPoint();
	wxPoint screenpt = ClientToScreen(point);

	wxMenu menu;
	menu.Append(ID_OPEN_FILE, _("&打开检测文件所在路径"));
	PopupMenu(&menu, point.x, point.y);
	event.Skip();

	wxString msg = "右键：打开检测文件所在路径";
	MyLog::LogUser(msg.mb_str());
	
	return;
}

void ListPanel::OpenPhotoFile(wxCommandEvent &)
{
	wxString temp_image_name = m_mylistctrl->GetItemText(m_itemid, 1);
#ifdef __LIZI__
	wxString temp_image_path = m_mylistctrl->GetItemText(m_itemid, 6);
#else
	wxString temp_image_path = m_mylistctrl->GetItemText(m_itemid, 5);
#endif // __LIZI__
	
	wxString temp_Find_path;

	char *t;
	string t_s_image_name = temp_image_name.c_str();
	t = (char *)malloc((t_s_image_name.length() + 1) * sizeof(char*));
	strcpy_s(t, t_s_image_name.length() + 1, t_s_image_name.c_str());
	char *p = strrchr(t, '\\');
	string s_t;
	if (*p != NULL)
	{
		s_t = p;
		temp_Find_path = temp_image_path.SubString(0, temp_image_path.size() - s_t.size());
	}
	else
	{
		temp_Find_path = temp_image_path.SubString(0, temp_image_path.size() - temp_image_name.size() - 2);
	}

	wxString temp_Open_path = "start \"\" \"" + temp_Find_path;
	system(temp_Open_path.c_str());

	wxString msg = wxString::Format("打开路径：%s", temp_Open_path);
	MyLog::LogMessage(msg.mb_str());
}

void ListPanel::SetMapRedRect(map<string, list<wxRect>> map_red_rect)
{
	m_map_red_rect = map_red_rect;
}