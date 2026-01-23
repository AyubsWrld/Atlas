#pragma once 

// __purecall

namespace Atlas 
{
    class IPowerManager 
    {
        virtual bool PowerManagementCapabilites() const = 0;
        virtual bool CanWakeRemotely() const = 0;
        virtual bool Sleep() const = 0;
        virtual bool Shutdown() const = 0;
        virtual bool Restart() const = 0;
    };
}
