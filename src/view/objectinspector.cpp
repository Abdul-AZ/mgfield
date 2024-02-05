#include "objectinspector.h"
#include "ui_objectinspector.h"

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
    ui->PositionEditY->setValue(m_CurrentlySelectedObject->Position.y());
    ui->PositionEditZ->setValue(m_CurrentlySelectedObject->Position.z());
    connect(ui->PositionEditX, &QDoubleSpinBox::valueChanged, this, [this](double val) {
        m_CurrentlySelectedObject->Position.setX(val);

        emit ObjectEdited(m_CurrentlySelectedObject);
    });

    connect(ui->PositionEditY, &QDoubleSpinBox::valueChanged, this, [this](double val) {
        m_CurrentlySelectedObject->Position.setY(val);

        emit ObjectEdited(m_CurrentlySelectedObject);
    });

    connect(ui->PositionEditZ, &QDoubleSpinBox::valueChanged, this, [this](double val) {
        m_CurrentlySelectedObject->Position.setZ(val);

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

void ObjectInspector::ObjectSelected(std::shared_ptr<Object> obj)
{
    DisconnectSignals();
    m_CurrentlySelectedObject = obj;

    if(obj == nullptr)
    {
        setEnabled(false);
        return;
    }

    ConnectSignals();
    setEnabled(true);
}
