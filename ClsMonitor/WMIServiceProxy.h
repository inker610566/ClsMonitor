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
	HRESULT TerminateProcess();

private:
	IWbemLocator *pLoc;
	IUnsecuredApartment *pUnsecApp;
    IUnknown *pStubUnk; 
    IWbemObjectSink *pStubSink;
	IWbemServices *pSvc;
};
