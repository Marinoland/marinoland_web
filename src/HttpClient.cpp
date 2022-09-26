#include "web/HttpClient.hpp"

#include <cstdlib>
#include <iostream>
#include <sstream>

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
        resolve(host, port);
    }

    void HttpClient::resolve(const string & host, const string & port)
    {
        this->host = host;
        this->port = port;
        boost::asio::ip::tcp::resolver resolver(ioc);
        try
        {
            resolvedHost = resolver.resolve(host, port);
        }
        catch (boost::system::system_error err)
        {
            std::cerr << "ERROR: resolving " << host << ":" << port <<
                " " << err.what() << std::endl;
        }
    }

    void HttpClient::request(string url,
        boost::beast::http::verb method,
        const map<string, string> & header,
        const string body,
        function<void(WebResponse &)> done) {

        try {
            beast::tcp_stream stream(ioc);
            stream.connect(resolvedHost);
            http::request<http::string_body> req{method, url, 11};
            req.set(http::field::host, host);
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            std::stringstream contentLength;
            contentLength << body.size();
            req.set("Content-Length", contentLength.str());
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
            done(response);
        } catch (boost::system::system_error err) {
            std::stringstream msg;
            msg << "HTTP ERROR: requesting " << host << ":" << port <<
                " path: " << url << "\n" << err.code() << "\n" << err.what() << "\n" << err.code().message();
            std::cerr << msg.str() << std::endl;
            WebResponse res(
                convertBoostError(err.code().value()),
                msg.str().c_str()
            );
            done(res);
        }
    }

    void HttpClient::get(const string & path, const map<string, string> header,
            function<void(WebResponse &)> done) {
        request(path, http::verb::get, header, "", done);
    }

    void HttpClient::post(const string & path, const map<string, string> header, const string body,
            function<void(WebResponse &)> done) {
        request(path, http::verb::post, header, body, done);
    }

    void HttpClient::put(const string & path, const map<string, string> header, const string body,
            function<void(WebResponse &)> done) {
        request(path, http::verb::put, header, body, done);
    }

    void HttpClient::patch(const string & path, const map<string, string> header, const string body,
            function<void(WebResponse &)> done) {
        request(path, http::verb::patch, header, body, done);
    }

    void HttpClient::del(const string & path, const map<string, string> header,
            function<void(WebResponse &)> done) {
        request(path, http::verb::delete_, header, "", done);
    }

    void HttpClient::head(const string & path, const map<string, string> header,
            function<void(WebResponse &)> done) {
        request(path, http::verb::head, header, "", done);
    }

    void HttpClient::options(const string & path, const map<string, string> header,
            function<void(WebResponse &)> done) {
        request(path, http::verb::options, header, "", done);
    }
}
