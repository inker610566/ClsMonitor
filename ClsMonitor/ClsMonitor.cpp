// ClsMonitor.cpp : 定義應用程式的進入點。
//

#include "stdafx.h"
#include "ClsMonitor.h"
#include "EventSink.h"
#include "ConsoleLogger.h"
#include "WMIServiceProxy.h"

#define MAX_LOADSTRING 100

// 全域變數: 
HINSTANCE hInst;                                // 目前執行個體
WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱

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

	WMIServiceProxy *proxy = new WMIServiceProxy();
	EventSink *sink = new EventSink(proxy);
	proxy->SetCreateProcessCallback(sink);

	Sleep(1000000);

	return 0;
}
