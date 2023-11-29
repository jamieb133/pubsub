#include "PipeServer.h"
#include "PipeServerImpl.h"
#include "IDeserialiser.h"
#include "IDeliverer.h"
#include "ITopic.h"

using namespace pubsub;

PipeServer::PipeServer(std::string const& name,
                        std::shared_ptr<IDeliverer> const deliverer,
                        std::shared_ptr<IDeserialiser> deserialiser) :
    PipeServer {name, deliverer, deserialiser, std::make_shared<PipeServerImpl>(name)}
{}

PipeServer::PipeServer(std::string const& name,
                        std::shared_ptr<IDeliverer> const deliverer,
                        std::shared_ptr<IDeserialiser> deserialiser,
                        std::shared_ptr<IPipeServerImpl> const impl) :
    mName{name},
    mDeliverer{deliverer},
    mImpl{impl},
    mDeserialiser{deserialiser},
    mPollDataQueueThread{[this](){ poll_data_queue(); }}
{}

PipeServer::~PipeServer()
{
    mRunning = false;
    mPollDataQueueThread.join();
    mReadPipeThread.join();
}

void PipeServer::read_pipe()
{
    std::shared_ptr<std::vector<char>> rxData{};

    while(mRunning)
    {
        if(mImpl->read_pipe(*rxData))
        {
            mDataRxQueue.push(rxData);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{25});
    }
}

void PipeServer::poll_data_queue()
{
    std::shared_ptr<std::vector<char> const> data;

    while(mRunning)
    {
        if(mDataRxQueue.pop(data))
        {
            std::shared_ptr<ITopic> deserialisedTopic { mDeserialiser->deserialise_bytes(*data) };
            if(deserialisedTopic)
            {
                mDeliverer->on_data(deserialisedTopic);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds{25});
    }
}
