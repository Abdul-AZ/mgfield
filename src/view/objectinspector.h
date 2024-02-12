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

private:
    void DisconnectSignals();
    void ConnectSignals();

    void AddUniqueComponentWidgets(std::shared_ptr<Object> obj);

signals:
    void ObjectEdited(std::shared_ptr<Object> obj);

public slots:
    void ObjectSelected(std::shared_ptr<Object> obj);

private:
    Ui::ObjectInspector *ui;
    std::shared_ptr<Object> m_CurrentlySelectedObject;
};

#endif // OBJECTINSPECTOR_H
