#pragma once
#include <wx/wx.h>
#include <wx/thread.h>
#include <vector>
#include <io.h>
#include "../ImageModel.h"
#include "../UserInterface/VerifyDlg.h"
#include "../Util/ThreadPool.h"
#include <wx/busyinfo.h>
#include "../BO/DataElement.h"

class AutomaticDetetionThread :
	public wxThread
{
public:
	enum 
	{
		ID_CREATE_REJUDE_UI = 10000,
		ID_SEND_READ_SINGLE,
		ID_NO_PIC,
		ID_SEND_NG_SINGLE,
	};
	AutomaticDetetionThread(wxWindow *frame);
	virtual ~AutomaticDetetionThread();

	virtual void *Entry();
	virtual void OnExit();
	inline void SetRunFlag(bool flag)
	{
		m_b_run_flag = flag;
	};
	inline bool GetRunFlag()
	{
		return m_b_run_flag;
	};
	inline void SetImageModel(ImageModel *model)
	{
		m_gmodel = model;
	};
	string GetNowTime();
	void getJustCurrentDir(string path, vector<string>& files);

	inline void SetPlanOneRunFlag(bool flag)
	{
		m_b_planone_run_flag = flag;
	};
	inline bool GetPlanOneRunFlag()
	{
		return m_b_planone_run_flag;
	};
	inline void SetType(int type)
	{
		m_type = type;
	};

	inline void SetDectionList(list<string> decList)
	{
	//	m_list_is_dection = decList;
		m_list_is_dection.assign(decList.begin(), decList.end());
	};
	static void SetFinFileFlag(int result);
	bool GetDirCreateTime(TCHAR* DirName, SYSTEMTIME& stime);

	bool LoadCsvFile(const string& csv);
	string SplitString(const string& line, vector<string>& vectorStr, const string& delimiter);

private:
	wxWindow *m_frame;

	bool m_b_run_flag;
	ImageModel *m_gmodel;
	VerifyDlg	*m_verifyDlg;
	int			m_type;
	ThreadPool	*m_thread_pool;

	wxMutex *m_mutex;

	list<pic_data> m_list_pic_data;
	static int m_i_fin_file_num;//��ǰƬ���Ƿ������ (��Ƭ���µ�ͼƬ����)

	bool m_b_planone_run_flag;

	bool m_b_first;//����ʹ�ã���ʽʹ��ʱ����Ҫɾ��

	list<string> m_list_is_dection;
	int m_i_delta_T;//ʱ���,�����ж��Ƿ�Ϊ�����ļ���
	wxString m_wxs_dection_catalogue;//���¼��Ŀ¼

	wxBusyInfo *busy_info;
	//���ļ���Ϊ��ʱ����������
	wxArrayString m_delayImage;
	HasBeenTest* m_hasBeenTest;

};

