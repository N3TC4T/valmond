#include "memory.hpp"

#include <cmath>
#include <sys/sysinfo.h>

#include "common/utils.hpp"

namespace collectors {
namespace memory {

int
inMegabyte(long long d)
{
    return d / (1024 * 1024);
}

Json::Value
getMemoryInfo()
{
    struct sysinfo si;

    Json::Value result;

    if (sysinfo(&si) == -1)
        return result;

    long long totalPhysMem = si.totalram;
    totalPhysMem *= si.mem_unit;

    long long physMemUsed = si.totalram - si.freeram;
    physMemUsed *= si.mem_unit;

    long long physMemFree = si.freeram;
    physMemFree *= si.mem_unit;

    long long totalSwap = si.totalswap;
    totalSwap *= si.mem_unit;

    long long freeSwap = si.freeswap;
    totalSwap *= si.mem_unit;

    long long usedSwap = si.totalswap - si.freeswap;
    usedSwap *= si.mem_unit;

    double usedPercent = (double(physMemUsed) / double(totalPhysMem)) * 100.0;

    double swapUsedPercent = (double(usedSwap) / double(totalSwap)) * 100.0;

    result["used_percent"] = floatDecimalPoint(usedPercent);
    result["used_mb"] = inMegabyte(physMemUsed);
    result["total_mb"] = inMegabyte(totalPhysMem);
    result["free_mb"] = inMegabyte(physMemFree);
    result["swap_used_mb"] = inMegabyte(usedSwap);
    result["swap_free_mb"] = inMegabyte(freeSwap);
    result["swap_total_mb"] = inMegabyte(totalSwap);
    result["swap_used_percent"] = floatDecimalPoint(swapUsedPercent);

    return result;
};

}  // namespace memory
}  // namespace collectors

