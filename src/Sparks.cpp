#include "gof2/Sparks.h"


extern "C" void *operator_new(uint32_t size);
namespace AbyssEngine { namespace AERandom { int nextInt(void *rng, int bound); } }
extern "C" void operator_delete(void *ptr);
extern "C" void PaintCanvas_SetTexture(void *canvas, uint32_t texture, uint32_t mode);
extern "C" void PaintCanvas_SetBlendMode(void *canvas, int mode);
extern "C" void PaintCanvas_SetWorldViewMatrix(void *canvas, Matrix const &m);
extern "C" void PaintCanvas_DrawSpriteSystem(void *canvas, uint32_t sprite);

// ---- Sparks_15e38c.cpp ----
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
    uint32_t *outSprite = &this->spriteSystem;
    void **canvas = g_Sparks_canvas_ctor;
    uint32_t lifetime = 0x514;
    if (kind == 0)
        lifetime = 0x1f4;

    this->kind = kind;
    this->count = count;
    this->lifetime = lifetime;

    PaintCanvas_SpriteSystemCreate(*canvas, (uint16_t)count, false, outSprite);
    PaintCanvas_SpriteSystemSetAllUv(*canvas, this->spriteSystem, 0.626953125f,
                                     0.001953125f, 0.748046875f, 0.123046875f);

    uint32_t n = this->count;
    this->active = 0;
    this->lifetimeThresholds = new int[n];
    this->totalThreshold = 0;

    if (this->kind == 1) {
        void **rng = g_Sparks_random_ctor;
        for (uint32_t i = 0; i < n; i++) {
            PaintCanvas_SpriteSystemSetSize(*canvas, this->spriteSystem, (uint16_t)i, 1);
            int value = AbyssEngine::AERandom::nextInt(*rng, 0x1f4);
            this->lifetimeThresholds[i] = value;
            n = this->count;
            this->totalThreshold = value + this->totalThreshold;
        }
    } else {
        this->lifetimeThresholds[0] = 0;
        this->totalThreshold = 0;
    }

    this->elapsed = 0;
}

// ---- translate_15e6d4.cpp ----
using AbyssEngine::AEMath::Vector;

extern "C" void PaintCanvas_SpriteSystemAddPosition(void *canvas, uint32_t sprite, uint16_t index,
                                                     float x, float y, float z);

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_translate;

void Sparks::translate(Vector const &v)
{
    void **canvas = g_Sparks_canvas_translate;
    for (uint32_t i = 0; i < this->count; i++)
        PaintCanvas_SpriteSystemAddPosition(*canvas, this->spriteSystem, (uint16_t)i,
                                            v.x, v.y, v.z);
}

// ---- _Sparks_15e474.cpp ----
Sparks::~Sparks()
{
    void *p = this->lifetimeThresholds;
    if (p != 0)
        operator_delete(p);
    this->lifetimeThresholds = 0;
}

// ---- explode_15e594.cpp ----
void Sparks::explode(Vector const &v)
{
    return this->explode((int)v.x, (int)v.y, (int)v.z);
}

// ---- isRocket_15e468.cpp ----
bool Sparks::isRocket()
{
    return this->kind == 1;
}

// ---- update_15e660.cpp ----
extern "C" void PaintCanvas_SpriteSystemSetSize(void *canvas, uint32_t sprite, uint16_t index,
                                                 int16_t size);

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_update;

void Sparks::update(int step)
{
    if (this->active == 0)
        return;

    int elapsed0 = this->elapsed;
    uint32_t i = 0;
    elapsed0 += step;
    this->elapsed = elapsed0;
    for (; i < this->count; i++) {
        int *thresholds = this->lifetimeThresholds;
        int elapsed = this->elapsed;
        if (elapsed <= thresholds[i])
            continue;
        int size = this->lifetime - (elapsed << 1);
        PaintCanvas_SpriteSystemSetSize(*g_Sparks_canvas_update, this->spriteSystem, (uint16_t)i,
                                        (int16_t)size);
    }

    int elapsed = this->elapsed;
    int kind = this->kind;
    if (kind == 1) {
        if (elapsed <= 0x1f4) {
            int limit = this->lifetime;
            elapsed += elapsed;
            if (elapsed <= limit)
                return;
        }
    } else {
        if (elapsed <= 0x1f4)
            return;
    }

    this->active = 0;
    this->elapsed = 0;
}

// ---- explode_15e48c.cpp ----
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

    if (this->active != 0)
        return;

    if (this->kind == 1) {
        uint32_t i = 0;
        void **canvas = g_Sparks_canvas_explode_rocket;
        void **rng = g_Sparks_random_explode;
        int (*nextInt)(void *, int) = g_Sparks_nextInt_explode;
        for (; i < this->count; i++) {
            uint32_t sprite = this->spriteSystem;
            void *canvasObj = *canvas;
            float px = (float)(nextInt(*rng, 0x190) + x0);
            float py = (float)(nextInt(*rng, 0x190) + y0);
            float pz = (float)(nextInt(*rng, 0x190) + z0);
            PaintCanvas_SpriteSystemSetPosition(canvasObj, sprite, (uint16_t)i,
                                                px, py, pz);
        }
    } else {
        void **canvas = g_Sparks_canvas_explode_single;
        PaintCanvas_SpriteSystemSetPosition(*canvas, this->spriteSystem, 0,
                                            (float)x0, (float)y0, (float)z0);
    }

    this->active = 1;
}

// ---- render_15e5c0.cpp ----
using AbyssEngine::AEMath::Matrix;

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_render;

void Sparks::render()
{
    Matrix matrix;

    if (this->active != 0) {
        void **canvas = g_Sparks_canvas_render;
        PaintCanvas_SetTexture(*canvas, this->texture, 0xffffffff);
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
        PaintCanvas_DrawSpriteSystem(*canvas, this->spriteSystem);
    }
}
