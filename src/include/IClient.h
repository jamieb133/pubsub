#pragma once

#include <array>

#include "pubsub_macros.h"

namespace pubsub
{
    class IClient
    {
    public:
        virtual void send_bytes(std::array<char,MAXIMUM_BUFFER_SIZE> const& data,
                                size_t const size) = 0;
    };
}
