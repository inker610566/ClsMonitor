// ClsMonitor.cpp : �w�q���ε{�����i�J�I�C
//

#include "stdafx.h"
#include "ClsMonitor.h"
#include "EventSink.h"
#include "ConsoleLogger.h"
#include "WMIServiceProxy.h"
#include "EventQueue.h"
#include "Blacklist.h"

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

	// Get Setting

	WMIServiceProxy proxy;
	EventQueue queue;
	Blacklist list({L"notepad.exe"}); // put default list here
	EventSink sink(&proxy, &queue, &list);
	proxy.SetCreateProcessCallback(&sink);
	//proxy.TerminateProcessesWithName(L"notepad.exe");

	while (true)
	{
		QueueEvent* evt = queue.Pop();
		switch (evt->type)
		{
		case Kill:
			proxy.TerminateProcess(evt->RelPath.bstrVal);
			break;
		case Add:
			list.Add(evt->Name);
			proxy.TerminateProcessesWithName(evt->Name);
			break;
		case Del:
			list.Del(evt->Name);
			break;
		}
		delete evt;
	}

	return 0;
}
