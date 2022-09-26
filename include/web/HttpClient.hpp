#pragma once

#include "web/WebResponse.hpp"

#include <string>
#include <memory>

#include <boost/asio/ip/tcp.hpp>
#include <functional>

namespace web {

    using namespace std;

    class HttpClient {
    public:
        HttpClient(const string & host, const string & port);

        void get(const string & path, const map<string, string> header,
            function<void(WebResponse &)> done);
        void post(const string & path, const map<string, string> header, const string body,
            function<void(WebResponse &)> done);
        void del(const string & path, const map<string, string> header,
            function<void(WebResponse &)> done);

        virtual void request(string url,
            boost::beast::http::verb method,
            const map<string, string>& header,
            const string body,
            function<void(WebResponse &)> done);

    protected:
        HttpClient() = default;
        void resolve(const string & host, const string & port);

        string host;
        string port;
        boost::asio::io_context ioc;
        boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> resolvedHost;
    };
}