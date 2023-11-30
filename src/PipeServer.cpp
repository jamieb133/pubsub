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
    mReadPipeThread{[this](){ read_pipe(); }}
{}

PipeServer::~PipeServer()
{
    mRunning = false;
    mReadPipeThread.join();
}

void PipeServer::read_pipe()
{
    while(mRunning)
    {
        std::shared_ptr<std::vector<char>> rxData{};
        if(mImpl->read_pipe(rxData))
        {
            
            // TODO: push this onto a queue to be processed by a different thread.

            std::shared_ptr<ITopic> deserialisedTopic { mDeserialiser->deserialise(*rxData) };
            if(deserialisedTopic)
            {
                mDeliverer->on_data(deserialisedTopic);
            }

        }
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
}

void PipeServer::register_topic(std::string const& topicName, 
                                ITopicReconstructor const& topicReconstructor)
{
    mDeserialiser->register_topic(topicName, topicReconstructor);
}
