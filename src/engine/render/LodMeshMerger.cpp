#include "engine/render/LodMeshMerger.h"
#include "engine/math/BSphere.h"
#include "engine/math/AEMath.h"
#include "engine/render/PaintCanvas.h"
#include <cstring>

void LodMeshMerger::setEnabled(int index, bool value) {
    if (enabled[index] != value) {
        enabled[index] = value;
        if (visible[index]) {
            dirty = 1;
        }
    }
}

void LodMeshMerger::setLod(int index, signed char lod) {
    if (lodLevels[index] != lod) {
        lodLevels[index] = lod;
        if (enabled[index]) {
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
    meshes[rows * lod + index] = ptr;
}

void LodMeshMerger::update() {
    for (int i = 0; i < this->rows; i++) {
        Mesh *mesh = this->transformedMeshes[i];
        bool isVisible = this->canvas->CameraIsSphereinViewFrustum(mesh->bounds.center,
                                                                   mesh->bounds.radius);
        if (isVisible != this->visible[i]) {
            this->visible[i] = isVisible;
            if (this->enabled[i])
                this->dirty = 1;
        }
    }
    if (!this->dirty)
        return;

    // Keep the merged index count addressable by 16-bit indices: coarsen LODs until it fits.
    int indexTotal = 0;
    for (int i = 0; i < this->rows; i++) {
        if (this->enabled[i] && this->visible[i])
            indexTotal += this->transformedMeshes[this->rows * this->lodLevels[i] + i]->indexCount;
    }
    for (int i = 0; indexTotal >= 0x10000 && i < this->rows; i++) {
        if (!this->enabled[i] || !this->visible[i])
            continue;
        signed char lod = this->lodLevels[i];
        if (lod < this->cols - 1) {
            Mesh *previous = this->transformedMeshes[this->rows * lod + i];
            setLod(i, (signed char) (lod + 1));
            Mesh *current = this->transformedMeshes[this->rows * this->lodLevels[i] + i];
            indexTotal = indexTotal - previous->indexCount + current->indexCount;
        }
    }

    int vertexBase = 0;
    int indexBase = 0;
    for (int i = 0; i < this->rows; i++) {
        if (!this->enabled[i] || !this->visible[i])
            continue;
        Mesh *src = this->transformedMeshes[this->rows * this->lodLevels[i] + i];
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

LodMeshMerger::LodMeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags)
    : mergedMesh(nullptr), transformedMeshes(nullptr), transforms(nullptr), lodLevels(nullptr) {
    this->canvas = canvas;
    this->rows = rows;
    this->cols = cols;
    ArraySetLength(cols * rows, this->meshes);

    this->transformedMeshes = new Mesh *[cols * this->rows];
    memset(this->transformedMeshes, 0, cols * this->rows * sizeof(Mesh *));
    this->lodLevels = new int8_t[this->rows];
    memset(this->lodLevels, 0, this->rows);
    this->transforms = new Matrix[this->rows];
    // Matrix has a non-trivial default constructor; the explicit re-initialisation loop is
    // part of the original ctor shape (new[] alone emits a different loop).
    for (int i = 0; i < this->rows; i++) {
        this->transforms[i] = Matrix();
    }
    this->enabled = new bool[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->enabled[i] = true;
    }
    this->visible = new bool[this->rows];
    for (int i = 0; i < this->rows; i++) {
        this->visible[i] = true;
    }
    this->flags = flags;
    this->dirty = 0;
    this->initialized = 0;
}

void LodMeshMerger::init() {
    if (initialized) {
        return;
    }

    for (int i = 0; i < rows; i++) {
        int c = 0;
        if (lodLevels[i] < -1 || cols <= lodLevels[i]) {
            lodLevels[i] = 0;
        }
        for (; c < cols; c++) {
            Mesh *mesh = meshes[rows * c + i];
            if (mesh) {
                transformedMeshes[rows * c + i] = transformMesh(mesh, transforms[i]);
            }
        }
    }

    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
    for (int i = 0; i < rows; i++) {
        Mesh *mesh = meshes[i];
        vertexCount += mesh->vertexCount;
        indexCount += (uint32_t) mesh->indexCount / 3u;
    }
    if (vertexCount > 65535u) vertexCount = 65535u;
    if (indexCount > 65535u) indexCount = 65535u;

    canvas->MeshCreate((uint16_t) vertexCount, (uint16_t) indexCount,
                       (signed char) meshes[0]->vertexFormat,
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
        out->positions = new float[mesh->vertexCount * 3];
        for (unsigned int i = 0; i < mesh->vertexCount; i++) {
            ((Vector *) out->positions)[i] =
                    AbyssEngine::AEMath::MatrixTransformVector(m, ((const Vector *) mesh->positions)[i]);
        }
    }
    if (mesh->vertexFormat & 4) {
        out->normals = new float[mesh->vertexCount * 3];
        for (unsigned int i = 0; i < mesh->vertexCount; i++) {
            ((Vector *) out->normals)[i] = AbyssEngine::AEMath::VectorNormalize(
                    AbyssEngine::AEMath::MatrixRotateVector(m, ((const Vector *) mesh->normals)[i]));
        }
    }

    // The original transforms the radius vector but then measures the untransformed one.
    Vector extent(mesh->bounds.radius, mesh->bounds.radius, mesh->bounds.radius);
    AbyssEngine::AEMath::MatrixTransformVector(m, extent);
    float ax = extent.x > 0.0f ? extent.x : -extent.x;
    float ay = extent.y > 0.0f ? extent.y : -extent.y;
    float az = extent.z > 0.0f ? extent.z : -extent.z;
    float radius = az;
    if (ay > radius)
        radius = ay;
    if (ax > radius)
        radius = ax;
    // The transform call stays in the constructor argument: it fixes the order the
    // centre and radius temporaries take on the stack (the original's slot layout).
    out->bounds = AbyssEngine::AEMath::BSphere(
            AbyssEngine::AEMath::MatrixTransformVector(m, mesh->bounds.center), radius, 1.0f);
    return out;
}


LodMeshMerger::~LodMeshMerger() {
    for (int i = 0; i < this->rows * this->cols; i++) {
        if (this->transformedMeshes[i]->positions)
            delete[] this->transformedMeshes[i]->positions;
        this->transformedMeshes[i]->positions = nullptr;
        if (this->transformedMeshes[i]->normals)
            delete[] this->transformedMeshes[i]->normals;
        this->transformedMeshes[i]->normals = nullptr;
        if (this->transformedMeshes[i])
            delete this->transformedMeshes[i];
        this->transformedMeshes[i] = nullptr;
    }
    if (this->transformedMeshes)
        delete[] this->transformedMeshes;
    this->transformedMeshes = nullptr;

    delete[] this->lodLevels;
    this->lodLevels = nullptr;
    delete[] this->enabled;
    this->enabled = nullptr;
    delete[] this->visible;
    this->visible = nullptr;

    if (this->transforms)
        delete[] this->transforms;
    this->transforms = nullptr;
}
