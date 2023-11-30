#pragma once

#include <memory>
#include <array>

#include "pubsub_macros.h"
#include "ITransporter.h"

namespace pubsub
{
    class ISerialiser;
    class IPipeTransporterImpl;

    class PipeTransporter : public ITransporter
    {
    private:
        std::shared_ptr<IPipeTransporterImpl> const mImpl;
        std::shared_ptr<ISerialiser> const mSerialiser;
        std::array<char,MAXIMUM_BUFFER_SIZE> mOutputBuffer{};
    public:
        PipeTransporter(std::string const& name, 
                        std::shared_ptr<ISerialiser> const serialiser);
        void send(std::shared_ptr<ITopic> const topic);

        // Only intended for testing.
        PipeTransporter(std::string const& name, 
                        std::shared_ptr<ISerialiser> const serialiser,
                        std::shared_ptr<IPipeTransporterImpl> const impl);
    };
}