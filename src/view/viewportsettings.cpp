#include "viewportsettings.h"
#include "ui_viewportsettings.h"

#include <QSettings>

ViewportSettings::ViewportSettings(QWidget *parent)
    : QGroupBox(parent)
    , ui(new Ui::ViewportSettings)
{
    ui->setupUi(this);

    QSettings settings;
    ui->AutoSimulateCheckbox->setCheckState(settings.value("ViewportSettings/AutoSimulate", true).toBool() ? Qt::Checked : Qt::Unchecked);
    ui->ShowGridCheckbox->setCheckState(settings.value("ViewportSettings/ShowGrid", true).toBool() ? Qt::Checked : Qt::Unchecked);
    ui->ShowGradientCheckbox->setCheckState(settings.value("ViewportSettings/ShowGradient", true).toBool() ? Qt::Checked : Qt::Unchecked);
    ui->ShowNormalsCheckbox->setCheckState(settings.value("ViewportSettings/ShowNormals", true).toBool() ? Qt::Checked : Qt::Unchecked);

    connect(ui->AutoSimulateCheckbox, &QCheckBox::stateChanged, this, [this](int state)
    {
        QSettings settings;
        settings.setValue("ViewportSettings/AutoSimulate", (bool)state);
        emit SettingsChanged();
    }
    );

    connect(ui->ShowGridCheckbox, &QCheckBox::stateChanged, this, [this](int state)
    {
        QSettings settings;
        settings.setValue("ViewportSettings/ShowGrid", (bool)state);
        emit SettingsChanged();
    }
    );

    connect(ui->ShowGradientCheckbox, &QCheckBox::stateChanged, this, [this](int state)
    {
        QSettings settings;
        settings.setValue("ViewportSettings/ShowGradient", (bool)state);
        emit SettingsChanged();
    }
    );

    connect(ui->ShowNormalsCheckbox, &QCheckBox::stateChanged, this, [this](int state)
    {
        QSettings settings;
        settings.setValue("ViewportSettings/ShowNormals", (bool)state);
        emit SettingsChanged();
    }
    );
}

ViewportSettings::~ViewportSettings()
{
    delete ui;
}
