#include "network.hpp"

#include "iostream"
#include <ifaddrs.h>
#include <net/if.h>

#include <chrono>
#include <thread>

namespace collectors {
namespace network {

// get static info for specific interface
StatisticInfo
getStatistic(std::string ifs)
{
    char rx_bytes_path[MAX_PATH_LEN];
    char tx_bytes_path[MAX_PATH_LEN];

    unsigned long long last_rx_bytes;
    unsigned long long last_tx_bytes;

    snprintf(rx_bytes_path, MAX_PATH_LEN, "/sys/class/net/%s/statistics/rx_bytes", ifs.c_str());
    snprintf(tx_bytes_path, MAX_PATH_LEN, "/sys/class/net/%s/statistics/tx_bytes", ifs.c_str());

    // get RX bytes
    FILE* rx_bytes_file = fopen(rx_bytes_path, "r");
    if (rx_bytes_file == NULL)
    {
        fprintf(stderr, "cannot open %s, error:%s\n", rx_bytes_path, strerror(errno));
    }
    fscanf(rx_bytes_file, "%llu", &last_rx_bytes);
    fclose(rx_bytes_file);

    // get TX bytes
    FILE* tx_bytes_file = fopen(tx_bytes_path, "r");
    if (tx_bytes_file == NULL)
    {
        fprintf(stderr, "cannot open %s, error:%s\n", tx_bytes_path, strerror(errno));
    }
    fscanf(tx_bytes_file, "%llu", &last_tx_bytes);
    fclose(tx_bytes_file);

    StatisticInfo info = {ifs, last_rx_bytes, last_tx_bytes};
    return info;
}

// get valid interfaces
std::vector<std::string>
getInterfaces()
{
    std::vector<std::string> validInterfaces;
    struct ifaddrs *ifap, *ifa;
    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr->sa_family == AF_INET && 0 == (ifa->ifa_flags & IFF_LOOPBACK))
        {
            validInterfaces.push_back(ifa->ifa_name);
        }
    }

    return validInterfaces;
}

std::vector<StatisticInfo>
gatterInfo()
{
    auto interfaces = getInterfaces();

    std::vector<StatisticInfo> info;
    // get all details for all interfaces
    for (auto const& i : interfaces)
    {
        auto st = getStatistic(i);
        info.push_back(st);
    }

    return info;
}

Json::Value
getNetworkInfo()
{
    Json::Value info;

    auto netIO = gatterInfo();

    // sleep for 1 secound
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));

    auto netIOSecount = gatterInfo();

    for (auto const& io : netIO)
    {
        for (auto const& lastio : netIOSecount)
        {
            if (io.name == lastio.name)
            {
                uint64_t inbound = (lastio.bytesRecv - io.bytesRecv) / 1024;
                uint64_t outbound = (lastio.bytesSent - io.bytesSent) / 1024;

                Json::Value obj;
                obj["name"] = io.name;
                obj["inbound"] = inbound;
                obj["outbound"] = outbound;

                info.append(obj);
            }
        }
    }

    return info;
};

}  // namespace network
}  // namespace collectors

