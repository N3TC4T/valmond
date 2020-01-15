#include "http.hpp"

#include <iostream>

Http::Http(std::string host) : url(host), timeout(10)
{
}

Http::Response
Http::handleResponse(std::string* httpData, CURLcode res, long HttpCode)
{
    Http::Response response;

    std::stringstream error;
    bool success = false;

    Json::Value jsonData;
    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    std::string errs;

    if (HttpCode == 200 && res == CURLE_OK)
    {
        if (reader->parse(
                httpData->c_str(), httpData->c_str() + httpData->size(), &jsonData, &errs))
        {
            success = true;
        }
        else
        {
            success = false;
            error << "Could not parse HTTP data as JSON: " << httpData;
        }
    }
    else
    {
        success = false;

        // conection issue
        if (res != CURLE_OK)
        {
            const char* err = curl_easy_strerror(res);
            error << err;
        }
        else
        {
            // http issue
            error << "Response code is invalid: " << HttpCode;
        }
    }

    if (success == true)
    {
        response.success = true;
        response.data = jsonData;
    }
    else
    {
        response.success = false;
        response.error = error.str();
    }

    return response;
}

Http::Response
Http::POST(std::string data)
{
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");

    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.length());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    // timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

    long httpCode(0);
    CURLcode res;

    std::unique_ptr<std::string> httpData(new std::string());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    res = curl_easy_perform(curl);

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return handleResponse(httpData.get(), res, httpCode);
}

Http::Response
Http::GET()
{
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

    // timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

    long httpCode(0);
    CURLcode res;

    std::unique_ptr<std::string> httpData(new std::string());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    return handleResponse(httpData.get(), res, httpCode);
}
