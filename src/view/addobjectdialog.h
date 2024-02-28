#ifndef ADDOBJECTDIALOG_H
#define ADDOBJECTDIALOG_H

#include <QDialog>

#include "src/scene/object.h"

namespace Ui {
class AddObjectDialog;
}

class AddObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddObjectDialog(QWidget *parent);
    virtual ~AddObjectDialog();

    ObjectType GetSelectedObjectType() const;

private:
    Ui::AddObjectDialog *ui;
};

#endif // ADDOBJECTDIALOG_H
