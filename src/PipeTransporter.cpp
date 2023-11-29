#include "PipeTransporter.h"
#include "PipeTransporterImpl.h"
#include "ISerialiser.h"

using namespace pubsub;

PipeTransporter::PipeTransporter(std::string const& name, 
                                    std::shared_ptr<ISerialiser> const serialiser,
                                    std::shared_ptr<IPipeTransporterImpl> const impl) :
    mSerialiser { serialiser },
    mImpl { impl }
{}

PipeTransporter::PipeTransporter(std::string const& name,
                                    std::shared_ptr<ISerialiser> const serialiser) :
    PipeTransporter { name, serialiser, std::make_shared<PipeTransporterImpl>(name) }
{}

void PipeTransporter::send(std::shared_ptr<ITopic> const topic)
{
    std::vector<char> serialisedMessage {};
    mSerialiser->serialise_topic(topic, serialisedMessage);
    mImpl->send_bytes(serialisedMessage);
}

