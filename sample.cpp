#include <iostream>
#include <string>
#include <algorithm>
#include "HttpsClient.hpp"
#include "WebClient.hpp"
#include "RestClient.hpp"

using namespace std;

int main (int argc, char **argv)
{
    string helloWorld("Yo C++! Been a while");

    for_each(helloWorld.begin(), helloWorld.end(), [] (char c) {
        cout << c;
    });
    cout << endl;


    web::WebClient().get("https://google.com/",
    [](web::WebResponse response) {
        cout << "Status: " << response.getCode() << endl;
        cout << "Body: " << response.getBody().substr(0, 100) << endl;
    });

    web::RestClient().get("http://dummy.restapiexample.com/api/v1/employees",
    [](web::RestWebResponse & response) {
        json::nodeptr tree = response.getJson();
        cout << tree->to_string() << endl;
        /*
        for (pair<string, nodeptr> node : tree) {
            const ptree & record = node.second;
            cout << record.get("employee_name", "null") <<  "\t"
                << record.get("employee_age", "null") << endl;
        }
        */
    });
    return 0;
}