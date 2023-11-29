#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include "PipeTransporterImpl.h"

using namespace pubsub;

PipeTransporterImpl::PipeTransporterImpl(std::string const& name) :
    mName { "/tmp/" + name }
{
    constexpr int DEFAULT_FILE_PERMISSION { 0666 };
    if(mkfifo(mName.c_str(), DEFAULT_FILE_PERMISSION) == -1)
    {
        std::cerr << "Error, couldn't create fifo " << errno << std::endl;
        return;
    }
}

void PipeTransporterImpl::send_bytes(std::vector<char> const& data)
{
    int fd { open(mName.c_str(), O_WRONLY) };
    
    if(fd == -1)
    {
        std::cerr << "Error, couldn't open fifo for sending " << errno << std::endl;
        return;
    }

    write(fd, data.data(), data.size());
}

