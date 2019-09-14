#ifndef __WEBCLIENT_HPP__
#define __WEBCLIENT_HPP__

#include <string>
#include <functional>

#include "WebResponse.hpp"
//#include "HttpClient.hpp"
//#include "HttpsClient.hpp"
namespace web {

    class WebClient {
    public:
        void get(std::string url, std::function<void(WebResponse response)> f);
    private:
    };
}

#endif