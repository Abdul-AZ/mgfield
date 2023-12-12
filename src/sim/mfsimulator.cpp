#include "mfsimulator.h"

#include <QDebug>

MFSimulator::MFSimulator() {}

MFSimulator* MFSimulator::singleton = nullptr;

void MFSimulator::RequestNewSimulationRun(Scene* scene)
{
    TargetScene = scene;

    emit SimulationStarted();

    ResetResults();

    foreach (auto& cable, scene->Cables)
        CalculateContributionsFromCable(*cable);

    emit SimulationFinished();
}

MFSimulator* MFSimulator::GetInstance()
{
    if(singleton == nullptr)
        singleton = new MFSimulator;

    return singleton;
}

void MFSimulator::ResetResults()
{
    for (int x = 0; x < SIMULATION_DIMENSION; x++)
        for (int y = 0; y < SIMULATION_DIMENSION; y++)
            for (int z = 0; z < SIMULATION_DIMENSION; z++)
                SimulationResults[x][y][z] = QVector3D(0.0f, 0.0f, 0.0f);
}

void MFSimulator::CalculateContributionsFromCable(const TransmissionCable& cable)
{
    for (int x = 0; x < SIMULATION_DIMENSION; x++)
        for (int y = 0; y < SIMULATION_DIMENSION; y++)
            for (int z = 0; z < SIMULATION_DIMENSION; z++)
            {
                QVector3D position = {(float)x - 2, (float)y - 2, (float)z - 2};

                float magnitude = position.distanceToLine(cable.GetPosition(), cable.GetDirection());
                QVector3D direction =  QVector3D::crossProduct(cable.GetDirection(), QVector3D{position.x(), position.y(), position.z()}).normalized();

                SimulationResults[x][y][z] += magnitude * direction;
            }
}
