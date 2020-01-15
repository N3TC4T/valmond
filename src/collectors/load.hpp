#pragma once

#include <json/json.h>

namespace collectors {
namespace load {

Json::Value
getLoadAvg();

}  // namespace load
}  // namespace collectors
