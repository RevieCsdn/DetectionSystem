#pragma once

#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <wx/wx.h>

#include <dog_api_cpp.h>
#include "errorprinter.h"
#include "../UserInterface/UIEvent.h"

using namespace std;
class SuperDog
{
public:
	SuperDog(wxWindow *parent, int dog_id);
	~SuperDog(void);

   bool			CheckDog();
//	bool    LoginDog();    

	void			displayTime(const CDogTime& dogTime);
	void			displayFileData(unsigned char *mdata, unsigned int mdataLen);
	void			displayCloseMessage();

	const char*     GetLastError();

	bool            CheckDogFunc();
	void            StartCheckDog();
  wxWindow*       GetParent();
	static DWORD WINAPI ThreadCheckDog(LPVOID lpData);
private:

	ErrorPrinter	m_errorPrinter;
	dogStatus		m_status;

	dog_u32_t       m_dog_id;

	HANDLE          h_check_dog_thread; 
    
  wxWindow        *parent_window;
};

