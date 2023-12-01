#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <vector>

bool const read_pipe(std::string const& name,
                        char* buffer,
                        size_t size)
{
    std::string const fullName { name };

    int fd { open(fullName.c_str(), O_RDONLY) };
    if(fd == -1)
    {
        std::cerr << "Error, couldn't open " << fullName << " " << errno << std::endl;
        return false;
    }

    if(read(fd, buffer, size) == -1)
    {
        std::cerr << "Error, couldn't read from " << fullName << " " << errno << std::endl;
        return false;
    }

    return true;
}
