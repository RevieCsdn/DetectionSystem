#pragma once
#include <wx/wx.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <time.h>
#include <string>
#include <wx/dir.h>
#include <fstream>
#include "./AI/curl_ai_client_lib.h"
#include "./UserInterface/Refreshwindow.h"
#include "./UserInterface/ListPanel.h"
#include "./UserInterface/ControlPanel.h"
#include "./Util/Profile.h"
#include "./UserInterface/DetectionSetDlg.h"
#include "./UserInterface/SocketCenter.h"
#include "./Util/bumpDetector.h"
#include "./UserInterface/VerifyDlg.h"
#include "./UserInterface/ResultDlgPanel.h"
#include "json/json.h"
#include "Util/004-foreignMaterialInspect.h"
#include "Util/lizi.h"
#include "./BO/DataElement.h"
#include "./UserInterface/ImagePool.h"
#include "./Util/ThreadPool.h"
#include "./Util/ForeignMaterialDetector.h"
#include "./Util/Profile.h"
#include "./Util/MyThreadPool.h"

#define MAXIMAGENUM 900

class NoTestDlg;
using namespace  std;

//class Profile;

struct file_struct
{
	wxString pic_name;
	wxString file_time;
	bool	is_result;
	bool	lizi_TorF;
};

struct  paper_file
{
	wxString file_name;
	list<file_struct> pic_list;
	wxString file_time;
	bool	paper_result;
	bool	paper_lizi_result;
	int		pic_num;
};

struct pic_data
{
	cv::String image_path;
	string	   image_name;
	cv::Mat	   image_data;
};

// struct lizi_paper
// {
// 	wxString file_name;
// 	wxString file_time;
// 	bool result;
// };

typedef unsigned int Idtype;
class ImageMsg
{
public:
	double      m_pos_x;
	double      m_pos_y;
	Idtype      m_element_id;
	int         m_img_index;
	ImageU1     m_image;
	list<wxRect> m_err_pos_list;
};
//////////////////////
class PostPara
{
public:
	list<SplitRect>* m_splitList;
	ImageU1 m_puzzle;
	vector<wxRect> m_errRedVec;

};
class ImageModel
{
private:
	static wxWindow				*m_frame;
	static Refreshwindow		*m_refresh_window;
	static ListPanel			*m_list_window;
	ControlPanel				*m_control_window;
	static ResultDlgPanel		*m_resultdlg_winodw;
	Profile						*m_profile;

	static SocketCenter			*m_socket_center;

	ModelPanelWindow			*m_model_panel;
	ImagePool					*m_image_pool;
	NoTestDlg* m_noTestDlg;

	static MyThreadPool* m_threadPool;

public:
	static ImageModel	*GetInstance(wxWindow *frame);
	enum
	{
		ID_CREATE_REJUDE_UI = 900,
		ID_SEND_READ_SINGLE,
	};
public:
	ImageModel(wxWindow *frame);
	virtual ~ImageModel();

	void SetProfile(Profile *profile);
	void SetListData(ListData* data);
	void SetResultLabel(float okNum,float ngNum,float ratio);

	void SetDataorganization(DataOrganization *data, string name, string path);
	void OnDataRecotery();
	void DeleteSocketThread();

	void SetNoTestData(NoTestData* data, string name, string path);
	void NoTestDataRecotery(const vector<NoTestElement>& eleVec);

	void ClearPaperFileList();
	void PlanOneAutoBtnFlag(bool flag);

	static void PostImageFun(PostPara& para, /*curl_interface& curlIf,*/ wxImage& tempImage);

	inline void SetPreOkNum(int num)
	{
		m_preOkNum = num;
	};
	inline void SetPreNgNum(int num)
	{
		m_preNgNum = num;
	};

	inline void SetImageModelWindow(Refreshwindow *panel)
	{
		m_refresh_window = panel;
	};
	inline void SetListWindow(ListPanel *panel)
	{
		m_list_window = panel;
	};
	inline void SetControlWindow(ControlPanel *panel)
	{
		m_control_window = panel;
	};
	inline void SetResultWinodw(ResultDlgPanel *panel)
	{
		m_resultdlg_winodw = panel;
	};
	inline bool GetTestingStartFlag()
	{
		return m_testing_start_flag;
	};
	inline void SetTestingStartFlag(bool flag)
	{
		m_testing_start_flag = flag;
	};
	inline ControlPanel* GetControlWindow()
	{
		m_list_recipe = m_model_panel->GetListRecipe();
		return m_control_window;
	};
	inline SocketCenter* GetSocketCenter()
	{
		return m_socket_center;
	};
	inline wxString GetTestingPath()
	{
		return m_save_path;
	};
	inline static string GetImageName()
	{
		return m_image_name;
	};
	inline void SetImageName(string imagename)
	{
		m_image_name = imagename;
	};
	inline wxString GetSavePath()
	{
		return m_save_path;
	};
	inline list<s_verify> GetListData()
	{
		return m_list_data;
	};
	inline wxString GetSaveIP()
	{
		return m_save_ip;
	};
	inline int GetSavePort()
	{
		return m_save_port;
	};
	inline void ClearListData()
	{
		m_list_data.clear();
	};
	inline int GetSaveICbinaryValue()
	{
		return m_save_ic_binaryValue;
	};
	inline int GetSaveICmedianBlurSize()
	{
		return m_save_ic_medianBlurSize;
	};
	inline int GetReadAddr()
	{
		return m_save_read_addr;
	};
	inline wxString GetReadType()
	{
		return m_save_read_type;
	};
	inline int GetWriteOKAddr()
	{
		return m_save_write_OK_addr;
	};
	inline wxString GetWriteOKType()
	{
		return m_save_write_OK_type;
	};
	inline int GetWriteNGAddr()
	{
		return m_save_write_NG_addr;
	};
	inline wxString GetWriteNGType()
	{
		return m_save_write_NG_type;
	};
	inline wxString GetShowPath()
	{
		return m_show_img_string;
	};
	inline wxString GetSavePathCsv()
	{
		return m_save_path_csv;
	};
	inline void SetSavePathCsv(wxString path)
	{
		m_save_path_csv = path;
	};
	inline int GetSingle()
	{
		return m_save_single;
	};
	inline void SetSingle(int i_single)
	{
		m_save_single = i_single;
	};
	inline static cv::Mat GetCvSrcmat()
	{
		return m_cv_srcmat;
	};
	inline static string GetImagePath()
	{
		return m_image_path;
	};
	inline void SetImagePath(string imagepath)
	{
		m_image_path = imagepath;
	};
	inline static int GetMatWidth()
	{
		return m_mat_width;
	};
	inline static int GetMatHeight()
	{
		return m_mat_height;
	};
	inline static void SetListPicData(list<pic_data> *m_list)
	{
		m_list_pic_data = m_list;
	};
	//////////////
	static void SetImageResult(wxString result);

	wxString GetImageResult();

	void SetDrawRect(bool flag, int pen_state);
	void SetDrawNoTestRect(bool flag,int penState);

public:
	static string GetNowTime(int flag);
	static void SaveImageData(wxImage image, string name, string image_name, wxString save_path);
	static wxRect SaveNGRect(wxRect wx_rect, cv::Rect cv_rect);
	bool PostAnImage2AI(gf::Point2d top_left, ImageMsg *im, curl_interface *curl_if, wxImage &ng_image, double len, wxString filename, int num, wxRect wx_rect, cv::Rect cv_rect, list<wxRect> &temp_rect, string image_name, wxString save_path);
	static int PostAnAllImage2AI(list<SplitRect> &list_split, ImageU1 &split_img, curl_interface *curl_if, SingleImage &sing_img, double len, wxImage &ng_image, vector<wxRect> &ErrRect, list<wxRect> &temp_rect, string temp_pic_name, wxString temp_path);
	static bool AIresult(double xx, double yy, double rw, double rh, double len, wxImage &NG_image, int num, wxString filename, wxRect wx_rect, cv::Rect cv_rect, list<wxRect> &temp_rect, string image_name, wxString save_path);
	//Menu
	static bool OnLoadImage(cv::String image_path, string image_name);
	void OnSetDetectionPath();
	void OnSetParemeter();
	void OnSetAddr();
	void OnOpenFileAlgorithmImg();
	void OnOpenFileAIImg();
	void OnOpenFileExcel();
	void OnOpenFileNowExcel();
	void OnOpenFileRecordExcel();

	//Control
	void OnConnectSocket();
	void OnConnectAI();
	static void OnDetection(bool send_result_flag, cv::Mat cvSrcmat, string image_name, string image_path, int mat_width, int mat_height);
	static bool OnLiziDetection(wxString grp_path, string grp_name, wxString org_path, string org_name);
	static bool GotoLizi(wxString Round_robin_path, wxString  target_path);
	bool OnSelfMotion();
	//ResultDlgPanel
	static void ChangeResult();
	void OnAddModel();
	void OnClearModel();
	void OnSaveModel();

	void OnAddNoTestModel();
	void OnClearNoTestModel();
	void OnSaveNoTestModel();

	vector<NoTestImage_t> GetNoTestImageVec();

	void OnNoPic();
	void OnNGSingle();

	void SendReadSingleOver();

	static void OnSaveFailurePics(bool result, int num);
	static bool OnCountRadio(string file_name, wxString &wxs_ratio);
	static string SplitString(const string& line, vector<string>& vectorStr, const string& delimiter);

	void OnSaveRecord(bool result, int num);
	void ReadRecordXls(const string& xls, vector<string>& vectorStr);
	string SplitRecordString(const string& line, vector<string>& vectorStr, const string& delimiter);

	static void Lock_(bool flag);

	static int	Task1(PVOID p);
private:
	static ImageModel			*m_instance;
//	wxMutex						*m_mutex;
//	VerifyDlg					*m_verifyDlg;
	bool						m_testing_start_flag;
	static list<s_verify>		m_list_data;
	static map<string, list<wxRect>> m_map_red_rect;
	static wxString				m_show_img_string;
	paper_file					m_paper_file;
	file_struct					m_file_struct;
	static list<paper_file>		m_list_paper_file;
//	list<file_struct>			m_list_file_struct;
	bool						m_is_draw_rect;
	static list<Image_Properties> m_list_image_model;
	static vector<NoTestImage_t> m_noTestImageVec;

//	list<lizi_paper>			m_list_lizi_result;  
//	paper_file					m_now_paper_file;//当前片子
	static int					m_paper_file_num;//当前片子中有多少图片
	int							m_draw_pen_state;
	//Profile
	//Connect
	bool						m_connect_btn_status_flag;
	wxString					m_save_ip;
	int							m_save_port;
	wxString					m_save_path;
	wxString					m_save_path_csv;
	static wxString				m_save_path_img;
	static wxString				m_save_path_excel;
	static int					m_excel_path_now_num;//Excel表里路径超链接位置
	int							m_save_single;
	
//	static wxString				m_save_report_path;
	int m_NGNum;
	int m_OKNum;
	int m_allPicesNum;
	int m_radio;
	//Parameter
	static int					m_save_ic_binaryValue;
	static int					m_save_ic_backValue;
	static int					m_save_ic_medianBlurSize;
	//Addr
	int							m_save_read_addr;
	wxString					m_save_read_type;
	static int					m_save_write_OK_addr;
	static wxString				m_save_write_OK_type;
	static int					m_save_write_NG_addr;
	static wxString				m_save_write_NG_type;
	//AI
	static bool					m_save_is_use_ai;
	static wxString				m_save_ai_path;
	//OpenCV
	static cv::Mat				m_cv_srcmat;
	static string				m_image_path;
	static string				m_image_name;
	static int					m_mat_width;
	static int					m_mat_height;
	static list<wxRect>			m_wx_vector_list;
	//Recipe
	string						m_recipe_name;
	string						m_recipe_path;
	list<Element*>				m_element_list;

	string m_noTestRecName;
	string m_noTestRecPath;

	static string				AI_pic_name;
	static wxMutex				*m_mutex;

	static list<pic_data>		*m_list_pic_data;
	static string				m_now_detection_file_name;
	static int					m_now_detection_file_all_num;
	static int					m_now_detection_file_num;
	static list<string>			m_list_recipe;

	static wxString	 m_save_directory_name;
	wxFileOffset m_offset;
	string m_getFirstLine;
	int m_lineNum;
	vector<string> m_lineVec;
	vector<vector<string>> m_allLineVec;

	//
	static int m_preOkNum;
	static int m_preNgNum;
	static float m_preRatio;
	static bool m_getPreData;

	static ListData* m_data;
	static wxString m_imageResult;
	static int img_num;
	static vector<cv::Rect> vecter_result;
	static wxString wxs_NG_num;
	static vector<wxRect> wx_vector_list;
	
	static mutex m_mutexPic;
	static mutex m_mutexPost;
	static mutex m_mutexThread;
	static mutex m_mutexArti;
	static mutex m_mutexNum;


	static list<PostPara> m_postParaVec;
	static SingleImage* s_image_temp;

	static list<wxRect> m_rectList;
	static string m_imageName;
	static string m_imagePath;
	static int m_aiNgNum;
	
};

