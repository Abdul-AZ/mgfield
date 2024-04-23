#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include "scene/object.h"
#include <reactphysics3d/reactphysics3d.h>

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
    void ObjectEdited(std::shared_ptr<Object> object);

public:
    QVector<std::shared_ptr<Object>> Objects;
    reactphysics3d::PhysicsCommon physicsCommon;
    reactphysics3d::PhysicsWorld* physicsWorld;
};

#endif // SCENE_H
