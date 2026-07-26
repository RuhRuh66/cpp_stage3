#include "HttpClient.hpp"



namespace {

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

class CurlEasy{
private:
    CURL* handle_;

public:
    CurlEasy():handle_(curl_easy_init()) {};
    ~CurlEasy(){
        if (handle_ != nullptr) {
            curl_easy_cleanup(handle_);
        }
    }
    bool is_ok() const{
        return handle_ != nullptr;
    }

    CURL* get() const {
        return handle_;
    }

    CurlEasy(const CurlEasy&) = delete;
    CurlEasy& operator=(const CurlEasy&) = delete;

};

class CurlGlobal{
private:
    CURLcode initialized_;

public:
    CurlGlobal(): initialized_(curl_global_init(CURL_GLOBAL_DEFAULT)){};
    ~CurlGlobal(){
        if (initialized_ == CURLE_OK) {
            curl_global_cleanup();
        }
    }

    bool is_ok() const {
        return initialized_== CURLE_OK;
    }
    CurlGlobal(const CurlGlobal&) = delete;
    CurlGlobal& operator=(const CurlGlobal&) = delete;
};

}

CURLcode http_get(const std::string& url, HttpResponse& response) {
    static CurlGlobal global;
    if (!global.is_ok()){
        return CURLE_FAILED_INIT;
    }

    response.status_code = 0;
    response.body.clear();

    CurlEasy easy;
    CURL* handle = easy.get();

    if (handle == nullptr) {
        return CURLE_FAILED_INIT;
    }

    CURLcode result = curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

    if (result == CURLE_OK) {
        result = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, save_to_data);
    }
    if (result == CURLE_OK) {
        result = curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response.body);

    }
    if (result == CURLE_OK) {
        result = curl_easy_perform(handle);
    }
    if (result == CURLE_OK) {
        result = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response.status_code);
    }
    
    return result;
}