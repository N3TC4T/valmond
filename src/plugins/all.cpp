#include "all.hpp"

// Plugins
#include "cpu.hpp"
#include "disk.hpp"
#include "host.hpp"
#include "load.hpp"
#include "memory.hpp"
#include "network.hpp"
#include "validator.hpp"

// Checks
#include "checks/process.hpp"

namespace plugins {
namespace all {

Json::Value
collectSystemData()
{
    Json::Value sysReport;

    auto const diskInfo = plugins::disk::getDiskInfo();
    auto const memInfo = plugins::memory::getMemoryInfo();
    auto const cpuInfo = plugins::cpu::getCpuInfo();
    auto const networkInfo = plugins::network::getNetworkInfo();
    auto const loadAvg = plugins::load::getLoadAvg();
    std::string const uptime = plugins::host::getUptime();

    // system
    sysReport["cpu"] = cpuInfo;
    sysReport["network"] = networkInfo;
    sysReport["disk"] = diskInfo;
    sysReport["memory"] = memInfo;
    sysReport["loadavg"] = loadAvg;
    sysReport["uptime"] = uptime;

    return sysReport;
}

Json::Value
collectHostData()
{
    auto const hostInfo = plugins::host::getHostInfo();

    return hostInfo
}

Json::Value
collectValidatorData()
{
    auto const validatorInfo = plugins::validator::getValidatorInfo();
    return validatorInfo
}

Json::Value
collectChecksData()
{
    Json::Value checksReport;

    // checks
    auto processCheck = checks::process::runProcessCheck();

    // append result
    checksReport.append(processCheck);

    return checksReport;
}

Json::Value
collectAllData()
{
    Json::Value report;

    auto const sysReport = collectSystemData();
    auto const hostReport = collectHostData();
    auto const validatorReport = collectValidatorData();
    auto const checksReport = collectChecksData();

    report["system"] = sysReport;
    report["host"] = hostReport;
    report["validator"] = validatorReport;
    report["checks"] = checksReport;

    return report;
}

}  // namespace all
}  // namespace plugins
