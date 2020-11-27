#pragma once
#include <wx/wx.h>
#include <wx/snglinst.h>
#include <CrashRpt.h>
#include <fstream>
#include <wx/log.h>
#include "MainFrame.h"
#include "./Util/PreDefine.h"
#include "MyLog.h"

class LNIApp :
	public wxApp
{
public:
	bool OnInit();
	int  OnExit();
private:
#ifdef __WITH_CONSOLE__
	wxLogStream             *wx_logs;
#else
	wxLogStderr             *wx_logs;
#endif
	wxSingleInstanceChecker *m_checker;
	std::ofstream           of;
};

