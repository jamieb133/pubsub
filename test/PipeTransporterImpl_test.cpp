#include <gtest/gtest.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "PipeTransporterImpl.h"

extern bool const read_pipe(std::string const& name,
                            std::vector<char>& buffer);

TEST(PipeTransporterImplTest, send_bytes) 
{
    std::string const pipename{ "test_pipe" };
    pubsub::PipeTransporterImpl impl{ pipename };

    bool success{false}; 
    bool finished{false}; 
    std::mutex mutex{};
    std::condition_variable cv{};
    std::vector<char> const writeBuffer{ 'a', 'b', 'c' };
    std::vector<char> readBuffer(writeBuffer.size());

    // Spawn thread to wait for data...
    std::thread t 
    {
        [&]()
        {
            std::lock_guard<std::mutex> lock { mutex };
            success = read_pipe(pipename, readBuffer);
            finished = true;
            cv.notify_all();
        }
    };

    impl.send_bytes(writeBuffer);
    std::unique_lock<std::mutex> lock { mutex };
    cv.wait(lock, [&finished](){ return finished; });
    t.join();

    EXPECT_EQ(readBuffer, writeBuffer);
}