#include <sstream>
#include <iostream>
#include "web/RestWebResponse.hpp"
using namespace std;
namespace web {
    RestWebResponse::RestWebResponse(const WebResponse & cp) : WebResponse(cp) {
    }

    json::nodeptr RestWebResponse::getJson() {
        json::nodeptr ret;
        json::parse(getBody(),
        [&] (json::nodeptr node)
        {
            ret = node;
        },
        [&] (string msg)
        {
            ret = make_shared<json::NullNode>();
            cerr << msg << endl;
        });

        return ret;
    }
}