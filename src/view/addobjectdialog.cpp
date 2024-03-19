#include "addobjectdialog.h"
#include "ui_addobjectdialog.h"

AddObjectDialog::AddObjectDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddObjectDialog)
{
    ui->setupUi(this);

    ui->ObjectTypeComboBox->addItem("Current Carrying Conductor", (int32_t)ObjectType::StraightWire);
    ui->ObjectTypeComboBox->addItem("Current Carrying Sheet", (int32_t)ObjectType::CurrentCarryingSheet);
    ui->ObjectTypeComboBox->addItem("Permanent Magnet", (int32_t)ObjectType::PermanentMagnet);

    ui->ShapeComboBox->addItem("Bar Magnet", (int32_t)PermanentMagnetShape::BarMagnet);

    ui->ShapeComboBox->setVisible(false);
    ui->ShapeLabel->setVisible(false);

    connect(ui->ObjectTypeComboBox, &QComboBox::currentIndexChanged, this, [this]()
    {
        bool enableShapeSelection = false;
        if(ui->ObjectTypeComboBox->currentData().toInt() == (int32_t)ObjectType::PermanentMagnet)
        {
            enableShapeSelection = true;
        }

        ui->ShapeComboBox->setVisible(enableShapeSelection);
        ui->ShapeLabel->setVisible(enableShapeSelection);

        ui->ShapeComboBox->setEnabled(enableShapeSelection);
        ui->ShapeLabel->setEnabled(enableShapeSelection);
    });
}

ObjectType AddObjectDialog::GetSelectedObjectType() const
{
    return (ObjectType)ui->ObjectTypeComboBox->currentData().toInt();
}

PermanentMagnetShape AddObjectDialog::GetMagnetShape() const
{
    return (PermanentMagnetShape)ui->ShapeComboBox->currentData().toInt();
}

AddObjectDialog::~AddObjectDialog()
{
    delete ui;
}
