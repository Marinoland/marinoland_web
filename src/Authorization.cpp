#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
#include <chrono>
#include <vector>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

#include "web/Authorization.hpp"
#include "web/URLEncode.hpp"

namespace web {
    string BearerHeader::operator() (string method, string url) {
        return string("Bearer ").append(bearerToken);
    }

    string OAuth1Header::operator() (string method, string url) {
        string timestamp = to_string(chrono::system_clock::now().time_since_epoch()/chrono::seconds(1));
        static int nonceCount = 1;
        string nonce = to_string(nonceCount++);
        nonceCount %= 1000000;

        vector<pair<string, string>> paramList = {
            {"oauth_consumer_key", apiKey},
            {"oauth_nonce", nonce},
            {"oauth_signature_method", "HMAC_SHA1"},
            {"oauth_timestamp", timestamp},
            {"oauth_token", token},
            {"oauth_version", "1.0"}
        };

        int i = url.find_first_of("?");
        if(i != std::string::npos) {
            string querystring = url.substr(i+1);
            url = url.substr(0, i);
            istringstream iss(querystring);
            string token;
            while(getline(iss, token, '&')) {
                i = token.find_first_of("=");
                paramList.push_back(pair<string, string>(token.substr(0, i), web::urlEncode(token.substr(i+1))));
            }
        }
        sort(paramList.begin(), paramList.end(), [](pair<string, string> a, pair<string, string> b) { return 0 > a.first.compare(b.first); });
        string params;
        for(pair<string, string> param: paramList) {
            if(params.length())
                params += "&";
            params += param.first + "=" + param.second;
        }
            
        string baseString = method + "&" + web::urlEncode(url) + "&" +  web::urlEncode(params);
        string signingKey = apiKeySecret + "&" + tokenSecret;

        vector<unsigned char> hmacResult(1024);
        vector<unsigned char> b64Result(1024);
        unsigned int hmacLength = 0;
        HMAC(EVP_sha1(), signingKey.c_str(), signingKey.length(), (unsigned char*)baseString.c_str(), baseString.length(),
            hmacResult.data(), &hmacLength);
        const int ol = EVP_EncodeBlock(b64Result.data(), hmacResult.data(), hmacLength);
        string signature = web::urlEncode((char*)b64Result.data());

        return string("OAuth ") +
            "oauth_consumer_key=\"" + apiKey + "\"" +
            ",oauth_token=\"" + token + "\"" +
            ",oauth_signature_method=\"HMAC_SHA1\"" +
            ",oauth_timestamp=\"" + timestamp + "\"" +
            ",oauth_nonce=\"" + nonce + "\"" +
            ",oauth_version=\"1.0\"" +
            ",oauth_signature=\"" + signature + "\"";
    }
}