#pragma once

#include "ISerialiser.h"

namespace pubsub
{
    class ITopic 
    {
    public:
        virtual void serialise(ISerialiser const& serialiser) = 0;
        virtual std::string const& get_name() = 0;
    };
}