#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <vector>

#include "PipeServer.h"
#include "pubsub_macros.h"

using namespace pubsub;

PipeServer::PipeServer(std::string const& name) :
    mName{"./" + name}
{
    constexpr int DEFAULT_FILE_PERMISSION { 0666 };
    if(mkfifo(mName.c_str(), DEFAULT_FILE_PERMISSION) == -1)
    {
        // Pipe may already exist.
        if(errno != 17)
        {
            std::cerr << "Error, couldn't create fifo " << errno << std::endl;
            return;
        }
    }
}

bool const PipeServer::read(std::shared_ptr<std::vector<char>>& buffer)
{
    int fd { open(mName.c_str(), O_RDONLY) };
    if(fd == -1)
    {
        std::cerr << "Error, couldn't open " << mName << " " << errno << std::endl;
        return false;
    }

    // TODO: implement more efficient strategy...
    buffer = std::make_shared<std::vector<char>>(MAXIMUM_BUFFER_SIZE);
    ssize_t numBytesRead { ::read(fd, buffer->data(), buffer->size()) };

    if(numBytesRead == -1)
    {
        std::cerr << "Error, couldn't read from " << mName << " " << errno << std::endl;
        return false;
    }

    buffer->resize(numBytesRead);

    return true;
}
