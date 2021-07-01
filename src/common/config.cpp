#include "config.hpp"

#include <fstream>
#include <memory>
#include "crypto/base64.hpp"

bool Config::instanceFlag = false;
Config* Config::instance = NULL;

const std::string HEARTBEAT_INTERVAL = "heartbeat_interval";
const std::string SERVER_URL = "server_url";
const std::string VALIDATOR_CONFIG = "validator_config";
const std::string VALIDATOR_TOKEN_SECTION = "validator_token";
const std::string VALIDATOR_RPC_SECTION = "port_rpc_admin_local";
const std::string VALIDATOR_RPC_PORT = "port";
const std::string VALIDATOR_RPC_IP = "ip";

std::string
delSpaces(std::string& str)
{
    int size = str.length();
    for (int j = 0; j <= size; j++)
    {
        for (int i = 0; i <= j; i++)
        {
            if (str[i] == ' ' && str[i + 1] == ' ')
            {
                str.erase(str.begin() + i);
            }
            else if (str[0] == ' ')
            {
                str.erase(str.begin());
            }
            else if (str[i] == '\0' && str[i - 1] == ' ')
            {
                str.erase(str.end() - 1);
            }
        }
    }
    return str;
}

Config::Config(std::string cfgFile)
{
    configFile = cfgFile;
    heartbeat_interval = 5;
    server_url = "";
    validator_rpc_port = "5005";
    validator_rpc_ip = "127.0.0.1";
    validation_secret_key = "";

    std::ifstream configFileStream{cfgFile};
    if (!configFileStream)
        throw std::runtime_error("cannot open config file " + cfgFile);

    std::string line;
    while (std::getline(configFileStream, line))
    {
        if (line.empty() || line[0] == '#')
        {
            // Blank line or comment, do nothing.
        }
        else
        {
            int delimeterPostion = line.find("=");
            std::string propertyName = line.substr(0, delimeterPostion);
            std::string propertyValue = line.substr(delimeterPostion + 1);

            // remove spaces
            propertyName = delSpaces(propertyName);
            propertyValue = delSpaces(propertyValue);

            if (propertyName.compare(HEARTBEAT_INTERVAL) == 0)
            {
                heartbeat_interval = atoi(propertyValue.c_str());
            }
            else if (propertyName.compare(SERVER_URL) == 0)
            {
                server_url = propertyValue;
            }
            else if (propertyName.compare(VALIDATOR_CONFIG) == 0)
            {
                validator_config_path = propertyValue;
            }
        }
    }
    configFileStream.close();

    // validate the config properties
    validateProperties();

    // get info from validator config
    std::ifstream validatorConfigFileStream{validator_config_path};
    if (!validatorConfigFileStream)
        throw std::runtime_error("cannot open validator config file " + validator_config_path);

    std::string strSection;
    std::stringstream validatorToken;

    std::string strValue;
    while (std::getline(validatorConfigFileStream, strValue))
    {
        if (strValue.empty() || strValue[0] == '#')
        {
            // Blank line or comment, do nothing.
        }
        else if (strValue[0] == '[' && strValue[strValue.length() - 1] == ']')
        {
            // New Section.
            strSection = strValue.substr(1, strValue.length() - 2);
        }
        else
        {
            // Another line for Section.
            if (!strValue.empty())
            {
                if (strSection == VALIDATOR_TOKEN_SECTION)
                {
                    validatorToken << strValue;
                }
                else if (strSection == VALIDATOR_RPC_SECTION)
                {
                    int delimeterPostion = strValue.find("=");
                    std::string propertyName = strValue.substr(0, delimeterPostion);
                    std::string propertyValue = strValue.substr(delimeterPostion + 1);

                    // remove spaces
                    propertyName = delSpaces(propertyName);
                    propertyValue = delSpaces(propertyValue);

                    if (propertyName.compare(VALIDATOR_RPC_IP) == 0)
                    {
                        validator_rpc_ip = propertyValue;
                    }
                    else if (propertyName.compare(VALIDATOR_RPC_PORT) == 0)
                    {
                        validator_rpc_port = propertyValue;
                    }
                }
            }
        }
    }

    validatorConfigFileStream.close();

    // decode validator_token
    Json::Value jsonMainfest;
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader { builder.newCharReader() };
    std::string errs;

    std::string decoded = crypto::base64_decode(validatorToken.str());

    if (reader->parse(decoded.c_str(), decoded.c_str() + decoded.size(), &jsonMainfest, &errs))
    {
        validation_secret_key = jsonMainfest["validation_secret_key"].asString();
    }
    else
    {
        throw std::runtime_error("Cannot load validation secret key from validator config.");
    }
}

Config::~Config()
{
    instanceFlag = false;
}

Config*
Config::getInstance(std::string cfgFile)
{
    if (!instanceFlag)
    {
        instance = new Config(cfgFile);
        instanceFlag = true;
    }

    return instance;
}

Config*
Config::getInstance()
{
    return Config::getInstance("");
}

void
Config::validateProperties()
{
    // check mendatory properties
    if (server_url.size() == 0 || validator_config_path.size() == 0)
    {
        throw std::invalid_argument("Missing required properties!");
    }

    // TODO: validate server url
    // TODO: validate heartbeat Interval
}

int
Config::getHeartbeatInterval()
{
    return heartbeat_interval;
}

std::string
Config::getServerURL()
{
    return server_url;
}

std::string
Config::getValidatorConfigPath()
{
    return validator_config_path;
}

std::string
Config::getValidatorRpcUrl()
{
    return validator_rpc_ip + ":" + validator_rpc_port;
}

std::string
Config::getValidationSecret()
{
    return validation_secret_key;
}
