#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->viewport3D, &Viewport3D::cameraMoved, this, &MainWindow::updateCameraLocationStatus);
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

