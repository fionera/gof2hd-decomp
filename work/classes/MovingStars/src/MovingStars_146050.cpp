#include "class.h"

using AbyssEngine::AEMath::Matrix;

// MovingStars::MovingStars()
typedef void *(*AllocFn)(int);

extern "C" int MovingStars_nextInt(uint32_t rng, int bound);               // AERandom::nextInt 0x71848
extern "C" uint32_t MovingStars_createBillBoard(int globals, int p2, float a, float b,
                                                float c, float d, int p7); // 0x76d74
extern "C" void MovingStars_TransformCreate(void *canvas, uint32_t *out);  // 0x720ac
extern "C" void MovingStars_TransformAddMeshId(void *canvas, uint32_t tf, uint32_t mesh); // 0x73030
extern "C" uint32_t MovingStars_TransformGetLocal(void *canvas, uint32_t tf);             // 0x720c4
extern "C" void MovingStars_MatrixSetTranslation(void *out, const void *base,
                                                 float x, float y, float z);               // 0x6f820
extern "C" void MovingStars_TextureCreate(void *canvas, int id, void *flag, int b);        // 0x6fbb0

__attribute__((visibility("hidden"))) extern AllocFn g_MovingStars_alloc;
__attribute__((visibility("hidden"))) extern int *g_MovingStars_rng_ctor;
__attribute__((visibility("hidden"))) extern int *g_MovingStars_globals_ctor;
__attribute__((visibility("hidden"))) extern void **g_MovingStars_canvas_ctor;

struct MovingStars {
    MovingStars();
};

static const float kBB0 = 1.0f, kBB1 = 2.0f, kBB2 = 3.0f, kBB3 = 4.0f;

MovingStars::MovingStars()
{
    char mat[60];

    AllocFn alloc = g_MovingStars_alloc;

    u32(this, 0x0) = 0;
    u32(this, 0x4) = 0;
    u32(this, 0x8) = 0;
    u32(this, 0xc) = 0;
    u32(this, 0x10) = 0;

    u32(this, 0x0) = (uint32_t)alloc(200);
    u32(this, 0x4) = (uint32_t)alloc(200);
    u32(this, 0x10) = (uint32_t)alloc(200);
    int *arr = (int *)alloc(200);
    u32(this, 0xc) = (uint32_t)arr;
    for (int i = 0; i != 0x32; i = i + 1)
        arr[i] = 0xffffffff;

    int *rng = g_MovingStars_rng_ctor;
    int *globals = g_MovingStars_globals_ctor;
    void **canvas = g_MovingStars_canvas_ctor;

    for (int j = 0; j != 200; j = j + 4) {
        MovingStars_nextInt(*rng, 4);
        uint32_t bb = MovingStars_createBillBoard(*globals, 0x46, kBB0, kBB1, kBB2, kBB3, 500);
        *(uint32_t *)(u32(this, 0x0) + j) = bb;
        MovingStars_TransformCreate(*canvas, (uint32_t *)(u32(this, 0x4) + j));
        MovingStars_TransformAddMeshId(*canvas, *(uint32_t *)(u32(this, 0x4) + j),
                                       *(uint32_t *)(u32(this, 0x0) + j));
        uint32_t loc = MovingStars_TransformGetLocal(*canvas, *(uint32_t *)(u32(this, 0x4) + j));
        MovingStars_MatrixSetTranslation(mat, (const void *)loc, 0, 0, 0);
    }

    MovingStars_TextureCreate(*canvas, 0x2711, (char *)this + 8, 0);
    u32(this, 0x18) = 0;
    u16(this, 0x14) = 0;
}
