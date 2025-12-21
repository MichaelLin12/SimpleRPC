#include <iostream>
#include "Msg/Message.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


int main(){

    boost::uuids::random_generator gen;
    boost::uuids::uuid id = gen();
    size_t size = sizeof(int);

    Message m{id,size};
    int j = 4;
    m.add(j);
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