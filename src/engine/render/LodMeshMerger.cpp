#include "engine/render/LodMeshMerger.h"
#include "engine/math/BSphere.h"
#include "engine/math/AEMath.h"
#include "engine/render/PaintCanvas.h"
#include <cstdlib>

void LodMeshMerger::setEnabled(int index, bool value) {
    if (enabled[index] != value) {
        enabled[index] = value;
        if (visible[index] != 0) {
            dirty = 1;
        }
    }
}

void LodMeshMerger::setLod(int index, signed char lod) {
    if (lodLevels[index] != lod) {
        lodLevels[index] = lod;
        if (enabled[index] != 0) {
            dirty = 1;
        }
    }
}

void LodMeshMerger::setMatrix(int index, const Matrix &m) {
    transforms[index] = m;
}

void LodMeshMerger::setMesh(int index, signed char lod, uint16_t meshId) {
    uint32_t id;
    canvas->MeshCreate(meshId, id, false);
    Mesh *ptr = canvas->MeshGetPointer(id);
    meshes.data()[rows * lod + index] = ptr;
}

void LodMeshMerger::update() {
    for (int i = 0; i < rows; i++) {
        Mesh *sph = transformedMeshes[i];
        uint8_t vis = (uint8_t) canvas->CameraIsSphereinViewFrustum(
            *(const Vector *) &sph->boundsCenterX, sph->boundsRadius);
        if (vis != visible[i]) {
            visible[i] = vis;
            if (enabled[i] != 0) {
                dirty = 1;
            }
        }
    }

    if (dirty == 0) {
        return;
    }

    int indexBudget = 0;
    for (int j = 0; j < rows; j++) {
        if (enabled[j] != 0 && visible[j] != 0) {
            signed char lod = lodLevels[j];
            Mesh *src = transformedMeshes[rows * lod + j];
            indexBudget += src->indexCount;
        }
    }

    for (int j = 0; indexBudget >= 0x10000 && j < rows; j++) {
        if (enabled[j] != 0 && visible[j] != 0) {
            signed char lod = lodLevels[j];
            if (lod < cols - 1) {
                Mesh *prev = transformedMeshes[rows * lod + j];
                setLod(j, (signed char) (lod + 1));
                signed char newLod = lodLevels[j];
                Mesh *cur = transformedMeshes[rows * newLod + j];
                indexBudget += cur->indexCount - prev->indexCount;
            }
        }
    }

    Mesh *out = mergedMesh;
    int vtxOffset = 0;
    int idxOffset = 0;
    for (int j = 0; j < rows; j++) {
        if (enabled[j] != 0 && visible[j] != 0) {
            uint8_t mask = out->vertexFormat;
            signed char lod = lodLevels[j];
            Mesh *src = transformedMeshes[rows * lod + j];

            if (mask & 1) {
                memcpy((char *) out->positions + vtxOffset * 0xc,
                       src->positions, src->vertexCount * 0xc);
            }
            if (mask & 4) {
                memcpy((char *) out->normals + vtxOffset * 0xc,
                       src->normals, src->vertexCount * 0xc);
            }
            if (mask & 8) {
                memcpy((char *) out->colors + vtxOffset * 0x10,
                       src->colors, (uint32_t) src->vertexCount << 4);
            }
            if (mask & 2) {
                memcpy((char *) out->texCoords + vtxOffset * 8,
                       src->texCoords, (uint32_t) src->vertexCount << 3);
            }
            if (mask & 0x10) {
                int16_t *si = (int16_t *) src->indices;
                int16_t *di = (int16_t *) out->indices + idxOffset;
                for (int k = -(int)(uint16_t)src->indexCount; k != 0; k++) {
                    *di = (int16_t)(*si + (int16_t) vtxOffset);
                    si++;
                    di++;
                }
            }
            idxOffset += src->indexCount;
            vtxOffset += src->vertexCount;
        }
    }
    out->indexCount = (uint16_t) idxOffset;
    out->vertexCount = (uint16_t) vtxOffset;
    dirty = 0;
}

LodMeshMerger::LodMeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags)
    : mergedMesh(nullptr), transformedMeshes(nullptr), transforms(nullptr), lodLevels(nullptr) {
    this->canvas = canvas;
    this->rows = rows;
    this->cols = cols;
    ArraySetLength(cols * rows, this->meshes);

    int count = cols * this->rows;
    this->transformedMeshes = new Mesh *[count];
    memset(this->transformedMeshes, 0, count * sizeof(Mesh *));
    this->lodLevels = new int8_t[this->rows];
    memset(this->lodLevels, 0, this->rows);
    this->transforms = new Matrix[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->transforms[i] = Matrix();
    }
    this->enabled = new uint8_t[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->enabled[i] = 1;
    }
    this->visible = new uint8_t[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->visible[i] = 1;
    }
    this->dirty = 0;
    this->flags = flags;
    this->initialized = 0;
}

void LodMeshMerger::init() {
    if (initialized != 0) {
        return;
    }

    for (int i = 0; i < rows; i++) {
        int c = 0;
        if (lodLevels[i] >= -1 && cols <= lodLevels[i]) {
            lodLevels[i] = 0;
        }
        for (; c < cols; c++) {
            Mesh *mesh = meshes.data()[rows * c + i];
            if (mesh != nullptr) {
                transformedMeshes[rows * c + i] = transformMesh(mesh, transforms[i]);
            }
        }
    }

    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
    for (int i = 0; i < rows; i++) {
        Mesh *mesh = meshes.data()[i];
        indexCount += (uint32_t) mesh->indexCount / 3u;
        vertexCount += mesh->vertexCount;
    }
    if (vertexCount > 65535u) vertexCount = 65535u;
    if (indexCount > 65535u) indexCount = 65535u;

    canvas->MeshCreate((uint16_t) vertexCount, (uint16_t) indexCount,
                       (signed char) meshes.data()[0]->vertexFormat,
                       flags, mergedMeshId);
    mergedMesh = canvas->MeshGetPointer(mergedMeshId);
    canvas->TransformCreate(transformId);
    canvas->TransformAddMeshId(transformId, mergedMeshId);
    dirty = 1;
    update();
}

Mesh *LodMeshMerger::transformMesh(Mesh *mesh, const Matrix &m) {
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

    // The original transforms the radius vector but then measures the untransformed one.
    Vector extent = {mesh->boundsRadius, mesh->boundsRadius, mesh->boundsRadius};
    AbyssEngine::AEMath::MatrixTransformVector(m, extent);
    float ax = extent.x > 0.0f ? extent.x : -extent.x;
    float ay = extent.y > 0.0f ? extent.y : -extent.y;
    float az = extent.z > 0.0f ? extent.z : -extent.z;
    Vector center = AbyssEngine::AEMath::MatrixTransformVector(m, *(const Vector *) &mesh->boundsCenterX);
    float radius = az;
    if (ay > radius)
        radius = ay;
    if (ax > radius)
        radius = ax;
    AbyssEngine::AEMath::BSphere bounds = {center, radius, 1.0f};
    *reinterpret_cast<AbyssEngine::AEMath::BSphere *>(&out->boundsCenterX) = bounds;
    return out;
}


LodMeshMerger::~LodMeshMerger() {
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

    delete[] this->lodLevels;
    this->lodLevels = nullptr;
    delete[] this->enabled;
    this->enabled = nullptr;
    delete[] this->visible;
    this->visible = nullptr;

    if (this->transforms != nullptr)
        delete[] this->transforms;
    this->transforms = nullptr;
}
