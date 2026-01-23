#pragma once
#include "ProcCommon.h"
#include <memory>

namespace Atlas::System::Process 
{
    class Win32Process: public IProcess 
    {

        std::unique_ptr<FWinProc> Attributes;

    public:
        
        /*
            CreationClassName indicates the name of the class or the subclass used
            in the creation of an instance. When used with the other key properties
            of this class, this property allows all instances of this class and its
            subclasses to be uniquely identified.
        */
    
        Win32Process(FWinProc&& ProcessAttributes) 
            : Attributes(std::make_unique<FWinProc>((ProcessAttributes)))
        {
        }

        Win32Process(FWinProc ProcessAttributes) 
            : Attributes(std::make_unique<FWinProc>(std::move(ProcessAttributes)))
        {
        }

        virtual WCHAR* CreationClassName() const override;
        
        /* 
            Time that the Process began executing.
        */
        virtual std::time_t CreationDate() const override; 
        
        /*
            The scoping ComputerSystem's CreationClassName.
        */
        virtual WCHAR* CSCreationClassName() const override;
        
        /* 
            The scoping ComputerSystem's Name.
        */
        virtual WCHAR* CSName() const override;
        
        /*
            Indicates the current operating condition of the process.
            Values include ready (2), running (3), and blocked (4), among others.
        */
        virtual EExectuionState ExecutionState() const override;
        
        /*
            A string used to identify the Process. A Process ID is a kind of Process Handle.
        */
        virtual WCHAR* Handle() const override;
        
        /*
            Time in kernel mode, in milliseconds. If this information is not available,
            or if the operating system does not distinguish between time in kernel and
            in user mode, a value of 0 should be used.
        */
        virtual std::uint64_t KernelModeTime() const override;
        
        /*
            The name of the process.
        */
        virtual WCHAR* Name() const override;
        
        /*
            The scoping OperatingSystem's CreationClassName.
        */
        virtual WCHAR* OSCreationClassName() const override;
        
        /*
            The scoping OperatingSystem's Name.
        */
        virtual WCHAR* OSName() const override;
        
        /*
            A string describing the state - used when the instance's ExecutionState
            property is set to 1 ("Other"). OtherExecutionDescription should be set
            to NULL when the Execution State property is any value other than 1.
        */
        virtual WCHAR* OtherExecutionDescription() const override;
        
        /*
            Priority indicates the urgency or importance of execution of a Process.
            Lower values reflect more favorable process scheduling. If a priority is
            not defined for a Process, a value of 0 should be used.
        */
        virtual std::uint32_t Priority() const override;
        
        /*
            Time that the Process was stopped or terminated.
        */
        virtual std::time_t TerminationDate() const override;
        
        /*
            Time in user mode, in milliseconds. If this information is not available,
            a value of 0 should be used. If the operating system does not distinguish
            between time in kernel mode and user mode, the time should be returned
            in this property.
        */
        virtual std::uint64_t UserModeTime() const override;
        
        /*
            The amount of memory in bytes that a Process needs to execute efficiently,
            for an OperatingSystem that uses page-based memory management. If an
            insufficient amount of memory is available (< working set size), thrashing
            will occur. If this information is not known, NULL or 0 should be entered.
        */
        virtual std::uint64_t WorkingSetSize() const override;

        bool    TerminateProcess() override; 

        virtual ~Win32Process() override; 
        
    };
}
