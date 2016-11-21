#pragma once

#define _WIN32_DCOM
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>
#include "WMIServiceConnectException.h"

#pragma comment(lib, "wbemuuid.lib")

class WMIServiceProxy
{
public:
	WMIServiceProxy() throw (WMIServiceConnectException);
	~WMIServiceProxy();

	HRESULT SetCreateProcessCallback(IWbemObjectSink *sink);
	HRESULT TerminateProcess(const BSTR ClassNameInstance);

private:
	IWbemLocator *pLoc;
	IUnsecuredApartment *pUnsecApp;
    IUnknown *pStubUnk; 
    IWbemObjectSink *pStubSink;
	IWbemServices *pSvc;

	// TerminateProcess
	IWbemClassObject *Win32ProcessClassInstance;
	_bstr_t MethodName= (L"Terminate");
};

