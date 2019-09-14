#ifndef __HTTPCLIENT_HPP__
#define __HTTPCLIENT_HPP__


#include "WebResponse.hpp"

#include <string>
#include <memory>

#include <boost/asio/ip/tcp.hpp>

namespace web {

    class HttpClient {
    public:
        HttpClient(const std::string & host, const std::string & port);

        virtual WebResponse get(const std::string & path);

    protected:
        const std::string host;
        const std::string port;
        boost::asio::io_context ioc;
        boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> resolvedHost;
    };
}

#endif