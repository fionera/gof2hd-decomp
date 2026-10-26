#include "engine/render/Sparks.h"
#include "game/core/Globals.h"
#include "engine/core/AbyssEngine.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/AERandom.h"

static PaintCanvas **g_Sparks_canvas_ctor = &Globals::Canvas;

static AERandom **g_Sparks_random_ctor = &Globals::rnd;

Sparks::Sparks(int kind) {
    uint32_t count = 5;
    if (kind == 0)
        count = 1;
    PaintCanvas **canvas = g_Sparks_canvas_ctor;
    uint32_t lifetime = 0x514;
    if (kind == 0)
        lifetime = 0x1f4;

    this->kind = kind;
    this->count = count;
    this->lifetime = lifetime;

    (*canvas)->SpriteSystemCreate((uint16_t) count, false, this->spriteSystem);
    (*canvas)->SpriteSystemSetAllUv(this->spriteSystem, 0.626953125f,
                                    0.001953125f, 0.748046875f, 0.123046875f);

    uint32_t n = this->count;
    this->active = 0;
    this->lifetimeThresholds = new int[n];
    this->totalThreshold = 0;

    if (this->kind == 1) {
        AERandom **rng = g_Sparks_random_ctor;
        for (uint32_t i = 0; i < n; i++) {
            (*canvas)->SpriteSystemSetSize(this->spriteSystem, (uint16_t) i, 1);
            int value = (*rng)->nextInt(0x1f4);
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

static PaintCanvas **g_Sparks_canvas_translate = &Globals::Canvas;

void Sparks::translate(Vector const &v) {
    PaintCanvas **canvas = g_Sparks_canvas_translate;
    for (uint32_t i = 0; i < this->count; i++)
        (*canvas)->SpriteSystemAddPosition(this->spriteSystem, (uint16_t) i,
                                           v.x, v.y, v.z);
}

Sparks::~Sparks() {
    delete[] this->lifetimeThresholds;
    this->lifetimeThresholds = nullptr;
}

void Sparks::explode(Vector const &v) {
    return this->explode((int) v.x, (int) v.y, (int) v.z);
}

bool Sparks::isRocket() {
    return this->kind == 1;
}

void Sparks::update(int step) {
    if (this->active == 0)
        return;

    this->elapsed += step;
    for (uint32_t i = 0; i < this->count; i++) {
        if (this->elapsed > this->lifetimeThresholds[i])
            Globals::Canvas->SpriteSystemSetSize(this->spriteSystem, (uint16_t) i,
                                                 (int16_t) (this->lifetime - (this->elapsed << 1)));
    }

    if ((this->kind == 1 && this->elapsed * 2 > this->lifetime) || this->elapsed > 500) {
        this->elapsed = 0;
        this->active = 0;
    }
}

static PaintCanvas **g_Sparks_canvas_explode_rocket = &Globals::Canvas;

static PaintCanvas **g_Sparks_canvas_explode_single = &Globals::Canvas;

static AERandom **g_Sparks_random_explode = &Globals::rnd;

void Sparks::explode(int x, int y, int z) {
    int x0 = x;
    int y0 = y;
    int z0 = z;

    if (this->active != 0)
        return;

    if (this->kind == 1) {
        uint32_t i = 0;
        PaintCanvas **canvas = g_Sparks_canvas_explode_rocket;
        AERandom **rng = g_Sparks_random_explode;
        for (; i < this->count; i++) {
            uint32_t sprite = this->spriteSystem;
            PaintCanvas *canvasObj = *canvas;
            float px = (float) ((*rng)->nextInt(0x190) + x0);
            float py = (float) ((*rng)->nextInt(0x190) + y0);
            float pz = (float) ((*rng)->nextInt(0x190) + z0);
            canvasObj->SpriteSystemSetPosition(sprite, (uint16_t) i,
                                               px, py, pz);
        }
    } else {
        PaintCanvas **canvas = g_Sparks_canvas_explode_single;
        (*canvas)->SpriteSystemSetPosition(this->spriteSystem, 0,
                                           (float) x0, (float) y0, (float) z0);
    }

    this->active = 1;
}

void Sparks::render() {
    if (this->active != 0) {
        Globals::Canvas->SetTexture(this->texture, 0xffffffff);
        Globals::Canvas->SetBlendMode(AbyssEngine::BlendMode_2);

        PaintCanvas *canvas = Globals::Canvas;
        Matrix matrix;
        matrix.m[0] = 1.0f;
        matrix.m[1] = 0.0f;
        matrix.m[2] = 0.0f;
        matrix.m[3] = 0.0f;
        matrix.m[4] = 0.0f;
        matrix.m[5] = 1.0f;
        matrix.m[6] = 0.0f;
        matrix.m[7] = 0.0f;
        matrix.m[8] = 0.0f;
        matrix.m[9] = 0.0f;
        matrix.m[10] = 1.0f;
        matrix.m[11] = 0.0f;
        matrix.m[12] = 1.0f;
        matrix.m[13] = 1.0f;
        matrix.m[14] = 1.0f;

        canvas->SetWorldViewMatrix(matrix);
        Globals::Canvas->DrawSpriteSystem(this->spriteSystem);
    }
}
