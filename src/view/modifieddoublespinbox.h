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

public:

    /*!
     *  Specifies the magnitude of the value change when holding and dragging the cursor
     */
    float DragSensitivity = 0.01f;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // MODIFIEDDOUBLESPINBOX_H
