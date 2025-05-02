#ifndef HEATMAPGENERATORWINDOW_H
#define HEATMAPGENERATORWINDOW_H

#include <QDialog>

namespace Ui {
class HeatmapGeneratorWindow;
}

class HeatmapGeneratorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit HeatmapGeneratorWindow(QWidget *parent = nullptr);
    ~HeatmapGeneratorWindow();

private slots:
    void GenerateHeatmap();

private:
    Ui::HeatmapGeneratorWindow *ui;
};

#endif // HEATMAPGENERATORWINDOW_H
