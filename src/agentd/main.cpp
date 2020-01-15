#include <iomanip>
#include <iostream>

// Agent
#include "agent.hpp"

// crypto
#include "crypto/signature.hpp"

// Utils
#include "common/daemon.hpp"

// version
#include "version.hpp"

const std::string USAGE_MESSAGE{
    "USAGE: ./agent [sign <content>] [-c/--config <config file path>] [-h / --help]"};
const std::string HELP_ARGUMENT = "help";
const std::string CONFIG_FILE_PATH_ARGUMENT = "configFilePath";
const std::string SIGN_MESSAGE_ARGUMENT = "signCommand";
const std::string DEFAULT_CONFIG_FILE = "/etc/opt/rippledagent/rippledagent.cfg";

void
startAgent(Config* config)
{
    // start the agent

    const auto start = std::time(nullptr);
    std::cout << "[!] Starting Agent version " << PROJECT_VER << " at "
              << std::put_time(std::localtime(&start), "%c %Z") << std::endl;
    std::cout << std::endl;

    // get an instance from Heartbeat
    const auto agent = Agent(config);
    const auto beater = std::bind(&Agent::beat, agent);

    // start the loop and daeamon
    Daemon daemon(config->getHeartbeatInterval(), beater);

    // run forever
    std::this_thread::sleep_until(
        std::chrono::system_clock::now() + std::chrono::hours(std::numeric_limits<int>::max()));
}

void
signContent(std::string content, Config* config)
{
    auto const secret = config->getValidationSecret();
    auto secretKey = crypto::SecretKey(secret);
    auto signature = crypto::Signature(secretKey, content);

    // convert signature to string
    std::stringstream sstream;
    sstream << signature;
    std::string signatureString = sstream.str();

    std::cout << signatureString << std::endl;
}

int
main(int argc, char** argv)
{
    std::map<std::string, std::string> arguments;
    std::string configFile = DEFAULT_CONFIG_FILE;
    std::string signMessage;

    try
    {
        int count = 1;  // First position skipping the program name
        while (count < argc)
        {
            std::string element = argv[count];
            if ((element.compare("-c") == 0 || element.compare("--config") == 0) &&
                (count + 2) <= argc)
            {
                arguments.insert(std::make_pair(CONFIG_FILE_PATH_ARGUMENT, argv[count + 1]));
                count = count + 2;
            }
            if (element.compare("sign") == 0 && (count + 2) <= argc)
            {
                arguments.insert(std::make_pair(SIGN_MESSAGE_ARGUMENT, argv[count + 1]));
                count = count + 2;
            }
            else
            {
                throw std::invalid_argument(USAGE_MESSAGE);
            }
        }

        if (arguments.count(CONFIG_FILE_PATH_ARGUMENT) == 1)
        {
            configFile = arguments[CONFIG_FILE_PATH_ARGUMENT];
        }

        if (arguments.count(SIGN_MESSAGE_ARGUMENT) == 1)
        {
            signMessage = arguments[SIGN_MESSAGE_ARGUMENT];
        }
    }
    catch (const std::invalid_argument& exception)
    {
        throw std::invalid_argument(USAGE_MESSAGE);
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        exit(-2);
    }

    // get config instance
    Config* config = Config::getInstance(configFile);

    if (signMessage.empty())
    {
        // if it's not sign start the agent
        startAgent(config);
    }
    else
    {
        // if it's sign requst, sign content
        signContent(signMessage, config);
    }

    return 0;
}
