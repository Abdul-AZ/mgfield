#include "modelloader.h"

#include <QFile>
#include <QQuaternion>
#include <QOpenGLFunctions>
#include <QDebug>

#include <thirdparty/tinygltf/tiny_gltf.h>

ModelLoader::SingleMeshData ModelLoader::LoadSingleMeshFile(QString path)
{
    ModelLoader::SingleMeshData meshData;

    using namespace tinygltf;
    Model model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    auto data = file.readAll();
    file.close();

    loader.LoadBinaryFromMemory(&model, &err, &warn, (uint8_t*)data.data(), data.length());

    if((model.meshes.size() != 1) || model.bufferViews.size() != 2)
    {
        qCritical("%s, path: %s", "Unexpected number of meshes present in model", path.toStdString().c_str());
        return ModelLoader::SingleMeshData();
    }

    bool foundVertexData = false;
    for (auto& bufferView : model.bufferViews)
        if(bufferView.target == GL_ARRAY_BUFFER)
        {
            foundVertexData = true;

            meshData.Vertices.resize(bufferView.byteLength / sizeof(QVector3D));

            memcpy(meshData.Vertices.data(), model.buffers.at(bufferView.buffer).data.data() + bufferView.byteOffset, bufferView.byteLength);
            bool needsScaling = model.nodes[0].scale.size() > 0;
            if(needsScaling)
            {
                std::vector<double>& scaleData = model.nodes[0].scale;
                QVector3D scale (scaleData[0], scaleData[1], scaleData[2]);
                for(QVector3D& vec : meshData.Vertices)
                    vec = vec * scale;
            }

            bool needsRotating = model.nodes[0].rotation.size() > 0;
            if(needsRotating)
            {
                std::vector<double>& rotationData = model.nodes[0].rotation; //quat
                QQuaternion rot (rotationData[0], rotationData[1], rotationData[2], rotationData[3]);
                for(QVector3D& vec : meshData.Vertices)
                    vec = rot * vec;
            }
            break;
        }

    if(!foundVertexData)
    {
        qCritical("%s, path: %s", "Invalid vertex buffer in model", path.toStdString().c_str());
        return ModelLoader::SingleMeshData();
    }

    bool foundIndexData = false;
    for (auto& bufferView : model.bufferViews)
        if(bufferView.target == GL_ELEMENT_ARRAY_BUFFER)
        {
            foundIndexData = true;

            // Implies that vertices are always uint16
            meshData.Indecies.resize(bufferView.byteLength / sizeof(uint16_t));
            memcpy(meshData.Indecies.data(), model.buffers.at(bufferView.buffer).data.data() + bufferView.byteOffset, bufferView.byteLength);
            break;
        }

    if(!foundIndexData)
    {
        qCritical("%s, path: %s", "Invalid index buffer in model", path.toStdString().c_str());
        return ModelLoader::SingleMeshData();
    }

    return meshData;
}
