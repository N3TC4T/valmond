#include "cpu.hpp"

#include <chrono>
#include <cmath>
#include <thread>

#include "common/utils.hpp"

namespace collectors {
namespace cpu {

size_t
getIdleTime(const CpuStat& e)
{
    return e.S_IDLE + e.S_IOWAIT;
}

size_t
getActiveTime(const CpuStat& e)
{
    return e.S_USER + e.S_NICE + e.S_SYSTEM + e.S_IRQ + e.S_SOFTIRQ + e.S_STEAL + e.S_GUEST +
        e.S_GUEST_NICE;
}

void
readStatsCPU(CpuStat& s)
{
    FILE* file;

    file = fopen("/proc/stat", "r");
    fscanf(
        file,
        "cpu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
        &s.S_USER,
        &s.S_NICE,
        &s.S_SYSTEM,
        &s.S_IDLE,
        &s.S_IOWAIT,
        &s.S_IRQ,
        &s.S_SOFTIRQ,
        &s.S_STEAL,
        &s.S_GUEST,
        &s.S_GUEST_NICE);
    fclose(file);
}

Json::Value
getCpuInfo()
{
    Json::Value result;

    CpuStat e1;
    CpuStat e2;

    // get first snapshot
    readStatsCPU(e1);

    // sleep for 1 secound
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));

    // get secound snapshot
    readStatsCPU(e2);

    const float ACTIVE_TIME = static_cast<float>(getActiveTime(e2) - getActiveTime(e1));
    const float IDLE_TIME = static_cast<float>(getIdleTime(e2) - getIdleTime(e1));
    const float TOTAL_TIME = ACTIVE_TIME + IDLE_TIME;

    const float system = 100.f * (e2.S_SYSTEM - e1.S_SYSTEM) / TOTAL_TIME;
    const float user = 100.f * (e2.S_USER - e1.S_USER) / TOTAL_TIME;
    const float nice = 100.f * (e2.S_NICE - e1.S_NICE) / TOTAL_TIME;
    const float idle = 100.f * (e2.S_IDLE - e1.S_IDLE) / TOTAL_TIME;
    const float steal = 100.f * (e2.S_STEAL - e1.S_STEAL) / TOTAL_TIME;
    const float iowait = 100.f * (e2.S_IOWAIT - e1.S_IOWAIT) / TOTAL_TIME;

    result["user"] = floatDecimalPoint(user);
    result["nice"] = floatDecimalPoint(nice);
    result["system"] = floatDecimalPoint(system);
    result["idle"] = floatDecimalPoint(idle);
    result["steal"] = floatDecimalPoint(steal);
    result["iowait"] = floatDecimalPoint(iowait);

    return result;
};

}  // namespace cpu
}  // namespace collectors

