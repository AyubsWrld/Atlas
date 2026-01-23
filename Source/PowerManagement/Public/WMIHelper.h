#pragma once 
#include <wbemidl.h>
#include <comdef.h>
#include <stdio.h>
#include <iostream>

namespace _com_util {
  BSTR      WINAPI      ConvertStringToBSTR(const char *pSrc);
  char      *WINAPI     ConvertBSTRToString(BSTR pSrc);
}

namespace Atlas 
{
    class WMIHelper {

        /* 
         * The locator used to retrieve the interface used by clients and providers to access WMI services. 
         * The interface is implemented by WMI and WMI providers, and is the primary WMI interface.
        */ 
        IWbemLocator*       pLoc            { NULL };

        IWbemServices*      pSvc            { NULL };

        bool                initialized     { false } ;

    public:

        WMIHelper() = default; 

        ~WMIHelper() {
            Cleanup();
        }

        /*
        @purpose:           Sets process up for communication with Windows Management
                            Instrumentation

                                            return

        @return:            bool                    Boolean describing whether the setup
                                                    process succesfully executed or not.
                                                    On error, A call to GetLastError()
                                                    should be made to get the error code.

        @notes:             This function sets up the current proceess for communication 
                            with WMI. Currently the routine performs 5 separate albeit
                            related subroutines which are necessary for setting up 
                            communicaton with Windows Managemnet Instrumentation. 
                            Should the routine succeed, the member variable initialized
                            will be set to true. 
        */
        bool    Initialize();

        bool    EnableWakeOnMagicPacket(const wchar_t* adapterName);

        bool    ListNetworkAdapters(); 

        bool    GetWakeOnLanStatus(const wchar_t* adapterName);

        void    Cleanup(); 
    };
}
