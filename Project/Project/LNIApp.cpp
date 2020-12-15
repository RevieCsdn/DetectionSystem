#include "LNIApp.h"

IMPLEMENT_APP(LNIApp)
bool LNIApp::OnInit()
{
//	_CrtSetBreakAlloc(30151);
#ifdef __CRASH_RPT__
	CR_INSTALL_INFO info;
	memset(&info, 0, sizeof(info));
	info.cb = sizeof(info);
	info.pszAppName = _T("LNIApp");
	info.pszAppVersion = _T("2.01");
	info.uMiniDumpType =
		MiniDumpNormal;
	info.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS | CR_INST_NO_GUI;

	info.dwFlags |= CR_INST_DONT_SEND_REPORT;
	info.pszErrorReportSaveDir = _T("err_rpt");

	int nResult = crInstall(&info);

	if (nResult != 0)
	{
		// something goes wrong. get err message
		TCHAR buff[512];
		crGetLastErrorMsg(buff, 512);
		wxMessageBox(buff, _("Error"), wxOK | wxICON_ERROR);
		return false;
	}

	crAddFile2(_T("stdout"), NULL, _T("Log File2"), CR_AF_MAKE_FILE_COPY);
	// We want the screenshot of the entire desktop is to be added on crash  我们希望在崩溃时添加整个桌面的截图
	crAddScreenshot2(CR_AS_VIRTUAL_SCREEN, 0);
#endif
	// log
	wx_logs = NULL;	//	日志
	m_checker = NULL;
#ifdef __WITH_CONSOLE__//1
	wx_logs = new wxLogStream(&std::cout);						//这个类可用于将日志消息重定向到C++文件流。
#else
	wxString aa = "log";
	if (_access("log", 0) != 0)									//access()	判断文件是否存在，并判断文件是否可写
		CreateDirectory(aa, NULL);

	FILE* m_pLogFile = NULL;
	char log_name[64] = { 0 };
	time_t t = time(NULL);
	strftime(log_name, 64, "log\\log %Y-%m-%d", localtime(&t));
	fopen_s(&m_pLogFile, log_name, "a");
	wx_logs = new wxLogStderr(m_pLogFile);						//这个类可用于将日志消息重定向到C文件流(不要与c++流混淆)。
#endif
	delete wxLog::SetActiveTarget(wx_logs);						//将指定的日志目标设置为活动目标。
																										//返回指向前一个活动日志目标的指针(可能为空)。要禁止日志记录，请使用wxLogNull not NULL的新实例。
																										//如果将活动日志目标设置为NULL，则在日志记录发生时将创建一个新的默认日志目标。
	wxLog::SetTimestamp(wxT("%Y-%m-%d %H:%M:%S"));		//将默认日志目标前缀的时间戳格式设置为所有消息。
	wxLog::SetLogLevel(wxLOG_Debug);									//指定级别(数值上)大于logLevel的日志消息应该被忽略，并且不发送到活动日志目标。
																										// for cout

#ifndef __WITH_CONSOLE__
	of = std::ofstream("stdout");
	std::cout.rdbuf(of.rdbuf());
#endif//1

	// check single
	const wxString name = wxString::Format("LNIAPP-%s",
		wxGetUserId().c_str());
	m_checker = new wxSingleInstanceChecker(name);				//wxSingleInstanceChecker允许检查只有一个程序实例在运行
	if (m_checker->IsAnotherRunning()) {						//如果程序的另一个副本已经运行，则返回true，否则返回false。
																//注意，如果对象是使用默认构造函数Create()创建的，在此方法之前没有调用过Create()，那么它将自动调用CreateDefault()。
		wxMessageBox(_("Program already running....."), _("Warning"),
			wxICON_WARNING | wxOK);
		return false;
	}

	wxString l_inter = "InterfaceLog";
	if (_access(l_inter, 0) != 0)									//access()	判断文件是否存在，并判断文件是否可写
		CreateDirectory(l_inter, NULL);
	// for User status	
	string mylog_name = "InterfaceLog\\";
	string mylog_name_data = MyLog::GetNowTime() + ".log";
	mylog_name += mylog_name_data;
	MyLog::SetTargeFile(mylog_name.c_str());					//设置要进行日志显示的文件及文件打开方式
	MyLog::SetTimeStamp("%Y-%m-%d %H:%M:%S");					//设置日志时间
	MyLog::SetLogLevel(MyLog::LOG_MESSAGE);						//设置日志等级（状态）？

	MainFrame* frame = new MainFrame(NULL);
	SetTopWindow(frame);
	frame->Show();

	//_CrtDumpMemoryLeaks();
	return true;
}

int LNIApp::OnExit()
{
	of.flush();													//刷新缓冲区
	of.close();													//关闭主程序打开的文件

	if (m_checker != NULL)
		delete m_checker;
#ifdef __CRASH_RPT__
	// Uninstall crash reporting								//卸载崩溃报告
//	crUninstall();
#endif 
	return 0;
}