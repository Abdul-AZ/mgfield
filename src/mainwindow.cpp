#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QPushButton>
#include "src/view/viewport3d.h"
#include "src/view/simulationsettingsdialog.h"
#include "src/view/addobjectdialog.h"
#include "src/sim/mfsimulator.h"

#include <qfonticon.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , simulator(MFSimulator::GetInstance())
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/res/icon_256px.png"));
    UseIconFont();

    scene = new Scene;

    /*
    ui->CameraTypeComboBox->addItem("Orbit", (int32_t)CameraControlMode::Orbit);
    ui->CameraTypeComboBox->addItem("Free Roam", (int32_t)CameraControlMode::FreeRoam);
    connect(ui->CameraTypeComboBox, &QComboBox::currentIndexChanged, this, [this]()
    {
        ui->viewport3D->cameraModeChanged((CameraControlMode)ui->CameraTypeComboBox->currentData().toInt());
    });
*/

    connect(ui->viewport3D, &Viewport3D::cameraMoved, this, &MainWindow::updateCameraLocationStatus);
    connect(ui->viewport3D, &Viewport3D::exportedImage, this, &MainWindow::showExportedImageStatus);
    connect(ui->viewport3D, &Viewport3D::ObjectSelected, ui->ObjectInspectorInstance, &ObjectInspector::ObjectSelected);
    connect(ui->viewport3D, &Viewport3D::ObjectSelected, ui->ObjectList, &ObjectListView::ObjectSelectedSlot);

    connect(ui->SimulateButton, &QPushButton::clicked, this,[this]()
    {
        MFSimulator::GetInstance()->RequestNewSimulationRun(scene);
    });

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutWindow()));

    // Connect add and remove buttons
    connect(ui->AddObjectButton, &QPushButton::clicked, scene, &Scene::AddObjectUsingDialog);

    connect(ui->RemoveObjectButton, &QPushButton::clicked, this, [this]()
    {
        scene->RemoveObject(ui->ObjectList->currentIndex().row());
        ui->RemoveObjectButton->setEnabled(false);
    });

    // Disable remove button when no object is selected
    connect(ui->ObjectList, &ObjectListView::currentChanged, this, [this](const QModelIndex &current, const QModelIndex &previou)
    {
        ui->RemoveObjectButton->setEnabled(current.row() != -1);
    });

    connect(ui->viewport3D, &Viewport3D::ObjectSelected, this, [this](std::shared_ptr<Object> obj)
    {
        ui->RemoveObjectButton->setEnabled(obj != nullptr);
    });

    connect(ui->ObjectList, &ObjectListView::ObjectSelected, this, [this](std::shared_ptr<Object> obj)
    {
        ui->RemoveObjectButton->setEnabled(obj != nullptr);
    });

    ui->viewport3D->SceneLoaded(scene);
    ui->ObjectList->SceneLoaded(scene);

    connect(scene, SIGNAL(ObjectAdded()), ui->ObjectList, SLOT(ObjectsChanged()));
    connect(scene, SIGNAL(ObjectRemoved()), ui->ObjectList, SLOT(ObjectsChanged()));

    ui->viewport3D->HookViewportSettings(ui->ViewportSettingsWidget);

    connect(ui->actionSimulationSettings, &QAction::triggered, this, [this]()
    {
        SimulationSettingsDialog dialog(this);

        dialog.exec();
    });

    connect(ui->actionSimulationClearResults, &QAction::triggered, this, [this]()
    {
        MFSimulator::GetInstance()->ClearResults();
    });

    connect(ui->ObjectList, &ObjectListView::ObjectSelected, ui->ObjectInspectorInstance, &ObjectInspector::ObjectSelected);
    connect(ui->ObjectInspectorInstance, &ObjectInspector::ObjectEdited, ui->ObjectList, &ObjectListView::ObjectEdited);
    connect(ui->ObjectInspectorInstance, &ObjectInspector::ObjectEdited, ui->viewport3D, &Viewport3D::Redraw);
    connect(ui->ObjectInspectorInstance, &ObjectInspector::ObjectEdited, this, [this] () {

        if(ui->ViewportSettingsWidget->getAutoSimulateEnabled())
            MFSimulator::GetInstance()->RequestNewSimulationRun(scene);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UseIconFont()
{
    ui->AddObjectButton->setText(nullptr);
    ui->AddObjectButton->setIcon(QFontIcon::icon(QChar(0xF199)));

    ui->RemoveObjectButton->setText(nullptr);
    ui->RemoveObjectButton->setIcon(QFontIcon::icon(QChar(0xF217)));

    ui->SimulateButton->setText(nullptr);
    ui->SimulateButton->setIcon(QFontIcon::icon(QChar(0xF198)));
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
