#pragma once

#include <vector>

namespace pubsub
{
    class IPipeServerImpl
    {
    public:
        virtual bool const read_pipe(std::shared_ptr<std::vector<char>>& buffer) = 0;
    };
}