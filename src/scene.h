#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include "scene/object.h"

class Scene : public QObject
{
    Q_OBJECT

public:
    Scene();

signals:
    void ObjectAdded();
    void ObjectRemoved();

public slots:
    void AddObjectUsingDialog();
    void RemoveObject(int32_t index);

public:
    QVector<std::shared_ptr<Object>> Objects;
};

#endif // SCENE_H
