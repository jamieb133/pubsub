#include <functional>
#include <iostream>
#include <string>
#include <atomic>

#include "Context.hpp"
#include "DirectTransporter.h"
#include "IndirectTransporter.h"
#include "PipeClient.h"
#include "PipeServer.h"
#include "Deliverer.h"
#include "Receiver.h"
#include "BasicSerialiser.h"
#include "BasicDeserialiser.h"
#include "ExampleTopic.h"

using namespace pubsub;

int main()
{
    // Create a deliverer for mapping received topics to subscribers.
    auto deliverer = std::make_shared<Deliverer>();

    // A direct transporter will distribute messages in the same process with no serialisation.
    auto directTransporter = std::make_shared<pubsub::DirectTransporter>(deliverer);

    // An indirect transporter will pass serialised messages and can therefore be used across
    // processes or machines if an appropriate client is used. In this case, let's use the 
    // basic serialiser and a named pipe.
    std::string const pipename { "pubsub_example_pipe" };
    auto pipeClient = std::make_shared<pubsub::PipeClient>(pipename);
    auto serialiser = std::make_shared<pubsub::BasicSerialiser>();
    auto indirectTransporter = std::make_shared<pubsub::IndirectTransporter>(serialiser, pipeClient);

    // We need a receiver to receive serialised messages sent via the indirect transporter. It
    // will use a matching deserialiser and a pipe server with the same name. It also needs the
    // deliver to pass up valid messages to the subscribers.
    auto pipeServer = std::make_shared<pubsub::PipeServer>(pipename);
    auto deserialiser = std::make_shared<pubsub::BasicDeserialiser>();
    auto receiver = std::make_shared<pubsub::Receiver>(deliverer, deserialiser, pipeServer);

    pubsub::Context context{};
    context.add_deliverer(deliverer);
    context.add_transporter(indirectTransporter);
    context.add_receiver(receiver);

    std::function<void(std::shared_ptr<ExampleTopic> const)> callback 
    {
        [](std::shared_ptr<ExampleTopic> const topic)
        {
            std::cout << "Received " << topic->getMessage() << std::endl;
        }
    };

    context.subscribe(callback);

    for(int i = 0; i < 10; i++)
    {
        auto topic = std::make_shared<ExampleTopic>();
        topic->message = "hello";
        topic->uint8 = i;
        topic->uint16 = 4096U;
        topic->uint32 = 123456789U;

        context.publish(topic);
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
}
