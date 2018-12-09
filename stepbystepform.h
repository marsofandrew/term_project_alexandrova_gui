#ifndef FORMTODELETE_H
#define FORMTODELETE_H
#include "stepbystepsimulationlogger.hpp"
#include <vector>
#include <QWidget>
#include <QTableWidget>
#include <QDialog>
#include <QWindow>
#include <memory>
#include <map>
#include <functional>
namespace Ui {
class StepByStepForm;
}

class StepByStepForm : public QDialog
{
    Q_OBJECT

public:
    explicit StepByStepForm(QWidget *parent = 0);
    StepByStepForm(QWidget *parent = 0,std::shared_ptr<StepByStepSimulationLogger> logger=nullptr,
                   int amountOfGenerators=0,int amountOfProcessors=0,int bufferSize=0);
    void setUpBufferTable();
    void setUpSystemTable();
    void setUpProcessorTable();
    void getNextAction(int i);
    ~StepByStepForm();

private slots:
    void on_nextStepButton_clicked();

private:
    int stepNum;
    Ui::StepByStepForm *ui;
    std::shared_ptr<StepByStepSimulationLogger> logger_;
    int amountOfGenerators_;
    int amountOfProcessors_;
    int bufferSize_;
    std::map<StepByStepSimulationLogger::Statuses, std::function<void(int, int,Ui::StepByStepForm *,
            std::shared_ptr<StepByStepSimulationLogger>)> > actions;
};

#endif // FORMTODELETE_H
