#pragma once

#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "ITransporter.h"

namespace pubsub
{
    class IDeliverer;
    class ITopic;

    class DirectTransporter : public pubsub::ITransporter
    {
    private:
        std::thread mThread;
        std::shared_ptr<IDeliverer> const mDeliverer;
        std::mutex mMutex{};
        std::queue<std::shared_ptr<ITopic> const> mMessageQueue{};
        std::atomic_bool mRunning{true};
        std::condition_variable mCv{};

        void poll_queue();
    public:
        DirectTransporter(std::shared_ptr<IDeliverer> const deliverer);
        virtual ~DirectTransporter();
        void send(std::shared_ptr<ITopic> const topic);
    };
}
