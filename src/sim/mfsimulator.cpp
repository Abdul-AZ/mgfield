#include "mfsimulator.h"

#include <algorithm>
#include <QDebug>

MFSimulator::MFSimulator() {}

MFSimulator* MFSimulator::singleton = nullptr;

void MFSimulator::RequestNewSimulationRun(Scene* scene)
{
    TargetScene = scene;

    emit SimulationStarted();

    ClearResults();
    SimulationResults.resize(SimulationNumDatapointsX * SimulationNumDatapointsY * SimulationNumDatapointsZ);

    foreach (auto& object, scene->Objects)
    {
        switch (object->Type) {
        case CurrentCarryingCable:
            CalculateContributionsFromCable(*(TransmissionCable*)object.get());
            continue;
        }
    }

    auto[minIterator, maxIterator] = std::minmax_element(SimulationResults.begin(), SimulationResults.end(), [](const QVector3D& a, const QVector3D& b) -> bool
    {
        return a.lengthSquared() < b.lengthSquared();
    });

    SimulationResultsMinMagnitude = (*minIterator).length();
    SimulationResultsMaxMagnitude = (*maxIterator).length();

    SimulationResultsExist = true;
    emit SimulationFinished();
}

MFSimulator* MFSimulator::GetInstance()
{
    if(singleton == nullptr)
        singleton = new MFSimulator;

    return singleton;
}

void MFSimulator::ClearResults()
{
    SimulationResultsExist = false;
    SimulationResults.clear();

    emit SimulationFinished();
}

size_t MFSimulator::GetResultsElementIndex(int32_t x, int32_t y, int32_t z)
{
    int index = (z * SimulationNumDatapointsX * SimulationNumDatapointsY) + (y * SimulationNumDatapointsX) + x;

    if(index < 0 || index >= SimulationNumDatapointsX * SimulationNumDatapointsY * SimulationNumDatapointsZ)
    {
        qFatal() << "Invalid simulation results access coords, value in (" << x << ", " << y << ", " << z << ") does not exist";
        return 0;
    }

    return index;
}

const QVector3D MFSimulator::GetResult(int32_t x, int32_t y, int32_t z)
{
    return SimulationResults.at(GetResultsElementIndex(x, y, z));
}

const QVector3D MFSimulator::GetPosition(int32_t x, int32_t y, int32_t z)
{
    return SimulationDatapointsSpacing * QVector3D(x - SimulationNumDatapointsX / 2, y - SimulationNumDatapointsY / 2, z - SimulationNumDatapointsZ / 2);
}

void MFSimulator::CalculateContributionsFromCable(const TransmissionCable& cable)
{
    for (int x = 0; x < SimulationNumDatapointsX; x++)
        for (int y = 0; y < SimulationNumDatapointsY; y++)
            for (int z = 0; z < SimulationNumDatapointsZ; z++)
            {
                QVector3D position = GetPosition(x, y, z);
                QVector3D cableDirection = (cable.Rotation * QVector3D{0.0f, 0.0f, 1.0f}).normalized();

                float magnitude = 1.0f / position.distanceToLine(cable.Position, cableDirection);
                QVector3D direction =  QVector3D::crossProduct(cableDirection, position).normalized();

                SimulationResults[GetResultsElementIndex(x, y, z)] += magnitude * direction;
            }
}
