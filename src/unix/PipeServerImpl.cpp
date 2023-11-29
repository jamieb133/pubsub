#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <vector>

#include "PipeServerImpl.h"
#include "pubsub_macros.h"

using namespace pubsub;

PipeServerImpl::PipeServerImpl(std::string const& name) :
    mName{"/tmp/" + name}
{}

bool const PipeServerImpl::read_pipe(std::vector<char>& buffer)
{
    int fd { open(mName.c_str(), O_RDONLY) };
    if(fd == -1)
    {
        std::cerr << "Error, couldn't open " << mName << " " << errno << std::endl;
        return false;
    }

    // TODO: implement more efficient strategy...
    buffer.resize(MAXIMUM_BUFFER_SIZE);
    ssize_t numBytesRead { read(fd, buffer.data(), buffer.size()) };

    if(numBytesRead == -1)
    {
        std::cerr << "Error, couldn't read from " << mName << " " << errno << std::endl;
        return false;
    }

    buffer.resize(numBytesRead);

    return true;
}
