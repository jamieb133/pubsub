#pragma once

#include <vector>

namespace pubsub
{
    class IPipeTransporterImpl
    {
    public:
        virtual void send_bytes(std::vector<char> const& data) = 0;
    };
}
