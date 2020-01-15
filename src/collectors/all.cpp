#include "all.hpp"

#include <functional>
#include <future>
#include <thread>

// Collectors
#include "cpu.hpp"
#include "disk.hpp"
#include "host.hpp"
#include "load.hpp"
#include "memory.hpp"
#include "network.hpp"
#include "validator.hpp"

// Checks
#include "checks/process.hpp"
#include "checks/validation.hpp"

using namespace collectors;

namespace collectors {
namespace all {

void
collectData(std::function<Json::Value()> collector, std::promise<Json::Value>* promiseObject)
{
    Json::Value report = collector();
    promiseObject->set_value(report);
}

Json::Value
collectSystemData()
{
    std::vector<std::thread> threads;
    Json::Value sysReport;

    // disk report
    std::promise<Json::Value> promiseDiskObj;
    std::future<Json::Value> diskInfo = promiseDiskObj.get_future();
    std::thread t1(collectData, disk::getDiskInfo, &promiseDiskObj);
    threads.push_back(std::move(t1));

    // memory report
    std::promise<Json::Value> promiseMemoryObj;
    std::future<Json::Value> memInfo = promiseMemoryObj.get_future();
    std::thread t2(collectData, memory::getMemoryInfo, &promiseMemoryObj);
    threads.push_back(std::move(t2));

    // cpu report
    std::promise<Json::Value> promiseCpuObj;
    std::future<Json::Value> cpuInfo = promiseCpuObj.get_future();
    std::thread t3(collectData, cpu::getCpuInfo, &promiseCpuObj);
    threads.push_back(std::move(t3));

    // network report
    std::promise<Json::Value> promiseNetworkObj;
    std::future<Json::Value> networkInfo = promiseNetworkObj.get_future();
    std::thread t4(collectData, network::getNetworkInfo, &promiseNetworkObj);
    threads.push_back(std::move(t4));

    // loadAvg report
    std::promise<Json::Value> promiseLoadObj;
    std::future<Json::Value> loadAvg = promiseLoadObj.get_future();
    std::thread t5(collectData, load::getLoadAvg, &promiseLoadObj);
    threads.push_back(std::move(t5));

    // uptime report
    std::string const uptime = host::getUptime();

    // system
    sysReport["cpu"] = cpuInfo.get();
    sysReport["network"] = networkInfo.get();
    sysReport["disk"] = diskInfo.get();
    sysReport["memory"] = memInfo.get();
    sysReport["loadavg"] = loadAvg.get();
    sysReport["uptime"] = uptime;

    // wait all jobs to be done
    // Iterate over the thread vector
    for (std::thread& th : threads)
    {
        // If thread Object is Joinable then Join that thread.
        if (th.joinable())
            th.join();
    }

    return sysReport;
}

Json::Value
collectHostData()
{
    auto const hostInfo = host::getHostInfo();

    return hostInfo;
}

Json::Value
collectValidatorData()
{
    std::promise<Json::Value> promiseValidatorObj;
    std::future<Json::Value> validatorInfo = promiseValidatorObj.get_future();
    std::thread gatterThread(collectData, validator::getValidatorInfo, &promiseValidatorObj);

    auto const validatorReport = validatorInfo.get();

    gatterThread.join();

    return validatorReport;
}

Json::Value
collectChecksData()
{
    std::vector<std::thread> threads;
    Json::Value checksReport;

    // checks
    auto processCheck = checks::process::runProcessCheck();
    auto validationCheck = checks::validation::runValidationCheck();

    // append result
    checksReport.append(processCheck);

    for (auto const& c : validationCheck)
    {
        checksReport.append(c);
    }

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
}  // namespace collectors
