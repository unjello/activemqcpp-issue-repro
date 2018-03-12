#define _HAS_AUTO_PTR_ETC 1	// required by ActiveMQ 3.9.4
#include <memory>

#pragma warning(push)
#pragma warning(disable: 4101)
#include <activemq/library/ActiveMQCPP.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQSession.h>
#pragma warning (pop)


int main() {
    const char* brokerUrl = "failover:(tcp://localhost:61616)?transport.useInactivityMonitor=false&wireFormat.maxInactivityDuration=0&startupMaxReconnectAttempts=5&maxReconnectAttempts=10&maxReconnectDelay=1000&timeout=-1";
    const char* requestQueueName = "Q.REQUEST";
    const char* responseQueueName = "Q.RESPONSE";
    const char* requestText = "{ request: true }";
    int timeToLive = 60;
    int timeToWait = 60;

    activemq::library::ActiveMQCPP::initializeLibrary();

    try { 
        do {
            auto connectionFactory = std::make_unique<activemq::core::ActiveMQConnectionFactory>(brokerUrl);
            std::unique_ptr<cms::Connection> connection{ connectionFactory->createConnection() };
            connection->start();

            std::unique_ptr<cms::Session> session{ connection->createSession(cms::Session::AUTO_ACKNOWLEDGE) };
            std::unique_ptr<cms::Destination> requestQueue{ session->createQueue(requestQueueName) };
            std::unique_ptr<cms::Destination> responseQueue{ session->createQueue(responseQueueName) };

            std::unique_ptr<cms::TextMessage> requestMessage{ session->createTextMessage(requestText) };
            std::unique_ptr<cms::MessageProducer> producer{ session->createProducer(requestQueue.get()) };

            producer->setDeliveryMode(cms::DeliveryMode::NON_PERSISTENT);
            producer->setTimeToLive(timeToLive*1000);
            producer->send(requestMessage.get());
            producer->close();

            std::unique_ptr<cms::MessageConsumer> consumer{ session->createConsumer(responseQueue.get()) };
            std::unique_ptr<const cms::Message> responseMessage{ consumer->receive(timeToWait*1000) };
            consumer->close();

            session->close();
            connection->close();
        } while(true);
    } catch (cms::CMSException& ex) {
        std::cout << "ActiveMQ exception: " << ex.getMessage() << "\n";
    } catch (std::exception& ex) {
        std::cout << "Standard exception: " << ex.what() << "\n";
    } catch (...) {
        std::cout << "Wierd.\n";
    }

    activemq::library::ActiveMQCPP::shutdownLibrary();
    return 0;
}

