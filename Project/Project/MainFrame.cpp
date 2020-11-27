#include "MainFrame.h"


BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	//Menu
	EVT_MENU(ID_MENU_LOAD, MainFrame::OnLoadPhoto)
	EVT_MENU(ID_MENU_READ_RECIPE, MainFrame::OnReadRecipe)
	EVT_MENU(ID_MENU_PATH, MainFrame::OnSetDetectionPath)
	EVT_MENU(ID_MENU_PARAMETER, MainFrame::OnSetParemeter)
	EVT_MENU(ID_MENU_ADDR, MainFrame::OnSetAddr)
	EVT_MENU(ID_MENU_DRAW_RECT, MainFrame::OnDrawRect)
	EVT_MENU(ID_MENU_DRAW_SQUARE_RECT, MainFrame::OnDrawSquareRect)
	EVT_MENU(ID_MENU_FILE_OPEN_ALGORITHM_IMG, MainFrame::OnOpenFileAlgorithmImg)
	EVT_MENU(ID_MENU_FILE_OPEN_AI_IMG, MainFrame::OnOpenFileAIImg)
	EVT_MENU(ID_MENU_FILE_OPEN_EXCEL, MainFrame::OnOpenFileExcel)
	EVT_MENU(ID_MENU_FILE_OPEN_NOW_EXCEL, MainFrame::OnOpenFileNowExcel)
	EVT_MENU(ID_MENU_FILE_OPEN_RECORD_EXCEL, MainFrame::OnOpenFileRecordExcel)
	//Control
	EVT_BUTTON(ControlPanel::ID_BTN_CONNECT, MainFrame::OnConnectSocket)
	EVT_BUTTON(ControlPanel::ID_BTN_CONNECT_AI, MainFrame::OnConnectAI)
	EVT_BUTTON(ControlPanel::ID_BTN_DETECTION, MainFrame::OnDetection)
	EVT_BUTTON(ControlPanel::ID_BTN_SELF_MOTION, MainFrame::OnSelfMotion)
	//Refresh
	EVT_BUTTON(Refreshwindow::ID_REFRESH_ADD_MODEL, MainFrame::OnAddModel)
	EVT_BUTTON(ModelPanelWindow::ID_BTN_CLEAR, MainFrame::OnClearModel)
	EVT_BUTTON(ModelPanelWindow::ID_BIN_SAVE, MainFrame::OnSaveModel)

	EVT_THREAD(SocketCenter::ID_READ_AND_DECTION, MainFrame::OnReadAndDection)
	EVT_THREAD(ImageModel::ID_SEND_READ_SINGLE, MainFrame::OnSendReadSingle)

	EVT_THREAD(SocketCenter::ID_DELETE_SOCKET, MainFrame::OnDeleteSocket)
	EVT_THREAD(ImageModel::ID_CREATE_REJUDE_UI, MainFrame::OnCreateRejUi)
	EVT_THREAD(AutomaticDetetionThread::ID_NO_PIC, MainFrame::OnNoPic)
	EVT_THREAD(AutomaticDetetionThread::ID_SEND_NG_SINGLE, MainFrame::OnSendNGSingle)
	EVT_CLOSE(MainFrame::OnClose)
	EVT_SUPERDOG(-1, MainFrame::OnSuperDogShow)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_SUPER_DOG)
MainFrame::MainFrame(wxWindow* parent, wxWindowID id /* = -1 */, const wxString& title /* = _("Project") */, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = MainFrame_STYLE */)
	:wxFrame(parent, id, title, pos, size, style), m_iClientX(0), m_iClientY(0), m_iMyScrolled_Width(0), m_iMyScrolled_Height(0), m_recipe_name(""), m_recipe_path("")
{
	AllocConsole();							//调用进程分配一个新的控制台
	freopen("CON", "r", stdin);				//用于重定向输入输出流。该函数可以在不改变代码原貌的情况下改变输入输出环境，
	freopen("CON", "w", stdout);			//但使用时应当保证流是可靠的。
	freopen("CON", "w", stderr);			//这四句话保证了cout输出可以传到控制台上

	wxString msg = "=========================================================================================================================";
	MyLog::LogMessage(msg.mb_str());
	msg = "开启软件";
	MyLog::LogMessage(msg.mb_str());

	m_AutomaticThread = NULL;

	m_verifyDlg = NULL;
	m_is_draw_rect = false;

 	m_gProfile = Profile::Instance();
 	m_gProfile->loadIni("Ini.ini");

	m_gModel = NULL;

	CreateGUIControls();
	Init();

	m_mutex = new wxMutex();

}

MainFrame::~MainFrame()
{
	m_mgr.UnInit();

	if (m_verifyDlg)
	{
		delete m_verifyDlg;
	}

	if (m_gModel)
	{
		delete m_gModel;
	}

	if (m_gProfile != NULL)
	{
		delete m_gProfile;
		m_gProfile = NULL;
	}

	if (m_refresh_window)
	{
		delete m_refresh_window;
	}

	if (m_mutex)
	{
		delete m_mutex;
	}

	if (m_listData)
	{
		delete m_listData;
		m_listData = nullptr;
	}
#ifdef __CHECK_SUPER_DOG__
	if (m_superdog)
	{
		delete m_superdog;
	}
#endif // __CHECK_SUPER_DOG__

	FreeConsole();
}

void MainFrame::CreateGUIControls()
{
	wxMessageDialog dlg(this, _("请选择当前工作模式："), _("设置"), wxYES_NO | wxCANCEL);
	dlg.SetYesNoLabels(_("在线"), _("脱机"));
	int i_status = dlg.ShowModal();
	if (i_status == wxID_YES)
	{
		PLAN_ONE = true;
	}
	//离线
	else if (i_status == wxID_NO)
	{
		PLAN_ONE = false;
	}

	else
	{
		dlg.Destroy();
		ExitSystem(false);
	}


	wxInitAllImageHandlers();

	m_mgr.SetManagedWindow(this);

	m_menu_bar = new wxMenuBar(wxMB_DOCKABLE);

	//Load
	m_menu_load = new wxMenu;
	m_menu_load->Append(ID_MENU_LOAD, _("载入图像"));
	m_menu_load->Append(ID_MENU_READ_RECIPE, _("读取配方"));
	m_menu_bar->Append(m_menu_load, ("载入"));

	//CHANGE
	m_menu_set = new wxMenu;
	m_menu_set->Append(ID_MENU_PATH, _("自动检测设置"));
	m_menu_set->Append(ID_MENU_PARAMETER, _("设置算法参数"));
	m_menu_set->Append(ID_MENU_ADDR, _("设置读取/写入地址"));
	m_menu_bar->Append(m_menu_set, _("设置"));

	//Draw
	m_menu_draw = new wxMenu;
	m_menu_draw->AppendCheckItem(ID_MENU_DRAW_RECT, _("模板"));
	m_menu_draw->AppendCheckItem(ID_MENU_DRAW_SQUARE_RECT, _("正方形模板"));
	m_menu_draw->Check(ID_MENU_DRAW_RECT, false);
	m_menu_draw->Check(ID_MENU_DRAW_SQUARE_RECT, false);
	m_menu_bar->Append(m_menu_draw, _("制作模板"));

	//File
	m_menu_file = new wxMenu;
	m_menu_file->Append(ID_MENU_FILE_OPEN_ALGORITHM_IMG, _("打开算法检测结果目录"));
	m_menu_file->Append(ID_MENU_FILE_OPEN_AI_IMG, _("打开AI检测结果目录"));
	m_menu_file->AppendSeparator();
	m_menu_file->Append(ID_MENU_FILE_OPEN_EXCEL, _("打开报表目录"));
	m_menu_file->AppendSeparator();
	m_menu_file->Append(ID_MENU_FILE_OPEN_NOW_EXCEL, _("打开当前时间段报表"));
	m_menu_file->Append(ID_MENU_FILE_OPEN_RECORD_EXCEL, _("打开汇总报表"));
	m_menu_bar->Append(m_menu_file, _("文件"));

	this->SetMenuBar(m_menu_bar);

	SetMinSize(wxSize(600, 400));
	Center();

	wxDisplay Screen;										//获取并设置系统的显示器的大小和位置
	wxRect ClientRect = Screen.GetClientArea();
	this->SetSize(ClientRect);

	this->GetClientSize(&m_iClientX, &m_iClientY);

	m_iMyScrolled_Width = m_iClientX;
	m_iMyScrolled_Height = m_iClientY / 2;

	m_iDown_Width = m_iClientX / 10 * 8;
	m_iDown_Height = m_iClientY / 2;

	//Main panel show
	m_refresh_window = new Refreshwindow(this, m_iMyScrolled_Width, m_iMyScrolled_Height);
	m_list_panel = new ListPanel(this);
	m_control_panel = new ControlPanel(this);
	m_result_panel = new ResultDlgPanel(this, m_gProfile);

	bool flag = true;

	m_mgr.AddPane(m_refresh_window, wxAuiPaneInfo().CenterPane().Layer(0).Row(0).Position(0).CloseButton(false));
	m_mgr.AddPane(m_list_panel, wxAuiPaneInfo().Caption(_("检测信息")).Top().Layer(1).Row(1).Position(0).CloseButton(false).MaximizeButton(true).MinSize(m_iDown_Width, m_iDown_Height).Show(flag));
	m_mgr.AddPane(m_result_panel, wxAuiPaneInfo().Caption(_("检测结果")).Top().Layer(1).Row(1).Position(1).CloseButton(false).MaximizeButton(true).MinSize(m_iClientX / 10, m_iClientY / 2).Show(flag));
	m_mgr.AddPane(m_control_panel, wxAuiPaneInfo().Caption(_("控制")).Top().Layer(1).Row(1).Position(2).CloseButton(false).MaximizeButton(true).MinSize(m_iClientX / 10 * 2, m_iClientY / 2).Show(flag));
	m_mgr.Update();

	RefreshPreviousData();
}

bool MainFrame::Init()
{
#ifdef __CHECK_SUPER_DOG__
	m_superdog = new SuperDog(this, 1);
	if (!m_superdog->CheckDog())
	{
		wxMessageBox(_("Check dog error:") + _(m_superdog->GetLastError()));
		ExitSystem(false);
		return false;
	}
	else
	{
		m_superdog->StartCheckDog();
	}
#endif // __CHECK_SUPER_DOG__

	m_gModel = ImageModel::GetInstance(this);
	m_gModel->SetImageModelWindow(m_refresh_window);
	m_gModel->SetListWindow(m_list_panel);
	m_gModel->SetControlWindow(m_control_panel);
	m_gModel->SetResultWinodw(m_result_panel);
	m_gModel->SetProfile(m_gProfile);
	m_gModel->SetListData(m_listData);

	wxString str = "MainFrame初始化完成";
	MyLog::LogMessage(str.mb_str());
	str = wxString::Format("当前设置IP：%s, 端口：%d, 自动检测路径：%s, CSV路径：%s",m_gModel->GetSaveIP(), m_gModel->GetSavePort(), m_gModel->GetSavePath(), m_gModel->GetSavePathCsv());
	MyLog::LogMessage(str.mb_str());
	str = wxString::Format("当前算法参数为：IC：%d, %d", m_gModel->GetSaveICbinaryValue(), m_gModel->GetSaveICmedianBlurSize());
	MyLog::LogMessage(str.mb_str());
	str = wxString::Format("当前读数据地址：%d, 类型：%s, 写OK地址：%d, 类型：%s,  写NG地址：%d, 类型：%s", m_gModel->GetReadAddr(), m_gModel->GetReadType(), m_gModel->GetWriteOKAddr(), m_gModel->GetWriteOKType(), m_gModel->GetWriteNGAddr(), m_gModel->GetWriteNGType());
	MyLog::LogMessage(str.mb_str());

	return true;
}

void MainFrame::OnClose(wxCloseEvent&)
{
	this->ExitSystem(true);
}

void MainFrame::ExitSystem(bool flag)
{
	wxString msg = "";
	if (flag)
	{
		msg = "用户按下退出按钮";
		MyLog::LogUser(msg.mb_str());

		wxMessageDialog dlg(this, _("确认退出系统？"), _("Prompt"), wxYES_NO);
		if (dlg.ShowModal() != wxID_YES)
		{
			msg = "取消退出";
			MyLog::LogMessage(msg.mb_str());

			dlg.Destroy();
			return;
		}
		msg = "确认退出";
		MyLog::LogMessage(msg.mb_str());
		dlg.Destroy();

		if (NULL != m_gModel->GetSocketCenter())
		{
			if (true == m_gModel->GetSocketCenter()->GetRunFlag())
			{
				m_gModel->GetSocketCenter()->SetRunFlag(false);

				if (m_gModel->GetSocketCenter()->IsAlive())
				{
					m_gModel->DeleteSocketThread();
				}

				msg = "Socket检测已取消，销毁线程";
				MyLog::LogMessage(msg.mb_str());
			}
		}
		if (true == m_gModel->GetTestingStartFlag())
		{
			msg = "自动检测已取消";
			MyLog::LogMessage(msg.mb_str());
			m_AutomaticThread->SetRunFlag(false);

			Sleep(10);
			m_AutomaticThread = NULL;

			msg = "自动检测已取消，销毁线程";
			MyLog::LogMessage(msg.mb_str());
		}
	}

	msg = "正常退出系统";
	MyLog::LogMessage(msg.mb_str());

	Destroy();
}

void MainFrame::RefreshPreviousData()
{
	//wxDateTime l_dt = wxDateTime::Now();
	//wxString l_dtStr = l_dt.Format("%Y-%m-%d");

	//wxString l_preDt = m_gProfile->GetDateTime();
	//if (l_dtStr != l_preDt)
	//{
	//	m_gProfile->SetDateTime(l_dtStr);
	//	m_gProfile->SetOkNum(0);
	//	m_gProfile->SetNgNum(0);
	//	m_gProfile->SetRatio(0);
	//}

	int l_previousOkNum = m_gProfile->GetOkNum();
	m_result_panel->SetEditOK(l_previousOkNum);

	int l_previousNgNum = m_gProfile->GetNgNum();
	m_result_panel->SetEditNG(l_previousNgNum);

	float l_previousRatio = m_gProfile->GetRatio();
	m_result_panel->SetEditRatio(l_previousRatio);

	
	wxString l_loadPath = "./listData.dat";
	string l_strPath = string(l_loadPath.mb_str());
	if (_access(l_loadPath, 0) == -1)
	{
		m_listData = new ListData;
		m_listData->OnSave(l_strPath);
	}
	else
	{
		m_listData = ListData::OnLoad(l_strPath);
	}
	
	//m_listData->ClearData();
	//m_listData->OnSave(l_strPath);

	std::vector<ImageInfo> l_infoVec = m_listData->GetListInfo();
	for (ImageInfo info: l_infoVec)
	{
		string l_name = info.imageName;
		wxString l_result = wxString(info.imageResult.c_str(),wxConvUTF8);
		wxString l_ngNum = wxString(info.imageNgNum.c_str(), wxConvUTF8);
		wxString l_path = wxString(info.imagePath.c_str(), wxConvUTF8);
		wxString l_time = wxString(info.imageTime.c_str(), wxConvUTF8);
		m_list_panel->AddData(l_name, l_result, "", l_ngNum, l_path, l_time);
	}

	std::map<string, list<wxRect>> l_mapRect = m_listData->GetRedRectMap();
	m_list_panel->SetMapRedRect(l_mapRect);
}

//Menu
//载入图像
void MainFrame::OnLoadPhoto(wxCommandEvent &)
{
	wxString msg = "用户按下载入图像按钮";
	MyLog::LogUser(msg.mb_str());

	if (true == m_gModel->GetTestingStartFlag())
	{
		msg = "自动线程正在启动中，请先停止后，在重新载入图像";
		MyLog::LogWarning(msg.mb_str());
		wxMessageBox(msg, _("Warning"));
		return;
	}

	wxFileDialog dlg(this, _("载入图像"), "", "", "JPG files (*.jpg)|*.jpg|BMP files (*.bmp)|*.bmp", wxFD_OPEN);//这里有内存泄漏
	if (wxID_OK == dlg.ShowModal())
	{
		string path = dlg.GetPath();
		string name = dlg.GetFilename();

		m_gModel->OnLoadImage(cv::String(path), name);

		msg = "载入图像成功";
		MyLog::LogMessage(msg.mb_str());
	}
	else
	{
		msg = "取消载入图像";
		MyLog::LogMessage(msg.mb_str());

		dlg.Destroy();
		return;
	}
	dlg.Destroy();
	m_refresh_window->SetFocus();
}
//读取配方
void MainFrame::OnReadRecipe(wxCommandEvent &event)
{
	wxString msg = "用户按下读取配方按钮";
	MyLog::LogUser(msg.mb_str());

	wxFileDialog dlg(this, _("选择配方"), "./recipe", "", "dat files (*.dat) | *.dat", wxFD_FILE_MUST_EXIST | wxFD_OPEN);
	if (wxID_OK == dlg.ShowModal())
	{
		m_recipe_name = dlg.GetFilename().c_str();
		m_recipe_path = dlg.GetPath().c_str();

		msg = wxString::Format("确认配方为：%s, 路径为：%s", m_recipe_name, m_recipe_path);
		MyLog::LogMessage(msg.mb_str());
	}
	else
	{
		msg = "用户取消选择配方";
		MyLog::LogMessage(msg.mb_str());

		dlg.Destroy();
		return;
	}
	dlg.Destroy();

	msg = "自动删除配方（删除原配方才能导入新配方）";
	MyLog::LogMessage(msg.mb_str());
	this->OnClearModel(event);

	DataOrganization *d = DataOrganization::OnLoad(m_recipe_name, m_recipe_path);
	m_gModel->SetDataorganization(d, m_recipe_name, m_recipe_path);

	wxMessageBox("读取成功");

	msg = "配方读取成功";
	MyLog::LogMessage(msg.mb_str());
}
//修改IP 端口 路径
void MainFrame::OnSetDetectionPath(wxCommandEvent &)
{
	wxString msg = "用户按下设置IP 端口 路径";
	MyLog::LogUser(msg.mb_str());
	m_gModel->OnSetDetectionPath();
}
//修改算法参数
void MainFrame::OnSetParemeter(wxCommandEvent &)
{
	wxString msg = "用户按下设置算法参数";
	MyLog::LogUser(msg.mb_str());
	m_gModel->OnSetParemeter();
}
//修改socket读取写入地址
void MainFrame::OnSetAddr(wxCommandEvent &)
{
	wxString msg = "用户按下设置读取/写入地址";
	MyLog::LogUser(msg.mb_str());
	m_gModel->OnSetAddr();
}

//画框
void MainFrame::OnDrawRect(wxCommandEvent &event)
{
	if (event.GetSelection())
	{
		m_is_draw_rect = true;
		m_gModel->SetDrawRect(m_is_draw_rect ,1);
	}
	else
	{
		m_is_draw_rect = false;
		m_gModel->SetDrawRect(m_is_draw_rect, 1);
	}
}

void MainFrame::OnDrawSquareRect(wxCommandEvent &event)
{
	if (event.GetSelection())
	{
		m_is_draw_rect = true;
		m_gModel->SetDrawRect(m_is_draw_rect, 2);
	}
	else
	{
		m_is_draw_rect = false;
		m_gModel->SetDrawRect(m_is_draw_rect, 2);
	}
}

void MainFrame::OnOpenFileAlgorithmImg(wxCommandEvent &)
{
	m_gModel->OnOpenFileAlgorithmImg();
}

void MainFrame::OnOpenFileAIImg(wxCommandEvent &)
{
	m_gModel->OnOpenFileAIImg();
}

void MainFrame::OnOpenFileExcel(wxCommandEvent &)
{
	m_gModel->OnOpenFileExcel();
}

void MainFrame::OnOpenFileNowExcel(wxCommandEvent &)
{
	m_gModel->OnOpenFileNowExcel();
}

void MainFrame::OnOpenFileRecordExcel(wxCommandEvent &)
{
	m_gModel->OnOpenFileRecordExcel();
}

//Control
void MainFrame::OnConnectSocket(wxCommandEvent &)
{
	wxString msg = "用户按下连接/停止 PLC(socket)按钮";
	MyLog::LogUser(msg.mb_str());
	m_gModel->OnConnectSocket();
}
void MainFrame::OnConnectAI(wxCommandEvent &)
{
	wxString msg = "用户按下连接AI/AI已连接/AI连接失败 按钮";
	MyLog::LogUser(msg.mb_str());
	m_gModel->OnConnectAI();
}
void MainFrame::OnDetection(wxCommandEvent &)
{
 	wxString msg = "用户按下检测按钮";
 	MyLog::LogUser(msg.mb_str());
	
	cv::Mat temp_cvSrcmat = m_gModel->GetCvSrcmat();
	string temp_image_name1 = m_gModel->GetImageName();
	string temp_image_path = m_gModel->GetImagePath();
	int temp_mat_width = m_gModel->GetMatWidth();
	int temp_mat_height = m_gModel->GetMatHeight();

// 	m_gModel->OnDetection(false, temp_cvSrcmat, temp_image_name1, temp_image_path, temp_mat_width, temp_mat_height);
 
 	if (m_verifyDlg)
 	{
 		delete m_verifyDlg;
 	}
 
 	msg = "正在生成检测后的缺陷详细信息";
 	MyLog::LogMessage(msg.mb_str());
 

 	wxString wxs_now_time;
 	wxs_now_time = m_gModel->GetNowTime(1).c_str();
 
 	wxString temp_image_name = m_gModel->GetImageName().c_str();
 	temp_image_name = temp_image_name.substr(0, temp_image_name.size() - 5);
 		
 	m_verifyDlg = new VerifyDlg(this);
 	m_verifyDlg->SetShowImagePath(m_gModel->GetShowPath());
 	m_verifyDlg->SetTestResult(m_gModel->GetListData(), 0);
 	m_verifyDlg->SetUseTime(wxs_now_time);
 	m_verifyDlg->SetMachineSn(m_gModel->GetImageName());
 	m_verifyDlg->SetWorkSide(m_gModel->GetListData().size());
 	m_verifyDlg->SetInsTime(time(NULL));
 	m_verifyDlg->Show();
 
 	msg = "VerifyDlg缺陷界面生成成功";
 	MyLog::LogMessage(msg.mb_str());
}
void MainFrame::OnSelfMotion(wxCommandEvent &)
{
	wxString msg = "用户按下自动检测/停止按钮";
	MyLog::LogUser(msg.mb_str());

	bool flag = m_gModel->OnSelfMotion();
	if (!flag)
	{
		return;
	}

	m_gModel->GetControlWindow()->SetBtnStatus(false);

	if (true == m_gModel->GetTestingStartFlag())
	{
		m_gModel->GetControlWindow()->SetSelfMotionBtnStatus(true);

		if (NULL != m_AutomaticThread)
		{
			m_AutomaticThread = NULL;
		}
		m_AutomaticThread = new AutomaticDetetionThread(this);
		m_AutomaticThread->SetImageModel(m_gModel);
		m_AutomaticThread->Create();
		m_AutomaticThread->Run();

		if (PLAN_ONE)
		{
			m_gModel->PlanOneAutoBtnFlag(true);
		}

		msg = "已开启自动检测线程";
		MyLog::LogMessage(msg.mb_str());
	}
	else
	{
		if (PLAN_ONE)
		{
			m_gModel->PlanOneAutoBtnFlag(false);
		}

		m_AutomaticThread->SetRunFlag(false);

		m_gModel->GetControlWindow()->SetBtnStatus(false);

		Sleep(3000);

		msg = "已停止自动检测线程";
		MyLog::LogMessage(msg.mb_str());

		m_gModel->GetControlWindow()->SetSelfMotionBtnStatus(false);
	}
}

void MainFrame::OnCreateRejUi(wxThreadEvent &)
{
	m_mutex->Lock();

	if (m_verifyDlg)
	{
		delete m_verifyDlg;
		m_verifyDlg = nullptr;
	}

	wxString msg = "正在生成检测后的缺陷详细信息";
	MyLog::LogMessage(msg.mb_str());

	wxString wxs_now_time;
	wxs_now_time = m_gModel->GetNowTime(1).c_str();

	wxString temp_image_name = m_gModel->GetImageName().c_str();
	temp_image_name = temp_image_name.SubString(0, temp_image_name.size() - 5);

	wxString l_result = m_gModel->GetImageResult();
	if (l_result == "NG")
	{
		m_verifyDlg = new VerifyDlg(this);
		m_verifyDlg->SetShowImagePath(m_gModel->GetShowPath());
		m_verifyDlg->SetTestResult(m_gModel->GetListData(), 0);
		m_verifyDlg->SetUseTime(wxs_now_time);
		m_verifyDlg->SetMachineSn(m_gModel->GetImageName());
		m_verifyDlg->SetWorkSide(m_gModel->GetListData().size());
		m_verifyDlg->SetInsTime(time(NULL));
		m_verifyDlg->Show();
	}


	msg = "VerifyDlg缺陷界面生成成功";
	MyLog::LogMessage(msg.mb_str());

	if (m_gModel->GetListData().size())
	{
		m_gModel->OnSaveFailurePics(false, m_gModel->GetListData().size());
		m_gModel->OnSaveRecord(false, m_gModel->GetListData().size());
	}

	else
	{
		m_gModel->OnSaveFailurePics(true, m_gModel->GetListData().size());
		m_gModel->OnSaveRecord(true, m_gModel->GetListData().size());
	}
		

	m_mutex->Unlock();

	m_gModel->ClearListData();
}

//读取信号后开启检测	读图并检测
void MainFrame::OnReadAndDection(wxThreadEvent &)
{
	if (PLAN_ONE)
	{
		int temp_type = m_gModel->GetSocketCenter()->GetType();
		int temp_single = m_gModel->GetSingle();
		if (temp_single == temp_type - 1)
		{
			m_AutomaticThread->SetType(temp_type);
			m_AutomaticThread->SetPlanOneRunFlag(true);

			wxString msg = wxString::Format("已收到检测信号%d, 开启自动检测线程", temp_type);
			MyLog::LogMessage(msg.mb_str());
		}
		else
		{
			wxString msg = wxString::Format("接收到异常检测信号%d, 请重新确认信号!!!!!!!!!!!!!!!!!", temp_type);
			MyLog::LogError(msg.mb_str());
		}
	}
}
//删除Socket
void MainFrame::OnDeleteSocket(wxThreadEvent &)
{
	if (NULL != m_AutomaticThread)
	{
		m_AutomaticThread->SetRunFlag(false);
		Sleep(10);
		m_AutomaticThread = NULL;
	}

	m_gModel->DeleteSocketThread();
	m_gModel->GetControlWindow()->SetConnectBtnStatus(true);
	m_gModel->SetTestingStartFlag(false);

	wxString msg = "删除socket线程及自动检测线程";
	MyLog::LogMessage(msg.mb_str());
}
//发送读信号
void MainFrame::OnSendReadSingle(wxThreadEvent &)
{
	if (PLAN_ONE)
	{
		m_gModel->GetSocketCenter()->SetBtnAutoFlag(true);
		if (m_AutomaticThread->GetPlanOneRunFlag())
		{
			m_AutomaticThread->SetPlanOneRunFlag(false);
		}
		wxString msg = "发送读取信号";
		MyLog::LogMessage(msg.mb_str());
	}
}

//添加模板
void MainFrame::OnAddModel(wxCommandEvent &)
{
	m_gModel->OnAddModel();
}
//清空模版
void MainFrame::OnClearModel(wxCommandEvent &)
{
	m_gModel->OnClearModel();
}
//保存模版
void MainFrame::OnSaveModel(wxCommandEvent &)
{
	m_gModel->OnSaveModel();
}

void MainFrame::OnNoPic(wxThreadEvent &)
{
	m_gModel->OnNoPic();
}

void MainFrame::OnSendNGSingle(wxThreadEvent &)
{
	m_gModel->OnNGSingle();
}

void MainFrame::OnSuperDogShow(SuperDogEvent &event)
{
	wxMessageBox("加密狗被拔出，程序自动退出!");
	ExitSystem(false);
}