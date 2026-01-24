#include "Win32Process.h"
#include <iostream>
#include <processthreadsapi.h>
#include <type_traits>
#include <cstdio>

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
        
        /*  Creates a new process and its primary thread. The new process
         *  runs in the security context of the calling process. */
//::LPCSTR                TempProcName    { "C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe" };
        
        ::STARTUPINFOA          sInfo           { CreateStartupInfo() };
        ::PROCESS_INFORMATION   pInfo           { CreateProcessInfo() };
        
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
