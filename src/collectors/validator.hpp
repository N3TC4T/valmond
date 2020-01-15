#pragma once

#include <json/json.h>

namespace collectors {
namespace validator {

Json::Value
getServerInfo();

Json::Value
getValidatorInfo();

}  // namespace validator
}  // namespace collectors
