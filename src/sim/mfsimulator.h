#ifndef MFSIMULATOR_H
#define MFSIMULATOR_H

#include <QObject>
#include <QVector3D>

#include "../../transmissioncable.h"

#include "../scene.h"

class MFSimulator : public QObject
{
    Q_OBJECT

public:
    MFSimulator();

    virtual ~MFSimulator() {};

    static MFSimulator *GetInstance();
    void ClearResults();

    inline size_t   GetResultsElementIndex(int32_t x, int32_t y, int32_t z);
    const QVector3D GetResult(int32_t x, int32_t y, int32_t z);
    const QVector3D GetPosition(int32_t x, int32_t y, int32_t z);

public slots:
    void RequestNewSimulationRun(Scene* scene);

signals:
    void SimulationStarted();
    void SimulationFinished();

private:
    void CalculateContributionsFromCable(const TransmissionCable& cable);

public:
    Scene* TargetScene = nullptr;

    int32_t SimulationNumDatapointsX = 5;
    int32_t SimulationNumDatapointsY = 5;
    int32_t SimulationNumDatapointsZ = 5;

    bool                   SimulationResultsExist = false;
    std::vector<QVector3D> SimulationResults;

protected:
    static MFSimulator* singleton;
};

#endif // MFSIMULATOR_H
