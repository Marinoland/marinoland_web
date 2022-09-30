#pragma once

#include <string>

namespace web {
    using namespace std;

    class AuthHeader {
    public:
        virtual string operator()(string method, string url) = 0;
    };

    class BearerHeader : public AuthHeader {
    public:
        BearerHeader(string _bearerToken) : bearerToken(_bearerToken)
        {}
        virtual string operator()(string method, string url) override;
    private:
        string bearerToken;
    };

    class OAuth1Header : public AuthHeader {
    public:
        OAuth1Header(string _apiKey, string _apiKeySecret,
                        string _token, string _tokenSecret) :
            apiKey(_apiKey),
            apiKeySecret(_apiKeySecret),
            token(_token),
            tokenSecret(_tokenSecret)
        {}
        virtual string operator()(string method, string url) override;
    private:
        string apiKey;
        string apiKeySecret;
        string token;
        string tokenSecret;
    };
}