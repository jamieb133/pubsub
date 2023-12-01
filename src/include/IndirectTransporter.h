#pragma once

#include <memory>
#include <array>

#include "pubsub_macros.h"
#include "ITransporter.h"

namespace pubsub
{
    class ISerialiser;
    class IClient;

    class IndirectTransporter : public ITransporter
    {
    private:
        std::shared_ptr<IClient> const mClient;
        std::shared_ptr<ISerialiser> const mSerialiser;
        std::array<char,MAXIMUM_BUFFER_SIZE> mOutputBuffer{};
    public:
        void send(std::shared_ptr<ITopic> const topic);

        IndirectTransporter(std::shared_ptr<ISerialiser> const serialiser,
                            std::shared_ptr<IClient> const client);
    };
}