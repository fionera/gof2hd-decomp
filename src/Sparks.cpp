#include "gof2/Sparks.h"


extern "C" void *operator_new(uint32_t size);
extern "C" int AERandom_nextInt(void *rng, int bound);
extern "C" void operator_delete(void *ptr);
extern "C" void Sparks_explode_int(Sparks *self, int x, int y, int z);
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
    uint32_t *outSprite = &this->field_0x4;
    void **canvas = g_Sparks_canvas_ctor;
    uint32_t lifetime = 0x514;
    if (kind == 0)
        lifetime = 0x1f4;

    this->field_0x14 = kind;
    this->field_0x18 = count;
    this->field_0x1c = lifetime;

    PaintCanvas_SpriteSystemCreate(*canvas, (uint16_t)count, false, outSprite);
    PaintCanvas_SpriteSystemSetAllUv(*canvas, this->field_0x4, 0.626953125f,
                                     0.001953125f, 0.748046875f, 0.123046875f);

    uint32_t n = this->field_0x18;
    this->field_0x10 = 0;
    this->field_0x0 = new int[n];
    this->field_0x20 = 0;

    if (this->field_0x14 == 1) {
        void **rng = g_Sparks_random_ctor;
        for (uint32_t i = 0; i < n; i++) {
            PaintCanvas_SpriteSystemSetSize(*canvas, this->field_0x4, (uint16_t)i, 1);
            int value = AERandom_nextInt(*rng, 0x1f4);
            this->field_0x0[i] = value;
            n = this->field_0x18;
            this->field_0x20 = value + this->field_0x20;
        }
    } else {
        this->field_0x0[0] = 0;
        this->field_0x20 = 0;
    }

    this->field_0xc = 0;
}

// ---- translate_15e6d4.cpp ----
using AbyssEngine::AEMath::Vector;

extern "C" void PaintCanvas_SpriteSystemAddPosition(void *canvas, uint32_t sprite, uint16_t index,
                                                     float x, float y, float z);

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_translate;

void Sparks::translate(Vector const &v)
{
    void **canvas = g_Sparks_canvas_translate;
    for (uint32_t i = 0; i < this->field_0x18; i++)
        PaintCanvas_SpriteSystemAddPosition(*canvas, this->field_0x4, (uint16_t)i,
                                            v.x, v.y, v.z);
}

// ---- _Sparks_15e474.cpp ----
Sparks::~Sparks()
{
    void *p = this->field_0x0;
    if (p != 0)
        operator_delete(p);
    this->field_0x0 = 0;
}

// ---- explode_15e594.cpp ----
void Sparks::explode(Vector const &v)
{
    return Sparks_explode_int(this, (int)v.x, (int)v.y, (int)v.z);
}

// ---- isRocket_15e468.cpp ----
bool Sparks::isRocket()
{
    return this->field_0x14 == 1;
}

// ---- update_15e660.cpp ----
extern "C" void PaintCanvas_SpriteSystemSetSize(void *canvas, uint32_t sprite, uint16_t index,
                                                 int16_t size);

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_update;

void Sparks::update(int step)
{
    if (this->field_0x10 == 0)
        return;

    int elapsed0 = this->field_0xc;
    uint32_t i = 0;
    elapsed0 += step;
    this->field_0xc = elapsed0;
    for (; i < this->field_0x18; i++) {
        int *thresholds = this->field_0x0;
        int elapsed = this->field_0xc;
        if (elapsed <= thresholds[i])
            continue;
        int size = this->field_0x1c - (elapsed << 1);
        PaintCanvas_SpriteSystemSetSize(*g_Sparks_canvas_update, this->field_0x4, (uint16_t)i,
                                        (int16_t)size);
    }

    int elapsed = this->field_0xc;
    int kind = this->field_0x14;
    if (kind == 1) {
        if (elapsed <= 0x1f4) {
            int limit = this->field_0x1c;
            elapsed += elapsed;
            if (elapsed <= limit)
                return;
        }
    } else {
        if (elapsed <= 0x1f4)
            return;
    }

    this->field_0x10 = 0;
    this->field_0xc = 0;
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

    if (this->field_0x10 != 0)
        return;

    if (this->field_0x14 == 1) {
        uint32_t i = 0;
        void **canvas = g_Sparks_canvas_explode_rocket;
        void **rng = g_Sparks_random_explode;
        int (*nextInt)(void *, int) = g_Sparks_nextInt_explode;
        for (; i < this->field_0x18; i++) {
            uint32_t sprite = this->field_0x4;
            void *canvasObj = *canvas;
            float px = (float)(nextInt(*rng, 0x190) + x0);
            float py = (float)(nextInt(*rng, 0x190) + y0);
            float pz = (float)(nextInt(*rng, 0x190) + z0);
            PaintCanvas_SpriteSystemSetPosition(canvasObj, sprite, (uint16_t)i,
                                                px, py, pz);
        }
    } else {
        void **canvas = g_Sparks_canvas_explode_single;
        PaintCanvas_SpriteSystemSetPosition(*canvas, this->field_0x4, 0,
                                            (float)x0, (float)y0, (float)z0);
    }

    this->field_0x10 = 1;
}

// ---- render_15e5c0.cpp ----
using AbyssEngine::AEMath::Matrix;

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_render;

void Sparks::render()
{
    Matrix matrix;

    if (this->field_0x10 != 0) {
        void **canvas = g_Sparks_canvas_render;
        PaintCanvas_SetTexture(*canvas, this->field_0x8, 0xffffffff);
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
        PaintCanvas_DrawSpriteSystem(*canvas, this->field_0x4);
    }
}
