#pragma once

#include <json/json.h>

#define MAX_PATH_LEN 64
#define SLEEP_MS 1000

namespace collectors {
namespace network {

struct StatisticInfo
{
    std::string name;
    unsigned long long bytesRecv;
    unsigned long long bytesSent;
};

Json::Value
getNetworkInfo();

}  // namespace network
}  // namespace collectors
