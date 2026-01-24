#include <iostream>
#include <stdio.h>
#include "ProcCommon.h"
#include "Win32Process.h"
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
    case Z3::unsat:   std::cout << "de-Morgan is valid\n"; break;
    case Z3::sat:     std::cout << "de-Morgan is not valid\n"; break;
    case Z3::unknown: std::cout << "unknown\n"; break;
    }
}

int main()
{
    using namespace Atlas::System::Process;

    sscanf(NULL, NULL);
    std::shared_ptr<Win32Process> Process = std::dynamic_pointer_cast<Win32Process>(SpawnProcess( "C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe" )); 
    if ( Process )
    {
        ::wprintf(L"Successfully spawned process\n");
    }

    int x; std::cin >> x; 
    return EXIT_SUCCESS;
}
