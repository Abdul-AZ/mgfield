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

signals:
    void SettingsChanged();

private:
    Ui::ViewportSettings *ui;
};

#endif // VIEWPORTSETTINGS_H
