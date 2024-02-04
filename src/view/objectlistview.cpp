#include "objectlistview.h"

ObjectListView::ObjectListView(QWidget* parent) :
    QListView(parent)
{
    m_Model = new QStringListModel();

    setModel(m_Model);
}

void ObjectListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if(current.row() >= 0)
        emit ObjectSelected(m_CurrentScene->Objects[current.row()]);
}

void ObjectListView::ObjectsChanged()
{
    m_Model->removeRows(0, m_Model->rowCount());
    emit ObjectSelected(nullptr);

    if(!m_CurrentScene)
        return;

    for (auto& object : m_CurrentScene->Objects)
    {
        m_Model->insertRow(0);
        m_Model->setData(m_Model->index(0), "Current Carrying Conductor");
    }
}

void ObjectListView::SceneLoaded(Scene* scene)
{
    m_CurrentScene = scene;

    ObjectsChanged();
}
