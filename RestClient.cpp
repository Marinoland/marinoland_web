#include "RestClient.hpp"
#include <iostream>
#include "WebClient.hpp"
#include "RestWebResponse.hpp"

using namespace std;

namespace web {

    void RestClient::get(std::string url,
            std::function<void(RestWebResponse & response)> f) {

        web::WebClient().get(url,
        [&](web::WebResponse response) {
            RestWebResponse restResp(response);
            f(restResp);
        });
    }
}