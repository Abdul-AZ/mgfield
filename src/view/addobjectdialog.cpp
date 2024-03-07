#include "addobjectdialog.h"
#include "ui_addobjectdialog.h"


AddObjectDialog::AddObjectDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddObjectDialog)
{
    ui->setupUi(this);

    ui->ObjectTypeComboBox->addItem("Current Carrying Conductor", (int32_t)ObjectType::StraightWire);
    ui->ObjectTypeComboBox->addItem("Current Carrying Sheet", (int32_t)ObjectType::CurrentCarryingSheet);
}

ObjectType AddObjectDialog::GetSelectedObjectType() const
{
    return (ObjectType)ui->ObjectTypeComboBox->currentData().toInt();
}

AddObjectDialog::~AddObjectDialog()
{
    delete ui;
}
