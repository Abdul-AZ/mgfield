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

    bool getGridEnabled() const { return m_GridEnabled; }

signals:
    void SettingsChanged();

private:
    Ui::ViewportSettings *ui;
    bool m_GridEnabled = true;
};

#endif // VIEWPORTSETTINGS_H
