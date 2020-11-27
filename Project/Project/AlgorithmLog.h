#pragma once
#include <ios>
#include <iostream>
#include <fstream>
#include <time.h>

class AlgorithmLog
{
public:
	enum LOG_LEVEL
	{
		LOG_USER = 1888,
		LOG_NONE,
		LOG_INFAT,
		LOG_ERROR,
		LOG_WARNING,
		LOG_MESSAGE,
		LOG_BUMPDETECTOR,
	};

	static bool SetTargeFile(const char *v_name, std::ios_base::openmode v_mode = std::ios_base::app);
	static void SetLogLevel(LOG_LEVEL v_level);
	static void SetTimeStamp(const char *v_time_stamp);
	static void CloseStream();

	static void LogUser(const char *v_log);
	static void LogInfat(const char *v_log);
	static void LogError(const char *v_log);
	static void LogWarning(const char *v_log);
	static void LogMessage(const char *v_log);
	static void LogBumpDetector(const char *v_log);
	static void Log(const LOG_LEVEL v_level, const char *v_log);

private:
	static std::ofstream    m_ofs;
	static LOG_LEVEL        m_log_level;
	static char             m_timestamp[128];
};

