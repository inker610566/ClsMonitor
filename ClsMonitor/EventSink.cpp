// EventSink.cpp
#include "stdafx.h"
#include "EventSink.h"
#include "ConsoleLogger.h"
#include <sstream>
using namespace std;

EventSink::EventSink(WMIServiceProxy *proxy) :m_lRef(0)
{
	m_proxy = proxy;
}

ULONG EventSink::AddRef()
{
    return InterlockedIncrement(&m_lRef);
}

ULONG EventSink::Release()
{
    LONG lRef = InterlockedDecrement(&m_lRef);
    if(lRef == 0)
        delete this;
    return lRef;
}

HRESULT EventSink::QueryInterface(REFIID riid, void** ppv)
{
    if (riid == IID_IUnknown || riid == IID_IWbemObjectSink)
    {
        *ppv = (IWbemObjectSink *) this;
        AddRef();
        return WBEM_S_NO_ERROR;
    }
    else return E_NOINTERFACE;
}

std::string& BstrToStdString(const BSTR bstr, std::string& dst, int cp = CP_UTF8)
{
    if (!bstr)
    {
        // define NULL functionality. I just clear the target.
        dst.clear();
        return dst;
    }

    // request content length in single-chars through a terminating
    //  nullchar in the BSTR. note: BSTR's support imbedded nullchars,
    //  so this will only convert through the first nullchar.
    int res = WideCharToMultiByte(cp, 0, bstr, -1, NULL, 0, NULL, NULL);
    if (res > 0)
    {
        dst.resize(res);
        WideCharToMultiByte(cp, 0, bstr, -1, &dst[0], res, NULL, NULL);
    }
    else
    {    // no content. clear target
        dst.clear();
    }
    return dst;
}

// conversion with temp.
std::string BstrToStdString(BSTR bstr, int cp = CP_UTF8)
{
    std::string str;
    BstrToStdString(bstr, str, cp);
    return str;
}

void Terminate()
{
}

void EnumP(IWbemClassObject *o)
{
	HRESULT hr;
	BSTR pstrName;
	VARIANT pval;
	CIMTYPE pvtType;
	char buf[10];
	o->BeginEnumeration(0);
	while (WBEM_S_NO_MORE_DATA != (hr = o->Next(0, &pstrName, &pval, &pvtType, NULL)))
	{
		if (hr != WBEM_S_NO_ERROR)
		{
			ConsoleLogger::getInstance()->log("ERROR");
			continue;
		}
		ConsoleLogger::getInstance()->log(BstrToStdString(pstrName));
		if (pvtType == 8)
		{
			if ((pval.vt == VT_NULL))
				ConsoleLogger::getInstance()->log("8 NULL");
			else if ((pval.vt == VT_EMPTY))
				ConsoleLogger::getInstance()->log("8 EMPTY");
			else
				ConsoleLogger::getInstance()->log("8 " + BstrToStdString(pval.bstrVal));
		}
		else
		{
			_ltoa_s(pvtType, buf, 10, 10);
			ConsoleLogger::getInstance()->log(buf);
		}
		SysFreeString(pstrName);
		VariantClear(&pval);
	}
}

HRESULT EventSink::Indicate(long lObjectCount,
    IWbemClassObject **apObjArray)
{
	HRESULT hres;
	for (int i = 0; i < lObjectCount; i++)
	{
		VARIANT vVar;
		_variant_t         vTarget;
		IUnknown          *pIUnknown;
		IWbemClassObject  *pinstPkgStatus = NULL;
		_variant_t         vName;
		_variant_t         vExecutablePath;
		_variant_t         vCommandLine;

		hres = (apObjArray[0])->Get(L"TargetInstance", 0, &vTarget, NULL, NULL);
		if (FAILED(hres))
		{
			ConsoleLogger::getInstance()->log("Cannot get TargetInstance!!");
			continue;
		}

		pIUnknown = (IUnknown *)vTarget;
		hres = pIUnknown->QueryInterface(IID_IWbemClassObject, (void **)&pinstPkgStatus);
		if (FAILED(hres))
		{
			ConsoleLogger::getInstance()->log("Failed to get IID_IWbemClassObject in TargetInstance!!");
			continue;
		}
		pIUnknown->Release();

		EnumP(pinstPkgStatus);
		/*
		hres = pinstPkgStatus->Get(L"Name", 0, &vName, NULL, NULL);
		Print(hres, vName, L"Name");

		hres = pinstPkgStatus->Get(L"ExecutablePath", 0, &vExecutablePath, NULL, NULL);
		Print(hres, vExecutablePath, L"ExecutablePath");

		hres = pinstPkgStatus->Get(L"CommandLine", 0, &vCommandLine, NULL, NULL);
		Print(hres, vCommandLine, L"CommandLine");

		pinstPkgStatus->Release();
		pinstPkgStatus = NULL;

		cout << endl << endl;
		*/
	}
    return WBEM_S_NO_ERROR;
}

HRESULT EventSink::SetStatus(
            /* [in] */ LONG lFlags,
            /* [in] */ HRESULT hResult,
            /* [in] */ BSTR strParam,
            /* [in] */ IWbemClassObject __RPC_FAR *pObjParam
        )
{
    if(lFlags == WBEM_STATUS_COMPLETE)
    {
        printf("Call complete. hResult = 0x%X\n", hResult);
    }
    else if(lFlags == WBEM_STATUS_PROGRESS)
    {
        printf("Call in progress.\n");
    }

    return WBEM_S_NO_ERROR;
}    // end of EventSink.cpp
