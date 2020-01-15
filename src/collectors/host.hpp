#pragma once

#include <json/json.h>

namespace collectors {
namespace host {

std::string
getUptime();

Json::Value
getHostInfo();

}  // namespace host
}  // namespace collectors

