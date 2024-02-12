#ifndef MFSIMULATOR_H
#define MFSIMULATOR_H

#include <QObject>
#include <QVector3D>

#include "../transmissioncable.h"

#include "../scene.h"

#define SIM_CONSTANT_PI                           (3.14159265   )
#define SIM_CONSTANT_VACUUM_MAGNETIC_PERMEABILITY (1.25663706e-6)

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

    int32_t SimulationNumDatapointsX    = 5;
    int32_t SimulationNumDatapointsY    = 5;
    int32_t SimulationNumDatapointsZ    = 5;
    float   SimulationDatapointsSpacing = 1.0f;

    bool                   SimulationResultsExist = false;

    // The resultant values are a multiple of SIM_CONSTANT_VACUUM_MAGNETIC_PERMEABILITY
    std::vector<QVector3D> SimulationResults;
    float                  SimulationResultsMaxMagnitude = 0.0f;
    float                  SimulationResultsMinMagnitude = 0.0f;

protected:
    static MFSimulator* singleton;
};

#endif // MFSIMULATOR_H
