#include "objectinspector.h"
#include "ui_objectinspector.h"

#include "QLabel"
#include <QCheckBox>

#include "src/scene/straightwireobject.h"
#include "src/scene/currentcarryingsheet.h"

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
    if(obj->Type == ObjectType::StraightWire)
    {
        StraightWireObject* cable = (StraightWireObject*)obj.get();

        QCheckBox* checkbox = new QCheckBox(this);
        checkbox->setText("");
        checkbox->setChecked(cable->GetIsInfiniteLength());
        connect(checkbox, &QCheckBox::stateChanged, this, [this](int val)
        {
            ((StraightWireObject*)m_CurrentlySelectedObject.get())->SetIsInfiniteLength((bool)val);

            emit ObjectEdited(m_CurrentlySelectedObject);
        });
        ui->UniqueComponents->addRow("Infinite Length", checkbox);

        ModifiedDoubleSpinBox* spinbox = new ModifiedDoubleSpinBox(this);
        spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinbox->setValue(cable->GetDCCurrent());
        spinbox->setMaximum(1e9);
        connect(spinbox, &QDoubleSpinBox::valueChanged, this, [this](double val)
        {
            ((StraightWireObject*)m_CurrentlySelectedObject.get())->SetDCCurrent(val);

            emit ObjectEdited(m_CurrentlySelectedObject);
        });
        ui->UniqueComponents->addRow("Current", spinbox);
    }
    else if (obj->Type == ObjectType::CurrentCarryingSheet)
    {
        CurrentCarryingSheet* sheet = (CurrentCarryingSheet*)obj.get();

        ModifiedDoubleSpinBox* widthSpinbox = new ModifiedDoubleSpinBox(this);
        widthSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        widthSpinbox->setValue(sheet->GetWidth());
        widthSpinbox->setMaximum(100.0f);
        widthSpinbox->setMinimum(std::numeric_limits<float>::epsilon());
        connect(widthSpinbox, &QDoubleSpinBox::valueChanged, this, [this](double val)
        {
            ((CurrentCarryingSheet*)m_CurrentlySelectedObject.get())->SetWidth(val);

            emit ObjectEdited(m_CurrentlySelectedObject);
        });
        ui->UniqueComponents->addRow("Width", widthSpinbox);


        ModifiedDoubleSpinBox* lengthSpinbox = new ModifiedDoubleSpinBox(this);
        lengthSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        lengthSpinbox->setValue(sheet->GetWidth());
        lengthSpinbox->setMaximum(100.0f);
        lengthSpinbox->setMinimum(std::numeric_limits<float>::epsilon());
        connect(lengthSpinbox, &QDoubleSpinBox::valueChanged, this, [this](double val)
        {
            ((CurrentCarryingSheet*)m_CurrentlySelectedObject.get())->SetLength(val);

            emit ObjectEdited(m_CurrentlySelectedObject);
        });
        ui->UniqueComponents->addRow("Length", lengthSpinbox);


        ModifiedDoubleSpinBox* brSpinbox = new ModifiedDoubleSpinBox(this);
        brSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        brSpinbox->setValue(sheet->GetBr());
        brSpinbox->setMaximum(100.0f);
        brSpinbox->setMinimum(std::numeric_limits<float>::epsilon());
        connect(brSpinbox, &QDoubleSpinBox::valueChanged, this, [this](double val)
        {
            ((CurrentCarryingSheet*)m_CurrentlySelectedObject.get())->SetBr(val);

            emit ObjectEdited(m_CurrentlySelectedObject);
        });
        ui->UniqueComponents->addRow("Br", brSpinbox);
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
