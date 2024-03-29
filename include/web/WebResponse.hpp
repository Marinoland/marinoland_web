#pragma once

#include <string>
#include <map>
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include "web/ErrorCodes.hpp"

namespace web {

    class WebResponse {
    public:
        WebResponse(boost::beast::http::response<boost::beast::http::dynamic_body> res);
        WebResponse(const WebResponse & cp);
        WebResponse(const int code, const char *body);
        int getCode();
        std::string getBody();
        std::string operator[] (std::string key);
    private:
        int code;
        std::string body;
        std::map<std::string, std::string> values;
    };

};