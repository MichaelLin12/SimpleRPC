#include <iostream>
#include "Msg/Message.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

int main(){
    boost::uuids::random_generator gen;
    boost::uuids::uuid id = gen();
    std::string funcName{"add"};
    size_t size = funcName.size() + sizeof(std::size_t);

    Message m{id,size};
    m.add(funcName.size());
    m.add(funcName);

    auto b = static_cast<char*>(m.getBuffer());
    auto c = static_cast<char*>(m.getCurr());
    if(static_cast<unsigned long>(c-b) == 2*sizeof(boost::uuids::uuid) + sizeof(size_t) + size){
        std::cout << "\033[032m" << std::endl;
        std::cout << "[ SUCCESS ]" << std::endl;
        std::cout << "EXPECTED: " << 2*sizeof(boost::uuids::uuid) + sizeof(size_t) + size<< std::endl;
        std::cout << "ACTUAL: " << c-b << std::endl;
        std::cout << "\033[37m" << std::endl;
    }else{
        std::cout << "\033[032m" << std::endl;
        std::cout << "[ ERROR ]" << std::endl;
        std::cout << "EXPECTED: " << 2*sizeof(boost::uuids::uuid) + sizeof(size_t) + size<< std::endl;
        std::cout << "ACTUAL: " << c-b << std::endl;
        std::cout << "\033[37m" << std::endl;
    }
    return 0;
}