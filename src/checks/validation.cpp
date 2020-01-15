#include "process.hpp"

#include "collectors/validator.hpp"

namespace checks {
namespace validation {

Json::Value
runValidationCheck()
{
    Json::Value result;

    auto const serverInfo = collectors::validator::getServerInfo();

    // server_state is full/validating/proposing (healthy), syncing/tracking/connected (warning),
    // or disconnected (critical)
    Json::Value serverStateCheck;
    serverStateCheck["command"] = "ServerState";

    if (serverInfo.empty())
    {
        serverStateCheck["exit_code"] = 2;
        serverStateCheck["output"] =
            "Check Server State CRITICAL: Cannot get server_info from rpc or crawl";
    }
    else
    {
        if (serverInfo["server_state"] == "full" || serverInfo["server_state"] == "validating" ||
            serverInfo["server_state"] == "proposing")
        {
            serverStateCheck["exit_code"] = 0;
            serverStateCheck["output"] =
                "Check Server State OK: Server state is in validation mode";
        }
        else if (
            serverInfo["server_state"] == "syncing" || serverInfo["server_state"] == "tracking" ||
            serverInfo["server_state"] == "connected")
        {
            serverStateCheck["exit_code"] = 1;
            serverStateCheck["output"] =
                "Check Server State WARNING: Server state is not in validation mode";
        }
        else
        {
            serverStateCheck["exit_code"] = 2;
            serverStateCheck["output"] =
                "Check Server State CRITICAL: Server state is in disconnected mode";
        }
    }

    // Age of last validated ledger is < 7s (healthy), <=20s (warning) or >20s (critical)
    Json::Value lastValidationCheck;
    lastValidationCheck["command"] = "AgeLastValidation";

    if (serverInfo.empty())
    {
        lastValidationCheck["exit_code"] = 2;
        lastValidationCheck["output"] =
            "Check Ledger age CRITICAL: Cannot get server_info from rpc or crawl";
    }
    else
    {
        if (serverInfo["validated_ledger"]["age"] < 7)
        {
            lastValidationCheck["exit_code"] = 0;
            lastValidationCheck["output"] =
                "Check Ledger age OK: Last validated ledger age is < 7s";
        }
        else if (serverInfo["validated_ledger"]["age"] <= 20)
        {
            lastValidationCheck["exit_code"] = 1;
            lastValidationCheck["output"] =
                "Check Ledger age WARNING: Last validated ledger age is <= 20s";
        }
        else
        {
            lastValidationCheck["exit_code"] = 2;
            lastValidationCheck["output"] =
                "Check Ledger age CRITICAL: Last validated ledger age is > 20s";
        }
    }

    // Number of peers is >7 (healthy) <5 (warning unless configured w/ peer_private),
    // or 0 (critical)
    Json::Value peersCheck;
    peersCheck["command"] = "PeersCount";

    if (serverInfo.empty())
    {
        peersCheck["exit_code"] = 2;
        peersCheck["output"] =
            "Check Peer count CRITICAL: Cannot get server_info from rpc or crawl";
    }
    else
    {
        if (serverInfo["peers"] > 7)
        {
            peersCheck["exit_code"] = 0;
            peersCheck["output"] = "Check Peer count OK: Peer count is greater than 7";
        }
        else if (serverInfo["peers"] < 5 && serverInfo["peers"] != 0)
        {
            peersCheck["exit_code"] = 1;
            peersCheck["output"] = "Check Peer count WARNING: Peer count is less than 5";
        }
        else
        {
            peersCheck["exit_code"] = 2;
            peersCheck["output"] = "Check Peer count CRITICAL: Peer count is zero.";
        }
    }

    // append result
    result.append(serverStateCheck);
    result.append(lastValidationCheck);
    result.append(peersCheck);

    return result;
}

}  // namespace validation
}  // namespace checks
