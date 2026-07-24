#pragma once
#include <string>
#include <curl/curl.h>




struct HttpResponse{
    long status_code = 0;
    std::string data;
};

CURLcode Http_get(
    const std::string& url,
    HttpResponse& response
);

struct Post {
    int user_id = 0;
    int id = 0;
    std::string title;
    std::string body;
};

struct User {
    int id = 0;
    std::string name;
    std::string email;
};