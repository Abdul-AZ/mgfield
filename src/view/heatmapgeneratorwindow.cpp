#include "heatmapgeneratorwindow.h"
#include "ui_heatmapgeneratorwindow.h"

#include "src/sim/mfsimulator.h"

#include "thirdparty/libheatmap/heatmap.hpp"

#include <QLabel>
#include <QImage>
#include <QPainter>

HeatmapGeneratorWindow::HeatmapGeneratorWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HeatmapGeneratorWindow)
{
    ui->setupUi(this);


    connect(ui->generateButton, SIGNAL(clicked()), this, SLOT(GenerateHeatmap()));
}

HeatmapGeneratorWindow::~HeatmapGeneratorWindow()
{
    delete ui;
}

void HeatmapGeneratorWindow::GenerateHeatmap()
{
    qDebug("Test!");

    // Get simulator
    auto simulator = MFSimulator::GetInstance();

    size_t npoints = simulator->SimulationNumDatapointsY * simulator->SimulationNumDatapointsZ;

    const float factor = ui->pointStrengthSlider->value();
    static const size_t w =  simulator->SimulationNumDatapointsY * factor;
    static const size_t h = simulator->SimulationNumDatapointsZ * factor;

    // Create the heatmap object with the given dimensions (in pixel).
    heatmap_t* hm = heatmap_new(w, h);


    // Add a bunch of random points to the heatmap now.
    for(uint32_t y = 0 ; y < simulator->SimulationNumDatapointsY ; y++)
        for(uint32_t z = 0 ; z < simulator->SimulationNumDatapointsZ ; z++)
        {
            auto position = simulator->GetPosition(0,y,z);
            float xloc = (float)w * (position.y() - simulator->GetBoundryMinY()) / simulator->GetBoundrySizeY();
            float yloc = (float)h * (position.z() - simulator->GetBoundryMinZ()) / simulator->GetBoundrySizeZ();

            float val = simulator->GetResult(0, y, z).length();
            if(val < 0)
                int y =3;

            heatmap_add_weighted_point(hm, xloc, yloc, val);
        }

    // This creates an image out of the heatmap.
    // `image` now contains the image data in 32-bit RGBA.
    std::vector<unsigned char> image(w*h*4);
    heatmap_render_default_to(hm, image.data());

    // Now that we've got a finished heatmap picture, we don't need the map anymore.
    heatmap_free(hm);

    QImage img(image.data(), w, h, QImage::Format_RGBA8888);
    QImage whiteBackground(img.size(), QImage::Format_ARGB32);
    whiteBackground.fill(Qt::white); // Fill with white

    QPainter painter(&whiteBackground);
    painter.drawImage(0, 0, img); // Draw original image onto white background
    painter.end();

    whiteBackground.save("/home/abdul/Desktop/test.png");

    QPixmap pixmap    =  QPixmap::fromImage(whiteBackground);

    ui->label-> setPixmap(pixmap.scaled(300,300));
    ui->label->adjustSize();
}
