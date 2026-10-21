#include "engine/render/SimpleMeshMerger.h"
#include "engine/render/Mesh.h"
#include "engine/core/AEString.h"
#include "engine/core/Array.h"
#include "engine/math/AEMath.h"
#include "engine/render/PaintCanvas.h"

SimpleMeshMerger::SimpleMeshMerger(const Array<unsigned short> &meshIds,
                                   Array<Matrix> transforms,
                                   PaintCanvas *canvas, unsigned short flags) {
    this->canvas = canvas;
    this->flags = flags;
    this->rows = (int) transforms.size();
    ArraySetLength(meshIds.size(), this->meshes);

    uint32_t vertexCount = 0;
    uint32_t triangleCount = 0;
    for (uint32_t i = 0; i < meshIds.size(); i++) {
        uint32_t meshId;
        canvas->MeshCreate(meshIds.data()[i], meshId, false);
        this->meshes[i] = canvas->MeshGetPointer(meshId);
        vertexCount += this->meshes[i]->vertexCount;
        triangleCount += (uint32_t) this->meshes[i]->indexCount / 3u;
    }

    canvas->MeshCreate((uint16_t) vertexCount, (uint16_t) triangleCount,
                       (signed char) this->meshes[0]->vertexFormat, flags, this->mergedMeshId);

    uint32_t vertexBase = 0;
    uint32_t triangleBase = 0;
    for (uint32_t i = 0; i < meshIds.size(); i++) {
        AbyssEngine::Mesh *mesh;
        int positionOffset = 0;
        int uvOffset = 0;
        int colorOffset = 0;
        for (uint32_t v = 0; v < (mesh = this->meshes[i])->vertexCount; v++) {
            if (mesh->vertexFormat & 1) {
                Vector p = AbyssEngine::AEMath::MatrixTransformVector(
                        transforms.data()[i], *(Vector *) (mesh->positions + positionOffset));
                canvas->MeshSetPoint(this->mergedMeshId, (uint16_t) (vertexBase + v), p.x, p.y, p.z);
                mesh = this->meshes[i];
            }
            if (mesh->vertexFormat & 4) {
                Vector n = AbyssEngine::AEMath::MatrixRotateVector(
                        transforms.data()[i], *(Vector *) (mesh->normals + positionOffset));
                canvas->MeshSetNormal(this->mergedMeshId, (uint16_t) (vertexBase + v), n);
                mesh = this->meshes[i];
            }
            if (mesh->vertexFormat & 2) {
                float *uv = (float *) (mesh->texCoords + uvOffset);
                canvas->MeshSetUv(this->mergedMeshId, (uint16_t) (vertexBase + v), uv[0], uv[1]);
                mesh = this->meshes[i];
            }
            if (mesh->vertexFormat & 8) {
                float *color = (float *) (mesh->colors + colorOffset);
                canvas->MeshSetColor(this->mergedMeshId, (uint16_t) (vertexBase + v),
                                     color[0], color[1], color[2], color[3]);
            }
            positionOffset += 12;
            uvOffset += 8;
            colorOffset += 16;
        }

        uint32_t triangles = (uint32_t) mesh->indexCount / 3u;
        int indexOffset = 0;
        for (uint32_t t = 0; t < triangles; t++) {
            if (mesh->vertexFormat & 0x10) {
                uint16_t *ix = (uint16_t *) (mesh->indices + indexOffset);
                canvas->MeshSetTriangle(this->mergedMeshId, (uint16_t) (triangleBase + t),
                                        (uint16_t) (ix[0] + vertexBase), (uint16_t) (ix[1] + vertexBase),
                                        (uint16_t) (ix[2] + vertexBase));
                mesh = this->meshes[i];
            }
            indexOffset += 6;
        }
        triangleBase += (uint32_t) mesh->indexCount / 3u;
        vertexBase += mesh->vertexCount;
    }

    canvas->TransformCreate(this->transformId);
    canvas->TransformAddMeshId(this->transformId, this->mergedMeshId);
    this->initialized = 1;
}
