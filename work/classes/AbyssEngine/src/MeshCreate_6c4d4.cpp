#include "class.h"

void *operator new(size_t_ size);
void *operator new[](size_t_ size);
extern "C" void *__aeabi_memclr(void *dst, size_t_ n);
extern "C" void *__aeabi_memclr4(void *dst, size_t_ n);

// AbyssEngine::MeshCreate(Engine*, unsigned short vertexCount, unsigned short triCount,
//                         signed char vertexFormat, Mesh** out)
// Allocate and zero-initialize a Mesh (0x88 bytes), then allocate the per-attribute CPU
// arrays selected by the vertex-format flag bits. Returns 1, or -4 on invalid arguments.
namespace AbyssEngine {

extern "C" char *g_Mesh_tangentEnabledFlag; // *(DAT_0007c604 + 0x7c5c8)

int MeshCreate(Engine * /*engine*/, unsigned short vertexCount, unsigned short triCount,
               unsigned int vertexFormat, void **out)
{
    if (vertexCount < 4 || triCount == 0 || (vertexFormat & 1) == 0)
        return -4;

    char *m = (char *)operator new(0x88);

    // Zero the identity/colour/transform region. The decompile splats a 16-byte zero vector
    // across many slots; clearing the whole struct is the faithful net effect, after which we
    // write the few non-zero defaults.
    __aeabi_memclr4(m, 0x88);
    f32(m, 0x4c) = 1.0f; // default scale = 1.0

    *out = m;
    s16(m, 0x2) = (short)vertexCount;
    u8(m, 0x0) = (char)vertexFormat;
    s16(m, 0x28) = (short)(triCount + (triCount << 1)); // 3 * triCount (index count)
    s16(m, 0x2a) = (short)triCount;

    unsigned int posBytes = (unsigned int)vertexCount * 0xc;
    void *p = operator new[](posBytes);
    pp(m, 0x4) = p;
    __aeabi_memclr4(p, posBytes);

    if (vertexFormat & 0x20) { // (fmt<<0x1b) negative -> index array
        p = operator new[]((unsigned int)triCount * 6);
        pp(m, 0x2c) = p;
        __aeabi_memclr(p, (unsigned int)triCount * 6);
    }
    if (vertexFormat & 4) { // (fmt<<0x1e) negative -> uv array
        p = operator new[]((unsigned int)vertexCount << 3);
        pp(m, 0x8) = p;
        __aeabi_memclr4(p, (unsigned int)vertexCount << 3);
    }
    if (vertexFormat & 8) { // (fmt<<0x1d) negative -> normal array (+ tangents/binormals)
        p = operator new[](posBytes);
        pp(m, 0x10) = p;
        __aeabi_memclr4(p, posBytes);
        if (*g_Mesh_tangentEnabledFlag != 0) {
            p = operator new[](posBytes);
            pp(m, 0x14) = p;
            __aeabi_memclr4(p, posBytes);
            p = operator new[](posBytes);
            pp(m, 0x18) = p;
            __aeabi_memclr4(p, posBytes);
        }
    }
    if (vertexFormat & 0x10) { // (fmt<<0x1c) negative -> colour array
        p = operator new[]((unsigned int)vertexCount << 4);
        pp(m, 0xc) = p;
        __aeabi_memclr4(p, (unsigned int)vertexCount << 4);
    }

    return 1;
}

} // namespace AbyssEngine
