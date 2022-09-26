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

        virtual void request(string url,
            boost::beast::http::verb method,
            const map<string, string>& header,
            const string body,
            function<void(WebResponse &)> done);

    private:
        std::shared_ptr<boost::asio::ssl::context> ctx;
    };
};