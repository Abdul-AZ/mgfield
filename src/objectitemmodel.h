#ifndef OBJECTITEMMODEL_H
#define OBJECTITEMMODEL_H

#include <QAbstractItemModel>

#include "scene.h"

class ObjectItemModel : public QAbstractItemModel
{
public:
    explicit ObjectItemModel(Scene* scene, QObject *parent = nullptr);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex& index) const override { return QModelIndex(); }
    virtual int         rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int         columnCount(const QModelIndex &parent = QModelIndex()) const override { return 1; }
    virtual QVariant    data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    Scene* m_Scene = nullptr;
};

#endif // OBJECTITEMMODEL_H
