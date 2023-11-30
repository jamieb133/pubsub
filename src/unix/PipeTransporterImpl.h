#pragma once

#include <string>
#include <vector>

#include "IPipeTransporterImpl.h"

namespace pubsub
{
    class PipeTransporterImpl : public IPipeTransporterImpl
    {
    private:
        std::string const mName;
        int mFd;
    public:
        PipeTransporterImpl(std::string const& name);
        virtual ~PipeTransporterImpl();
        void send_bytes(std::array<char,MAXIMUM_BUFFER_SIZE> const& data,
                        size_t size);
    };
}
