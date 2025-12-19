#include "Codec/Serializer.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

Serializer::Serializer(){}

Serializer& Serializer::operator=(Serializer&& other){}

Serializer::Serializer(Serializer&& other){}