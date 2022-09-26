#pragma once

#include "web/WebResponse.hpp"
#include "web/HttpClient.hpp"

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

        WebResponse get(const std::string & path, const std::map<std::string, std::string> header);
        WebResponse post(const std::string & path, const std::map<std::string, std::string> header, const std::string body);
        WebResponse del(const std::string & path, const std::map<std::string, std::string> header);

    private:
        std::shared_ptr<boost::asio::ssl::context> ctx;
    };
};