#include "objectitemmodel.h"

ObjectItemModel::ObjectItemModel(Scene* scene, QObject *parent)
    : QAbstractItemModel{parent}, m_Scene(scene)
{}

QModelIndex ObjectItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if(m_Scene->Objects.count() > row)
        return createIndex(row, column, m_Scene->Objects[row].get());

    return QModelIndex();
}

int ObjectItemModel::rowCount(const QModelIndex &parent) const
{
    return m_Scene->Objects.count();
}

QVariant ObjectItemModel::data(const QModelIndex &index, int role) const
{
    return QVariant::fromValue(m_Scene->Objects[index.row()].get());
}
