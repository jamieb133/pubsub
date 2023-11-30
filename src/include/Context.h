#pragma once

#include <memory>
#include <functional>
#include <vector>

namespace pubsub
{
    class ITopic;
    class ITransporter;
    class IDeliverer;
    class IServer;

    class Context
    {
    private:
        std::vector<std::shared_ptr<IDeliverer>> mDeliverers{};
        std::vector<std::shared_ptr<ITransporter>> mTransporters{};
        std::vector<std::shared_ptr<IServer>> mServers{};
    public:
        void publish(std::shared_ptr<ITopic> topic);
        template <class TopicType> 
        void subscribe(std::function<void(std::shared_ptr<TopicType>)> callback);
        void add_transporter(std::shared_ptr<ITransporter> transporter);
        void add_deliverer(std::shared_ptr<IDeliverer> deliverer);
        void add_server(std::shared_ptr<IServer> server);
    };
}
