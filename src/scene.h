#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include "transmissioncable.h"

class Scene : public QObject
{
    Q_OBJECT

public:
    Scene();

signals:
    void ObjectAdded();

public:
    QVector<std::shared_ptr<TransmissionCable>> Cables;
};

#endif // SCENE_H
