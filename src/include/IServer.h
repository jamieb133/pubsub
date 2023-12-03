#pragma once

#include <array>

#include "pubsub_macros.h"
#include "Buffers.h"

namespace pubsub
{
    class IServer
    {
    public:
        virtual bool const read(Buffer& buffer) = 0;
    };
}
