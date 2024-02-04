#ifndef OBJECTINSPECTOR_H
#define OBJECTINSPECTOR_H

#include <QGroupBox>
#include "src/scene.h"

namespace Ui {
class ObjectInspector;
}

class ObjectInspector : public QGroupBox
{
    Q_OBJECT

public:
    explicit ObjectInspector(QWidget *parent = nullptr);
    ~ObjectInspector();

public slots:
    void ObjectSelected(std::shared_ptr<Object> obj);

private:
    Ui::ObjectInspector *ui;
};

#endif // OBJECTINSPECTOR_H
