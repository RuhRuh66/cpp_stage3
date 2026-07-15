#include <curl/curl.h>
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>

namespace {

struct HttpResponse {
    long status_code = 0;
    std::string body;
};

std::size_t save_to_data(
    char* ptr,
    size_t size,
    size_t nmemb,
    void* userdata
) {
    size_t total_size = size * nmemb;
    auto* body = static_cast<std::string*>(userdata);
    body->append(ptr, total_size);
    return total_size;
}

class CurlEasy{
private:
    CURL* handle_;
public:
    CurlEasy():handle_(curl_easy_init()) {};

    ~CurlEasy() {
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



CURLcode http_get(const std::string& url, HttpResponse& response) {
    response.status_code = 0;
    response.body.clear();

    CurlEasy easy;
    CURL* handle = easy.get();

    if (handle == nullptr){
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

class CurlGlobal{
private:
    CURLcode result_;

public:
    CurlGlobal(): result_(curl_global_init(CURL_GLOBAL_DEFAULT)){};

    ~CurlGlobal(){
        if (result_ == CURLE_OK){
            curl_global_cleanup();
        }
    }

    bool is_ok() const {
        return result_ == CURLE_OK;
    }

    CURLcode result() const {
        return result_;
    }

    CurlGlobal(const CurlGlobal&) = delete;
    CurlGlobal& operator=(const CurlGlobal&) = delete;

};

}

int main() {
    CurlGlobal curl_global;

    if (!curl_global.is_ok()){
        std::cerr << "curl_global_init failed: " << curl_easy_strerror(curl_global.result()) << '\n';
        return 1;
    }
 
    std::string url = "https://jsonplaceholder.typicode.com/posts";
    HttpResponse response;

    CURLcode result = http_get(url, response);
    if (result != CURLE_OK) {
        std::cerr << "curl failed: " << curl_easy_strerror(result) << '\n';
        return 1;
    }

    nlohmann::json json_data = nlohmann::json::parse(response.body);
    if (!json_data.is_array()) {
        std::cerr << "JSON is not an array\n";
        return 1;
    }
    std::cout << "HTTP status: " << response.status_code << '\n';
    std::cout << "received: " << response.body.size() << " bytes\n";
    
    std::cout << "post count: " << json_data.size() << '\n';

    

    
    return 0;


}