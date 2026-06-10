#include "ParticleSystemMesh.h"


extern "C" void _psm_emitTrail(ParticleSystemMesh *self, int id);
extern "C" void _psm_emitUsual(ParticleSystemMesh *self, int id);
extern "C" void _psm_meshSetPoint(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float x, float y, float z);
extern "C" void _psm_mesh_base_dtor(ParticleSystemMesh *self);
extern "C" uint32_t g_ParticleSystemMesh_vtable[];
extern "C" void _psm_render2(PaintCanvas *canvas, uint32_t texture, uint32_t camera);
extern "C" void _psm_vectorMinus(Vector *out, const Vector *a, const Vector *b);
extern "C" void _psm_vectorPlus(void *out, const Vector *a, const Vector *b);
extern "C" char g_ParticleSetData[];
extern "C" void __aeabi_memclr4(void *dst, uint32_t size);
extern "C" void _psm_matrixGetRight(Vector *out, const Matrix *m);
extern "C" void _psm_matrixGetUp(Vector *out, const Matrix *m);
extern "C" void _psm_matrixGetDir(Vector *out, const Matrix *m);
extern "C" void _psm_vectorScale(Vector *out, const Vector *v, float scale);
extern "C" void _psm_vectorPlus(Vector *out, const Vector *a, const Vector *b);
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

// ---- getPrevId_1855f6.cpp ----
struct ParticleSystemMesh {
    int getPrevId(int id);
};

int ParticleSystemMesh::getPrevId(int id)
{
    if (id == 0)
        id = i32(this, 0x48);
    return id - 1;
}

// ---- setParticle_186820.cpp ----
struct ParticleSystemMesh {
    void setParticle(const Vector &pos, float scale, uint32_t color, float a, float b, float c, float d,
                     bool trail, float e, float f, const Vector &dir);
    void setParticle(const Vector &pos, float scale, uint32_t color, float a, float b, float c, float d,
                     bool trail, float e, float f, const Vector &dir, bool finished);
};

void ParticleSystemMesh::setParticle(const Vector &pos, float scale, uint32_t color, float a, float b, float c,
                                     float d, bool trail, float e, float f, const Vector &dir)
{
    return setParticle(pos, scale, color, a, b, c, d, trail, e, f, dir, false);
}

// ---- emit_18618c.cpp ----
extern "C" void _ZN18ParticleSystemMesh4emitEi(ParticleSystemMesh *self, int id)
{
    if (u8(self, 0xc) == 0 || u8(self, 0xd) == 0) {
        u8(self, 0x90) = 1;
        return;
    }

    uint32_t flags = u32(self, 0x34);
    if ((flags & 0x80) != 0)
        return;
    if ((int)(flags << 16) < 0)
        return _psm_emitTrail(self, id);
    return _psm_emitUsual(self, id);
}

// ---- finishCurrentTrailParticle_1855fe.cpp ----
struct ParticleSystemMesh {
    void finishCurrentTrailParticle(ParticleSet set, int id, const Vector &first, const Vector &second);
};

void ParticleSystemMesh::finishCurrentTrailParticle(ParticleSet set, int id, const Vector &first, const Vector &second)
{
    u8(ptr(this, 0x6c), id) = (uint8_t)set;
    i32(ptr(this, 0x68), (uint32_t)id * 4) = 0;

    uint32_t flags = u32(this, 0x34);
    uint32_t offset = (u32(this, 0x98) * id * 2) | 1;
    Vector *dst = (Vector *)((char *)ptr(this, 0x64) + offset * 12);

    if ((flags & 0x1000) != 0) {
        *dst = first;
        flags = u32(this, 0x34);
        dst = (Vector *)((char *)dst + 0x18);
    }
    if ((int)(flags << 18) < 0)
        *dst = second;
}

// ---- incId_1855e6.cpp ----
struct ParticleSystemMesh {
    void incId();
};

void ParticleSystemMesh::incId()
{
    int id = i32(this, 0x50) + 1;
    if (id >= i32(this, 0x48))
        id = 0;
    i32(this, 0x50) = id;
}

// ---- reset_18620a.cpp ----
struct ParticleSystemMesh {
    void reset();
};

void ParticleSystemMesh::reset()
{
    for (int i = 0; i < i32(this, 0x70); i++) {
        _psm_meshSetPoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54),
                          (uint16_t)(u32(this, 0x58) + i), 0.0f, 0.0f, 0.0f);
    }

    for (int i = 0; i < i32(this, 0x48); i++)
        i32(ptr(this, 0x68), (uint32_t)i * 4) = -1;

    *(volatile uint32_t *)((char *)this + 0x94) = 0;
    *(volatile uint32_t *)((char *)this + 0x50) = 0;
    *(volatile uint8_t *)((char *)this + 0x90) = 1;
    *(volatile uint32_t *)((char *)this + 0x60) = 0;
    *(volatile uint8_t *)((char *)this + 0x4) = 1;
}

// ---- _ParticleSystemMesh_1855c4.cpp ----
extern "C" void _ZN18ParticleSystemMeshD1Ev(ParticleSystemMesh *self)
{
    *(uint32_t **)self = g_ParticleSystemMesh_vtable + 2;
    return _psm_mesh_base_dtor(self);
}

// ---- render_1861b2.cpp ----
struct ParticleSystemMesh {
    static void render(PaintCanvas *canvas, uint32_t texture);
};

void ParticleSystemMesh::render(PaintCanvas *canvas, uint32_t texture)
{
    if (texture != 0xffffffff)
        return _psm_render2(canvas, texture, 0);
}

// ---- startNewSection_1855d8.cpp ----
struct ParticleSystemMesh {
    void startNewSection();
};

void ParticleSystemMesh::startNewSection()
{
    u8(this, 0x90) = 1;
}

// ---- wasNewSectionStarted_1855e0.cpp ----
struct ParticleSystemMesh {
    uint8_t wasNewSectionStarted();
};

uint8_t ParticleSystemMesh::wasNewSectionStarted()
{
    return u8(this, 0x90);
}

// ---- getQuadCount_1869ec.cpp ----
struct ParticleSystemMesh {
    int getQuadCount();
};

int ParticleSystemMesh::getQuadCount()
{
    return i32(this, 0x70) >> 2;
}

// ---- setQuadEdge_18608c.cpp ----
extern "C" void _psm_meshSetPointIndirect(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float x, float y,
                                          float z);

struct ParticleSystemMesh {
    void setQuadEdge(const Vector &edge, int point, const Vector &delta);
};

void ParticleSystemMesh::setQuadEdge(const Vector &edge, int point, const Vector &delta)
{
    Vector pos;
    volatile char tmpStorage[sizeof(Vector)];

    _psm_vectorMinus(&pos, &edge, &delta);
    _psm_meshSetPoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)point, pos.x, pos.y, pos.z);

    _psm_vectorPlus((void *)tmpStorage, &edge, &delta);
    pos = *(Vector *)(void *)tmpStorage;

    uint8_t wide = u8(this, 0x74);
    int next = point + 1;
    PaintCanvas *canvas = (PaintCanvas *)ptr(this, 0x8);
    uint32_t mesh = u32(this, 0x54);
    if (wide == 0) {
        _psm_meshSetPoint(canvas, mesh, (uint16_t)next, pos.x, pos.y, pos.z);
    } else {
        void (*setPoint)(PaintCanvas *, uint32_t, uint16_t, float, float, float) = _psm_meshSetPointIndirect;
        setPoint(canvas, mesh, (uint16_t)next, edge.x, edge.y, edge.z);
        setPoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 5), edge.x, edge.y, edge.z);
        setPoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 4), pos.x, pos.y, pos.z);
    }
}

// ---- ParticleSystemMesh_185490.cpp ----
extern "C" void _psm_base_ctor(ParticleSystemMesh *self, PaintCanvas *canvas, const Matrix *matrix, const void *sets,
                               bool a, bool b);

struct ParticleSystemMesh {
    ParticleSystemMesh(PaintCanvas *canvas, const Matrix *matrix, const void *sets, bool a, bool b);
};

ParticleSystemMesh::ParticleSystemMesh(PaintCanvas *canvas, const Matrix *matrix, const void *sets, bool a, bool b)
{
    _psm_base_ctor(this, canvas, matrix, sets, a, b);

    uint32_t flags = u32(this, 0x34);
    uint32_t particleCount = u32(this, 0x48);
    u32(this, 0x80) = 0;
    u32(this, 0x84) = 0;
    u32(this, 0x88) = 0;

    uint32_t edgeCount = ((flags >> 13) & 1) + ((flags >> 12) & 1);
    if ((flags & 0x4000) != 0)
        edgeCount++;

    *(uint32_t **)this = g_ParticleSystemMesh_vtable + 2;

    uint32_t wide = (flags >> 16) & 1;
    uint32_t stride = edgeCount << wide;
    u8(this, 0x74) = (uint8_t)wide;
    uint32_t quads = particleCount * stride;
    u32(this, 0x98) = edgeCount;
    u32(this, 0x9c) = stride;
    u32(this, 0x70) = quads << 2;

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
        if (u32((void *)sets, 0) != 0) {
            int set = *(int *)ptr((void *)sets, 4);
            if (set != -1) {
                char *data = g_ParticleSetData + (set * 160);
                if (*(float *)(data + 0x40) > 0.0f)
                    u32(this, 0x70) = (quads << 2) + (stride << 2);
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

    ptr(this, 0x64) = positions;
    u32(this, 0x78) = 0;
    u32(this, 0x7c) = 0;
}

// ---- setParticle_185d00.cpp ----
struct ParticleSystemMesh {
    void setParticle(const Vector &pos, float scale, uint32_t color, float u0, float u1, float v0, float v1,
                     bool useMaskedColor, float upScale, float dirScale, const Vector &delta, bool finish);
};

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

    _psm_matrixGetRight(&right, (const Matrix *)ptr(this, 0x18));
    _psm_vectorScale(&rightScaled, &right, scale);
    if (u8(this, 0x4c) != 0) {
        _psm_vectorScale(&tmpA, &rightScaled, -1.0f);
        _psm_vectorAssign(&rightScaled, &tmpA);
    }

    _psm_matrixGetUp(&up, (const Matrix *)ptr(this, 0x18));
    _psm_vectorScale(&upScaled, &up, upScale == 0.0f ? (float)useMaskedColor : upScale);
    _psm_matrixGetDir(&dir, (const Matrix *)ptr(this, 0x18));
    _psm_vectorScale(&dirScaled, &dir, dirScale == 0.0f ? scale : dirScale);

    uint32_t flags = u32(this, 0x34);
    if ((flags & 0x20000) != 0) {
        _psm_vectorMinus(&tmpA, &upScaled, &rightScaled);
        _psm_vectorScale(&dirScaled, &tmpA, 0.70710677f);
        _psm_vectorPlus(&tmpB, &rightScaled, &upScaled);
        _psm_vectorScale(&rightScaled, &tmpB, 0.70710677f);
        flags = u32(this, 0x34);
    }

    int point = i32(this, 0x58) + i32(this, 0x9c) * i32(this, 0x50) * 4;
    if ((int)(flags << 19) < 0) {
        _psm_vectorMinus(&tmpA, &pos, &upScaled);
        _psm_vectorMinus(&tmpB, &tmpA, &delta);
        _psm_setQuadEdge(this, &tmpB, point, &rightScaled);
        _psm_vectorPlus(&tmpA, &pos, &upScaled);
        _psm_vectorPlus(&tmpB, &tmpA, &delta);
        _psm_setQuadEdge(this, &tmpB, point + 2, &rightScaled);
        point += u8(this, 0x74) == 0 ? 4 : 8;
        flags = u32(this, 0x34);
    }
    if ((int)(flags << 18) < 0) {
        _psm_vectorMinus(&tmpA, &pos, &upScaled);
        _psm_vectorPlus(&tmpB, &tmpA, &delta);
        _psm_setQuadEdge(this, &tmpB, point, &dirScaled);
        _psm_vectorPlus(&tmpA, &pos, &upScaled);
        _psm_vectorMinus(&tmpB, &tmpA, &delta);
        _psm_setQuadEdge(this, &tmpB, point + 2, &dirScaled);
        point += u8(this, 0x74) == 0 ? 4 : 8;
        flags = u32(this, 0x34);
    }
    if ((int)(flags << 17) < 0) {
        _psm_vectorPlus(&tmpB, &pos, &rightScaled);
        _psm_setQuadEdge(this, &tmpB, point, &upScaled);
        _psm_vectorMinus(&tmpB, &pos, &rightScaled);
        _psm_setQuadEdge(this, &tmpB, point + 2, &upScaled);
    }

    uint32_t frontColor = color;
    uint32_t backColor = color;
    uint32_t mask = u8(this, 0x45) == 0 ? 0xffffff00u : 0xffu;
    if (useMaskedColor && !finish)
        frontColor &= mask;
    if (finish)
        backColor &= mask;

    int base = i32(this, 0x58) + i32(this, 0x9c) * i32(this, 0x50) * 4;
    for (int i = 0; i < i32(this, 0x9c); i++) {
        _psm_meshSetColorWord((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)base, backColor);
        _psm_meshSetColorWord((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(base + 1), backColor);
        _psm_meshSetColorWord((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(base + 2), frontColor);
        _psm_meshSetColorWord((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(base + 3), frontColor);
        _psm_meshSetUV2((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)base, u0, v0);
        _psm_meshSetUV2((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(base + 1), u1, v0);
        _psm_meshSetUV2((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(base + 2), u0, v1);
        _psm_meshSetUV2((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(base + 3), u1, v1);
        base += 4;
    }
}

// ---- init_186268.cpp ----
extern "C" void _psm_meshSetTriangle(PaintCanvas *canvas, uint32_t mesh, uint16_t triangle, uint16_t a, uint16_t b,
                                     uint16_t c);

struct ParticleSystemMesh {
    int init(uint32_t mesh, uint32_t firstPoint);
};

int ParticleSystemMesh::init(uint32_t mesh, uint32_t firstPoint)
{
    u32(this, 0x54) = mesh;
    u32(this, 0x58) = firstPoint;

    void (*setUV)(PaintCanvas *, uint32_t, uint16_t, float, float) = _psm_meshSetUV;
    void (*setZero)(PaintCanvas *, uint32_t, uint16_t, uint32_t) = _psm_meshSetZero;

    for (int i = 0; i < i32(this, 0x70); i += 4) {
        setUV((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i), 0.0f, 0.0f);
        setUV((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i + 1), 1.0f, 0.0f);
        setUV((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i + 2), 0.0f, 1.0f);
        setUV((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i + 3), 1.0f, 1.0f);
        setZero((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i), 0);
        setZero((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i + 1), 0);
        setZero((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i + 2), 0);
        setZero((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i + 3), 0);
    }

    int point = i32(this, 0x58);
    for (int tri = 0; tri < (i32(this, 0x70) >> 1); tri += 2) {
        _psm_meshSetTriangle((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54),
                             (uint16_t)(tri + (u32(this, 0x58) >> 1)), (uint16_t)(point + 2),
                             (uint16_t)(point + 1), (uint16_t)point);
        _psm_meshSetTriangle((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54),
                             (uint16_t)(tri + (u32(this, 0x58) >> 1) + 1), (uint16_t)(point + 1),
                             (uint16_t)(point + 2), (uint16_t)(point + 3));
        point += 4;
    }

    u8(this, 0x5c) = 1;
    return (*(int (**)(ParticleSystemMesh *))(*(uint32_t *)this + 8))(this);
}

// ---- updateUsualEdges_1865dc.cpp ----
struct ParticleSystemMesh {
    void updateUsualEdges(int id, int delta);
};

void ParticleSystemMesh::updateUsualEdges(int id, int delta)
{
    Vector move;
    Vector tmp;
    float scale = (float)delta * 0.001f;
    const Vector *src;
    if ((int)(u8(this, 0x36) << 28) < 0) {
        const Vector *trail = (const Vector *)((char *)ptr(this, 0x64) + id * 12);
        src = (const Vector *)((char *)this + 0x1c);
        scale *= trail->y;
    } else {
        src = (const Vector *)((char *)ptr(this, 0x64) + id * 12);
    }

    _psm_vectorScale(&tmp, src, scale);
    move = tmp;

    int point = i32(this, 0x58) + i32(this, 0x9c) * id * 4;
    for (int i = 0; i < i32(this, 0x9c) * 4; i++)
        _psm_meshTranslatePoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + i), move.x,
                                move.y, move.z);
}

// ---- updateSingleColor_1863a0.cpp ----
extern "C" void _psm_meshSetColor(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float a, float r, float g,
                                  float b);

struct ParticleSystemMesh {
    void updateSingleColor(int id);
};

void ParticleSystemMesh::updateSingleColor(int id)
{
    float b;
    float g;
    float r;
    float a;

    int start = i32(this, 0x58);
    int stride = i32(this, 0x9c);
    if ((int)(u8(this, 0x35) << 24) < 0) {
        int prev = id == 0 ? i32(this, 0x48) : id;
        if (i32(ptr(this, 0x68), (uint32_t)(prev - 1) * 4) == -1) {
            int set = *(signed char *)((char *)ptr(this, 0x6c) + id);
            uint32_t color = *(uint32_t *)(g_ParticleSetData + set * 160 + 0x38);
            uint32_t mask = u8(this, 0x45) == 0 ? 0xffffff00u : 0xffu;
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
    for (int i = 0; i < i32(this, 0x9c); i++) {
        _psm_meshSetColor((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 2), a, r, g, b);
        _psm_meshSetColor((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 3), a, r, g, b);
        point += 4;
    }

    if ((int)(u8(this, 0x35) << 24) < 0) {
        int next = (id == i32(this, 0x48) - 1) ? 0 : id + 1;
        if (i32(ptr(this, 0x68), (uint32_t)next * 4) == -1)
            return;
        point = i32(this, 0x58) + i32(this, 0x9c) * next * 4;
        for (int i = 0; i < i32(this, 0x9c); i++) {
            _psm_meshSetColor((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)point, a, r, g, b);
            _psm_meshSetColor((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 1), a, r, g, b);
            point += 4;
        }
    } else {
        point = start + stride * id * 4;
        for (int i = 0; i < i32(this, 0x9c); i++) {
            _psm_meshSetColor((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)point, a, r, g, b);
            _psm_meshSetColor((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 1), a, r, g, b);
            point += 4;
        }
    }
}

// ---- render_1861be.cpp ----
struct ParticleSystemMesh {
    static void render(PaintCanvas *canvas, uint32_t mesh, uint32_t texture, BlendMode blend);
};

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
struct ParticleSystemMesh {
    static void emitTrail(int self);
};

void ParticleSystemMesh::emitTrail(int)
{
}

// ---- updateSingle_186870.cpp ----
extern "C" void _psm_finishCurrentTrailParticle(ParticleSystemMesh *self, ParticleSet set, int id, const Vector *a,
                                                const Vector *b);

struct ParticleSystemMesh {
    void updateSingle(int id, float delta);
};

void ParticleSystemMesh::updateSingle(int id, float delta)
{
    int intDelta = (int)delta;
    int set = *(signed char *)((char *)ptr(this, 0x6c) + id);
    if ((int)(u8(this, 0x35) << 24) < 0) {
        _psm_updateTrailEdges(this, id, intDelta);
        if (i32(ptr(this, 0x68), (uint32_t)id * 4) == -2 && u8(this, 0x90) != 0) {
            Vector right;
            Vector scaledRight;
            Vector up;
            Vector scaledUp;
            _psm_matrixGetRight(&right, (const Matrix *)ptr(this, 0x18));
            _psm_vectorScale(&scaledRight, &right, u8(this, 0x4c) == 0 ? 1.0f : -1.0f);
            _psm_matrixGetUp(&up, (const Matrix *)ptr(this, 0x18));
            float s = (float)*(int32_t *)(g_ParticleSetData + set * 160 + 0x44);
            _psm_vectorScale(&scaledRight, &scaledRight, s);
            _psm_vectorScale(&scaledUp, &up, s);
            _psm_finishCurrentTrailParticle(this, (ParticleSet)set, id, &scaledRight, &scaledUp);
        }
    } else {
        _psm_updateUsualEdges(this, id, intDelta);
    }

    int age = i32(ptr(this, 0x68), (uint32_t)id * 4);
    age = (int)((float)age + delta);
    i32(ptr(this, 0x68), (uint32_t)id * 4) = age;
    _psm_updateSingleColor(this, id);

    int lifetime = *(int32_t *)(g_ParticleSetData + set * 160 + 0x28);
    if (age > lifetime) {
        i32(ptr(this, 0x68), (uint32_t)id * 4) = -1;
        int point = i32(this, 0x58) + i32(this, 0x9c) * id * 4;
        for (int i = 0; i < i32(this, 0x9c) * 4; i++)
            _psm_meshSetPoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + i), 0.0f, 0.0f,
                              0.0f);
    }
}

// ---- updateTrailEdges_1866b4.cpp ----
struct ParticleSystemMesh {
    void updateTrailEdges(int id, int delta);
};

void ParticleSystemMesh::updateTrailEdges(int id, int delta)
{
    int edgeCount = i32(this, 0x98);
    int stride = i32(this, 0x9c);
    int point = i32(this, 0x58) + id * stride * 4;
    Vector *edge = (Vector *)((char *)ptr(this, 0x64) + id * edgeCount * 24);
    float scale = (float)delta * 0.001f;

    for (int i = 0; i < edgeCount; i++) {
        Vector move;
        _psm_vectorScale(&move, edge, scale);
        _psm_meshTranslatePoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)point, -move.x, move.y,
                                -move.z);
        int span = u8(this, 0x74) == 0 ? 1 : 4;
        _psm_meshTranslatePoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + span), move.x,
                                move.y, move.z);

        if (i32(ptr(this, 0x68), (uint32_t)id * 4) != -2 || (int)(u8(this, 0x35) << 24) >= 0) {
            Vector move2;
            _psm_vectorScale(&move2, edge + 1, scale);
            _psm_meshTranslatePoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 2),
                                    -move2.x, move2.y, -move2.z);
            _psm_meshTranslatePoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + span + 2),
                                    move2.x, move2.y, move2.z);
            edge += 2;
            point += u8(this, 0x74) == 0 ? 4 : 8;
        } else {
            edge += 1;
        }
    }
}
