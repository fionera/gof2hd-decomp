#include "class.h"

void *operator new(size_t_ size);
void *operator new[](size_t_ size);
void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;
extern "C" void *__aeabi_memclr(void *dst, size_t_ n);
extern "C" void *__aeabi_memclr4(void *dst, size_t_ n);

// AbyssEngine::SpriteSystemCreate(Engine*, unsigned short count, bool sharedSize,
//                                 SpriteSystem** out)
// Allocate a SpriteSystem of `count` quads: build a backing Mesh (4 verts + 2 tris per
// sprite), fill the shared index buffer, default vertex colors to white and normals/tangents
// to the identity basis. Returns 1, -1 on mesh failure, -4 on count == 0.
namespace AbyssEngine {

extern "C" char *g_SpriteSystem_tangentFlag; // *(DAT_00096c68 + 0x96c30)

int MeshCreate(Engine *engine, unsigned short vertexCount, unsigned short triCount,
               unsigned int fmt, void **out);
void MeshRelease(Engine *engine, Mesh **slot);

int SpriteSystemCreate(Engine *engine, unsigned short count, bool sharedSize, SpriteSystem **out)
{
    unsigned int n = count;
    if (n == 0)
        return -4;

    char *sys = (char *)operator new(0x14);
    pp(sys, 0x4) = 0;
    pp(sys, 0x8) = 0;
    pp(sys, 0x10) = 0;
    *out = (SpriteSystem *)sys;
    u16(sys, 0x0) = count;

    unsigned int triCount = (n << 1) & 0xffff;
    int rc = MeshCreate(engine, (unsigned short)((n & 0x3fff) << 2), (unsigned short)triCount,
                        0x1f, (void **)(sys + 0x10));
    char *s = (char *)*out;
    if (rc != 1) {
        MeshRelease(engine, (Mesh **)(s + 0x10));
        if (pp(*out, 0x4) != 0)
            operator delete[](pp(*out, 0x4));
        pp(*out, 0x4) = 0;
        if (pp(*out, 0x8) != 0)
            operator delete[](pp(*out, 0x8));
        pp(*out, 0x8) = 0;
        if (*out != 0)
            operator delete((void *)*out);
        *out = 0;
        return -1;
    }

    Mesh *mesh = (Mesh *)pp(s, 0x10);
    int *indexArr = (int *)pp(mesh, 0x2c);

    void *posCpu = operator new[](n * 0xc);
    pp(s, 0x4) = posCpu;
    __aeabi_memclr4(pp(*out, 0x4), n * 0xc);

    s = (char *)*out;
    u8(s, 0xc) = sharedSize ? 1 : 0;
    if (sharedSize) {
        void *sz = operator new[](2);
        pp(s, 0x8) = sz;
        *(unsigned short *)pp(*out, 0x8) = 0;
    } else {
        void *sz = operator new[]((n << 1));
        pp(s, 0x8) = sz;
        __aeabi_memclr(pp(*out, 0x8), (n << 1));
    }

    // Fill the index buffer: two triangles (0,1,2, 0,2,3) per quad.
    unsigned short base = 0;
    char *idx = (char *)indexArr;
    for (unsigned int off = 0; (off & 0xffff) < n * 6; off += 6) {
        unsigned short *p = (unsigned short *)(idx + (off & 0xffff) * 2);
        p[0] = base;
        p[1] = base | 1;
        p[2] = base | 2;
        p[3] = base;
        p[4] = base | 2;
        p[5] = base | 3;
        base = base + 4;
    }

    Mesh *m = (Mesh *)pp(*out, 0x10);
    unsigned int vcount = (unsigned int)(unsigned short)u16(m, 0x2);

    // Default vertex colors to white (1.0 per channel).
    float *colors = (float *)pp(m, 0xc);
    for (unsigned int i = 0; i <= vcount * 4 && vcount * 4 - i != 0; ++i)
        colors[i] = 1.0f;

    // Default normals (and optionally tangents/binormals) to the identity basis.
    char tangent = *g_SpriteSystem_tangentFlag;
    int vo = 0;
    char *normals = (char *)pp(m, 0x10);
    char *tangents = (char *)pp(m, 0x14);
    char *binormals = (char *)pp(m, 0x18);
    for (unsigned int k = vcount; k != 0; --k) {
        float *nrm = (float *)(normals + vo);
        nrm[0] = 0.0f;
        nrm[1] = 0.0f;
        nrm[2] = 1.0f;
        if (tangent != 0) {
            float *tan = (float *)(tangents + vo);
            tan[0] = 1.0f;
            tan[1] = 0.0f;
            tan[2] = 0.0f;
            float *bin = (float *)(binormals + vo);
            bin[0] = 0.0f;
            bin[1] = 1.0f;
            bin[2] = 0.0f;
        }
        vo += 0xc;
    }

    return 1;
}

} // namespace AbyssEngine
