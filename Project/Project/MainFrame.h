#pragma once
#include <wx/wx.h>
#include <wx/datetime.h>
#include <wx/aui/framemanager.h>
#include <wx/display.h>
#include <iostream>
#include "./UserInterface/Refreshwindow.h"
#include "./UserInterface/ListPanel.h"
#include "./UserInterface/ControlPanel.h"
#include "ImageModel.h"
#include "./Util/Profile.h"
#include "./UserInterface/AutomaticDetetionThread.h"
#include "./UserInterface/VerifyDlg.h"
#include "MyLog.h"
#include "./UserInterface/SocketCenter.h"
#include "./UserInterface/ResultDlgPanel.h"
#include "./UserInterface/DetectionSetDlg.h"
#include "./BO/DataElement.h"
#include "./SuperDog/SuperDog.h"
using namespace std;

#undef MainFrame_STYLE
#define MainFrame_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX

class MainFrame :
	public wxFrame
{
	DECLARE_EVENT_TABLE();
public:
	MainFrame(wxWindow* parent, wxWindowID id = -1,
		const wxString& title = _("AI人工智能Bonding异物检系统") + SOFT_VER,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = MainFrame_STYLE);
	virtual ~MainFrame();
	
private:
	enum
	{
		//FIlE
		ID_MENU_LOAD,
		ID_MENU_READ_RECIPE,
		//CHANGE
		ID_MENU_PATH,
		ID_MENU_PARAMETER,
		ID_MENU_ADDR,
		ID_MENU_HISDATA,
		//DRAW
		ID_MENU_DRAW_RECT,
		ID_MENU_DRAW_SQUARE_RECT,
		//File
		ID_MENU_FILE_OPEN_ALGORITHM_IMG,
		ID_MENU_FILE_OPEN_AI_IMG,
		ID_MENU_FILE_OPEN_EXCEL,
		ID_MENU_FILE_OPEN_NOW_EXCEL,
		ID_MENU_FILE_OPEN_RECORD_EXCEL,
		//WINODW
		ID_SHOW_PHOTO_PANEL,
		ID_SHOW_MSG_PANEL,
		ID_SHOW_CONTROL_PANEL,
		//
		
	};
//	wxWindow		*m_frame;

	wxMenuBar   *m_menu_bar;
	wxMenu			*m_menu_load;
	wxMenu			*m_menu_set;
	wxMenu			*m_menu_draw;
	wxMenu			*m_menu_file;
	
	wxAuiManager	m_mgr;

#ifdef __CHECK_SUPER_DOG__
	SuperDog		*m_superdog;
#endif

	int				m_iClientX;
	int				m_iClientY;
	int				m_iMyScrolled_Width;
	int				m_iMyScrolled_Height;
	int				m_iDown_Width;
	int				m_iDown_Height;

	string			m_recipe_name;
	string			m_recipe_path;

	Refreshwindow	*m_refresh_window;
	ListPanel		*m_list_panel;
	ControlPanel	*m_control_panel;
	ResultDlgPanel	*m_result_panel;

	Profile			*m_gProfile;
	ImageModel		*m_gModel;

	AutomaticDetetionThread *m_AutomaticThread;	
	VerifyDlg		*m_verifyDlg;

	wxMutex			*m_mutex;

	bool			m_is_draw_rect;
	//////////////////////////////////
	ListData* m_listData;
	string m_strPath;
private:
	void CreateGUIControls();
 	bool Init();
	void OnClose(wxCloseEvent&);
	void ExitSystem(bool flag);
	void RefreshPreviousData();

	//Menu
	void OnLoadPhoto(wxCommandEvent &);
	void OnReadRecipe(wxCommandEvent &event);
	void OnSetDetectionPath(wxCommandEvent &);
	void OnSetParemeter(wxCommandEvent &);
	void OnSetAddr(wxCommandEvent &);
	void OnDrawRect(wxCommandEvent &event);
	void OnDrawSquareRect(wxCommandEvent &event);
	void OnOpenFileAlgorithmImg(wxCommandEvent &);
	void OnOpenFileAIImg(wxCommandEvent &);
	void OnOpenFileExcel(wxCommandEvent &);
	void OnOpenFileNowExcel(wxCommandEvent &);
	void OnOpenFileRecordExcel(wxCommandEvent &);
	void OnCleraHistoricalData(wxCommandEvent &);
	//Control
	void OnConnectSocket(wxCommandEvent &);
	void OnConnectAI(wxCommandEvent &);
	void OnDetection(wxCommandEvent &);
	void OnSelfMotion(wxCommandEvent &);
	//Refresh
	void OnAddModel(wxCommandEvent &);
	void OnClearModel(wxCommandEvent &);
	void OnSaveModel(wxCommandEvent &);
private:
	//AutomaticDetetionThread
	void OnCreateRejUi(wxThreadEvent &);
	void OnSendReadSingle(wxThreadEvent &);
	void OnNoPic(wxThreadEvent &);
	void OnSendNGSingle(wxThreadEvent &);
	//SocketCenter
	void OnReadAndDection(wxThreadEvent &);
	void OnDeleteSocket(wxThreadEvent &);
	//SuperDog
	void OnSuperDogShow(SuperDogEvent &event);
	//

};

