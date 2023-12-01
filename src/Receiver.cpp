#include "Receiver.h"
#include "IServer.h"
#include "IDeserialiser.h"
#include "IDeliverer.h"
#include "ITopic.h"

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
    while(mRunning)
    {
        std::shared_ptr<std::vector<char>> rxData{};
        if(mServer->read(rxData))
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

void Receiver::register_topic(std::string const& topicName, 
                                ITopicReconstructor const& topicReconstructor)
{
    mDeserialiser->register_topic(topicName, topicReconstructor);
}
