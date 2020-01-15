#include "validator.hpp"

#include <algorithm>

#include "common/http.hpp"

namespace collectors {
namespace validator {

// rpc method
// this may fail so should be another method as well
Json::Value
getFromRPC()
{
    // rpc url
    const std::string url("127.0.0.1:5005");

    Json::Value result;
    Http::Response httpResponse;

    // make an http instance
    Http http(url);
    httpResponse = http.GET();

    if (httpResponse.success == true)
    {
        result = httpResponse.data["result"]["info"];
    }

    return result;
}

Json::Value
getFromCrawler()
{
    // crawl url
    const std::string url("https://127.0.0.1:51235/crawl");

    Json::Value result;
    Http::Response httpResponse;

    // make an http instance
    Http http(url);
    httpResponse = http.GET();

    if (httpResponse.success == true)
    {
        result = httpResponse.data["server"];
    }

    return result;
}

Json::Value
getServerInfo()
{
    // get from rpc
    auto info = getFromRPC();
    // if rpc method is not success get from crawl
    if (info.empty())
    {
        info = getFromCrawler();
    }

    return info;
}

Json::Value
getValidatorInfo()
{
    Json::Value result;
    // get from rpc
    auto info = getFromRPC();
    // if rpc method is not success get from crawl
    if (info.empty())
    {
        info = getFromCrawler();
    }

    // set required keys to report
    std::string required_keys[]{"build_version",
                                "time",
                                "uptime",
                                "complete_ledgers",
                                "pubkey_node",
                                "pubkey_validator",
                                "server_state",
                                "server_state_duration_us"};

    for (auto const& key : info.getMemberNames())
    {
        bool exists = std::find(std::begin(required_keys), std::end(required_keys), key) !=
            std::end(required_keys);
        if (exists)
        {
            result[key] = info[key].asString();
        }
    }

    return result;
};

}  // namespace validator
}  // namespace collectors

