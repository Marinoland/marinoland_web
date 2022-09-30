#pragma once 

#include <string>
#include <map>

namespace web {
    using namespace std;
    inline string urlEncode(string src) {
        static map<char, string> escapeChars = {
            {' ', "%20"}, {'<', "%3C"}, {'>', "%3E"}, {'#', "%23"},
            {'%', "%25"}, {'+', "%2B"}, {'{', "%7B"}, {'}', "%7D"},
            {'|', "%7C"}, {'\\', "%5C"}, {'^', "%5E"}, {'~', "%7E"},
            {'[', "%5B"}, {']', "%5D"}, {';', "%3B"}, {'/', "%2F"},
            {'?', "%3F"}, {':', "%3A"}, {'@', "%40"}, {'=', "%3D"},
            {'&', "%26"}, {'$', "%24"}, {'\'', "%27"}, {',', "%2C"}
        };
        string dest;
        for(char c : src) {
            if(escapeChars.find(c) == escapeChars.end())
                dest += c;
            else
                dest += escapeChars[c];
        }
        return dest;
    }
}