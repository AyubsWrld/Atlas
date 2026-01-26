#include "ProcCommon.h"

namespace Atlas 
{
    FProcessDetails GetProcessDetails()
    {
        return  FProcessDetails{};
    }
}

namespace Atlas::System::Process 
{
    STARTUPINFO CreateStartupInfo()
    {
        STARTUPINFOA StartupInformation; 
        ::ZeroMemory(&StartupInformation, sizeof(STARTUPINFOA));
        StartupInformation.cb = sizeof(STARTUPINFOA);
        return StartupInformation; 
    }

    PROCESS_INFORMATION CreateProcessInfo()
    {
        PROCESS_INFORMATION  ProcessInformation; 
        ::ZeroMemory(&ProcessInformation, sizeof(PROCESS_INFORMATION));
        return ProcessInformation; 
    }
    IProcess* GetProcessByPID(UINT64 PID)
    {
        #if defined(_WIN32)
            return Impl::GetProcessByPID_Win32(PID);
        #elif defined(__linux__)
            return Impl::GetProcessByPID_Linux(PID);
        #else
            #error "Unsupported platform"
            return {};
        #endif 
    }

    const TCHAR* EnumerateProcesses()
    {
        #if defined(_WIN32)
            return Impl::EnumerateProcesses_Win32();
        #elif defined(__linux__)
            return Impl::EnumerateProcesses_Linux();
        #else
            #error "Unsupported platform"
            return {};
        #endif 
    }

    std::shared_ptr<IProcess>   SpawnProcess(LPCSTR ProcessName)
    {
    #if defined(_WIN32)
        return Impl::SpawnProcess_Win32(ProcessName);
    #elif defined(__linux__)
        return Impl::SpawnProcess_Linux(ProcessName);
    #else
        #error "Unsupported platform"
        return {};
    #endif 
    }
}


namespace Atlas::System::Process 
{
    /* 
     *  This gives us the minimal information to derive the rest of the
     *  attributes.
     */
    
    FWinProc::FWinProc(const PROCESS_INFORMATION& ProcessInformation)
        :   Handle(ProcessInformation.hProcess),
            ProcessId(ProcessInformation.dwProcessId),
            ThreadHandle(ProcessInformation.hThread),
            ThreadId(ProcessInformation.dwThreadId)
    {
        /* Derive rest of the info here using ^ */ 
  
        ::wprintf(L"PID: %d\n TID: %d\n", ProcessInformation.dwProcessId, ProcessInformation.hThread);
    }
}
