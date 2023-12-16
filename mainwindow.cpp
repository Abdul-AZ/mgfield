#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QPushButton>
#include "src/view/viewport3d.h"
#include "src/sim/mfsimulator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , simulator(MFSimulator::GetInstance())
{
    ui->setupUi(this);
    scene = new Scene;

    ui->CameraTypeComboBox->addItem("Orbit", (int32_t)CameraControlMode::Orbit);
    ui->CameraTypeComboBox->addItem("Free Roam", (int32_t)CameraControlMode::FreeRoam);
    connect(ui->CameraTypeComboBox, &QComboBox::currentIndexChanged, this, [this]()
    {
        ui->viewport3D->cameraModeChanged((CameraControlMode)ui->CameraTypeComboBox->currentData().toInt());
    });

    connect(ui->viewport3D, &Viewport3D::cameraMoved, this, &MainWindow::updateCameraLocationStatus);
    connect(ui->viewport3D, &Viewport3D::exportedImage, this, &MainWindow::showExportedImageStatus);

    connect(ui->simulatePushButton, &QPushButton::clicked, this,[this]()
    {
        MFSimulator::GetInstance()->RequestNewSimulationRun(scene);
    });

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutWindow()));

    connect(ui->AddObjectButton, SIGNAL(clicked()), ui->viewport3D, SLOT(RequestAddObject()));
    ui->viewport3D->SceneLoaded(scene);
    ui->ObjectList->SceneLoaded(scene);

    connect(scene, SIGNAL(ObjectAdded()), ui->ObjectList, SLOT(ObjectsChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCameraLocationStatus(QVector3D newPos)
{
    const int fieldWidth = 7;
    const char format = 'f';
    const int precision = 2;

    QString message = QString("X:%1 Y:%2 Z:%3").arg(newPos.x(), fieldWidth, format, precision).arg(newPos.y(), fieldWidth, format, precision).arg(newPos.z(), fieldWidth, format, precision);

    ui->statusbar->showMessage(message, 100000000);
}

void MainWindow::showExportedImageStatus(QString path)
{
    ui->statusbar->showMessage("Saved Image: " + path, 5000);
}

void MainWindow::showAboutWindow()
{
    QMessageBox dialog(this);

    dialog.setWindowTitle("About");
    dialog.setText("Author: Abdulrahman Al Zeidi");

    dialog.exec();
}
