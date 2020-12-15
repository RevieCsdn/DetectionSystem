#ifndef __PIS2_H__
#define __PIS2_H__

#include <MCMQWin32Api.h>
#include <wx/wx.h>
#include <Wininet.h>
#include <WinBase.h>
#include "./Profile.h"
//#include "../Util/preDefine.h"
#include <list>

using namespace std;

class PIS2
{
private:
    static PIS2 *ms_instance;
    static bool m_locker;

    bool m_has_connect;
    Profile *m_profile;
    int m_conn_handle;
    HANDLE  h_locker;


protected:
    PIS2();
		wxString StringBetween(wxString &str_src, wxString &str_b, wxString &str_e);
		bool SendQueueFunc(char sz_buffer[4096], int &data_len, int sleep_time);
		bool SendQueue(char sz_buffer[4096], int &data_len, int sleep_time);
		void GetQueue();

		void GetBufferForAuoSn(char *buffer, int size, string lot_no, string chip_id, string user_id, string model_no);
		void GetBufferForAuoReport(char *buffer, int size, string unique_id, int result, string ftp_names, string ftp_path, string user_id, string svr_aoi_data, string model_no);
		void GetBufferForAuoResult(char *buffer, int size, int result, string user_id, string lot_no, list<string> chip_id);

public:
    virtual ~PIS2();
    static PIS2* GetInstance();
    void  Lock();
    void  Unlock();

    bool Connect(bool re = false);
		bool AUOCheckOperation(string lot_no, string chip_id, string user_id, string model_no, bool &is_valid, char err_msg[1024], char ftp_add[1024], int &m_count, bool &is_input_chip, int &lot_qty, int sleep_time);
		bool AUOReport(string unique_id, int result, string ftp_names, string ftp_path, string user_id, string svr_aoi_data, string model_no, char err_msg[1024]);
		bool AUOSendResult(int result, string user_id, string lot_no, list<string> chip_id, string &grade_change, char err_msg[1024]);
};

#endif  //__PIS2_H__