// ClsMonitor.cpp : �w�q���ε{�����i�J�I�C
//

#include "stdafx.h"
#include "ClsMonitor.h"
#include "EventSink.h"
#include "ConsoleLogger.h"

#define MAX_LOADSTRING 100

// �����ܼ�: 
HINSTANCE hInst;                                // �ثe�������
WCHAR szTitle[MAX_LOADSTRING];                  // ���D�C��r
WCHAR szWindowClass[MAX_LOADSTRING];            // �D�������O�W��

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �b���m�J�{���X�C

    // ��l�ƥ���r��
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

	/*
	// Get Setting
	HRESULT hres;

	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM negotiates service
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
	);

	if (FAILED(hres))
	{
		cout << "Failed to initialize security. Error code = 0x"
			<< hex << hres << endl;
		CoUninitialize();
		return 1;                      // Program has failed.
	}
	*/
    
	while (true)
	{
		Sleep(1000);
		ConsoleLogger::getInstance()->log("Hell O Error");
	}
	
	return 0;
}
