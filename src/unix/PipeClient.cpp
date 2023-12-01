#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include "PipeClient.h"

using namespace pubsub;

static constexpr int DEFAULT_FILE_PERMISSION { 0666 };

PipeClient::PipeClient(std::string const& name) :
    mName { "./" + name },
    mFd {mkfifo(mName.c_str(), DEFAULT_FILE_PERMISSION)}
{
    if(mFd == -1
        && errno != 17)
    {
        std::cerr << "Error, couldn't create fifo " << errno << std::endl;
        return;
    }
}

PipeClient::~PipeClient()
{
    if (mFd != -1
        && close(mFd))
    {
        std::cerr << "Error, couldn't delete fifo " << errno << std::endl;
    }
}

void PipeClient::send_bytes(std::array<char,MAXIMUM_BUFFER_SIZE> const& data,
                                        size_t const size)
{
    int fd { open(mName.c_str(), O_WRONLY) };
    
    if(fd == -1)
    {
        std::cerr << "Error, couldn't open fifo for sending " << errno << std::endl;
        return;
    }

    if(write(fd, data.data(), size) == -1)
    {
        std::cerr << "Error, couldn't write to fifo " << errno << std::endl;
    }
}

