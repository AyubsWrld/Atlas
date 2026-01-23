#include "WMIHelper.h"

namespace _com_util 
{
    inline BSTR WINAPI    ConvertStringToBSTR(const char *pSrc){
        int     wcSize;
        BSTR    bstr;

        if(!pSrc)
        {
            return NULL;
        }

        wcSize=::MultiByteToWideChar(CP_ACP,0,pSrc,-1,NULL,0);

        if (wcSize==0) {
            _com_issue_error(HRESULT_FROM_WIN32(GetLastError()));
            return NULL;
        }

        bstr=::SysAllocStringLen(NULL,wcSize-1);

        if(!bstr) {

            _com_issue_error(E_OUTOFMEMORY);
            return NULL;
        }

        if(::MultiByteToWideChar(CP_ACP,0,pSrc,-1,bstr,wcSize)==0) {
            DWORD err = ::GetLastError();
            ::SysFreeString(bstr);
            _com_issue_error(HRESULT_FROM_WIN32(err));
            return NULL;
        }

        return bstr;
    }


    inline char *WINAPI ConvertBSTRToString(BSTR pSrc){

        int         mbSize      {};
        char        *str        {};

        if(!pSrc)
        {
            return NULL;
        }

        mbSize = ::WideCharToMultiByte(CP_ACP,0,pSrc,-1,NULL,0,NULL,NULL);

        if (mbSize==0) 
        {
            _com_issue_error(HRESULT_FROM_WIN32(::GetLastError()));
            return NULL;
        }

        str=new(::std::nothrow) char[mbSize];
        if(!str) 
        {
            _com_issue_error(E_OUTOFMEMORY);
            return NULL;
        }

        if(::WideCharToMultiByte(CP_ACP,0,pSrc,-1,str,mbSize,NULL,NULL)==0) 
        {
            DWORD err = ::GetLastError();
            delete[] str;
            _com_issue_error(HRESULT_FROM_WIN32(err));
            return NULL;
        }

        return str;
    }
}


namespace Atlas 
{
    // TODO: should do this in ctor. 
    bool WMIHelper::Initialize() 
    {
            HRESULT hres;

            /* 
             * Initialize a COM object which facilitates the communication between
             * our process and the Windows Management Instrumentation. 
            */
            
            hres = CoInitializeEx(0, COINIT_MULTITHREADED);
            if (FAILED(hres)) {
                std::cerr << "Failed to initialize COM library. Error code = 0x" 
                        << std::hex << hres << std::endl;
                return false;
            }

            // set general COM security levels
            hres = CoInitializeSecurity(
                NULL,
                -1,                          // com authentication
                NULL,                        // authentication services
                NULL,                        // reserved
                RPC_C_AUTHN_LEVEL_DEFAULT,   // default authentication 
                RPC_C_IMP_LEVEL_IMPERSONATE, // default Impersonation  
                NULL,                        // authentication info
                EOAC_NONE,                   // additional capabilities 
                NULL                         // reserved
            );

            if (FAILED(hres)) {
                std::cerr << "Failed to initialize security. Error code = 0x" 
                        << std::hex << hres << std::endl;
                CoUninitialize();
                return false;
            }

            // obtain the initial Locator to WMI
            hres = CoCreateInstance(
                CLSID_WbemLocator,
                0,
                CLSCTX_INPROC_SERVER,
                IID_IWbemLocator,
                (LPVOID*)&pLoc
            );

            if (FAILED(hres)) {
                std::cerr << "Failed to create IWbemLocator object. Error code = 0x" 
                        << std::hex << hres << std::endl;
                CoUninitialize();
                return false;
            }

            /*
             *  Connection to a WMI namespace on the computer specified in the 
             *  strNetworkResource parameter.
             */
            
            hres = pLoc->ConnectServer(
                _bstr_t(L"ROOT\\STANDARDCIMV2"),    // WMI namespace 
                NULL,                               // user name
                NULL,                               // user password
                0,                                  // locale
                NULL,                               // security flags
                0,                                  // authority
                0,                                  // context object 
                &pSvc                               // iwBEMsERVICES proxy
            );

            if (FAILED(hres)) {
                std::cerr << "Could not connect to WMI namespace. Error code = 0x" 
                        << std::hex << hres << std::endl;
                pLoc->Release();
                CoUninitialize();
                return false;
            }

            hres = CoSetProxyBlanket(
                pSvc,                        // indicates the proxy to set
                RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
                RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
                NULL,                        // server principal name 
                RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
                RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
                NULL,                        // client identity
                EOAC_NONE                    // proxy capabilities 
            );

            if (FAILED(hres)) {
                std::cerr << "Could not set proxy blanket. Error code = 0x" 
                        << std::hex << hres << std::endl;
                pSvc->Release();
                pLoc->Release();
                CoUninitialize();
                return false;
            }

            initialized = true;
            return true;
    }

    bool WMIHelper::EnableWakeOnMagicPacket(const wchar_t* adapterName)
    {
            if (!initialized) {
                std::cerr << "WMI not initialized" << std::endl;
                return false;
            }

            HRESULT hres;

            // build query to find the specific adapter
            wchar_t query[512];
            swprintf_s(query, 512, 
                L"SELECT * FROM MSNdis_DeviceWakeOnMagicPacketOnly WHERE InstanceName LIKE '%%%s%%'",
                adapterName);

            // Execute query
            IEnumWbemClassObject* pEnumerator = NULL;
            hres = pSvc->ExecQuery(
                bstr_t("WQL"),
                bstr_t(query),
                WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                NULL,
                &pEnumerator
            );

            if (FAILED(hres)) {
                std::cerr << "Query for WakeOnMagicPacket failed. Error code = 0x" 
                        << std::hex << hres << std::endl;
                return false;
            }

            IWbemClassObject* pclsObj = NULL;
            ULONG uReturn = 0;

            bool success = false;
            while (pEnumerator) {
                hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
                if (0 == uReturn) {
                    break;
                }

                // set the EnableWakeOnMagicPacket property
                VARIANT vtProp;
                VariantInit(&vtProp);
                vtProp.vt = VT_BOOL;
                vtProp.boolVal = VARIANT_TRUE;

                hres = pclsObj->Put(L"EnableWakeOnMagicPacket", 0, &vtProp, 0);
                VariantClear(&vtProp);

                if (FAILED(hres)) {
                    std::cerr << "Failed to set EnableWakeOnMagicPacket. Error code = 0x" 
                            << std::hex << hres << std::endl;
                    pclsObj->Release();
                    continue;
                }

                // Commit the changes
                hres = pSvc->PutInstance(pclsObj, WBEM_FLAG_UPDATE_ONLY, NULL, NULL);
                
                if (SUCCEEDED(hres)) {
                    std::cout << "Successfully enabled Wake-on-Magic-Packet" << std::endl;
                    success = true;
                } else {
                    std::cerr << "Failed to commit changes. Error code = 0x" 
                            << std::hex << hres << std::endl;
                }

                pclsObj->Release();
            }

            pEnumerator->Release();
            return success;
    }

    bool WMIHelper::ListNetworkAdapters() {
        if (!initialized) {
            std::cerr << "WMI not initialized" << std::endl;
            return false;
        }

        HRESULT hres;

        IWbemServices* pSvcCIMV2 = NULL;
        hres = pLoc->ConnectServer(
            _bstr_t(L"ROOT\\StandardCimv2"),
            NULL, NULL, 0, NULL, 0, 0,
            &pSvcCIMV2
        );

        if (FAILED(hres)) {
            std::cerr << "Failed to connect to ROOT\\CIMV2. Error code = 0x" 
                        << std::hex << hres << std::endl;
            return false;
        }

        // set security on the new connection
        CoSetProxyBlanket(
            pSvcCIMV2,
            RPC_C_AUTHN_WINNT,
            RPC_C_AUTHZ_NONE,
            NULL,
            RPC_C_AUTHN_LEVEL_CALL,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            EOAC_NONE
        );

        // execute query
        IEnumWbemClassObject* pEnumerator = NULL;
        hres = pSvcCIMV2->ExecQuery(
            bstr_t("WQL"),
            bstr_t("SELECT * FROM Win32_NetworkAdapter WHERE NetEnabled=TRUE"),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator
        );

        if (FAILED(hres)) {
            std::cerr << "Query for network adapters failed. Error code = 0x" 
                        << std::hex << hres << std::endl;
            pSvcCIMV2->Release();
            return false;
        }

        // enumerate adapters
        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;

        std::cout << "\n=== Available Network Adapters ===" << std::endl;
        int index = 0;

        while (pEnumerator) {
            hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
            if (0 == uReturn) {
                break;
            }

            VARIANT vtProp;

            // get adapter name
            hres = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hres)) {
                std::wcout << L"[" << index++ << L"] Name: " << vtProp.bstrVal << std::endl;
                VariantClear(&vtProp);
            }

            // get adapter description
            hres = pclsObj->Get(L"Description", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hres)) {
                std::wcout << L"    Description: " << vtProp.bstrVal << std::endl;
                VariantClear(&vtProp);
            }

            // get device ID
            hres = pclsObj->Get(L"DeviceID", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hres)) {
                std::wcout << L"    DeviceID: " << vtProp.bstrVal << std::endl;
                VariantClear(&vtProp);
            }

            // get MAC address
            hres = pclsObj->Get(L"MACAddress", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hres) && vtProp.vt != VT_NULL) {
                std::wcout << L"    MAC: " << vtProp.bstrVal << std::endl;
                VariantClear(&vtProp);
            }

            std::cout << std::endl;
            pclsObj->Release();
        }

        pEnumerator->Release();
        pSvcCIMV2->Release();
        return true;
    }

    bool WMIHelper::GetWakeOnLanStatus(const wchar_t* adapterName) {
        if (!initialized) {
            std::cerr << "WMI not initialized" << std::endl;
            return false;
        }

        HRESULT hres;

        wchar_t query[512];

        swprintf_s(query, 512, 
            L"SELECT * FROM MSFT_NetAdapterPowerManagementSettingData"
            );

        IEnumWbemClassObject* pEnumerator = NULL;
        hres = pSvc->ExecQuery(
            bstr_t("WQL"),
            bstr_t(query),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator
        );

        if (FAILED(hres)) {
            std::cerr << "Query failed. Error code = 0x" << std::hex << hres << std::endl;
            return false;
        }

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;

        bool found = false;
        while (pEnumerator) {

            wprintf(L"Invoking Call\n");
            hres = pEnumerator->Next(
                    WBEM_INFINITE,
                    1,
                    &pclsObj,
                    &uReturn
                    );

            if (0 == uReturn) 
            {
                wprintf(L"Exhausted Enumerators\n");
                break;
            }

            found = true;
            VARIANT vtProp;

            // get instance name
            hres = pclsObj->Get(L"InstanceName", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hres)) {
                std::wcout << L"Instance: " << vtProp.bstrVal << std::endl;
                VariantClear(&vtProp);
            }

            // get WOL enabled 
            hres = pclsObj->Get(L"WakeOnPattern", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hres)) {
                std::cout << "Wake-on-Magic-Packet: " 
                            << (vtProp.bVal ? "ENABLED" : "DISABLED") << std::endl;
                VariantClear(&vtProp);
            }

            pclsObj->Release();
        }

        if (!found) {
            std::cout << "No WOL settings found for this adapter" << std::endl;
        }

        pEnumerator->Release();
        return found;
    }


    void WMIHelper::Cleanup() {
        if (pSvc) {
            pSvc->Release();
            pSvc = NULL;
        }
        if (pLoc) {
            pLoc->Release();
            pLoc = NULL;
        }
        if (initialized) {
            CoUninitialize();
            initialized = false;
        }
    }
}

