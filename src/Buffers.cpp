#include "Buffers.h"

using namespace pubsub;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Buffer methods.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::array<char, MAXIMUM_BUFFER_SIZE>& Buffer::get()
{
    return mBuffer;
}

std::array<char, MAXIMUM_BUFFER_SIZE> const& Buffer::cget() const
{
    return mBuffer;
}

size_t const Buffer::size()
{
    return mSize;
}

void Buffer::set_size(size_t const s)
{
    mSize = s;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Buffers methods.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Buffers& Buffers::get_instance()
{
    static Buffers instance{};
    return instance;
}

std::array<Buffer, 10>::iterator Buffers::get_buffer()
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

bool const Buffers::is_end(std::array<Buffer, 10>::iterator iter)
{
    std::lock_guard<std::mutex> lock {mMutex};
    return (iter == mBuffers.end());
}

void Buffers::release_buffer(Buffer& bufferIn)
{
    std::lock_guard<std::mutex> lock {mMutex};
    mBuffers[bufferIn.mBufferId].mAvailable = true;
}