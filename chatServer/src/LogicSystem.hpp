#include "Singleton.hpp"
#include <functional>
#include <map>

class HttpConnection;

using HttpHandler = std::function<void(std::shared_ptr<HttpConnection> )>;

class LogicSystem : public Singleton<LogicSystem> {
    friend class Singleton<LogicSystem>;
public:
    ~LogicSystem();
    bool HandleGet(std::string , std::shared_ptr<HttpConnection> );
    bool HandlePost(std::string , std::shared_ptr<HttpConnection> );
    void RegGet(std::string , HttpHandler);
    void RegPost(std::string , HttpHandler);
private:
    LogicSystem();
    std::map<std::string, HttpHandler>  postHandlers_;
    std::map<std::string, HttpHandler>  getHandlers_;

};