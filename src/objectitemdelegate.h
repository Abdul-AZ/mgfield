#ifndef OBJECTITEMDELEGATE_H
#define OBJECTITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include "scene/object.h"

#define OBJECT_ITEM_DELEGATE_HORIZONTAL_MARGIN (3)
#define OBJECT_ITEM_DELEGATE_VERTICAL_MARGIN   (3)

class ObjectItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ObjectItemDelegate(QObject *parent = nullptr);
    virtual ~ObjectItemDelegate() {}

    virtual void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

public slots:
    void ObjectSelected(std::shared_ptr<Object> object);

private:
    std::shared_ptr<Object> m_SelectedObject = nullptr;

};

#endif // OBJECTITEMDELEGATE_H
