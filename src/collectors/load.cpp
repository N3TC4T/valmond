
#include "load.hpp"

#include <cmath>
#include <sys/sysinfo.h>

#include "common/utils.hpp"

namespace collectors {
namespace load {
Json::Value
getLoadAvg()
{
    struct sysinfo si;

    Json::Value result;

    if (sysinfo(&si) == -1)
        return result;

    float f_load = 1.f / (1 << SI_LOAD_SHIFT);

    result["minute"] = floatDecimalPoint(si.loads[0] * f_load);
    result["five_minutes"] = floatDecimalPoint(si.loads[1] * f_load);
    result["fifteen_minutes"] = floatDecimalPoint(si.loads[2] * f_load);
    result["cores"] = get_nprocs();

    return result;
};

}  // namespace load
}  // namespace collectors

