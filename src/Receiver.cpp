#include "Receiver.h"
#include "IServer.h"
#include "IDeserialiser.h"
#include "IDeliverer.h"
#include "ITopic.h"
#include "Buffers.h"

using namespace pubsub;

Receiver::Receiver(std::shared_ptr<IDeliverer> const deliverer,
                    std::shared_ptr<IDeserialiser> deserialiser,
                    std::shared_ptr<IServer> const server) :
    mDeliverer{deliverer},
    mServer{server},
    mDeserialiser{deserialiser},
    mReadPipeThread{[this](){ read_server(); }}
{}

Receiver::~Receiver()
{
    mRunning = false;
    mReadPipeThread.join();
}

void Receiver::read_server()
{
    Buffers& buffers { Buffers::get_instance() };
    while(mRunning)
    {
        auto rxBufferIter = buffers.get_buffer();
        if(buffers.is_end(rxBufferIter))
        {
            continue;
        }

        if(mServer->read(*rxBufferIter))
        {
            
            // TODO: push this onto a queue to be processed by a different thread.

            std::shared_ptr<ITopic> deserialisedTopic { mDeserialiser->deserialise(*rxBufferIter) };
            if(deserialisedTopic)
            {
                mDeliverer->on_data(deserialisedTopic);
            }

        }

        buffers.release_buffer(*rxBufferIter);
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
}

void Receiver::register_topic(std::string const& topicName, 
                                ITopicReconstructor const& topicReconstructor)
{
    mDeserialiser->register_topic(topicName, topicReconstructor);
}
