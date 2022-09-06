#include "HttpsClient.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>

#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>


namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = boost::asio::ip::tcp;
using namespace std;

namespace web
{

    void loadRootCerts(ssl::context& ctx)
    {
        boost::system::error_code ec;
        for(boost::filesystem::directory_entry & p :
                boost::filesystem::directory_iterator("rootcerts")) {
            ifstream certfile(p.path().c_str());
            string cert;

            while (!certfile.fail() && !certfile.eof()) {
                string line;
                getline(certfile, line);
                cert += line;
                cert += "\n";
            }
            ctx.add_certificate_authority(
                boost::asio::buffer(cert.data(), cert.size()), ec);
            if(ec)
                throw boost::system::system_error{ec};
        }
    }

    HttpsClient::HttpsClient(
            const string & host,
            const string & port)
    {
        ctx = make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12_client);
        ctx->set_default_verify_paths();
        loadRootCerts(*ctx);
        ctx->set_verify_mode(ssl::verify_peer);

        resolve(host, port);
    }


    WebResponse HttpsClient::get(const string & path, const map<string, string> header)
    {
        beast::ssl_stream<beast::tcp_stream> stream(ioc, *ctx);
        // Set SNI Hostname (many hosts need this to handshake successfully)
        
        if(! SSL_set_tlsext_host_name(stream.native_handle(), host.c_str()))
        {
            beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
            std::stringstream msg;
            msg << "ERROR contacting host: " << host << " path: " << path << "\n" << ec.message();
            std::cerr << msg.str() << std::endl;
            WebResponse res(-1, msg.str().c_str());
            return res;
        } else {
            try
            {
                beast::get_lowest_layer(stream).connect(resolvedHost);
                stream.handshake(ssl::stream_base::client);

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
                stream.shutdown(ec);
                WebResponse response(res);
                return response;
            }
            catch (boost::system::system_error err)
            {
                std::stringstream msg;
                msg << "ERROR: getting " << host << ":" << port <<
                    " path: " << path << "\n" << err.code() << "\n" << err.what() << "\n" << err.code().message();
                std::cerr << msg.str() << std::endl;
                WebResponse res(
                    convertBoostError(err.code().value()),
                    msg.str().c_str()
                );
                return res;
            }
        }
    }

    WebResponse HttpsClient::post(const string & path, const map<string, string> header, const string body)
    {
        beast::ssl_stream<beast::tcp_stream> stream(ioc, *ctx);
        // Set SNI Hostname (many hosts need this to handshake successfully)
        if(! SSL_set_tlsext_host_name(stream.native_handle(), host.c_str()))
        {
            beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
            throw beast::system_error{ec};
        }
        beast::get_lowest_layer(stream).connect(resolvedHost);
        stream.handshake(ssl::stream_base::client);

        http::request<http::string_body> req{
            http::verb::post, path, 11};
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
        stream.shutdown(ec);
        WebResponse response(res);
        return response;
    }
}
