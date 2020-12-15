#include "PIS2.h"

PIS2 * PIS2::ms_instance = NULL;
bool PIS2::m_locker = false;

PIS2::PIS2()
{
    m_has_connect = false;
    m_profile = Profile::Instance();
    m_conn_handle = 0;
    h_locker = ::CreateEvent(NULL, false, true, _T("PIS2LOCKER"));
}

wxString PIS2::StringBetween(wxString & str_src, wxString & str_b, wxString & str_e)
{
	wxString rs = "";

	unsigned int b_at = str_src.find(str_b) + str_b.length();
	unsigned int e_at = str_src.find(str_e);
	if (b_at < str_src.length() && e_at >= b_at)
		rs = str_src.Mid(b_at, e_at - b_at);

	cout << "  pis2>> " << str_b.ToStdString() << ": " << rs << endl;
	return rs;

}

bool PIS2::SendQueueFunc(char sz_buffer[4096], int & data_len, int sleep_time)
{
	//if (!m_profile->GetToPis2())
	//	return true;

	this->Connect();

	//put queue
	char sz_queue_name[16] = { 0 };
	char sz_reply_name[16] = { 0 };
	char sz_corralation_id[32] = { 0 };
	sprintf_s(sz_queue_name, 16, "%s", ""/*m_profile->GetMsgQueueName().ToStdString().c_str()*/);
	sprintf_s(sz_reply_name, 16, "%s", ""/*m_profile->GetMachineSn().ToStdString().c_str()*/);
	sprintf_s(sz_corralation_id, 32, "%s", "1234567890");

	char sz_queque_handle[32] = { 0 };
	char sz_time_stamp[16] = { 0 };

	//this->Lock();
	wxLogMessage(wxString::Format("%s(%d): Wait for locker.", __FILE__, __LINE__));
	::WaitForSingleObject(h_locker, INFINITE);
	int nRt = 0;

	//int nCount = 0;
	//getMessageCount(m_conn_handle, sz_queque_handle, &nCount);
	//if(nCount>0) nRt = cleanQueue(m_conn_handle, sz_queque_handle);

	wxLogMessage(wxString::Format("%s(%d): put queue start.", __FILE__, __LINE__));

	wxLogMessage(wxString::Format("Put Queue queue name: %s, reply name %s.", sz_queue_name, sz_reply_name));
	nRt = putQueue(m_conn_handle, sz_queue_name, sz_reply_name, sz_corralation_id, (unsigned char *)sz_buffer, data_len);
	if (nRt != DEF_SUCCESS)
	{
		wxLogError(wxString::Format("%s(%d): put queue error. Return %d", __FILE__, __LINE__, nRt));
		wxMessageBox(wxString::Format("PIS2 MCMQ putQueue error code : %d", nRt));
		this->Connect(true);
		::SetEvent(h_locker);
		return false;
	}

	wxLogMessage(wxString::Format("%s(%d): put queue end. Return %d\n%s", __FILE__, __LINE__, nRt, sz_buffer));

	if (sleep_time > 0) Sleep(sleep_time);

	wxLogMessage(wxString::Format("%s(%d): open queue start. szQueueName : %s", __FILE__, __LINE__, sz_reply_name));

	nRt = openQueue(m_conn_handle, sz_reply_name, 1000000, 5000, 500000, 2500, 0, 0, sz_reply_name, sz_queque_handle, 0);
	if (nRt != DEF_SUCCESS)
	{
		wxLogError(wxString::Format("%s(%d): Open queue error. Return %d", __FILE__, __LINE__, nRt));
		wxMessageBox(wxString::Format("PIS2 MCMQ openQueue error code : %d", nRt));
		this->Connect(true);
		::SetEvent(h_locker);
		return false;
	}

	wxLogMessage(wxString::Format("%s(%d): open queue end. Return %d", __FILE__, __LINE__, nRt));

	wxLogMessage(wxString::Format("%s(%d): get queue start. szReplyQueueName: %s", __FILE__, __LINE__, sz_reply_name));

	memset(sz_corralation_id, 0, 32);
	ZeroMemory(sz_buffer, 10240);
	data_len = 0;
	nRt = getQueue(m_conn_handle, sz_queque_handle, sz_reply_name, sz_time_stamp,
		sz_corralation_id, 60000, 10240, (unsigned char *)sz_buffer, &data_len);
	if (nRt != DEF_SUCCESS)
	{
		wxLogError(wxString::Format("%s(%d): get queue error. %d", __FILE__, __LINE__, nRt));
		wxMessageBox(wxString::Format("PIS2 MCMQ getQueue error code : %d", nRt));
		this->Connect(true);
		::SetEvent(h_locker);
		return false;
	}

	wxLogMessage(wxString::Format("%s(%d): get queue end. Return %d. Data %d\n%s", __FILE__, __LINE__, nRt, data_len, sz_buffer));

	if (data_len == 0)
	{
		wxLogError(wxString::Format("%s(%d): %s", __FILE__, __LINE__, _T("PIS2 MCMQ getQueue return 0 but data length is 0.")));
		wxMessageBox(_T("PIS2 MCMQ getQueue return 0 but data length is 0."));
		this->Connect(true);
		::SetEvent(h_locker);
		return false;
	}

	//getMessageCount(m_conn_handle, sz_queque_handle, &nCount);
	//if(nCount>0) nRt = cleanQueue(m_conn_handle, sz_queque_handle);

	wxLogMessage(wxString::Format("%s(%d): close queue start.", __FILE__, __LINE__));

	nRt = closeQueue(m_conn_handle, sz_queque_handle);
	if (nRt != DEF_SUCCESS)
	{
		wxLogError(wxString::Format("%s(%d): close queue error. %d", __FILE__, __LINE__, nRt));
		wxMessageBox(wxString::Format("PIS2 MCMQ closeQueue error code : %d", nRt));
		this->Connect(true);
		::SetEvent(h_locker);
		return false;
	}

	wxLogMessage(wxString::Format("%s(%d): close queue end. Return %d", __FILE__, __LINE__, nRt));

	::SetEvent(h_locker);

	return true;
}

bool PIS2::SendQueue(char sz_buffer[4096], int & data_len, int sleep_time)
{
	if (SendQueueFunc(sz_buffer, data_len, sleep_time))
		return true;

	// try again
	Sleep(1000);
	wxLogMessage(wxString::Format("%s(%d): Send queue fail. try again after 1 second.", __FILE__, __LINE__));
	cout << "Send queue fail. try again after 1 second" << endl;
	return SendQueueFunc(sz_buffer, data_len, sleep_time);
}

void PIS2::GetQueue()
{
	//if (!m_profile->GetToPis2())
	//	return;

	this->Connect();

	//put queue
	//char sz_reply_name[16] = {0};
	char sz_corralation_id[32] = { 0 };
	//sprintf_s(sz_reply_name, 16, "%s", "AOI.RLY");
	sprintf_s(sz_corralation_id, 32, "%s", "1234567890");

	char sz_queque_handle[32] = { 0 };
	char sz_time_stamp[16] = { 0 };

	//    this->Lock();
	wxLogMessage(wxString::Format("%s(%d): Wait for locker.", __FILE__, __LINE__));
	::WaitForSingleObject(h_locker, INFINITE);
	int nRt = 0;

	wxLogMessage(wxString::Format("%s(%d): open queue start.", __FILE__, __LINE__));
	//todo 当前平台设备编号
	nRt = openQueue(m_conn_handle,"" /*m_profile->GetMachineSn().char_str()*/, 1000000, 5000, 500000, 2500, 0, 0, ""/*m_profile->GetMachineSn().char_str()*/, sz_queque_handle, 0);
	if (nRt != DEF_SUCCESS)
	{
		wxLogError(wxString::Format("%s(%d): Open queue error. Return %d", __FILE__, __LINE__, nRt));
		wxMessageBox(wxString::Format("PIS2 MCMQ openQueue error code : %d", nRt));
		::SetEvent(h_locker);
		this->Connect(true);
		return;
	}

	wxLogMessage(wxString::Format("%s(%d): open queue end. Return %d", __FILE__, __LINE__, nRt));

	wxLogMessage(wxString::Format("%s(%d): get queue start.", __FILE__, __LINE__));

	memset(sz_corralation_id, 0, 32);
	char sz_buffer[10240] = { 0 };
	int data_len = -99999;
	//todo
	nRt = getQueue(m_conn_handle, sz_queque_handle,"" /*m_profile->GetMachineSn().char_str()*/, sz_time_stamp,
		sz_corralation_id, 2000, 10240, (unsigned char *)sz_buffer, &data_len);
	if (nRt != DEF_SUCCESS)
	{
		wxLogError(wxString::Format("%s(%d): get queue error. %d", __FILE__, __LINE__, nRt));
		wxMessageBox(wxString::Format("PIS2 MCMQ getQueue error code : %d", nRt));
		::SetEvent(h_locker);
		this->Connect(true);
		return;
	}

	wxLogMessage(wxString::Format("%s(%d): get queue end. Return %d. Data %d\n%s", __FILE__, __LINE__, nRt, data_len, sz_buffer));

	if (data_len <= 0)
	{
		wxLogError(wxString::Format("%s(%d): %s", __FILE__, __LINE__, _T("PIS2 MCMQ getQueue return 0 but data length is 0.")));
		wxMessageBox(_T("PIS2 MCMQ getQueue return 0 but data length is 0."));
		::SetEvent(h_locker);
		this->Connect(true);
		return;
	}

	wxLogMessage(wxString::Format("%s(%d): close queue start.", __FILE__, __LINE__));

	nRt = closeQueue(m_conn_handle, sz_queque_handle);
	if (nRt != DEF_SUCCESS)
	{
		wxLogError(wxString::Format("%s(%d): close queue error. %d", __FILE__, __LINE__, nRt));
		wxMessageBox(wxString::Format("PIS2 MCMQ closeQueue error code : %d", nRt));
		::SetEvent(h_locker);
		this->Connect(true);
		return;
	}

	wxLogMessage(wxString::Format("%s(%d): close queue end. Return %d", __FILE__, __LINE__, nRt));

	::SetEvent(h_locker);
}

void PIS2::GetBufferForAuoSn(char * buffer, int size, string lot_no, string chip_id, string user_id, string model_no)
{
}

void PIS2::GetBufferForAuoReport(char * buffer, int size, string unique_id, int result, string ftp_names, string ftp_path, string user_id, string svr_aoi_data, string model_no)
{
}

void PIS2::GetBufferForAuoResult(char * buffer, int size, int result, string user_id, string lot_no, list<string> chip_id)
{
}

PIS2::~PIS2()
{
    if(ms_instance)
    {
        int rs = disconnect(&m_conn_handle);
        if(rs!=0)
            wxLogError(wxString::Format("%s(%d): Disconnect PIS2 error. Code: %d", __FILE__, __LINE__, rs));
        else
            wxLogMessage(" Disconnect PIS2 success.");
    }
}

PIS2* PIS2::GetInstance()
{
    if(ms_instance == 0)
        ms_instance = new PIS2();

    return ms_instance;
}

void PIS2::Lock()
{
	while (m_locker)
	{
		Sleep(5);
		wxLogWarning("locked.");
	}

	m_locker = true;
}

void PIS2::Unlock()
{
	m_locker = false;
}

bool PIS2::Connect(bool re)
{
    if (m_has_connect)
    {
        if(re)
        {
            disconnect(&m_conn_handle);
			Sleep(1000);
        }else
            return true;
    }

    //char sz_ip[32] = {0};
    //char sz_port[32] = {0};
    //sprintf_s(sz_ip, 32, "%s", m_profile->GetMsgIp());
    //sprintf_s(sz_port, 32, "%s", m_profile->GetMsgPort());
    //int nRt = connectMCMQ(&m_conn_handle, sz_ip, sz_port);
	int nRt = connectMCMQ(&m_conn_handle,"" /*m_profile->GetMsgIp().char_str()*/, ""/*m_profile->GetMsgPort().char_str()*/);
    if(nRt == 0)
    {
        m_has_connect = true;
        wxLogMessage(" Connect PIS2 success.");
        return true;
    }
	wxLogError(wxString::Format("%s(%d): Connect PIS2 error. Code: %d", __FILE__, __LINE__, nRt));
	return false;
}

bool PIS2::AUOCheckOperation(string lot_no, string chip_id, string user_id, string model_no, bool & is_valid, char err_msg[1024], char ftp_add[1024], int & m_count, bool & is_input_chip, int & lot_qty, int sleep_time)
{
	return false;
}

bool PIS2::AUOReport(string unique_id, int result, string ftp_names, string ftp_path, string user_id, string svr_aoi_data, string model_no, char err_msg[1024])
{
	return false;
}

bool PIS2::AUOSendResult(int result, string user_id, string lot_no, list<string> chip_id, string & grade_change, char err_msg[1024])
{
	return false;
}

