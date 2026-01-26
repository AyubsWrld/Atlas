#pragma once
#include <ctime>
#include <cstdint>
#include "Scope.h"

#if defined(_WIN32)
    #include <windows.h>
    #include <tchar.h>
    #include <psapi.h>
    #include <winnt.h>
    #include <processthreadsapi.h>
    #include <shlwapi.h>
    #include <aclapi.h>
#elif defined(__linux__)
    #include <spawn.h>
#endif


#ifndef __WCHAR_DEFINED
#define __WCHAR_DEFINED
  using     WCHAR   =   wchar_t;
  using     UINT64  =   std::uint16_t;
  using     TCHAR   =   std::uint16_t;
#endif


/* 
 *  Each instance of the Process class represents a single instance of a running program.
 *  A user of the OperatingSystem will typically see a Process as an application or task.
 *  Within an OperatingSystem, a Process is defined by a workspace of memory resources and
 *  environmental settings that are allocated to it. On a multitasking System, this workspace
 *  prevents intrusion of resources by other Processes. Additionally, a Process can execute as
 *  multiple Threads, all which run within the same workspace.
*/

namespace Atlas::System::Process 
{
    /* 
        Indicates the current operating condition of the Process.
    */

    enum 
    { 
        ERRONEOUS_TIME_VALUE    = -1,
        UNIMPLEMENTED_VALUE     = -1  // use this to signify that invocation called a stub. 
    };

    enum class EExectuionState : uint16_t 
    {
        UNSPEC, 
        READY,
        RUNNING,
        BLOCKED,
        TERMINATED,
        STOPPED,
    };

    class IProcess 
    {

    public:
        using TIME = std::time_t;
        
        /*
            CreationClassName indicates the name of the class or the subclass used
            in the creation of an instance. When used with the other key properties
            of this class, this property allows all instances of this class and its
            subclasses to be uniquely identified.
        */
        virtual WCHAR* CreationClassName() const = 0;
        
        /* 
            Time that the Process began executing.
        */
        virtual std::time_t CreationDate() const = 0;
        
        /*
            The scoping ComputerSystem's CreationClassName.
        */
        virtual WCHAR* CSCreationClassName() const = 0;
        
        /* 
            The scoping ComputerSystem's Name.
        */
        virtual WCHAR* CSName() const = 0;
        
        /*
            Indicates the current operating condition of the process.
            Values include ready (2), running (3), and blocked (4), among others.
        */
        virtual EExectuionState ExecutionState() const = 0;
        
        /*
            A string used to identify the Process. A Process ID is a kind of Process Handle.
        */
        virtual WCHAR* Handle() const = 0;
        
        /*
            Time in kernel mode, in milliseconds. If this information is not available,
            or if the operating system does not distinguish between time in kernel and
            in user mode, a value of 0 should be used.
        */
        virtual std::uint64_t KernelModeTime() const = 0;
        
        /*
            The name of the process.
        */
        virtual WCHAR* Name() const = 0;
        
        /*
            The scoping OperatingSystem's CreationClassName.
        */
        virtual WCHAR* OSCreationClassName() const = 0;
        
        /*
            The scoping OperatingSystem's Name.
        */
        virtual WCHAR* OSName() const = 0;
        
        /*
            A string describing the state - used when the instance's ExecutionState
            property is set to 1 ("Other"). OtherExecutionDescription should be set
            to NULL when the Execution State property is any value other than 1.
        */
        virtual WCHAR* OtherExecutionDescription() const = 0;
        
        /*
            Priority indicates the urgency or importance of execution of a Process.
            Lower values reflect more favorable process scheduling. If a priority is
            not defined for a Process, a value of 0 should be used.
        */
        virtual std::uint32_t Priority() const = 0;
        
        /*
            Time that the Process was stopped or terminated.
        */
        virtual std::time_t TerminationDate() const = 0;
        
        /*
            Time in user mode, in milliseconds. If this information is not available,
            a value of 0 should be used. If the operating system does not distinguish
            between time in kernel mode and user mode, the time should be returned
            in this property.
        */
        virtual std::uint64_t UserModeTime() const = 0;
        
        /*
            The amount of memory in bytes that a Process needs to execute efficiently,
            for an OperatingSystem that uses page-based memory management. If an
            insufficient amount of memory is available (< working set size), thrashing
            will occur. If this information is not known, NULL or 0 should be entered.
        */
        
        virtual std::uint64_t WorkingSetSize() const = 0;

        virtual bool    TerminateProcess() = 0;
        
        virtual ~IProcess() = default;

    };
}
