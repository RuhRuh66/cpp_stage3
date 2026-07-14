#include <curl/curl.h>
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>

namespace {

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

CURLcode http_get(const std::string& url, long& status_code, std::string& data) {
    status_code = 0;
    data.clear();

    CURL* handle = curl_easy_init();
    if (handle == nullptr){
        return CURLE_FAILED_INIT;
    }

    CURLcode result = curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

    if (result == CURLE_OK) {
        result = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, save_to_data);
    }
    if (result == CURLE_OK) {
        result = curl_easy_setopt(handle, CURLOPT_WRITEDATA, &data);
    }
    if (result == CURLE_OK) {
        result = curl_easy_perform(handle);
    }

    if (result == CURLE_OK) {
        result = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &status_code);
    }

    curl_easy_cleanup(handle);

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
    long status_code = 0;
    std::string body;
    std::string url = "https://jsonplaceholder.typicode.com/posts/1";

    CURLcode result = http_get(url, status_code, body);
    if (result != CURLE_OK) {
        std::cerr << "curl failed: " << curl_easy_strerror(result) << '\n';
        return 1;
    }

    nlohmann::json json_data = nlohmann::json::parse(body);
    std::cout << "HTTP status: " << status_code << '\n';
    std::cout << "received: " << body.size() << " bytes\n";
    std::cout << "id: " << json_data["id"].get<int>() << '\n';
    std::cout << "title: " << json_data["title"].get<std::string>() << '\n';
    
    return 0;


}