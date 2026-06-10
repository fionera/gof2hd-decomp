#include "Sparks.h"


extern "C" void *operator_new(uint32_t size);
extern "C" int AERandom_nextInt(void *rng, int bound);
extern "C" void operator_delete(void *ptr);
extern "C" void Sparks_explode_int(Sparks *self, int x, int y, int z);
extern "C" void PaintCanvas_SetTexture(void *canvas, uint32_t texture, uint32_t mode);
extern "C" void PaintCanvas_SetBlendMode(void *canvas, int mode);
extern "C" void PaintCanvas_SetWorldViewMatrix(void *canvas, Matrix const &m);
extern "C" void PaintCanvas_DrawSpriteSystem(void *canvas, uint32_t sprite);

// ---- Sparks_15e38c.cpp ----
struct Sparks {
    Sparks(int kind);
};

extern "C" void PaintCanvas_SpriteSystemCreate(void *canvas, uint16_t count, bool dynamic,
                                                uint32_t *outSprite);
extern "C" void PaintCanvas_SpriteSystemSetAllUv(void *canvas, uint32_t sprite, float u0,
                                                  float v0, float u1, float v1);
extern "C" void PaintCanvas_SpriteSystemSetSize(void *canvas, uint32_t sprite, uint16_t index,
                                                 int16_t size);

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_ctor;
__attribute__((visibility("hidden"))) extern void **g_Sparks_random_ctor;

Sparks::Sparks(int kind)
{
    uint32_t count = 5;
    if (kind == 0)
        count = 1;
    uint32_t *outSprite = &F<uint32_t>(this, 0x4);
    void **canvas = g_Sparks_canvas_ctor;
    uint32_t lifetime = 0x514;
    if (kind == 0)
        lifetime = 0x1f4;

    F<int>(this, 0x14) = kind;
    F<uint32_t>(this, 0x18) = count;
    F<uint32_t>(this, 0x1c) = lifetime;

    PaintCanvas_SpriteSystemCreate(*canvas, (uint16_t)count, false, outSprite);
    PaintCanvas_SpriteSystemSetAllUv(*canvas, F<uint32_t>(this, 0x4), 0.626953125f,
                                     0.001953125f, 0.748046875f, 0.123046875f);

    uint32_t n = F<uint32_t>(this, 0x18);
    F<uint8_t>(this, 0x10) = 0;
    F<int *>(this, 0x0) = new int[n];
    F<int>(this, 0x20) = 0;

    if (F<int>(this, 0x14) == 1) {
        void **rng = g_Sparks_random_ctor;
        for (uint32_t i = 0; i < n; i++) {
            PaintCanvas_SpriteSystemSetSize(*canvas, F<uint32_t>(this, 0x4), (uint16_t)i, 1);
            int value = AERandom_nextInt(*rng, 0x1f4);
            F<int *>(this, 0x0)[i] = value;
            n = F<uint32_t>(this, 0x18);
            F<int>(this, 0x20) = value + F<int>(this, 0x20);
        }
    } else {
        F<int *>(this, 0x0)[0] = 0;
        F<int>(this, 0x20) = 0;
    }

    F<int>(this, 0x0c) = 0;
}

// ---- translate_15e6d4.cpp ----
using AbyssEngine::AEMath::Vector;

struct Sparks {
    void translate(Vector const &v);
};

extern "C" void PaintCanvas_SpriteSystemAddPosition(void *canvas, uint32_t sprite, uint16_t index,
                                                     float x, float y, float z);

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_translate;

void Sparks::translate(Vector const &v)
{
    void **canvas = g_Sparks_canvas_translate;
    for (uint32_t i = 0; i < F<uint32_t>(this, 0x18); i++)
        PaintCanvas_SpriteSystemAddPosition(*canvas, F<uint32_t>(this, 0x4), (uint16_t)i,
                                            v.x, v.y, v.z);
}

// ---- _Sparks_15e474.cpp ----
struct Sparks {
    ~Sparks();
};


Sparks::~Sparks()
{
    void *p = F<void *>(this, 0x0);
    if (p != 0)
        operator_delete(p);
    F<void *>(this, 0x0) = 0;
}

// ---- explode_15e594.cpp ----
using AbyssEngine::AEMath::Vector;

struct Sparks {
    void explode(Vector const &v);
};


void Sparks::explode(Vector const &v)
{
    return Sparks_explode_int(this, (int)v.x, (int)v.y, (int)v.z);
}

// ---- isRocket_15e468.cpp ----
struct Sparks {
    bool isRocket();
};

bool Sparks::isRocket()
{
    return F<int>(this, 0x14) == 1;
}

// ---- update_15e660.cpp ----
struct Sparks {
    void update(int step);
};

extern "C" void PaintCanvas_SpriteSystemSetSize(void *canvas, uint32_t sprite, uint16_t index,
                                                 int16_t size);

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_update;

void Sparks::update(int step)
{
    if (F<uint8_t>(this, 0x10) == 0)
        return;

    int elapsed0 = F<int>(this, 0x0c);
    uint32_t i = 0;
    elapsed0 += step;
    F<int>(this, 0x0c) = elapsed0;
    for (; i < F<uint32_t>(this, 0x18); i++) {
        int *thresholds = F<int *>(this, 0x0);
        int elapsed = F<int>(this, 0x0c);
        if (elapsed <= thresholds[i])
            continue;
        int size = F<int>(this, 0x1c) - (elapsed << 1);
        PaintCanvas_SpriteSystemSetSize(*g_Sparks_canvas_update, F<uint32_t>(this, 0x4), (uint16_t)i,
                                        (int16_t)size);
    }

    int elapsed = F<int>(this, 0x0c);
    int kind = F<int>(this, 0x14);
    if (kind == 1) {
        if (elapsed <= 0x1f4) {
            int limit = F<int>(this, 0x1c);
            elapsed += elapsed;
            if (elapsed <= limit)
                return;
        }
    } else {
        if (elapsed <= 0x1f4)
            return;
    }

    F<volatile uint8_t>(this, 0x10) = 0;
    F<volatile int>(this, 0x0c) = 0;
}

// ---- explode_15e48c.cpp ----
struct Sparks {
    void explode(int x, int y, int z);
};

extern "C" void PaintCanvas_SpriteSystemSetPosition(void *canvas, uint32_t sprite,
                                                     uint16_t index, float x, float y, float z);

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_explode_rocket;
__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_explode_single;
__attribute__((visibility("hidden"))) extern void **g_Sparks_random_explode;
__attribute__((visibility("hidden"))) extern int (*g_Sparks_nextInt_explode)(void *rng, int bound);

void Sparks::explode(int x, int y, int z)
{
    int x0 = x;
    int y0 = y;
    int z0 = z;

    if (F<uint8_t>(this, 0x10) != 0)
        return;

    if (F<int>(this, 0x14) == 1) {
        uint32_t i = 0;
        void **canvas = g_Sparks_canvas_explode_rocket;
        void **rng = g_Sparks_random_explode;
        int (*nextInt)(void *, int) = g_Sparks_nextInt_explode;
        for (; i < F<uint32_t>(this, 0x18); i++) {
            uint32_t sprite = F<uint32_t>(this, 0x4);
            void *canvasObj = *canvas;
            float px = (float)(nextInt(*rng, 0x190) + x0);
            float py = (float)(nextInt(*rng, 0x190) + y0);
            float pz = (float)(nextInt(*rng, 0x190) + z0);
            PaintCanvas_SpriteSystemSetPosition(canvasObj, sprite, (uint16_t)i,
                                                px, py, pz);
        }
    } else {
        void **canvas = g_Sparks_canvas_explode_single;
        PaintCanvas_SpriteSystemSetPosition(*canvas, F<uint32_t>(this, 0x4), 0,
                                            (float)x0, (float)y0, (float)z0);
    }

    F<uint8_t>(this, 0x10) = 1;
}

// ---- render_15e5c0.cpp ----
using AbyssEngine::AEMath::Matrix;

struct Sparks {
    void render();
};


__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_render;

void Sparks::render()
{
    Matrix matrix;

    if (F<uint8_t>(this, 0x10) != 0) {
        void **canvas = g_Sparks_canvas_render;
        PaintCanvas_SetTexture(*canvas, F<uint32_t>(this, 0x8), 0xffffffff);
        PaintCanvas_SetBlendMode(*canvas, 2);

        void *canvasObj = *canvas;
        uint32_t *m = (uint32_t *)&matrix;
        m[0] = 0x3f800000;
        m[1] = 0;
        m[2] = 0;
        m[3] = 0;
        m[4] = 0;
        m[5] = 0x3f800000;
        m[6] = 0;
        m[7] = 0;
        m[8] = 0;
        m[9] = 0;
        m[10] = 0x3f800000;
        m[11] = 0;
        m[12] = 0x3f800000;
        m[13] = 0x3f800000;
        m[14] = 0x3f800000;

        PaintCanvas_SetWorldViewMatrix(canvasObj, matrix);
        PaintCanvas_DrawSpriteSystem(*canvas, F<uint32_t>(this, 0x4));
    }
}
