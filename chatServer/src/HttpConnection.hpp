#include "const.h"

class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
      friend class LogicSystem;
  public:
      HttpConnection(boost::asio::io_context &ioc);
      void Start();
      tcp::socket& GetSocket();
  private:
      void CheckDeadline();
      void WriteResponse();
      void HandleRequest();
      
      void PreaParseGetParam();
      
      tcp::socket socket_;

      beast::flat_buffer buffer_{1024*8};
      http::request<http::dynamic_body> request_;
      http::response<http::dynamic_body> response_;
      
      net::steady_timer deadline_{
          socket_.get_executor(),
          std::chrono::seconds(60)
      };
      
      //解析get请求参数
      std::string getUrl_;
      std::unordered_map<std::string, std::string> getParams_;
};