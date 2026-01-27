#include "Win32Process.h"
#include <accctrl.h>
#include <errhandlingapi.h>
#include <handleapi.h>

namespace Atlas::System::Process 
{
    WCHAR* Win32Process::CreationClassName() const { return IProcess::CreationClassName() ; }
    
    std::time_t Win32Process::CreationDate() const  { return IProcess::CreationDate() ; }
    
    WCHAR* Win32Process::CSCreationClassName() const { return IProcess::CSCreationClassName(); }

    WCHAR* Win32Process::CSName() const { return IProcess::CSName() ; }
    
    EExectuionState Win32Process::ExecutionState() const { return IProcess::ExecutionState(); }
    
    WCHAR* Win32Process::Handle() const { return IProcess::Handle(); }
    
    std::uint64_t Win32Process::KernelModeTime() const { return IProcess::KernelModeTime(); }
    
    WCHAR* Win32Process::Name() const { return IProcess::Name() ; }
    
    WCHAR* Win32Process::OSCreationClassName() const { return IProcess::OSCreationClassName(); }
    
    WCHAR* Win32Process::OSName() const { return IProcess::OSName(); }
    
    WCHAR* Win32Process::OtherExecutionDescription() const { return IProcess::OtherExecutionDescription() ; }
    
    std::uint32_t Win32Process::Priority() const { return IProcess::Priority(); }
    
    std::time_t Win32Process::TerminationDate() const { return IProcess::TerminationDate(); }
    
    std::uint64_t Win32Process::UserModeTime() const {  return IProcess::UserModeTime(); }
    
    std::uint64_t Win32Process::WorkingSetSize() const { return IProcess::WorkingSetSize(); }

    
    bool Win32Process::TerminateProcess() 
    { 
        printf("[%s]\n", __PRETTY_FUNCTION__);
        if (Attributes->Handle == NULL || Attributes->Handle == INVALID_HANDLE_VALUE) 
        {
            return false; 
        }
    
        if (::TerminateProcess(Attributes->Handle, 1))
        {
            return true;
        }
    
        DWORD lastError = ::GetLastError();
        ::wprintf(L"Initial TerminateProcess failed with error 0x%x\n", lastError);
        
        if (lastError == ERROR_ACCESS_DENIED)
        {
            ::wprintf(L"Trying to open new handle with PROCESS_ALL_ACCESS...\n");
            return false; 
        }
        return false; 
    }
    
    Win32Process::~Win32Process() 
    {
        TerminateProcess();
        if (Attributes->Handle && Attributes->Handle != INVALID_HANDLE_VALUE)
        {
            ::CloseHandle(Attributes->Handle);
        }
        
        if (Attributes->ThreadHandle && Attributes->ThreadHandle != INVALID_HANDLE_VALUE)
        {
            ::CloseHandle(Attributes->ThreadHandle);
        }
    }
}
