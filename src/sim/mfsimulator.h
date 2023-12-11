#ifndef MFSIMULATOR_H
#define MFSIMULATOR_H

#include <QObject>
#include <array>
#include <QVector3D>

#define SIMULATION_DIMENSION (3)

class MFSimulator : public QObject
{
    Q_OBJECT

public:
    MFSimulator();

    virtual ~MFSimulator() {};

    static MFSimulator *GetInstance();

public slots:
    void RequestNewSimulationRun();

signals:
    void SimulationStarted();
    void SimulationFinished();

public:
    std::array<std::array<std::array<QVector3D, SIMULATION_DIMENSION>, SIMULATION_DIMENSION>, SIMULATION_DIMENSION> SimulationResults;


protected:
    static MFSimulator* singleton;
};

#endif // MFSIMULATOR_H
