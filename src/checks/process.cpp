#include "process.hpp"

#include <dirent.h>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <vector>

namespace checks {
namespace process {

Json::Value
runProcessCheck()
{
    int pid = -1;

    // Open the /proc directory
    DIR* dp = opendir("/proc");
    if (dp != NULL)
    {
        // Enumerate all entries in directory until process found
        struct dirent* dirp;
        while (pid < 0 && (dirp = readdir(dp)))
        {
            // Skip non-numeric entries
            int id = atoi(dirp->d_name);
            if (id > 0)
            {
                // Read contents of virtual /proc/{pid}/cmdline file
                std::string cmdPath = std::string("/proc/") + dirp->d_name + "/cmdline";
                std::ifstream cmdFile(cmdPath.c_str());
                std::string cmdLine;
                std::getline(cmdFile, cmdLine);
                if (!cmdLine.empty())
                {
                    // Keep first cmdline item which contains the program path
                    size_t pos = cmdLine.find('\0');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(0, pos);
                    // Keep program name only, removing the path
                    pos = cmdLine.rfind('/');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(pos + 1);
                    // Compare against requested process name
                    if (PROCESS_NAME == cmdLine)
                        pid = id;
                }
            }
        }
    }

    closedir(dp);

    Json::Value result;

    result["exit_code"] = 2;
    result["output"] = "Check Proccess CRITICAL: Found 0 matching rippled process";
    result["command"] = "CheckProcess";

    // find the proccess
    if (pid > -1)
    {
        result["exit_code"] = 0;
        result["output"] = "Check Proccess OK: Found process blong to rippled";
        result["command"] = "CheckProcess";
    }

    return result;
}

}  // namespace process
}  // namespace checks
