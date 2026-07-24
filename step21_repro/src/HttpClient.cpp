#include "HttpClient.hpp"
#include <curl/curl.h>
#include <curl/easy.h>

namespace{
std::size_t save_to_data(
    char* ptr,
    std::size_t size,
    std::size_t nmemb,
    void* userdata
){
    std::size_t total_size = size * nmemb;
    auto* body = static_cast<std::string*>(userdata);
    body->append(ptr, total_size);
    return total_size;
}

class CurlGlobal {
private: 
    CURLcode result_;
public:
    CurlGlobal(): result_(curl_global_init(CURL_GLOBAL_DEFAULT)){}
    ~CurlGlobal() {
        if (result_ == CURLE_OK) {
            curl_global_cleanup();
        }
    }

    bool is_ok() const {
        return result_ == CURLE_OK;
    }

    CurlGlobal(const CurlGlobal&) = delete;
    CurlGlobal& operator=(const CurlGlobal&) = delete;

};


class CurlEasy{
private:
    CURL* handle_;

public:
    CurlEasy():handle_(curl_easy_init()){};
    ~CurlEasy(){
        if (handle_!= nullptr) {
            curl_easy_cleanup(handle_);
        }
    }
    bool is_ok() const {
        return handle_ != nullptr;
    }

    CURL* get() const {
        return handle_;
    }

    CurlEasy(const CurlEasy&) = delete;
    CurlEasy& operator=(const CurlEasy&) = delete;
};

};



CURLcode Http_get(const std::string& url, HttpResponse& response){
    static CurlGlobal global;
    if (!global.is_ok()) {
        return CURLE_FAILED_INIT;
    }

    CurlEasy easy;
    if (easy.is_ok() == false) {
        return CURLE_FAILED_INIT;
    }

    CURL* handle = easy.get();
    response.status_code = 0;
    response.data.clear();

    CURLcode result = curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    if (result == CURLE_OK) {
        result = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, save_to_data);
    }
    if (result == CURLE_OK) {
        result = curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response.data);
    }
    
    if (result == CURLE_OK) {
        result = curl_easy_perform(handle);
    }

    if (result == CURLE_OK) {
        result = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response.status_code);
    }

    return result;

}