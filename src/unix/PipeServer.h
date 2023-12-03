#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "IServer.h"

namespace pubsub
{
    class ITopic;

    class PipeServer : public IServer
    {
    private:
        std::string const mName;
    public:
        PipeServer(std::string const& name);
        bool const read(Buffer& buffer);
    };
}