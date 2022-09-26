#pragma once

#include <map>
#include <string>
#include "web/RestWebResponse.hpp"

namespace web {

    class RestClient {
    public:
        void get(std::string url, std::map<std::string, std::string> header, std::function<void(RestWebResponse & response)> f);
        void post(std::string url, std::map<std::string, std::string> header, json::nodeptr body, std::function<void(RestWebResponse & response)> f);
        void del(std::string url, std::map<std::string, std::string> header, std::function<void(RestWebResponse & response)> f);
    };
}