#include "objectinspector.h"
#include "ui_objectinspector.h"

ObjectInspector::ObjectInspector(QWidget *parent)
    : QGroupBox(parent)
    , ui(new Ui::ObjectInspector)
{
    ui->setupUi(this);
}

ObjectInspector::~ObjectInspector()
{
    delete ui;
}

void ObjectInspector::ObjectSelected(std::shared_ptr<Object> obj)
{

}
