#include "stdafx.h"
#include "WMIServiceProxy.h"
#include "ConsoleLogger.h"
#include <sstream>
using namespace std;


WMIServiceProxy::WMIServiceProxy() throw(WMIServiceConnectException)
{
    HRESULT hres;

    // Step 1: --------------------------------------------------
    // Initialize COM. ------------------------------------------

    hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
		stringstream ss;
        ss << "Failed to initialize COM library. Error code = 0x"
             << hex << hres << endl;
		throw WMIServiceConnectException(); // Program has failed.
    }

    // Step 2: --------------------------------------------------
    // Set general COM security levels --------------------------

    hres =  CoInitializeSecurity(
        NULL,
        -1,                          // COM negotiates service
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities
        NULL                         // Reserved
        );


    if (FAILED(hres))
    {
		stringstream ss;
		ss << "Failed to initialize security. Error code = 0x"
			<< hex << hres << endl;
		ConsoleLogger::getInstance()->log(ss.str());
		CoUninitialize();
		throw WMIServiceConnectException(); // Program has failed.
    }

    // Step 3: ---------------------------------------------------
    // Obtain the initial locator to WMI -------------------------

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID *) &pLoc);

    if (FAILED(hres))
    {
		stringstream ss;
        ss << "Failed to create IWbemLocator object. "
             << "Err code = 0x"
             << hex << hres << endl;
		ConsoleLogger::getInstance()->log(ss.str());
        CoUninitialize();
        throw WMIServiceConnectException(); // Program has failed.
    }

    // Step 4: ---------------------------------------------------
    // Connect to WMI through the IWbemLocator::ConnectServer method

    // Connect to the local root\cimv2 namespace
    // and obtain pointer pSvc to make IWbemServices calls.
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &pSvc
    );

    if (FAILED(hres))
    {
		stringstream ss;
        ss << "Could not connect. Error code = 0x"
             << hex << hres << endl;
		ConsoleLogger::getInstance()->log(ss.str());
        pLoc->Release();
        CoUninitialize();
        throw WMIServiceConnectException(); // Program has failed.
    }

    //cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;


    // Step 5: --------------------------------------------------
    // Set security levels on the proxy -------------------------

    hres = CoSetProxyBlanket(
        pSvc,                        // Indicates the proxy to set
        RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
        NULL,                        // Server principal name
        RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
        RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
        NULL,                        // client identity
        EOAC_NONE                    // proxy capabilities
    );

    if (FAILED(hres))
    {
		stringstream ss;
        ss << "Could not set proxy blanket. Error code = 0x"
             << hex << hres << endl;
		ConsoleLogger::getInstance()->log(ss.str());
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        throw WMIServiceConnectException(); // Program has failed.
    }

	// Prepare TerminateProcess
	BSTR className = SysAllocString(L"Win32_Process");
	IWbemClassObject *Win32ProcessClass;
	IWbemClassObject *TerminateMethod, *pInParamsDefination, *pOutMethod;
    BSTR ReasonString = SysAllocString(L"Reason");
	hres = pSvc->GetObjectW(className, 0, NULL, &Win32ProcessClass, NULL);
	if (FAILED(hres))
	{
		ConsoleLogger::getInstance()->log("Cannot get Win32_Process Class!!");
		throw WMIServiceConnectException();
	}

	hres = Win32ProcessClass->GetMethod(MethodName, 0,
		&pInParamsDefination, &pOutMethod);
	if (FAILED(hres))
	{
		ConsoleLogger::getInstance()->log("Cannot getMethod Terminate!!");
		throw WMIServiceConnectException();
	}

	hres = pInParamsDefination->SpawnInstance(0, &Win32ProcessClassInstance);
	if (FAILED(hres))
	{
		ConsoleLogger::getInstance()->log("Cannot Spawn Win32Process Instance!!");
		throw WMIServiceConnectException();
	}

	VARIANT pcVal;
	VariantInit(&pcVal);
	V_VT(&pcVal) = VT_I4;

	hres = Win32ProcessClassInstance->Put(L"Reason", 0, &pcVal, 0);
	if (FAILED(hres))
	{
		ConsoleLogger::getInstance()->log("Cannot put reason parameter");
		throw WMIServiceConnectException();
	}
}


WMIServiceProxy::~WMIServiceProxy()
{
    HRESULT hres;
    hres = pSvc->CancelAsyncCall(pStubSink);
    pSvc->Release();
    pLoc->Release();
    pUnsecApp->Release();
    pStubUnk->Release();
    pStubSink->Release();
    CoUninitialize();
}

HRESULT WMIServiceProxy::SetCreateProcessCallback(IWbemObjectSink * pSink)
{
	HRESULT hres;
    // Step 6: -------------------------------------------------
    // Receive event notifications -----------------------------

    // Use an unsecured apartment for security

    hres = CoCreateInstance(CLSID_UnsecuredApartment, NULL,
        CLSCTX_LOCAL_SERVER, IID_IUnsecuredApartment,
        (void**)&pUnsecApp);

    pUnsecApp->CreateObjectStub(pSink, &pStubUnk);

    pStubUnk->QueryInterface(IID_IWbemObjectSink,
        (void **) &pStubSink);

    pSink->AddRef();

    // The ExecNotificationQueryAsync method will call
    // The EventQuery::Indicate method when an event occurs
    hres = pSvc->ExecNotificationQueryAsync(
        _bstr_t("WQL"),
        _bstr_t("SELECT * "
            "FROM __InstanceCreationEvent WITHIN 1 "
            "WHERE TargetInstance ISA 'Win32_Process'"),
        WBEM_FLAG_SEND_STATUS,
        NULL,
        pStubSink);

    // Check for errors.
    if (FAILED(hres))
    {
		stringstream ss;
		ss << "ExecNotificationQueryAsync failed "
			<< "with = 0x" << hex << hres << "\n";
		ConsoleLogger::getInstance()->log(ss.str());
        pSvc->Release();
        pLoc->Release();
        pUnsecApp->Release();
        pStubUnk->Release();
        pSink->Release();
        pStubSink->Release();
        CoUninitialize();
    }
	return hres;
}

HRESULT WMIServiceProxy::TerminateProcess(const BSTR ClassNameInstance)
{
	HRESULT hres;
	hres = pSvc->ExecMethod(ClassNameInstance, MethodName, 0,
		NULL, Win32ProcessClassInstance, NULL, NULL);
	return hres;
}
