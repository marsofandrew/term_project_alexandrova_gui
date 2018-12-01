#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "fullsimulationlogger.hpp"

#include <QMainWindow>
#include <memory>

#include <library/include/interfaces/ProcessorPool.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bSimulate_pressed();

private:
    Ui::MainWindow *ui;

    int amountOfProcessors_;
    int amountOfGenerators_;
    int amountOfOrders_;
    int bufferSize_;
    double minTime_;
    double maxTime_;
    double lamda_;

    std::shared_ptr<Timer> timer_;

    const static QString FULL_SIMULATE;
    const static QString STEP_BY_STEP_SIMULATE;

    void showCriticalAlert(const QString &message);
    bool getVariables();

    void runStepByStepSimulation();
    void runFullSimulation();

    void showFullSimulationResults(std::shared_ptr<FullSimulationLogger> &logger, std::vector<std::shared_ptr<Processor>>& processors);
    void showStepByStepSimulationResults();
};

#endif // MAINWINDOW_HPP
