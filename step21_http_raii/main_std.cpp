#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>

namespace {

std::size_t save_to_data(
    char* ptr,
    std::size_t size,
    std::size_t nmemb,
    void* userdata
)
{
    const std::size_t total_size = size * nmemb;

    auto* data = static_cast<std::string*>(userdata);
    data->append(ptr, total_size);

    return total_size;
}

CURLcode http_get(
    const std::string& url,
    std::string& body,
    long& status_code
)
{
    body.clear();
    status_code = 0;

    CURL* handle = curl_easy_init();

    if (handle == nullptr) {
        return CURLE_FAILED_INIT;
    }

    CURLcode result = curl_easy_setopt(
        handle,
        CURLOPT_URL,
        url.c_str()
    );

    if (result == CURLE_OK) {
        result = curl_easy_setopt(
            handle,
            CURLOPT_WRITEFUNCTION,
            save_to_data
        );
    }

    if (result == CURLE_OK) {
        result = curl_easy_setopt(
            handle,
            CURLOPT_WRITEDATA,
            &body
        );
    }

    if (result == CURLE_OK) {
        result = curl_easy_perform(handle);
    }

    if (result == CURLE_OK) {
        result = curl_easy_getinfo(
            handle,
            CURLINFO_RESPONSE_CODE,
            &status_code
        );
    }

    curl_easy_cleanup(handle);

    return result;
}

} // namespace

int main()
{
    CURLcode global_result = curl_global_init(CURL_GLOBAL_DEFAULT);

    if (global_result != CURLE_OK) {
        std::cerr << "curl_global_init failed\n";
        return 1;
    }

    std::string body;
    long status_code = 0;

    CURLcode result = http_get(
        "https://jsonplaceholder.typicode.com/posts/1",
        body,
        status_code
    );

    if (result != CURLE_OK) {
        std::cerr << "curl failed: "
                  << curl_easy_strerror(result)
                  << '\n';

        curl_global_cleanup();
        return 1;
    }

    nlohmann::json json_data = nlohmann::json::parse(body);

    std::cout << "HTTP status: " << status_code << '\n';
    std::cout << "received: " << body.size() << " bytes\n";
    std::cout << "id: "
              << json_data["id"].get<int>()
              << '\n';
    std::cout << "title: "
              << json_data["title"].get<std::string>()
              << '\n';

    curl_global_cleanup();

    return 0;
}