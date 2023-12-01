#include <gtest/gtest.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <array>
#include "PipeClient.h"
#include "pubsub_macros.h"

extern bool const read_pipe(std::string const& name,
                            char* buffer,
                            size_t size);

TEST(PipeClientTest, send_bytes) 
{
    std::string const pipename{ "test_pipe" };
    pubsub::PipeClient client{ pipename };

    bool success{false}; 
    bool finished{false}; 
    std::mutex mutex{};
    std::condition_variable cv{};
    std::array<char, pubsub::MAXIMUM_BUFFER_SIZE> const writeBuffer{ 'a', 'b', 'c' };
    std::array<char, 3> readBuffer{};

    // Spawn thread to wait for data...
    std::thread t 
    {
        [&]()
        {
            std::lock_guard<std::mutex> lock { mutex };
            success = read_pipe(pipename, readBuffer.data(), readBuffer.size());
            finished = true;
            cv.notify_all();
        }
    };

    client.send_bytes(writeBuffer, writeBuffer.size());
    std::unique_lock<std::mutex> lock { mutex };
    cv.wait(lock, [&finished](){ return finished; });
    t.join();

    // Allow comparison using gtest macro with a vector.
    std::vector<char> inputBuffer { readBuffer.begin(), readBuffer.end() };
    std::vector<char> outputBuffer { writeBuffer.begin(), writeBuffer.end() };

    EXPECT_EQ(inputBuffer, inputBuffer);
}