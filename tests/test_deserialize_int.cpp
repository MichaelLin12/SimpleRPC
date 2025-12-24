#include <iostream>
#include <Msg/Message.hpp>
#include <Codec/Deserializer.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

int main(){
    boost::uuids::random_generator gen;
    boost::uuids::uuid id = gen();
    size_t size = sizeof(int);

    Message m{id,size};
    m.add(6);
    Deserializer d{};

    int rt = d.deserialize<int>(m.getBuffer());
    if(rt == 6){
        std::cout << "\033[032m" << std::endl;
        std::cout << "[ SUCCESS ]" << std::endl;
        std::cout << "\033[37m" << std::endl;
    }else{
        std::cout << "\033[032m" << std::endl;
        std::cout << "[ SUCCESS ]" << std::endl;
        std::cout << "\033[37m" << std::endl;
    }
    return 0;
}