#ifndef EVENTSINK_H
#define EVENTSINK_H

#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>
#include "../Process/Blacklist.h"
#include "../Process/KillProcessScheduler.h"

#pragma comment(lib, "wbemuuid.lib")

class EventSink : public IWbemObjectSink
{
	LONG m_lRef;
	bool bDone;
	WMIServiceProxy *m_proxy;
	Process::KillProcessScheduler *sch;
	Blacklist *list;

public:
	EventSink(Process::KillProcessScheduler *sch, Blacklist *list);
	~EventSink() { bDone = true; }

	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	virtual HRESULT
		STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);

	virtual HRESULT STDMETHODCALLTYPE Indicate(
		LONG lObjectCount,
		IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray
	);

	virtual HRESULT STDMETHODCALLTYPE SetStatus(
		/* [in] */ LONG lFlags,
		/* [in] */ HRESULT hResult,
		/* [in] */ BSTR strParam,
		/* [in] */ IWbemClassObject __RPC_FAR *pObjParam
	);
};

#endif    // end of EventSink.h
