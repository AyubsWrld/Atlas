#include "Win32Process.h"
#include <accctrl.h>
#include <cstdlib>
#include <iostream>
#include <minwinbase.h>
#include <processthreadsapi.h>
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

    /* 
     * Implications of using wchar_t as opposed to char
     */
  
    DWORD _SetSecurityDescriptor(HANDLE Handle)
    {
        DWORD   dwResult                {}; 
        DWORD   dwDisposition           {}; 
        PSID    pEveryoneSID            {NULL};
        PACL    pAccessControlList      {NULL};
        PSECURITY_DESCRIPTOR pSecDesc   {NULL};
        EXPLICIT_ACCESS ExpicitAccess   {};

        SID_IDENTIFIER_AUTHORITY SIDAuthWorld   {SECURITY_WORLD_SID_AUTHORITY};
        SECURITY_ATTRIBUTES sa;

        LONG lRes;
        HKEY hkSub = NULL;

        auto Guard = std::unique_ptr<DWORD, std::function<void(DWORD*)>>{&dwResult, [&](DWORD*){ delete s; } };

        /* Capture within context of the struct */ 

        // TODO: Implement https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0052r10.pdf
        // struct 
        // {
        //     void(*Callable)(){nullptr};
        //     ~(){ if(Callable){ Callable(); }}
        // } _deferred { /* Some lambda that captures & performs cleanup */ }; 

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
            // Cleanup 
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

        if (NULL == pSecDesc)
        {
            return !ERROR_SUCCESS; // TODO: use different sentinel value. 
            // Cleanup 
        }

        return ERROR_SUCCESS;
    }

    std::shared_ptr<IProcess>   SpawnProcess_Win32(LPCSTR ProcessName)
    {
        if (!ProcessName || !::PathFileExistsA(ProcessName)) 
        {
            printf("Invalid Path to module\n");
            return {}; 
        }

        DWORD FileAttributes{ GetFileAttributesA(ProcessName) };

        if ( FileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            printf("Invalid Path to module\n");
            return {}; 
        }
        
        // check whether what was passed in was an exectuble or not.
        
        STARTUPINFOA          sInfo           { CreateStartupInfo() };
        PROCESS_INFORMATION   pInfo           { CreateProcessInfo() };
        EXPLICIT_ACCESS       eExplAccess     {};
        SECURITY_DESCRIPTOR   sSecDesc        {};
        
        if (!::CreateProcessA(
                NULL,                               // use lpCommandLine instead
                const_cast<LPSTR>(ProcessName),     // cast mutable for CreateProcessA
                NULL,                               // process security attributes
                NULL,                               // thread security attributes
                FALSE,                              // inherit handles
                NORMAL_PRIORITY_CLASS,
                NULL,                               // environment
                NULL,                               // current directory
                &sInfo,
                &pInfo))
        {
            wprintf(L"Failed to create process 0x%x\n", GetLastError());
            return {};
        }


        // Locally Populate the FWinProc Information 
        // TODO: Move this elsewhere. 
        // inside of GetProcessAttributes. 
      
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
