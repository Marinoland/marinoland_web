#ifndef __ErrorCodes_hpp__
#define __ErrorCodes_hpp__

#include <boost/beast/core.hpp>
#include <openssl/storeerr.h>
namespace web {
    enum error {
        connect_failed      = -101
        ,ssl_unreg_scheme    = -301
    };

    inline int convertBoostError(int boostErr)
    {
        switch(boostErr & 0x0000FFFF)
        {
            case boost::asio::error::not_found:
                return error::connect_failed;
            case OSSL_STORE_R_UNREGISTERED_SCHEME:
                return error::ssl_unreg_scheme;
            default:
                return -1;
        }
    }
}

#endif