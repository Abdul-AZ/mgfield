#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <QString>
#include <QVector3D>
#include <vector>

class ModelLoader
{
public:

    // Assumes mesh only has vertex position data
    struct SingleMeshData
    {
        std::vector<QVector3D> Vertices;
        std::vector<uint16_t> Indecies;
    };

    static SingleMeshData LoadSingleMeshFile(QString path);
};

#endif // MODELLOADER_H
