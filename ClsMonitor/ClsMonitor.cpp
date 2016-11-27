// ClsMonitor.cpp : 定義應用程式的進入點。
//

#include "stdafx.h"
#include "ClsMonitor.h"
#include "EventSink.h"
#include "ConsoleLogger.h"
#include "WMIServiceProxy.h"
#include "EventQueue.h"
#include "Blacklist.h"
#include "SockThread.h"
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

#define MAX_LOADSTRING 100

// 全域變數: 
HINSTANCE hInst;                                // 目前執行個體
WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱

Blacklist InitBlacklist(wstring filepath, EventQueue *queue)
{
	// read blacklist
	wifstream stream(filepath);
	wstring line;
	vector<wstring> v;
	while (getline(stream, line))
	   v.push_back(line);
   for (int i = 0; i < v.size(); i++)
	   queue->Push(new QueueEvent(true, v[i]));
   return Blacklist(v);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此置入程式碼。

    // 初始化全域字串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

	// Get Setting

	WMIServiceProxy proxy;
	EventQueue queue;
	wchar_t ww[512];
	ExpandEnvironmentStrings(L"%AppData%\\blacklist.txt", ww, 512);
	Blacklist list = InitBlacklist(ww, &queue);
	EventSink sink(&proxy, &queue, &list);
	proxy.SetCreateProcessCallback(&sink);
	SockThread st("192.168.1.239", 5566, &queue);

	while (true)
	{
		QueueEvent* evt = queue.Pop();
		switch (evt->type)
		{
		case Kill:
			proxy.TerminateProcessesWithName(evt->Name);
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
