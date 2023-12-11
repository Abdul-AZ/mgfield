#include "mfsimulator.h"

MFSimulator::MFSimulator() {}

MFSimulator* MFSimulator::singleton = nullptr;

void MFSimulator::RequestNewSimulationRun()
{
    emit SimulationStarted();

    for (auto& x : SimulationResults)
    {
        for (auto& y : x)
        {
            for (int i = 0; i < SIMULATION_DIMENSION; i++)
            {
                y[i] = QVector3D{1.0f, 1.0f, 1.0f};
            }
        }
    }

    emit SimulationFinished();
}

MFSimulator* MFSimulator::GetInstance()
{
    if(singleton == nullptr)
        singleton = new MFSimulator;

    return singleton;
}
