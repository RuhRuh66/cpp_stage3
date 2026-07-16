#pragma once

#include <string>
#include <curl/curl.h>

struct HttpResponse {
    long status_code = 0;
    std::string body;
};

CURLcode http_get(
    const std::string& url,
    HttpResponse& response
);

