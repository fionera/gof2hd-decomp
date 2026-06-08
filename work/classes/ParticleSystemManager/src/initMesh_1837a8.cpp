#include "class.h"

// ParticleSystemManager::initMesh()
// Mesh analogue of initSprites. When the mesh array (+0x3c) is non-empty it creates the shared
// particle mesh on the canvas (+0x04) from the packed vertex/index counts in +0x5c (low 14 bits
// << 2 verts, low 15 bits << 1 indices, format 0x1b), optionally creating its texture (+0x4a),
// stores the mesh handle at +0x54, builds a transform (+0x58) bound to that mesh, then binds
// every sub-system at its running vertex offset (4 bytes per particle).

extern "C" void PaintCanvas_MeshCreate(void *canvas, int verts, int indices, int fmt, unsigned int *out);
extern "C" void PaintCanvas_MeshCreateTex(void *canvas, int verts, int indices, int fmt,
                                          short tex, unsigned int *out);
extern "C" void PaintCanvas_TextureCreate(unsigned short canvas, unsigned int texId, bool b);
extern "C" void PaintCanvas_TransformCreate(void *canvas, unsigned int *out);
extern "C" void PaintCanvas_TransformAddMeshId(void *canvas, unsigned int transform, unsigned int mesh);

struct ParticleSystemManager { void initMesh(); };
void ParticleSystemManager::initMesh()
{
    if (i32(this, 0x3c) == 0)
        return;

    u32(this, 0x54) = 0xffffffff;
    u32(this, 0x58) = 0xffffffff;

    void *canvas = pp(this, 0x04);
    int verts   = (u16(this, 0x5c) & 0x3fff) << 2;
    int indices = (u16(this, 0x5c) & 0x7fff) << 1;

    if ((short)u16(this, 0x48) == -1) {
        if ((short)u16(this, 0x4a) != -1) {
            PaintCanvas_MeshCreate(canvas, verts, indices, 0x1b,
                                   (unsigned int *)((char *)this + 0x54));
            PaintCanvas_TextureCreate((unsigned short)(unsigned long)pp(this, 0x04),
                                      u16(this, 0x4a), (((char)(unsigned long)this + 'P') != 0));
        }
    } else {
        PaintCanvas_MeshCreateTex(canvas, verts, indices, 0x1b, (short)u16(this, 0x48),
                                  (unsigned int *)((char *)this + 0x54));
    }

    PaintCanvas_TransformCreate(pp(this, 0x04), (unsigned int *)((char *)this + 0x58));
    PaintCanvas_TransformAddMeshId(pp(this, 0x04), u32(this, 0x58), u32(this, 0x54));

    short offset = 0;
    for (unsigned i = 0; i < u32(this, 0x3c); ++i) {
        void *sys = ((void **)u32(this, 0x40))[i];
        typedef void (*pfn)(void *, unsigned int, short);
        ((pfn)(*(void ***)sys)[0])(sys, u32(this, 0x54), offset);

        void *sys2 = ((void **)u32(this, 0x40))[i];
        typedef short (*pfn2)(void *);
        short count = ((pfn2 *)*(void **)sys2)[4](sys2);   // vtable+0x10
        offset = offset + (short)(count * 4);
    }
}
