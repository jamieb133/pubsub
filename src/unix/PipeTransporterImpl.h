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
    public:
        PipeTransporterImpl(std::string const& name);
        void send_bytes(std::vector<char> const& data);
    };
}
