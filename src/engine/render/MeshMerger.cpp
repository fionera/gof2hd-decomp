#include "engine/render/MeshMerger.h"
#include "engine/render/Mesh.h"
#include "engine/core/AEString.h"
#include "engine/core/Array.h"
#include "engine/math/AEMath.h"
#include "engine/math/BSphere.h"
#include "engine/render/PaintCanvas.h"
#include <cstdlib>
#include <cstring>

uint16_t aeabi_uidiv16(uint16_t a, uint16_t b);

void aeabi_memcpy4(void *dst, const void *src, uint32_t n); // lint: void_ptr (external symbol signature)

void *aeabi_memclr4(void *p, uint32_t n); // lint: void_ptr (external symbol signature)

void *aeabi_memclr(void *p, uint32_t n); // lint: void_ptr (external symbol signature)

void AEMath_MatrixTransformVector(Vector *out, const Vector *v);

void AEMath_MatrixRotateVector(Vector *out, const Vector *v);



MeshMerger::MeshMerger(const Array<uint16_t> &meshIds, Array<Matrix> transforms,
                       PaintCanvas *canvas, uint16_t flags) {
    this->matrices = nullptr;
    this->canvas = canvas;
    this->cols = 1;
    this->flags = flags;
    this->rows = (int) transforms.size();

    uint32_t count = meshIds.size();
    Mesh **table = new Mesh *[count];
    this->sourceMeshes = table;

    int16_t totalV = 0;
    int16_t totalI = 0;
    for (uint32_t i = 0; i < meshIds.size(); i++) {
        uint32_t localId;
        canvas->MeshCreate(meshIds.data()[i], localId, false);
        table[i] = (Mesh *) canvas->MeshGetPointer(localId);
        Mesh *m = table[i];
        totalV = (int16_t)(totalV + m->vertexCount);
        totalI = (int16_t)(totalI + aeabi_uidiv16(m->indexCount, 3));
    }

    Mesh *m0 = table[0];
    canvas->MeshCreate((uint16_t) totalV, (uint16_t) totalI, (signed char) m0->vertexFormat,
                       flags, this->mergedMeshId);

    int16_t triBase = 0;
    int16_t vtxBase = 0;
    for (uint32_t i = 0; i < meshIds.size(); i++) {
        Mesh *m = table[i];
        Matrix *xf = &transforms.data()[i];
        int colOff = 0;
        int uvOff = 0;
        uint16_t nv = m->vertexCount;
        for (uint16_t v = 0; v < nv; v++) {
            m = table[i];
            uint8_t fl = m->vertexFormat;
            Vector tmp;
            if (fl & 1) {
                AEMath_MatrixTransformVector(&tmp, (const Vector *) xf);
                canvas->MeshSetPoint(this->mergedMeshId, (uint16_t)(vtxBase + v), tmp.x, tmp.y, tmp.z);
                fl = m->vertexFormat;
            }
            if (fl & 8) {
                AEMath_MatrixRotateVector(&tmp, (const Vector *) xf);
                canvas->MeshSetNormal(this->mergedMeshId, (uint16_t)(vtxBase + v), tmp);
                fl = m->vertexFormat;
            }
            if (fl & 2) {
                float *uv = (float *) ((char *) m->texCoords + uvOff);
                canvas->MeshSetUv(this->mergedMeshId, (uint16_t)(vtxBase + v), uv[1], uv[0]);
                fl = m->vertexFormat;
            }
            if (fl & 0x10) {
                float *col = (float *) ((char *) (uintptr_t) m->colors + colOff);
                canvas->MeshSetColor(this->mergedMeshId, (uint16_t)(vtxBase + v),
                                     col[1], col[0], col[2], col[3]);
            }
            uvOff += 8;
            colOff += 0x10;
        }

        uint16_t tris = aeabi_uidiv16(m->indexCount, 3);
        int triOff = 0;
        for (uint16_t t = 0; t < tris; t++) {
            if (m->vertexFormat & 0x20) {
                int16_t *ix = (int16_t *) ((char *) m->indices + triOff);
                canvas->MeshSetTriangle(this->mergedMeshId, (uint16_t)(triBase + t),
                                        (uint16_t)(ix[0] + vtxBase), (uint16_t)(ix[1] + vtxBase),
                                        (uint16_t)(ix[2] + vtxBase));
                m = table[i];
            }
            triOff += 6;
        }
        triBase = (int16_t)(triBase + aeabi_uidiv16(m->indexCount, 3));
        vtxBase = (int16_t)(vtxBase + m->vertexCount);
    }

    canvas->TransformCreate(this->transformId);
    canvas->TransformAddMeshId(this->transformId, this->mergedMeshId);
    this->transformedMeshes = nullptr;
    this->initialized = 1;
    this->mergedMesh = nullptr;
    this->lods = nullptr;
    this->dirty = 0;
}

MeshMerger::MeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags) {
    this->transformedMeshes = nullptr;
    this->matrices = nullptr;
    this->mergedMesh = nullptr;
    this->lods = nullptr;
    this->canvas = canvas;
    this->rows = rows;
    this->cols = cols;

    int bytes = cols * rows * 4;
    long long need = (long long) (unsigned) (cols * rows) * 4;
    int req = (int) ((need >> 32) != 0 ? 0xffffffff : (unsigned) need);

    this->sourceMeshes = (Mesh **) new unsigned char[req];
    memset(this->sourceMeshes, 0, bytes);
    this->transformedMeshes = (Mesh **) new unsigned char[req];
    memset(this->transformedMeshes, 0, bytes);

    this->lods = (int8_t *) new unsigned char[rows];
    memset(this->lods, 0, rows);

    long long mneed = (long long) (unsigned) rows * 0x3c;
    int mreq = (int) ((mneed >> 32) != 0 ? 0xffffffff : (unsigned) mneed);
    char *matrices = (char *) new unsigned char[mreq];
    for (int off = 0; rows != 0 && off != rows * 0x3c; off += 0x3c)
        new(matrices + off) Matrix();
    this->matrices = matrices;

    Matrix ident;
    int n = this->rows;
    for (int i = 0, off = 0; i < n; i++, off += 0x3c)
        *(Matrix *) (matrices + off) = ident;

    n = this->rows;
    uint8_t *en = new uint8_t[n];
    this->enabledFlags = en;
    for (int i = 0; i < n; i++) en[i] = 1;

    uint8_t *vis = new uint8_t[n];
    this->visibleFlags = vis;
    for (int i = 0; i < n; i++) vis[i] = 1;

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
    *(Matrix *) (this->matrices + index * 0x3c) = m;
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
                        transformMesh(mesh, *(const Matrix *) (this->matrices + i * 0x3c));
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
