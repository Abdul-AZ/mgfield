#include "scene.h"

#include <QDebug>

#include "src/scene/straightwireobject.h"

Scene::Scene() {}

void Scene::AddObject(ObjectType type)
{
    switch (type)
    {
    case ObjectType::StraightWire:
        Objects.append(std::make_shared<StraightWireObject>());
        break;

    default:
        qWarning("Tried creating unknown object");
        return;
    }

    emit ObjectAdded();
}

void Scene::RemoveObject(int32_t index)
{
    if((index < 0) || (index >= Objects.size()))
    {
        qWarning("Tried deleting object that does not exist");
        return;
    }

    Objects.remove(index);

    emit ObjectRemoved();
}
