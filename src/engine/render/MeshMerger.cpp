#include "engine/render/MeshMerger.h"
#include "engine/render/Mesh.h"
#include "engine/core/AEString.h"
#include "engine/core/Array.h"
#include "engine/math/AEMath.h"
#include "engine/render/PaintCanvas.h"

uint16_t aeabi_uidiv16(uint16_t a, uint16_t b);

void aeabi_memcpy4(void *dst, const void *src, uint32_t n); // lint: void_ptr (external symbol signature)

void *aeabi_memclr4(void *p, uint32_t n); // lint: void_ptr (external symbol signature)

void *aeabi_memclr(void *p, uint32_t n); // lint: void_ptr (external symbol signature)

void AEMath_MatrixTransformVector(Vector *out, const Vector *v);

void AEMath_MatrixRotateVector(Vector *out, const Vector *v);

void AEMath_BSphere_assign(void *dst, const void *src); // lint: void_ptr (external symbol signature)

static void (**g_mmRenderFn)(PaintCanvas *canvas, uint32_t transformId, int z) = nullptr;

static unsigned char *(**g_allocFn)(int) = nullptr;

static void (**g_freeFn)(unsigned char *) = nullptr;

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
    unsigned char *(*alloc)(int) = *g_allocFn;

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

    this->sourceMeshes = (Mesh **) alloc(req);
    aeabi_memclr4(this->sourceMeshes, bytes);
    this->transformedMeshes = (Mesh **) alloc(req);
    aeabi_memclr4(this->transformedMeshes, bytes);

    this->lods = (int8_t *) alloc(rows | (rows >> 31));
    aeabi_memclr(this->lods, rows);

    long long mneed = (long long) (unsigned) rows * 0x3c;
    int mreq = (int) ((mneed >> 32) != 0 ? 0xffffffff : (unsigned) mneed);
    char *matrices = (char *) alloc(mreq);
    for (int off = 0; rows != 0 && off != rows * 0x3c; off += 0x3c)
        new(matrices + off) Matrix();
    this->matrices = matrices;

    Matrix ident;
    int n = this->rows;
    for (int i = 0, off = 0; i < n; i++, off += 0x3c)
        *(Matrix *) (matrices + off) = ident;

    n = this->rows;
    uint8_t *en = new uint8_t[n | (n >> 31)];
    this->enabledFlags = en;
    for (int i = 0; i < n; i++) en[i] = 1;

    uint8_t *vis = new uint8_t[n | (n >> 31)];
    this->visibleFlags = vis;
    for (int i = 0; i < n; i++) vis[i] = 1;

    this->dirty = 0;
    this->flags = flags;
    this->initialized = 0;
}

MeshMerger::~MeshMerger() {
    if (this->sourceMeshes != nullptr) delete[] this->sourceMeshes;
    this->sourceMeshes = nullptr;

    Mesh **slots = nullptr;
    for (int i = 0; ; i++) {
        slots = this->transformedMeshes;
        if (i >= this->rows * this->cols) break;

        Mesh *cell = slots[i];
        if (cell != nullptr) {
            if (cell->positions != nullptr) delete[] (char *) cell->positions;
            cell->positions = nullptr;
            if (cell->normals != nullptr) delete[] (char *) cell->normals;
            cell->normals = nullptr;
            delete cell;
        }
        slots[i] = nullptr;
    }
    if (slots != nullptr) delete[] slots;
    this->transformedMeshes = nullptr;

    void (*freeFn)(unsigned char *) = *g_freeFn;
    freeFn((unsigned char *) this->lods);
    this->lods = nullptr;
    freeFn(this->enabledFlags);
    this->enabledFlags = nullptr;
    freeFn(this->visibleFlags);
    this->visibleFlags = nullptr;

    if (this->matrices != nullptr) delete[] this->matrices;
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
    (*g_mmRenderFn)(this->canvas, this->transformId, 0);
}

void MeshMerger::update() {
    int rows = this->rows;
    for (int i = 0; i < rows; i++) {
        Mesh *sph = this->transformedMeshes[i];
        Vector boundsCenter = {sph->boundsCenterX, sph->boundsCenterY, sph->boundsCenterZ};
        uint8_t vis = (uint8_t) this->canvas->CameraIsSphereinViewFrustum(
            boundsCenter, sph->boundsRadius);
        int8_t *visArr = (int8_t *) this->visibleFlags;
        if (vis != (uint8_t) visArr[i]) {
            visArr[i] = (int8_t) vis;
            if (((int8_t *) this->enabledFlags)[i] != 0) {
                this->dirty = 1;
            }
        }
        rows = this->rows;
    }

    if (this->dirty == 0) {
        return;
    }

    Mesh *out = this->mergedMesh;
    int idxOff = 0;
    int vtxOff = 0;
    for (int j = 0; j < rows; j++) {
        if (((int8_t *) this->enabledFlags)[j] != 0 &&
            ((int8_t *) this->visibleFlags)[j] != 0) {
            uint8_t mask = out->vertexFormat;
            signed char lod = this->lods[j];
            Mesh *src = this->transformedMeshes[rows * lod + j];

            if (mask & 1) {
                aeabi_memcpy4((char *) out->positions + vtxOff * 0xc,
                              src->positions, (uint32_t) src->vertexCount * 0xc);
            }
            if (mask & 4) {
                aeabi_memcpy4((char *) out->normals + vtxOff * 0xc,
                              src->normals, (uint32_t) src->vertexCount * 0xc);
            }
            if (mask & 8) {
                aeabi_memcpy4((char *) (uintptr_t) out->colors + vtxOff * 0x10,
                              (const char *) (uintptr_t) src->colors,
                              (uint32_t) src->vertexCount << 4);
            }
            if (mask & 2) {
                aeabi_memcpy4((char *) out->texCoords + vtxOff * 8,
                              src->texCoords, (uint32_t) src->vertexCount << 3);
            }
            if (mask & 0x10) {
                int16_t *si = (int16_t *) src->indices;
                int16_t *di = (int16_t *) ((char *) out->indices + idxOff * 2);
                for (int k = -(int) (uint16_t) src->indexCount; k != 0; k++) {
                    *di = (int16_t)(*si + (int16_t) vtxOff);
                    si++;
                    di++;
                }
            }
            rows = this->rows;
            idxOff += src->indexCount;
            vtxOff += src->vertexCount;
        }
    }
    out->indexCount = (uint16_t) idxOff;
    out->vertexCount = (uint16_t) vtxOff;
    this->dirty = 0;
}

int MeshMerger::init() {
    if (this->initialized != 0) {
        return this->initialized;
    }

    int rows;
    for (int i = 0; i < (rows = this->rows); i++) {
        int lod = this->lods[i];
        if (lod >= -1 && this->cols <= lod) {
            this->lods[i] = 0;
        }
        for (int c = 0; c < this->cols; c++) {
            Mesh *mesh = this->sourceMeshes[this->rows * c + i];
            if (mesh != nullptr) {
                Mesh *t = (Mesh *) this->transformMesh(mesh, *(const Matrix *) (this->matrices + i * 0x3c));
                this->transformedMeshes[this->rows * c + i] = t;
            }
        }
    }

    uint16_t nv = 0;
    uint16_t ni = 0;
    for (int i = 0; i < rows; i++) {
        Mesh *m0 = this->sourceMeshes[i];
        ni = ni + aeabi_uidiv16(m0->indexCount, 3);
        nv = nv + m0->vertexCount;
    }

    Mesh *first = this->sourceMeshes[0];
    this->canvas->MeshCreate(nv, ni, (signed char) first->vertexFormat, this->mergedMeshId);
    this->mergedMesh = this->canvas->MeshGetPointer(this->mergedMeshId);
    this->canvas->TransformCreate(this->transformId);
    this->canvas->TransformAddMeshId(this->transformId, this->mergedMeshId);
    this->dirty = 1;

    this->initialized = 1;
    this->update();
    return this->initialized;
}

void *MeshMerger::transformMesh(Mesh *mesh, const Matrix &m) { // lint: void_ptr (method return type baked into symbol-parity contract)
    // lint: void_ptr (method return type baked into symbol-parity contract)
    Mesh *out = new Mesh(mesh);
    out->vertexCount = mesh->vertexCount;
    out->indexCount = mesh->indexCount;
    uint8_t flags = mesh->vertexFormat;
    out->vertexFormat = flags;

    if (flags & 2) {
        out->texCoords = mesh->texCoords;
    }
    if (flags & 8) {
        out->colors = mesh->colors;
    }
    if (flags & 0x10) {
        out->indices = mesh->indices;
    }
    uint16_t nv = mesh->vertexCount;
    if (flags & 1) {
        char *buf = new char[(uint32_t) nv * 0xc];
        out->positions = (unsigned char *) buf;
        int off = 0;
        for (uint32_t k = 0; k < nv; k++) {
            Vector tmp;
            AEMath_MatrixTransformVector(&tmp, (const Vector *) &m);
            *(Vector *) (buf + off) = tmp;
            off += 0xc;
            nv = mesh->vertexCount;
        }
        flags = mesh->vertexFormat;
    }
    if (flags & 4) {
        char *buf = new char[(uint32_t) nv * 0xc];
        out->normals = (unsigned char *) buf;
        int off = 0;
        for (uint32_t k = 0; k < nv; k++) {
            Vector rot;
            AEMath_MatrixRotateVector(&rot, (const Vector *) &m);
            *(Vector *) (buf + off) = AbyssEngine::AEMath::VectorNormalize(rot);
            off += 0xc;
            nv = mesh->vertexCount;
        }
    }

    Vector center;
    AEMath_MatrixTransformVector(&center, (const Vector *) &m);
    float bs[4];
    bs[0] = center.x;
    bs[1] = center.y;
    bs[2] = mesh->boundsRadius;
    bs[3] = 1.0f;
    AEMath_BSphere_assign(&out->boundsCenterX, bs);
    return out;
}
