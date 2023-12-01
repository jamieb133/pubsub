#include "IndirectTransporter.h"
#include "IClient.h"
#include "ISerialiser.h"

using namespace pubsub;

IndirectTransporter::IndirectTransporter(std::shared_ptr<ISerialiser> const serialiser,
                                            std::shared_ptr<IClient> const client) :
    mSerialiser { serialiser },
    mClient { client }
{}

void IndirectTransporter::send(std::shared_ptr<ITopic> const topic)
{
    size_t messageSize { mSerialiser->serialise(topic, mOutputBuffer) };
    mClient->send_bytes(mOutputBuffer, messageSize);
}

