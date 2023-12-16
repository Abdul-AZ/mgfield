#ifndef OBJECTLISTVIEW_H
#define OBJECTLISTVIEW_H

#include <QListView>
#include <QObject>
#include <QWidget>
#include <QStringListModel>

#include "scene.h"

class ObjectListView : public QListView
{
    Q_OBJECT
public:
    ObjectListView(QWidget* parent);

public slots:
    void SceneLoaded(Scene* scene);
    void ObjectsChanged();

private:
    Scene* m_CurrentScene = nullptr;
    QStringListModel* m_Model = nullptr;
};

#endif // OBJECTLISTVIEW_H
