#ifndef __WEBCLIENT_HPP__
#define __WEBCLIENT_HPP__

#include <string>
#include <map>
#include <functional>

#include "WebResponse.hpp"
//#include "HttpClient.hpp"
//#include "HttpsClient.hpp"
namespace web {

    class WebClient {
    public:
        void get(std::string url, std::map<std::string, std::string> header, std::function<void(WebResponse response)> f);
        void post(std::string url, std::map<std::string, std::string> header, std::string body, std::function<void(WebResponse response)> f);
    private:
    };
}

#endif