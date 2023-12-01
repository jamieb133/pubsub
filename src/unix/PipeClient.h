#pragma once

#include <string>
#include <array>

#include "pubsub_macros.h"
#include "IClient.h"

namespace pubsub
{
    class PipeClient : public IClient
    {
    private:
        std::string const mName;
        int mFd;
    public:
        PipeClient(std::string const& name);
        virtual ~PipeClient();
        void send_bytes(std::array<char,MAXIMUM_BUFFER_SIZE> const& data,
                        size_t size);
    };
}
