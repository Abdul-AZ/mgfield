#include "objectitemdelegate.h"

#include "scene/object.h"
#include <QApplication>
#include <QPainter>
#include <QPainterPath>

ObjectItemDelegate::ObjectItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{}

void ObjectItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const int elementMarginLeft = OBJECT_ITEM_DELEGATE_HORIZONTAL_MARGIN;
    const int elementMarginHeight = OBJECT_ITEM_DELEGATE_VERTICAL_MARGIN;
    const int elementHeight = 2 * elementMarginHeight + option.fontMetrics.height();
    QPoint position = QPoint(0, elementHeight + elementHeight * index.row());
    if(index.data().canConvert<Object*>())
    {
        Object* object = index.data().value<Object*>();

        QPoint textLocation = {position.x()+ elementMarginLeft, position.y() - elementMarginHeight * 2};

        painter->setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        QRect rect(position.x() , position.y(), option.widget->width(), -elementHeight);
        path.addRect(rect);
        if(m_SelectedObject.get() == object)
        {
            painter->fillPath(path, QApplication::palette().highlight().color());
        }
        else
        {
            painter->fillPath(path, option.backgroundBrush);
        }

        QFont font(option.font.family(), -1, QFont::Bold);
        painter->setFont(font);
        painter->setPen(QPen(QApplication::palette().text().color(), 2));
        painter->drawText(textLocation, object->Name);
    }
}

void ObjectItemDelegate::ObjectSelected(std::shared_ptr<Object> object)
{
    m_SelectedObject = object;
}

QSize ObjectItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const int elementMarginHeight = OBJECT_ITEM_DELEGATE_VERTICAL_MARGIN;
    const int elementHeight = 2 * elementMarginHeight + option.fontMetrics.height();
    return QSize(option.widget->width() - 3, elementHeight);
}
