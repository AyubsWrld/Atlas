#pragma once
#include "IProcess.h"
#include <processthreadsapi.h>


namespace Atlas::System::Process 
{

#if defined(_WIN32)
    struct FWinProc
    {
        WCHAR*      CreationClassName;
        WCHAR*      Caption;
        WCHAR*      CommandLine;
        FILETIME    CreationDate;
        WCHAR*      CSCreationClassName;
        WCHAR*      CSName;
        WCHAR*      Description;
        WCHAR*      ExecutablePath;
        UINT16      ExecutionState;
        HANDLE      Handle;
        DWORD       HandleCount;
        FILETIME    InstallDate;
        DWORD64     KernelModeTime;
        DWORD       MaximumWorkingSetSize;
        DWORD       MinimumWorkingSetSize;
        WCHAR*      Name;
        WCHAR*      OSCreationClassName;
        WCHAR*      OSName;
        DWORD64     OtherOperationCount;
        DWORD64     OtherTransferCount;
        DWORD       PageFaults;
        DWORD       PageFileUsage;
        DWORD       ParentProcessId;
        DWORD       PeakPageFileUsage;
        DWORD64     PeakVirtualSize;
        DWORD       PeakWorkingSetSize;
        DWORD       Priority;
        DWORD64     PrivatePageCount;
        DWORD       ProcessId;
        DWORD       QuotaNonPagedPoolUsage;
        DWORD       QuotaPagedPoolUsage;
        DWORD       QuotaPeakNonPagedPoolUsage;
        DWORD       QuotaPeakPagedPoolUsage;
        DWORD64     ReadOperationCount;
        DWORD64     ReadTransferCount;
        DWORD       SessionId;
        WCHAR*      Status;
        FILETIME    TerminationDate;
        DWORD       ThreadCount;
        DWORD       ThreadId;
        HANDLE      ThreadHandle;
        DWORD64     UserModeTime;
        DWORD64     VirtualSize;
        WCHAR*      WindowsVersion;
        DWORD64     WorkingSetSize;
        DWORD64     WriteOperationCount;
        DWORD64     WriteTransferCount;

        
        ~FWinProc() = default; 
        FWinProc()  = default;

        FWinProc(const FWinProc& Other)                 =   default; 
        FWinProc& operator=(const FWinProc& Other)      =   default; 

        FWinProc(FWinProc&& Other) noexcept             =   default; 
        FWinProc& operator=(FWinProc&& Other) noexcept  =   default; 

        FWinProc(const PROCESS_INFORMATION& ProcessInformation); 

    };

#endif

#if defined(__linux__)
    struct FLinuxProc {
        std::wstring Name;                   // comm (process name)
        std::int32_t ProcessId;              // pid
        std::int32_t ParentProcessId;        // ppid
        std::int32_t ProcessGroupId;         // pgrp
        std::int32_t SessionId;              // session
        wchar_t State;                       // state (R, S, D, Z, T, etc.)
        std::wstring CommandLine;            // cmdline
        std::wstring ExecutablePath;         // exe (symlink to executable)
        std::time_t StartTime;               // starttime (converted from jiffies)
        
        // Memory statistics (from /proc/[pid]/status and statm)
        std::uint64_t VirtualSize;           // VmSize (virtual memory size in bytes)
        std::uint64_t ResidentSetSize;       // VmRSS (resident set size in bytes)
        std::uint64_t SharedMemory;          // RssFile + RssShmem
        std::uint64_t TextSize;              // VmExe (text/code size)
        std::uint64_t DataSize;              // VmData (data size)
        std::uint64_t StackSize;             // VmStk (stack size)
        
        // CPU statistics (from /proc/[pid]/stat)
        std::uint64_t UserModeTime;          // utime (jiffies in user mode)
        std::uint64_t KernelModeTime;        // stime (jiffies in kernel mode)
        std::int32_t Priority;               // priority
        std::int32_t Nice;                   // nice value
        std::int32_t NumThreads;             // num_threads
        
        // I/O statistics (from /proc/[pid]/io)
        std::uint64_t ReadBytes;             // read_bytes
        std::uint64_t WriteBytes;            // write_bytes
        std::uint64_t ReadSyscalls;          // syscr (read syscalls)
        std::uint64_t WriteSyscalls;         // syscw (write syscalls)
        
        // Additional information
        std::uint32_t RealUid;               // Real user ID
        std::uint32_t EffectiveUid;          // Effective user ID
        std::uint32_t RealGid;               // Real group ID
        std::uint32_t EffectiveGid;          // Effective group ID
        std::uint64_t MinorFaults;           // Minor page faults
        std::uint64_t MajorFaults;           // Major page faults
        std::int32_t Processor;              // CPU number last executed on
        
        // Process environment
        std::wstring Cwd;                    // Current working directory
        std::wstring Root;                   // Root directory
    };

#endif

    STARTUPINFO          CreateStartupInfo();
    PROCESS_INFORMATION  CreateProcessInfo();
    IProcess*            GetProcessByPID(UINT64 PID);
    const TCHAR*         EnumerateProcesses();
    IProcess*            SpawnProcess(LPCSTR ProcessName);
    
    namespace Impl 
    {
#ifdef _WIN32
        IProcess*      GetProcessByPID_Win32(UINT64 PID);
        const TCHAR*   EnumerateProcesses_Win32();
        IProcess*      SpawnProcess_Win32(LPCSTR ProcessName);
#endif
            
#ifdef __linux__
        IProcess*      GetProcessByPID_Linux(UINT64 PID);
        const TCHAR*   EnumerateProcesses_Linux();
        IProcess*      SpawnProcess_Linux(LPCSTR ProcessName);
#endif
    }
}

