#include "Scope.h"
#include "Win32Process.h"
#include <accctrl.h>
#include <cstdlib>
#include <iostream>
#include <minwinbase.h>
#include <processthreadsapi.h>
#include <securitybaseapi.h>
#include <type_traits>
#include <cstdio>
#include <winerror.h>
#include <winnt.h>

#define TOWCHAR(X) \
    static_assert(std::is_same_v<char*, std:remove_cv<decltype(X)>>)

namespace Atlas::System::Process::Impl
{
    IProcess* GetProcessByPID_Win32(UINT64 PID)
    {
        return nullptr;
    };

    const TCHAR* EnumerateProcesses_Win32()
    {
        DWORD   aProcesses[1024];
        DWORD   cbNeeded;  
        DWORD   cProcesses;

        unsigned int i;
    
        if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        {
            return {};
        }

        cProcesses = cbNeeded / sizeof(DWORD);

        for ( i = 0; i < cProcesses; i++ )
        {
            if( aProcesses[i] != 0 )
            {
                wprintf(L"%lu\n", aProcesses[i]);
            }
        }

        return {};
    };

    // This routine is for testing security functions and 
    // will be deleted for one that returns a more stringent version. 
    
    DWORD _GetLenientSecurityDescriptor(HANDLE Handle)
    {
        if (true) return {}; // Remove call for now

        DWORD   dwResult                {}; 
        DWORD   dwDisposition           {}; 
        PSID    pEveryoneSID            {NULL};
        PACL    pAccessControlList      {NULL};
        PSECURITY_DESCRIPTOR pSecDesc   {NULL};
        EXPLICIT_ACCESS ExpicitAccess   {};

        SID_IDENTIFIER_AUTHORITY SIDAuthWorld   {SECURITY_WORLD_SID_AUTHORITY};
        SECURITY_ATTRIBUTES SecurityAttributes;

        LONG lRes;
        HKEY hkSub = NULL;

        // TODO: Implement https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0052r10.pdf

        /* 
            Allocates and initializez a Security Identifier for 
            EVERYONE. Give read access as well.  
TODO:       Make this more granular.
         */

        if (!::AllocateAndInitializeSid(
                    &SIDAuthWorld,
                    1,
                    SECURITY_WORLD_RID,
                    0, 0, 0, 0, 0, 0, 0,
                    &pEveryoneSID))
        {
            ::_tprintf(_T("AllocateAndInitializeSid Error 0x%x\n"), GetLastError());
            return !ERROR_SUCCESS; // TODO: use different sentinel value. 
        }

        ::ZeroMemory(&ExpicitAccess, sizeof(EXPLICIT_ACCESS));

        ExpicitAccess.grfAccessPermissions = KEY_WRITE  ; // 
        ExpicitAccess.grfAccessMode = SET_ACCESS        ;
        ExpicitAccess.grfInheritance= NO_INHERITANCE;

        ExpicitAccess.Trustee.TrusteeForm = TRUSTEE_IS_SID;
        ExpicitAccess.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
        ExpicitAccess.Trustee.ptstrName  = (LPTSTR) pEveryoneSID;

        pSecDesc = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, 
                                SECURITY_DESCRIPTOR_MIN_LENGTH); 

        if (pSecDesc == NULL)
        {
            ::_tprintf(_T("AllocateAndInitializeSid Error 0x%x\n"), GetLastError());
            return !ERROR_SUCCESS; // TODO: use different sentinel value. 
        }

        if (!::SetSecurityDescriptorDacl(
                    pSecDesc,
                    TRUE,
                    pAccessControlList,
                    FALSE
                    ))
        {
            _tprintf(_T("SetSecurityDescriptorDacl Error %u\n"),
            GetLastError());
            return !ERROR_SUCCESS;
        }

        SecurityAttributes.nLength = sizeof (SECURITY_ATTRIBUTES);
        SecurityAttributes.lpSecurityDescriptor = pSecDesc;
        SecurityAttributes.bInheritHandle = FALSE;
    
        lRes = RegCreateKeyEx(HKEY_CURRENT_USER, _T("mykey"), 0, _T(""), 0, 
                KEY_READ | KEY_WRITE, &SecurityAttributes, &hkSub, &dwDisposition); 
        _tprintf(_T("RegCreateKeyEx result %u\n"), lRes );
        return ERROR_SUCCESS;
    }

    std::shared_ptr<IProcess> SpawnProcess_Win32(LPCSTR ProcessName)
    {
        if (!ProcessName || !::PathFileExistsA(ProcessName)) 
        {
            printf("Invalid Path to module\n");
            return {}; 
        }
    
        DWORD FileAttributes = GetFileAttributesA(ProcessName);
    
        if (FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            printf("Invalid Path to module\n");
            return {}; 
        }
    
        STARTUPINFOA sInfo = {};
        PROCESS_INFORMATION pInfo = {};
        
        ZeroMemory(&sInfo, sizeof(sInfo));
        sInfo.cb = sizeof(sInfo);
        ZeroMemory(&pInfo, sizeof(pInfo));
    
        if (!::CreateProcessA(
                (LPSTR)ProcessName,         // cast mutable for CreateProcessA
                NULL,                       // use lpCommandLine instead
                NULL,                       // process security attributes (default)
                NULL,                       // thread security attributes (default)
                FALSE,                      // inherit handles
                NORMAL_PRIORITY_CLASS,
                NULL,                       // environment
                NULL,                       // current directory
                &sInfo,
                &pInfo))
        {
            printf("Failed to create process 0x%x\n", GetLastError());
            return {};
        }
    
        if (pInfo.hThread)
        {
            CloseHandle(pInfo.hThread);
        }
    
        FWinProc ProcessAttributes(pInfo); 
        
        return std::make_shared<Win32Process>(ProcessAttributes);
    }

    FWinProc GetProcessAttributes(HANDLE ProcessHandle)
    {
        if (ProcessHandle == INVALID_HANDLE_VALUE)
        {
            return {};
        }
        return{}; 
    }
}
