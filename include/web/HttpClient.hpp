#pragma once

#include "web/WebResponse.hpp"

#include <string>
#include <memory>

#include <boost/asio/ip/tcp.hpp>
#include <functional>

namespace web {

    class HttpClient {
    public:
        HttpClient(const std::string & host, const std::string & port);

        virtual WebResponse get(const std::string & path, const std::map<std::string, std::string> header);
        virtual WebResponse post(const std::string & path, const std::map<std::string, std::string> header, const std::string body);
        virtual WebResponse del(const std::string & path, const std::map<std::string, std::string> header);

    protected:
        HttpClient() = default;
        void resolve(const std::string & host, const std::string & port);

        std::string host;
        std::string port;
        boost::asio::io_context ioc;
        boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> resolvedHost;
    };
}