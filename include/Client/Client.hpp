#pragma once
#include <string>
#include "Codec/Serializer.hpp"
#include "Msg/Message.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

//https://beej.us/guide/bgnet/html/split/index.html
// Only support IPV4
// Only support integral types and strings(kinda ... only for function name (need a better way for the general case))
class Client{
public:
    Client(const std::string addr);


    // Function templates (including template member functions) must be defined 
    // in the header, not just declared.
    template<typename Return, typename... Args>
    Return call(const std::string& functionName, Args&& ... args){

        //1.Serialize the data
        Message data = serializer.serialize<Args...>(this->id,functionName,std::forward<Args>(args)...);
        send();
        Return result = receive();
        return result;
    }

    ~Client();

    Client(const Client& other) = delete;
    Client(Client&& other) noexcept;
    Client& operator=(const Client& other) = delete;
    Client& operator=(Client&& other) = delete;

    void snd(const Message& m);

    template<typename Return>
    Return receive(){
        char* buf[sizeof(Return)];
        ssize_t totalRecv = 0;
        ssize_t bytesRecv = 0;

        while (totalRecv < sizeof(Return)) {
            bytesRecv = recv(this->soc, buf + totalRecv, sizeof(Return) - totalRecv, 0);
            if (bytesRecv == 0) {
                // Connection closed cleanly
                break;
            } else if (bytesRecv == -1) {
                perror("recv");
                break;
            }
            totalRecv += bytesRecv;
        }
        //deserialize return
        return ret;
    }

private:
    Serializer serializer;
    boost::uuids::uuid id;
    int soc;
    //logging should be GLOBAL!!!!
    //able to enable/disable it based on the preprocessor/compiler
    //maybe need an object for an exception 
};