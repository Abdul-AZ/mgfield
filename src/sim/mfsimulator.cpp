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
        qCritical() << "Invalid simulation results access coords, value in (" << x << ", " << y << ", " << z << ") does not exist";
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

static QVector3D CalculateContributionFromInfiniteCable(const QVector3D& point, const TransmissionCable& cable)
{
    // Assuming cable is infinite in length
    // Using Biot-Savart Law to calculate magnitude of magnetic field
    // |B| = (u0 . I) / (2PI . h)
    // Where h is the shortest distance of point from cable
    const double pi = SIM_CONSTANT_PI;
    QVector3D cableDirection = (cable.Rotation * QVector3D{1.0f, 0.0f, 0.0f}).normalized();

    float h = point.distanceToLine(cable.Position, cableDirection);
    float magnitude = cable.GetDCCurrent() / (2 * pi * h);
    QVector3D direction =  QVector3D::crossProduct(cableDirection, cable.Position - point).normalized();

    return magnitude * direction;
}

static QVector3D CalculateContributionFromfiniteCable(const QVector3D& point, const TransmissionCable& cable)
{
    const double pi = SIM_CONSTANT_PI;
    QVector3D result;

    QVector3D start = cable.Rotation.rotatedVector(QVector3D(1.0f, 0.0f, 0.0f));
    QVector3D end = cable.Rotation.rotatedVector(QVector3D(-1.0f, 0.0f, 0.0f));

    start += cable.Position;
    end += cable.Position;

    const int32_t n = 100;

    const QVector3D deltaL = (end - start) / (float)n;

    // Calculate Biot-Savart Law integral
    for (int32_t i = 0; i < n; i++)
    {
        float t = (float)i / n;
        QVector3D P = start + (end - start) * t;
        QVector3D r = P - point;

        if(r.lengthSquared() == 0)
            continue;

        QVector3D funcEval = (QVector3D::crossProduct(deltaL, r.normalized()))  / r.lengthSquared();

        result += funcEval;
    }

    return result * cable.GetDCCurrent() / (4 * pi);
}

void MFSimulator::CalculateContributionsFromCable(const TransmissionCable& cable)
{
    for (int x = 0; x < SimulationNumDatapointsX; x++)
        for (int y = 0; y < SimulationNumDatapointsY; y++)
            for (int z = 0; z < SimulationNumDatapointsZ; z++)
            {
                QVector3D position = GetPosition(x, y, z);
                QVector3D result;
                if(cable.GetIsInfiniteLength())
                    result = CalculateContributionFromInfiniteCable(position, cable);
                else
                    result = CalculateContributionFromfiniteCable(position, cable);

                SimulationResults[GetResultsElementIndex(x, y, z)] = result;
            }
}
