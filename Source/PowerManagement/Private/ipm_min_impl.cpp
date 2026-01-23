#include "IPowerManager.h"

// Set flag for unimplemented. 
namespace Atlas 
{
    bool IPowerManager::PowerManagementCapabilites() const
    {
        return false; 
    }

    bool IPowerManager::CanWakeRemotely() const 
    {
        return false; 
    }

    bool IPowerManager::Sleep() const 
    {
        return false;
    }

    bool IPowerManager::Shutdown() const
    {
        return false;
    }

    bool IPowerManager::Restart() const
    {
        return false;
    }
}
