#include "web/WebClient.hpp"
#include "web/HttpClient.hpp"
#include "web/HttpsClient.hpp"
#include <iostream>
#include <vector>

using namespace std;

namespace web {
    vector<string> split(string str, string delim) {
        string copy = str;
        vector<string> ret;
        size_t pos;
        while((pos = copy.find(delim)) != string::npos) {
            ret.push_back(copy.substr(0, pos));
            copy.erase(0, pos+delim.length());
        }
        ret.push_back(copy);
        return ret;
    }
    void parseUrl(string url,
        std::function<void(HttpClient & client, string path)> f) {
        vector<string> v = split(url, "://");
        if(v.size() > 1) {
            string host;
            string path;
            string port;
            string proto = v[0];
            size_t slashpos = v[1].find("/");
            if(slashpos == string::npos) {
                host = v[1];
                path = "/";
            } else {
                host = v[1].substr(0, slashpos);
                path = v[1].substr(slashpos);
            }
            v = split(host, ":");
            if(v.size() > 1) {
                host = v[0];
                port = v[1];
            }

            if(proto == "http") {
                port = "80";
                HttpClient client(host, port);
                f(client, path);
            } else if(proto == "https") {
                port = "443";
                HttpsClient client(host, port);
                f(client, path);
            }
        }
    }
    void WebClient::get(string url,
            map<string, string> header,
            std::function<void(WebResponse response)> f) {
        
        parseUrl(url, [&] (HttpClient & client, string path) {
            WebResponse response = client.get(path, header);
            if(response.getCode() == 301) {
                get(response["Location"], header, f);
            } else {
                f(response);
            }
        });
    }
    void WebClient::post(string url,
            map<string, string> header,
            std::string body,
            std::function<void(WebResponse response)> f) {
        
        parseUrl(url, [&] (HttpClient & client, string path) {
            WebResponse response = client.post(path, header, body);
            if(response.getCode() == 301) {
                post(response["Location"], header, body, f);
            } else {
                f(response);
            }
        });
    }
}
