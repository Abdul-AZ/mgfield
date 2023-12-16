#include "objectlistview.h"

ObjectListView::ObjectListView(QWidget* parent) :
    QListView(parent)
{
    m_Model = new QStringListModel();

    setModel(m_Model);
}

void ObjectListView::ObjectsChanged()
{
    m_Model->removeRows(0, m_Model->rowCount());

    if(!m_CurrentScene)
        return;

    for (auto& cable : m_CurrentScene->Cables)
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
