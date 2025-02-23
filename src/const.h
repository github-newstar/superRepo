#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <sw/redis++/redis++.h>

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>
                                  // 
enum ErrorCodes{
    Success = 0,
    ErrorJson = 1001,
    RPCFailed = 1002,
};

class ConfigMgr;
extern ConfigMgr gCfgMgr;