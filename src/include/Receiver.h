#pragma once

#include <queue>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <thread>
#include <atomic>

#include "pubsub_macros.h"
#include "IReceiver.h"
#include "ThreadSafeQueue.h"

namespace pubsub
{
    class IDeliverer;
    class IDeserialiser;
    class ITopic;
    class IServer;

    class Receiver : public IReceiver
    {
    private:
        ThreadSafeQueue<std::shared_ptr<std::vector<char>>> mDataRxQueue{};
        std::shared_ptr<IDeliverer> const mDeliverer;
        std::shared_ptr<IServer> const mServer;
        std::shared_ptr<IDeserialiser> const mDeserialiser;
        std::thread mPollDataQueueThread, mReadPipeThread;
        std::atomic_bool mRunning {true};
        std::condition_variable mCv{};
        
        void poll_data_queue();
        void read_server();

    public:
        virtual ~Receiver();
        void register_topic(std::string const& topicName, 
                            ITopicReconstructor const& topicReconstructor);

        Receiver(std::shared_ptr<IDeliverer> const deliverer,
                    std::shared_ptr<IDeserialiser> deserialiser,
                    std::shared_ptr<IServer> const server);
    };
}
