#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "IPipeServerImpl.h"

namespace pubsub
{
    class ITopic;

    class PipeServerImpl : public IPipeServerImpl
    {
    private:
        std::string const mName;
    public:
        PipeServerImpl(std::string const& name);
        bool const read_pipe(std::vector<char>& buffer);
    };
}