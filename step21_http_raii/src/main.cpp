#include <curl/curl.h>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

std::size_t save_to_data(
    char* ptr,
    std::size_t size,
    std::size_t nmemb,
    void* userdata
) {
    std::size_t total_size = size * nmemb;
    auto* body = static_cast<std::string*>(userdata);
    body->append(ptr, total_size);

    return total_size;
}

CURLcode http_get(
    const std::string& url,
    long& status_code,
    std::string& body
){
    body.clear();
    status_code = 0;

    CURL* handle = curl_easy_init();

    if (handle == nullptr) {
        return CURLE_FAILED_INIT;
    }

    CURLcode result = curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

    if (result == CURLE_OK) {
        result = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, save_to_data);
    }

    if (result == CURLE_OK) {
        result = curl_easy_setopt(handle, CURLOPT_WRITEDATA, &body);
    }

    if (result == CURLE_OK) {
        result = curl_easy_perform(handle);
    }

    if (result == CURLE_OK) {
        result = curl_easy_getinfo(handle, CURLINFO_HTTP_CODE, &status_code);
    }

    curl_easy_cleanup(handle);

    return result;
  
}




int main(){
    CURLcode global_result = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_result != CURLE_OK) {
        std::cerr << "curl_global_init failed\n";
        return 1;
    }
    long status_code = 0;
    std::string body;

    CURLcode result = http_get("https://jsonplaceholder.typicode/posts/1", status_code, body);

    if (result != CURLE_OK) {
        std::cerr << "curl failed: " << curl_easy_strerror(result) << '\n';
        curl_global_cleanup();
        return 1;
    }


    if (result == CURLE_OK) {
        nlohmann::json result_json = nlohmann::json::parse(body);
        std::cout << "HTTP status: " << status_code << '\n';
        std::cout << "received: " << body.size() << " bytes" << '\n';
        std::cout << "id: " << result_json["id"] << '\n';
        std::cout << "title: " << std::string(result_json["title"]) << '\n';

    }
    curl_global_cleanup();

    return 1;


}