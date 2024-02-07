#ifndef MODIFIEDDOUBLESPINBOX_H
#define MODIFIEDDOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include <QMouseEvent>

class ModifiedDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    ModifiedDoubleSpinBox(QWidget* parent);
    virtual ~ModifiedDoubleSpinBox() {};

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // MODIFIEDDOUBLESPINBOX_H
