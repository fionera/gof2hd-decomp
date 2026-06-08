#include "class.h"

// AbyssEngine::SpriteSystemDraw(Engine*, AEMath::Matrix const& view, AEMath::Matrix const& world,
//                               SpriteSystem*)
// Rebuild every sprite's billboard quad in view space (4 corners offset by the sprite's size
// around its world-transformed center), then either draw the backing mesh directly or, when
// the mesh carries a batching context (ptr@0x30), enqueue the mesh + its view/world/uv matrices
// into the batch arrays.
namespace AbyssEngine {

extern "C" void AE_AEMath_matMul(Matrix *out, const Matrix *in);

int MeshDraw(Engine *engine, Mesh *mesh);

// Array<T>::AddCached helpers (templated in the binary).
extern "C" void AE_ArrayAddCached_MeshPtr(Mesh *value, void *array);
extern "C" void AE_ArrayAddCached_uint(unsigned int value, void *array);

extern "C" void AE_SpriteSystem_pushMatrix(
    unsigned int m0, unsigned int m1, unsigned int m2, unsigned int m3, unsigned int m4,
    unsigned int m5, unsigned int m6, unsigned int m7, unsigned int m8, unsigned int m9,
    unsigned int m10, unsigned int m11, unsigned int m12, unsigned int m13, unsigned int m14,
    int dst);

extern "C" unsigned int g_SpriteSystem_oneHalf; // DAT_00097058 (0.5 bit pattern)

void SpriteSystemDraw(Engine *engine, Matrix *view, Matrix *world, SpriteSystem *sys)
{
    if (sys == 0)
        return;

    Mesh *mesh = (Mesh *)pp(sys, 0x10);
    float *vbuf = (float *)pp(mesh, 0x4);

    Matrix mv;
    AE_AEMath_matMul(&mv, view);

    unsigned short count = u16(sys, 0x0);
    unsigned char sharedSize = u8(sys, 0xc);
    float *posCpu = (float *)pp(sys, 0x4);
    short *sizeCpu = (short *)pp(sys, 0x8);
    const float *W = (const float *)world;

    unsigned short sizeIdx = 0;
    unsigned short posIdx = 0;
    for (unsigned int q = 0; (q & 0xffff) < (unsigned int)count * 0xc; q += 0xc) {
        float *p = posCpu + (unsigned int)posIdx;
        posIdx = posIdx + 3;
        float px = p[0], py = p[1], pz = p[2];

        unsigned int si = sizeIdx;
        sizeIdx = (unsigned short)(((sharedSize ^ 1) & 0xff) + sizeIdx);
        float half = (float)((int)sizeCpu[si] >> 1);

        // Transform the sprite center by the world matrix (column-major 3x4 used here).
        float cx = W[0xc / 4 + 0] + W[4 / 4] * py + W[0] * px + W[8 / 4] * pz;
        float cy = W[0x1c / 4] + py * W[0x14 / 4] + px * W[0x10 / 4] + pz * W[0x18 / 4];
        float cz = W[0x2c / 4] + py * W[0x24 / 4] + px * W[0x20 / 4] + pz * W[0x28 / 4];

        float left = cx - half, right = cx + half;
        float bottom = cy - half, top = cy + half;

        float *out = vbuf + q;
        out[0] = left;  out[1] = top;    out[2] = cz;
        out[3] = right; out[4] = top;    out[5] = cz;
        out[6] = right; out[7] = bottom; out[8] = cz;
        out[9] = left;  out[10] = bottom; out[11] = cz;
    }

    if (i32(mesh, 0x30) == 0) {
        MeshDraw(engine, mesh);
    } else {
        void *batch = pp(mesh, 0x30);
        AE_ArrayAddCached_MeshPtr(mesh, (char *)batch + 0x44);

        const unsigned int *v = (const unsigned int *)view;
        AE_SpriteSystem_pushMatrix(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9],
                                   v[10], v[11], v[12], v[13], v[14], (int)pp(mesh, 0x30) + 0x2c);
        unsigned int one = 0x3f800000;
        AE_SpriteSystem_pushMatrix(one, 0, 0, 0, 0, g_SpriteSystem_oneHalf, 0, 0, 0, 0, one, 0,
                                   one, one, one, (int)pp(mesh, 0x30) + 0x38);
        const unsigned int *w = (const unsigned int *)world;
        AE_SpriteSystem_pushMatrix(w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9],
                                   w[10], w[11], w[12], w[13], w[14], (int)pp(mesh, 0x30) + 0x5c);
        AE_ArrayAddCached_uint(0xffffffff, (char *)pp(mesh, 0x30) + 0x50);
    }
}

} // namespace AbyssEngine
