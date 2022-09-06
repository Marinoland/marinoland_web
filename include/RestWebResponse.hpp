#ifndef __RESTWEBRESPONSE_HPP__
#define __RESTWEBRESPONSE_HPP__

#include "WebResponse.hpp"
#include <Json.hpp>

namespace web {
    class RestWebResponse : public WebResponse {

    public:
        RestWebResponse(const WebResponse & cp);
        json::nodeptr getJson();
    };
}

#endif