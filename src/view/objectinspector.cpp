#include "objectinspector.h"
#include "ui_objectinspector.h"

#include "QLabel"

#include "src/transmissioncable.h"

ObjectInspector::ObjectInspector(QWidget *parent)
    : QGroupBox(parent)
    , ui(new Ui::ObjectInspector)
{
    ui->setupUi(this);
    setEnabled(false);
}

ObjectInspector::~ObjectInspector()
{
    delete ui;
}

void ObjectInspector::DisconnectSignals()
{
    ui->NameLineEdit->disconnect();
    ui->PositionEditX->disconnect();
    ui->PositionEditY->disconnect();
    ui->PositionEditZ->disconnect();
    ui->RotationEditX->disconnect();
    ui->RotationEditY->disconnect();
    ui->RotationEditZ->disconnect();
}

void ObjectInspector::ConnectSignals()
{
    // Connect name edit
    ui->NameLineEdit->setText(m_CurrentlySelectedObject->Name);
    connect(ui->NameLineEdit, &QLineEdit::editingFinished, this, [this]() {
        m_CurrentlySelectedObject->Name = ui->NameLineEdit->text();

        emit ObjectEdited(m_CurrentlySelectedObject);
    });

    // Connect position edit
    ui->PositionEditX->setValue(m_CurrentlySelectedObject->Position.x());
    ui->PositionEditZ->setValue(m_CurrentlySelectedObject->Position.z());
    ui->PositionEditY->setValue(m_CurrentlySelectedObject->Position.y());
    connect(ui->PositionEditX, &QDoubleSpinBox::valueChanged, this, [this](double val) {
        m_CurrentlySelectedObject->Position.setX(val);

        emit ObjectEdited(m_CurrentlySelectedObject);
    });

    connect(ui->PositionEditY, &QDoubleSpinBox::valueChanged, this, [this](double val) {
        m_CurrentlySelectedObject->Position.setZ(val);

        emit ObjectEdited(m_CurrentlySelectedObject);
    });

    connect(ui->PositionEditZ, &QDoubleSpinBox::valueChanged, this, [this](double val) {
        m_CurrentlySelectedObject->Position.setY(val);

        emit ObjectEdited(m_CurrentlySelectedObject);
    });

    QVector3D euler = m_CurrentlySelectedObject->Rotation.toEulerAngles();
    // Connect rotation edit
    ui->RotationEditX->setValue(euler.x());
    ui->RotationEditY->setValue(euler.y());
    ui->RotationEditZ->setValue(euler.z());
    connect(ui->RotationEditX, &QDoubleSpinBox::valueChanged, this, [this](double val) {
        QVector3D euler = m_CurrentlySelectedObject->Rotation.toEulerAngles();
        euler.setX(val);

        m_CurrentlySelectedObject->Rotation = QQuaternion::fromEulerAngles(euler);

        emit ObjectEdited(m_CurrentlySelectedObject);
    });

    connect(ui->RotationEditY, &QDoubleSpinBox::valueChanged, this, [this](double val) {
        QVector3D euler = m_CurrentlySelectedObject->Rotation.toEulerAngles();
        euler.setY(val);

        m_CurrentlySelectedObject->Rotation = QQuaternion::fromEulerAngles(euler);

        emit ObjectEdited(m_CurrentlySelectedObject);
    });

    connect(ui->RotationEditZ, &QDoubleSpinBox::valueChanged, this, [this](double val) {
        QVector3D euler = m_CurrentlySelectedObject->Rotation.toEulerAngles();
        euler.setZ(val);

        m_CurrentlySelectedObject->Rotation = QQuaternion::fromEulerAngles(euler);

        emit ObjectEdited(m_CurrentlySelectedObject);
    });
}

//TODO organize this better
void ObjectInspector::AddUniqueComponentWidgets(std::shared_ptr<Object> obj)
{
    if(obj->Type == CurrentCarryingCable)
    {
        TransmissionCable* cable = (TransmissionCable*)obj.get();

        ModifiedDoubleSpinBox* spinbox = new ModifiedDoubleSpinBox(nullptr);
        spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinbox->setValue(cable->GetDCCurrent());
        connect(spinbox, &QDoubleSpinBox::valueChanged, this, [this](double val)
        {
            ((TransmissionCable*)m_CurrentlySelectedObject.get())->SetDCCurrent(val);

            emit ObjectEdited(m_CurrentlySelectedObject);
        });
        ui->UniqueComponents->addRow("Current", spinbox);

    }
}

void ObjectInspector::ObjectSelected(std::shared_ptr<Object> obj)
{
    DisconnectSignals();
    while (ui->UniqueComponents->rowCount() > 0)
        ui->UniqueComponents->removeRow(0);
    m_CurrentlySelectedObject = obj;

    if(obj == nullptr)
    {
        setEnabled(false);
        return;
    }

    AddUniqueComponentWidgets(obj);

    ConnectSignals();
    setEnabled(true);
}
