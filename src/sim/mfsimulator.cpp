#include "mfsimulator.h"

#include <QDebug>

MFSimulator::MFSimulator() {}

MFSimulator* MFSimulator::singleton = nullptr;

void MFSimulator::RequestNewSimulationRun()
{
    emit SimulationStarted();

    for (int x = 0; x < SIMULATION_DIMENSION; x++)
        for (int y = 0; y < SIMULATION_DIMENSION; y++)
            for (int z = 0; z < SIMULATION_DIMENSION; z++)
            {
                QVector3D position = {(float)x - 2, (float)y - 2, (float)z - 2};
                SimulationResults[x][y][z] = QVector3D::crossProduct(CurrentFlowDirection, QVector3D{position.x(), position.y(), position.z()}).normalized();

                qCritical() << position << "RESULTS IN: " << SimulationResults[x][y][z];
            }

    emit SimulationFinished();
}

MFSimulator* MFSimulator::GetInstance()
{
    if(singleton == nullptr)
        singleton = new MFSimulator;

    return singleton;
}
