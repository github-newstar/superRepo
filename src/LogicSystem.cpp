#include "LogicSystem.hpp"
#include "const.h"
#include "HttpConnection.hpp"
#include <json/json.h>
#include <json/reader.h>
#include "VarifyGrpcClient.hpp"

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> con) {
    if(getHandlers_.find(path) == getHandlers_.end()){
        return false;
    }
    getHandlers_[path](con);
    return true;
}
bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> con) {
    if(postHandlers_.find(path) == postHandlers_.end()){
        return false;
        }
    postHandlers_[path](con);
    return true;
}
void LogicSystem::RegGet(std::string url, HttpHandler handler) {
    getHandlers_[url] = handler;
}
void LogicSystem::RegPost(std::string url, HttpHandler handler) {
    postHandlers_[url] = handler;
}
LogicSystem::LogicSystem() {
    RegGet("/get_test",[](std::shared_ptr<HttpConnection> conncetion){
        beast::ostream(conncetion->response_.body()) << "receive get_test req";
    int i = 0;
    for(auto &elem : conncetion->getParams_){
        i++;
        beast::ostream(conncetion->response_.body()) << "param " << 
        i << " key is " << elem.first;
        beast::ostream(conncetion->response_.body()) << " value is " << elem.second << std::endl;
    }
    });
    
    RegPost("/get_code", [](std::shared_ptr<HttpConnection> conncetion){
            auto bodyStr = boost::beast::buffers_to_string(conncetion->request_.body().data());
            
            std::cout << "receive body is " << bodyStr << std::endl;
            
            conncetion->request_.set(http::field::content_type, "text/json");
            
            Json::Value root;
            Json::Reader reader;
            Json::Value src_root;
            
            bool parseSuccess = reader.parse(bodyStr, src_root);
            if(!parseSuccess){
                std::cout << "parse json failed" << std::endl;
                root["error"] = ErrorCodes::ErrorJson;
                std::string jsonStr = root.toStyledString();
                beast::ostream(conncetion->response_.body()) << jsonStr;
                return true;
            }
            
            auto email = src_root["email"].asString();
            message::GetVarifyRsp rsp = VarifyGrpcClient::GetInstance()->GetVarifyCode(email);
            std::cout <<  "email is " << email << std::endl;
            root["error"] = rsp.error();
            root["email"] = src_root["email"];
            
            std::string jsonStr = root.toStyledString();
            beast::ostream(conncetion->response_.body()) << jsonStr;
            return true;
    });
    

}
LogicSystem::~LogicSystem(){};
