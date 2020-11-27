#include "AlgorithmLog.h"

std::ofstream AlgorithmLog::m_ofs;
AlgorithmLog::LOG_LEVEL AlgorithmLog::m_log_level = LOG_BUMPDETECTOR;
char AlgorithmLog::m_timestamp[128] = "%Y-%m-%d %H:%M:%S";

bool AlgorithmLog::SetTargeFile(const char *v_name, std::ios_base::openmode v_mode /* = std::ios_base::app */)
{
	m_ofs.close();
	m_ofs.open(v_name, v_mode);
	if (!m_ofs.is_open())
		return false;
	return true;
}

void AlgorithmLog::SetLogLevel(LOG_LEVEL v_level)
{
	m_log_level = v_level;
}

void AlgorithmLog::SetTimeStamp(const char *v_time_stamp)
{
	strcpy_s(m_timestamp, v_time_stamp);
}

void AlgorithmLog::CloseStream()
{
	m_ofs.flush();
	m_ofs.close();
}

void AlgorithmLog::LogUser(const char *v_log)
{
	Log(LOG_USER, v_log);
}

void AlgorithmLog::LogInfat(const char *v_log)
{
	Log(LOG_INFAT, v_log);
}

void AlgorithmLog::LogError(const char *v_log)
{
	Log(LOG_ERROR, v_log);
}

void AlgorithmLog::LogWarning(const char *v_log)
{
	Log(LOG_WARNING, v_log);
}

void AlgorithmLog::LogMessage(const char *v_log)
{
	Log(LOG_MESSAGE, v_log);
}
void AlgorithmLog::LogBumpDetector(const char *v_log)
{
	Log(LOG_BUMPDETECTOR, v_log);
}

void AlgorithmLog::Log(const LOG_LEVEL v_level, const char *v_log)
{
	if (v_level > m_log_level) return;

	time_t t = time(NULL);
	char rs[64] = { 0 };
	strftime(rs, sizeof(rs), m_timestamp, localtime(&t));
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
	case LOG_BUMPDETECTOR:
		m_ofs << "BUMPDETECTOR     : " << rs << ", " << v_log << std::endl;
		break;
	}
}