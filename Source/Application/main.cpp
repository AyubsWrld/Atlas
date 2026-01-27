#include <iostream>
#include <memory>
#include <stdio.h>
#include "IProcess.h"
#include "ProcCommon.h"
#include "Win32Process.h"
#include "LanguageProcessing.h"
#include "z3++.h"


void demorgan() {
    std::cout << "de-Morgan example\n";

    Z3::context c;

    Z3::expr x = c.bool_const("x");
    Z3::expr y = c.bool_const("y");
    Z3::expr conjecture = (!(x && y)) == (!x || !y);

    Z3::solver s(c);
    s.add(!conjecture);
    std::cout << s << "\n";
    std::cout << s.to_smt2() << "\n";
    switch (s.check()) {
    case Z3::sat:     std::cout << "de-Morgan is not valid\n"; break;
    case Z3::unknown: std::cout << "unknown\n"; break;
    }
}


void TestEnumerateW32()
{
    using namespace Atlas;
    std::wcout << L"Enumerating Installed Programs..." << std::endl;
    std::wcout << L"=================================" << std::endl << std::endl;

    std::vector<Software> programs = InstalledPrograms::GetInstalledPrograms();

    std::wcout << L"Found " << programs.size() << L" programs:" << std::endl << std::endl;

    bool bHasSpawned { FALSE } ;

    for (size_t i = 0; i < programs.size(); i++)
    {
        const Software& sw = programs[i];
       
        std::wcout << L"[" << (i + 1) << L"] " << sw.DisplayName << std::endl;
        
        if (!sw.InstallLocation.empty())
        {
            // std::wstring FullPath = sw.InstallLocation + L"\\" + sw.DisplayName + L".exe";
            // std::wcout << L"    Location: " << FullPath << std::endl;
            std::wcout << L"    Location: " << sw.InstallLocation << std::endl;
        }
        
        std::wcout << L"    Arch: ";
        switch (sw.Architecture)
        {
            case EBitMode::x64:
                std::wcout << L"64-bit";
                break;
            case EBitMode::x32:
                std::wcout << L"32-bit";
                break;
            default:
                std::wcout << L"Unknown";
                break;
        }

        std::wcout << std::endl << std::endl;

        // always append '\'  if bitmode == 32bit
        if (!bHasSpawned 
                && sw.Architecture == EBitMode::x64 
                && !sw.InstallLocation.empty() 
                && !sw.InstallLocation.contains(L"(x86)"))
        {

            std::string Location { 
                sw.DisplayName.begin(),
                sw.DisplayName.end() 
            };

            if ( auto Proc = System::Process::SpawnProcess(Location.c_str())) 
            {
                std::cout << Location << "\n" << std::endl; 
                int x; std::cin >> x; 
            };
        }
    }
}

int main()
{
    using namespace Atlas::System::Process;
    
    // Unneeded as what was constructed was a Win32Process
    // std::shared_ptr<Win32Process> Process = std::dynamic_pointer_cast<Win32Process>(SpawnProcess( "C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe" )); 
    
    // std::shared_ptr<IProcess> Process = SpawnProcess( "C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe" ); 
    // std::shared_ptr<IProcess> Process = SpawnProcess("C:\\Program Files\\PureRef\\PureRef.exe"); 
    // if ( Process )
    // {
    //     ::wprintf(L"Successfully spawned process: 0x%x\n", GetLastError());
    // }
    // int x; std::cin >> x; 
    
    TestEnumerateW32(); 
    return EXIT_SUCCESS;
}
