#include <curl/curl.h>
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>

#include "HttpClient.hpp"





int main() {



 
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

    for (int i = 0; i < 3 && i < json_data.size(); ++i) {
        const nlohmann::json & post = json_data[i];
        std::cout << "id: " << post["id"].get<int>() << '\n';
        std::cout << "title: " << post["title"].get<std::string>() << '\n';

    }
    

    
    return 0;


}