#ifndef SIMULATIONRESULTSEXPORTER_H
#define SIMULATIONRESULTSEXPORTER_H

#include <QAction>
#include <vector>

#define SIMULATION_RESULTS_EXPORTER_NUMBER_OF_OPTIONS (1)

class SimulationResultsExporter
{
public:
    struct ExportOption
    {
        QString Header;
        std::function<void()> Func;
    };

public:
    static std::array<ExportOption, SIMULATION_RESULTS_EXPORTER_NUMBER_OF_OPTIONS> GetExportOptions();

};

#endif // SIMULATIONRESULTSEXPORTER_H
