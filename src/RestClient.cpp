#include "RestClient.hpp"
#include <iostream>
#include "WebClient.hpp"
#include "RestWebResponse.hpp"

using namespace std;

namespace web {

    void RestClient::get(std::string url,
            std::map<std::string, std::string> header,
            std::function<void(RestWebResponse & response)> f) {

        web::WebClient().get(url,
        header,
        [&](web::WebResponse response) {
            RestWebResponse restResp(response);
            f(restResp);
        });
    }

    void RestClient::post(std::string url,
            std::map<std::string, std::string> header,
            json::nodeptr body,
            std::function<void(RestWebResponse & response)> f) {

        header["Content-Type"] = "application/json";
        web::WebClient().post(url,
        header,
        body->to_json(),
        [&](web::WebResponse response) {
            RestWebResponse restResp(response);
            f(restResp);
        });
    }
}