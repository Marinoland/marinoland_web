#ifndef __HTTPSCLIENT_HPP__
#define __HTTPSCLIENT_HPP__

#include "WebResponse.hpp"
#include "HttpClient.hpp"

#include <string>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

namespace web {

    class HttpsClient : public HttpClient {
    public:
        HttpsClient(const std::string & host, const std::string & port);

        WebResponse get(const std::string & path);

    private:
        std::shared_ptr<boost::asio::ssl::context> ctx;
    };
};

#endif