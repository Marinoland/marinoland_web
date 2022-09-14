#pragma once

#include "web/WebResponse.hpp"
#include <json/Json.hpp>

namespace web {
    class RestWebResponse : public WebResponse {

    public:
        RestWebResponse(const WebResponse & cp);
        json::nodeptr getJson();
    };
}