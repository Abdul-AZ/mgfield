#ifndef MFSIMULATOR_H
#define MFSIMULATOR_H

#include <QObject>
#include <array>
#include <QVector3D>

#include "../scene.h"

#define SIMULATION_DIMENSION (5)

class MFSimulator : public QObject
{
    Q_OBJECT

public:
    MFSimulator();

    virtual ~MFSimulator() {};

    static MFSimulator *GetInstance();

public slots:
    void RequestNewSimulationRun(Scene* scene);

signals:
    void SimulationStarted();
    void SimulationFinished();

private:
    void ResetResults();
    void CalculateContributionsFromCable(const TransmissionCable& cable);

public:
    Scene* TargetScene = nullptr;
    std::array<std::array<std::array<QVector3D, SIMULATION_DIMENSION>, SIMULATION_DIMENSION>, SIMULATION_DIMENSION> SimulationResults;

protected:
    static MFSimulator* singleton;
};

#endif // MFSIMULATOR_H
