
#include "web/WebResponse.hpp"
#include <iostream>

using namespace std;

namespace web {

    WebResponse::WebResponse(boost::beast::http::response<boost::beast::http::dynamic_body> res)
    {
        for(auto & field : res) {
            string key(field.name_string());
            string val(field.value().data());
            val.erase(val.find("\r"));
            values[key] = val;
        }
        code = res.result_int();
        body = boost::beast::buffers_to_string(res.body().data());
    }

    WebResponse::WebResponse(const int code, const char *body)
    {
        this->code = code;
        this->body = body;
    }

    WebResponse::WebResponse(const WebResponse & cp)
    {
        code = cp.code;
        body = cp.body;
        values = cp.values;
    }

    int WebResponse::getCode()
    {
        return code;
    }
    string WebResponse::getBody()
    {
        return body;
    }

    string WebResponse::operator[] (string key)
    {
        return values[key];
    }
}