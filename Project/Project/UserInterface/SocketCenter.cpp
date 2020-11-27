#include "SocketCenter.h"

#define SOCKETCOUT cout << "SocketCenter >> "

SocketCenter::SocketCenter(wxWindow *frame, string IP, int port):wxThread(wxTHREAD_DETACHED), m_IP(IP), m_Port(port), m_skt(NULL), m_Test_Connect(true), m_result(false)
,m_i_read_addr(0), m_wxs_read_type(""), m_i_write_OK_addr(0), m_wxs_write_OK_type(""), m_i_write_NG_addr(0), m_wxs_write_NG_type("")
{
	m_frame = frame;
	m_btn_anto_flag = false;
	m_run_flag = true;
	m_err_message = "";
	m_type = 0;

	m_mutex = new wxMutex();
	ConnectInit();
}

SocketCenter::~SocketCenter(void)
{
	if (m_mutex)
	{
		delete m_mutex;
	}

	closesocket(m_skt);
	WSACleanup();
}

bool SocketCenter::ConnectInit()
{
	WORD    versionRequested;
	WSADATA wsaData;
	int     err;

	versionRequested = MAKEWORD(2, 2);
	err = WSAStartup(versionRequested, &wsaData);
	if(err != 0)
	{
		if(err == WSASYSNOTREADY)
			SOCKETCOUT <<  "Create Socket failed.WSAStartup: WSA SYS NOT READY" << endl;
		else if(err == WSAVERNOTSUPPORTED)
			SOCKETCOUT <<  "Create Socket failed.WSAStartup: WSA VER NOT SUPPORTED" << endl;
		else if(err == WSAEINVAL)
			SOCKETCOUT <<  "Create Socket failed.WSAStartup: WSAEINVAL" << endl;
		else
			SOCKETCOUT <<  "Create Socket failed.WSAStartup: " << err << endl;
		return false;
	}

	if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		SOCKETCOUT <<  "Create Socket failed.WSADATA version : " << LOBYTE(wsaData.wVersion) << HIBYTE(wsaData.wVersion) << endl;
		return false;
	}

	return true;
}
 
bool SocketCenter::CreateSocket()
{
	wxString msg;

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(m_IP.c_str());
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(m_Port);

	int con_time = 0;
	int err_code = 0;

	bool con_suc = false;
	
	m_skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	do 
	{
		if(connect(m_skt, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == 0)
		{
			con_suc = true;

			break;
		}

		err_code = GetLastError();
		SOCKETCOUT << "Connect Socket failed: " << err_code << ", with ip: " << m_IP << ":" << m_Port << endl;
		msg = wxString::Format("连接PLC(socket)失败，错误代码：%d， IP： %s， 端口： %d", err_code, m_IP, m_Port);
		MyLog::LogError(msg.mb_str());
		Sleep(200);
		con_time += 200;
	} while (con_time < 3000);

	if (!con_suc)
	{
		return false;
	}

	if(true == m_Test_Connect)
	{
		m_Test_Connect = false;
		SOCKETCOUT << "Test: Connect Socket success, with ip: " << m_IP << ":" << m_Port << endl;
		msg = wxString::Format("测试：PLC(socket)连接成功， IP： %s， 端口： %d", m_IP, m_Port);
		MyLog::LogMessage(msg.mb_str());
		closesocket(m_skt);
	}
	else
	{
		SOCKETCOUT << "Connect Socket success, with ip: " << m_IP << ":" << m_Port << endl;
		msg = wxString::Format("正式：PLC(socket)连接成功， IP： %s， 端口： %d", m_IP, m_Port);
		MyLog::LogMessage(msg.mb_str());
	}

	return true;
}

void *SocketCenter::Entry()
{
	wxString wx_msg = "PLC(socket)线程开始运行";
	MyLog::LogMessage(wx_msg.mb_str());
	char msg[1024] = { 0 };

	bool need_recon = true;
	int  recon_times = 0;

	// connect
	SOCKETCOUT << "Connect to server." << endl;
	while (m_run_flag)
	{
		Sleep(100);
		
		if (TestDestroy())
		{
			wx_msg = "SocketCenter TestDestroy() error";
			MyLog::LogError(wx_msg.c_str());

			wxMessageBox(wx_msg);
			break;
		}

		if (!m_run_flag)
		{
			return 0;
		}

		if (need_recon && !this->CreateSocket())
		{
			SOCKETCOUT << "ERROR>> Thread can not connect to server." << endl;
			++recon_times;
			wx_msg = wxString::Format("PLC(socket)线程连接服务器失败， 当前重连时间：%d", recon_times);
			MyLog::LogWarning(wx_msg.mb_str());
			if (recon_times > 10)
			{
				return false;
			}
			continue;
		}
		else
		{
			need_recon = false;
		}
		recon_times = 0;

		if (!m_run_flag)
		{
			return 0;
		}

		if (PLAN_ONE)
		{
			if (m_btn_anto_flag)
			{
				//wxString转成char*
				string temp = m_wxs_read_type.c_str();
				char *temp_read_type = (char*)temp.c_str();

				if (this->SendMsg_Read(m_i_read_addr, temp_read_type, 1))
				{
					//m_btn_anto_flag = false;
				}
			}
		}

		if (!m_run_flag)
		{
			return 0;
		}
	}

	return NULL;
}

bool SocketCenter::SendMsg_Write(int addr, char* type, int num, unsigned int* data)
{
// 	write_addr = addr;	//用于断线重发
// 	write_type = type;
// 	write_num = num;
// 	write_data = data;
	wxString msg;
	char SendBuff_1[8] = { 0x50, 0x00, 0x00, 0xff, 0xff, 0x03, 0x00 };
	char SendBuff_2[2] = { 0 };
	char SendBuff_3[8] = { 0x00, 0x00, 0x00, 0x01, 0x14, 0x00, 0x00 };
	char w_addr[3] = { 0 };
	char w_type[3] = { 0 };
	char w_num[3] = { 0 };
	char w_data[100] = { 0 };
	char SendBuff[256] = { 0 };
	//匹配
	if (strcmp(type, "m") == 0 || strcmp(type, "M") == 0)
	{
		if (addr >= 0 && addr <= 7670)
		{
			w_type[0] = 0x00;
			w_type[1] = 0x90;
			w_type[2] = '\0';
		}
		else
		{
			cout << "Soft element code：M   " << "The initial soft element number is not in the range" << endl;
			return false;
		}
	}
	else if (strcmp(type, "d") == 0 || strcmp(type, "D") == 0)
	{
		if (addr >= 0 && addr < 8000)
		{
			w_type[0] = 0x00;
			w_type[1] = 0xa8;
			w_type[2] = '\0';
		}
		else
		{
			cout << "Soft element code：D   " << "The initial soft element number is not in the range" << endl;
			return false;
		}
	}
	else
	{
		cout << "Soft element code type error" << endl;
		return false;
	}
	//高低字节对调 起始位置
	if (addr >= 0 && addr < 256)
	{
		w_addr[0] = addr;
		w_addr[1] = 0;
		w_addr[2] = '\0';
	}
	else if (addr >= 256)
	{
		w_addr[0] = addr % 256;
		w_addr[1] = addr / 256;
		w_addr[2] = '\0';
	}
	//高地字节对调	点数
	if (num >= 0 && num < 256)
	{
		w_num[0] = num;
		w_num[1] = 0;
		w_num[2] = '\0';
	}
	else if (num >= 256)
	{
		w_num[0] = num % 256;
		w_num[1] = num / 256;
		w_num[2] = '\0';
	}
	int j = 0;
	unsigned int *temp = data;

	//高地字节对调	数据
	for (int i = 0; i < num; i++)
	{
		if (temp[i] >= 0 && temp[i] < 256)
		{
			w_data[j++] = temp[i];
			w_data[j++] = 0;
		}
		else if (temp[i] >= 256)
		{
			w_data[j++] = temp[i] % 256;
			w_data[j++] = temp[i] / 256;
		}
	}
	w_data[j] = '\0';
	//请求数据长度
	int length = 12 + num * 2;
	sprintf_s(SendBuff_2, "%c", length);
	//发送数据拼接
	memcpy(SendBuff, SendBuff_1, 7);
	memcpy(SendBuff + 7, SendBuff_2, 1);
	memcpy(SendBuff + 8, SendBuff_3, 7);
	memcpy(SendBuff + 15, w_addr, 2);
	memcpy(SendBuff + 17, w_type, 2);
	memcpy(SendBuff + 19, w_num, 2);
	memcpy(SendBuff + 21, w_data, num * 2 + 1);
	//发送
	m_mutex->Lock();
	if (SendMsg(m_skt, SendBuff, 21 + num * 2))
	{
		m_mutex->Unlock();

		msg = "写：发送socket信息失败";
		MyLog::LogError(msg.mb_str());

		m_err_message = "写：发送socket信息失败，请检查后重连";

		wxQueueEvent(m_frame->GetEventHandler(),
			new wxCommandEvent(wxEVT_THREAD, ID_DELETE_SOCKET));

		return false;
	}
	m_mutex->Unlock();

	string temp_data = SendBuff;
	msg = wxString::Format("写：发送起始软元件编号：%d, 软元件代码：%s, 软元件点数：%d, 发送数据：%s", addr, type, num, temp_data);
	MyLog::LogMessage(msg.mb_str());
//	cout << msg << endl;
	//======================================
	char Recv_data[1024];
	ZeroMemory(Recv_data, 1024);

	m_mutex->Lock();
	if (RecvMsg(m_skt, Recv_data))
	{
		m_mutex->Unlock();

		msg = "写：接收socket信息失败";
		MyLog::LogError(msg.mb_str());

		m_err_message = "写：接收socket信息失败，请检查后重连";

		wxQueueEvent(m_frame->GetEventHandler(),
			new wxCommandEvent(wxEVT_THREAD, ID_DELETE_SOCKET));

		return false;
	}
	m_mutex->Unlock();
	//========================================

	return true;
}

bool SocketCenter::SendMsg_Read(int addr, char * type, int num)
{
// 	read_addr = addr;	//如果需要重连操作，则需要此参数
// 	read_type = type;
// 	read_num = num;

	wxString msg = "";
	char SendBuff_1[8] = { 0x50, 0x00, 0x00, 0xff, 0xff, 0x03, 0x00 };
	char SendBuff_2[2] = { 0 };
	char SendBuff_3[8] = { 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00 };
	char w_addr[3] = { 0 };
	char w_type[3] = { 0 };
	char w_num[3] = { 0 };
	char SendBuff[22] = { 0 };

	//匹配
	if (strcmp(type, "m") == 0 || strcmp(type, "M") == 0)
	{
		if (addr >= 0 && addr < 7680)
		{
			w_type[0] = 0x00;
			w_type[1] = 0x90;
			w_type[2] = '\0';
		}
		else
		{
			cout << "Soft element code：M   " << "The initial soft element number is not in the range" << endl;
			msg = "Soft element code：M. The initial soft element number is not in the range";
			MyLog::LogError(msg.mb_str());
			return false;
		}
	}
	else if (strcmp(type, "d") == 0 || strcmp(type, "D") == 0)
	{
		if (addr >= 0 && addr < 8000)
		{
			w_type[0] = 0x00;
			w_type[1] = 0xa8;
			w_type[2] = '\0';
		}
		else
		{
			cout << "Soft element code：D   " << "The initial soft element number is not in the range" << endl;
			msg = "Soft element code：D. The initial soft element number is not in the range";
			MyLog::LogError(msg.mb_str());
			return false;
		}
	}
	else
	{
		cout << "Soft element code type error" << endl;
		msg = "Soft element code type error";
		MyLog::LogError(msg.mb_str());
		return false;
	}
	//高低字节对调 起始地址
	if (addr < 256 && addr >= 0)
	{
		w_addr[0] = addr;
		w_addr[1] = 0;
		w_addr[2] = '\0';
	}
	else if (addr >= 256)
	{
		w_addr[0] = addr % 256;
		w_addr[1] = addr / 256;
		w_addr[2] = '\0';
	}
	//高低字节对调	点数
	if (num < 256 && num >= 0)
	{
		w_num[0] = num;
		w_num[1] = 0;
		w_num[2] = '\0';
	}
	else if (num >= 256)
	{
		w_num[0] = num % 256;
		w_num[1] = num / 256;
		w_num[2] = '\0';
	}
	//请求数据长度
	int length = 12;
	sprintf_s(SendBuff_2, "%c", length);
	//发送数据拼接
	memcpy(SendBuff, SendBuff_1, 7);
	memcpy(SendBuff + 7, SendBuff_2, 1);
	memcpy(SendBuff + 8, SendBuff_3, 7);
	memcpy(SendBuff + 15, w_addr, 2);
	memcpy(SendBuff + 17, w_type, 2);
	memcpy(SendBuff + 19, w_num, 3);
	//发送
	m_mutex->Lock();

	if (SendMsg(m_skt, SendBuff, 21))
	{
		m_mutex->Unlock();

		msg = "读：发送socket信息失败";
		MyLog::LogError(msg.mb_str());

		m_err_message = "读：发送socket信息失败，请检查后重连";

		wxQueueEvent(m_frame->GetEventHandler(),
			new wxCommandEvent(wxEVT_THREAD, ID_DELETE_SOCKET));

		return false;
	}
	m_mutex->Unlock();

	string temp_data = SendBuff;
	msg = wxString::Format("读：发送起始软元件编号：%d, 软元件代码：%s, 软元件点数：%d， 发送数据：%s", addr, type, num, temp_data);
	MyLog::LogMessage(msg.mb_str());
//	cout << msg << endl;
	//接收
	//======================================
	char Recv_data[1024];
	ZeroMemory(Recv_data, 1024);

	m_mutex->Lock();
	if (RecvMsg(m_skt, Recv_data))
	{
		m_mutex->Unlock();
		
		msg = "读：接收socket信息失败";
		MyLog::LogError(msg.mb_str());

		m_err_message = "读：接收socket信息失败，请检查后重连";

		wxQueueEvent(m_frame->GetEventHandler(),
			new wxCommandEvent(wxEVT_THREAD, ID_DELETE_SOCKET));

		return false;
	}
	m_mutex->Unlock();
	//========================================

	if (PLAN_ONE)
	{
		if (Recv_data[11] != 0)//确认后通信协议后更改
		{
			//		cout << "bbbbbbbbbbbb:" << m_frame->GetEventHandler() << endl;
			m_type = 0;
			if (Recv_data[11] == 1)
			{
				m_type = 1;
			}
			else if (Recv_data[11] == 2)
			{
				m_type = 2;
			}
			else
			{
				m_type = Recv_data[11];
			}

			wxQueueEvent(m_frame->GetEventHandler(),
				new wxCommandEvent(wxEVT_THREAD, ID_READ_AND_DECTION));

			return true;
		}

	//	cout << Recv_data[0] << endl;
	}

	return false;
}

int SocketCenter::SendMsg(SOCKET socket, char * send_msg, int len)
{
	wxString msg;
	if(SOCKET_ERROR == send(socket, send_msg, len, 0))
	{
		int err_code = WSAGetLastError();

		switch(err_code)
		{
		case WSANOTINITIALISED:
			SOCKETCOUT << "Send socket error. Code: WSANOTINITIALISED, wsastartup() first." << endl;
			break;
		case WSAENETDOWN:
			SOCKETCOUT << "Send socket error. Code: WSAENETDOWN, net failure." << endl;
			break;
		case WSAENETRESET:
			SOCKETCOUT << "Send socket error. Code: WSAENETRESET, need reconnect." << endl;
			break;
		case WSAENOTCONN:
			SOCKETCOUT << "Send socket error. Code: WSAENOTCONN, not connect." << endl;
			break;
		case WSAEOPNOTSUPP:
			SOCKETCOUT << "Send socket error. Code: WSAEOPNOTSUPP, not a  msg_oob connect." << endl;
			break;
		case WSAESHUTDOWN:
			SOCKETCOUT << "Send socket error. Code: WSAESHUTDOWN, connect has be closed." << endl;
			break;
		default:
			SOCKETCOUT << "Send socket error. Code: " << err_code << endl;
		}
		SOCKETCOUT << "ERROR:" << "Send socket error. Code: " << err_code << endl;
		msg = wxString::Format("发送信息失败，错误代码：%d", err_code);
		MyLog::LogError(msg.mb_str());

		return err_code;
	}

	msg = wxString::Format("发送信息成功，信息内容为：%s", send_msg);
	MyLog::LogMessage(msg.mb_str());
//	cout << msg << endl;

	return 0;
}

int SocketCenter::RecvMsg(SOCKET socket, char * send_msg)
{
	wxString msg;
	int rev_num = recv(socket, send_msg, 1024, 0);
	if(SOCKET_ERROR == rev_num)
	{
		int err_code = WSAGetLastError();

		switch(err_code)
		{
		case WSAETIMEDOUT:
			SOCKETCOUT << "receive socket error. Code: WSAETIMEDOUT, time out." << endl;
			break;
		case WSANOTINITIALISED:
			SOCKETCOUT << "receive socket error. Code: WSANOTINITIALISED, wsastartup() first." << endl;
			break;
		case WSAENETDOWN:
			SOCKETCOUT << "receive socket error. Code: WSAENETDOWN, net failure." << endl;
			break;
		case WSAENOTCONN:
			SOCKETCOUT << "receive socket error. Code: WSAENOTCONN, not connect." << endl;
			break;
		case WSAEOPNOTSUPP:
			SOCKETCOUT << "receive socket error. Code: WSAEOPNOTSUPP, not sock_stream." << endl;
			break;
		case WSAESHUTDOWN:
			SOCKETCOUT << "receive socket error. Code: WSAESHUTDOWN, socket has closed." << endl;
			break;
		case WSAEMSGSIZE:
			SOCKETCOUT << "receive socket error. Code: WSAEMSGSIZE, message cut." << endl;
			break;
		case WSAEINVAL:
			SOCKETCOUT << "receive socket error. Code: WSAEINVAL, not bind()." << endl;
			break;
		case WSAECONNABORTED:
			SOCKETCOUT << "receive socket error. Code: WSAECONNABORTED, time out or other" << endl;
			break;
		case WSAECONNRESET:
			SOCKETCOUT << "receive socket error. Code: WSAECONNRESET, closed by server." << endl;
			break;
		default:
			SOCKETCOUT << "receive socket error. Code: " << err_code << endl;
		}
		SOCKETCOUT << "receive socket error. Code: " << err_code << endl;
		msg = wxString::Format("接收信息失败，错误代码：%d", err_code);
		MyLog::LogError(msg.mb_str());
		return err_code;
	}

	msg = wxString::Format("接收信息成功，长度为：%d， 信息内容为：%s", rev_num, send_msg);
	MyLog::LogMessage(msg.mb_str());
//	cout << msg << endl;

	if (20 == rev_num)
	{
		msg = "接收socket信息为异常响应，请重新确认配置后重连";
		MyLog::LogError(msg.mb_str());

		m_err_message = msg;

		wxQueueEvent(m_frame->GetEventHandler(),
			new wxCommandEvent(wxEVT_THREAD, ID_DELETE_SOCKET));
	}
	
	//打印===================================
//	this->RecMsg(rev_num, send_msg);

	return 0;
}

bool SocketCenter::RecMsg(int len, char *data)
{
	data[len] = '\0';
	int temp = 0;
	int tempstr = 0;
	char m_c = 0;
	for(int i = 0; i < len; i++)
	{
		tempstr = data[i];
		for(int j = 1; j >= 0; j--)
		{
			temp = (tempstr >> 4 * j) & 15;
			if(temp > 9)
			{
//					printf("%c", temp - 10 + 'A');
				m_c = temp - 10 + 'A';
				cout << m_c;
			}
			else
			{
//					printf("%d", temp);
				cout << temp;
			}
		}
		printf(" ");
	}
	if(len == 20)
		printf("Abnormal response occurs !!!\n");
	else
		printf("\n");

	return true;
}