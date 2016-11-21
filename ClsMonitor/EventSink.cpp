// EventSink.cpp
#include "stdafx.h"
#include "eventsink.h"
#include "ConsoleLogger.h"

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

HRESULT EventSink::Indicate(long lObjectCount,
    IWbemClassObject **apObjArray)
{
	HRESULT hr;

	BSTR pstrName;
	VARIANT pval;
	CIMTYPE pvtType;
	char buf[10];
	_variant_t cn, vtProp;

	// http://stackoverflow.com/a/28946830/1548625
    for (int i = 0; i < lObjectCount; i++)
    {
		IWbemClassObject *o = apObjArray[i];

		hr = o->Get(_bstr_t(L"__Class"), 0, &cn, 0, 0);

		if (!SUCCEEDED(hr))
		{
			ConsoleLogger::getInstance()->log("Cannot handle Event!!");
			continue;
		}
		wstring LClassStr(cn.bstrVal);
		if (0 != LClassStr.compare(L"__InstanceCreationEvent"))
		{
			// should be '__InstanceDeletionEvent' or others
			continue;
		}
		VariantClear(&cn);

		hr = apObjArray[i]->Get(_bstr_t(L"TargetInstance"), 0, &vtProp, 0, 0);
		if (FAILED(hr))
		{
			ConsoleLogger::getInstance()->log("Cannot get TargetInstance!!");
			continue;
		}

		IUnknown* str = vtProp;
		hr = str->QueryInterface( IID_IWbemClassObject, reinterpret_cast< void** >( &o ) );
		if (!SUCCEEDED(hr))
		{
			ConsoleLogger::getInstance()->log("Cannot use IID_IWbemClassObject interface!!");
			continue;
		}

		//hr = o->Get(L"Name", 0, &cn)

		ConsoleLogger::getInstance()->log("=======");

		o->BeginEnumeration(0);

		while (WBEM_S_NO_MORE_DATA != (hr = o->Next(0, &pstrName, &pval, &pvtType, NULL)))
		{
			if(hr != WBEM_S_NO_ERROR)
				ConsoleLogger::getInstance()->log("ERROR");
			ConsoleLogger::getInstance()->log(BstrToStdString(pstrName));
			if (pvtType == 8)
			{
				if ((cn.vt==VT_NULL))
					ConsoleLogger::getInstance()->log("8 NULL");
				else if((cn.vt==VT_EMPTY))
					ConsoleLogger::getInstance()->log("8 EMPTY");
				else
					ConsoleLogger::getInstance()->log("8 "+BstrToStdString(pval.bstrVal));
			}
			else
			{
				_ltoa_s(pvtType, buf, 10, 10);
				ConsoleLogger::getInstance()->log(buf);
			}
			SysFreeString(pstrName);
			VariantClear(&pval);
		}
		o->EndEnumeration();
		hr = o->Get( L"Name", 0, &cn, NULL, NULL );
		if ( SUCCEEDED( hr ) )
		{
			ConsoleLogger::getInstance()->log("Get Name:");
			if ((cn.vt==VT_NULL))
				ConsoleLogger::getInstance()->log("8 NULL");
			else if((cn.vt==VT_EMPTY))
				ConsoleLogger::getInstance()->log("8 EMPTY");
			else
				ConsoleLogger::getInstance()->log("8 "+BstrToStdString(pval.bstrVal));
		}
		VariantClear(&cn);    
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
