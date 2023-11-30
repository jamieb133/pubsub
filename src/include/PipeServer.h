#pragma once

#include <queue>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <thread>
#include <atomic>

#include "pubsub_macros.h"
#include "IServer.h"
#include "IPipeServerImpl.h"
#include "ThreadSafeQueue.h"

namespace pubsub
{
    class IDeliverer;
    class IDeserialiser;
    class ITopic;

    class PipeServer : public IServer
    {
    private:
        ThreadSafeQueue<std::shared_ptr<std::vector<char>>> mDataRxQueue{};
        std::string const mName;
        std::shared_ptr<IDeliverer> const mDeliverer;
        std::shared_ptr<IPipeServerImpl> const mImpl;
        std::shared_ptr<IDeserialiser> const mDeserialiser;
        std::thread mPollDataQueueThread, mReadPipeThread;
        std::atomic_bool mRunning {true};
        std::condition_variable mCv{};
        
        void poll_data_queue();
        void read_pipe();

    public:
        PipeServer(std::string const& name, 
                    std::shared_ptr<IDeliverer> const deliverer,
                    std::shared_ptr<IDeserialiser> deserialiser);
        virtual ~PipeServer();
        void register_topic(std::string const& topicName, 
                            ITopicReconstructor const& topicReconstructor);

        // Test purposes only.
        PipeServer(std::string const& name, 
                    std::shared_ptr<IDeliverer> const deliverer,
                    std::shared_ptr<IDeserialiser> deserialiser,
                    std::shared_ptr<IPipeServerImpl> const impl);
    };
}
