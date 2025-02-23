#include "CServer.hpp"
#include "HttpConnection.hpp"
#include "AsioIOServicePool.hpp"
#include <iostream>

CServer::CServer(net::io_context &ioc, unsigned short &port)
    : ioc_(ioc), acceptor_(ioc, tcp::endpoint(tcp::v4(), port)) {}
void CServer::Start() {
    auto self = shared_from_this();
    auto &ioc = AsioIOServicePool::GetInstance()->GetIOService();
    std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(ioc);
    
    acceptor_.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code ec) {
        try {
            if (ec) {
                self->Start();
                return;
            }

            // 处理新连接
            new_con->Start();
            self->Start();
        } catch (std::exception &e) {
            std::cerr << __FUNCTION__ << " start connection failed " << e.what()
                      << "at " << __FILE__ << ":" << __LINE__ << std::endl;
            self->Start();
        }
    });
}