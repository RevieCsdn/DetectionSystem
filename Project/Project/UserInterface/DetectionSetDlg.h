#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../UserInterface/Refreshwindow.h"
#include <list>
#include <iostream>

using namespace std;

class DetectionSetDlg :
	public wxDialog
{
	DECLARE_EVENT_TABLE()
public:
	enum
	{
		ID_BTN_OK,
		ID_BTN_NO,
		ID_BTN_PATH,
		ID_BTN_PATH_TWO,
		ID_BTN_PATH_SAVE_IMG,
		ID_BTN_PATH_EXCEL,
	};
	DetectionSetDlg(wxWindow* parent, wxWindowID id = -1,
		const wxString& title = "自动检测设置",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION);
	~DetectionSetDlg();

	void CreateGUIcontrols();
	void OnBtnOK(wxCommandEvent &);
	void OnBtnNO(wxCommandEvent &);
	void OnBtnPath(wxCommandEvent &);
	void OnBtnPathCsv(wxCommandEvent &);
	void OnBtnPathSaveImg(wxCommandEvent &);
	void OnBtnPathExcel(wxCommandEvent &);

	inline void SetShowIP(wxString ip)
	{
		m_wxs_ip = ip;
	};
	inline wxString GetShowIP()
	{
		return m_wxs_ip;
	};
	inline void SetShowPort(int port)
	{
		m_i_port = port;
	};
	inline int GetShowPort()
	{
		return m_i_port;
	};
	inline void SetShowPath(wxString path)
	{
		m_wxs_path = path;
	};
	inline wxString GetShowPath()
	{
		return m_wxs_path;
	};
	inline void SetShowPathCsv(wxString path)
	{
		m_wxs_path_csv = path;
	};
	inline wxString GetShowPathCsv()
	{
		return m_wxs_path_csv;
	};
	inline void SetShowPathSaveImg(wxString path)
	{
		m_wxs_path_save_img = path;
	};
	inline wxString GetShowPathSaveImg()
	{
		return m_wxs_path_save_img;
	};
	inline void SetShowPathExcel(wxString path)
	{
		m_wxs_path_excel = path;
	};
	inline wxString GetShowPathExcel()
	{
		return m_wxs_path_excel;
	};
	inline void SetSingle(int i_single)
	{
		m_i_single = i_single;
	};
	inline int GetSingle()
	{
		return m_i_single;
	};
private:
	wxStaticText	*m_lab_ip;
	wxTextCtrl		*m_edit_ip;
	wxStaticText	*m_lab_port;
	wxTextCtrl		*m_edit_port;
	wxStaticText	*m_lab_path;
	wxTextCtrl		*m_edit_path;
	wxStaticText	*m_lab_path_csv;
	wxTextCtrl		*m_edit_path_csv;
	wxStaticText	*m_lab_read_img_path;
	wxTextCtrl		*m_edit_read_img_path;
	wxStaticText	*m_lab_excel_path;
	wxTextCtrl		*m_edit_excel_path;
	wxStaticText	*m_lab_single;
	wxRadioBox		*m_radiobtn_single;

	wxButton		*m_btn_OK;
	wxButton		*m_btn_NO;
	wxButton		*m_btn_path;
	wxButton		*m_btn_path_csv;
	wxButton		*m_btn_path_save_img;
	wxButton		*m_btn_path_excel;

	wxWindow		*m_frame;

	wxString		m_wxs_ip;
	int				m_i_port;
	wxString		m_wxs_path;
	wxString		m_wxs_path_csv;
	wxString		m_wxs_path_save_img;
	wxString		m_wxs_path_excel;
	int				m_i_single;
};

class ParameterSetDig : public wxDialog
{
	DECLARE_EVENT_TABLE()
public:
	enum
	{
		ID_BTN_OK,
		ID_BTN_NO,
	};
	ParameterSetDig(wxWindow* parent, wxWindowID id = -1,
		const wxString& title = "算法参数设置",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION);
	~ParameterSetDig();

	void CreateGUIcontrols();
	void OnBtnOK(wxCommandEvent &);
	void OnBtnNO(wxCommandEvent &);

	inline int GetICbinaryValue()
	{
		return m_i_ic_binaryValue;
	};
	inline void SetICbinaryValue(int binaryValue)
	{
		m_i_ic_binaryValue = binaryValue;
	};
	inline int GetICbackValue()
	{
		return m_i_ic_backValue;
	};
	inline void SetICbackValue(int backValue)
	{
		m_i_ic_backValue = backValue;
	};
	inline int GetICmedianBlurSize()
	{
		return m_i_ic_medianBlurSize;
	};
	inline void SetICmedianBlurSize(int medianBlurSize)
	{
		m_i_ic_medianBlurSize = medianBlurSize;
	};
private:
	wxStaticText	*m_ic_lab_binaryValue;
	wxTextCtrl		*m_ic_edit_binaryValue;
	wxStaticText	*m_ic_lab_backValue;
	wxTextCtrl		*m_ic_edit_backValue;
	wxStaticText	*m_ic_lab_medianBlurSize;
	wxTextCtrl		*m_ic_edit_medianBlurSize;

	wxButton		*m_btn_OK;
	wxButton		*m_btn_NO;

	wxWindow		*m_frame;

	int				m_i_ic_binaryValue;
	int				m_i_ic_backValue;
	int				m_i_ic_medianBlurSize;
};

class AddrSetDig : public wxDialog
{
	DECLARE_EVENT_TABLE()
public:
	enum
	{
		ID_BTN_OK,
		ID_BTN_NO,
	};
	AddrSetDig(wxWindow* parent, wxWindowID id = -1,
		const wxString& title = "地址参数设置",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION);
	~AddrSetDig();

	void CreateGUIcontrols();
	void OnBtnOK(wxCommandEvent &);
	void OnBtnNO(wxCommandEvent &);

	inline int GetReadAddr()
	{
		return m_i_read_addr;
	};
	inline void SetReadAddr(int addr)
	{
		m_i_read_addr = addr;
	};
	inline wxString GetReadType()
	{
		return m_wxs_read_type;
	};
	inline void SetReadType(wxString type)
	{
		m_wxs_read_type = type;
	};
	inline int GetWriteOKAddr()
	{
		return m_i_write_OK_addr;
	};
	inline void SetWriteOKAddr(int addr)
	{
		m_i_write_OK_addr = addr;
	};
	inline wxString GetWriteOKType()
	{
		return m_wxs_write_OK_type;
	};
	inline void SetWriteOKType(wxString type)
	{
		m_wxs_write_OK_type = type;
	};
	inline int GetWriteNGAddr()
	{
		return m_i_write_NG_addr;
	};
	inline void SetWriteNGAddr(int addr)
	{
		m_i_write_NG_addr = addr;
	};
	inline wxString GetWriteNGType()
	{
		return m_wxs_write_NG_type;
	};
	inline void SetWriteNGType(wxString type)
	{
		m_wxs_write_NG_type = type;
	};
private:
	wxStaticText	*m_lab_read_addr;
	wxTextCtrl		*m_edit_read_addr;
	wxStaticText	*m_lab_write_OK_addr;
	wxTextCtrl		*m_edit_write_OK_addr;
	wxStaticText	*m_lab_write_NG_addr;
	wxTextCtrl		*m_edit_write_NG_addr;
	wxRadioBox		*m_radiobox_read_type;
	wxRadioBox		*m_radiobox_write_OK_type;
	wxRadioBox		*m_radiobox_write_NG_type;

	wxButton		*m_btn_OK;
	wxButton		*m_btn_NO;

	wxWindow		*m_frame;

	int				m_i_read_addr;
	wxString		m_wxs_read_type;
	int				m_i_write_OK_addr;
	wxString		m_wxs_write_OK_type;
	int				m_i_write_NG_addr;
	wxString		m_wxs_write_NG_type;
};

class SetAIDlg :public wxDialog
{
	DECLARE_EVENT_TABLE()
public:
	enum 
	{
		ID_BTN_OK,
		ID_BTN_CANCEL,
	};
	SetAIDlg(wxWindow* parent, wxWindowID id = -1,
		const wxString& title = "AI设置",
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION);
	~SetAIDlg();

	void CreateGUIcontrols();
	void OnBtnOK(wxCommandEvent &);
	void OnBtnCancel(wxCommandEvent &);

	inline void SetAIPath(wxString path)
	{
		m_wxs_ai_path = path;
	};
	inline wxString GetAIPath()
	{
		return m_wxs_ai_path;
	};
	inline void SetIsUseAI(bool flag)
	{
		m_b_is_use_ai = flag;
	};
	inline bool GetIsUseAI()
	{
		return m_b_is_use_ai;
	};
private:
	wxWindow	*m_frame;
	
	wxStaticText	*m_lab_is_use_ai;
	wxCheckBox		*m_check_is_use_ai;
	wxStaticText	*m_lab_ai_path;
	wxTextCtrl		*m_edit_ai_path;

	wxButton		*m_btn_OK;
	wxButton		*m_btn_Cancel;

	bool			m_b_is_use_ai;
	wxString		m_wxs_ai_path;
};

class ModelListCtrl : public wxListCtrl
{
	DECLARE_EVENT_TABLE()
public:
	ModelListCtrl(wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxLC_REPORT | wxLC_EDIT_LABELS);
	virtual ~ModelListCtrl() {};


private:
	wxWindow	*m_frame;
};

class ModelPanel :
	public wxPanel
{
	DECLARE_EVENT_TABLE()

public:
	ModelPanel(wxWindow *parent);
	virtual ~ModelPanel();

	void CreateGui();
	void AddData(list<Image_Properties> list_data);
	void DeleteAllModel();
	inline list<string> GetListRecipe()
	{
		return m_list_recipe_name;
	};
private:
	enum
	{
		ID_LISTCTRL,
	};

private:
	wxWindow	*m_frame;
	ModelListCtrl	*m_modellistctrl;
	wxListItem	m_itemid;

	list<string> m_list_recipe_name;
};

class ModelPanelWindow :public wxDialog
{
	DECLARE_EVENT_TABLE()
public:
	ModelPanelWindow(wxWindow* parent);
	~ModelPanelWindow();

	void DeleteAllModel();
	void OnBtnClear(wxCommandEvent &);
	void OnBtnSave(wxCommandEvent &);
	inline ModelPanel* GetModelPanel()
	{
		return m_modelpanel;
	};
	inline list<string> GetListRecipe()
	{
		return m_modelpanel->GetListRecipe();
	};

	enum 
	{
		ID_BTN_CLEAR = 700,
		ID_BIN_SAVE,
	};
private:
	wxWindow	*m_parent;
	ModelPanel	*m_modelpanel;
	wxButton	*m_btn_clear;
	wxButton	*m_btn_save;
};