#pragma once
#include <wx/fileconf.h> 
#include <wx/wfstream.h> 
#include <wx/file.h>
#include <wx/wx.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <wx/datetime.h>

using namespace std;
class Profile
{
private:
	wxFileConfig	*m_pFileConfig;
	static Profile	*m_instance;
	//Connect
	wxString		m_wxs_ip;
	int				m_i_port;
	wxString		m_wxs_path;
	wxString		m_wxs_path_csv;
	wxString		m_wxs_path_save_img;
	wxString		m_wxs_path_excel;
	int				m_i_platform_deck_single;
	//Parameter
	int				m_i_ic_binaryValue;
	int				m_i_ic_backValue;
	int				m_i_ic_medianBlurSize;
	//Addr
	int				m_i_read_addr;
	wxString		m_wxs_read_type;
	int				m_i_write_OK_addr;
	wxString		m_wxs_write_OK_type;
	int				m_i_write_NG_addr;
	wxString		m_wxs_write_NG_type;
	//AI
	bool			m_b_is_use_ai;
	wxString		m_wxs_ai_path;
	//
	int m_okNum;
	int m_ngNum;
	float m_ratio;
	wxString m_dataTime;

protected:
	Profile();

public:
	virtual			~Profile();
	static Profile	*Instance();
	void			loadIni(wxString path);
	void			SaveIni(wxString path);

	inline void SetOkNum(int okNum)
	{
		m_okNum = okNum;
		m_pFileConfig->Write(wxT("PreviousData/m_okNum"), m_okNum);
		
	};

	inline int GetOkNum()
	{
		return m_okNum;
	};

	inline void SetNgNum(int ngNum)
	{
		m_ngNum = ngNum;
		m_pFileConfig->Write(wxT("PreviousData/m_ngNum"), ngNum);
	};

	inline int GetNgNum()
	{
		return m_ngNum;
	};

	inline void SetRatio(float ratio)
	{
		m_ratio = ratio;
		m_pFileConfig->Write(wxT("PreviousData/m_ratio"), m_ratio);
	};

	inline float GetRatio()
	{
		return m_ratio;
	};

	inline void SetDateTime(wxString time)
	{
		m_dataTime = time;
		m_pFileConfig->Write(wxT("PreviousData/m_dateTime"), m_dataTime);
	};

	inline wxString GetDateTime()
	{
		return m_dataTime;
	};

	/////////////////////////////////////////////////////////////////////////////
	inline int GetPort()
	{
		return m_i_port;
	};
	inline void SetPort(int port)
	{
		m_i_port = port;
		m_pFileConfig->Write(wxT("Connect/m_i_port"), m_i_port);
	};
	inline wxString GetIPPath()
	{
		return m_wxs_ip;
	};
	inline void SetIPPath(wxString ip)
	{
		m_wxs_ip = ip;
		m_pFileConfig->Write(wxT("Connect/m_wxs_ip"), m_wxs_ip);
	};
	inline wxString GetPath()
	{
		return m_wxs_path;
	};
	inline void SetPath(wxString path)
	{
		m_wxs_path = path;
		m_pFileConfig->Write(wxT("Connect/m_wxs_path"), m_wxs_path);
	};
	inline wxString GetPathCsv()
	{
		return m_wxs_path_csv;
	};
	inline void SetPathCsv(wxString path)
	{
		m_wxs_path_csv = path;
		m_pFileConfig->Write(wxT("Connect/m_wxs_path_csv"), m_wxs_path_csv);
	};
	inline wxString GetPathSaveImg()
	{
		return m_wxs_path_save_img;
	};
	inline void SetPathSaveImg(wxString path)
	{
		m_wxs_path_save_img = path;
		m_pFileConfig->Write(wxT("Connect/m_wxs_path_save_img"), m_wxs_path_save_img);
	};
	inline wxString GetPathExcel()
	{
		return m_wxs_path_excel;
	};
	inline void SetPathExcel(wxString path)
	{
		m_wxs_path_excel = path;
		m_pFileConfig->Write(wxT("Connect/m_wxs_path_excel"), m_wxs_path_excel);
	};
	inline int GetNowPlatformDeckSingle()
	{
		return m_i_platform_deck_single;
	};
	inline void SetNowPlatformDeckSingle(int i_single)
	{
		m_i_platform_deck_single = i_single;
		m_pFileConfig->Write(wxT("Connect/m_i_platform_deck_single"), m_i_platform_deck_single);
	};
	inline int GetICbinaryValue()
	{
		return m_i_ic_binaryValue;
	};
	inline void SetICbinaryValue(int binaryValue)
	{
		m_i_ic_binaryValue = binaryValue;
		m_pFileConfig->Write(wxT("Parameter/m_i_ic_binaryValue"), m_i_ic_binaryValue);
	};
	inline int GetICbackValue()
	{
		return m_i_ic_backValue;
	};
	inline void SetICbackValue(int backValue)
	{
		m_i_ic_backValue = backValue;
		m_pFileConfig->Write(wxT("Parameter/m_i_ic_backValue"), m_i_ic_backValue);
	};
	inline int GetICmedianBlurSize()
	{
		return m_i_ic_medianBlurSize;
	};
	inline void SetICmedianBlurSize(int medianBlurSize)
	{
		m_i_ic_medianBlurSize = medianBlurSize;
		m_pFileConfig->Write(wxT("Parameter/m_i_ic_medianBlurSize"), m_i_ic_medianBlurSize);
	};
	//Addr
	inline int GetReadAddr()
	{
		return m_i_read_addr;
	};
	inline void SetReadAddr(int addr)
	{
		m_i_read_addr = addr;
		m_pFileConfig->Write(wxT("Addr/m_i_read_addr"), m_i_read_addr);
	};
	inline wxString GetReadType()
	{
		return m_wxs_read_type;
	};
	inline void SetReadType(wxString type)
	{
		m_wxs_read_type = type;
		m_pFileConfig->Write(wxT("Addr/m_wxs_read_type"), m_wxs_read_type);
	};
	inline int GetWriteOKAddr()
	{
		return m_i_write_OK_addr;
	};
	inline void SetWriteOKAddr(int addr)
	{
		m_i_write_OK_addr = addr;
		m_pFileConfig->Write(wxT("Addr/m_i_write_OK_addr"), m_i_write_OK_addr);
	};
	inline wxString GetWriteOKType()
	{
		return m_wxs_write_OK_type;
	};
	inline void SetWriteOKType(wxString type)
	{
		m_wxs_write_OK_type = type;
		m_pFileConfig->Write(wxT("Addr/m_wxs_write_OK_type"), m_wxs_write_OK_type);
	};
	inline int GetWriteNGAddr()
	{
		return m_i_write_NG_addr;
	};
	inline void SetWriteNGAddr(int addr)
	{
		m_i_write_NG_addr = addr;
		m_pFileConfig->Write(wxT("Addr/m_i_write_NG_addr"), m_i_write_NG_addr);
	};
	inline wxString GetWriteNGType()
	{
		return m_wxs_write_NG_type;
	};
	inline void SetWriteNGType(wxString type)
	{
		m_wxs_write_NG_type = type;
		m_pFileConfig->Write(wxT("Addr/m_wxs_write_NG_type"), m_wxs_write_NG_type);
	};
	inline bool GetIsUseAI()
	{
		return m_b_is_use_ai;
	};
	inline void SetIsUseAI(bool flag)
	{
		m_b_is_use_ai = flag;
		m_pFileConfig->Write(wxT("AI/m_b_is_use_ai"), m_b_is_use_ai);
	};
	inline wxString GetAIPath()
	{
		return m_wxs_ai_path;
	};
	inline void SetAIPath(wxString path)
	{
		m_wxs_ai_path = path;
		m_pFileConfig->Write(wxT("AI/m_wxs_ai_path"), m_wxs_ai_path);
	};
};

