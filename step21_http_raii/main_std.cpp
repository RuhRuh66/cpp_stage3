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

    auto* body = static_cast<std::string*>(userdata);
    body->append(ptr, total_size);

    return total_size;
}

class CurlGlobal {
public:
    CurlGlobal()
        : result_(curl_global_init(CURL_GLOBAL_DEFAULT))
    {
    }

    ~CurlGlobal()
    {
        if (result_ == CURLE_OK) {
            curl_global_cleanup();
        }
    }

    bool ok() const
    {
        return result_ == CURLE_OK;
    }

    CURLcode result() const
    {
        return result_;
    }

    CurlGlobal(const CurlGlobal&) = delete;
    CurlGlobal& operator=(const CurlGlobal&) = delete;

private:
    CURLcode result_;
};

CURLcode get_http(
    const std::string& url,
    long& status_code,
    std::string& body
)
{
    status_code = 0;
    body.clear();

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
    CurlGlobal curl_global;

    if (!curl_global.ok()) {
        std::cerr << "curl_global_init failed: "
                  << curl_easy_strerror(curl_global.result())
                  << '\n';
        return 1;
    }

    long status_code = 0;
    std::string body;
    std::string url = "https://jsonplaceholder.typicode.com/posts/1";

    CURLcode result = get_http(url, status_code, body);

    if (result != CURLE_OK) {
        std::cerr << "curl failed: "
                  << curl_easy_strerror(result)
                  << '\n';
        return 1;
    }

    nlohmann::json result_json = nlohmann::json::parse(body);

    std::cout << "HTTP status: " << status_code << '\n';
    std::cout << "received: " << body.size() << " bytes\n";
    std::cout << "id: "
              << result_json["id"].get<int>()
              << '\n';
    std::cout << "title: "
              << result_json["title"].get<std::string>()
              << '\n';

    return 0;
}