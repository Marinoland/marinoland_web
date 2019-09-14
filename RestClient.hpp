#ifndef __RESTCLIENT_HPP__
#define __RESTCLIENT_HPP__

#include <string>
#include "RestWebResponse.hpp"

namespace web {

    class RestClient {
    public:
        void get(std::string url, std::function<void(RestWebResponse & response)> f);
    };
}

#endif