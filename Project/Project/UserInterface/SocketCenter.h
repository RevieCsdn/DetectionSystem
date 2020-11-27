#pragma once
#include <iostream>

#include <WinSock2.h>
#include <Windows.h>
#include <iomanip>
#include <cmath>
#include <string>
#include <wx/wx.h>
#include "../Util/PreDefine.h"
#include "../MyLog.h"

using namespace std;
#pragma comment(lib, "ws2_32.lib")

class SocketCenter: public wxThread
{
public:
	enum
	{
		ID_READ_AND_DECTION = 10010,
		ID_DELETE_SOCKET,
	};
	SocketCenter(wxWindow *frame, string IP, int port);
	virtual ~SocketCenter(void);

	virtual void    *Entry();
	virtual void    OnExit() {};
	wxMutex	*m_mutex;

	bool ConnectInit();
	bool CreateSocket();
	int	 RecvMsg(SOCKET socket, char * send_msg);
	int  SendMsg(SOCKET socket, char * send_msg, int len);
	bool SendMsg_Read(int addr, char * type, int num);
	bool SendMsg_Write(int addr, char* type, int num, unsigned int* data);
	//²âÊÔ´òÓ¡ÓÃ
	bool RecMsg(int len, char *data);
	
	inline bool GetResul()
	{
		return this->m_result;
	};
	inline void SetRunFlag(bool flag)
	{
		m_run_flag = flag;
	};
	inline bool GetRunFlag()
	{
		return m_run_flag;
	}
	inline void SetBtnAutoFlag(bool flag)
	{
		m_btn_anto_flag = flag;
	};
	inline bool GetBtnAutoFlag()
	{
		return m_btn_anto_flag;
	};
	inline wxString GetErrMessage()
	{
		return m_err_message;
	};
	inline void SetErrMessage(wxString err_message)
	{
		m_err_message = err_message;
	};
	inline void SetReadAddr(int addr)
	{
		m_i_read_addr = addr;
	};
	inline void SetReadType(wxString type)
	{
		m_wxs_read_type = type;
	};
	inline void SetWriteOKAddr(int addr)
	{
		m_i_write_OK_addr = addr;
	};
	inline void SetWriteOKType(wxString type)
	{
		m_wxs_write_OK_type = type;
	};
	inline void SetWriteNGAddr(int addr)
	{
		m_i_write_NG_addr = addr;
	};
	inline void SetWriteNGType(wxString type)
	{
		m_wxs_write_NG_type = type;
	};
	inline int GetType()
	{
		return m_type;
	};
private:
	wxWindow	*m_frame;
	string		m_IP;
	int			m_Port;
	SOCKET		m_skt;
	bool		m_Test_Connect;
	bool		m_result;
	bool		m_run_flag;
	bool		m_btn_anto_flag;

	wxString	m_err_message;

	int			m_i_read_addr;
	wxString	m_wxs_read_type;
	int			m_i_write_OK_addr;
	wxString	m_wxs_write_OK_type;
	int			m_i_write_NG_addr;
	wxString	m_wxs_write_NG_type;

	int			m_type;
};

