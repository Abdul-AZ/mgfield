#ifndef ADDOBJECTDIALOG_H
#define ADDOBJECTDIALOG_H

#include <QDialog>

#include "src/scene/object.h"

#include "src/scene/permanentmagnet.h"

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
    int32_t    GetMagnetModelID() const;

private:
    void LoadMagnetModelNames();

private:
    Ui::AddObjectDialog *ui;
};

#endif // ADDOBJECTDIALOG_H
