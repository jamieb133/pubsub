#pragma once

#include <vector>

namespace pubsub
{
    class IPipeServerImpl
    {
    public:
        virtual bool const read_pipe(std::vector<char>& buffer) = 0;
    };
}