#include "engine/render/MeshMerger.h"
#include "engine/render/Mesh.h"
#include "engine/core/AEString.h"
#include "engine/core/Array.h"
#include "engine/math/AEMath.h"
#include "engine/math/BSphere.h"
#include "engine/render/PaintCanvas.h"
#include <cstdlib>
#include <cstring>

MeshMerger::MeshMerger(const Array<uint16_t> &meshIds, Array<Matrix> transforms,
                       PaintCanvas *canvas, uint16_t flags) {
    this->matrices = nullptr;
    this->canvas = canvas;
    this->cols = 1;
    this->flags = flags;
    this->rows = (int) transforms.size();
    this->sourceMeshes = new Mesh *[meshIds.size()];

    uint32_t vertexCount = 0;
    uint32_t triangleCount = 0;
    for (uint32_t i = 0; i < meshIds.size(); i++) {
        uint32_t meshId;
        canvas->MeshCreate(meshIds.data()[i], meshId, false);
        this->sourceMeshes[i] = canvas->MeshGetPointer(meshId);
        vertexCount += this->sourceMeshes[i]->vertexCount;
        triangleCount += (uint32_t) this->sourceMeshes[i]->indexCount / 3u;
    }

    canvas->MeshCreate((uint16_t) vertexCount, (uint16_t) triangleCount,
                       (signed char) this->sourceMeshes[0]->vertexFormat, flags, this->mergedMeshId);

    uint32_t vertexBase = 0;
    uint32_t triangleBase = 0;
    for (uint32_t i = 0; i < meshIds.size(); i++) {
        Mesh *mesh;
        int positionOffset = 0;
        int uvOffset = 0;
        int colorOffset = 0;
        for (uint32_t v = 0; v < (mesh = this->sourceMeshes[i])->vertexCount; v++) {
            if (mesh->vertexFormat & 1) {
                Vector p = AbyssEngine::AEMath::MatrixTransformVector(
                        transforms.data()[i], *(Vector *) (mesh->positions + positionOffset));
                canvas->MeshSetPoint(this->mergedMeshId, (uint16_t) (vertexBase + v), p.x, p.y, p.z);
                mesh = this->sourceMeshes[i];
            }
            if (mesh->vertexFormat & 4) {
                Vector n = AbyssEngine::AEMath::MatrixRotateVector(
                        transforms.data()[i], *(Vector *) (mesh->normals + positionOffset));
                canvas->MeshSetNormal(this->mergedMeshId, (uint16_t) (vertexBase + v), n);
                mesh = this->sourceMeshes[i];
            }
            if (mesh->vertexFormat & 2) {
                float *uv = (float *) (mesh->texCoords + uvOffset);
                canvas->MeshSetUv(this->mergedMeshId, (uint16_t) (vertexBase + v), uv[0], uv[1]);
                mesh = this->sourceMeshes[i];
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
                mesh = this->sourceMeshes[i];
            }
            indexOffset += 6;
        }
        triangleBase += (uint32_t) mesh->indexCount / 3u;
        vertexBase += mesh->vertexCount;
    }

    canvas->TransformCreate(this->transformId);
    canvas->TransformAddMeshId(this->transformId, this->mergedMeshId);
    this->transformedMeshes = nullptr;
    this->initialized = 1;
    this->mergedMesh = nullptr;
    this->lods = nullptr;
    this->dirty = 0;
}

MeshMerger::MeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags)
    : transformedMeshes(nullptr), matrices(nullptr), mergedMesh(nullptr), lods(nullptr) {
    this->canvas = canvas;
    this->rows = rows;
    this->cols = cols;

    int count = cols * rows;
    this->sourceMeshes = new Mesh *[count];
    memset(this->sourceMeshes, 0, count * sizeof(Mesh *));
    this->transformedMeshes = new Mesh *[count];
    memset(this->transformedMeshes, 0, count * sizeof(Mesh *));
    this->lods = new int8_t[rows];
    memset(this->lods, 0, rows);
    this->matrices = new Matrix[rows];
    for (int i = 0; i < this->rows; i++) {
        this->matrices[i] = Matrix();
    }
    this->enabledFlags = new uint8_t[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->enabledFlags[i] = 1;
    }
    this->visibleFlags = new uint8_t[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->visibleFlags[i] = 1;
    }
    this->dirty = 0;
    this->flags = flags;
    this->initialized = 0;
}

MeshMerger::~MeshMerger() {
    if (this->sourceMeshes != nullptr)
        delete[] this->sourceMeshes;
    this->sourceMeshes = nullptr;

    for (int i = 0; i < this->rows * this->cols; i++) {
        if (this->transformedMeshes[i]->positions != nullptr)
            delete[] this->transformedMeshes[i]->positions;
        this->transformedMeshes[i]->positions = nullptr;
        if (this->transformedMeshes[i]->normals != nullptr)
            delete[] this->transformedMeshes[i]->normals;
        this->transformedMeshes[i]->normals = nullptr;
        if (this->transformedMeshes[i] != nullptr)
            delete this->transformedMeshes[i];
        this->transformedMeshes[i] = nullptr;
    }
    if (this->transformedMeshes != nullptr)
        delete[] this->transformedMeshes;
    this->transformedMeshes = nullptr;

    delete[] this->lods;
    this->lods = nullptr;
    delete[] this->enabledFlags;
    this->enabledFlags = nullptr;
    delete[] this->visibleFlags;
    this->visibleFlags = nullptr;

    if (this->matrices != nullptr)
        delete[] this->matrices;
    this->matrices = nullptr;
}

void MeshMerger::setMatrix(int index, const Matrix &m) {
    this->matrices[index] = m;
}

void MeshMerger::setLod(int index, signed char lod) {
    if (this->lods[index] != lod) {
        this->lods[index] = lod;
        if (((int8_t *) this->enabledFlags)[index] == 0) {
            return;
        }
        this->dirty = 1;
    }
}

void MeshMerger::setMesh(int index, signed char lod, uint16_t meshId) {
    uint32_t id;
    this->canvas->MeshCreate(meshId, id, false);
    Mesh *ptr = this->canvas->MeshGetPointer(id);
    this->sourceMeshes[this->rows * lod + index] = ptr;
}

void MeshMerger::setEnabled(int index, bool enabled) {
    if (this->enabledFlags[index] != enabled) {
        this->enabledFlags[index] = enabled;
        if (((int8_t *) this->visibleFlags)[index] == 0) {
            return;
        }
        this->dirty = 1;
    }
}

void MeshMerger::render() {
    this->canvas->DrawTransform(this->transformId, nullptr);
}

void MeshMerger::update() {
    for (int i = 0; i < this->rows; i++) {
        Mesh *mesh = this->transformedMeshes[i];
        int visible = this->canvas->CameraIsSphereinViewFrustum(*(const Vector *) &mesh->boundsCenterX,
                                                                 mesh->boundsRadius);
        if (visible != this->visibleFlags[i]) {
            this->visibleFlags[i] = (uint8_t) visible;
            if (this->enabledFlags[i] != 0)
                this->dirty = 1;
        }
    }
    if (this->dirty == 0)
        return;

    int vertexBase = 0;
    int indexBase = 0;
    for (int i = 0; i < this->rows; i++) {
        if (this->enabledFlags[i] == 0 || this->visibleFlags[i] == 0)
            continue;
        Mesh *src = this->transformedMeshes[this->rows * this->lods[i] + i];
        if (this->mergedMesh->vertexFormat & 1)
            memcpy((Vector *) this->mergedMesh->positions + vertexBase, (const Vector *) src->positions,
                   src->vertexCount * 12);
        if (this->mergedMesh->vertexFormat & 4)
            memcpy((Vector *) this->mergedMesh->normals + vertexBase, (const Vector *) src->normals,
                   src->vertexCount * 12);
        if (this->mergedMesh->vertexFormat & 8)
            memcpy((uint32_t *) this->mergedMesh->colors + vertexBase * 4, (const uint32_t *) src->colors,
                   src->vertexCount * 16);
        if (this->mergedMesh->vertexFormat & 2)
            memcpy((float *) this->mergedMesh->texCoords + vertexBase * 2, (const float *) src->texCoords,
                   src->vertexCount * 8);
        if (this->mergedMesh->vertexFormat & 0x10) {
            uint16_t *dst = (uint16_t *) this->mergedMesh->indices + indexBase;
            const uint16_t *idx = (const uint16_t *) src->indices;
            for (int k = src->indexCount; k != 0; k--)
                *dst++ = (uint16_t) (*idx++ + vertexBase);
        }
        vertexBase += src->vertexCount;
        indexBase += src->indexCount;
    }
    this->mergedMesh->vertexCount = (uint16_t) vertexBase;
    this->mergedMesh->indexCount = (uint16_t) indexBase;
    this->dirty = 0;
}

void MeshMerger::init() {
    if (this->initialized != 0) {
        return;
    }

    for (int i = 0; i < this->rows; i++) {
        int c = 0;
        if (this->lods[i] >= -1 && this->cols <= this->lods[i]) {
            this->lods[i] = 0;
        }
        for (; c < this->cols; c++) {
            Mesh *mesh = this->sourceMeshes[this->rows * c + i];
            if (mesh != nullptr) {
                this->transformedMeshes[this->rows * c + i] =
                        transformMesh(mesh, this->matrices[i]);
            }
        }
    }

    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
    for (int i = 0; i < this->rows; i++) {
        Mesh *mesh = this->sourceMeshes[i];
        indexCount += (uint32_t) mesh->indexCount / 3u;
        vertexCount += mesh->vertexCount;
    }

    this->canvas->MeshCreate((uint16_t) vertexCount, (uint16_t) indexCount,
                             (signed char) this->sourceMeshes[0]->vertexFormat, this->flags,
                             this->mergedMeshId);
    this->mergedMesh = this->canvas->MeshGetPointer(this->mergedMeshId);
    this->canvas->TransformCreate(this->transformId);
    this->canvas->TransformAddMeshId(this->transformId, this->mergedMeshId);
    this->dirty = 1;
    this->update();
}

MeshMerger::Mesh *MeshMerger::transformMesh(Mesh *mesh, const Matrix &m) {
    Mesh *out = new Mesh();
    out->vertexCount = mesh->vertexCount;
    out->indexCount = mesh->indexCount;
    out->vertexFormat = mesh->vertexFormat;
    if (mesh->vertexFormat & 2)
        out->texCoords = mesh->texCoords;
    if (mesh->vertexFormat & 8)
        out->colors = mesh->colors;
    if (mesh->vertexFormat & 0x10)
        out->indices = mesh->indices;

    if (mesh->vertexFormat & 1) {
        out->positions = new unsigned char[mesh->vertexCount * 12];
        for (unsigned int i = 0; i < mesh->vertexCount; i++) {
            ((Vector *) out->positions)[i] =
                    AbyssEngine::AEMath::MatrixTransformVector(m, ((const Vector *) mesh->positions)[i]);
        }
    }
    if (mesh->vertexFormat & 4) {
        out->normals = new unsigned char[mesh->vertexCount * 12];
        for (unsigned int i = 0; i < mesh->vertexCount; i++) {
            ((Vector *) out->normals)[i] = AbyssEngine::AEMath::VectorNormalize(
                    AbyssEngine::AEMath::MatrixRotateVector(m, ((const Vector *) mesh->normals)[i]));
        }
    }

    Vector center = AbyssEngine::AEMath::MatrixTransformVector(m, *(const Vector *) &mesh->boundsCenterX);
    AbyssEngine::AEMath::BSphere bounds = {center, mesh->boundsRadius, 1.0f};
    *reinterpret_cast<AbyssEngine::AEMath::BSphere *>(&out->boundsCenterX) = bounds;
    return out;
}
