#ifndef VIEWPORTSETTINGS_H
#define VIEWPORTSETTINGS_H

#include <QGroupBox>

namespace Ui {
class ViewportSettings;
}

class ViewportSettings : public QGroupBox
{
    Q_OBJECT

public:
    explicit ViewportSettings(QWidget *parent = nullptr);
    ~ViewportSettings();

    bool getAutoSimulateEnabled() const { return m_AutoSimulateEnabled; }
    bool getGridEnabled() const { return m_GridEnabled; }
    bool getGradientEnabled() const { return m_GradientEnabled; }

signals:
    void SettingsChanged();

private:
    Ui::ViewportSettings *ui;
    bool m_AutoSimulateEnabled = true;
    bool m_GridEnabled = true;
    bool m_GradientEnabled = true;
};

#endif // VIEWPORTSETTINGS_H
