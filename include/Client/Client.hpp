#pragma once
#include <string>
#include "Codec/Serializer.hpp"
#include "Utility/Error.hpp"
#include <expected>
#include "Msg/Message.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

//https://beej.us/guide/bgnet/html/split/index.html
// Only support IPV4
// Only support integral types and strings(kinda ... only for function name (need a better way for the general case))
class Client{
public:
    static std::expected<Client,ERROR> create(const std::string& addr);

    // Function templates (including template member functions) must be defined 
    // in the header, not just declared.
    template<typename Return, typename... Args>
    std::expected<Return,ERROR> call(const std::string& functionName, Args&& ... args){
        Message data = serializer.serialize<Args...>(this->id,functionName,std::forward<Args>(args)...);
        std::expected<void,ERROR> rt = send();
        if(rt) [[unlikely]] {
            return std::unexpected{rt.error()};
        }
        return receive();
    }

    ~Client();

    Client(const Client& other) = delete;
    Client(Client&& other) noexcept;
    Client& operator=(const Client& other) = delete;
    Client& operator=(Client&& other) = delete;

    std::expected<void,ERROR> snd(const Message& m);

    template<typename Return>
    std::expected<Return,ERROR> receive(){
        char* buf[sizeof(Return)];
        ssize_t totalRecv = 0;
        ssize_t bytesRecv = 0;

        while (totalRecv < sizeof(Return)) {
            bytesRecv = recv(this->soc, buf + totalRecv, sizeof(Return) - totalRecv, 0);
            if (bytesRecv == 0) [[unlikely]] {
                return std::expected{std::unexpect,ERROR::CONN_CLOSED};
            } else if (bytesRecv == -1) [[unlikely]] {
                return std::expected{std::unexpect, ERROR::RECV_FAILURE};
            }
            totalRecv += bytesRecv;
        }
        //deserialize return
        return ret;
    }

private:

    Client();

    Serializer serializer;
    boost::uuids::uuid id;
    int soc;
};