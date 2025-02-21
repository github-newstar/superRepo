#include "CServer.hpp"
#include "HttpConnection.hpp"
#include <iostream>

CServer::CServer(net::io_context &ioc, unsigned short &port)
    : ioc_(ioc), acceptor_(ioc, tcp::endpoint(tcp::v4(), port)), socket_(ioc) {}
void CServer::Start() {
    auto self = shared_from_this();
    acceptor_.async_accept(socket_, [self](beast::error_code ec) {
        try {
            if (ec) {
                self->Start();
                return;
            }

            // 处理新连接
            std::make_shared<HttpConnection>(std::move(self->socket_))->Start();
            self->Start();
        } catch (std::exception &e) {
            std::cerr << __FUNCTION__ << " start connection failed " << e.what()
                      << "at " << __FILE__ << ":" << __LINE__ << std::endl;
            self->Start();
        }
    });
}