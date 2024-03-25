#include "permanentmagnet.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

PermanentMagnet::PermanentMagnet()
:
    Object(ObjectType::PermanentMagnet)
{
    Name = "Magnet";
}

void PermanentMagnet::SetShape(int32_t magnetID)
{
    QFile file(":/res/data/magnet_models.json");
    file.open(QIODevice::ReadOnly);

    QByteArray byteArray;
    byteArray = file.readAll();
    file.close();

    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(byteArray);

    QJsonArray array = jsonDoc.object().value("models").toArray();
    for(const QJsonValue& model : array)
    {
        QJsonObject obj1 = model.toObject();
        QJsonValue id = obj1.value("id");
        if(id.toInt() == magnetID)
        {
            this->Name = obj1.value("name").toString();

            //TODO load sheet data here

            break;
        }
    }
}
