#ifndef SIMULATIONSETTINGSDIALOG_H
#define SIMULATIONSETTINGSDIALOG_H

#include <QDialog>

#define SIMULATION_SETTINGS_MAX_DATAPOINTS_PER_AXIS (50)

namespace Ui {
class SimulationSettingsDialog;
}

class SimulationSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SimulationSettingsDialog(QWidget *parent = nullptr);
    ~SimulationSettingsDialog();

private slots:
    void SaveSimulatorSettings();

private:
    Ui::SimulationSettingsDialog *ui;
};

#endif // SIMULATIONSETTINGSDIALOG_H
