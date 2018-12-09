#ifndef STEPBYSTEPSIMULATIONLOGGER_HPP
#define STEPBYSTEPSIMULATIONLOGGER_HPP

#include <map>
#include <vector>
#include <list>
#include <library/include/interfaces/Logger.hpp>

class Order;
class GeneratorPool;
class ProcessorImpl;
class ProcessorPool;
class BufferImpl;

class StepByStepSimulationLogger: public Logger
{
public:
    StepByStepSimulationLogger(const std::shared_ptr<GeneratorPool>& generatorPool, std::shared_ptr<BufferImpl>& buffer,
                               std::shared_ptr<ProcessorPool>& processorPool);

    void sendCreatedOrder(const std::shared_ptr<Order> &order) override;
    void sendRefusedOrder(const std::shared_ptr<Order> &order) override;
    void sendProcessedOrder(const std::shared_ptr<Order> &order) override;
    void sendBufferedOrder(const std::shared_ptr<Order> &order) override;
    void sendGetOrderFromBuffer(const std::shared_ptr<Order> &order) override;
    void sendAddingOrderToBuffer(const std::shared_ptr<Order> &order) override;
    void sendOrderToProcessor(const std::shared_ptr<Order> &order, const std::shared_ptr<Processor> &processor) override;
    void sendMessage(const Type &type, const std::string &message) override;
    void setTimer(const std::shared_ptr<Timer> &timer) override;

    std::vector<unsigned long> getGeneratorsIDs();

    std::size_t getAmountOfCreatedOrders(unsigned long generatorID);
    enum Statuses {CREATED,REFUSED,ADDING,BUFFERED,SENTTOPROCESS,PROCESSED};
    struct Step
    {
        Step(Statuses status, Order order, std::list<Order> bufferQueue):
            status_(status),
            order_(order),
            bufferQueue_(bufferQueue)
        {}
        Statuses status_;
        Order order_;
        std::list<Order> bufferQueue_;
    };
    std::shared_ptr<Step> getStep(int i);

private:
    std::shared_ptr<Timer> timer_;
    std::vector<Step> steps;
    std::shared_ptr<BufferImpl> buffer_;
    std::shared_ptr<ProcessorPool> processorPool_;
};

#endif // STEPBYSTEPSIMULATIONLOGGER_HPP
