#include "objectlistview.h"

ObjectListView::ObjectListView(QWidget* parent) :
    QListView(parent)
{
    m_Model = new QStringListModel(this);

    setModel(m_Model);
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
        m_Model->setData(m_Model->index(m_Model->rowCount() - 1), object->Name);
    }
}
void ObjectListView::ObjectSelectedSlot(std::shared_ptr<Object> object)
{
    int row = m_CurrentScene->Objects.indexOf(object);
    setCurrentIndex(model()->index(row, 0));
}

/*! \brief Called when there is an edit in an object but object focus not remoed
 *
 */
void ObjectListView::ObjectEdited()
{
    for (auto& object : m_CurrentScene->Objects)
    {
        m_Model->setData(m_Model->index(m_Model->rowCount() - 1), object->Name);
    }
}

void ObjectListView::SceneLoaded(Scene* scene)
{
    m_CurrentScene = scene;

    ObjectsChanged();
}
