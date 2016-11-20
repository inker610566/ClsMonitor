#pragma once

#define _WIN32_DCOM
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>
#include "CallbackRegisterException.h"

#pragma comment(lib, "wbemuuid.lib")

class CreateProcessCallbackRegister
{
public:
	CreateProcessCallbackRegister(IWbemObjectSink *sink) throw (CallbackRegisterException);

	void Release();
	~CreateProcessCallbackRegister();
private:
	IWbemLocator *pLoc;
	IWbemServices *pSvc;
	IUnsecuredApartment *pUnsecApp;
    IUnknown *pStubUnk; 
    IWbemObjectSink *pStubSink;
	IWbemObjectSink *pSink;
};

