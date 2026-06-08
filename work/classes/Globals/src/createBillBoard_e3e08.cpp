#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" void PaintCanvas_MeshCreate(void *canvas, int a, int b, int c, unsigned short d,
                                       void *meshOut);
// per-face index triple setter
extern "C" void Mesh_setFace(void *canvas, int mesh, int face, int i0, int i1, int i2);
// per-vertex UV/attr setter (variadic-ish; extra floats are uv coords)
extern "C" void Mesh_setUV(void *canvas, int mesh, int vert, float u, float v);
// per-vertex position setter (x, y, z)
extern "C" void Mesh_setVertex(void *canvas, int mesh, int vert, float x, float y, float z);
extern "C" float VectorSignedToFloat(int v, int mode);

extern void *const gCBB_counter __attribute__((visibility("hidden")));   // DAT_000f40a0
extern void *const gCBB_canvas __attribute__((visibility("hidden")));    // DAT_000f40a4

// Globals::createBillBoard(int p1, int height, float u0, float v0, float u1, float v1, int width)
// p1 ignored layout-wise; height in r1, width passed on stack(param_7).
extern "C" void Globals_createBillBoard(int p1, int height, float u0, float v0, float u1, float v1,
                                        int width)
{
    (void)p1;
    unsigned *counter = *(unsigned **)gCBB_counter;
    int *canvasP = *(int **)gCBB_canvas;
    int snapshot = *counter;

    long long mesh64 = 0;
    PaintCanvas_MeshCreate((void *)(long)*canvasP, 0xc, 6, 0x13, 0, &mesh64);
    int mesh = (int)mesh64;
    int cv = *canvasP;

    Mesh_setFace((void *)(long)cv, mesh, 0, 0, 1, 2);
    Mesh_setFace((void *)(long)cv, mesh, 1, 2, 1, 3);
    Mesh_setFace((void *)(long)cv, mesh, 2, 4, 5, 6);
    Mesh_setFace((void *)(long)cv, mesh, 3, 6, 5, 7);
    Mesh_setFace((void *)(long)cv, mesh, 4, 8, 9, 10);
    Mesh_setFace((void *)(long)cv, mesh, 5, 10, 9, 0xb);

    // UV coords: pairs (u0,v0) and (u0,v1) drive the texture mapping of each face.
    Mesh_setUV((void *)(long)cv, mesh, 0, u0, 0);
    Mesh_setUV((void *)(long)cv, mesh, 1, u1, 0);
    Mesh_setUV((void *)(long)cv, mesh, 2, v1, u0);
    Mesh_setUV((void *)(long)cv, mesh, 3, v1, v0);
    Mesh_setUV((void *)(long)cv, mesh, 4, u0, 0);
    Mesh_setUV((void *)(long)cv, mesh, 5, u1, 0);
    Mesh_setUV((void *)(long)cv, mesh, 6, v1, u0);
    Mesh_setUV((void *)(long)cv, mesh, 7, v1, v0);
    Mesh_setUV((void *)(long)cv, mesh, 8, u0, 0);
    Mesh_setUV((void *)(long)cv, mesh, 9, u1, 0);
    Mesh_setUV((void *)(long)cv, mesh, 10, v1, u0);
    Mesh_setUV((void *)(long)cv, mesh, 0xb, v1, v0);

    float pw = VectorSignedToFloat(width, 0);
    float nh = VectorSignedToFloat(-height, 0);
    Mesh_setVertex((void *)(long)cv, mesh, 0, nh, 0, pw);
    float nw = VectorSignedToFloat(-width, 0);
    Mesh_setVertex((void *)(long)cv, mesh, 1, nh, 0, nw);
    float ph = VectorSignedToFloat(height, 0);
    Mesh_setVertex((void *)(long)cv, mesh, 2, ph, 0, pw);
    Mesh_setVertex((void *)(long)cv, mesh, 3, ph, 0, nw);
    Mesh_setVertex((void *)(long)cv, mesh, 4, 0, nh, pw);
    Mesh_setVertex((void *)(long)cv, mesh, 5, 0, nh, nw);
    Mesh_setVertex((void *)(long)cv, mesh, 6, 0, ph, pw);
    Mesh_setVertex((void *)(long)cv, mesh, 7, 0, ph, nw);
    Mesh_setVertex((void *)(long)cv, mesh, 8, nh, ph, 0);
    Mesh_setVertex((void *)(long)cv, mesh, 9, nh, nh, 0);
    Mesh_setVertex((void *)(long)cv, mesh, 10, ph, ph, 0);
    Mesh_setVertex((void *)(long)cv, mesh, 0xb, ph, nh, 0);

    if (*counter == (unsigned)snapshot) {
        return;
    }
    __stack_chk_fail(*counter - snapshot);
}
