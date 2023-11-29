#pragma

#include <queue>
#include <mutex>

namespace pubsub
{
    template <typename T>
    class ThreadSafeQueue
    {
    private:
        std::queue<T> mQueue{};
        std::mutex mMutex{};
    public:
        void push(T const& value)
        {
            std::lock_guard<std::mutex> lock { mMutex };
            mQueue.push(value);
        }

        bool const pop(T& value)
        {
            std::lock_guard<std::mutex> lock { mMutex };
            if(mQueue.empty())
            {
                return false;
            }
            value = mQueue.front();
            mQueue.pop();
            return true;
        }
    };
}
