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
    enum Statuses {CREATED,REFUSED,BUFFERED,SENTTOPROCESS,PROCESSED};
    struct Step
    {
        Step(Statuses status, Order order, std::list<Order> bufferQueue,
             std::shared_ptr<Processor> processorsInfo):
            status_(status),
            order_(order),
            bufferQueue_(bufferQueue),
            processorsInfo_(processorsInfo)
        {}
        Statuses status_;
        //std::map<Statuses, std::vector<std::shared_ptr<Order>>> cache_;
        Order order_;//std::shared_ptr<Order>
        std::list<Order> bufferQueue_;//std::shared_ptr<Order>
        //std::shared_ptr<ProcessorPool> processorPool_;
        std::shared_ptr<Processor> processorsInfo_;
        //not a shared_ptr. We need to save a copy

    };
    //double getRefusedProbability(unsigned long generatorID);

    //Timer::time getAverageTimeInSystem(unsigned long generatorID);

    //Timer::time getAverageTimeInProcessor(unsigned long generatorID);

   // Timer::time getAverageTimeInBuffer(unsigned long generatorID);

    //double getProcessorWorkFactor(unsigned long processorID, Timer::time fullTime);
    //double getBufferDispersion(unsigned long generatorID);
    //double getProcessorDispersion(unsigned long generatorID);
    std::shared_ptr<Step> getStep(int i);

private:
    std::shared_ptr<Timer> timer_;

    //std::vector<Step> cache_;

    std::vector<Step> steps;
    //std::vector<std::list<std::shared_ptr<Order>>> bufferSteps_;
    std::shared_ptr<BufferImpl> buffer_;
    std::shared_ptr<ProcessorPool> processorPool_;
};

#endif // STEPBYSTEPSIMULATIONLOGGER_HPP
