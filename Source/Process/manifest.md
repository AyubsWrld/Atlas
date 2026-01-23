Each instance of the Process class represents a single instance of a running program. A user of the OperatingSystem will typically see a Process as an application or task. Within an OperatingSystem, a Process is defined by a workspace of memory resources and environmental settings that are allocated to it. On a multitasking System, this workspace prevents intrusion of resources by other Processes. Additionally, a Process can execute as multiple Threads, all which run within the same workspace.



Currently the Interface for manipulating Processes is divided into three separate logical parts.
    - A forward facing API which exposes a uniform interface for the invocation of process operations like Creating/Destroying/Interacting agnostically of the underlying operating system. ( Proccommon.h ). Proccommon does this by statically dispatching calls made to the underlying operating system at compile time based on the target platform/environment. The platform specific definitions of the functions is defined within _proccommon_*PLATFORM*.cpp file. For example, Proccommon might declare a function, `CreateProcess`, as: 

```cpp
IProcess* CreateProcess(WCHAR* pProcessName);
```

and define it as: 

```cpp
IProcess* CreateProcess(WCHAR* pProcessName)
{
#if defined(_WIN)
    return Impl::CreateProcess_Win32(WCHAR* pProcessName)
#elif defined(__linux__)
    return Impl::CreateProcess_Linux(WCHAR* pProcessName)
#else  // Unsupported platform. 
    return {} 
#endif
}
```

Interface -> Platform Specific Knowledge. 

The result of these functions are interfaces returned to the client for further manipulation of the processes they create.
