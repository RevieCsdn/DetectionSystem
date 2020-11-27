#include "MyLog.h"

std::ofstream MyLog::m_ofs;
MyLog::LOG_LEVEL MyLog::m_log_level = LOG_MESSAGE;
char MyLog::m_timestamp[128] = "%Y-%m-%d %H:%M:%S";
string MyLog::m_now_time = "";

bool MyLog::SetTargeFile(const char *v_name, std::ios_base::openmode v_mode /* = std::ios_base::app */)
{
	m_ofs.close();
	m_ofs.open(v_name, v_mode);
	m_now_time = v_name;
	if (!m_ofs.is_open())
		return false;
	return true;
}

void MyLog::SetLogLevel(LOG_LEVEL v_level)
{
	m_log_level = v_level;
}

void MyLog::SetTimeStamp(const char *v_time_stamp)
{
	strcpy_s(m_timestamp, v_time_stamp);
}

string MyLog::GetNowTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&timep));
	return tmp;
}

void MyLog::CloseStream()
{
	m_ofs.flush();
	m_ofs.close();
}

void MyLog::LogUser(const char *v_log)
{
	Log(LOG_USER, v_log);
}

void MyLog::LogInfat(const char *v_log)
{
	Log(LOG_INFAT, v_log);
}

void MyLog::LogError(const char *v_log)
{
	Log(LOG_ERROR, v_log);
}

void MyLog::LogWarning(const char *v_log)
{
	Log(LOG_WARNING, v_log);
}

void MyLog::LogMessage(const char *v_log)
{
	Log(LOG_MESSAGE, v_log);
}

void MyLog::Log(const LOG_LEVEL v_level, const char *v_log)
{
	if (v_level > m_log_level) return;
	
	time_t t = time(NULL);
	char rs[64] = { 0 };
	strftime(rs, sizeof(rs), m_timestamp, localtime(&t));

	string now_time = GetNowTime();
	if (!strstr(m_now_time.c_str(), now_time.c_str()))
	{
		string mylog_name = "InterfaceLog\\";
		string mylog_name_data = now_time + ".log";
		mylog_name += mylog_name_data;
		SetTargeFile(mylog_name.c_str());
	}

	switch (v_level)
	{
	case LOG_INFAT:
		m_ofs << "INFAT    : " << rs << ", " << v_log << std::endl;
		break;
	case LOG_ERROR:
		m_ofs << "ERROR    : " << rs << ", " << v_log << std::endl;
		break;
	case LOG_WARNING:
		m_ofs << "WARNING  : " << rs << ", " << v_log << std::endl;
		break;
	case LOG_MESSAGE:
		m_ofs << "MESSAGE  : " << rs << ", " << v_log << std::endl;
		break;
	case LOG_USER:
		m_ofs << "USER     : " << rs << ", " << v_log << std::endl;
		break;
	}
}