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
        switch (object->Type)
        {
        case ObjectType::StraightWire:
            CalculateContributionsFromCable(*(StraightWireObject*)object.get());
            continue;

        case ObjectType::CurrentCarryingSheet:
            CalculateContributionsFromSheet(*(CurrentCarryingSheet*)object.get());
            continue;

        default:
            qWarning("Tried to simulate unknown object");
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

static QVector3D CalculateContributionFromInfiniteCable(const QVector3D& point, const StraightWireObject& cable)
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

static QVector3D CalculateContributionFromFiniteCable(const QVector3D& point, const StraightWireObject& cable)
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

void MFSimulator::CalculateContributionsFromCable(const StraightWireObject& cable)
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
                    result = CalculateContributionFromFiniteCable(position, cable);

                SimulationResults[GetResultsElementIndex(x, y, z)] += result;
            }
}

float I1(float s, float p, float a, float b, float c)
{
    float R = a + b * s + c * s * s;
    float rootR = std::sqrt(R);

    if(p > 0.0f)
    {
        float rootP = std::sqrt(p);
        return (1.0f / rootP) * std::atan(rootR / rootP);
    }
    else if (p < 0.0f)
    {
        float rootP = std::sqrt(-p);
        if(std::abs(rootP - rootR) < std::numeric_limits<float>::epsilon())
            return 0.0f;

        return (1.0f / (2.0f * rootP)) * std::log(abs(rootP - rootR) / (rootP + rootR));
    }

    return 0.0f;
}

float I2(float s, float p, float a, float b, float c, bool& out_i_on_denom)
{
    float R = a + b * s + c * s * s;
    float rootR = std::sqrt(R);

    float d = b * b - 4 * (a + p) * c;

    if(p * d > 0.0f)
    {
        out_i_on_denom = false;
        return -std::atan(std::sqrt(p/d)) * (b + 2 * c * s) / rootR;
    }
    else if ((p * d < 0.0f) && p > 0.0f)
    {
        out_i_on_denom = true;
        float rootD = std::sqrt(-d);
        float rootP = std::sqrt(p);
        float num = rootD * rootR + rootP * (b + 2 * c * s);
        float denom = rootD * rootR - rootP * (b + 2 * c * s);

        return std::log(num / denom) / 2.0f;
    }
    else if ((p * d < 0.0f) && p < 0.0f)
    {
        out_i_on_denom = true;

        float rootD = std::sqrt(d);
        float rootP = std::sqrt(-p);
        float num = rootD * rootR + rootP * (b + 2 * c * s);
        float denom = rootD * rootR - rootP * (b + 2 * c * s);

        return std::log(num / denom) / 2.0f;
    }

    return 0.0f;
}

float CalculateJOdd(float p1, float a1, float b1, float c1)
{
    float d = b1 * b1 - 4 * (a1 + p1) * c1;
    float constant = -2.0f * c1 / std::sqrt(std::abs(c1 * c1 * p1 * d));

    bool invertSign = false;
    float posEval = I2(0.5f, p1, a1, b1, c1, invertSign);
    if(invertSign)
        posEval *= -1.0f;

    invertSign = false;
    float negEval = I2(-0.5f, p1, a1, b1, c1, invertSign);
    if(invertSign)
        negEval *= -1.0f;


    return constant * (posEval - negEval);
}


float CalculateJEven(float p1, float a1, float b1, float c1)
{
    float i1constant = 1.0f / c1;
    float i1posEval = I1(0.5f, p1, a1, b1, c1);
    float i1negEval = I1(-0.5f, p1, a1, b1, c1);

    float i1contribution = i1constant * (i1posEval - i1negEval);

    float d = b1 * b1 - 4 * (a1 + p1) * c1;
    float i2constant = b1 / std::sqrt(std::abs(c1 * c1 * p1 * d));

    bool invertSign = false;
    float i2posEval = I2(0.5f, p1, a1, b1, c1, invertSign);
    if(invertSign)
        i2posEval *= -1.0f;

    invertSign = false;
    float i2negEval = I2(-0.5f, p1, a1, b1, c1, invertSign);
    if(invertSign)
        i2negEval *= -1.0f;


    float i2contribution = i2constant * (i2posEval - i2negEval);


    return i1contribution + i2contribution;
}

void MFSimulator::CalculateContributionsFromSheet(const CurrentCarryingSheet& sheet)
{
    QVector3D r_a = sheet.Position + sheet.Rotation * QVector3D(-sheet.GetLength(), 0.0f, 0.0f); // Start of sheet
    QVector3D r_b = sheet.Position + sheet.Rotation * QVector3D(sheet.GetLength(), 0.0f, 0.0f);  // end of sheet
    float alpha = sheet.GetLength() / sheet.GetWidth();
    QVector3D n = (sheet.Rotation * QVector3D(0.0f, 1.0f, 0.0f)).normalized();
    QVector3D p = QVector3D::crossProduct(alpha * (r_b- r_a), n);

    float Br = sheet.GetBr();

    for (int x = 0; x < SimulationNumDatapointsX; x++)
        for (int y = 0; y < SimulationNumDatapointsY; y++)
            for (int z = 0; z < SimulationNumDatapointsZ; z++)
            {
                QVector3D position = GetPosition(x, y, z);

                float A = (position - r_a).lengthSquared();
                float B = QVector3D::dotProduct(position - r_a, r_b - r_a);
                float C = (r_b - r_a).lengthSquared();
                float D = QVector3D::dotProduct(position - r_a, p);

                float p1 = -(B-C) * (B-C);
                float p2 = -B * B;

                float a1 = A * C - 2 * B * C + C * C;
                float b1 = -2.0f * C * D;
                float c1 = alpha * alpha * C * C;

                float j1 = 0.0f;
                float j2 = 0.0f;
                if(std::abs(p1) > std::numeric_limits<float>::epsilon())
                {
                    j1 = CalculateJOdd(p1, a1, b1, c1);
                    j2 = CalculateJEven(p1, a1, b1, c1);
                }

                float a2 = A * C;
                float b2 = b1;
                float c2 = c1;

                float j3 = 0.0f;
                float j4 = 0.0f;
                if(std::abs(p2) > std::numeric_limits<float>::epsilon())
                {
                    j3 = CalculateJOdd(p2, a2, b2, c2);
                    j4 = CalculateJEven(p2, a2, b2, c2);
                }

                QVector3D result(0.0f, 0.0f, 0.0f);
                float leftCrossFactor = (Br * alpha * C / (4 * SIM_CONSTANT_PI)) * ((C-B) * j1 + B * j3);
                result += QVector3D::crossProduct(leftCrossFactor * (r_b - r_a), position - r_a);

                float secondTermFactor = (Br * alpha * C * C / (4 * SIM_CONSTANT_PI)) * ((C-B)*j2 + B * j4);
                result += secondTermFactor * n;

                SimulationResults[GetResultsElementIndex(x, y, z)] += result;
            }
}
