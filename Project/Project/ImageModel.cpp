#include "ImageModel.h"
#include "./UserInterface/NoTestPanel.h"

ImageModel *ImageModel::m_instance = NULL;
wxWindow			*ImageModel::m_frame;
wxMutex		*ImageModel::m_mutex;
Refreshwindow		*ImageModel::m_refresh_window;
ListPanel			*ImageModel::m_list_window;
ResultDlgPanel		*ImageModel::m_resultdlg_winodw;
SocketCenter		*ImageModel::m_socket_center;
list<s_verify>		ImageModel::m_list_data;
map<string, list<wxRect>> ImageModel::m_map_red_rect;
wxString			ImageModel::m_show_img_string;
list<paper_file>	ImageModel::m_list_paper_file;
list<Image_Properties> ImageModel::m_list_image_model;
vector<NoTestImage_t> ImageModel::m_noTestImageVec;
vector<cv::Rect> ImageModel::vecter_result;
wxString ImageModel::wxs_NG_num;
vector<wxRect> ImageModel::wx_vector_list;

int					ImageModel::m_paper_file_num;
int					ImageModel::m_save_ic_binaryValue;
int					ImageModel::m_save_ic_backValue;
int					ImageModel::m_save_ic_medianBlurSize;
int					ImageModel::m_save_write_OK_addr;
wxString			ImageModel::m_save_write_OK_type;
int					ImageModel::m_save_write_NG_addr;
wxString			ImageModel::m_save_write_NG_type;
bool				ImageModel::m_save_is_use_ai;
wxString			ImageModel::m_save_ai_path;
cv::Mat				ImageModel::m_cv_srcmat;
string				ImageModel::m_image_path;
string				ImageModel::m_image_name;
int					ImageModel::m_mat_width;
int					ImageModel::m_mat_height;
list<wxRect>		ImageModel::m_wx_vector_list;
string				ImageModel::AI_pic_name;
list<pic_data>		*ImageModel::m_list_pic_data;
string				ImageModel::m_now_detection_file_name;
int					ImageModel::m_now_detection_file_num;
int					ImageModel::m_now_detection_file_all_num;
list<string>		ImageModel::m_list_recipe;
wxString			ImageModel::m_save_directory_name;
wxString			ImageModel::m_save_path_img;
wxString			ImageModel::m_save_path_excel;
int					ImageModel::m_excel_path_now_num;

bool ImageModel::m_getPreData;
int ImageModel::m_preOkNum;
int ImageModel::m_preNgNum;
float ImageModel::m_preRatio;

ListData* ImageModel::m_data;
wxString ImageModel::m_imageResult;
int ImageModel::img_num;
MyThreadPool* ImageModel::m_threadPool;

mutex ImageModel::m_mutexPic;
mutex ImageModel::m_mutexPost;
mutex ImageModel::m_mutexThread;
mutex ImageModel::m_mutexArti;
mutex ImageModel::m_mutexNum;

list<PostPara> ImageModel::m_postParaVec;
SingleImage* ImageModel::s_image_temp;
string ImageModel::m_imageName;
string ImageModel::m_imagePath;
list<wxRect> ImageModel::m_rectList;
int ImageModel::m_aiNgNum;

bool json_resolverxxx(char *indata, ai_result_type &outdata)
{
	Json::Reader jreader;
	Json::Value jv;
	bool res;
	res = jreader.parse(indata, jv);
	if (res)
	{
		outdata.id = jv["id"].asString();
		unsigned int i_sum = jv["predictions"].size();
		outdata.predictions.resize(i_sum);
		if (i_sum)
		{
			for (unsigned int i = 0; i < i_sum; i++)
			{
				Json::Value jv_predictions = jv["predictions"][i];
				unsigned int j_sum = jv_predictions["Pos"].size();
				outdata.predictions[i].Pos.resize(j_sum);
				if (j_sum)
				{
					for (unsigned int j = 0; j < j_sum; j++)
					{
						Json::Value jv_pos = jv_predictions["Pos"][j];
						for (unsigned int k = 0; k < 5; k++)
						{
							outdata.predictions[i].Pos[j].Pos[k] = jv_pos[k].asDouble();
						}
					}
				}
				outdata.predictions[i].label = jv_predictions["label"].asString();
			}
		}
		if (jv["success"].asString() == string("true"))
		{
			outdata.succes = true;
		}
		else
		{
			outdata.succes = false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

ImageModel *ImageModel::GetInstance(wxWindow *frame)
{
	if (NULL == m_instance)
	{
		m_instance = new ImageModel(frame);
	}
	return m_instance;
}

ImageModel::ImageModel(wxWindow *frame)
{
	m_frame = frame;
	m_save_ip = "";
	m_save_port = 0;
	m_save_path = "";
	m_save_path_csv = "";
	m_save_path_img = "";
	m_save_path_excel = "";
	m_save_single = 0;
	m_aiNgNum = 0;

	m_refresh_window = NULL;
	m_list_window = NULL;
	m_control_window = NULL;
	m_socket_center = NULL;
	s_image_temp = NULL;
//	m_verifyDlg = NULL;
	
	m_testing_start_flag = false;

	m_connect_btn_status_flag = false;

	m_wx_vector_list.clear();
	m_list_data.clear();

	m_map_red_rect.clear();

	m_list_paper_file.clear();
//	m_list_file_struct.clear();
//	m_mutex = new wxMutex();
	m_is_draw_rect = false;
	m_model_panel = NULL;
	m_noTestDlg = nullptr;

	m_list_image_model.clear();
//	m_list_lizi_result.clear();
	m_element_list.clear();
	m_recipe_name = "";
	m_recipe_path = "";
	m_paper_file_num = 0;
	m_now_detection_file_name = "null";
	m_now_detection_file_num = 0;
	m_now_detection_file_all_num = 0;
	m_list_recipe.clear();

	m_mutex = new wxMutex();
	m_draw_pen_state = 0;
	m_save_directory_name = "";
	m_NGNum = 0;
	m_OKNum = 0;
	m_allPicesNum = 0;
	m_radio = 0;
	m_excel_path_now_num = 2;

	m_getPreData = false;
	m_preOkNum = 0;
	m_preNgNum = 0;
	m_preRatio = 0;

	m_data = nullptr;
	m_threadPool = new MyThreadPool;
	//OK_num = 0;
	//Ng_num = 0;
	//Ratio_Num = 0;
	//m_runExe = false;
	m_popUpDlg = nullptr;
}

ImageModel::~ImageModel()
{
	if (m_socket_center)
	{
		m_socket_center->SetRunFlag(false);
		Sleep(1);
		m_socket_center = NULL;
	}

// 	if (m_verifyDlg)
// 	{
// 		delete m_verifyDlg;
// 	}
	if (m_model_panel)
	{
		delete m_model_panel;
		m_model_panel = NULL;
	}
	if (m_noTestDlg)
	{
		delete m_noTestDlg;
		m_noTestDlg = nullptr;
	}

	m_refresh_window = NULL;
	m_profile = NULL;
	
	m_wx_vector_list.clear();
	m_list_data.clear();
	m_map_red_rect.clear();
	m_list_image_model.clear();
//	m_list_lizi_result.clear();
	if (m_mutex)
	{
		delete m_mutex;
		m_mutex = NULL;
	}
	m_list_paper_file.clear();
	m_list_recipe;
	//
	if (m_threadPool != nullptr)
	{
		delete m_threadPool;
		m_threadPool = nullptr;
	}
	if (m_popUpDlg != nullptr)
	{
		delete m_popUpDlg;
		m_popUpDlg = nullptr;
	}
}
void ImageModel::DeleteSocketThread()
{
	if (m_socket_center->GetErrMessage() != "")
	{
		wxMessageBox(m_socket_center->GetErrMessage());
		m_socket_center->SetErrMessage("");
	}

	if (m_socket_center)
	{
		m_socket_center->SetRunFlag(false);
		Sleep(1);
		m_socket_center = NULL;
	}

	if (m_mutex)
	{
		delete m_mutex;
		m_mutex = NULL;
	}

	m_control_window->SetConnectBtnStatus(true);

	m_connect_btn_status_flag = false;
}

void ImageModel::SetNoTestData(NoTestData* data, string name, string path)
{
	vector<NoTestElement> l_noTestEleVec = data->GetNoTestElementVec();
	m_noTestRecName = name;
	m_noTestRecPath = path;

	this->NoTestDataRecotery(l_noTestEleVec);
}

void ImageModel::NoTestDataRecotery(const vector<NoTestElement>& eleVec)
{
	vector<NoTestImage_t> l_noTestImageVec;
	for (NoTestElement ele: eleVec)
	{
		NoTestImage_t l_noTest;
		l_noTest.ImageName = ele.GetImageName();
		l_noTest.RectPointVec = ele.GetRectPointVec();
		l_noTestImageVec.push_back(l_noTest);
	}
	m_noTestImageVec = l_noTestImageVec;

	m_noTestDlg->GetModelPanel()->AddData(m_noTestImageVec);
	m_noTestDlg->Show();
}

void ImageModel::ClearPaperFileList()
{
	m_list_paper_file.clear();
}

void ImageModel::SetProfile(Profile *profile)
{
	m_profile = profile;

	m_save_ip = m_profile->GetIPPath();
	m_save_port = m_profile->GetPort();
	m_save_path = m_profile->GetPath();
	m_save_path_csv = m_profile->GetPathCsv();
	m_save_path_img = m_profile->GetPathSaveImg();
	m_save_path_excel = m_profile->GetPathExcel();
	m_save_single = m_profile->GetNowPlatformDeckSingle();

	//todo
	m_preOkNum = m_profile->GetOkNum();
	m_preNgNum = m_profile->GetNgNum();
	m_preRatio = m_profile->GetRatio();

	m_list_window->SetSavePathImg(m_save_path_img);

	m_save_ic_binaryValue = m_profile->GetICbinaryValue();
	m_save_ic_backValue = m_profile->GetICbackValue();
	m_save_ic_medianBlurSize = m_profile->GetICmedianBlurSize();

	m_save_read_addr = m_profile->GetReadAddr();
	m_save_read_type = m_profile->GetReadType();
	m_save_write_OK_addr = m_profile->GetWriteOKAddr();
	m_save_write_OK_type = m_profile->GetWriteOKType();
	m_save_write_NG_addr = m_profile->GetWriteNGAddr();
	m_save_write_NG_type = m_profile->GetWriteNGType();

	m_save_is_use_ai = m_profile->GetIsUseAI();
	m_save_ai_path = m_profile->GetAIPath();

	if (m_save_is_use_ai)
	{
		cout << "Connecting to AI FTP." << m_save_ai_path << endl;
		wxLogMessage("Connecting to AI FTP.");

		curl_interface::curl_init();
		curl_interface curl_if(m_save_ai_path.ToStdString().c_str(), 1);

		wxString msg = "";
		if (!curl_if.is_connect())
		{
			wxMessageBox(_("Cannot connect to AI server, please check it."), _("Error"), wxICON_ERROR | wxOK);
			m_control_window->SetAIBtnStatus(2);
			msg = "AI连接失败";
			MyLog::LogWarning(msg.mb_str());
		}
		else
		{
			m_control_window->SetAIBtnStatus(1);
			m_list_window->SetAIFlag(true);
			msg = "AI已连接";
			MyLog::LogWarning(msg.mb_str());
		}
	}

	m_image_pool = ImagePool::Instance(224, 10000, 1);
}

void ImageModel::SetListData(ListData* data)
{
	m_data = data;
}


void ImageModel::SetResultLabel(float okNum, float ngNum, float ratio)
{
	m_preOkNum = okNum;
	m_preNgNum = ngNum;
	m_preRatio = ratio;
}

//Menu
//载入图像
bool ImageModel::OnLoadImage(cv::String image_path, string image_name)
{
	wxString msg;
	m_cv_srcmat = cv::imread(image_path, 0);
	m_image_path = image_path;
	m_image_name = image_name;

	if (0 == m_cv_srcmat.cols)
	{
		return false;
	}
	if (m_cv_srcmat.rows < 550 || m_cv_srcmat.cols < 550)
	{
		msg = wxString::Format("图:%s，长宽过小，可能不是检测图片，已忽略", image_name);
		MyLog::LogError(msg.mb_str());
		return false;
	}
//	cv::imwrite("./m_cv_srcmat.jpg", m_cv_srcmat);

	m_mat_width = m_cv_srcmat.cols;
	m_mat_height = m_cv_srcmat.rows;

	int temp_window_width = m_refresh_window->GetWindowWidth();
	int temp_zoomX = 0, temp_zoomY = 0;

	double image_radio = (double)m_mat_width / (double)temp_window_width;
	temp_zoomX = temp_window_width;
	temp_zoomY = m_mat_height / image_radio;

	m_refresh_window->SetZoom(temp_zoomX, temp_zoomY);

	m_refresh_window->SetMat_Width_Height(m_mat_width, m_mat_height);
	m_refresh_window->SetBitmapBuf(m_cv_srcmat);
	m_refresh_window->SetImagePath(m_image_name);
	m_refresh_window->Refresh();

	return true;
}
//修改IP 端口 检测路径
void ImageModel::OnSetDetectionPath()
{
	DetectionSetDlg *dlg = new DetectionSetDlg(m_frame);

	dlg->SetShowIP(m_save_ip);
	dlg->SetShowPort(m_save_port);
	dlg->SetShowPath(m_save_path);
	dlg->SetShowPathCsv(m_save_path_csv);
	dlg->SetShowPathSaveImg(m_save_path_img);
	dlg->SetShowPathExcel(m_save_path_excel);
	dlg->SetSingle(m_save_single);

	dlg->CreateGUIcontrols();
	wxString msg;
	if (1 == dlg->ShowModal())
	{
		m_save_ip = dlg->GetShowIP();
		m_save_port = dlg->GetShowPort();
		m_save_path = dlg->GetShowPath();
		m_save_path_csv = dlg->GetShowPathCsv();
		m_save_path_img = dlg->GetShowPathSaveImg();
		m_save_path_excel = dlg->GetShowPathExcel();
		m_save_single = dlg->GetSingle();

		m_list_window->SetSavePathImg(m_save_path_img);

		m_profile->SetIPPath(m_save_ip);
		m_profile->SetPort(m_save_port);
		m_profile->SetPath(m_save_path);
		m_profile->SetPathCsv(m_save_path_csv);
		m_profile->SetPathSaveImg(m_save_path_img);
		m_profile->SetPathExcel(m_save_path_excel);
		m_profile->SetNowPlatformDeckSingle(m_save_single);

		msg = wxString::Format("已完成修改——当前IP：%s， 端口：%d， 路径：%s , csv路径: %s , 图片保存路径: %s， 报表保存路径: %s，载台信号:%d", 
			m_save_ip, m_save_port, m_save_path, m_save_path_csv, m_save_path_img, m_save_path_excel, m_save_single);
		MyLog::LogMessage(msg.mb_str());
	}
	else
	{
		msg = "取消修改";
		MyLog::LogUser(msg.mb_str());

		dlg->Destroy();
		return;
	}
	dlg->Destroy();

	m_profile->SaveIni("Ini.ini");
}
//算法参数修改
void ImageModel::OnSetParemeter()
{
	ParameterSetDig *dlg = new ParameterSetDig(m_frame);

	dlg->SetICbinaryValue(m_save_ic_binaryValue);
	dlg->SetICbackValue(m_save_ic_backValue);
	dlg->SetICmedianBlurSize(m_save_ic_medianBlurSize);

	dlg->CreateGUIcontrols();
	wxString msg;
	if (1 == dlg->ShowModal())
	{
		m_save_ic_binaryValue = dlg->GetICbinaryValue();
		m_save_ic_backValue = dlg->GetICbackValue();
		m_save_ic_medianBlurSize = dlg->GetICmedianBlurSize();

		m_profile->SetICbinaryValue(m_save_ic_binaryValue);
		m_profile->SetICbackValue(m_save_ic_backValue);
		m_profile->SetICmedianBlurSize(m_save_ic_medianBlurSize);
		
		msg = wxString::Format("已完成修改——IC当前算法参数为：%d, %d, %d", m_save_ic_binaryValue, m_save_ic_backValue, m_save_ic_medianBlurSize);
		MyLog::LogMessage(msg.mb_str());
	}
	else
	{
		msg = "取消修改";
		MyLog::LogUser(msg.mb_str());

		dlg->Destroy();
		return;
	}
	dlg->Destroy();

	m_profile->SaveIni("Ini.ini");
}
//Addr
void ImageModel::OnSetAddr()
{
	AddrSetDig *dlg = new AddrSetDig(m_frame);
	dlg->SetReadAddr(m_save_read_addr);
	dlg->SetReadType(m_save_read_type);
	dlg->SetWriteOKAddr(m_save_write_OK_addr);
	dlg->SetWriteOKType(m_save_write_OK_type);
	dlg->SetWriteNGAddr(m_save_write_NG_addr);
	dlg->SetWriteNGType(m_save_write_NG_type);

	dlg->CreateGUIcontrols();
	wxString msg;
	if (1 == dlg->ShowModal())
	{
		m_save_read_addr = dlg->GetReadAddr();
		m_save_read_type = dlg->GetReadType();
		m_save_write_OK_addr = dlg->GetWriteOKAddr();
		m_save_write_OK_type = dlg->GetWriteOKType();
		m_save_write_NG_addr = dlg->GetWriteNGAddr();
		m_save_write_NG_type = dlg->GetWriteNGType();

		m_profile->SetReadAddr(m_save_read_addr);
		m_profile->SetReadType(m_save_read_type);
		m_profile->SetWriteOKAddr(m_save_write_OK_addr);
		m_profile->SetWriteOKType(m_save_write_OK_type);
		m_profile->SetWriteNGAddr(m_save_write_NG_addr);
		m_profile->SetWriteNGType(m_save_write_NG_type);

		if (m_socket_center != NULL)
		{
			m_socket_center->SetReadAddr(m_save_read_addr);
			m_socket_center->SetReadType(m_save_read_type);
			m_socket_center->SetWriteOKAddr(m_save_write_OK_addr);
			m_socket_center->SetWriteOKType(m_save_write_OK_type);
			m_socket_center->SetWriteNGAddr(m_save_write_NG_addr);
			m_socket_center->SetWriteNGType(m_save_write_NG_type);
		}

		msg = wxString::Format("已完成修改——当前读取地址为：%d， 读取类型为：%s， OK写入地址为：%d， OK写入类型为：%s， NG写入地址为：%d， NG写入类型为：%s", m_save_read_addr, m_save_read_type, m_save_write_OK_addr, m_save_write_OK_type, m_save_write_NG_addr, m_save_write_OK_type);
		MyLog::LogMessage(msg.mb_str());
	}
	else
	{
		msg = "取消修改";
		MyLog::LogUser(msg.mb_str());

		dlg->Destroy();
		return;
	}
	dlg->Destroy();

	m_profile->SaveIni("Ini.ini");
}

void ImageModel::OnOpenFileAlgorithmImg()
{
	wxString file_path = m_save_path_img + "\\Result_Image" + "\\Arithmetic_Img\\";

	if (_access(file_path, 0) == -1)
	{
		wxMessageBox(_("算法目录未生成"), _("Error"));
		return;
	}

	string s_file_path = wxString::Format("start \"\" \"%s", file_path).ToStdString();
	system(s_file_path.c_str());
}

void ImageModel::OnOpenFileAIImg()
{
	wxString file_path = m_save_path_img + "\\Result_Image" + "\\AI_Img\\";

	if (_access(file_path, 0) == -1)
	{
		wxMessageBox(_("AI目录未生成"), _("Error"));
		return;
	}

	string s_file_path = wxString::Format("start \"\" \"%s", file_path).ToStdString();
	system(s_file_path.c_str());
}

void ImageModel::OnOpenFileExcel()
{
	wxString file_path = m_save_path_excel + "\\Report\\";
	
	if (_access(file_path, 0) == -1)
	{
		wxMessageBox(_("报表目录未生成"), _("Error"));
		return;
	}

	string s_file_path = wxString::Format("start \"\" \"%s", file_path).ToStdString();
	system(s_file_path.c_str());
}

void ImageModel::OnOpenFileNowExcel()
{
	char nd[64] = { 0 };
	char time_A_or_P[64] = { 0 };
	time_t t = time((NULL));
	tm *tm_cur = localtime(&t);
	if (tm_cur->tm_hour < 8 || tm_cur->tm_hour > 19)
	{
		t = t - 12 * 60 * 60;
		strftime(time_A_or_P, sizeof(time_A_or_P), "%Y-%m-%d-PM.xls", localtime(&t));
	}
	else
	{
		strftime(time_A_or_P, sizeof(time_A_or_P), "%Y-%m-%d-AM.xls", localtime(&t));
	}
	strftime(nd, sizeof(nd), "%Y%m%d\\", localtime(&t));

	wxString file_path = m_save_path_excel + "\\Report\\" + nd + "\\" + time_A_or_P;
	fstream _file;
	_file.open(file_path.ToStdString(), ios::in);
	if (!_file)
	{
		wxMessageBox(_("当前时间段报表未生成"), _("Error"));
		return;
	}

	_file.close();
	string s_file_path = wxString::Format("start \"\" \"%s", file_path).ToStdString();
	system(s_file_path.c_str());
}

void ImageModel::OnOpenFileRecordExcel()
{
	wxString file_path = m_save_path_excel + "\\Report\\record.xls";
	fstream _file;
	_file.open(file_path.ToStdString(), ios::in);
	if (!_file)
	{
		wxMessageBox(_("汇总报表未生成"), _("Error"));
		return;
	}

	_file.close();
	string s_file_path = wxString::Format("start \"\" \"%s", file_path).ToStdString();
	system(s_file_path.c_str());
}

//Control
void ImageModel::OnConnectSocket()
{
	wxString msg;

	if (!m_connect_btn_status_flag)
	{
		m_control_window->SetBtnStatus(false);
		string temp_ip = m_save_ip.c_str();

		msg = "正在创建socket";
		MyLog::LogMessage(msg.mb_str());
		m_socket_center = new SocketCenter(m_frame, temp_ip, m_save_port/*"192.168.20.23", 4000*/);
		if (!m_socket_center->CreateSocket())
		{
			m_control_window->SetConnectBtnStatus(true);
			return;
		}
		m_control_window->SetBtnStatus(true);

		m_socket_center->SetReadAddr(m_save_read_addr);
		m_socket_center->SetReadType(m_save_read_type);
		m_socket_center->SetWriteOKAddr(m_save_write_OK_addr);
		m_socket_center->SetWriteOKType(m_save_write_OK_type);
		m_socket_center->SetWriteNGAddr(m_save_write_NG_addr);
		m_socket_center->SetWriteNGType(m_save_write_NG_type);

		m_socket_center->Create();
//		m_socket_center->Run();

		m_connect_btn_status_flag = true;
	}
	else
	{
		if (NULL != m_socket_center)
		{
			if (m_socket_center->GetRunFlag() == true)
			{
				m_socket_center->SetRunFlag(false);
				Sleep(1);
				m_socket_center = NULL;
			}
		}

		msg = "销毁socket";
		MyLog::LogMessage(msg.mb_str());

		m_control_window->SetConnectBtnStatus(true);

		m_connect_btn_status_flag = false;
	}

}
//AI
void ImageModel::OnConnectAI()
{
	SetAIDlg *dlg = new SetAIDlg(m_frame);

	dlg->SetIsUseAI(m_save_is_use_ai);
	dlg->SetAIPath(m_save_ai_path);

	dlg->CreateGUIcontrols();
	wxString msg;
	if (1 == dlg->ShowModal())
	{
		m_save_is_use_ai = dlg->GetIsUseAI();
		m_save_ai_path = dlg->GetAIPath();

		m_profile->SetIsUseAI(m_save_is_use_ai);
		m_profile->SetAIPath(m_save_ai_path);

		msg = wxString::Format("已完成修改——当前是否使用AI：%d，AI路径为：%s", m_save_is_use_ai, m_save_ai_path);
		MyLog::LogMessage(msg.mb_str());
	}
	else
	{
		msg = "取消修改";
		MyLog::LogUser(msg.mb_str());

		dlg->Destroy();
		return;
	}
	dlg->Destroy();

	if (m_save_is_use_ai)
	{
		cout << "Connecting to AI FTP." << m_save_ai_path << endl;
		wxLogMessage("Connecting to AI FTP.");

		curl_interface::curl_init();
		curl_interface curl_if(m_save_ai_path.ToStdString().c_str(), 1);
		if (!curl_if.is_connect())
		{
			wxMessageBox(_("Cannot connect to AI server, please check it."), _("Error"), wxICON_ERROR | wxOK);
			m_control_window->SetAIBtnStatus(2);
			m_list_window->SetAIFlag(false);
		}
		else
		{
			m_control_window->SetAIBtnStatus(1);
			m_list_window->SetAIFlag(true);
		}
	}
	else
	{
		m_control_window->SetAIBtnStatus(0);
		m_list_window->SetAIFlag(false);
	}

	m_profile->SaveIni("Ini.ini");
}
//检测
void ImageModel::OnDetection(bool send_result_flag, cv::Mat cvSrcmat, string image_name, string image_path, int mat_width, int mat_height)
{
	wxString msg;
	AI_pic_name = image_name;
	cv::Mat image_data = cvSrcmat.clone();

	list<SplitRect>::iterator it_split;
	list<SplitRect> *list_split_rect_temp = NULL;
	//SingleImage *s_image_temp = NULL;

	if (image_data.empty())
		cout << "errorImage" << endl;

	//vector<cv::Rect> vecter_result;
	vecter_result.clear();
//	vecter_result.reserve(10000);

// 	map<string, list<wxRect>> map_red_rect;
// 	map_red_rect.clear();

	wxString wxs_result = "";
	wxString temp_image_name = image_name.c_str();
	temp_image_name = temp_image_name.SubString(0, temp_image_name.size() - 5);

	//wxString wxs_NG_num = "";
	wxs_NG_num = "";

	wxString temp_path = image_path;
	temp_path = image_path.substr(0, image_path.size() - image_name.size() - 1);

	wxString file_save_path = temp_path + "\\" + temp_image_name;
	wxString file_path = temp_path + "\\" +temp_image_name + "-HasBeenTesting\\";

	wxString temp_file_name = "";
	temp_file_name = strstr(file_save_path, "[");
	string s_t;
	wxString temp_now_time = GetNowTime(0).c_str();
	if ("" != temp_file_name)
	{
		string temp_s_file_save_path = file_save_path.c_str();
		temp_file_name = temp_s_file_save_path.substr(0, file_save_path.size() - temp_file_name.size() - 1).c_str();
		char *t_c;
		t_c = (char *)malloc((temp_file_name.length() + 1) * sizeof(char*));
		strcpy_s(t_c, temp_file_name.length() + 1, temp_file_name.c_str());
		char *p_c = strrchr(t_c, '\\');

		if (*p_c != NULL)
		{
			s_t = p_c;
			s_t = s_t.substr(1);
			bool is_find = false;
			for (list<paper_file>::iterator it = m_list_paper_file.begin(); it != m_list_paper_file.end(); it++)
			{
				if (it->file_name == s_t.c_str())
				{
					if (it->file_time == temp_now_time)
					{
						is_find = true;
					}
				}
			}
			if (!is_find)
			{
				m_paper_file_num = 0;
				wxArrayString img_list;
				wxDir::GetAllFiles(temp_file_name, &img_list, "*.jpg", wxDIR_DEFAULT);
				if (img_list.size() == 0)
				{
					//wxMessageBox(_("该片子下没有图片"));
					cout << "该片子下没有图片" << endl;
				}
				for (int i = 0; i < img_list.size(); i++)
				{
					if (strstr(img_list[i], "-HasBeenTesting") || strstr(img_list[i], "_SUB_"))
					{
						continue;
					}
					else
					{
						m_paper_file_num++;
					}
				}
				paper_file temp_paper_file;
				temp_paper_file.file_name = s_t.c_str();
				temp_paper_file.file_time = temp_now_time;
				temp_paper_file.paper_result = /*false*/true;
				temp_paper_file.paper_lizi_result = false;
				temp_paper_file.pic_num = 0;
				m_list_paper_file.push_back(temp_paper_file);
			}
		}
	}

	char *t;
	t = (char *)malloc((image_name.length() + 1) * sizeof(char*));
	strcpy_s(t, image_name.length() + 1, image_name.c_str());
	char *p = strrchr(t, '\\');

	bool image_type = false; //true是IC, false是FPC
	string s_tt;
	if (*p != NULL)
	{
		s_tt = p;
		s_tt = s_tt.substr(1);
		file_path += s_tt.c_str();

		for (list<string>::iterator it = m_list_recipe.begin(); it != m_list_recipe.end(); it++)
		{
			if (strstr(s_tt.c_str(), it->c_str()))
			{
				image_type = true;
				break;
			}
		}
	}
	else
	{
		file_path += image_name.c_str();

		for (list<string>::iterator it = m_list_recipe.begin(); it != m_list_recipe.end(); it++)
		{
			if (strstr(file_path, it->c_str()))
			{
				image_type = true;
				break;
			}
		}
	}

	//msg = "进入算法";
	//MyLog::LogMessage(msg.mb_str());

	//调用算法
	if (image_type)
	{
		cv::Mat Mat_model_image{};
		for (list<Image_Properties>::iterator it = m_list_image_model.begin(); it != m_list_image_model.end(); it++)
		{
			wxString temp_image_name = image_name.c_str();
			wxString temp_model_name = it->image_name.c_str();
			if (strstr(temp_image_name, temp_model_name))
			{
				cv::Mat markimg = it->image_mark.front();
				int mark_x = it->image_point.front().x;
				int mark_y = it->image_point.front().y;
				float l_markX = mark_x;
				float l_markY = mark_y;
				Mat_model_image = it->Model_image.clone();
				cout << temp_model_name << ":" << " x:" << mark_x << " y:" << mark_y << endl;

				
				//todo
				vector<RectPoint_t> l_rectPointVec;
				for (NoTestImage_t l_notestImage : m_noTestImageVec)
				{
					wxString l_noTestModel = l_notestImage.ImageName.c_str();
					if (strstr(temp_image_name, l_noTestModel))
					{
						l_rectPointVec = l_notestImage.RectPointVec;
						break;
					}
				}

				std::unique_lock<mutex> l_artiLock(m_mutexArti);
				string l_imageName = string(temp_image_name.mb_str());
				vecter_result.clear();
				DWORD time1_for = GetTickCount();
				ForeignMaterialDetector(markimg/*markIMG*/, /*131*/mark_x, /*436*/mark_y,
					image_data, Mat_model_image, m_save_ic_binaryValue, m_save_ic_medianBlurSize, l_rectPointVec, vecter_result, l_imageName);//新参数名为 m_save_ic_backValue
				DWORD time2_for = GetTickCount();
				cout << temp_image_name << ":  ForeignMaterialDetector:" << (time2_for - time1_for) / 1000.0 << "S" << endl;
				l_artiLock.unlock();

			}
		}
	}
	else
	{
//		bumpDetector(m_cv_srcmat, m_save_fpc_xc, m_save_fpc_dw, m_save_fpc_ow, m_save_fpc_sigma, m_save_fpc_dividesize, m_save_fpc_mergelength, m_save_fpc_filterThres, m_save_fpc_filterSize, m_vecter_result);
	}
	
	//msg = "算法结束";
	//MyLog::LogMessage(msg.mb_str());

//	vecter_result.clear();
	cout << "vecter_result: " << vecter_result.size() << endl;
	curl_interface curl_if(m_save_ai_path.ToStdString().c_str(), 15);
	//int img_num = 0;
	img_num = 0;
	if (!vecter_result.empty())
	{
		Lock_(true);
		m_refresh_window->DrawResult(vecter_result, image_data);
		//添加截图数据
//		m_wx_vector_list.clear();
		//vector<wxRect> wx_vector_list;
		wx_vector_list.clear();
		wx_vector_list = m_refresh_window->GetwxRectList();

		wxImage temp_image;
		cv::Mat rgbMat;
		cv::Mat temp_paint_fin_iamge = m_refresh_window->GetPaintFinImage();
		cv::cvtColor(temp_paint_fin_iamge, rgbMat, cv::COLOR_BGR2RGB);
		temp_image.SetData(rgbMat.data, mat_width, mat_height, true);

//		temp_image.SaveFile("C:\\Users\\Hwww\\Desktop\\Detecting\\2.jpg");
		Lock_(false);

		msg = "开始缺陷图保存";
		MyLog::LogMessage(msg.mb_str());

		vector<cv::Rect>::iterator iter = vecter_result.begin();
		list<wxRect> temp_rect;

		if (m_save_is_use_ai)
		{
			s_image_temp = ImagePool::GetInstance()->GetFreeSingleImage();
			if (!s_image_temp)
			{
				while (1)
				{
					Sleep(10);
					s_image_temp = ImagePool::GetInstance()->GetFreeSingleImage();
					if (s_image_temp)
						break;
				}
			}
			//
			int l_realNum = wx_vector_list.size();
			int l_exitFlag = 1;
			int l_aiNgNum = 0;
			int l_picNum = 0;

			vector<cv::Rect>::iterator l_aiIter = vecter_result.begin();
			vector<wxRect>::iterator it = wx_vector_list.begin();
			//string name = "";
			//int index = 0;
			//add threadpool
			int l_vectest = 0;
			if (!m_postParaVec.empty())
			{
				std::list<PostPara>::iterator l_it = m_postParaVec.begin();
				while (l_it != m_postParaVec.end())
				{
					if (l_it->m_splitList != nullptr)
					{
						//delete l_it->m_splitList;
						l_it->m_splitList->clear();
					}
					if (l_it->m_puzzle.m_data != nullptr)
					{
						delete l_it->m_puzzle.m_data;
						l_it->m_puzzle.m_data = nullptr;
					}
					l_it->m_errRedVec.clear();
					l_it = m_postParaVec.erase(l_it);
				}
			}
		//	cout << "m_postParaVec==========" << m_postParaVec.size() << endl;
		//	m_postParaVec.clear();

			m_aiNgNum = 0;
			while (true)
			{
				bool l_isEnd = false;
				if (l_realNum > MAXIMAGENUM)
				{
					l_realNum = l_realNum - MAXIMAGENUM;
					l_exitFlag = l_realNum;
					s_image_temp->SetImageNum(MAXIMAGENUM);
					l_picNum = MAXIMAGENUM;

				}
				else
				{
					l_realNum = l_realNum%MAXIMAGENUM;
					l_exitFlag = l_exitFlag / MAXIMAGENUM;
					s_image_temp->SetImageNum(l_realNum);
					l_picNum = l_realNum;
				}
				
				if (l_exitFlag == 0)
				{
					l_isEnd = true;
				}

				list_split_rect_temp = s_image_temp->Buf2List();
				it_split = list_split_rect_temp->begin();
				s_image_temp->SetSingImageStatu(true);

				msg = "准备拼大图";
				MyLog::LogMessage(msg.mb_str());

				vector<wxRect> ErrRedRect;
				ErrRedRect.clear();

				for (int i = 0; i < l_picNum; i++, it++, l_aiIter++)
				{
					//if (m_save_is_use_ai)
					{
						wxImage AI_temp_image;
						AI_temp_image = temp_image.GetSubImage(wxRect(it->x, it->y, it->width, it->height));

						wxString wxs_algorithm = "";
						wxs_algorithm = wxString::Format("yw-%d", i);
						string str_algorithm = "";
						str_algorithm = wxs_algorithm.c_str();
						SaveImageData(AI_temp_image, str_algorithm, image_name, temp_path);

						ImageU1 img_u1(224, 224);
						img_u1.CopyFromWxImage(temp_image.GetSubImage(wxRect(it->x, it->y, it->width, it->height)));

						std::memcpy(it_split->getData(), img_u1.m_data, it->width *it->height);

						it_split->setRoi(RoiRect(it->x, it->y, it->width, it->height));
						it_split++;
						ErrRedRect.push_back(wxRect(l_aiIter->x, l_aiIter->y, l_aiIter->width, l_aiIter->height));
					}
			
				}
					s_image_temp->List2Buf();
					int size_img = 224;
					int size_side = s_image_temp->GetAiImageSideNum();
					//ImageU1 temp_imgu1(size_side*size_img, size_side*size_img);
					//std::memcpy(temp_imgu1.m_data, s_image_temp->GetImageBuf(size_side*size_side), size_side*size_side*size_img*size_img);
					//cv::Mat l_srcMat = cv::Mat(temp_imgu1.m_height, temp_imgu1.m_width, CV_8UC1, temp_imgu1.m_data);
					//name = "image" + std::to_string(index)+".jpg";
					//cv::imwrite(name, l_srcMat);
					//
					PostPara l_postPara;
					l_postPara.m_splitList = list_split_rect_temp;
					l_postPara.m_puzzle.m_width = size_side*size_img;
					l_postPara.m_puzzle.m_height = size_side*size_img;
					l_postPara.m_puzzle.Create(size_side*size_img, size_side*size_img);
					std::memcpy(l_postPara.m_puzzle.m_data, s_image_temp->GetImageBuf(size_side*size_side), size_side*size_side*size_img*size_img);
					l_postPara.m_errRedVec = ErrRedRect;
					m_postParaVec.push_back(l_postPara);

					m_rectList.clear();
					m_rectList = temp_rect;
					m_imageName = image_name;
					m_imagePath = temp_path;

					/*img_num = PostAnAllImage2AI(*list_split_rect_temp, temp_imgu1, &curl_if, *s_image_temp, 5, temp_image, ErrRedRect, temp_rect, image_name, temp_path);

					if (-2 == img_num || -1 == img_num)
					{
						int i = 1;
						iter = vecter_result.begin();
						for (vector<wxRect>::iterator it = wx_vector_list.begin(); it != wx_vector_list.end(); it++, iter++)
						{
							wxs_result = "NG";
							s_verify sy;
							sy.m_pos_code = wxString::Format("AI-%d", i).ToStdString();
							sy.m_rect = wxRect(100 * i, 100 * i, 500, 500);
							sy.m_img = wxImage(temp_image).GetSubImage(wxRect(it->x, it->y, it->width, it->height));
							sy.m_ng_rect = SaveNGRect(wxRect(it->x, it->y, it->width, it->height), cv::Rect(iter->x, iter->y, iter->width, iter->height));
							SaveImageData(sy.m_img, sy.m_pos_code, image_name, temp_path);
							sy.m_pos_x = 0;
							sy.m_err_num = 1;
							sy.m_len = 0;
							sy.m_line_name = "X10101";
							sy.m_pos_y = 0;
							sy.m_type = 1;

							m_list_data.push_back(sy);
							temp_rect.push_back(sy.m_ng_rect);

							msg = wxString::Format("AI超时，保存算法结果缺陷图%d", i);
							MyLog::LogWarning(msg.mb_str());

							i++;
						}
						wxs_NG_num << wx_vector_list.size();
						s_image_temp->SetSingImageStatu(false);
						break;
					}
					else
					{
						l_aiNgNum += img_num;
						if (l_isEnd)
						{
							wxs_NG_num << l_aiNgNum;
						}

					}*/
					s_image_temp->SetSingImageStatu(false);
				
				if (l_isEnd)
				{
					break;
				}
	
			}//while
	
			int l_num = 0;
			 int l_threadNum = m_postParaVec.size();
			 m_threadPool->Start(l_threadNum);
		   m_threadPool->SetExitThread(l_threadNum);
			
			 for (PostPara& para: m_postParaVec)
			{
			//	this_thread::sleep_for(chrono::milliseconds(200));
				l_num++;
				m_threadPool->SetRunNum(l_num);
				std::function<void()> l_task = bind(ImageModel::PostImageFun,std::ref(para), /*std::ref(curl_if),*/ std::ref(temp_image));
				m_threadPool->AddTask(l_task);
			}

			 m_threadPool->Finish();
			 std::unique_lock<mutex> l_numLock(m_mutexNum);
			 wxs_NG_num = "";
			 wxs_NG_num << m_aiNgNum;
			 l_numLock.unlock();
		}//AI
		else
		{
			//算法结果
			vector<wxRect>::iterator it = wx_vector_list.begin();

			for (int i = 0; i < wx_vector_list.size(); i++, it++, iter++)
			{
					wxs_result = "NG";
					s_verify sy;
					sy.m_pos_code = wxString::Format("yw-%d", i).ToStdString();
					sy.m_rect = wxRect(100 * i, 100 * i, 500, 500);
					sy.m_img = wxImage(temp_image).GetSubImage(wxRect(it->x, it->y, it->width, it->height));
					sy.m_ng_rect = SaveNGRect(wxRect(it->x, it->y, it->width, it->height), cv::Rect(iter->x, iter->y, iter->width, iter->height));
					SaveImageData(sy.m_img, sy.m_pos_code, image_name, temp_path);
					sy.m_pos_x = 0;
					sy.m_err_num = 1;
					sy.m_len = 0;
					sy.m_line_name = "X10101";
					sy.m_pos_y = 0;
					sy.m_type = 1;

					m_list_data.push_back(sy);
					temp_rect.push_back(sy.m_ng_rect);

					msg = wxString::Format("已保存缺陷图%d", i);
					MyLog::LogMessage(msg.mb_str());
	
			}
			wxs_NG_num = "";
			wxs_NG_num << wx_vector_list.size();
		}
		//todo
		Lock_(true);
		std::map<string, list<wxRect>> l_mapRect = m_data->GetRedRectMap();
		l_mapRect.insert(make_pair(image_name, temp_rect));
		m_list_window->SetMapRedRect(l_mapRect);
		m_data->SetRedRectMap(l_mapRect);
		Lock_(false);

		msg = "完成所有缺陷图保存";
		MyLog::LogMessage(msg.mb_str());

	}
	else
	{
		wxs_result = "OK";
		//wxs_NG_num = "";
		wxs_NG_num = "0";
	}

	if (m_save_is_use_ai)
	{
		if (wxs_NG_num == "0")
		{
			wxs_result = "OK";
		}
		else
		{
			wxs_result = "NG";
			if (-2 == img_num)
				wxs_result = "NG(AI_OverTime)";
			else if(-1 == img_num)
				wxs_result = "NG(AI_Error)";
		}
	}
	else
	{
		if (!image_type)
		{
			wxs_result = "OK";
		}
	}

	cout << "wxs_result :" << wxs_result << endl;
	//在列表界面加入检测后信息
	wxString wxs_now_time;
	wxs_now_time = GetNowTime(1).c_str();
	
	wxString str_lizi_result = "OK";
	//file_save_path += "-HasBeenTesting";
	//file_save_path += ".jpg";
	//判断片子的图片OK或者NG数
	bool is_find_pic = false;
	for (list<paper_file>::iterator it = m_list_paper_file.begin(); it != m_list_paper_file.end(); it++)
	{
		if (it->file_name == s_t.c_str())
		{
			if (it->file_time == temp_now_time)
			{
				list<file_struct> temp_file_struct = it->pic_list;
				for (list<file_struct>::iterator iter = temp_file_struct.begin(); iter != temp_file_struct.end(); iter++)
				{
					if (!is_find_pic)
					{
						if (iter->pic_name == file_path)
						{
							is_find_pic = true;
							break;
						}
					}
				}
				if (!is_find_pic)
				{
					file_struct temp_struct;
					temp_struct.pic_name = file_path;
					temp_struct.file_time = temp_now_time;
					temp_struct.lizi_TorF = true;
					//str_lizi_result = "NG";
					if (wxs_result == "OK")
					{
						temp_struct.is_result = true;
					}
					else
					{
						temp_struct.is_result = false;
						it->paper_result = false;
					}
					it->pic_list.push_back(temp_struct);
					it->pic_num++;
				}
			}
		}
	}
	//再次判断片子异物结果是OK还是NG
	bool find_NG = false;
	bool lizi_result = true;
	for (list<paper_file>::iterator it = m_list_paper_file.begin(); it != m_list_paper_file.end(); it++)
	{
		list<file_struct> temp_file_struct = it->pic_list;
		for (list<file_struct>::iterator iter = temp_file_struct.begin(); iter != temp_file_struct.end(); iter++)
		{
#ifdef __LIZI__
			bool temp_lizi_result = true;
//			DWORD  time_1 = GetTickCount();
			temp_lizi_result = GotoLizi(temp_file_name, iter->pic_name);
// 			DWORD  time_2 = GetTickCount();
// 			cout << "lizi >> " << "All time:" << (time_2 - time_1) / 1000.0 << "s" << endl;

			if (!temp_lizi_result)
			{
				lizi_result = temp_lizi_result;
				str_lizi_result = "NG";
			}	
#endif // __LIZI__
			if (!iter->is_result)
			{
				iter->lizi_TorF = lizi_result;
				find_NG = true;
				break;
			}
			else
			{
				//判断某张图中是否带有LR偏移图
				string str_L = iter->pic_name.c_str();
				//				string son_str = "";
				if (str_L == file_path)
				{
					string::size_type position;
					position = str_L.find("]");
					if (position != str_L.npos)
					{
						str_L = str_L.insert(position + 6, "SUB_L_");
						position = str_L.find("-HasBeenTesting");
						if (position != str_L.npos)
						{
							str_L = str_L.substr(0, position);
							str_L += ".jpg";

							fstream temp_file;
							temp_file.open(str_L, ios::in);
							if (temp_file)
							{
								temp_file.close();
								find_NG = true;
								wxs_result = "OK";

								if (vecter_result.empty())
								{
									wxs_NG_num = "S";
								}
								it->paper_result = true;

// 								string str_R = "";
// 								size_t pos_R = str_L.find("SUB_L_");
// 								str_R = str_L.substr(0, pos_R) + "SUB_R_" + str_L.substr(pos_R + 6);
// 
// 								wxString L_name = "";
// 								wxString R_name = "";
// 								wxString Transfer_Path_L = "";
// 								wxString Transfer_Path_R = "";
// 
// 								L_name = str_L.substr(str_L.find("[")).c_str();
// 								R_name = str_R.substr(str_R.find("[")).c_str();
// 
// 								Transfer_Path_L = file_save_path + "\\" + L_name;
// 								Transfer_Path_R = file_save_path + "\\" + R_name;
// 
// 								if (!wxDir::Exists(file_save_path))
// 								{
// 									wxDir::Make(file_save_path);
// 								}
// 								if (wxCopyFile(str_L, Transfer_Path_L))
// 								{
// 									wxRemoveFile(str_L);
// 								}
// 								if (wxCopyFile(str_R, Transfer_Path_R))
// 								{
// 									wxRemoveFile(str_R);
// 								}
							}
							else
							{
								iter->lizi_TorF = lizi_result;
							}
							break;
						}
					}
				}
			}
		}
		if (!find_NG)
		{
#ifdef __LIZI__
			if (lizi_result)
#endif 
				it->paper_result = true;
		}
	}

	if (m_list_paper_file.back().pic_num == m_paper_file_num)
	{
		ChangeResult();
		//发送检测结果
		if (m_list_paper_file.back().paper_result)
		{
			if (m_socket_center != NULL)
			{
				unsigned int data[256] = { 1 };
				string temp = m_save_write_OK_type.c_str();
				char* temp_write_type = (char*)temp.c_str();

				//DWORD  time_1 = GetTickCount();
				m_socket_center->SendMsg_Write(m_save_write_OK_addr, temp_write_type, 1, data); //确认通信协议后更改
				
// 				DWORD  time_2 = GetTickCount();
// 				cout << "ImageModel >> " << "SengMsg time:" << (time_2 - time_1) / 1000.0 << "s" << endl;
			}
		}
		else
		{
			if (m_socket_center != NULL)
			{
				unsigned int data[256] = { 2 };
				string temp = m_save_write_NG_type.c_str();
				char* temp_write_type = (char*)temp.c_str();

				//DWORD  time_1 = GetTickCount();

				m_socket_center->SendMsg_Write(m_save_write_NG_addr, temp_write_type, 1, data); //确认通信协议后更改
				
// 				DWORD  time_2 = GetTickCount();
// 				cout << "ImageModel >> " << "SengMsg time:" << (time_2 - time_1) / 1000.0 << "s" << endl;
			}
		}
	}

	Lock_(true);
	SetImageResult(wxs_result);
	if (wxs_result == "NG" || wxs_result == "NG(AI_OverTime)"|| wxs_result == "NG(AI_Error)")
	{
		m_list_window->AddData(image_name, wxs_result, str_lizi_result, wxs_NG_num, file_path, wxs_now_time);
		string l_result = string(wxs_result.mb_str());
		string l_ngNum = string(wxs_NG_num.mb_str());
		string l_path = string(file_path.mb_str());
		string l_time = string(wxs_now_time.mb_str());

	  ImageInfo l_info;
		l_info.imageName = image_name;
		l_info.imageResult = l_result;
		l_info.imageNgNum = l_ngNum;
		l_info.imagePath = l_path;
		l_info.imageTime = l_time;

		std::vector<ImageInfo> l_infoVec = m_data->GetListInfo();
		l_infoVec.push_back(l_info);
		m_data->SetListInfo(l_infoVec);
		m_data->OnSave("listData.dat");
		//m_data->SetListInfo(l_infoVec);

	}
	//cout << "image_name " << image_name << "wxs_result " << wxs_result << "str_lizi_result " << str_lizi_result << "wxs_NG_num " << wxs_NG_num << "file_path " << file_path << "wxs_now_time " << wxs_now_time << endl;
	Lock_(false);

	m_show_img_string = file_path;

	//转移图片位置
	if (!wxDir::Exists(file_save_path))
	{
		wxDir::Make(file_save_path);
	}

	//if (wxCopyFile(image_path, file_path))
	//{
	//	wxRemoveFile(image_path);
	//}
	//msg = "图片转移完成";
	//MyLog::LogMessage(msg.mb_str());
	cout << "Image Change" << endl;
}
//粒子检测
bool ImageModel::OnLiziDetection(wxString grp_path, string grp_name, wxString org_path, string org_name)
{
	cv::String cv_str_grp_path = grp_path.c_str();
	cv::String cv_str_org_path = org_path.c_str();
	bool lizi_flag = false;
	
//	bool is_grpOrL = false;
	wxString msg = "";

	cv::Mat image_grp = cv::imread(cv_str_grp_path);
	cv::Mat image_org = cv::imread(cv_str_org_path);

	long long blob_num = 0;
	double *blobs_det = NULL;
		
	msg = "粒子算法开始";
	MyLog::LogMessage(msg.c_str());

	MIL_ID MilApplicantion, MilSystem, MilDisplay;
	MappAllocDefault(M_DEFAULT, &MilApplicantion, &MilSystem, /*M_NULL*/&MilDisplay, M_NULL, M_NULL);

	bool b_find_recip = false;
	for (list<Image_Properties>::iterator it = m_list_image_model.begin(); it != m_list_image_model.end(); it++)
	{
		wxString temp_name = it->image_name;
		if (strstr(grp_path, temp_name))
		{
			b_find_recip = true;
			bool flag_result = true;
			//flag_result = BoolDetection(image_grp.data, image_grp.cols, image_grp.rows, image_org.data, image_org.cols, image_org.rows, blob_num, &blobs_det, MilSystem, 160, 1, 100, lizi_flag);
			break;
		}
	}

	if (!b_find_recip)
	{
		//BoolDetection(image_grp.data, image_grp.cols, image_grp.rows, image_org.data, image_org.cols, image_org.rows, blob_num, &blobs_det, MilSystem, 160, 5, 180, lizi_flag);
	}
	MappFreeDefault(MilApplicantion, MilSystem,/*M_NULL*/MilDisplay, M_NULL, M_NULL);

	msg = "粒子算法结束";
	MyLog::LogMessage(msg.c_str());
	
	//转移图片
	wxString wxs_Transfer_Path = "";
	wxString wxs_Transfer_grp_path = "";
	wxString wxs_Transfer_org_path = "";
	size_t temp_pos = grp_path.find("]");
	
	wxs_Transfer_Path = grp_path.substr(0, temp_pos + 6) + grp_path.substr(temp_pos + 17, 6) + "-HasBeenTesting\\";

	wxs_Transfer_grp_path = wxs_Transfer_Path + grp_name;
	wxs_Transfer_org_path = wxs_Transfer_Path + org_name;

	msg = "开始转移图片";
	MyLog::LogMessage(msg.c_str());

	//转移图片位置
	if (!wxDir::Exists(wxs_Transfer_Path))
	{
		wxDir::Make(wxs_Transfer_Path);
	}
	cout << "11== " << grp_path << "22= " << wxs_Transfer_grp_path;
	if (wxCopyFile(grp_path, wxs_Transfer_grp_path))
	{
		wxRemoveFile(grp_path);
	}

	msg = "转移完成grp";
	MyLog::LogMessage(msg.c_str());

	if (wxCopyFile(org_path, wxs_Transfer_org_path))
	{
		wxRemoveFile(org_path);
	}

	msg = "转移完成org";
	MyLog::LogMessage(msg.c_str());

	return lizi_flag;
}

bool ImageModel::GotoLizi(wxString Round_robin_path, wxString target_path)
{
	wxArrayString img_list;
	wxString msg = "";
//	bool b_not_find_grp_or_org = false;
	bool b_result = true;

	wxDir::GetAllFiles(Round_robin_path, &img_list, "*.jpg", wxDIR_DEFAULT);
	if (img_list.size() == 0)
	{
		return b_result;
	}

	for (size_t i_img = 0; i_img < img_list.size(); i_img++)
	{
		if (strstr(img_list[i_img], "-HasBeenTesting"))
		{
			continue;
		}

		if (strstr(img_list[i_img], "_SUB_"))
		{
			wxString temp_grp_path = "";
			wxString temp_org_path = "";
			wxString temp_grp_name = "";
			wxString temp_org_name = "";
			if (strstr(img_list[i_img], "_grp_"))
			{
				temp_grp_path = img_list[i_img];
				size_t grp_name_pos = temp_grp_path.find("[");
				temp_grp_name = img_list[i_img].substr(grp_name_pos);
				string str_grp = temp_grp_name.c_str();

				size_t pos_grp = temp_grp_path.find("grp");
				temp_org_path = temp_grp_path.substr(0, pos_grp) + "org" + temp_grp_path.substr(pos_grp + 3);

				size_t org_name_pos = temp_grp_path.find("[");
				temp_org_name = temp_org_path.substr(org_name_pos);
				string str_org = temp_org_name.c_str();

				//判断文件是否存在
				fstream fs_grp_file;
				string fs_file_name = temp_grp_path.c_str();
				fs_grp_file.open(fs_file_name, ios::in);
				if (fs_grp_file)
				{
					fstream fs_org_file;
					string fs_file_name = temp_org_path.c_str();
					fs_org_file.open(fs_file_name, ios::in);
					if (!fs_org_file)
						continue;
					fs_org_file.close();
				}
				else
					continue;
				fs_grp_file.close();

				msg = "进入粒子算法";
				MyLog::LogMessage(msg.c_str());

				bool lizi_result = true;
				DWORD  time_1 = GetTickCount();
				lizi_result = OnLiziDetection(temp_grp_path, str_grp, temp_org_path, str_org);

				DWORD  time_2 = GetTickCount();
				cout << "lizi >> " << "All time:" << (time_2 - time_1) / 1000.0 << "s" << endl;
				msg = "结束粒子算法";
				MyLog::LogMessage(msg.c_str());
				//如果粒子结果为true，则继续取后面的图检测，直到没图，如果为false，则不需要继续粒子检
				if (lizi_result)
					continue;
				else
				{
					b_result = false;
				}
			}
			else if (strstr(img_list[i_img], "_org_"))
			{
				temp_org_path = img_list[i_img];
				size_t org_name_pos = temp_org_path.find("[");
				temp_org_name = img_list[i_img].substr(org_name_pos);
				string str_org = temp_org_name.c_str();

				size_t pos_org = temp_org_path.find("org");
				temp_grp_path = temp_org_path.substr(0, pos_org) + "grp" + temp_org_path.substr(pos_org + 3);

				size_t grp_name_pos = temp_grp_path.find("[");
				temp_grp_name = temp_grp_path.substr(grp_name_pos);
				string str_grp = temp_grp_name.c_str();

				//判断文件是否存在
				fstream fs_org_file;
				string fs_file_name = temp_org_path.c_str();
				fs_org_file.open(fs_file_name, ios::in);
				if (fs_org_file)
				{
					fstream fs_grp_file;
					string fs_file_name = temp_grp_path.c_str();
					fs_grp_file.open(fs_file_name, ios::in);
					if (!fs_grp_file)
						continue;

					fs_grp_file.close();
				}
				else
					continue;
				fs_org_file.close();

				msg = "进入粒子算法";
				MyLog::LogMessage(msg.c_str());

				bool lizi_result = true;
				lizi_result = OnLiziDetection(temp_grp_path, str_grp, temp_org_path, str_org);

				msg = "结束粒子算法";
				MyLog::LogMessage(msg.c_str());
				//如果粒子结果为true，则继续取后面的图检测，直到没图，如果为false，则不需要继续粒子检
				if (lizi_result)
					continue;
				else
				{
					b_result = false;
				}		
			}
			else
			{
				continue;
			}
		}
	}

	return b_result;
}

//自动检测
bool ImageModel::OnSelfMotion()
{
	wxString msg;

	if (m_save_path.empty() || m_save_path_csv.empty() || m_save_path_img.empty() || m_save_path_excel.empty())
	{
		return false;
	}

	if (false == m_testing_start_flag)
	{
		if (!m_list_image_model.size())
		{
			wxMessageBox(_("模板为空，请先制作模板"), _("Error"));
			return false;
		}

		msg = "准备开启自动检测";
		MyLog::LogMessage(msg.mb_str());
		m_testing_start_flag = true;
		this->SetDrawRect(false, m_draw_pen_state);
		m_model_panel->Hide();
	}
	else
	{
		msg = "准备停止自动检测";
		MyLog::LogMessage(msg.mb_str());
		m_testing_start_flag = false;

		this->SetDrawRect(true, m_draw_pen_state);
		m_model_panel->Show();
	}

	return true;
}
//获取当前时间
string ImageModel::GetNowTime(int flag)
{
	time_t timep;
	time(&timep);
	char tmp[64];
	if (1 == flag)
	{
		strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
	}
	else
	{
		strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&timep));
	}
	return tmp;
}
//保存缺陷图
void ImageModel::SaveImageData(wxImage image, string name, string image_name, wxString save_path)
{
	wxString temp_image_name = image_name.c_str();
	temp_image_name = temp_image_name.SubString(0, temp_image_name.size() - 5);
	wxImage temp_image = image;
#ifdef __STATE_ELICITATION__

	wxString file_name = save_path + "\\" + temp_image_name;
	m_save_directory_name = file_name.substr(0, file_name.find_last_of('\\'));
	wxString save_pic_name = temp_image_name.substr(temp_image_name.find_last_of('\\') + 1);
	file_name += "-HasBeenTesting";
	if (!wxDir::Exists(file_name))
	{
		wxDir::Make(file_name);
	}

	if (strstr(name.c_str(), "yw"))
	{
		temp_image.SaveFile(file_name + "\\" + name + ".jpg");
	}
	else
	{
		file_name = file_name + "\\AI-" + save_pic_name + "\\";
		if (!wxDir::Exists(file_name))
		{
			wxDir::Make(file_name);
		}
		temp_image.SaveFile(file_name + "\\" + name + ".jpg");
	}

#else

	wxString H_path = m_save_path_img + "\\Result_Image";

	if (!wxDir::Exists(H_path))
	{
		wxDir::Make(H_path);
	}

	if (strstr(name.c_str(), "yw"))
	{
		H_path += "\\Arithmetic_Img\\";
	}
	else
	{
		H_path += "\\AI_Img\\";
	}
	if (!wxDir::Exists(H_path))
	{
		wxDir::Make(H_path);
	}

	wxString wxs_now_time = GetNowTime(0).c_str();

	H_path += wxs_now_time;

	if (!wxDir::Exists(H_path))
	{
		wxDir::Make(H_path);
	}

	wxString directory_name = "";
	directory_name = temp_image_name.substr(0, temp_image_name.find_last_of('\\'));
	directory_name = directory_name.substr(directory_name.find_last_of('\\') + 1);
	wxString file_name = H_path + "\\" + directory_name;

	if (!wxDir::Exists(file_name))
	{
		wxDir::Make(file_name);
	}
	m_save_directory_name = file_name;
	file_name = file_name + "\\" + temp_image_name.substr(temp_image_name.find_last_of('\\') + 1);
	wxString save_pic_name = temp_image_name.substr(temp_image_name.find_last_of('\\') + 1);
	file_name += "-HasBeenTesting";
	if (!wxDir::Exists(file_name))
	{
		wxDir::Make(file_name);
	}

	temp_image.SaveFile(file_name + "\\" + name + ".jpg");
#endif // __STATE_ELICITATION__
}
//保存缺陷位置
wxRect ImageModel::SaveNGRect(wxRect wx_rect, cv::Rect cv_rect)
{
	wxRect temp_rect;
	//x
	if (cv_rect.x - wx_rect.x - 25 > 0)
	{
		temp_rect.x = cv_rect.x - wx_rect.x - 25;
	}
	else
	{
		temp_rect.x = 0;
	}
	//y
	if (cv_rect.y - wx_rect.y - 25 > 0)
	{
		temp_rect.y = cv_rect.y - wx_rect.y - 25;
	}
	else
	{
		temp_rect.y = 0;
	}
	//w
	if (cv_rect.width < wx_rect.width)
	{
		if (temp_rect.x + cv_rect.width + 50 < wx_rect.x + wx_rect.width)
		{
			temp_rect.width = cv_rect.width + 50;
		}
		else
		{
			temp_rect.width = cv_rect.width;
		}
	}
	else
	{
		temp_rect.width = wx_rect.width;
	}
	//h
	if (cv_rect.height < wx_rect.height)
	{
		if (temp_rect.y + cv_rect.height + 50 < wx_rect.y + wx_rect.height)
		{
			temp_rect.height = cv_rect.height + 50;
		}
		else
		{
			temp_rect.height = cv_rect.height;
		}
	}
	else
	{
		temp_rect.height = wx_rect.height;
	}

	return temp_rect;
}
// 打开/关闭 自动检测按钮标志位
void ImageModel::PlanOneAutoBtnFlag(bool flag)
{
	if (flag)
	{
		if (NULL != m_socket_center)
		{
			m_socket_center->Run();
			m_socket_center->SetBtnAutoFlag(flag);
			m_socket_center->SetRunFlag(flag);
		}
		else
		{
			string temp_ip = m_save_ip.c_str();
			m_socket_center = new SocketCenter(m_frame, temp_ip, m_save_port/*"192.168.20.23", 4000*/);
			if (!m_socket_center->CreateSocket())
			{
				m_control_window->SetConnectBtnStatus(true);
				return;
			}
			m_socket_center->Create();
			m_socket_center->SetReadAddr(m_save_read_addr);
			m_socket_center->SetReadType(m_save_read_type);
			m_socket_center->SetWriteOKAddr(m_save_write_OK_addr);
			m_socket_center->SetWriteOKType(m_save_write_OK_type);
			m_socket_center->SetWriteNGAddr(m_save_write_NG_addr);
			m_socket_center->SetWriteNGType(m_save_write_NG_type);
			m_socket_center->SetBtnAutoFlag(flag);
			m_socket_center->SetRunFlag(flag);
			m_socket_center->Run();
		}
	}
	else
	{
		if (NULL != m_socket_center)
		{
			m_socket_center->SetBtnAutoFlag(flag);
			m_socket_center->SetRunFlag(flag);

			Sleep(1);
//			delete m_socket_center;
			m_socket_center = NULL;
		}
	}
	
}




void ImageModel::PostImageFun(PostPara& para,/* curl_interface& curlIf,*/ wxImage& tempImage)
{
	unique_lock<mutex> l_lock(m_mutexPost);
	img_num = 0;
	curl_interface curl_if(m_save_ai_path.ToStdString().c_str(), 15);
	list<SplitRect>* l_spitRect = para.m_splitList;
	//ImageU1 l_puzzle(para.m_puzzle.m_width,para.m_puzzle.m_height);
	//l_puzzle = para.m_puzzle;
	vector<wxRect> l_rectVec = para.m_errRedVec;

	img_num = PostAnAllImage2AI(*l_spitRect, para.m_puzzle, &curl_if, *s_image_temp, 5, tempImage, l_rectVec, m_rectList, m_imageName, m_imagePath);
	l_lock.unlock();

	if (-2 == img_num || -1 == img_num)
	{
		unique_lock<mutex> l_lock(m_mutexPic);
		m_aiNgNum = wx_vector_list.size();
		l_lock.unlock();
	}
	else
	{
		unique_lock<mutex> l_lock(m_mutexPic);
		m_aiNgNum += img_num;
		l_lock.unlock();
	}

}

void ImageModel::ShowPopDlg()
{
	if (m_popUpDlg == nullptr)
	{
		m_popUpDlg = new AlignFailFrm(m_frame);
		m_popUpDlg->OnShowFail();
	}
	else
	{
		m_popUpDlg->OnShowFail();
	}
}

void ImageModel::HidePopDlg()
{
	if (m_popUpDlg != nullptr)
	{
		delete m_popUpDlg;
		m_popUpDlg = nullptr;
	}
}

void ImageModel::ChangeResult()
{
	int OK_num = 0;
	int Ng_num = 0;
	float Ratio_Num = 0;
	wxString msg = "";
	msg = "开始计算OK或者NG片子数量";
	MyLog::LogMessage(msg.c_str());

	{
		OK_num += m_preOkNum;
		Ng_num += m_preNgNum;
	}
	//todo1
	for (list<paper_file>::iterator it = m_list_paper_file.begin(); it != m_list_paper_file.end(); it++)
	{
		if (it->paper_result/* && it->paper_lizi_result*/)
		{
			OK_num += 1;
		}
		else
		{
			Ng_num += 1;
		}

	}

	Ratio_Num = (float)Ng_num / ((float)OK_num + (float)Ng_num);


	msg = "结果面板添加OK、NG、Ratio数据";
	MyLog::LogMessage(msg.c_str());

	m_resultdlg_winodw->SetEditOK(OK_num);
	m_resultdlg_winodw->SetEditNG(Ng_num);
	m_resultdlg_winodw->SetEditRatio(Ratio_Num);

	msg = "结束添加";
	MyLog::LogMessage(msg.c_str());
}

bool ImageModel::PostAnImage2AI(gf::Point2d top_left, ImageMsg *im, curl_interface *curl_if, wxImage &ng_image, double len, wxString filename, int num, wxRect wx_rect, cv::Rect cv_rect, list<wxRect> &temp_rect, string image_name, wxString save_path)
{
	wxRect r_ai(top_left.x, top_left.y, 224, 224);

	bool is_suc = false;
	char *content = NULL;

	ImageU1 r_img/* = m_test_image.GetSubImage(r_ai)*/;
	r_img.CopyFromWxImage(ng_image);

	if (curl_if->post_gray_raw_type("xxxx.jpg", r_img.m_data, r_img.m_width, r_img.m_height, &content) && content != NULL)
	{
		cout << "content: " << content << endl;
		if (content[0] == '{')
		{
			ai_result_type art;
			if (json_resolverxxx(content, art))
			{
				if (art.succes)
				{
					is_suc = true;
					//cout << art.id << endl;
					size_t type_sum = art.predictions.size();
					for (size_t i = 0; i < type_sum; i++)
					{
						size_t count_sum = art.predictions[i].Pos.size();
						for (size_t j = 0; j < count_sum; j++)
						{
							AIresult(art.predictions[i].Pos[j].Pos[0]/* + r_ai.x*/,
								art.predictions[i].Pos[j].Pos[1]/* + r_ai.y*/,
								art.predictions[i].Pos[j].Pos[2]/* + r_ai.width*/,
								art.predictions[i].Pos[j].Pos[3]/* + r_ai.height*/,
								len,
								ng_image, num, filename, wx_rect, cv_rect, temp_rect, image_name, save_path);
						}
						//cout << art.predictions[i].label << endl;
					} // for

					return true;
				}
				else
				{
					cout << "SampleMsg >> " << "AI false. ai_result_type.success is false." << endl;
					wxLogError(wxString::Format("%s(%d) AI false. ai_result_type.success is false.", __FILE__, __LINE__));
				}
			}
			else
			{
				cout << "SampleMsg >> " << "json_resolver error." << endl;
				wxLogError(wxString::Format("%s(%d) json_resolver error.", __FILE__, __LINE__));
			}
		}
	}
	else
	{
		cout << "SampleMsg >> " << "post gray raw fail, or no content returned." << curl_if->get_code_res() << endl;
		wxLogError(wxString::Format("%s(%d) post gray raw fail, or no content returned.", __FILE__, __LINE__));
	}

	if (!is_suc)
		AIresult(0, 0, r_ai.width, r_ai.height, len, ng_image, 0, filename, wx_rect, cv_rect, temp_rect, image_name, save_path);

	return false;
}

int ImageModel::PostAnAllImage2AI(list<SplitRect> &list_split, ImageU1 &split_img, curl_interface *curl_if, SingleImage &sing_img, double len, wxImage &ng_image, vector<wxRect> &ErrRect, list<wxRect> &temp_rect, string temp_pic_name, wxString temp_path)
{
	char *content = NULL;

	DWORD  time_1 = GetTickCount();
	if (curl_if->post_gray_raw_type("xxxx.jpg", split_img.m_data, split_img.m_width, split_img.m_height, &content) && content != NULL)
	{
		cout << "content: " << content << endl;
		wxString msg = wxString::Format("AI content, %s", content);
		MyLog::LogMessage(msg.c_str());

		if (content[0] == '{')
		{
			ai_result_type art;
			if (json_resolverxxx(content, art))
			{
				if (art.succes)
				{
					//cout << art.id << endl;
					int i_AI_num = 0;
					size_t type_sum = art.predictions.size();
					
					for (size_t i = 0; i < type_sum; i++)
					{
						size_t count_sum = art.predictions[i].Pos.size();
						i_AI_num = i_AI_num + count_sum;

						for (size_t j = 0; j < count_sum; j++)
						{
							int img_size = 224;
							int img_pos_x = art.predictions[i].Pos[j].Pos[0];
							int img_pos_y = art.predictions[i].Pos[j].Pos[1];

							sing_img.GetErrPosImage(img_pos_x, img_pos_y);
							list<SplitRect> ::iterator it = list_split.begin();
							advance(it, sing_img.GetSelectImageNum());

							vector<wxRect>::iterator iter = ErrRect.begin();
							advance(iter, sing_img.GetSelectImageNum());

							AIresult(art.predictions[i].Pos[j].Pos[0]/* + r_ai.x*/,
								art.predictions[i].Pos[j].Pos[1]/* + r_ai.y*/,
								art.predictions[i].Pos[j].Pos[2]/* + r_ai.width*/,
								art.predictions[i].Pos[j].Pos[3]/* + r_ai.height*/,
								len,
								ng_image, j, "AI", wxRect(it->getRoi().getX(), it->getRoi().getY(), it->getRoi().getWidth(), it->getRoi().getHeight()), \
								cv::Rect(iter->x, iter->y, iter->width, iter->height), temp_rect, temp_pic_name, temp_path);
						}
						//cout << art.predictions[i].label << endl;
					} // for

					DWORD  time_2 = GetTickCount();
					cout << "SampleMsg >> " << "AI trans  :" << (time_2-time_1)/1000.0 <<"s"<<endl;

					wxString msg = wxString::Format("SampleMsg >> AI trans: %f s", (time_2 - time_1) / 1000.0);
					MyLog::LogMessage(msg.c_str());

					return i_AI_num;
				}
				else
				{
					cout << "SampleMsg >> " << "AI false. ai_result_type.success is false." << endl;
					wxLogError(wxString::Format("%s(%d) AI false. ai_result_type.success is false.", __FILE__, __LINE__));

					wxString msg = "AI false.ai_result_type.success is false.";
					MyLog::LogError(msg.c_str());
				}
			}
			else
			{
				cout << "SampleMsg >> " << "json_resolver error." << endl;
				wxLogError(wxString::Format("%s(%d) json_resolver error.", __FILE__, __LINE__));

				wxString msg = "json_resolver error.";
				MyLog::LogError(msg.c_str());
			}
		}
		else
		{
			wxString msg = wxString::Format("AI returns data error, %s", content);
			MyLog::LogError(msg.c_str());
		}
	}
	else
	{
		cout << "SampleMsg >> " << "post gray raw fail, or no content returned." << curl_if->get_code_res() << endl;

		wxString msg = wxString::Format("post gray raw fail, or no content returned. %d", curl_if->get_code_res());
		MyLog::LogError(msg.c_str());

		return -2;

	}

	return -1;
}

bool ImageModel::AIresult(double xx, double yy, double rw, double rh, double len, wxImage &NG_image, int num, wxString filename, wxRect wx_rect, cv::Rect cv_rect, list<wxRect> &temp_rect, string image_name, wxString save_path)
{
	s_verify sy;
	sy.m_pos_code = wxString::Format("%s-%d", filename, num);
	sy.m_rect = wxRect(100 * num, 100 * num, 500, 500);
	sy.m_img = NG_image.GetSubImage(wxRect(wx_rect.x, wx_rect.y, wx_rect.GetWidth(), wx_rect.GetHeight()));
	sy.m_ng_rect = SaveNGRect(wx_rect, cv_rect);
	SaveImageData(sy.m_img, sy.m_pos_code, image_name, save_path);
	sy.m_pos_x = 23.789;
	sy.m_err_num = 1;
	sy.m_len = len;
	sy.m_line_name = "X10101";
	sy.m_pos_y = 32.789;
	sy.m_type = 1;

	m_list_data.push_back(sy);
	temp_rect.push_back(sy.m_ng_rect);

	wxString msg;
	msg = wxString::Format("已保存缺陷图%d", num);
	MyLog::LogMessage(msg.mb_str());

	return true;
}

void ImageModel::SetImageResult(wxString result)
{
	m_imageResult = result;
}

wxString ImageModel::GetImageResult()
{
	return m_imageResult;
}

void ImageModel::SetDrawRect(bool flag, int pen_state)
{
	m_is_draw_rect = flag;
	m_refresh_window->SetIsDrawRect(m_is_draw_rect);
	m_refresh_window->SetDrawPenState(pen_state);
	m_draw_pen_state = pen_state;
	
	if (NULL == m_model_panel)
	{
		m_model_panel = new ModelPanelWindow(m_frame);
	}

	if (flag)
	{
		m_model_panel->Show();
	}
	else
	{
		m_model_panel->Hide();
	}
}

void ImageModel::SetDrawNoTestRect(bool flag,int penState)
{
	m_is_draw_rect = flag;
	m_refresh_window->SetIsDrawRect(m_is_draw_rect);
	m_refresh_window->SetDrawPenState(penState);
	m_draw_pen_state = penState;

	if (nullptr == m_noTestDlg)
	{
		m_noTestDlg = new NoTestDlg(m_frame);
	}

	if (flag)
	{
		m_noTestDlg->Show();
	}
	else
	{
		m_noTestDlg->Hide();
	}
}

void ImageModel::OnAddModel()
{
	Image_Properties temp_model = m_refresh_window->GetImageModel();
	bool b_is_find = false;

	for (list<Image_Properties>::iterator it = m_list_image_model.begin(); it != m_list_image_model.end(); it++)
	{
		if (it->image_name == temp_model.image_name)
		{
			b_is_find = true;
			it->image_mark.push_back(temp_model.image_mark.front());
			it->image_point.push_back(temp_model.image_point.front());
			break;
		}
	}
	if (!b_is_find)
	{
		m_list_image_model.push_back(temp_model);
	}

// 	for (list<Image_Properties>::iterator it = m_list_image_model.begin(); it != m_list_image_model.end(); it++)
// 	{
// 		cv::imwrite("addimage.jpg", it->Model_image);
// 	}
	
	m_model_panel->GetModelPanel()->AddData(m_list_image_model);
}

void ImageModel::OnClearModel()
{
	if (NULL == m_model_panel)
	{
		m_model_panel = new ModelPanelWindow(m_frame);
	}
	m_model_panel->DeleteAllModel();
	m_list_image_model.clear();
	m_list_recipe.clear();
}

void ImageModel::SetDataorganization(DataOrganization *data, string name, string path)
{
	list<Element*> temp_element_list = data->GetElementList();
	m_element_list.assign(temp_element_list.begin(), temp_element_list.end());

	m_recipe_name = name;
	m_recipe_path = path;

	this->OnDataRecotery();
}

void ImageModel::OnDataRecotery()
{
	list<Image_Properties> temp_element_list;
	wxString msg = "dat配方数据恢复";
	MyLog::LogMessage(msg.mb_str());

	for (list<Element*>::iterator it = m_element_list.begin(); it != m_element_list.end(); it++)
	{
		Image_Properties temp_image_prop;
		temp_image_prop.image_name = (*it)->GetImageName();
		list<cv::Mat> temp_mark = (*it)->GetImageMark();
		temp_image_prop.image_mark.assign(temp_mark.begin(), temp_mark.end());
		list<cv::Point> temp_point = (*it)->GetImagePoint();
		temp_image_prop.image_point.assign(temp_point.begin(), temp_point.end());
		temp_image_prop.Model_image = (*it)->GetModelImage();
		temp_element_list.push_back(temp_image_prop);
	}

	m_list_image_model.clear();
	m_list_image_model.assign(temp_element_list.begin(), temp_element_list.end());

	msg = "数据恢复完成";
	MyLog::LogMessage(msg.mb_str());

	m_model_panel->GetModelPanel()->AddData(m_list_image_model);
	m_model_panel->Show();
}

void ImageModel::OnSaveModel()
{
	wxString msg = "";

	if (!m_list_image_model.size())
	{
		msg = "模版为空，无法保存";
		MyLog::LogError(msg.c_str());

		wxMessageBox(_("模版为空，无法保存"));
		return;
	}

	list<Element*> temp_element_list;
	for (list<Image_Properties>::iterator it = m_list_image_model.begin(); it != m_list_image_model.end(); it++)
	{
		Element *temp_element = new Element();
		temp_element->SetImageName(it->image_name);
		temp_element->SetImagePoint(it->image_point);
		temp_element->SetImageMark(it->image_mark);
		temp_element->SetModelIamge(it->Model_image);
		temp_element_list.push_back(temp_element);
	}

	m_element_list.clear();
	m_element_list.assign(temp_element_list.begin(), temp_element_list.end());

	DataOrganization *d_save = new DataOrganization(m_recipe_name, m_recipe_path);
	d_save->SetElementList(m_element_list);

	wxFileDialog dlg(m_frame, _("选择配方保存"), "./recipe", "", "dat files (*.dat) | *.dat", wxFD_OVERWRITE_PROMPT | wxFD_SAVE);
	if (wxID_OK == dlg.ShowModal())
	{
		string v_path = "";
		v_path = dlg.GetPath().c_str();
		m_detectPath = v_path;

		if (true == d_save->OnSave(v_path))
		{
			msg = wxString::Format("配方保存成功，路径为：%s", v_path);
			MyLog::LogMessage(msg.mb_str());

			wxMessageBox("配方保存成功");
		}
		else
		{
			msg = "配方保存失败";
			MyLog::LogWarning(msg.mb_str());

			wxMessageBox("配方保存失败，请清空配方后重新操作");
		}
	}
	else
	{
		msg = "用户取消选择配方";
		MyLog::LogMessage(msg.mb_str());

		dlg.Destroy();
		return;
	}
	dlg.Destroy();

	for (list<Element*>::iterator it = temp_element_list.begin(); it != temp_element_list.end(); it++)
	{
		delete (*it);
	}
}

void ImageModel::OnAddNoTestModel()
{
	bool l_isSame = false;
	NoTestImage_t l_noTest = m_refresh_window->GetNoTestImage();
	for (NoTestImage_t& noTest: m_noTestImageVec)
	{
		if (l_noTest.ImageName == noTest.ImageName)
		{
			RectPoint_t l_rectPoint = l_noTest.RectPointVec[0];
			noTest.RectPointVec.push_back(l_rectPoint);
			l_isSame = true;
			break;
		}
	}

	if (!l_isSame)
	{
		m_noTestImageVec.push_back(l_noTest);
	}

	m_noTestDlg->GetModelPanel()->AddData(m_noTestImageVec);
}

void ImageModel::OnClearNoTestModel()
{
	if (nullptr == m_noTestDlg)
	{
		m_noTestDlg = new NoTestDlg(m_frame);
	}

	m_noTestDlg->DeleteAllModel();
	m_noTestImageVec.clear();
	//to do 

}

void ImageModel::OnSaveNoTestModel()
{
	if (!m_noTestImageVec.size())
	{
		wxMessageBox(_("模版为空，无法保存"));
		return;
	}

	vector<NoTestElement> l_noTestEleVec;
	for (NoTestImage_t noTest : m_noTestImageVec)
	{
		NoTestElement l_element;
		l_element.SetImageName(noTest.ImageName);
		l_element.SetRectPointVec(noTest.RectPointVec);
		l_noTestEleVec.push_back(l_element);
	}

	NoTestData* l_noTestData = new NoTestData(m_noTestRecName, m_noTestRecPath);
	l_noTestData->SetNoTestElementVec(l_noTestEleVec);

	wxFileDialog l_dlg(m_frame, _("选择配方保存"), "./recipe", "", "dat files (*.dat) | *.dat", wxFD_OVERWRITE_PROMPT | wxFD_SAVE);

	if (wxID_OK == l_dlg.ShowModal())
	{
		string l_path = "";
		l_path = l_dlg.GetPath().c_str();

		if (true == l_noTestData->OnSave(l_path))
		{
			wxMessageBox("配方保存成功");
		}
		else
		{
			wxMessageBox("配方保存失败，请清空配方后重新操作");
		}

	}

	l_dlg.Destroy();
	if (l_noTestData != nullptr)
	{
		delete l_noTestData;
		l_noTestData = nullptr;
	}


}

std::vector<NoTestImage_t> ImageModel::GetNoTestImageVec()
{
	return m_noTestImageVec;
}

void ImageModel::OnNoPic()
{
	wxString msg;
	msg = "准备停止自动检测";
	MyLog::LogMessage(msg.mb_str());
	m_testing_start_flag = false;

	this->SetDrawRect(false, m_draw_pen_state);
	m_model_panel->Show();

	this->GetControlWindow()->SetSelfMotionBtnStatus(false);
}

void ImageModel::OnNGSingle()
{
	unsigned int data[256] = { 2 };
	string temp = m_save_write_NG_type.c_str();
	char* temp_write_type = (char*)temp.c_str();

	m_socket_center->SendMsg_Write(m_save_write_NG_addr, temp_write_type, 1, data); 
}

void ImageModel::SendReadSingleOver()
{
	unsigned int data[256] = { 1 };

	m_socket_center->SendMsg_Write(5601, "D", 1, data); 
}
//报表
void ImageModel::OnSaveFailurePics(bool result, int num)
{
	wxString wxs_now_report_path = "";
	wxString wxs_report_path = "Report\\";
	if (!wxDir::Exists(wxs_report_path))
		wxDir::Make(wxs_report_path);

	char nd[64] = { 0 };
	char time_A_or_P[64] = { 0 };
	time_t t = time((NULL));
	tm *tm_cur = localtime(&t);
	if (tm_cur->tm_hour < 8 || tm_cur->tm_hour > 20)
	{
		//t = t - 12 * 60 * 60;
		strftime(time_A_or_P, sizeof(time_A_or_P), "%Y-%m-%d-PM.xls", localtime(&t));
	}
	else
	{
		strftime(time_A_or_P, sizeof(time_A_or_P), "%Y-%m-%d-AM.xls", localtime(&t));
	}
	strftime(nd, sizeof(nd), "%Y%m%d\\", localtime(&t));

	wxString wxs_save_path_excel = m_save_path_excel + "\\Report\\";
	
	if (!wxDir::Exists(wxs_save_path_excel))
		wxDir::Make(wxs_save_path_excel);

	wxs_now_report_path = wxs_save_path_excel + nd;
	if (!wxDir::Exists(wxs_now_report_path))
		wxDir::Make(wxs_now_report_path);

	wxString exl_name = wxs_report_path + nd;
	if (!wxDir::Exists(exl_name))
		wxDir::Make(exl_name);

	exl_name += time_A_or_P;

	wxString temp_exl_name = wxs_now_report_path + time_A_or_P;

	if (access(exl_name, 0) != 0)
	{
		wxFile tf(exl_name, wxFile::write_append);
		tf.Write("ID_NAME\t");
		tf.Write("RESULT\t");
		tf.Write("NUM\t");
		tf.Write("PATH\t");
		tf.Write("OPEN\t");
		tf.Write("INSPECTING_TIME\t");
		tf.Write("RATIO\n");
		tf.Close();
	}
	wxFile fp(exl_name, wxFile::write_append);
	//ID_NAME
	wxString wxs_id_name = m_image_path;
	wxs_id_name = wxs_id_name.substr(0, wxs_id_name.find_last_of('\\', wxs_id_name.Len() - 2));
	size_t pos_find = wxs_id_name.find_last_of('\\', wxs_id_name.Len() - 2);
	wxs_id_name = wxs_id_name.substr(pos_find + 1);
	fp.Write(wxs_id_name);
	fp.Write("\t");
	//RESULT
	if(result)
		fp.Write("OK");
	else
		fp.Write("NG");
	fp.Write("\t");
	//NUM
	fp.Write(wxString::Format("%d\t", num));
	//PATH
	fp.Write(m_save_directory_name);
	fp.Write("\t");

	string s_exl_name = exl_name.c_str();
	bool is_suc = true;
	double ratio = 0;
	wxString wxs_ratio = "";
	is_suc = OnCountRadio(s_exl_name, wxs_ratio);
	//OPEN
	if (!result)
		fp.Write(wxString::Format("=HYPERLINK(D%d, \"Open\")", m_excel_path_now_num));
	m_excel_path_now_num++;
	fp.Write("\t");
	//INSPECTING_TIME
	t = time((NULL));
	char ti[64] = { 0 };
	strftime(ti, sizeof(ti), "%Y-%m-%d %H:%M:%S", localtime(&t));
	fp.Write(ti);
	fp.Write("\t");
 	//RATIO
	if (is_suc)
		fp.Write(wxs_ratio);
	fp.Write("\t\n");
	fp.Close();

	wxCopyFile(exl_name, temp_exl_name);
}

bool ImageModel::OnCountRadio(string file_name, wxString &wxs_ratio)
{
	vector<string> l_countVec;
	string l_line;
	ifstream l_file(file_name);
	if (l_file.fail())
	{
		cout << "File not found" << endl;
		return false;
	}

	bool l_bol = true;
	int l_index = 0;
	int all_count = 0;
	int ng_count = 0;
	double ratio = 0;
	while (getline(l_file, l_line)/* && l_file.good()*/)
	{
		if (l_index != 1)
		{
			l_index++;
			continue;
		}
		all_count++;

		vector<string> l_strVec;
		string l_str = l_line;
		string l_countS = SplitString(l_line, l_strVec, "\t");

		if (!isdigit(l_countS[0]))
		{
			l_file.close();
			break;
		}
		int l_count = std::stoi(l_countS);

		if (l_count != 0)
			ng_count++;
	}
	l_file.close();
	m_excel_path_now_num = all_count + 1;

	ratio = (double)ng_count / (double)all_count;
	wxs_ratio = wxString::Format("%.3lf", ratio);
	
	return true;
}

std::string ImageModel::SplitString(const string& line, vector<string>& vectorStr, const string& delimiter)
{
	string::size_type l_index, l_index1;
	l_index = line.find(delimiter);
	l_index1 = 0;

	while (string::npos != l_index)
	{
		vectorStr.push_back(line.substr(l_index1, l_index - l_index1));

		l_index1 = l_index + delimiter.size();
		l_index = line.find(delimiter, l_index1);
	}
	if (l_index1 != line.length())
	{
		vectorStr.push_back(line.substr(l_index1));
	}
	string l_count = vectorStr[2];

	return l_count;
}
static int m_size = 0;
void ImageModel::OnSaveRecord(bool result, int num)
{
	wxString l_path = "Record\\";
	wxString l_reportPath = m_save_path_excel + "\\Report\\";
	wxString l_readPath = l_reportPath + "record.xls";

	if (!wxDir::Exists(l_path))
	{
		wxDir::Make(l_path);
	}
	if (!wxDir::Exists(l_reportPath))
	{
		wxDir::Make(l_reportPath);
	}
	int l_tempNgNum = 0;
	int l_tempOkNum = 0;
	int l_tempAllPices = 0;
	int l_tempRadio = 0;

	char l_day[64] = { 0 };
	char l_TimeAmPm[64] = { 0 };

	time_t t = time((NULL));
	tm *tm_cur = localtime(&t);
	if (tm_cur->tm_hour < 8 || tm_cur->tm_hour > 19)
	{
		strftime(l_TimeAmPm, sizeof(l_TimeAmPm), "%Y/%m/%d-AM", localtime(&t));
	}
	else
	{
		strftime(l_TimeAmPm, sizeof(l_TimeAmPm), "%Y/%m/%d-PM", localtime(&t));
	}
	strftime(l_day, sizeof(l_day), "%Y/%m/%d", localtime(&t));

	wxString l_recordXls = l_path + "record.xls";
	vector<string> l_strVec;
	ReadRecordXls(string(l_recordXls.mb_str()), l_strVec);

	//const char* l_url = l_recordXls.c_str();
	//ofstream l_outFile(l_url, ios::out);
	//if (!l_outFile.is_open())
	//{
	//	l_outFile.close();
	//	wxMessageBox("请关闭Record.xls!", "waring");
	//	return;
	//}
	wxFile l_fileXls(l_recordXls, wxFile::write);
	l_fileXls.Flush();

	l_fileXls.Write("Date\t");
	l_fileXls.Write("NGNum\t");
	l_fileXls.Write("OKNum\t");
	l_fileXls.Write("AllPices\t");
	l_fileXls.Write("RATIO\n");

	if (l_strVec.size() != 0)
	{
		wxString l_timeStr(l_TimeAmPm, wxConvUTF8);
		int l_lastIndex = m_allLineVec.size();
		if (l_lastIndex == 0)
		{
			return;
		}
		vector<string>& l_lastLine = m_allLineVec[l_lastIndex-1];

		string l_AmOrPm = l_lastLine[0];
		wxFileOffset l_offset = sizeof(l_AmOrPm);
		string l_NgNum = l_lastLine[1];
		string l_OkNum = l_lastLine[2];
		string l_allPices = l_lastLine[3];

		wxString l_AmOrPm1 = l_AmOrPm.c_str();

		wxFileOffset l_offset1 = sizeof(m_getFirstLine);
		l_fileXls.Seek(l_offset1);
//		l_fileXls.Write("\n");
		if (l_timeStr == l_AmOrPm1)
		{
			int l_allPices = 0;
			int l_radio = 0;

			if (result)
			{
				int l_ngNum = std::stoi(l_NgNum);
				int l_okNum = std::stoi(l_OkNum);
				l_okNum++;
				l_allPices = l_ngNum + l_okNum;
				l_radio = l_ngNum / l_allPices;
				l_lastLine[2] = wxString::Format("%d", l_okNum);
				l_lastLine[3] = wxString::Format("%d", l_allPices);
				l_lastLine[4] = wxString::Format("%d", l_radio);
				for (vector<string> lineVec: m_allLineVec)
				{
					l_fileXls.Write(lineVec[0]);
					l_fileXls.Write("\t");

					l_fileXls.Write(lineVec[1]);
					l_fileXls.Write("\t");

					l_fileXls.Write(lineVec[2]);
					l_fileXls.Write("\t");

					l_fileXls.Write(lineVec[3]);
					l_fileXls.Write("\t");

					l_fileXls.Write(lineVec[4]);
					l_fileXls.Write("\n");
				}

			}

			else
			{
				int l_ngNum = std::stoi(l_NgNum);
				int l_okNum = std::stoi(l_OkNum);
				l_ngNum++;
				l_allPices = l_ngNum + l_okNum;
				l_radio = l_ngNum / l_allPices;
				l_lastLine[1] = wxString::Format("%d", l_ngNum);
				l_lastLine[3] = wxString::Format("%d", l_allPices);
				l_lastLine[4] = wxString::Format("%d", l_radio);
				for (vector<string> lineVec : m_allLineVec)
				{
					l_fileXls.Write(lineVec[0]);
					l_fileXls.Write("\t");

					l_fileXls.Write(lineVec[1]);
					l_fileXls.Write("\t");

					l_fileXls.Write(lineVec[2]);
					l_fileXls.Write("\t");

					l_fileXls.Write(lineVec[3]);
					l_fileXls.Write("\t");

					l_fileXls.Write(lineVec[4]);
					l_fileXls.Write("\n");
				}

			}
			
		}
		else
		{

			for (vector<string> lineVec : m_allLineVec)
			{
				l_fileXls.Write(lineVec[0]);
				l_fileXls.Write("\t");

				l_fileXls.Write(lineVec[1]);
				l_fileXls.Write("\t");

				l_fileXls.Write(lineVec[2]);
				l_fileXls.Write("\t");

				l_fileXls.Write(lineVec[3]);
				l_fileXls.Write("\t");

				l_fileXls.Write(lineVec[4]);
				l_fileXls.Write("\n");
			}

			l_fileXls.Write(l_TimeAmPm);
			l_fileXls.Write("\t");

			if (result)
				l_tempOkNum++;
			else
				l_tempNgNum++;

			l_fileXls.Write(wxString::Format("%d\t", l_tempNgNum));
			l_fileXls.Write(wxString::Format("%d\t", l_tempOkNum));
		
			l_tempAllPices = l_tempOkNum + l_tempNgNum;
			l_fileXls.Write(wxString::Format("%d\t", l_tempAllPices));
			l_tempRadio = l_tempNgNum / l_tempAllPices;
			l_fileXls.Write(wxString::Format("%d\t", l_tempRadio));
			l_fileXls.Write("\n");
			l_fileXls.Close();

		}

	}
	else
	{
		wxFileOffset l_offset1 = sizeof(m_getFirstLine);
		l_fileXls.Seek(l_offset1);
		//l_fileXls.Write("\n");

		l_fileXls.Write(l_TimeAmPm);
		l_fileXls.Write("\t");

		if (result)
			l_tempOkNum++;
		else
			l_tempNgNum++;
		
		l_fileXls.Write(wxString::Format("%d\t", l_tempNgNum));
		l_fileXls.Write(wxString::Format("%d\t", l_tempOkNum));

		l_tempAllPices = l_tempOkNum + l_tempNgNum;
	  l_fileXls.Write(wxString::Format("%d\t", l_tempAllPices));
		l_tempRadio = l_tempNgNum / l_tempAllPices;
		l_fileXls.Write(wxString::Format("%d", l_tempRadio));
	//	l_fileXls.Write("\n");
		l_fileXls.Close();

	}

	wxCopyFile(l_recordXls,l_readPath);
}

void ImageModel::ReadRecordXls(const string& xls, vector<string>& vectorStr)
{
	wxString l_xls = xls.c_str();
	ifstream l_file(xls);
	string l_line;
	string l_lineLast;
	string l_test;

	int l_index = 0;
	if (l_file.fail())
	{
		cout << "File not found" << endl;
		return;
	}
	m_allLineVec.clear();

	while (getline(l_file, l_line))
	{
		if (l_index != 1)
		{
			m_getFirstLine = l_line;
			l_index++;
			continue;
		}
		if (l_line == "")
		{
			continue;
		}
		l_lineLast = l_line;
		SplitRecordString(l_lineLast, vectorStr, "\t");
	}

	
	
}

std::string ImageModel::SplitRecordString(const string& line, vector<string>& vectorStr, const string& delimiter)
{
	string::size_type l_index, l_index1;
	l_index = line.find(delimiter);
	l_index1 = 0;
	vectorStr.clear();
	while (string::npos != l_index)
	{
		vectorStr.push_back(line.substr(l_index1, l_index - l_index1));

		l_index1 = l_index + delimiter.size();
		l_index = line.find(delimiter, l_index1);
	}
	if (l_index1 != line.length())
	{
		vectorStr.push_back(line.substr(l_index1));
	}

	if (vectorStr.size() != 0)
	{
		m_allLineVec.push_back(vectorStr);
	}
	
	return "";
}

void ImageModel::Lock_(bool flag)
{
	if (flag)
	{
		m_mutex->Lock();
	}
	else
	{
		m_mutex->Unlock();
	}
}

int ImageModel::Task1(PVOID p)
{
	Lock_(true);
	DWORD time_1 = GetTickCount();
	pic_data temp_pic_Data = m_list_pic_data->front();
	m_list_pic_data->pop_front();
	//判断当前的图所在的文件夹一共有几张图片
	string::size_type position;
	position = temp_pic_Data.image_path.find("[");
	string s_t = "";
	if (position != temp_pic_Data.image_path.npos)
	{
		string temp_file = temp_pic_Data.image_path.substr(0, position - 1);
		char *t_c;
		t_c = (char *)malloc((temp_file.length() + 1) * sizeof(char*));
		strcpy_s(t_c, temp_file.length() + 1, temp_file.c_str());
		char *p_c = strrchr(t_c, '\\');

		if (*p_c != NULL)
		{
			s_t = p_c;
			s_t = s_t.substr(1);
			if (s_t != m_now_detection_file_name)
			{
				m_now_detection_file_name = s_t;

				wxArrayString img_list;
				wxDir::GetAllFiles(temp_file.c_str(), &img_list, "*jpg", wxDIR_DEFAULT);
				m_now_detection_file_all_num = 0;
				for (int i = 0; i < img_list.size(); i++)
				{
					if (!strstr(img_list[i], "_SUB_"))
					{
						m_now_detection_file_all_num++;
					}
				}
			}
		}
	}

	//cout << "OnLoadImage start:" << temp_pic_Data.image_name << endl;
	OnLoadImage(temp_pic_Data.image_path, temp_pic_Data.image_name);
	//cout << "OnLoadImage stop:" << temp_pic_Data.image_name << endl;

	cv::Mat temp_cvSrcmat = GetCvSrcmat();
	string temp_image_name = GetImageName();
	string temp_image_path = GetImagePath();
	int temp_mat_width = GetMatWidth();
	int temp_mat_height = GetMatHeight();
	Lock_(false);

//	cout << "OnDetection start:" << temp_image_name << endl;
	OnDetection(true, temp_cvSrcmat, temp_image_name, temp_image_path, temp_mat_width, temp_mat_height);
//	cout << "OnDetection stop:" << temp_image_name << endl;

	DWORD  time_2 = GetTickCount();
	cout << temp_image_name << ": ===Tasktime=== :" << (time_2 - time_1) / 1000.0 << "s" << endl;
	

	Lock_(true);
	m_now_detection_file_num++;
	if (m_now_detection_file_num == m_now_detection_file_all_num)
	{
		m_now_detection_file_num = 0;
		wxQueueEvent(m_frame->GetEventHandler(),
			new wxCommandEvent(wxEVT_THREAD, ID_CREATE_REJUDE_UI));

		wxString msg = "当前片子检测完毕";
		MyLog::LogMessage(msg.mb_str());
	}
	Lock_(false);

	return 0;
}