#include "AutomaticDetetionThread.h"
#include <thread>

#include <chrono>  

int	AutomaticDetetionThread::m_i_fin_file_num;

AutomaticDetetionThread::AutomaticDetetionThread(wxWindow *frame)
	:wxThread(wxTHREAD_DETACHED)
{
	m_frame = frame;
	m_b_run_flag = true;
	m_verifyDlg = NULL;
	m_type = 0;

	m_thread_pool = new ThreadPool(2, 10);
	m_mutex = new wxMutex();
	
	m_b_planone_run_flag = false;
	m_b_first = true;

	m_list_pic_data.clear();
	m_list_is_dection.clear();

	m_i_fin_file_num = 0;
	m_i_delta_T = 86400;
	m_wxs_dection_catalogue = "";

	busy_info = NULL;
	m_hasBeenTest = new HasBeenTest;
	m_picesName = "";
	m_isDetect = false;

}

AutomaticDetetionThread::~AutomaticDetetionThread()
{
	if (m_mutex)
	{
		delete m_mutex;
	}

	if (m_verifyDlg)
	{
		delete m_verifyDlg;
	}

	if (m_thread_pool)
	{
		delete m_thread_pool;
	};

	if (busy_info)
	{
		delete busy_info;
		busy_info = NULL;
	}
	if (m_hasBeenTest)
	{
		delete m_hasBeenTest;
		m_hasBeenTest = nullptr;
	}

	m_list_pic_data.clear();
	m_list_is_dection.clear();
}

void *AutomaticDetetionThread::Entry()
{
	wxString msg = "";
	
	while (m_b_run_flag)
	{
		if (TestDestroy())
		{
			msg = "AutomaticDetetionThread TestDestroy() error";
			MyLog::LogError(msg.c_str());
			wxMessageBox(msg);
			break;
		}

		wxArrayString img_list;
		wxArrayString l_dirArray;
		wxString temp_path = "";

		if (PLAN_ONE)
		{
			if (m_b_planone_run_flag)
			{
				msg = "m_b_planone_run_flag开启";
				MyLog::LogInfat(msg.c_str());

				if (busy_info != NULL)
				{
					delete busy_info;
					busy_info = NULL;
				}

				if (m_type == 2 || m_type == 1)
				{
					temp_path = m_gmodel->GetTestingPath() + "\\";
				}
				else
				{
					wxMessageBox("读取到未知载台信号", _("Error"));
					msg = "读取到未知载台信号";
					MyLog::LogError(msg.c_str());
					wxQueueEvent(m_frame->GetEventHandler(),
						new wxCommandEvent(wxEVT_THREAD, ID_NO_PIC));
					return 0;
				}
				char nd[64] = { 0 };
				char temp_now_time[64] = { 0 };
				time_t t = time((NULL));
				tm *tm_cur = localtime(&t);
				if (tm_cur->tm_hour < 8)
				{
					t = t - 12 * 60 * 60;
				}
				strftime(temp_now_time, sizeof(temp_now_time), "%Y%m%d", localtime(&t));

				temp_path = temp_path + temp_now_time + "\\";

				if (!m_b_run_flag)
				{
					return 0;
				}

				//获取子目录
				const char* filePath = temp_path.c_str();
				vector<string> files;
				vector<string> next_file;
				next_file.clear();
				this->getJustCurrentDir(filePath, files);
				wxString wxs_temp_filepath = "";
				for (vector<string>::iterator iter = files.begin(); iter != files.end(); iter++)
				{
					wxs_temp_filepath  = temp_path + iter->c_str() + "\\";
					const char* cfilePath = wxs_temp_filepath.c_str();
					vector<string> vfiles;
					this->getJustCurrentDir(cfilePath, vfiles);
					m_i_delta_T = 86400;
//==============================================================================================
					//在获取下一级子目录的创建时间，与当前时间比较，算出时间差，时间差最小的送入检测
					for (vector<string>::iterator it = vfiles.begin(); it != vfiles.end(); it++)
					{
						wxString wxs_temp_path = "";
						wxs_temp_path = wxs_temp_filepath + it->c_str() + "\\";
						int i_charSize = wxs_temp_path.size() + 1;
						SYSTEMTIME s_now;
						GetLocalTime(&s_now);//获取当前时间
						SYSTEMTIME s_new_path;
						/*TCHAR *wchar_string = new TCHAR(i_charSize);*/
						TCHAR wchar_string[180];
						wcscpy(wchar_string, wxs_temp_path.wc_str());
						GetDirCreateTime(wchar_string, s_new_path);//获取目录的创建时间
																   /*delete[]wchar_string;*/
																   //获取时间差
						struct tm t_now;
						struct tm t_new_path;
						memset(&t_now, 0, sizeof(t_now));
						memset(&t_new_path, 0, sizeof(t_new_path));
						t_now = { s_now.wSecond, s_now.wMinute, s_now.wHour, s_now.wDay, s_now.wMonth - 1,
							s_now.wYear - 1900, s_now.wDayOfWeek, 0, 0 };
						t_new_path = { s_new_path.wSecond, s_new_path.wMinute, s_new_path.wHour, s_new_path.wDay, s_new_path.wMonth - 1,
							s_new_path.wYear - 1900, s_new_path.wDayOfWeek, 0, 0 };
						time_t time_now = mktime(&t_now);
						time_t time_new_path = mktime(&t_new_path);
						long remainSec = difftime(time_now, time_new_path);//计算间隔的秒
						//比较时间差 
						if (remainSec < m_i_delta_T)
						{
							m_i_delta_T = remainSec;
							m_wxs_dection_catalogue = it->c_str();
						}
					}
//					files.clear();
					string str_Test_directory = m_wxs_dection_catalogue.c_str();
					next_file.push_back(str_Test_directory);
//==============================================================================================
				}
				//若要信号触发轮循，则屏蔽以上代码
				for (vector<string>::iterator it = next_file.begin(); it != next_file.end(); it++)
				{
					m_i_fin_file_num = 0;
					int i_file_num = 0;

					wxString wxs_temp_path = "";
					wxs_temp_path = wxs_temp_filepath + it->c_str() + "\\";

					wxDir dir_demp(wxs_temp_path);
					if (!dir_demp.Open(wxs_temp_path))
					{
						msg = wxString::Format("无法打开路径%s", wxs_temp_path);
						MyLog::LogError(msg.c_str());
						wxQueueEvent(m_frame->GetEventHandler(),
							new wxCommandEvent(wxEVT_THREAD, ID_NO_PIC));
						return 0;
					}
					else
					{
						//msg = wxString::Format("当前检测路径%s", wxs_temp_path);
						//MyLog::LogMessage(msg.c_str());
					}

					if (!m_b_run_flag)
					{
						return 0;
					}
#ifdef __READ_CSV__
					//在csv路径中寻找相对应的csv
					wxString csv_path = m_gmodel->GetSavePathCsv() + "\\" + temp_now_time + "\\";
					const char* csv_filePath = csv_path.c_str();
					vector<string> csv_files;
					this->getJustCurrentDir(csv_filePath, csv_files);
					wxString wxs_temp_csv_filepath = "";
					bool Is_TestYiwu = true;
					for (vector<string>::iterator ii = csv_files.begin(); ii != csv_files.end(); ii++)
					{
						wxs_temp_csv_filepath = csv_path + ii->c_str();
					
						string csv_file = "[" + it->substr(0, 6) + "]" + it->substr(7);
						cout << "csv_file:" << csv_file << endl;
						wxArrayString csv_list;
						wxDir::GetAllFiles(wxs_temp_csv_filepath, &csv_list, "", wxDIR_FILES);
						bool csv_T_or_F = true;
						bool find_csv = false;
						if (csv_list.size() == 0)
						{
							msg = "csv下没有文件";
							MyLog::LogError(msg.mb_str());
						}
						else
						{
							for (size_t csv_i = 0; csv_i < csv_list.size(); csv_i++)
							{
								if (strstr(csv_list[csv_i], csv_file.c_str()))
								{
									string s_csv_path = csv_list[csv_i].c_str();
//									DWORD time_csv_1 = GetTickCount();
									csv_T_or_F = LoadCsvFile(s_csv_path);
// 									DWORD time_csv_2 = GetTickCount();
// 									cout << "csv_time:" << (time_csv_2 - time_csv_1) / 1000.0 << "S" << endl;
									find_csv = true;
									break;
								}
							}
						}
					
						if (!csv_T_or_F)
						{
							msg = "粒子检测结果为NG，将不进行异物检";
							MyLog::LogError(msg.mb_str());
							Is_TestYiwu = false;

							if (busy_info != NULL)
							{
								delete busy_info;
								busy_info = NULL;
							}
							busy_info = new wxBusyInfo(msg);

							wxQueueEvent(m_frame->GetEventHandler(),
								new wxCommandEvent(wxEVT_THREAD, ID_SEND_NG_SINGLE));
							break;
						}

						if (find_csv)
							break;
					}

					if(!Is_TestYiwu)
						break;
#endif // __READ_CSV__

					wxDir::GetAllFiles(wxs_temp_path, &img_list, "*.jpg", wxDIR_DEFAULT);
					//wxDir::GetAllFiles(wxs_temp_path, &l_dirArray, "*.jpg", wxDIR_DIRS);

					if (img_list.size() == 0)
					{
						wxMessageBox(_("该目录下未找到图片，请重新选择目录"), _("Error"));
						m_b_run_flag = false;
						wxQueueEvent(m_frame->GetEventHandler(),
							new wxCommandEvent(wxEVT_THREAD, ID_NO_PIC));
						return 0;
					}

					if (!m_b_run_flag)
					{
						return 0;
					}
					//m_gmodel->SendReadSingleOver();	//发送读取成功信号，用于PLC做清零操作
					for (size_t i_img = 0; i_img < img_list.size(); i_img++)
					{
						if (!m_b_run_flag)
						{
							return 0;
						}
#ifdef __LIZI__ 

						if (strstr(img_list[i_img], "-HasBeenTesting")/* || strstr(img_list[i_img], "XU02_")*/)
						{
							// 				msg = wxString::Format("已检测图：%s", img_list[i_img]);
							// 				MyLog::LogMessage(msg.c_str());
							continue;
						}

						if (strstr(img_list[i_img], "_SUB_"))
						{
							wxString temp_grp_path = "";
							wxString temp_org_path = "";
							wxString temp_grp_name = "";
							wxString temp_org_name = "";
							if (strstr(img_list[i_img], "_grp_") || strstr(img_list[i_img], "_SUB_L_"))
							{
								msg = wxString::Format("图grp：%s", img_list[i_img]);
								MyLog::LogMessage(msg.c_str());

								temp_grp_path = img_list[i_img];
								temp_grp_name = img_list[i_img].substr(m_gmodel->GetTestingPath().size() + 1, img_list[i_img].size());
								string str_grp = temp_grp_name.c_str();
								i_img++;
								if (i_img < img_list.size())
								{
									if (strstr(img_list[i_img], "_org_") || strstr(img_list[i_img], "_SUB_R_"))
									{
										msg = wxString::Format("图org：%s", img_list[i_img]);
										MyLog::LogMessage(msg.c_str());

										temp_org_path = img_list[i_img];
										temp_org_name = img_list[i_img].substr(m_gmodel->GetTestingPath().size() + 1, img_list[i_img].size());
										string str_org = temp_org_name.c_str();

										msg = "进入粒子算法";
										MyLog::LogMessage(msg.c_str());

										m_gmodel->OnLiziDetection(temp_grp_path, str_grp, temp_org_path, str_org);

										msg = "结束粒子算法";
										MyLog::LogMessage(msg.c_str());

										continue;
									}
									else
									{
										break;
									}
								}
								else
								{
									break;
								}
							}
							else if (strstr(img_list[i_img], "_org_") || strstr(img_list[i_img], "_SUB_R_"))
							{
								msg = wxString::Format("图org：%s", img_list[i_img]);
								MyLog::LogMessage(msg.c_str());

								temp_org_path = img_list[i_img];
								temp_org_name = img_list[i_img].substr(m_gmodel->GetTestingPath().size() + 1, img_list[i_img].size());
								string str_org = temp_org_name.c_str();
								i_img++;
								if (i_img < img_list.size())
								{
									if (strstr(img_list[i_img], "_grp_") || strstr(img_list[i_img], "_SUB_L_"))
									{
										msg = wxString::Format("图grp：%s", img_list[i_img]);
										MyLog::LogMessage(msg.c_str());

										temp_grp_path = img_list[i_img];
										temp_grp_name = img_list[i_img].substr(m_gmodel->GetTestingPath().size() + 1, img_list[i_img].size());
										string str_grp = temp_grp_name.c_str();

										msg = "进入粒子算法";
										MyLog::LogMessage(msg.c_str());

										m_gmodel->OnLiziDetection(temp_grp_path, str_grp, temp_org_path, str_org);

										msg = "结束粒子算法";
										MyLog::LogMessage(msg.c_str());

										continue;
									}
									else
									{
										break;
									}
								}
								else
								{
									break;
								}
							}
							else
							{
								continue;
							}
						}
#else
						if (strstr(img_list[i_img], "-HasBeenTesting") || strstr(img_list[i_img], "_SUB_"))
						{
							continue;
						}
#endif
						wxString strtemp;
						strtemp = img_list[i_img].substr(m_gmodel->GetTestingPath().size() + 1, img_list[i_img].size());
						string str = strtemp.c_str();
						//判断当前图是否检测过
						bool is_find = false;
						for (list<string>::iterator it = m_list_is_dection.begin(); it != m_list_is_dection.end(); it++)
						{
							if (it->c_str() == str)
							{
								is_find = true;
								break;
							}
						}
						if (is_find) continue;

						//msg = "开始读取图并显示";
						//MyLog::LogMessage(msg.c_str());

						if (!m_b_run_flag)
						{
							return 0;
						}
						//DWORD  time_1 = GetTickCount();

						pic_data temp_pic_data;
						temp_pic_data.image_path = cv::String(img_list[i_img]);
						temp_pic_data.image_name = str;
						cv::Mat temp_image_data = cv::imread(temp_pic_data.image_path, 0);
						temp_pic_data.image_data = temp_image_data;

						m_list_pic_data.push_back(temp_pic_data);
						m_gmodel->SetListPicData(&m_list_pic_data);

						m_thread_pool->QueueTaskItem(ImageModel::Task1, NULL, AutomaticDetetionThread::SetFinFileFlag);

						if (!m_b_run_flag)
						{
							return 0;
						}

						m_list_is_dection.push_back(str);
						i_file_num++;
					}
					wxString msg = "已完成队列添加，自动线程进入等待";
					MyLog::LogMessage(msg.mb_str());

					if (!m_b_run_flag)
					{
						return 0;
					}

					msg = "m_b_planone_run_flag关闭";
					MyLog::LogInfat(msg.c_str());

					while (m_i_fin_file_num < i_file_num)
					{
						Sleep(1);
					}
// 					DWORD  time_6 = GetTickCount();
// 					cout << "AutomaticDetetion >> " << "All time:" << (time_6 - time_5) / 1000.0 << "s" << endl;
// 
// 					msg = wxString::Format("AutomaticDetetion >> All time: %f s", (time_6 - time_5) / 1000.0);
// 					MyLog::LogMessage(msg.c_str());
				}
				m_b_planone_run_flag = false;
			}
		}
		//离线
		else
		{
			temp_path = m_gmodel->GetTestingPath() + "\\";

			char nd[64] = { 0 };
			char temp_now_time[64] = { 0 };
			time_t t = time((NULL));
			tm *tm_cur = localtime(&t);
			if (tm_cur->tm_hour < 8)
			{
				t = t - 12 * 60 * 60;
			}
			strftime(temp_now_time, sizeof(temp_now_time), "%Y%m%d", localtime(&t));

			temp_path = temp_path + temp_now_time + "\\";
			if (!m_b_run_flag)
			{
				return 0;
			}

			//获取子目录
			const char* filePath = temp_path.c_str();
			vector<string> files;
			this->getJustCurrentDir(filePath, files);
			//test 级别
			for (vector<string>::iterator iter = files.begin(); iter != files.end(); iter++)
			{
				if (!strstr(m_picesName.c_str(), iter->c_str()))
				{
					wxMessageBox(_("配方模板与片子不一致！"), _("Error"));
					wxQueueEvent(m_frame->GetEventHandler(),
						new wxCommandEvent(wxEVT_THREAD, ID_RECIPE_ERROR));
					return 0;
				}

				wxString wxs_filePath = "";
				wxs_filePath = temp_path + iter->c_str() + "\\";
				const char* cfilePath = wxs_filePath.c_str();
				vector<string> vfiles;
				this->getJustCurrentDir(cfilePath, vfiles);

				//在获取一级子目录
				for (vector<string>::iterator it = vfiles.begin(); it != vfiles.end(); it++)
				{
					m_i_fin_file_num = 0;
					int i_file_num = 0;

					wxString wxs_temp_path = "";
					wxs_temp_path = wxs_filePath + it->c_str()+ "\\";

					//todo
					const char* l_hasBeenDir = wxs_temp_path.c_str();
					vector<string> l_hasBeenVec;
					this->getJustCurrentDir(l_hasBeenDir, l_hasBeenVec);
					
					wxDir dir_demp(wxs_temp_path);
					if (!dir_demp.Open(wxs_temp_path))
					{
						msg = wxString::Format("无法打开路径%s", wxs_temp_path);
						MyLog::LogError(msg.c_str());
						//wxQueueEvent(m_frame->GetEventHandler(),
						//	new wxCommandEvent(wxEVT_THREAD, ID_NO_PIC));
						break;
					}
					else
					{
						//msg = wxString::Format("当前检测路径%s", wxs_temp_path);
						//MyLog::LogMessage(msg.c_str());
					}

					if (!m_b_run_flag)
					{
						return 0;
					}
					Sleep(10);
					wxArrayString img_list1;
					wxDir::GetAllFiles(wxs_temp_path, &img_list1, "*.jpg", wxDIR_FILES);
					//cout << "img_list======= " << img_list1.size() << endl;
					m_delayImage = img_list1;
					//if (img_list1.size() == 0)
					//{ 
					//	wxQueueEvent(m_frame->GetEventHandler(),
					//		new wxCommandEvent(wxEVT_THREAD, ID_NO_PIC));
					//	continue;
					//}

					if (!m_b_run_flag)
					{
						return 0;
					}
					//m_gmodel->SendReadSingleOver();	//发送读取成功信号，用于PLC做清零操作
					for (size_t i_img = 0; i_img < img_list1.size(); i_img++)
					{
// 	#ifdef __LIZI__ 
// 
// 						if (strstr(img_list[i_img], "-HasBeenTesting")/* || strstr(img_list[i_img], "XU02_")*/)
// 						{
// 							// 				msg = wxString::Format("已检测图：%s", img_list[i_img]);
// 							// 				MyLog::LogMessage(msg.c_str());
// 							continue;
// 						}
// 
// 						if (strstr(img_list[i_img], "_SUB_"))
// 						{
// 							wxString temp_grp_path = "";
// 							wxString temp_org_path = "";
// 							wxString temp_grp_name = "";
// 							wxString temp_org_name = "";
// 							if (strstr(img_list[i_img], "_grp_") || strstr(img_list[i_img], "_SUB_L_"))
// 							{
// 								msg = wxString::Format("图grp：%s", img_list[i_img]);
// 								MyLog::LogMessage(msg.c_str());
// 
// 								temp_grp_path = img_list[i_img];
// 								temp_grp_name = img_list[i_img].substr(m_gmodel->GetTestingPath().size() + 1, img_list[i_img].size());
// 								string str_grp = temp_grp_name.c_str();
// 								i_img++;
// 								if (i_img < img_list.size())
// 								{
// 									if (strstr(img_list[i_img], "_org_") || strstr(img_list[i_img], "_SUB_R_"))
// 									{
// 										msg = wxString::Format("图org：%s", img_list[i_img]);
// 										MyLog::LogMessage(msg.c_str());
// 
// 										temp_org_path = img_list[i_img];
// 										temp_org_name = img_list[i_img].substr(m_gmodel->GetTestingPath().size() + 1, img_list[i_img].size());
// 										string str_org = temp_org_name.c_str();
// 
// 										msg = "进入粒子算法";
// 										MyLog::LogMessage(msg.c_str());
// 
// 										m_gmodel->OnLiziDetection(temp_grp_path, str_grp, temp_org_path, str_org);
// 
// 										msg = "结束粒子算法";
// 										MyLog::LogMessage(msg.c_str());
// 
// 										continue;
// 									}
// 									else
// 									{
// 										break;
// 									}
// 								}
// 								else
// 								{
// 									break;
// 								}
// 							}
// 							else if (strstr(img_list[i_img], "_org_") || strstr(img_list[i_img], "_SUB_R_"))
// 							{
// 								msg = wxString::Format("图org：%s", img_list[i_img]);
// 								MyLog::LogMessage(msg.c_str());
// 
// 								temp_org_path = img_list[i_img];
// 								temp_org_name = img_list[i_img].substr(m_gmodel->GetTestingPath().size() + 1, img_list[i_img].size());
// 								string str_org = temp_org_name.c_str();
// 								i_img++;
// 								if (i_img < img_list.size())
// 								{
// 									if (strstr(img_list[i_img], "_grp_") || strstr(img_list[i_img], "_SUB_L_"))
// 									{
// 										msg = wxString::Format("图grp：%s", img_list[i_img]);
// 										MyLog::LogMessage(msg.c_str());
// 
// 										temp_grp_path = img_list[i_img];
// 										temp_grp_name = img_list[i_img].substr(m_gmodel->GetTestingPath().size() + 1, img_list[i_img].size());
// 										string str_grp = temp_grp_name.c_str();
// 
// 										msg = "进入粒子算法";
// 										MyLog::LogMessage(msg.c_str());
// 
// 										m_gmodel->OnLiziDetection(temp_grp_path, str_grp, temp_org_path, str_org);
// 
// 										msg = "结束粒子算法";
// 										MyLog::LogMessage(msg.c_str());
// 
// 										continue;
// 									}
// 									else
// 									{
// 										break;
// 									}
// 								}
// 								else
// 								{
// 									break;
// 								}
// 							}
// 							else
// 							{
// 								continue;
// 							}
// 						}
//	#else
						//cout << "img_list[i_img]: " << img_list[i_img] << endl;
						if (/*strstr(img_list[i_img], "-HasBeenTesting") ||*/ strstr(img_list1[i_img], "_SUB_"))
						{
							continue;
						}
//	#endif
						wxString strtemp;

						strtemp = img_list1[i_img].substr(m_gmodel->GetTestingPath().size() + 1, img_list1[i_img].size());
						string str = strtemp.c_str();

						vector<string> l_strVec;
						ImageModel::SplitString(str, l_strVec, "\\");
						string l_subImage = "";
						if (!l_strVec.empty())
						{
							string l_imageName = "";
							int l_index = l_strVec.size() - 1;
							l_imageName = l_strVec[l_index];
							int l_size = l_imageName.size() - 4;
							l_subImage = l_imageName.substr(0, l_size);
						}

						m_isDetect = false;
						for (vector<string>::iterator it = l_hasBeenVec.begin(); it != l_hasBeenVec.end(); it++)
						{
							if (l_subImage == it->c_str())
							{
								m_isDetect = true;
								break;
							}
						}

						if (m_isDetect)
						{
							continue;
						}

						//判断当前图是否检测过
						//bool is_find = false;
						//for (list<string>::iterator it = m_list_is_dection.begin(); it != m_list_is_dection.end(); it++)
						//{
						//	if (it->c_str() == str)
						//	{
						//		is_find = true;
						//		break;
						//	}
						//}
						//if (is_find) continue;

						//msg = "开始读取图并显示";
						pic_data temp_pic_data;
						temp_pic_data.image_path = cv::String(img_list1[i_img]);
						temp_pic_data.image_name = str;
						cv::Mat temp_image_data = cv::imread(temp_pic_data.image_path, 0);
						temp_pic_data.image_data = temp_image_data;

						m_list_pic_data.push_back(temp_pic_data);
						m_gmodel->SetListPicData(&m_list_pic_data);

						m_thread_pool->QueueTaskItem(ImageModel::Task1, NULL, AutomaticDetetionThread::SetFinFileFlag);

						m_list_is_dection.push_back(str);
						//m_hasBeenTest->SetTestNameList(m_list_is_dection);
						//m_hasBeenTest->OnSave("./HasBeenTest.dat");
						i_file_num++;

					}//检测图片目录

					//防止占用过多cup资源
					while (m_i_fin_file_num < i_file_num)
					{
						Sleep(1);
					}
				}
			}
			if (m_delayImage.size() == 0)
			{
				Sleep(1);
			}
		}
	}
	return 0;
}

void AutomaticDetetionThread::OnExit()
{

}

//获取当前时间(年月日);
string AutomaticDetetionThread::GetNowTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&timep));
	return tmp;
}

//获取子目录
void AutomaticDetetionThread::getJustCurrentDir(string path, vector<string>& files)
{
	//文件句柄
	intptr_t hFile = 0;
	//文件信息 
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(fileinfo.name);
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void AutomaticDetetionThread::SetFinFileFlag(int result)
{
	m_i_fin_file_num++;
}
// 获取指定目录的创建时间
bool AutomaticDetetionThread::GetDirCreateTime(TCHAR* DirName, SYSTEMTIME& stime)
{
	HANDLE hDir;
	// 打开目录的Win32 API调用
	hDir = CreateFile(DirName,
		GENERIC_READ,// 只需读方式打开即可
		FILE_SHARE_READ | FILE_SHARE_DELETE, NULL,
		OPEN_EXISTING,// 打开现存的目录
		FILE_FLAG_BACKUP_SEMANTICS,//只有这样才可以打开目录
		NULL);
	FILETIME lpCreationTime;// creation time目录创建时间
	FILETIME lpLastAccessTime;// last access time目录最近访问时间
	FILETIME lpLastWriteTime;// last write time目录最近修改时间
							 // 获取目录日期和时间的Win32 API调用
	BOOL retval = GetFileTime(hDir, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime);
	if (retval)
	{
		FILETIME ftime;
		FileTimeToLocalFileTime(&lpCreationTime, &ftime); // 转换成本地时间
		FileTimeToSystemTime(&ftime, &stime); // 转换成系统时间格式
		CloseHandle(hDir);
	}
	return retval;
}

bool AutomaticDetetionThread::LoadCsvFile(const string& csv)
{
	vector<string> l_countVec;
	string l_line;
	ifstream l_file(csv);
	if (l_file.fail())
	{
		cout << "File not found" << endl;
		return false;
	}

	bool l_bol = true;
	int l_index = 0;
	while (getline(l_file, l_line) && l_file.good())
	{
		if (l_index != 1)
		{
			l_index++;
			continue;
		}
		vector<string> l_strVec;
		string l_str = l_line;
		string l_countS = SplitString(l_line, l_strVec, ",");

		if (!isdigit(l_countS[0]))
		{
			l_file.close();
			break;
		}
		//int l_count = atoi(l_str.c_str());
		int l_count = std::stoi(l_countS);

		if (l_count < 7)
		{
			l_file.close();
			return false;
		}
	}
	l_file.close();
	return true;
}

std::string AutomaticDetetionThread::SplitString(const string& line, vector<string>& vectorStr, const string& delimiter)
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
	string l_count = vectorStr[5];

	return l_count;
}

//====================================================================================================================