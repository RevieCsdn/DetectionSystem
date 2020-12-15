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
	// We want the screenshot of the entire desktop is to be added on crash  ����ϣ���ڱ���ʱ�����������Ľ�ͼ
	crAddScreenshot2(CR_AS_VIRTUAL_SCREEN, 0);
#endif
	// log
	wx_logs = NULL;	//	��־
	m_checker = NULL;
#ifdef __WITH_CONSOLE__//1
	wx_logs = new wxLogStream(&std::cout);						//���������ڽ���־��Ϣ�ض���C++�ļ�����
#else
	wxString aa = "log";
	if (_access("log", 0) != 0)									//access()	�ж��ļ��Ƿ���ڣ����ж��ļ��Ƿ��д
		CreateDirectory(aa, NULL);

	FILE* m_pLogFile = NULL;
	char log_name[64] = { 0 };
	time_t t = time(NULL);
	strftime(log_name, 64, "log\\log %Y-%m-%d", localtime(&t));
	fopen_s(&m_pLogFile, log_name, "a");
	wx_logs = new wxLogStderr(m_pLogFile);						//���������ڽ���־��Ϣ�ض���C�ļ���(��Ҫ��c++������)��
#endif
	delete wxLog::SetActiveTarget(wx_logs);						//��ָ������־Ŀ������Ϊ�Ŀ�ꡣ
																										//����ָ��ǰһ�����־Ŀ���ָ��(����Ϊ��)��Ҫ��ֹ��־��¼����ʹ��wxLogNull not NULL����ʵ����
																										//��������־Ŀ������ΪNULL��������־��¼����ʱ������һ���µ�Ĭ����־Ŀ�ꡣ
	wxLog::SetTimestamp(wxT("%Y-%m-%d %H:%M:%S"));		//��Ĭ����־Ŀ��ǰ׺��ʱ�����ʽ����Ϊ������Ϣ��
	wxLog::SetLogLevel(wxLOG_Debug);									//ָ������(��ֵ��)����logLevel����־��ϢӦ�ñ����ԣ����Ҳ����͵����־Ŀ�ꡣ
																										// for cout

#ifndef __WITH_CONSOLE__
	of = std::ofstream("stdout");
	std::cout.rdbuf(of.rdbuf());
#endif//1

	// check single
	const wxString name = wxString::Format("LNIAPP-%s",
		wxGetUserId().c_str());
	m_checker = new wxSingleInstanceChecker(name);				//wxSingleInstanceChecker������ֻ��һ������ʵ��������
	if (m_checker->IsAnotherRunning()) {						//����������һ�������Ѿ����У��򷵻�true�����򷵻�false��
																//ע�⣬���������ʹ��Ĭ�Ϲ��캯��Create()�����ģ��ڴ˷���֮ǰû�е��ù�Create()����ô�����Զ�����CreateDefault()��
		wxMessageBox(_("Program already running....."), _("Warning"),
			wxICON_WARNING | wxOK);
		return false;
	}

	wxString l_inter = "InterfaceLog";
	if (_access(l_inter, 0) != 0)									//access()	�ж��ļ��Ƿ���ڣ����ж��ļ��Ƿ��д
		CreateDirectory(l_inter, NULL);
	// for User status	
	string mylog_name = "InterfaceLog\\";
	string mylog_name_data = MyLog::GetNowTime() + ".log";
	mylog_name += mylog_name_data;
	MyLog::SetTargeFile(mylog_name.c_str());					//����Ҫ������־��ʾ���ļ����ļ��򿪷�ʽ
	MyLog::SetTimeStamp("%Y-%m-%d %H:%M:%S");					//������־ʱ��
	MyLog::SetLogLevel(MyLog::LOG_MESSAGE);						//������־�ȼ���״̬����

	MainFrame* frame = new MainFrame(NULL);
	SetTopWindow(frame);
	frame->Show();

	//_CrtDumpMemoryLeaks();
	return true;
}

int LNIApp::OnExit()
{
	of.flush();													//ˢ�»�����
	of.close();													//�ر�������򿪵��ļ�

	if (m_checker != NULL)
		delete m_checker;
#ifdef __CRASH_RPT__
	// Uninstall crash reporting								//ж�ر�������
//	crUninstall();
#endif 
	return 0;
}