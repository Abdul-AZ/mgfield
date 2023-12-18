#include "simulationsettingsdialog.h"
#include "ui_simulationsettingsdialog.h"

#include "src/sim/mfsimulator.h"

SimulationSettingsDialog::SimulationSettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SimulationSettingsDialog)
{
    ui->setupUi(this);

    connect(this, &SimulationSettingsDialog::accepted, this, &SimulationSettingsDialog::SaveSimulatorSettings);
    setWindowTitle("Simulation Settings");

    auto simulator = MFSimulator::GetInstance();

    ui->DatapointsXSpinBox->setValue(simulator->SimulationNumDatapointsX);
    ui->DatapointsYSpinBox->setValue(simulator->SimulationNumDatapointsY);
    ui->DatapointsZSpinBox->setValue(simulator->SimulationNumDatapointsZ);

    ui->DatapointsXSpinBox->setMaximum(SIMULATION_SETTINGS_MAX_DATAPOINTS_PER_AXIS);
    ui->DatapointsYSpinBox->setMaximum(SIMULATION_SETTINGS_MAX_DATAPOINTS_PER_AXIS);
    ui->DatapointsZSpinBox->setMaximum(SIMULATION_SETTINGS_MAX_DATAPOINTS_PER_AXIS);
}

SimulationSettingsDialog::~SimulationSettingsDialog()
{
    delete ui;
}

void SimulationSettingsDialog::SaveSimulatorSettings()
{
    auto simulator = MFSimulator::GetInstance();

    simulator->SimulationNumDatapointsX = ui->DatapointsXSpinBox->value();
    simulator->SimulationNumDatapointsY = ui->DatapointsYSpinBox->value();
    simulator->SimulationNumDatapointsZ = ui->DatapointsZSpinBox->value();

    simulator->ClearResults();
}
