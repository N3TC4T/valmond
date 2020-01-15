#pragma once

#include <json/json.h>

namespace collectors {
namespace disk {

struct DevInfo
{
    std::string name;
    std::string path;
    std::string fstype;
    uint64_t total;
    uint64_t free;
    uint64_t used;
    double percent;
};

Json::Value
getDiskInfo();

}  // namespace disk
}  // namespace collectors
