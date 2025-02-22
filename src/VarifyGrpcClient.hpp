#pragma once
#include<grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.hpp"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;
using message::VarifyService;
using message::GetVarifyReq;
using message::GetVarifyRsp;

class VarifyGrpcClient : public Singleton<VarifyGrpcClient>
{
    friend class Singleton<VarifyGrpcClient>;
public:
    GetVarifyRsp GetVarifyCode(std::string email){
        ClientContext context;
        GetVarifyReq requset;
        GetVarifyRsp reply;
        requset.set_email(email);
        
        Status status = stub_->GetVarifyCode(&context, requset, &reply);
        
        if(status.ok()){
            return reply;
        }else{
            reply.set_error(ErrorCodes::RPCFailed);
            return reply;
        }
    }
private:
    VarifyGrpcClient(){
        std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
        stub_ = VarifyService::NewStub(channel);
    }
    std::unique_ptr<VarifyService::Stub>  stub_;
};
