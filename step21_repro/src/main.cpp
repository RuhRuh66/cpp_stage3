#include "HttpClient.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

namespace {

std::vector<User> parse_users(const std::string& json_text){
    std::vector<User> users;
    nlohmann::json user_json = nlohmann::json::parse(json_text);
    for (const nlohmann::json& data: user_json) {
        User user;
        user.id = data["id"].get<int>();
        user.name = data["name"].get<std::string>();
        user.email = data["email"].get<std::string>();

        users.push_back(user);
    }
    return users;
}

std::vector<Post> parse_posts(const std::string& json_text) {
    nlohmann::json post_data = nlohmann::json::parse(json_text);
    std::vector<Post> posts;
    for (const nlohmann::json& data: post_data){
        Post post;
        post.user_id = data["userId"].get<int>();
        post.id = data["id"].get<int>();
        post.title = data["title"].get<std::string>();
        post.body = data["body"].get<std::string>();

        posts.push_back(post);
    }
    return posts;
}

}

int main() {
    HttpResponse user_response;
    HttpResponse posts_response;

    std::string user_url = "https://jsonplaceholder.typicode.com/users";

    std::string posts_url = "https://jsonplaceholder.typicode.com/posts?userId=1";

    CURLcode result = Http_get(user_url, user_response);
    if (result != CURLE_OK){
        std::cerr << "curl failed\n";
        return 1;
    }
    
    result = Http_get(posts_url, posts_response);
    if (result != CURLE_OK){
        std::cerr << "curl failed\n";
        return 1;
    }
   

    std::vector<User> users = parse_users(user_response.data);
    std::vector<Post> posts = parse_posts(posts_response.data);

 

  std::cout << '\n';

  int target_user_id = 1;
  bool target_found = false;

  for (const User& user: users){
    if (user.id != target_user_id){
        continue;
    }

    target_found = true;

    std::cout << "user: " << user.name << '\n';
    std::cout << "email: " << user.email << '\n';
    std::cout << "posts:\n";

    std::size_t post_count = 0;

    for (const Post& post: posts) {
        if (post.user_id == user.id) {
            std::cout << post.id << ": " << post.title << '\n';
        }
        ++post_count;
    }

    std::cout << "post count: " << post_count << '\n';
    break;

  
  if (!target_found) {
    std::cerr << "user not found\n";
    return 1;
  }
}
    return 0;
}