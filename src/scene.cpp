#include "scene.h"

#include <QDebug>

#include "src/scene/straightwireobject.h"
#include "src/scene/currentcarryingsheet.h"
#include "src/scene/permanentmagnet.h"

#include "view/addobjectdialog.h"

Scene::Scene() {}

void Scene::AddObjectUsingDialog()
{
    AddObjectDialog dialog(nullptr);

    if(dialog.exec() != QDialog::Accepted)
        return;

    ObjectType type = dialog.GetSelectedObjectType();

    switch (type)
    {
    case ObjectType::StraightWire:
        Objects.append(std::make_shared<StraightWireObject>());
        break;

    case ObjectType::CurrentCarryingSheet:
        Objects.append(std::make_shared<CurrentCarryingSheet>());
        break;

    case ObjectType::PermanentMagnet:
        Objects.append(std::make_shared<PermanentMagnet>());
        ((PermanentMagnet*)Objects.last().get())->SetShape(dialog.GetMagnetShape());
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
