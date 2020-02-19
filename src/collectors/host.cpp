#include "host.hpp"

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

namespace collectors {
namespace host {

std::string
getIPAddress()
{
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen("hostname -i", "r");
    if (!pipe)
        throw std::runtime_error("hostname command failed!");
    try
    {
        while (!feof(pipe))
        {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    }
    catch (...)
    {
        pclose(pipe);
        throw;
    }
    pclose(pipe);

    const std::string delim = " ";
    std::vector<std::string> ips;
    size_t prev = 0, pos = 0;
    do
    {
        pos = result.find(delim, prev);
        if (pos == std::string::npos)
            pos = result.length();
        std::string ip = result.substr(prev, pos - prev);
        if (!ip.empty())
            ips.push_back(ip);
        prev = pos + delim.length();
    } while (pos < result.length() && prev < result.length());

    // get first ip address
    return ips[0];
}

// Uptime - returns uptime string
// uptime = "{0} days {1} hours {2} minutes".format(days, hours, minutes)
std::string
getUptime()
{
    /* Conversion constants. */
    const long minute = 60;
    const long hour = minute * 60;
    const long day = hour * 24;

    struct sysinfo si;
    sysinfo(&si);

    char buffer[100];

    snprintf(
        buffer,
        sizeof(buffer),
        "%ld days %ld hours %02ld minutes",
        si.uptime / day,
        (si.uptime % day) / hour,
        (si.uptime % hour) / minute);

    std::string uptime = buffer;

    return uptime;
}

Json::Value
getHostInfo()
{
    Json::Value result;

    // os distro
    struct utsname utname;
    int rc = uname(&utname);
    if (rc >= 0)
    {
        result["distro"]["name"] = std::string(utname.sysname);
        result["distro"]["version"] = std::string(utname.release) + std::string(utname.version);
    }
    else
    {
        result["distro"]["name"] = "Unknown";
        result["distro"]["version"] = "Unknows";
    }

    // hostname
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    result["host"] = hostname;

    // get matchin ip address
    // result["ip_address"] = getIPAddress();

    return result;
};

}  // namespace host
}  // namespace collectors

