#pragma once

#include <json/json.h>

#define SLEEP_MS 1000

namespace collectors {
namespace cpu {

const int NUM_CPU_STATES = 10;

struct CpuStat
{
    unsigned long long S_USER;
    unsigned long long S_NICE;
    unsigned long long S_SYSTEM;
    unsigned long long S_IDLE;
    unsigned long long S_IOWAIT;
    unsigned long long S_IRQ;
    unsigned long long S_SOFTIRQ;
    unsigned long long S_STEAL;
    unsigned long long S_GUEST;
    unsigned long long S_GUEST_NICE;
};

Json::Value
getCpuInfo();

}  // namespace cpu
}  // namespace collectors
