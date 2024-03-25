#include "addobjectdialog.h"
#include "ui_addobjectdialog.h"

#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>

AddObjectDialog::AddObjectDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddObjectDialog)
{
    ui->setupUi(this);

    ui->ObjectTypeComboBox->addItem("Current Carrying Conductor", (int32_t)ObjectType::StraightWire);
    ui->ObjectTypeComboBox->addItem("Current Carrying Sheet", (int32_t)ObjectType::CurrentCarryingSheet);
    ui->ObjectTypeComboBox->addItem("Permanent Magnet", (int32_t)ObjectType::PermanentMagnet);

    LoadMagnetModelNames();

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

void AddObjectDialog::LoadMagnetModelNames()
{
    QFile file(":/res/data/magnet_models.json");
    file.open(QIODevice::ReadOnly);

    QByteArray byteArray;
    byteArray = file.readAll();
    file.close();

    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(byteArray);

    QJsonArray array = jsonDoc.object().value("models").toArray();
    for(const QJsonValue& model : array)
    {
        QJsonObject obj1 = model.toObject();
        QJsonValue id = obj1.value("id");
        QJsonValue name = obj1.value("name");

        ui->ShapeComboBox->addItem(name.toString(), id.toInt());
    }
}

ObjectType AddObjectDialog::GetSelectedObjectType() const
{
    return (ObjectType)ui->ObjectTypeComboBox->currentData().toInt();
}

int32_t AddObjectDialog::GetMagnetModelID() const
{
    return ui->ShapeComboBox->currentData().toInt();
}

AddObjectDialog::~AddObjectDialog()
{
    delete ui;
}
