#pragma once

#ifdef _WIN32
#include <limits>
#include <stdint.h>
#include <handleapi.h>
#include <cstring>
#include <windows.h>
#include <devguid.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <ddk/ndisguid.h>
#include <winnt.h>
#include <vector>
#include <string>
#include <ntddndis.h>
#include <wbemidl.h>
#include <comdef.h>
#include "WMIHelper.h"

#ifndef NODISCARD 
    #define NODISCARD [[nodiscard]]
#endif


namespace Atlas 
{
    struct FGuid
    {
    uint32_t  A {}; 
    uint32_t  B {}; 
    uint32_t  C {}; 
    uint32_t  D {}; 
    };

    enum : ULONG 
    {
    CAPMASK_DEVICE_D1            = 1,
    CAPMASK_DEVICE_D2            = 1 << 1,
    CAPMASK_LOCK_SUPPORTED       = 1 << 2,
    CAPMASK_EJECT_SUPPORTED      = 1 << 3,
    CAPMASK_REMOVABLE            = 1 << 4,
    CAPMASK_DOCK_DEVICE          = 1 << 5,
    CAPMASK_UNIQUE_ID            = 1 << 6,
    CAPMASK_SILENT_INSTALL       = 1 << 7,
    CAPMASK_RAW_DEVICE_OK        = 1 << 8,
    CAPMASK_SURPRISE_REMOVAL_OK  = 1 << 9,
    CAPMASK_WAKE_FROM_D0         = 1 << 10,
    CAPMASK_WAKE_FROM_D1         = 1 << 11,
    CAPMASK_WAKE_FROM_D2         = 1 << 12,
    CAPMASK_WAKE_FROM_D3         = 1 << 13,
    CAPMASK_HARDWARE_DISABLED    = 1 << 14,
    CAPMASK_NON_DYNAMIC          = 1 << 15,
    CAPMASK_WARM_EJECT_SUPPORTED = 1 << 16,
    CAPMASK_NO_DISPLAY_IN_UI     = 1 << 17,
    CAPMASK_RESERVED             = (std::numeric_limits<ULONG>::max() << 18)
    };

    struct FDeviceCapabilities
    {


        ULONG DeviceD1:1;
        ULONG DeviceD2:1;
        ULONG LockSupported:1;
        ULONG EjectSupported:1;
        ULONG Removable:1;
        ULONG DockDevice:1;
        ULONG UniqueID:1;
        ULONG SilentInstall:1;
        ULONG RawDeviceOK:1;
        ULONG SurpriseRemovalOK:1;
        ULONG WakeFromD0:1;
        ULONG WakeFromD1:1;
        ULONG WakeFromD2:1;
        ULONG WakeFromD3:1;
        ULONG HardwareDisabled:1;
        ULONG NonDynamic:1;
        ULONG WarmEjectSupported:1;
        ULONG NoDisplayInUI:1;
        ULONG Reserved:14;

        using   PointerType     =       FDeviceCapabilities*;


        FDeviceCapabilities() = default;
        FDeviceCapabilities(ULONG Value)
        {
            std::memcpy(this, &Value, sizeof(ULONG));
        }


        void DumpCapabilities() const
        {
            std::cout << "Device Capabilities:\n"
                        << "  DeviceD1: " << DeviceD1 << "\n"
                        << "  DeviceD2: " << DeviceD2 << "\n"
                        << "  LockSupported: " << LockSupported << "\n"
                        << "  EjectSupported: " << EjectSupported << "\n"
                        << "  Removable: " << Removable << "\n"
                        << "  DockDevice: " << DockDevice << "\n"
                        << "  UniqueID: " << UniqueID << "\n"
                        << "  SilentInstall: " << SilentInstall << "\n"
                        << "  RawDeviceOK: " << RawDeviceOK << "\n"
                        << "  SurpriseRemovalOK: " << SurpriseRemovalOK << "\n"
                        << "  WakeFromD0: " << WakeFromD0 << "\n"
                        << "  WakeFromD1: " << WakeFromD1 << "\n"
                        << "  WakeFromD2: " << WakeFromD2 << "\n"
                        << "  WakeFromD3: " << WakeFromD3 << "\n"
                        << "  HardwareDisabled: " << HardwareDisabled << "\n"
                        << "  NonDynamic: " << NonDynamic << "\n"
                        << "  WarmEjectSupported: " << WarmEjectSupported << "\n"
                        << "  NoDisplayInUI: " << NoDisplayInUI << "\n"
                        << "  Reserved: " << Reserved << "\n";
        }

        operator ULONG() const 
        {
            return *reinterpret_cast<ULONG*>(const_cast<PointerType>(this));
        }


    };


    // BSTR WINAPI ConvertStringToBSTR(const char *pSrc);
    // char *WINAPI ConvertBSTRToString(BSTR pSrc);




    /*
    @purpose:        Utility function for enumarting all present network interfaces.

    @param:          [in]                     PCSTR         p_partition_name

                                        return

    @return:         vector<string>          Vector containing a list of human 
                                                readable names of present network
                                                interfaces.


    @notes:          This function delegates platform logic and does not itself implement
                        partition logic directly.

    */


    NODISCARD std::vector<std::string> EnumerateNetInterfaces();

    NODISCARD std::vector<FDeviceCapabilities> EnumeratePropertyKeys();

}

#endif
