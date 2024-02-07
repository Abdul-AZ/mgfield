#include "modifieddoublespinbox.h"

#include <QDebug>
#include <QLineEdit>

ModifiedDoubleSpinBox::ModifiedDoubleSpinBox(QWidget* parent) :
    QDoubleSpinBox(parent)
{

    auto x = lineEdit();
    x->setAttribute(Qt::WA_TransparentForMouseEvents);

}

static QPointF last;

void ModifiedDoubleSpinBox::mousePressEvent(QMouseEvent *event)
{
    last = event->globalPosition();

    grabMouse();
}


void ModifiedDoubleSpinBox::mouseReleaseEvent(QMouseEvent *event)
{
    releaseMouse();
}

void ModifiedDoubleSpinBox::mouseMoveEvent(QMouseEvent *event)
{
    if(event->type() == QEvent::MouseMove && event->buttons() & Qt::LeftButton)
    {
        QPointF delta = event->globalPosition() - last;

        double val = value();
        val += delta.x() * 0.01 - delta.y() * 0.01;
        setValue(val);

        last = event->globalPosition();
    }

}
