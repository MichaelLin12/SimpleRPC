#pragma once
#include <string>
#include "Codec/Serializer.hpp"
#include "Msg/Message.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class Client{
public:
    Client();


    // Function templates (including template member functions) must be defined 
    // in the header, not just declared.
    template<typename Return, typename... Args>
    Return call(const std::string& functionName, Args&& ... args){

        //1.Serialize the data
        Message data = serializer.serialize<Args...>(this->id,functionName,std::forward<Args>(args)...);
        if constexpr (std::is_void_v<Return>) {
            // 2. Send data over --> expect an OK reply
            return;
        } else {
            // 2. Send data over (expect a result)
            return result;
        }
    }

    Client(const Client& other) = delete;
    Client(Client&& other) noexcept;
    Client& operator=(const Client& other) = delete;
    Client& operator=(Client&& other) = delete;

private:
    Serializer serializer;
    boost::uuids::uuid id; // need to be able to pass this down to the message obj
    //socket for client
    //maybe a logger obj ... but I don't now ... we want to be 
    //able to enable/disable it based on the preprocessor/compiler
    //maybe need an object for an exception 
};