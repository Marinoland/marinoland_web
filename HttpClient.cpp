#include "HttpClient.hpp"

#include <cstdlib>
#include <iostream>

#include <boost/asio/connect.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/core.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
using namespace std;

namespace web {
    HttpClient::HttpClient(
            const string & host,
            const string & port) :
        host(host),
        port(port)
    {
        boost::asio::ip::tcp::resolver resolver(ioc);
        resolvedHost = resolver.resolve(host, port);
    }

    WebResponse HttpClient::get(const string & path, const map<string, string> header)
    {
        beast::tcp_stream stream(ioc);
        stream.connect(resolvedHost);
        http::request<http::string_body> req{
            http::verb::get, path, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        for(pair<string, string> entry : header) {
            req.set(entry.first, entry.second);
        }
        http::write(stream, req);
        beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(stream, buffer, res);
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);
        WebResponse response(res);
        return response;
    }

    WebResponse HttpClient::post(const string & path, const map<string, string> header, const string body)
    {
        beast::tcp_stream stream(ioc);
        stream.connect(resolvedHost);
        http::request<http::string_body> req{
            http::verb::post, path, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        std::string contentLength;
        contentLength += body.size();
        req.set("Content-Length", contentLength);
        for(pair<string, string> entry : header) {
            req.set(entry.first, entry.second);
        }
        req.body() = body;
        http::write(stream, req);
        beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(stream, buffer, res);
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);
        WebResponse response(res);
        return response;
    }
}
