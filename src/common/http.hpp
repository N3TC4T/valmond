#pragma once

#include <curl/curl.h>
#include <json/json.h>

class Http
{
public:
    struct Response
    {
        bool success;
        Json::Value data;
        std::string error;
    };

public:
    Http(std::string url);

    Response
    POST(std::string data);
    Response
    GET();

private:
    std::string url;  // endpoint url
    int timeout;      // request timeout

    static std::size_t
    callback(const char* in, std::size_t size, std::size_t num, std::string* out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    }

    Response
    handleResponse(std::string* httpData, CURLcode res, long HttpCode);
};
