#include "Profile.h"

Profile * Profile::m_instance = NULL;

Profile::Profile()
{
	m_pFileConfig = NULL;
	m_okNum = 0;
	m_ngNum = 0;
	m_ratio = 0;
}

Profile::~Profile()
{
	if (m_pFileConfig)
		delete m_pFileConfig;
}

Profile * Profile::Instance()
{
	if (m_instance == 0) {
		m_instance = new Profile();
	}

	return m_instance;
}

void Profile::loadIni(wxString path)
{
	if (!wxFile::Exists(path))
	{
		wxFile file;
		file.Create(path, true);
	}

	wxFileInputStream configInStream(path);
	m_pFileConfig = new wxFileConfig(configInStream, wxConvUTF8);
	//Connect
	m_pFileConfig->Read(wxT("Connect/m_wxs_ip"), &m_wxs_ip, "127.0.0.1");
	m_pFileConfig->Read(wxT("Connect/m_i_port"), &m_i_port, 3306);
	m_pFileConfig->Read(wxT("Connect/m_wxs_path"), &m_wxs_path, "D:\\");
	m_pFileConfig->Read(wxT("Connect/m_wxs_path_csv"), &m_wxs_path_csv, "D:\\");
	m_pFileConfig->Read(wxT("Connect/m_wxs_path_save_img"), &m_wxs_path_save_img, "D:\\");
	m_pFileConfig->Read(wxT("Connect/m_wxs_path_excel"), &m_wxs_path_excel, "D:\\");
	m_pFileConfig->Read(wxT("Connect/m_i_platform_deck_single"), &m_i_platform_deck_single, 0);
	//Parameter
	m_pFileConfig->Read(wxT("Parameter/m_i_ic_binaryValue"), &m_i_ic_binaryValue, 100);
	m_pFileConfig->Read(wxT("Parameter/m_i_ic_backValue"), &m_i_ic_backValue, 100);
	m_pFileConfig->Read(wxT("Parameter/m_i_ic_medianBlurSize"), &m_i_ic_medianBlurSize, 7);
	//Addr
	m_pFileConfig->Read(wxT("Addr/m_i_read_addr"), &m_i_read_addr, 6000);
	m_pFileConfig->Read(wxT("Addr/m_wxs_read_type"), &m_wxs_read_type, "D");
	m_pFileConfig->Read(wxT("Addr/m_i_write_OK_addr"), &m_i_write_OK_addr, 5800);
	m_pFileConfig->Read(wxT("Addr/m_wxs_write_OK_type"), &m_wxs_write_OK_type, "M");
	m_pFileConfig->Read(wxT("Addr/m_i_write_NG_addr"), &m_i_write_NG_addr, 5801);
	m_pFileConfig->Read(wxT("Addr/m_wxs_write_NG_type"), &m_wxs_write_NG_type, "M");
	//AI
	m_pFileConfig->Read(wxT("AI/m_b_is_use_ai"), &m_b_is_use_ai, false);
	m_pFileConfig->Read(wxT("AI/m_wxs_ai_path"), &m_wxs_ai_path, "http://192.168.0.112:30003/predict");
	m_pFileConfig->Read(wxT("AI/m_typeList"), &m_typeList, "0");
	//Previous
	m_pFileConfig->Read(wxT("PreviousData/m_okNum"), &m_okNum,0);
	m_pFileConfig->Read(wxT("PreviousData/m_ngNum"), &m_ngNum,0);
	m_pFileConfig->Read(wxT("PreviousData/m_ratio"), &m_ratio);
	m_pFileConfig->Read(wxT("PreviousData/m_dateTime"), &m_dataTime);
	//DataReport
	m_pFileConfig->Read(wxT("DataReport/m_msgQueenName"), &m_msgQueenName);
	m_pFileConfig->Read(wxT("DataReport/m_machineSn"), &m_machineSn);

}

void Profile::SaveIni(wxString path)
{
	if (NULL == m_pFileConfig)
		return;

	wxFileOutputStream ConfigOutStream(path);
	m_pFileConfig->Save(ConfigOutStream, wxConvUTF8);
}