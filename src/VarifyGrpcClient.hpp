#pragma once
#include<grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
// #include "const.h"
#include "Singleton.hpp"
#include<queue>
#include<mutex>
#include<condition_variable>

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;
using message::VarifyService;
using message::GetVarifyReq;
using message::GetVarifyRsp;
class RPCconPool {
  public:
    RPCconPool(size_t poolSize, std::string host, std::string port); ~RPCconPool();
    std::unique_ptr<VarifyService::Stub> getConnection();
    void returnConnection(std::unique_ptr<VarifyService::Stub> cotext);
    void Close();
  private:
  std::atomic_bool bStop_;
  std::size_t poolSize_;
  std::string host_;
  std::string port_;
  std::queue<std::unique_ptr<VarifyService::Stub>> connections_;;
  std::mutex mutex_;
  std::condition_variable cond_;
};

class VarifyGrpcClient : public Singleton<VarifyGrpcClient>
{
    friend class Singleton<VarifyGrpcClient>;
public:
    GetVarifyRsp GetVarifyCode(std::string email);
    
private:
    VarifyGrpcClient();
    std::unique_ptr<RPCconPool> pool_;
};
