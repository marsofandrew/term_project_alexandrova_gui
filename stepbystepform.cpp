#include "stepbystepform.h"
#include "ui_stepbystepform.h"
#include <QTableWidget>
#include <QMessageBox>

namespace details{
    static void updateSystemTable(Ui::StepByStepForm *ui,Timer::time time,unsigned long ordGenId, unsigned long ordId,
           std::string state,QString proc)
    //because proc can be both string null and number of processor
    {
        int tempRow = ui->tableSystem->rowCount();
        ui->tableSystem->insertRow( tempRow );
        ui->tableSystem->setItem(tempRow, 0, new QTableWidgetItem(QString::number(time)));
        ui->tableSystem->setItem(tempRow, 1, new QTableWidgetItem(QString::number(ordGenId)));
        ui->tableSystem->setItem(tempRow, 2, new QTableWidgetItem(QString::number(ordGenId)+"."+QString::number(ordId)));
        ui->tableSystem->setItem(tempRow, 3, new QTableWidgetItem(QString::fromStdString(state)));
        ui->tableSystem->setItem(tempRow, 4, new QTableWidgetItem(proc));

        static QStringList verticalHeader;
        verticalHeader.append("Step" + QString::number(tempRow));
        ui->tableSystem->setVerticalHeaderLabels(verticalHeader);
    }

    static void updateBufferTable(Ui::StepByStepForm *ui,std::list<Order>& buffer,
           int bufferRealSize)
    {
        std::list<Order>::iterator it;
        for(int i = 0; i < bufferRealSize; i++){
            it = std::next(buffer.begin(),i);
            if (i<buffer.size()) {
                unsigned long ordGenId = (*it).getGenerator()->getId();
                ui->tableBuffer->setItem(i, 0, new QTableWidgetItem(QString::number(ordGenId)));
                ui->tableBuffer->setItem(i, 1, new QTableWidgetItem(QString::number(ordGenId)+"."+QString::number((*it).getId())));
                ui->tableBuffer->setItem(i, 2, new QTableWidgetItem(QString::number((*it).getGeneratedTime())));
                //TODO when senttoprocess call update on buffer

            } else {
                ui->tableBuffer->setItem(i, 0, new QTableWidgetItem("null"));
                ui->tableBuffer->setItem(i, 1, new QTableWidgetItem("null"));
                ui->tableBuffer->setItem(i, 2, new QTableWidgetItem("null"));
            }
        }
    }

    static void updateProcessorTable(Ui::StepByStepForm *ui,Order& ord, std::string state)
    {
        int procId = ord.getProcessor()->getId();
        unsigned long ordGenId =  ord.getGenerator()->getId();
        ui->tableProcessor->setItem(procId, 0, new QTableWidgetItem(QString::fromStdString(state)));
        ui->tableProcessor->setItem(procId, 3, new QTableWidgetItem(QString::number(ordGenId)+"."+QString::number(ord.getId())));
        if (state=="Busy")
        {
            ui->tableProcessor->setItem(procId, 1, new QTableWidgetItem(QString::number(ord.getStartProcessTime())));
            ui->tableProcessor->setItem(procId, 2, new QTableWidgetItem("null"));

        }
        else
        {
            Timer::time finishTime = ord.getStartProcessTime() + ord.getProcessor()->getTimeToNextEvent();
            ui->tableProcessor->setItem(procId, 1, new QTableWidgetItem("null"));
            ui->tableProcessor->setItem(procId, 2, new QTableWidgetItem(QString::number(finishTime)));
        }

    }

    static void onBufferedAction(int step,  int bufferSize, Ui::StepByStepForm *ui,
               std::shared_ptr<StepByStepSimulationLogger> logger_)
    {
        std::list<Order> buffer = logger_->getStep(step)->bufferQueue_;

        Order ord = logger_->getStep(step)->order_;
        updateSystemTable(ui,ord.getInsertionTime(),ord.getGenerator()->getId(),ord.getId(),
              "Buffered", QString::fromStdString("null"));
        updateBufferTable(ui, buffer, bufferSize);
    }

    static void onRefusedAction(int step,  int bufferSize, Ui::StepByStepForm *ui,
               std::shared_ptr<StepByStepSimulationLogger> logger_)
    {
        Order ord = logger_->getStep(step)->order_;
        updateSystemTable(ui, ord.getRefusedTime(), ord.getGenerator()->getId(), ord.getId(),
              "Refused", QString::fromStdString("null"));

        std::list<Order> buffer = logger_->getStep(step)->bufferQueue_;
        updateBufferTable(ui, buffer, bufferSize);
    }
    static void onSentToProcessAction(int step,  int bufferSize, Ui::StepByStepForm *ui,
               std::shared_ptr<StepByStepSimulationLogger> logger_)
    {
        Order ord = logger_->getStep(step)->order_;
        updateSystemTable(ui, ord.getStartProcessTime(), ord.getGenerator()->getId(), ord.getId(),
               "SentToProcess", QString::number(ord.getProcessor()->getId()));

        std::list<Order> buffer = logger_->getStep(step)->bufferQueue_;
        updateBufferTable(ui, buffer, bufferSize);

        updateProcessorTable(ui,ord,"Busy");
    }

    static void onProcessedAction(int step,  int, Ui::StepByStepForm *ui,
               std::shared_ptr<StepByStepSimulationLogger> logger_)
    {
        Order ord = logger_->getStep(step)->order_;
        updateSystemTable(ui, ord.getFinishProcessingTime(), ord.getGenerator()->getId(), ord.getId(),
               "Processed", QString::number(ord.getProcessor()->getId()));

        updateProcessorTable(ui,ord,"Free");

    }

    static void onCreatedAction(int step,  int, Ui::StepByStepForm *ui,
               std::shared_ptr<StepByStepSimulationLogger> logger_)
    {
        Order ord = logger_->getStep(step)->order_;
        updateSystemTable(ui, ord.getGeneratedTime(), ord.getGenerator()->getId(), ord.getId(),
               "Created",  QString::fromStdString("null"));
    }

    static void onAddingAction(int step,  int bufferSize, Ui::StepByStepForm *ui,
               std::shared_ptr<StepByStepSimulationLogger> logger_)
    {
        std::list<Order> buffer = logger_->getStep(step)->bufferQueue_;

        Order ord = logger_->getStep(step)->order_;
        //trying to add an order to buffer
        updateSystemTable(ui,ord.getGeneratedTime(), ord.getGenerator()->getId(), ord.getId(),
              "Adding", QString::fromStdString("null"));
        updateBufferTable(ui, buffer, bufferSize);
    }

}

StepByStepForm::StepByStepForm(QWidget *parent,std::shared_ptr<StepByStepSimulationLogger> logger,
                               int amountOfGenerators,int amountOfProcessors,int bufferSize) :
    QDialog(parent),
    ui(new Ui::StepByStepForm),
    logger_(logger),
    amountOfGenerators_(amountOfGenerators),
    amountOfProcessors_(amountOfProcessors),
    bufferSize_(bufferSize),
    stepNum(0)
{

    actions[StepByStepSimulationLogger::CREATED]=details::onCreatedAction;
    actions[StepByStepSimulationLogger::BUFFERED]=details::onBufferedAction;
    actions[StepByStepSimulationLogger::REFUSED]=details::onRefusedAction;
    actions[StepByStepSimulationLogger::SENTTOPROCESS]=details::onSentToProcessAction;
    actions[StepByStepSimulationLogger::PROCESSED]=details::onProcessedAction;
    actions[StepByStepSimulationLogger::ADDING]=details::onAddingAction;

    ui->setupUi(this);
    this->setFixedSize(1200,700);
    setUpBufferTable();
    setUpSystemTable();
    setUpProcessorTable();
}

void StepByStepForm::setUpBufferTable()
{
    QStringList horizontalHeader;

    horizontalHeader.append("GenNum");
    horizontalHeader.append("OrderNum");
    horizontalHeader.append("GenTime");
    QStringList verticalHeader;

    for (int i=0; i<bufferSize_;++i)
    {
        verticalHeader.append("BufCell" + QString::number(i));
    }
    ui->tableBuffer->setFixedSize(301,660);
    ui->tableBuffer->setColumnCount(3);
    ui->tableBuffer->setRowCount(bufferSize_);
    ui->tableBuffer->setShowGrid(true);

    ui->tableBuffer->setHorizontalHeaderLabels(horizontalHeader);
    ui->tableBuffer->setVerticalHeaderLabels(verticalHeader);
    ui->tableBuffer->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

}

void StepByStepForm::setUpSystemTable()
{
    QStringList horizontalHeader;

    horizontalHeader.append("Time");
    horizontalHeader.append("OrdGen");
    horizontalHeader.append("OrdNum");
    horizontalHeader.append("OrdState");
    horizontalHeader.append("ProcId");

    ui->tableSystem->setFixedSize(521,660);
    ui->tableSystem->setColumnCount(5);
    ui->tableSystem->setRowCount(0);
    ui->tableSystem->setShowGrid(true);

    ui->tableSystem->setHorizontalHeaderLabels(horizontalHeader);
    ui->tableSystem->setColumnWidth(0, 70);
    ui->tableSystem->setColumnWidth(1, 70);
    ui->tableSystem->setColumnWidth(2, 70);
    ui->tableSystem->setColumnWidth(3, 130);
    ui->tableSystem->setColumnWidth(4, 65);
}

void StepByStepForm::setUpProcessorTable()
{
    QStringList horizontalHeader;

    horizontalHeader.append("State");
    horizontalHeader.append("TimeStart");
    horizontalHeader.append("TimeFinish");
    horizontalHeader.append("OrderNum");
    QStringList verticalHeader;

    for (int i=0; i<amountOfProcessors_;++i)
    {
        verticalHeader.append("Proc" + QString::number(i));
    }
    ui->tableProcessor->setFixedSize(351,660);
    ui->tableProcessor->setColumnCount(4);
    ui->tableProcessor->setRowCount(amountOfProcessors_);
    ui->tableProcessor->setShowGrid(true);

    ui->tableProcessor->setHorizontalHeaderLabels(horizontalHeader);
    ui->tableProcessor->setVerticalHeaderLabels(verticalHeader);
    ui->tableProcessor->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

StepByStepForm::~StepByStepForm()
{
    delete ui;
}

void StepByStepForm::getNextAction(int i)
{
    std::shared_ptr<StepByStepSimulationLogger::Step> step = logger_->getStep(i);
    if (step==nullptr)
    {
        QMessageBox::warning(this, "Done","The modeling of the system has been finished");
        return;
    }
    StepByStepSimulationLogger::Statuses status = (*step).status_;
    actions[status](i,bufferSize_,ui,logger_);
}

void StepByStepForm::on_nextStepButton_clicked()
{
    getNextAction(stepNum++);
    ui->tableSystem->scrollToBottom();
}
