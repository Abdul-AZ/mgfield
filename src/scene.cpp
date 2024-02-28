#include "scene.h"

#include <QDebug>

#include "src/transmissioncable.h"

Scene::Scene() {}

void Scene::AddObject(ObjectType type)
{
    switch (type)
    {
    case CurrentCarryingCable:
        Objects.append(std::make_shared<TransmissionCable>());
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
