//
// Created by burso on 3/20/16.
//

#include <iostream>
#include <curl/curl.h>
#include "ssllabs/ssllabs.h"

static size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) {
    size_t realsize = size * nmemb;

    std::string * mem = (std::string *)up;
    mem->append(buf);

    return realsize;
}

SSLlabs::SSLlabs() {
    curl_global_init(CURL_GLOBAL_ALL);
}

SSLlabs::~SSLlabs() {
    curl_global_cleanup();
}

int SSLlabs::analyze(const std::string domain) {
    std::string url(SSLLABS_API_URL);
    std::string data = {};

    url += "/analyze?host=";
    url += domain;

    curl_read(url, data);
    std::cout << "url: " << url << std::endl;
    std::cout << data << std::endl;

    return 0;
}

int SSLlabs::getRootCertsRaw(const std::string &certs) {
    std::string url(SSLLABS_API_URL);

    url += "/getRootCertsRaw";
    curl_read(url, certs);

    return 0;
}

int SSLlabs::getInfoRaw(const std::string &info) {
    std::string url(SSLLABS_API_URL);

    url += "/info";
    curl_read(url, info);

    return 0;
}

int SSLlabs::getStatusCodesRaw(const std::string &codes) {
    std::string url(SSLLABS_API_URL);

    url += "/getStatusCodes";
    curl_read(url, codes);

    return 0;
}

int SSLlabs::curl_read(const std::string &url, const std::string &data, long timeout) {
    CURLcode code(CURLE_FAILED_INIT);
    CURL * curl = curl_easy_init();

    if(curl) {
        if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_USERAGENT, SSLLABS_AGENT))
           && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str()))) {

            if ((code = curl_easy_perform(curl)) != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(code) << "\n";
            }
        }

        curl_easy_cleanup(curl);
    }

    return code;
}