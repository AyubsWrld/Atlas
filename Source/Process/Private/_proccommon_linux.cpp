#include "ProcCommon.h"

#include <iostream>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <stdexcept>

namespace Atlas::System::Process::Impl 
{
    IProcess* GetProcessByPID_Linux(UINT64 PID)
    {
        return nullptr;
    };

    bool is_digits(const std::string &str) 
    {
        return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
    }

    std::string get_process_name(pid_t pid) {
        std::string path = "/proc/" + std::to_string(pid) + "/comm";
        std::ifstream file(path);
        if (!file.is_open()) {
            return "[unknown]"; // process may have ended or permission denied
        }
        std::string name;
        std::getline(file, name);
        return name;
    }

    const TCHAR*    EnumerateProcesses_Linux()
    {
        std::vector<std::pair<pid_t, std::string>> processes;

        for (const auto &entry : std::filesystem::directory_iterator("/proc")) {
            if (entry.is_directory()) {
                std::string dirname = entry.path().filename().string();
                if (is_digits(dirname)) {
                    pid_t pid = std::stoi(dirname);
                    std::string name = get_process_name(pid);
                    processes.emplace_back(pid, name);
                }
            }
        }

        std::cout << "PID\tProcess Name\n";
        std::cout << "----------------------\n";
        for (const auto &[pid, name] : processes) {
            std::cout << pid << "\t" << name << "\n";
        }
        return{}; 
    }

    std::shared_ptr<IProcess>   SpawnProcess_Linux(LPCSTR ProcessName)
    {
        return{};
    }
}

