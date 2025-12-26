#pragma once
#include <Codec/Deserializer.hpp>
#include <expected>
#include <Utility/Encoder.hpp>
#include <Utility/Error.hpp>
#include <unordered_map>
#include <string>
#include <functional>
#include <type_traits>
#include <tuple>

class Server{
public:
    static std::expected<Server,ERROR> create();
    Server(const Server& other)=delete;
    Server(Server&& other);
    Server& operator=(const Server& other)=delete;
    Server& operator=(Server&& other);

    ~Server();

    void run();
    std::expected<void,ERROR> respond();

    template<typename Arg>
    void receive(int socket, Arg& arg){ //we are going to use and assume fixed size strings
        Arg buff{};
        size_t received = 0;
        auto* buff = std::reinterpret_cast<std::byte*>(&buff);
        while(received < sizeof(buff)){
            ssize_t sz = recv(socket,buff+received,buff-receive,0);
            if(sz == 0 || sz == -1) [[unlikely]]{ //assume socket will never be closed or anything bad will happen 
                close(socket);
                return;
            }
            received+=static_cast<size_t>(sz);
        }
    }

    void receive(int socket, std::string& funcName, std::size_t sz);

    //using a lambda can be a little faster
    template <typename R, typename... Args>
    void callProxy(std::function<R(Args...)>&& func, char* buffer, int socket) { //turning this into a lambda function is a better approach as it is not compiler dependent
        char* ptr = buffer;
        std::tuple<Args...> args = std::make_tuple(deserializer.deserialize<Args>(ptr)...);
        R res = std::apply(func,args);
        // send function --> assume no errors --> assume int
        R swapped = std::byteswap(res);
        char* buff = reinterpret_cast<char*>(std::byteswap(&swapped));
        size_t len = sizeof(swapped);
        size_t sent = 0;
        while(sent < len){
            ssize_t bytes = send(socket,buff,len,0);
            if(bytes == -1) [[unlikely]] {
                return;
            }
            sent += static_cast<ssize_t>(bytes);
        }
    }

    template <typename R, typename... Args>
    void registerFunction(const std::string& function_name, R (*function)(Args...)) {
        functions[function_name] = std::bind(&callProxy, function, std::placeholders::_1,std::placeholders::_2);
    }
private:
    Server();
    Deserializer deserializer;
    int acceptSoc;
    std::unordered_map<std::string, std::function<void(char* buffer, int socket)>> functions;
};