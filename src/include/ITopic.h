#pragma once

namespace pubsub
{
    class ISerialiser;

    class ITopic 
    {
    public:
        virtual void serialise(ISerialiser const& serialiser) = 0;
        virtual std::string const& get_name() = 0;
    };
}