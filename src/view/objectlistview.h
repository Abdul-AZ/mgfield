#ifndef OBJECTLISTVIEW_H
#define OBJECTLISTVIEW_H

#include <QListView>
#include <QObject>
#include <QWidget>
#include "src/objectitemmodel.h"
#include "src/objectitemdelegate.h"

#include "../scene.h"

class ObjectListView : public QListView
{
    Q_OBJECT
public:
    ObjectListView(QWidget* parent);

    virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

public slots:
    void SceneLoaded(Scene* scene);
    void ObjectsChanged();
    void ObjectEdited();
    void ObjectSelectedSlot(std::shared_ptr<Object> object);

signals:
    void ObjectSelected(std::shared_ptr<Object> object);

private:
    Scene* m_CurrentScene = nullptr;
    ObjectItemModel* m_Model = nullptr;
    ObjectItemDelegate* m_Delegate = nullptr;
};

#endif // OBJECTLISTVIEW_H
