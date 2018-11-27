#ifndef FULLSIMULATIONLOGGER_HPP
#define FULLSIMULATIONLOGGER_HPP

#include <memory>
#include "library/include/interfaces/Logger.hpp"
#include "library/include/interfaces/Timer.hpp"

class FullSimulationLogger: public Logger
{
public:
    FullSimulationLogger() = default;

    void sendCratedOrder(const std::shared_ptr<Order> &order) override;

    void sendRefusedOrder(const std::shared_ptr<Order> &order) override;

    void sendProcessedOrder(const std::shared_ptr<Order> &order) override;

    void sendBufferedOrder(const std::shared_ptr<Order> &order) override;

    void sendGetOrderFromBuffer(const std::shared_ptr<Order> &order) override;

    void sendAddingOrderToBuffer(const std::shared_ptr<Order> &order) override;

    void sendOrderToProcessor(const std::shared_ptr<Order> &order,
                                      const std::shared_ptr<Processor> &processor) override;

    void sendMessage(const Type &type, const std::string &message) override;

    void setTimer(const std::shared_ptr<Timer> &timer) override;

private:
    std::shared_ptr<Timer> timer_;
};

#endif // FULLSIMULATIONLOGGER_HPP
