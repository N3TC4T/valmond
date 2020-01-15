#pragma once

#include <json/json.h>

#define PROCESS_NAME "rippled"

namespace checks {
namespace process {

Json::Value
runProcessCheck();

}  // namespace process
}  // namespace checks

