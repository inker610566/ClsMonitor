// ClsMonitor.cpp : �w�q���ε{�����i�J�I�C
//

#include "stdafx.h"
#include "ClsMonitor.h"
#include "WMI\EventSink.h"
#include "Utils\ConsoleLogger.h"
#include "WMI\WMIServiceProxy.h"
#include "Process\EventQueue.h"
#include "Process\Blacklist.h"
#include "Network\SockThread.h"
#include "Process\KillProcessScheduler.h"
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

using Process::KillProcessScheduler;
using namespace Network;

#define MAX_LOADSTRING 100

// �����ܼ�: 
HINSTANCE hInst;                                // �ثe�������
WCHAR szTitle[MAX_LOADSTRING];                  // ���D�C��r
WCHAR szWindowClass[MAX_LOADSTRING];            // �D�������O�W��

Blacklist *InitBlacklist(wstring filepath)
{
	// read blacklist
	wifstream stream(filepath);
	wstring line;
	vector<wstring> v;
	while (getline(stream, line))
	   v.push_back(line);
	Blacklist *ret = new Blacklist(v);
    return ret;
}

void BlacklistNTReconnect(void *params)
{
	KillProcessScheduler *blist = (KillProcessScheduler*) params;
	blist->Reset();
}

void BlacklistNTDisconnect(void *params)
{
	KillProcessScheduler *p = (KillProcessScheduler*) params;
	p->Reset();
}

void NTFrame(void *params, Frame *f)
{
	KillProcessScheduler *p = (KillProcessScheduler*) params;
	switch (f->type)
	{
	case 0:
		p->Del(f->param);
		break;
	case 1:
		p->Add(f->param);
		break;
	case 2:
		p->LockScreen();
		break;
	}
	delete f;
}

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
	wchar_t ww[512];
	ExpandEnvironmentStrings(L"%AppData%\\blacklist.txt", ww, 512);
	Blacklist *list = InitBlacklist(ww);
	KillProcessScheduler scheduler(list, &proxy);
	scheduler.Reset(); // kill all process in default list at start

	SockThread st("192.168.1.239", 5566);
	st.SetConnectCallback({BlacklistNTReconnect, &scheduler});
	st.SetDisconnectCallback({ BlacklistNTDisconnect, &scheduler });
	st.SetFrameCallback({NTFrame, &scheduler});
	st.Start();
	EventSink sink(&scheduler, list);
	proxy.SetCreateProcessCallback(&sink);

	scheduler.ExecuteLoop();

	return 0;
}
