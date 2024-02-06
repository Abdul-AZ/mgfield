#include "viewportsettings.h"
#include "ui_viewportsettings.h"

ViewportSettings::ViewportSettings(QWidget *parent)
    : QGroupBox(parent)
    , ui(new Ui::ViewportSettings)
{
    ui->setupUi(this);

    connect(ui->ShowGridCheckbox, &QCheckBox::stateChanged, this, [this](int state)
    {
        this->m_GridEnabled = state;
        emit SettingsChanged();
    }
    );

    connect(ui->ShowGradientCheckbox, &QCheckBox::stateChanged, this, [this](int state)
    {
        this->m_GradientEnabled = state;
        emit SettingsChanged();
    }
    );
}

ViewportSettings::~ViewportSettings()
{
    delete ui;
}
