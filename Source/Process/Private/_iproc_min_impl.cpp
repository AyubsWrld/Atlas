#include "../Public/IProcess.h"

namespace Atlas::System::Process 
{
    WCHAR* IProcess::CreationClassName() const { return nullptr; }
    
    std::time_t IProcess::CreationDate() const  { return ERRONEOUS_TIME_VALUE ; }
    
    WCHAR* IProcess::CSCreationClassName() const { return nullptr; }

    WCHAR* IProcess::CSName() const { return nullptr; }
    
    EExectuionState IProcess::ExecutionState() const { return EExectuionState::UNSPEC; }
    
    WCHAR* IProcess::Handle() const { return nullptr; }
    
    std::uint64_t IProcess::KernelModeTime() const { return 0; }
    
    WCHAR* IProcess::Name() const { return nullptr; }
    
    WCHAR* IProcess::OSCreationClassName() const { return nullptr; }
    
    WCHAR* IProcess::OSName() const { return nullptr; }
    
    WCHAR* IProcess::OtherExecutionDescription() const { return nullptr; }
    
    std::uint32_t IProcess::Priority() const { return 0; }
    
    std::time_t IProcess::TerminationDate() const { return 0; }
    
    std::uint64_t IProcess::UserModeTime() const {  return 0; }
    
    std::uint64_t IProcess::WorkingSetSize() const { return 0; }

    bool    IProcess::TerminateProcess() { return false; }
        
}
