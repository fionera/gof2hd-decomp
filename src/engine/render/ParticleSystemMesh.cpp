#include "gof2/engine/render/ParticleSystemMesh.h"
#include "gof2/externs.h"


extern "C" void _psm_emitTrail(ParticleSystemMesh *self, int id);
extern "C" void _psm_emitUsual(ParticleSystemMesh *self, int id);
extern "C" void _psm_meshSetPoint(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float x, float y, float z);
extern "C" void _psm_mesh_base_dtor(ParticleSystemMesh *self);
extern "C" uint32_t g_ParticleSystemMesh_vtable[];
extern "C" void _psm_render2(PaintCanvas *canvas, uint32_t texture, uint32_t camera);
extern "C" void _psm_vectorMinus(Vector *out, const Vector *a, const Vector *b);
extern "C" void _psm_vectorPlus(void *out, const Vector *a, const Vector *b);
extern "C" char g_ParticleSetData[];
extern "C" void _psm_matrixGetRight(Vector *out, const Matrix *m);
extern "C" void _psm_matrixGetUp(Vector *out, const Matrix *m);
extern "C" void _psm_matrixGetDir(Vector *out, const Matrix *m);
extern "C" void _psm_vectorScale(Vector *out, const Vector *v, float scale);
extern "C" void _psm_vectorAssign(Vector *dst, const Vector *src);
extern "C" void _psm_setQuadEdge(ParticleSystemMesh *self, const Vector *edge, int point, const Vector *delta);
extern "C" void _psm_meshSetColorWord(PaintCanvas *canvas, uint32_t mesh, uint16_t point, uint32_t color);
extern "C" void _psm_meshSetUV2(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float u, float v);
extern "C" void _psm_meshSetUV(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float u, float v);
extern "C" void _psm_meshSetZero(PaintCanvas *canvas, uint32_t mesh, uint16_t point, uint32_t value);
extern "C" void _psm_meshTranslatePoint(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float x, float y, float z);
extern "C" void _psm_interpolateColor(ParticleSystemMesh *self, int id, float *b, float *g, float *a, float *r);
extern "C" void _psm_canvasSetTexture(PaintCanvas *canvas, uint32_t texture, uint32_t fallback);
extern "C" void _psm_canvasSetBlendMode(PaintCanvas *canvas, BlendMode mode);
extern "C" uint32_t _psm_cameraGetCurrent(PaintCanvas *canvas);
extern "C" uint32_t _psm_cameraGetLocal(PaintCanvas *canvas, uint32_t current);
extern "C" void _psm_updateUsualEdges(ParticleSystemMesh *self, int id, int delta);
extern "C" void _psm_updateTrailEdges(ParticleSystemMesh *self, int id, int delta);
extern "C" void _psm_updateSingleColor(ParticleSystemMesh *self, int id);
extern "C" void _psm_meshSetPointIndirect(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float x, float y,
                                          float z);
extern "C" void _psm_meshSetTriangle(PaintCanvas *canvas, uint32_t mesh, uint16_t triangle, uint16_t a, uint16_t b,
                                     uint16_t c);
extern "C" void _psm_meshSetColor(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float a, float r, float g,
                                  float b);
extern "C" void _psm_finishCurrentTrailParticle(ParticleSystemMesh *self, ParticleSet set, int id, const Vector *a,
                                                const Vector *b);
extern "C" void _psm_base_ctor(ParticleSystemMesh *self, PaintCanvas *canvas, const Matrix *matrix, const void *sets,
                               bool a, bool b);

// ---- getPrevId_1855f6.cpp ----
int ParticleSystemMesh::getPrevId(int id)
{
    if (id == 0)
        id = (int)this->particleCount;
    return id - 1;
}

// ---- setParticle_186820.cpp ----
void ParticleSystemMesh::setParticle(const Vector &pos, float scale, uint32_t color, float a, float b, float c,
                                     float d, bool trail, float e, float f, const Vector &dir)
{
    return setParticle(pos, scale, color, a, b, c, d, trail, e, f, dir, false);
}

// ---- emit_18618c.cpp ----
void _ZN18ParticleSystemMesh4emitEi(ParticleSystemMesh *self, int id)
{
    if (self->field_0xc == 0 || self->field_0xd == 0) {
        self->newSectionStarted = 1;
        return;
    }

    uint32_t flags = self->flags;
    if ((flags & 0x80) != 0)
        return;
    if ((int)(flags << 16) < 0)
        return _psm_emitTrail(self, id);
    return _psm_emitUsual(self, id);
}

// ---- finishCurrentTrailParticle_1855fe.cpp ----
void ParticleSystemMesh::finishCurrentTrailParticle(ParticleSet set, int id, const Vector &first, const Vector &second)
{
    *(uint8_t *)((char *)this->setIds + id) = (uint8_t)set;
    *(int *)((char *)this->ages + (uint32_t)id * 4) = 0;

    uint32_t flags = this->flags;
    uint32_t offset = (this->edgeCount * id * 2) | 1;
    Vector *dst = (Vector *)((char *)this->positions + offset * 12);

    if ((flags & 0x1000) != 0) {
        *dst = first;
        flags = this->flags;
        dst = (Vector *)((char *)dst + 0x18);
    }
    if ((int)(flags << 18) < 0)
        *dst = second;
}

// ---- incId_1855e6.cpp ----
void ParticleSystemMesh::incId()
{
    int id = this->currentId + 1;
    if (id >= (int)this->particleCount)
        id = 0;
    this->currentId = id;
}

// ---- reset_18620a.cpp ----
void ParticleSystemMesh::reset()
{
    for (int i = 0; i < (int)this->pointCount; i++) {
        _psm_meshSetPoint((PaintCanvas *)this->canvas, this->mesh,
                          (uint16_t)(this->firstPoint + i), 0.0f, 0.0f, 0.0f);
    }

    for (int i = 0; i < (int)this->particleCount; i++)
        *(int *)((char *)this->ages + (uint32_t)i * 4) = -1;

    this->field_0x94 = 0;
    this->currentId = 0;
    this->newSectionStarted = 1;
    this->field_0x60 = 0;
    this->field_0x4 = 1;
}

// ---- _ParticleSystemMesh_1855c4.cpp ----
void _ZN18ParticleSystemMeshD1Ev(ParticleSystemMesh *self)
{
    self->field_0x0 = g_ParticleSystemMesh_vtable + 2;
    return _psm_mesh_base_dtor(self);
}

// ---- render_1861b2.cpp ----
void ParticleSystemMesh::render(PaintCanvas *canvas, uint32_t texture)
{
    if (texture != 0xffffffff)
        return _psm_render2(canvas, texture, 0);
}

// ---- startNewSection_1855d8.cpp ----
void ParticleSystemMesh::startNewSection()
{
    this->newSectionStarted = 1;
}

// ---- wasNewSectionStarted_1855e0.cpp ----
uint8_t ParticleSystemMesh::wasNewSectionStarted()
{
    return this->newSectionStarted;
}

// ---- getQuadCount_1869ec.cpp ----
int ParticleSystemMesh::getQuadCount()
{
    return (int)this->pointCount >> 2;
}

// ---- setQuadEdge_18608c.cpp ----
void ParticleSystemMesh::setQuadEdge(const Vector &edge, int point, const Vector &delta)
{
    Vector pos;
    volatile char tmpStorage[sizeof(Vector)];

    _psm_vectorMinus(&pos, &edge, &delta);
    _psm_meshSetPoint((PaintCanvas *)this->canvas, this->mesh, (uint16_t)point, pos.x, pos.y, pos.z);

    _psm_vectorPlus((void *)tmpStorage, &edge, &delta);
    pos = *(Vector *)(void *)tmpStorage;

    uint8_t wide = this->wide;
    int next = point + 1;
    PaintCanvas *canvas = (PaintCanvas *)this->canvas;
    uint32_t mesh = this->mesh;
    if (wide == 0) {
        _psm_meshSetPoint(canvas, mesh, (uint16_t)next, pos.x, pos.y, pos.z);
    } else {
        void (*setPoint)(PaintCanvas *, uint32_t, uint16_t, float, float, float) = _psm_meshSetPointIndirect;
        setPoint(canvas, mesh, (uint16_t)next, edge.x, edge.y, edge.z);
        setPoint((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(point + 5), edge.x, edge.y, edge.z);
        setPoint((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(point + 4), pos.x, pos.y, pos.z);
    }
}

// ---- ParticleSystemMesh_185490.cpp ----
ParticleSystemMesh::ParticleSystemMesh(PaintCanvas *canvas, const Matrix *matrix, const void *sets, bool a, bool b)
{
    _psm_base_ctor(this, canvas, matrix, sets, a, b);

    uint32_t flags = this->flags;
    uint32_t particleCount = this->particleCount;
    this->field_0x80 = 0;
    this->field_0x84 = 0;
    this->field_0x88 = 0;

    uint32_t edgeCount = ((flags >> 13) & 1) + ((flags >> 12) & 1);
    if ((flags & 0x4000) != 0)
        edgeCount++;

    this->field_0x0 = g_ParticleSystemMesh_vtable + 2;

    uint32_t wide = (flags >> 16) & 1;
    uint32_t stride = edgeCount << wide;
    this->wide = (uint8_t)wide;
    uint32_t quads = particleCount * stride;
    this->edgeCount = edgeCount;
    this->stride = stride;
    this->pointCount = quads << 2;

    void *positions;
    uint32_t clearSize;
    uint32_t clearCount;
    if ((flags & 0x8000) == 0) {
        uint64_t bytes = (uint64_t)particleCount * 12u;
        uint32_t size = (uint32_t)bytes;
        if ((uint32_t)(bytes >> 32) != 0)
            size = 0xffffffff;
        positions = operator new(size);
        clearCount = particleCount;
        clearSize = particleCount * 12u;
    } else {
        if (*(uint32_t *)((char *)sets + 0) != 0) {
            int set = *(int *)(*(void **)((char *)sets + 4));
            if (set != -1) {
                char *data = g_ParticleSetData + (set * 160);
                if (*(float *)(data + 0x40) > 0.0f)
                    this->pointCount = (quads << 2) + (stride << 2);
            }
        }

        uint32_t vectorCount = particleCount * edgeCount * 2;
        uint64_t bytes = (uint64_t)vectorCount * 12u;
        uint32_t size = (uint32_t)bytes;
        if ((uint32_t)(bytes >> 32) != 0)
            size = 0xffffffff;
        positions = operator new(size);
        clearCount = vectorCount;
        clearSize = particleCount * edgeCount * 24u;
    }

    if (clearCount != 0) {
        uint32_t n = clearSize - 12u;
        uint32_t rem = n % 12u;
        __aeabi_memclr4(positions, (n - rem) + 12u);
    }

    this->positions = positions;
    this->field_0x78 = 0;
    this->field_0x7c = 0;
}

// ---- setParticle_185d00.cpp ----
void ParticleSystemMesh::setParticle(const Vector &pos, float scale, uint32_t color, float u0, float u1, float v0,
                                     float v1, bool useMaskedColor, float upScale, float dirScale,
                                     const Vector &delta, bool finish)
{
    Vector right;
    Vector rightScaled;
    Vector up;
    Vector upScaled;
    Vector dir;
    Vector dirScaled;
    Vector tmpA;
    Vector tmpB;

    _psm_matrixGetRight(&right, (const Matrix *)this->matrix);
    _psm_vectorScale(&rightScaled, &right, scale);
    if (this->field_0x4c != 0) {
        _psm_vectorScale(&tmpA, &rightScaled, -1.0f);
        _psm_vectorAssign(&rightScaled, &tmpA);
    }

    _psm_matrixGetUp(&up, (const Matrix *)this->matrix);
    _psm_vectorScale(&upScaled, &up, upScale == 0.0f ? (float)useMaskedColor : upScale);
    _psm_matrixGetDir(&dir, (const Matrix *)this->matrix);
    _psm_vectorScale(&dirScaled, &dir, dirScale == 0.0f ? scale : dirScale);

    uint32_t flags = this->flags;
    if ((flags & 0x20000) != 0) {
        _psm_vectorMinus(&tmpA, &upScaled, &rightScaled);
        _psm_vectorScale(&dirScaled, &tmpA, 0.70710677f);
        _psm_vectorPlus(&tmpB, &rightScaled, &upScaled);
        _psm_vectorScale(&rightScaled, &tmpB, 0.70710677f);
        flags = this->flags;
    }

    int point = (int)this->firstPoint + (int)this->stride * this->currentId * 4;
    if ((int)(flags << 19) < 0) {
        _psm_vectorMinus(&tmpA, &pos, &upScaled);
        _psm_vectorMinus(&tmpB, &tmpA, &delta);
        _psm_setQuadEdge(this, &tmpB, point, &rightScaled);
        _psm_vectorPlus(&tmpA, &pos, &upScaled);
        _psm_vectorPlus(&tmpB, &tmpA, &delta);
        _psm_setQuadEdge(this, &tmpB, point + 2, &rightScaled);
        point += this->wide == 0 ? 4 : 8;
        flags = this->flags;
    }
    if ((int)(flags << 18) < 0) {
        _psm_vectorMinus(&tmpA, &pos, &upScaled);
        _psm_vectorPlus(&tmpB, &tmpA, &delta);
        _psm_setQuadEdge(this, &tmpB, point, &dirScaled);
        _psm_vectorPlus(&tmpA, &pos, &upScaled);
        _psm_vectorMinus(&tmpB, &tmpA, &delta);
        _psm_setQuadEdge(this, &tmpB, point + 2, &dirScaled);
        point += this->wide == 0 ? 4 : 8;
        flags = this->flags;
    }
    if ((int)(flags << 17) < 0) {
        _psm_vectorPlus(&tmpB, &pos, &rightScaled);
        _psm_setQuadEdge(this, &tmpB, point, &upScaled);
        _psm_vectorMinus(&tmpB, &pos, &rightScaled);
        _psm_setQuadEdge(this, &tmpB, point + 2, &upScaled);
    }

    uint32_t frontColor = color;
    uint32_t backColor = color;
    uint32_t mask = this->colorMask == 0 ? 0xffffff00u : 0xffu;
    if (useMaskedColor && !finish)
        frontColor &= mask;
    if (finish)
        backColor &= mask;

    int base = (int)this->firstPoint + (int)this->stride * this->currentId * 4;
    for (int i = 0; i < (int)this->stride; i++) {
        _psm_meshSetColorWord((PaintCanvas *)this->canvas, this->mesh, (uint16_t)base, backColor);
        _psm_meshSetColorWord((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(base + 1), backColor);
        _psm_meshSetColorWord((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(base + 2), frontColor);
        _psm_meshSetColorWord((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(base + 3), frontColor);
        _psm_meshSetUV2((PaintCanvas *)this->canvas, this->mesh, (uint16_t)base, u0, v0);
        _psm_meshSetUV2((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(base + 1), u1, v0);
        _psm_meshSetUV2((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(base + 2), u0, v1);
        _psm_meshSetUV2((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(base + 3), u1, v1);
        base += 4;
    }
}

// ---- init_186268.cpp ----
int ParticleSystemMesh::init(uint32_t mesh, uint32_t firstPoint)
{
    this->mesh = mesh;
    this->firstPoint = firstPoint;

    void (*setUV)(PaintCanvas *, uint32_t, uint16_t, float, float) = _psm_meshSetUV;
    void (*setZero)(PaintCanvas *, uint32_t, uint16_t, uint32_t) = _psm_meshSetZero;

    for (int i = 0; i < (int)this->pointCount; i += 4) {
        setUV((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(this->firstPoint + i), 0.0f, 0.0f);
        setUV((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(this->firstPoint + i + 1), 1.0f, 0.0f);
        setUV((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(this->firstPoint + i + 2), 0.0f, 1.0f);
        setUV((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(this->firstPoint + i + 3), 1.0f, 1.0f);
        setZero((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(this->firstPoint + i), 0);
        setZero((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(this->firstPoint + i + 1), 0);
        setZero((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(this->firstPoint + i + 2), 0);
        setZero((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(this->firstPoint + i + 3), 0);
    }

    int point = (int)this->firstPoint;
    for (int tri = 0; tri < ((int)this->pointCount >> 1); tri += 2) {
        _psm_meshSetTriangle((PaintCanvas *)this->canvas, this->mesh,
                             (uint16_t)(tri + (this->firstPoint >> 1)), (uint16_t)(point + 2),
                             (uint16_t)(point + 1), (uint16_t)point);
        _psm_meshSetTriangle((PaintCanvas *)this->canvas, this->mesh,
                             (uint16_t)(tri + (this->firstPoint >> 1) + 1), (uint16_t)(point + 1),
                             (uint16_t)(point + 2), (uint16_t)(point + 3));
        point += 4;
    }

    this->field_0x5c = 1;
    return (*(int (**)(ParticleSystemMesh *))(*(uint32_t *)this + 8))(this);
}

// ---- updateUsualEdges_1865dc.cpp ----
void ParticleSystemMesh::updateUsualEdges(int id, int delta)
{
    Vector move;
    Vector tmp;
    float scale = (float)delta * 0.001f;
    const Vector *src;
    if ((int)(this->field_0x36 << 28) < 0) {
        const Vector *trail = (const Vector *)((char *)this->positions + id * 12);
        src = (const Vector *)((char *)this + 0x1c);
        scale *= trail->y;
    } else {
        src = (const Vector *)((char *)this->positions + id * 12);
    }

    _psm_vectorScale(&tmp, src, scale);
    move = tmp;

    int point = (int)this->firstPoint + (int)this->stride * id * 4;
    for (int i = 0; i < (int)this->stride * 4; i++)
        _psm_meshTranslatePoint((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(point + i), move.x,
                                move.y, move.z);
}

// ---- updateSingleColor_1863a0.cpp ----
void ParticleSystemMesh::updateSingleColor(int id)
{
    float b;
    float g;
    float r;
    float a;

    int start = (int)this->firstPoint;
    int stride = (int)this->stride;
    if ((int)(this->field_0x35 << 24) < 0) {
        int prev = id == 0 ? (int)this->particleCount : id;
        if (*(int *)((char *)this->ages + (uint32_t)(prev - 1) * 4) == -1) {
            int set = *(signed char *)((char *)this->setIds + id);
            uint32_t color = *(uint32_t *)(g_ParticleSetData + set * 160 + 0x38);
            uint32_t mask = this->colorMask == 0 ? 0xffffff00u : 0xffu;
            color &= mask;
            r = (float)((color >> 16) & 0xff) * 0.0039215689f;
            a = (float)(color >> 24) * 0.0039215689f;
            g = (float)((color >> 8) & 0xff) * 0.0039215689f;
            b = (float)(color & 0xff) * 0.0039215689f;
        } else {
            _psm_interpolateColor(this, id, &b, &g, &a, &r);
        }
    } else {
        _psm_interpolateColor(this, id, &b, &g, &a, &r);
    }

    int point = start + stride * id * 4;
    for (int i = 0; i < (int)this->stride; i++) {
        _psm_meshSetColor((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(point + 2), a, r, g, b);
        _psm_meshSetColor((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(point + 3), a, r, g, b);
        point += 4;
    }

    if ((int)(this->field_0x35 << 24) < 0) {
        int next = (id == (int)this->particleCount - 1) ? 0 : id + 1;
        if (*(int *)((char *)this->ages + (uint32_t)next * 4) == -1)
            return;
        point = (int)this->firstPoint + (int)this->stride * next * 4;
        for (int i = 0; i < (int)this->stride; i++) {
            _psm_meshSetColor((PaintCanvas *)this->canvas, this->mesh, (uint16_t)point, a, r, g, b);
            _psm_meshSetColor((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(point + 1), a, r, g, b);
            point += 4;
        }
    } else {
        point = start + stride * id * 4;
        for (int i = 0; i < (int)this->stride; i++) {
            _psm_meshSetColor((PaintCanvas *)this->canvas, this->mesh, (uint16_t)point, a, r, g, b);
            _psm_meshSetColor((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(point + 1), a, r, g, b);
            point += 4;
        }
    }
}

// ---- render_1861be.cpp ----
void ParticleSystemMesh::render(PaintCanvas *canvas, uint32_t mesh, uint32_t texture, BlendMode blend)
{
    if (mesh == 0xffffffff)
        return;
    _psm_canvasSetTexture(canvas, texture, 0xffffffff);
    _psm_canvasSetBlendMode(canvas, blend);
    uint32_t current = _psm_cameraGetCurrent(canvas);
    uint32_t local = _psm_cameraGetLocal(canvas, current);
    return _psm_render2(canvas, mesh, local);
}

// ---- emitTrail_18564c.cpp ----
void ParticleSystemMesh::emitTrail(int)
{
}

// ---- updateSingle_186870.cpp ----
void ParticleSystemMesh::updateSingle(int id, float delta)
{
    int intDelta = (int)delta;
    int set = *(signed char *)((char *)this->setIds + id);
    if ((int)(this->field_0x35 << 24) < 0) {
        _psm_updateTrailEdges(this, id, intDelta);
        if (*(int *)((char *)this->ages + (uint32_t)id * 4) == -2 && this->newSectionStarted != 0) {
            Vector right;
            Vector scaledRight;
            Vector up;
            Vector scaledUp;
            _psm_matrixGetRight(&right, (const Matrix *)this->matrix);
            _psm_vectorScale(&scaledRight, &right, this->field_0x4c == 0 ? 1.0f : -1.0f);
            _psm_matrixGetUp(&up, (const Matrix *)this->matrix);
            float s = (float)*(int32_t *)(g_ParticleSetData + set * 160 + 0x44);
            _psm_vectorScale(&scaledRight, &scaledRight, s);
            _psm_vectorScale(&scaledUp, &up, s);
            _psm_finishCurrentTrailParticle(this, (ParticleSet)set, id, &scaledRight, &scaledUp);
        }
    } else {
        _psm_updateUsualEdges(this, id, intDelta);
    }

    int age = *(int *)((char *)this->ages + (uint32_t)id * 4);
    age = (int)((float)age + delta);
    *(int *)((char *)this->ages + (uint32_t)id * 4) = age;
    _psm_updateSingleColor(this, id);

    int lifetime = *(int32_t *)(g_ParticleSetData + set * 160 + 0x28);
    if (age > lifetime) {
        *(int *)((char *)this->ages + (uint32_t)id * 4) = -1;
        int point = (int)this->firstPoint + (int)this->stride * id * 4;
        for (int i = 0; i < (int)this->stride * 4; i++)
            _psm_meshSetPoint((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(point + i), 0.0f, 0.0f,
                              0.0f);
    }
}

// ---- updateTrailEdges_1866b4.cpp ----
void ParticleSystemMesh::updateTrailEdges(int id, int delta)
{
    int edgeCount = (int)this->edgeCount;
    int stride = (int)this->stride;
    int point = (int)this->firstPoint + id * stride * 4;
    Vector *edge = (Vector *)((char *)this->positions + id * edgeCount * 24);
    float scale = (float)delta * 0.001f;

    for (int i = 0; i < edgeCount; i++) {
        Vector move;
        _psm_vectorScale(&move, edge, scale);
        _psm_meshTranslatePoint((PaintCanvas *)this->canvas, this->mesh, (uint16_t)point, -move.x, move.y,
                                -move.z);
        int span = this->wide == 0 ? 1 : 4;
        _psm_meshTranslatePoint((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(point + span), move.x,
                                move.y, move.z);

        if (*(int *)((char *)this->ages + (uint32_t)id * 4) != -2 || (int)(this->field_0x35 << 24) >= 0) {
            Vector move2;
            _psm_vectorScale(&move2, edge + 1, scale);
            _psm_meshTranslatePoint((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(point + 2),
                                    -move2.x, move2.y, -move2.z);
            _psm_meshTranslatePoint((PaintCanvas *)this->canvas, this->mesh, (uint16_t)(point + span + 2),
                                    move2.x, move2.y, move2.z);
            edge += 2;
            point += this->wide == 0 ? 4 : 8;
        } else {
            edge += 1;
        }
    }
}
