#pragma once

#include <array>
#include <mutex>

#include "pubsub_macros.h"

namespace pubsub
{
    class Buffers;

    class Buffer
    {
    private:
        std::array<char, MAXIMUM_BUFFER_SIZE> mBuffer;
        size_t mBufferId;
        size_t mSize;
        bool mAvailable{true};
        friend class Buffers;

    public:
        std::array<char, MAXIMUM_BUFFER_SIZE>& get()
        {
            return mBuffer;
        }

        std::array<char, MAXIMUM_BUFFER_SIZE> const& cget() const
        {
            return mBuffer;
        }

        size_t const size()
        {
            return mSize;
        }

        void set_size(size_t const s)
        {
            mSize = s;
        }

    };

    class Buffers
    {
    private:
        std::array<Buffer, 10> mBuffers{};
        std::mutex mMutex{};
        
        Buffers()
        {
            for(int i{0}; i < mBuffers.size(); i++)
            {
                mBuffers[i].mBufferId = i;
            }
        }

        Buffers(Buffers const&) = delete;
        Buffers(Buffers&&) = delete;
        Buffers& operator=(Buffers const&) = delete;
        Buffers& operator=(Buffers&&) = delete;

    public:
        static Buffers& get_instance()
        {
            static Buffers instance{};
            return instance;
        }

        std::array<Buffer, 10>::iterator get_buffer()
        {
            std::lock_guard<std::mutex> lock {mMutex};
            for(auto iter = mBuffers.begin(); iter != mBuffers.end(); iter++)
            {
                if (iter->mAvailable)
                {
                    iter->mAvailable = false;
                    return iter;
                }
            }
            return mBuffers.end();
        }

        bool const is_end(std::array<Buffer, 10>::iterator iter)
        {
            std::lock_guard<std::mutex> lock {mMutex};
            return (iter == mBuffers.end());
        }

        void release_buffer(Buffer& bufferIn)
        {
            std::lock_guard<std::mutex> lock {mMutex};
            mBuffers[bufferIn.mBufferId].mAvailable = true;
        }
    };
}