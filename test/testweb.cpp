#include <iostream>
#include <string>
#include <algorithm>
#include "web/HttpsClient.hpp"
#include "web/WebClient.hpp"
#include "web/RestClient.hpp"

using namespace std;

int main (int argc, char **argv)
{
    string helloWorld("Yo C++! Been a while");

    for_each(helloWorld.begin(), helloWorld.end(), [] (char c) {
        cout << c;
    });
    cout << endl;


    web::WebClient().get("https://google.com/",
    map<string,string>(),
    [](web::WebResponse response) {
        cout << "Status: " << response.getCode() << endl;
        cout << "Body: " << response.getBody().substr(0, 100) << endl;
    });

    web::RestClient().get("http://dummy.restapiexample.com/api/v1/employees",
    map<string, string>(),
    [](web::RestWebResponse & response) {
        response.getJson()->asObject([] (json::ObjectNode & root) {
            cout << root.to_json() << endl;
            root.get("data")->asArray([] (json::ArrayNode & data) {
                for(json::nodeptr employeeptr : data) {
                    employeeptr->asObject([] (json::ObjectNode & employee) {
                        cout
                        << employee.get("id")->to_string()
                        << ' '
                        << employee.get("employee_name")->to_string()
                        << " age "
                        << employee.get("employee_age")->to_string()
                        << endl;
                    });
                }
            });
        });
    });
    return 0;
}