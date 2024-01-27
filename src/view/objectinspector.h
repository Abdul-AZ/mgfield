#ifndef OBJECTINSPECTOR_H
#define OBJECTINSPECTOR_H

#include <QGroupBox>

namespace Ui {
class ObjectInspector;
}

class ObjectInspector : public QGroupBox
{
    Q_OBJECT

public:
    explicit ObjectInspector(QWidget *parent = nullptr);
    ~ObjectInspector();

private:
    Ui::ObjectInspector *ui;
};

#endif // OBJECTINSPECTOR_H
