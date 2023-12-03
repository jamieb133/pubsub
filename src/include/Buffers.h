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
        std::array<char, MAXIMUM_BUFFER_SIZE>& get();
        std::array<char, MAXIMUM_BUFFER_SIZE> const& cget() const;
        size_t const size();
        void set_size(size_t const s);

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
        static Buffers& get_instance();
        std::array<Buffer, 10>::iterator get_buffer();
        bool const is_end(std::array<Buffer, 10>::iterator iter);
        void release_buffer(Buffer& bufferIn);
    };
}