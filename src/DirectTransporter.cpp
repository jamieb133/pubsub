#include <chrono>

#include "DirectTransporter.h"
#include "ITopic.h"
#include "IDeliverer.h"

#include <iostream>

using namespace pubsub;

void DirectTransporter::poll_queue()
{
    while(mRunning)
    {
        {
            std::unique_lock<std::mutex> lock { mMutex };
            if (!mMessageQueue.empty())
            {
                std::shared_ptr<ITopic> const topic { mMessageQueue.front() };
                mMessageQueue.pop();
                mDeliverer->on_data(topic);
            }
        }

        mCv.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds{25});
    }
}

DirectTransporter::DirectTransporter(std::shared_ptr<IDeliverer> const deliverer) :
    mDeliverer { deliverer },
    mThread { [this](){ poll_queue(); } }
{}

DirectTransporter::~DirectTransporter()
{
    { // Wait for the message queue to be processed.
        std::unique_lock<std::mutex> lock { mMutex };
        mCv.wait(lock, [this](){ return mMessageQueue.empty(); });
    }

    mRunning = false;
    mThread.join();
}

void DirectTransporter::send(std::shared_ptr<ITopic> const topic)
{
    std::unique_lock<std::mutex> lock { mMutex };
    mMessageQueue.push(topic);
}
