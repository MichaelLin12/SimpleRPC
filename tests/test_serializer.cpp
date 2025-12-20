#include <iostream>
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "Codec/Serializer.hpp"
#include "Msg/Message.hpp"

int main(int argv, char** argc){
    boost::uuids::random_generator gen;
    boost::uuids::uuid id = gen();
    Serializer s{};
    std::string funcName = "add";
    int a = 5;
    int b = 6;
    Message m = s.serialize(id,funcName,a,b);
    auto x = m.getBuffer();
    auto y = m.getCurr();

    if(y-x== 2*sizeof(boost::uuids::uuid) + sizeof(size_t) + sizeof(a) + sizeof(b) + funcName.size() + sizeof(size_t)){
        std::cout << "\033[032m" << std::endl;
        std::cout << "[ SUCCESS ]" << std::endl;
        std::cout << "EXPECTED: " << 2*sizeof(boost::uuids::uuid) + sizeof(size_t) + sizeof(a) + sizeof(b) + funcName.size() + sizeof(size_t)<< std::endl;
        std::cout << "ACTUAL: " << y-x << std::endl;
        std::cout << "\033[37m" << std::endl;
    }else{
        std::cout << "\033[032m" << std::endl;
        std::cout << "[ ERROR ]" << std::endl;
        std::cout << "EXPECTED: " << 2*sizeof(boost::uuids::uuid) + sizeof(size_t) + sizeof(a) + sizeof(b) + funcName.size() + sizeof(size_t)<< std::endl;
        std::cout << "ACTUAL: " << y-x << std::endl;
        std::cout << "\033[37m" << std::endl;
    }
    return 0;
}