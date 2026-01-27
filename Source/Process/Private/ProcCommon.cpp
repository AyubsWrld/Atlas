#include "ProcCommon.h"
#include <algorithm>
#include <set>


// NOTE: temporary needs cleaning
namespace Atlas 
{
    static std::wstring ReadRegistryString(HKEY hKey, const wchar_t* valueName)
    {
        WCHAR buffer[MAX_PATH] = {0};
        DWORD bufferSize = sizeof(buffer);
        DWORD type = 0;
        
        if (RegQueryValueExW(hKey, valueName, NULL, &type, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
        {
            if (type == REG_SZ || type == REG_EXPAND_SZ)
            {
                if (type == REG_EXPAND_SZ)
                {
                    WCHAR expanded[MAX_PATH] = {0};
                    ExpandEnvironmentStringsW(buffer, expanded, MAX_PATH);
                    return std::wstring(expanded);
                }
                return std::wstring(buffer);
            }
        }
        return L"";
    }

    static DWORD ReadRegistryDWORD(HKEY hKey, const wchar_t* valueName)
    {
        DWORD value = 0;
        DWORD bufferSize = sizeof(DWORD);
        DWORD type = 0;
        
        if (RegQueryValueExW(hKey, valueName, NULL, &type, (LPBYTE)&value, &bufferSize) == ERROR_SUCCESS)
        {
            if (type == REG_DWORD)
                return value;
        }
        return 0;
    }

    // scan a single uninstall registry key
    static void ScanUninstallKey(HKEY rootKey, const wchar_t* subKeyPath, REGSAM access, 
                                  EBitMode arch, std::vector<Software>& programs)
    {
        HKEY uninstallKey;
        if (RegOpenKeyExW(rootKey, subKeyPath, 0, KEY_READ | access, &uninstallKey) != ERROR_SUCCESS)
            return;

        // enumerate all subkeys
        DWORD index = 0;
        WCHAR subKeyName[256];
        
        while (true)
        {
            DWORD subKeyNameSize = 256;
            if (RegEnumKeyExW(uninstallKey, index++, subKeyName, &subKeyNameSize, 
                             NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                break;

            // open this program's key
            HKEY programKey;
            if (RegOpenKeyExW(uninstallKey, subKeyName, 0, KEY_READ | access, &programKey) == ERROR_SUCCESS)
            {
                // skip system components
                if (ReadRegistryDWORD(programKey, L"SystemComponent") == 1)
                {
                    RegCloseKey(programKey);
                    continue;
                }

                // FIXME: get display name swapped for getting executable name. 
                // std::wstring displayName = ReadRegistryString(programKey, L"DisplayName");
                std::wstring displayName = ReadRegistryString(programKey, L"DisplayIcon");
                
                // Only add if we have a name
                if (!displayName.empty())
                {
                    std::wstring installLocation = ReadRegistryString(programKey, L"InstallLocation");
                    programs.push_back(Software(displayName, installLocation, arch));
                }

                RegCloseKey(programKey);
            }
        }

        RegCloseKey(uninstallKey);
    }

    std::vector<Software> InstalledPrograms::GetInstalledPrograms()
    {
        std::vector<Software> programs;
        std::set<std::wstring> seen; // track duplicates

        // Scan 64-bit registry
        ScanUninstallKey(HKEY_LOCAL_MACHINE, 
                        L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
                        KEY_WOW64_64KEY, EBitMode::x64, programs);

        // Scan 32-bit registry
        ScanUninstallKey(HKEY_LOCAL_MACHINE, 
                        L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
                        KEY_WOW64_32KEY, EBitMode::x32, programs);

        // Remove duplicates (keep first occurrence)
        std::vector<Software> uniquePrograms;
        for (const auto& prog : programs)
        {
            if (seen.find(prog.DisplayName) == seen.end())
            {
                seen.insert(prog.DisplayName);
                uniquePrograms.push_back(prog);
            }
        }

        // Sort alphabetically
        std::sort(uniquePrograms.begin(), uniquePrograms.end(), 
                 [](const Software& a, const Software& b) {
                     std::wstring aLower = a.DisplayName;
                     std::wstring bLower = b.DisplayName;
                     std::transform(aLower.begin(), aLower.end(), aLower.begin(), ::towlower);
                     std::transform(bLower.begin(), bLower.end(), bLower.begin(), ::towlower);
                     return aLower < bLower;
                 });

        return uniquePrograms;
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
    }
}
