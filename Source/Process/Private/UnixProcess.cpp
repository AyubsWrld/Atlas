#include "UnixProcess.h"
#include "IProcess.h"

namespace Atlas::System::Process 
{
    WCHAR* UnixProcess::CreationClassName() const { return IProcess::CreationClassName() ; }
    
    std::time_t UnixProcess::CreationDate() const  { return IProcess::CreationDate() ; }
    
    WCHAR* UnixProcess::CSCreationClassName() const { return IProcess::CSCreationClassName(); }

    WCHAR* UnixProcess::CSName() const { return IProcess::CSName() ; }
    
    EExectuionState UnixProcess::ExecutionState() const { return IProcess::ExecutionState(); }
    
    WCHAR* UnixProcess::Handle() const { return IProcess::Handle(); }
    
    std::uint64_t UnixProcess::KernelModeTime() const { return IProcess::KernelModeTime(); }
    
    WCHAR* UnixProcess::Name() const { return IProcess::Name() ; }
    
    WCHAR* UnixProcess::OSCreationClassName() const { return IProcess::OSCreationClassName(); }
    
    WCHAR* UnixProcess::OSName() const { return IProcess::OSName(); }
    
    WCHAR* UnixProcess::OtherExecutionDescription() const { return IProcess::OtherExecutionDescription() ; }
    
    std::uint32_t UnixProcess::Priority() const { return IProcess::Priority(); }
    
    std::time_t UnixProcess::TerminationDate() const { return IProcess::TerminationDate(); }
    
    std::uint64_t UnixProcess::UserModeTime() const {  return IProcess::UserModeTime(); }
    
    std::uint64_t UnixProcess::WorkingSetSize() const { return IProcess::WorkingSetSize(); }

    bool    UnixProcess::TerminateProcess() { return IProcess::TerminateProcess(); }
        
}
