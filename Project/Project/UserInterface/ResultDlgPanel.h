#pragma once

#include <wx/wx.h>
#include "../Util/Profile.h"

class ResultDlgPanel :
	public wxPanel
{
	DECLARE_EVENT_TABLE()
public:
	ResultDlgPanel(wxWindow* parent, Profile* file);
	virtual ~ResultDlgPanel();

	void	SetEditOK(int num);
	void	SetEditNG(int num);
	void	SetEditRatio(float num);
private:
	wxWindow *m_frame;
	
	wxStaticText	*m_lab_ng;
	wxTextCtrl		*m_edit_ng;
	wxStaticText	*m_lab_ok;
	wxTextCtrl		*m_edit_ok;
	wxStaticText	*m_lab_ratio;
	wxTextCtrl		*m_edit_ratio;

	Profile* m_proFile;
	wxString m_path;

};

