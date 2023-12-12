#ifndef MFSIMULATOR_H
#define MFSIMULATOR_H

#include <QObject>
#include <array>
#include <QVector3D>

#define SIMULATION_DIMENSION (5)

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

    //SCENE description
    QVector3D CurrentFlowDirection = {1.0f, 0.0f, 0.0f};

protected:
    static MFSimulator* singleton;
};

#endif // MFSIMULATOR_H
