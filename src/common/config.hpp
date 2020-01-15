#pragma once

#include <json/json.h>

class Config
{
public:
    static Config*
    getInstance(std::string configFile);
    static Config*
    getInstance();

    int
    getHeartbeatInterval();
    std::string
    getServerURL();
    std::string
    getRippledConfigPath();
    std::string
    getRippledRpcUrl();
    std::string
    getValidationSecret();

private:
    static bool instanceFlag;
    static Config* instance;

    int heartbeat_interval;
    std::string server_url;
    std::string rippled_config_path;
    std::string configFile;
    std::string validation_secret_key;
    std::string rippled_rpc_port;
    std::string rippled_rpc_ip;

    void
    validateProperties();

    Config(std::string cfgFile);
    ~Config();
};
