#include "class.h"

// AbyssEngine::MaterialDraw(PaintCanvas*, Engine*, Material*, bool setTextures)
// Draw all submeshes of a material: optionally bind its textures, push the add-data, set the
// ambient light, then for each submesh multiply in its model matrix, set the UV matrix, unpack
// the packed RGBA color, and draw the mesh. Afterwards the per-frame submesh counters are
// reset to zero. Mirrors the recovered control flow; the heavy float register aliasing in the
// Ghidra output is normalized here.
namespace AbyssEngine {

extern "C" void AE_Engine_SetTexturesExt(Engine *self, unsigned int t0, unsigned int t1,
                                         unsigned int t2, unsigned int t3);
extern "C" void AE_Engine_SetAddData(Engine *self, void *data, int count);
extern "C" void AE_Engine_LightSetAmbient(float r, float g, float b, Engine *self);
extern "C" void AE_AEMath_matMul(Matrix *out, const Matrix *in);
extern "C" void AE_PaintCanvas_SetWorldViewMatrix(Matrix *self);
extern "C" void AE_Engine_SetModelMatrix(Matrix *self);
extern "C" void AE_Engine_SetUVMatrix(Engine *self, const Matrix *uv);
extern "C" void AE_Engine_SetColor(float r, float g, float b, float a);

int MeshDraw(Engine *engine, Mesh *mesh);

void MaterialDraw(PaintCanvas *canvas, Engine *engine, Material *mat, bool setTextures)
{
    if (canvas == 0 || mat == 0)
        return;

    char *m = (char *)mat;

    if (setTextures) {
        AE_Engine_SetTexturesExt(engine, u32(m, 0x0), u32(m, 0x4), u32(m, 0x8), 0xffffffff);
    }
    AE_Engine_SetAddData(engine, pp(m, 0x24), i32(m, 0x28));

    float ambient = f32(m, 0x68);
    if (ambient != -10.0f) {
        AE_Engine_LightSetAmbient(ambient, 0.0f, 0.0f, engine);
    }

    const float inv255 = 1.0f / 255.0f;
    int matOff = 0;
    for (unsigned int i = 0; i < u32(m, 0x44); ++i) {
        Matrix world;
        AE_AEMath_matMul(&world, (const Matrix *)((char *)pp(m, 0x60) + matOff));
        AE_PaintCanvas_SetWorldViewMatrix((Matrix *)canvas);
        AE_Engine_SetModelMatrix((Matrix *)engine);
        AE_Engine_SetUVMatrix(engine, (const Matrix *)((char *)pp(m, 0x3c) + matOff));

        unsigned int packed = *(unsigned int *)((char *)pp(m, 0x54) + i * 4);
        float cr = (float)((packed >> 16) & 0xff);
        float cg = (float)((packed >> 8) & 0xff);
        float cb = (float)(packed & 0xff);
        AE_Engine_SetColor(cb * inv255, cg * inv255, cr * inv255, 0.0f);

        MeshDraw(engine, *(Mesh **)((char *)pp(m, 0x48) + i * 4));
        matOff += 0x3c;
    }

    if (f32(m, 0x68) != -10.0f) {
        AE_Engine_LightSetAmbient(f32(m, 0x68), 0.0f, 0.0f, engine);
    }

    // Reset per-frame submesh accumulators.
    i32(m, 0x2c) = 0;
    i32(m, 0x5c) = 0;
    i32(m, 0x38) = 0;
    i32(m, 0x44) = 0;
    i32(m, 0x50) = 0;
}

} // namespace AbyssEngine
