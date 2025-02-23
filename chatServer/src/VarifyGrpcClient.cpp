#include "VarifyGrpcClient.hpp"
#include "configMgr.hpp"
#include "const.h"

RPCconPool::RPCconPool(size_t poolSize, std::string host, std::string port)
    : bStop_(false), poolSize_(poolSize), host_(host), port_(port)
{
    for(size_t i = 0; i < poolSize; ++i){
        std::shared_ptr<Channel> channel = grpc::CreateChannel(host_+":"+port_, 
            grpc::InsecureChannelCredentials());
        connections_.push(VarifyService::NewStub(channel));
    }
}
RPCconPool::~RPCconPool(){
    std::lock_guard<std::mutex> lock(mutex_);
    this->Close();
    while(!connections_.empty()){
        connections_.pop();
    }
}
std::unique_ptr<VarifyService::Stub> RPCconPool::getConnection(){
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this]{
            if(bStop_){
                return true;
            }
            return !connections_.empty();
    });
    
    if(bStop_){
        return nullptr;
    }
    auto context = std::move(connections_.front());
    connections_.pop();
    return context;
}

void RPCconPool::returnConnection(std::unique_ptr<VarifyService::Stub> context){
    std::lock_guard<std::mutex> lock(mutex_);
    if(bStop_){
        return;
    }
    connections_.push(std::move(context));
    cond_.notify_one();
}
void RPCconPool::Close(){
    bStop_ = true;
    cond_.notify_all();
}

GetVarifyRsp VarifyGrpcClient::GetVarifyCode(std::string email) {
    ClientContext context;
    GetVarifyReq requset;
    GetVarifyRsp reply;
    requset.set_email(email);
    auto stub = pool_->getConnection();
    Status status = stub->GetVarifyCode(&context, requset, &reply);

    if (status.ok()) {
        pool_->returnConnection(std::move(stub));
        return reply;
    } else {
        pool_->returnConnection(std::move(stub));
        reply.set_error(ErrorCodes::RPCFailed);
        return reply;
    }
}
VarifyGrpcClient::VarifyGrpcClient() {
    auto & gCfgMgr = ConfigMgr::GetInstance();
    std::string host = gCfgMgr["VarifyServer"]["Host"];
    std::string port = gCfgMgr["VarifyServer"]["Port"];
    std::cerr << "host is " << host << " port is " << port << std::endl;
    pool_.reset(new RPCconPool(5, host, port));
}
