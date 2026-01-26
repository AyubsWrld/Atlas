#include "Win32Process.h"
#include <accctrl.h>
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

    bool  Win32Process::TerminateProcess() 
    { 
        // Don't need to set size; 
        PSECURITY_DESCRIPTOR pSecurityDescriptor = NULL;
        DWORD dSuccess = ::GetSecurityInfo(
                Attributes->Handle,
                SE_KERNEL_OBJECT,
                SCOPE_SECURITY_INFORMATION,
                NULL,
                NULL,
                NULL,
                NULL,
                &pSecurityDescriptor
                );

        if (dSuccess != ERROR_SUCCESS)
        {
            ::wprintf(L"Failed to retrieve security information 0x%x\n", Attributes.get()->Handle);
            _tprintf(TEXT("GetSecurityInfo error = %d\n"), GetLastError());
            return{}; 
        }

        SECURITY_DESCRIPTOR* SecurityDescriptor = (SECURITY_DESCRIPTOR*)pSecurityDescriptor;

        ::wprintf(L"Retreived Security Descriptor: %d\n", SecurityDescriptor->Control);
        ::wprintf(L"Terminating Processs with handle 0x%x\n", Attributes.get()->Handle);
        if (Attributes->Handle == INVALID_HANDLE_VALUE) 
        {
            ::wprintf(L"Invalid Handle\n");
            return false; 
        }

        if (::TerminateProcess(Attributes->Handle, -1) > 0)
        {
            ::wprintf(L"Failed to Terminate Processs With handle 0x%x\n", Attributes->Handle);
        }

        ::LocalFree(pSecurityDescriptor);
        return {}; 
    }

    Win32Process::~Win32Process() 
    {
        bool success = Win32Process::TerminateProcess(); 
        if (success)
        {
            printf("[%s] Successfully terminated process\n, 0x%x", __PRETTY_FUNCTION__, GetLastError());
        }
        printf("[%s]\n", __PRETTY_FUNCTION__);
        ::CloseHandle(Attributes->Handle);
        ::CloseHandle(Attributes->ThreadHandle);
    }
}
