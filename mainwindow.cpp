#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <memory>
#include <QMessageBox>
#include "fullsimulationlogger.hpp"
#include "stepbystepsimulationlogger.hpp"
#include "stepbystepform.h"
#include "library/include/interfaces/Buffer.hpp"
#include <library/include/interfaces/WorkCondition.hpp>
#include <code/BufferImpl.hpp>
#include <library/include/Condition.hpp>
#include <code/ProcessorPoolImpl.hpp>
#include <code/SupportiveFunctions.hpp>
#include <library/include/SimpleGeneratorPool.hpp>
#include <library/include/SimpleTimer.hpp>
#include <library/include/Worker.hpp>

const QString MainWindow::FULL_SIMULATE = "Full simulate";
const QString MainWindow::STEP_BY_STEP_SIMULATE = "Step by step";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    amountOfProcessors_(-1),
    amountOfGenerators_(-1),
    amountOfOrders_(-1),
    minTime_(-1),
    maxTime_(-1),
    lamda_(-1),
    timer_(std::make_shared<SimpleTimer>()),
    bufferSize_(-1)

{
    ui->setupUi(this);
    ui->comboBox->addItem(FULL_SIMULATE);
    ui->comboBox->addItem(STEP_BY_STEP_SIMULATE);
    ui->fullTimeSimulationScreen->setEnabled(false);
    ui->fullTimeSimulationScreen->setHidden(true);
    this->setFixedSize(800,600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showCriticalAlert(const QString &message){
    QMessageBox messageBox;
    messageBox.critical(this, "Error", message);
    messageBox.setFixedSize(500,200);
}

bool MainWindow::getVariables(){
    bool ok = false;
    amountOfGenerators_ = ui->tGemeratorAmount_2->text().toInt(&ok);
    if (!ok || amountOfGenerators_ <= 0){
        showCriticalAlert("Amount of generators must be positive integer");
        return false;
    }

    minTime_ = ui->tA_2->text().toDouble(&ok);
    if (!ok || minTime_ < 0){
        showCriticalAlert("A must be a non negative double");
        return false;
    }

    maxTime_ = ui->tB_2->text().toDouble(&ok);
    if (!ok || maxTime_ < 0){
        showCriticalAlert("B must be a non negative double");
        return false;
    }

    amountOfProcessors_ = ui->tProcessorAmount_2->text().toInt(&ok);
    if (!ok || amountOfGenerators_ <= 0){
        showCriticalAlert("Amount of processors must be positive integer");
        return false;
    }
    lamda_ = ui->tLamda_2->text().toDouble(&ok);
    if (!ok || lamda_ <= 0){
        showCriticalAlert("Lamda must be a non negative double");
        return false;
    }

    amountOfOrders_ = ui->tOrderAmount_2->text().toInt(&ok);
    if (!ok || amountOfOrders_ <= 0){
        showCriticalAlert("Amount of orders must be positive integer");
        return false;
    }
    bufferSize_ = ui->tBufferSize->text().toInt(&ok);
    if (!ok || bufferSize_ < 0){
        showCriticalAlert("Buffer size must be positive integer or 0");
        return false;
    }
    return true;
}

void MainWindow::runFullSimulation(){
    std::shared_ptr<Buffer> buffer = std::make_shared<BufferImpl>(BufferImpl(static_cast<std::size_t>(bufferSize_)));
    std::vector<int> priorities;

    for (int i = 0; i<amountOfGenerators_; ++i){
        priorities.push_back(i);
    }

    std::vector<double> lamdas;
    for (int i = 0; i< amountOfProcessors_; i++){
        lamdas.push_back(lamda_);
    }
    std::vector<std::shared_ptr<Processor>> processors = SupportiveFunctions::createProcessors(lamdas);
    std::shared_ptr<ProcessorPool> processorPool = std::make_shared<ProcessorPoolImpl>(processors);
    std::shared_ptr<GeneratorPool> generatorPool = std::make_shared<SimpleGeneratorPool>(SupportiveFunctions::createGenerators(priorities, minTime_, maxTime_));
    std::shared_ptr<FullSimulationLogger> fullSimulationLogger = std::make_shared<FullSimulationLogger>(FullSimulationLogger(generatorPool));
    std::shared_ptr<WorkCondition> condition = std::make_shared<Condition>(Condition(amountOfOrders_, fullSimulationLogger));
    Worker worker(generatorPool, processorPool, buffer,timer_ ,condition, fullSimulationLogger);
    ui->mainScreen->setEnabled(false);
    ui->mainScreen->setHidden(true);
    ui->fullTimeSimulationScreen->setHidden(false);
    ui->fullTimeSimulationScreen->setEnabled(true);
    worker.run();
    showFullSimulationResults(fullSimulationLogger, processors, generatorPool);
}

void MainWindow::showFullSimulationResults(std::shared_ptr<FullSimulationLogger> &logger, std::vector<std::shared_ptr<Processor>>& processors, std::shared_ptr<GeneratorPool> &generatorPool)
{
    ui->tableWidget->setFixedSize(800,400);
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(amountOfGenerators_ + 1);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget_2->setRowCount(amountOfProcessors_);
    ui->tableWidget_2->setColumnCount(1);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList("Efficiency"));

    QStringList verticalHeaderProc;
    for (auto processor: processors){
        verticalHeaderProc.append("Proc" + QString::number(processor->getId()));
    }
    ui->tableWidget_2->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->tableWidget_2->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);

    for (std::size_t i =0; i<processors.size(); i++){
        ui->tableWidget_2->setItem(i,0, new QTableWidgetItem(QString::number(logger->getProcessorWorkFactor(processors.at(i)->getId(), timer_->getCurrentTime()))));
    }

    QStringList horizontalHeader;
    horizontalHeader.append("Orders");
    horizontalHeader.append("Failure Prob");
    horizontalHeader.append("System time");
    horizontalHeader.append("Buffer time");
    horizontalHeader.append("Proc time");
    horizontalHeader.append("Buffer disp");
    horizontalHeader.append("Proc disp");

    QStringList verticalHeader;

    auto ids = logger->getGeneratorsIDs();

    for (unsigned long id: ids){
        verticalHeader.append("Gen" + QString::number(id));
    }
        verticalHeader.append("All");
    ui->tableWidget->setHorizontalHeaderLabels(horizontalHeader);
    ui->tableWidget->setVerticalHeaderLabels(verticalHeader);
    ui->tableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

    for(std::size_t i = 0; i < ids.size(); i++){
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(logger->getAmountOfCreatedOrders(ids[i]))));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(logger->getRefusedProbability(ids[i]))));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(logger->getAverageTimeInSystem(ids[i]))));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(logger->getAverageTimeInBuffer(ids[i]))));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(logger->getAverageTimeInProcessor(ids[i]))));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(logger->getBufferDispersion(ids[i]))));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(logger->getProcessorDispersion(ids[i]))));
    }

    ui->tableWidget->setItem(ids.size(), 0, new QTableWidgetItem(QString::number(generatorPool->getAmountOfGeneratedOrders())));
    ui->tableWidget->setItem(ids.size(), 1, new QTableWidgetItem(QString::number(logger->getAverageRefusedProbability())));
}

void MainWindow::runStepByStepSimulation(){
    //TODO StepByStepSimulation
    //simulate here then pass logger to the StepByStepForm
    std::shared_ptr<BufferImpl> buffer = std::make_shared<BufferImpl>(BufferImpl(static_cast<std::size_t>(bufferSize_)));

    std::vector<int> priorities;

    for (int i = 0; i<amountOfGenerators_; ++i){
        priorities.push_back(i);
    }

    std::vector<double> lamdas;
    for (int i = 0; i< amountOfProcessors_; i++){
        lamdas.push_back(lamda_);
    }
    std::vector<std::shared_ptr<Processor>> processors = SupportiveFunctions::createProcessors(lamdas);
    std::shared_ptr<ProcessorPool> processorPool = std::make_shared<ProcessorPoolImpl>(processors);

    std::shared_ptr<GeneratorPool> generatorPool = std::make_shared<SimpleGeneratorPool>(SupportiveFunctions::createGenerators(priorities, minTime_, maxTime_));
    std::shared_ptr<StepByStepSimulationLogger> stepByStepSimulationLogger = std::make_shared<StepByStepSimulationLogger>(
                StepByStepSimulationLogger(generatorPool,buffer,processorPool));

    std::shared_ptr<WorkCondition> condition = std::make_shared<Condition>(Condition(amountOfOrders_, stepByStepSimulationLogger));
    Worker worker(generatorPool, processorPool, buffer,timer_ ,condition, stepByStepSimulationLogger);

    /*ui->mainScreen->setEnabled(false);
    ui->mainScreen->setHidden(true);
    ui->fullTimeSimulationScreen->setHidden(false);
    ui->fullTimeSimulationScreen->setEnabled(true);*/
    worker.run();
    showStepByStepSimulationResults(stepByStepSimulationLogger,processors);
}

void MainWindow::showStepByStepSimulationResults(std::shared_ptr<StepByStepSimulationLogger> &logger, std::vector<std::shared_ptr<Processor>>& processors)
{
    std::shared_ptr<StepByStepForm> sbs = std::make_shared<StepByStepForm>(nullptr,
                       logger,
                       amountOfGenerators_,
                       amountOfProcessors_,
                       bufferSize_);
    sbs->setModal(true);
    sbs->exec();
   // this->hide();
}

void MainWindow::on_bSimulate_pressed()
{
    if (getVariables()){
        if (ui->comboBox->currentText() == FULL_SIMULATE){
            runFullSimulation();
        } else if (ui->comboBox->currentText() == STEP_BY_STEP_SIMULATE){
            runStepByStepSimulation();
            //QMessageBox::warning(this, "Внимание","Это очень важный текст");

 /*std::shared_ptr<StepByStepSimulationLogger> l;
            StepByStepForm sbs(this,l,7,5,10);
            sbs.setModal(true);
            sbs.exec();*/

        }
    }
}
