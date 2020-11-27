#include "SuperDog.h"
#include <vendor_code.h>

#define   __SuperDogMSG(x) cout << "SuperDogMsg  >> " << x << endl
#define   SuperDogOut  cout << "SuperDogOut  >>" 
#define   SuperDogErrOut  cout << "SuperDogError >>" 
//Data for the demo of the encryption/decryption functionality
unsigned char data_temp[] = 
{
	0x74, 0x65, 0x73, 0x74, 0x20, 0x73, 0x74, 0x72,
	0x69, 0x6e, 0x67, 0x20, 0x31, 0x32, 0x33, 0x34
};
const unsigned int dataLen = sizeof(data_temp);

//The size of the part of the data file which 
//will be printed during the data file demo
const dog_size_t numBytesToShow = 64;

DWORD WINAPI SuperDog::ThreadCheckDog( LPVOID lpData )
{
	SuperDog* self = static_cast<SuperDog*>(lpData);
	bool check_flag =self->CheckDogFunc();
	if (!check_flag)
	{
		SuperDogEvent event1(wxEVT_SUPER_DOG, self->GetParent()->GetId());
		event1.SetType(1);
		self->GetParent()->GetEventHandler()->AddPendingEvent(event1);
		
	}
	return 0;
}

SuperDog::SuperDog(wxWindow *parent,int dog_id)
{
	m_dog_id = dog_id;
	h_check_dog_thread =NULL;
	parent_window =parent;
}

SuperDog::~SuperDog(void)
{
	if (h_check_dog_thread)
	{
		WaitForSingleObject(h_check_dog_thread, 1000);
		CloseHandle(h_check_dog_thread);
	}
}

bool SuperDog::CheckDog()
{
	CDogFeature feature = CDogFeature::fromFeature(m_dog_id);
	const char* defaultScope = "<dogscope />";

/************************Login dog*************************************/
	CDog m_dog =CDog(feature);
	m_status = m_dog.login(vendorCode, defaultScope);

	if (!DOG_SUCCEEDED(m_status))
		return false;
	m_dog.logout();
	return true;
}

const char* SuperDog::GetLastError()
{
	return m_errorPrinter.getError(m_status);
}

bool SuperDog::CheckDogFunc()
{
	while(1)
	{
		Sleep(100);
		CDogFeature feature = CDogFeature::fromFeature(m_dog_id);
		const char* defaultScope = "<dogscope />";

		/************************Login dog*************************************/
//		__SuperDogMSG("Login dog");
		CDog m_dog =CDog(feature);
		m_status = m_dog.login(vendorCode, defaultScope);

		if (!DOG_SUCCEEDED(m_status))
		{
			SuperDogErrOut<<m_errorPrinter.getError(m_status)<<endl;
			return false;
		}

//********************Get Information***************************//
//		__SuperDogMSG("get seesion information");
		std::string info;
		m_status = m_dog.getSessionInfo(CDog::keyInfo(), info);

		if (!DOG_SUCCEEDED(m_status))
		{
			SuperDogErrOut<<m_errorPrinter.getError(m_status)<<endl;
			return false;
		}
//*************************Check the size of data file***********************//  
//		__SuperDogMSG("Check the size of data file");

		dog_size_t size = 0;
		CDogFile defaultFile = m_dog.getFile(CDogFile::fileReadWrite);

		m_status = defaultFile.getFileSize(size);

		if (!DOG_SUCCEEDED(m_status))
		{
			SuperDogErrOut<<m_errorPrinter.getError(m_status)<<endl;
			return false;
		}
//			SuperDogOut<< "check size is "<<size << "bytes"<<endl;
  
/**********************************************************************************/
		if(size)
		{
			//Reads a part of the data file into a buffer
			unsigned int fsize = (size < numBytesToShow) ? size : numBytesToShow;
//			SuperDogOut<<"reading "<<fsize<<" bytes from data file  : ";

			unsigned char *filedata = new unsigned char[fsize];

			m_status = defaultFile.read(filedata, fsize);

			if (!DOG_SUCCEEDED(m_status))
			{
				SuperDogErrOut<<m_errorPrinter.getError(m_status)<<endl;
				delete[] filedata;
				return false;
			}

//			__SuperDogMSG("incrementing every byte in the buffer");

			for (unsigned int i=0; i<fsize; i++) 
				(filedata[i])++;

			//Writes the changed buffer back to SuperDog
//			SuperDogOut<<"writing "<<fsize<<" bytes to data file    : ";
			m_status = defaultFile.write(filedata, fsize);

			if (!DOG_SUCCEEDED(m_status))
			{
				delete[] filedata;
				SuperDogErrOut<<m_errorPrinter.getError(m_status)<<endl;
				return false;
			}

			memset(filedata, 0, fsize);

			//Reads the data file again in order to check the write function
//			SuperDogOut<<"reading "<<fsize<<" bytes from data file  : ";
			m_status = defaultFile.read(filedata, fsize);

			if (!DOG_SUCCEEDED(m_status))
			{
				delete[] filedata;
				SuperDogErrOut<<m_errorPrinter.getError(m_status)<<endl;
				return false;
			}

			delete[] filedata;
		}

		//Encryption demo
//		__SuperDogMSG("encrypting the buffer            : ");
		m_status = m_dog.encrypt(data_temp, dataLen);
		if (!DOG_SUCCEEDED(m_status))
		{	
			SuperDogErrOut<<m_errorPrinter.getError(m_status)<<endl;
			return false;
		}

//		__SuperDogMSG("decrypting the buffer            : ");
		m_status = m_dog.decrypt(data_temp, dataLen);
		if (!DOG_SUCCEEDED(m_status))
		{	
			SuperDogErrOut<<m_errorPrinter.getError(m_status)<<endl;
			return false;
		}
		m_dog.logout();
	}
}

void SuperDog::StartCheckDog()
{
	if (!h_check_dog_thread)
	{
		h_check_dog_thread =::CreateThread(NULL, 0, ThreadCheckDog, (LPVOID)this, 0, NULL);
	}
}

wxWindow* SuperDog::GetParent()
{
	return parent_window;
}
