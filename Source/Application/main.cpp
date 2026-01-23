#include "IProcess.h"
#include "PowerManager.h"
#include <errhandlingapi.h>
#include <iostream>
#include "ProcCommon.h"
#include "z3++.h"

void demorgan() {
    std::cout << "de-Morgan example\n";
    
    context c;

    expr x = c.bool_const("x");
    expr y = c.bool_const("y");
    expr conjecture = (!(x && y)) == (!x || !y);
    
    solver s(c);
    // adding the negation of the conjecture as a constraint.
    s.add(!conjecture);
    std::cout << s << "\n";
    std::cout << s.to_smt2() << "\n";
    switch (s.check()) {
    case unsat:   std::cout << "de-Morgan is valid\n"; break;
    case sat:     std::cout << "de-Morgan is not valid\n"; break;
    case unknown: std::cout << "unknown\n"; break;
    }
}

#ifdef _WIN32
void DumpRoutine()
{
    Atlas::WMIHelper wmi;

    if (!wmi.Initialize()) {
        std::cerr << "Failed to initialize WMI" << std::endl;
        return;
    }

    wmi.ListNetworkAdapters();

    const wchar_t* adapterName = L"D-Link DWA-171 AC600 MU-MIMO Wi-Fi USB Adapter";

    std::wcout << L"\n=== Checking WOL status for: " << adapterName << L" ===" << std::endl;
    wmi.GetWakeOnLanStatus(adapterName);

    std::wcout << L"\n=== Enabling Wake-on-LAN ===" << std::endl;
    if (wmi.EnableWakeOnMagicPacket(adapterName)) {
        std::cout << "Successfully configured Wake-on-LAN" << std::endl;
    } else {
        std::cout << "Failed to configure Wake-on-LAN" << std::endl;
    }

    std::wcout << L"\n=== Checking WOL status again ===" << std::endl;
    wmi.GetWakeOnLanStatus(adapterName);
}
#endif

int main()
{
    using namespace Atlas::System::Process;
    
    IProcess* Process = SpawnProcess( "C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe" );
    if ( Process )
    {
        ::wprintf(L"It worked\n");
    }

    int x; 
    std::cin >> x; 

    bool xx = Process->TerminateProcess();


    xx ? ::wprintf(L"Termination Successfully\n") : ::wprintf(L"Termination Failed: 0x%x\n", ::GetLastError()) ;
    Process->IProcess::~IProcess();
    return EXIT_SUCCESS;
}
