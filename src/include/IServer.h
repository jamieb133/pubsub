#pragma once

namespace pubsub
{
    class IServer
    {
    public:
        virtual bool const read(std::shared_ptr<std::vector<char>>& buffer) = 0;
    };
}
