#include "objectlistview.h"

ObjectListView::ObjectListView(QWidget* parent) :
    QListView(parent)
{
    connect(this, &ObjectListView::ObjectSelected, this, &ObjectListView::ObjectSelectedSlot);
}

void ObjectListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if(current.row() >= 0)
        emit ObjectSelected(m_CurrentScene->Objects[current.row()]);
}

/*! \brief Called an object is added or remoed, removes focus on currently selected object
 *
 */
void ObjectListView::ObjectsChanged()
{
    m_Model->removeRows(0, m_Model->rowCount());
    emit ObjectSelected(nullptr);

    if(!m_CurrentScene)
        return;

    for (auto& object : m_CurrentScene->Objects)
    {
        m_Model->insertRow(m_Model->rowCount());
        m_Model->setData(m_Model->index(m_Model->rowCount() - 1, 0), QVariant::fromValue(object));
    }

    emit dataChanged(QModelIndex(), QModelIndex());

}
void ObjectListView::ObjectSelectedSlot(std::shared_ptr<Object> object)
{
    if(!object)
    {
        setCurrentIndex(QModelIndex());
        emit m_Delegate->ObjectSelected(object);
        return;
    }

    int row = m_CurrentScene->Objects.indexOf(object);
    setCurrentIndex(model()->index(row, 0));
    emit m_Delegate->ObjectSelected(object);
}

/*! \brief Called when there is an edit in an object but object focus not remoed
 *
 */
void ObjectListView::ObjectEdited()
{
    for (auto& object : m_CurrentScene->Objects)
    {
        m_Model->setData(m_Model->index(m_Model->rowCount() - 1, 0), object->Name);
    }

    emit dataChanged(QModelIndex(), QModelIndex());
}

void ObjectListView::SceneLoaded(Scene* scene)
{
    m_CurrentScene = scene;

    m_Model = new ObjectItemModel(scene, this);
    m_Delegate = new ObjectItemDelegate(this);
    setModel(m_Model);
    setItemDelegate(m_Delegate);

    ObjectsChanged();
}
