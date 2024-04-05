#include "simulationresultsexporter.h"

#include <QMessageBox>
#include <QFileDialog>

#include "mfsimulator.h"

#define CSV_SEPARATOR ','

static void WriteResultsToFileAsCSV(QFile& file)
{
    std::string str;
    auto simulator = MFSimulator::GetInstance();
    QTextStream stream(&file);
    const char sep = CSV_SEPARATOR;

    // Write header
    stream << "Magnetic field factor" << sep << SIM_CONSTANT_VACUUM_MAGNETIC_PERMEABILITY << sep << "Multiply by this factor to to get results in Tesla (T)" << "\n";
    stream << "Point (X)" << sep << "Point (Y)" << sep << "Point (Z)" << sep << "B (X-Component)" << sep << "B (Y-Component)" << sep << "B (Z-Component)" << "\n";

    for (int x = 0; x < simulator->SimulationNumDatapointsX; x++)
        for (int y = 0; y < simulator->SimulationNumDatapointsY; y++)
            for (int z = 0; z < simulator->SimulationNumDatapointsZ; z++)
            {
                QVector3D position = simulator->GetPosition(x, y, z);
                QVector3D result = simulator->SimulationResults[simulator->GetResultsElementIndex(x, y, z)];

                stream << position.x() << sep << position.y() << sep << position.z() << sep << result.x() << sep << result.y() << sep << result.z() << "\n";
            }
}

static void ExportResultsAsCSV()
{
    auto simulator = MFSimulator::GetInstance();

    if(simulator->SimulationResultsExist == false)
    {
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Could not export results", "There are no results to export.\nRun the simulation first before exporting.");
        return;
    }

    QString path = QFileDialog::getSaveFileName(nullptr, "Save CSV File", QString(), "CSV (*.csv)");
    if (!path.isEmpty())
    {
        if(!path.endsWith(".csv"))
            path.append(".csv");


        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            WriteResultsToFileAsCSV(file);
            file.close();
        }
        else
        {
            QMessageBox messageBox;
            messageBox.critical(nullptr, "Could not export results", "Could not open file.");
            return;
        }
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Could not export results", "Invalid path.");
        return;
    }

}

std::array<SimulationResultsExporter::ExportOption, SIMULATION_RESULTS_EXPORTER_NUMBER_OF_OPTIONS> SimulationResultsExporter::GetExportOptions()
{
    std::array<SimulationResultsExporter::ExportOption, SIMULATION_RESULTS_EXPORTER_NUMBER_OF_OPTIONS> options;

    options[0].Header = "Export As CSV";
    options[0].Func = &ExportResultsAsCSV;

    return options;
}
