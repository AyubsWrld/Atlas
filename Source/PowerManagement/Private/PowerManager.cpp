#include "PowerManager.h"

#ifdef _WIN32
#include <bitset>
#include <errhandlingapi.h>
#include <handleapi.h>
#include <minwindef.h>
#include <ntddndis.h>
#include <setupapi.h>
#include <string>

#define NODISCARD [[nodiscard]]

namespace Atlas 
{
    FORCEINLINE SP_DEVINFO_DATA CreateDeviceInfoElement()
    {
    return SP_DEVINFO_DATA 
    {
        .cbSize = sizeof(SP_DEVINFO_DATA )
    };
    }

    NODISCARD std::vector<std::string> EnumerateNetInterfaces()
    {
    std::vector<std::string> NetInterfaces;
    HDEVINFO DeviceInfoSet { SetupDiGetClassDevsA(
        &GUID_DEVCLASS_NET,
        NULL,
        NULL,
        DIGCF_PRESENT
        )
    };

    if (DeviceInfoSet != INVALID_HANDLE_VALUE)
    {
        DWORD             MemberIndex           { 0 };
        DWORD             RequiredSize          { 0 };
        SP_DEVINFO_DATA   DeviceInfoData        { CreateDeviceInfoElement() };
        char              PropertyBuffer[256]   { 0 };

        while(SetupDiEnumDeviceInfo(DeviceInfoSet, MemberIndex, &DeviceInfoData))
        {
            if(SetupDiGetDeviceRegistryProperty(
                DeviceInfoSet,
                &DeviceInfoData,
                SPDRP_FRIENDLYNAME,
                NULL,
                (PBYTE)PropertyBuffer,
                sizeof(PropertyBuffer),
                &RequiredSize)
            )
            {
                NetInterfaces.push_back(PropertyBuffer);
            }
            MemberIndex++;
        }
        SetupDiDestroyDeviceInfoList(DeviceInfoSet);
    }
        return NetInterfaces; 
    }

    NODISCARD std::vector<FDeviceCapabilities> EnumeratePropertyKeys()
    {
        HDEVINFO DeviceInfoSet = SetupDiGetClassDevs( 
                &GUID_DEVCLASS_NET, 
                NULL,
                NULL,
                DIGCF_PRESENT
                );
        if (DeviceInfoSet == INVALID_HANDLE_VALUE) return{} ;  // Should return a meaningful value on fail.

        DWORD               MemberIndex     { 0 };
        SP_DEVINFO_DATA     DeviceInfoElem  { CreateDeviceInfoElement() };
        DWORD               Property        { 0 };
        DWORD               PropertyBuffer  { 0 };

        std::vector<FDeviceCapabilities> DeviceCap;

        while(SetupDiEnumDeviceInfo(DeviceInfoSet, MemberIndex, &DeviceInfoElem))
        {
            if (SetupDiGetDeviceRegistryPropertyA(
                    DeviceInfoSet,
                    &DeviceInfoElem,
                    SPDRP_CAPABILITIES,
                    NULL,
                    (PBYTE)&PropertyBuffer,
                    sizeof(DWORD),
                    NULL
                    )
                )
            {
                DeviceCap.emplace_back(PropertyBuffer);
            }
            MemberIndex++;
        }
        SetupDiDestroyDeviceInfoList(DeviceInfoSet);
        return DeviceCap; 
    }
}
#endif

