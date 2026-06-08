#include "class.h"

extern "C" void glDeleteBuffers(int n, const void *buffers);
void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;

// AbyssEngine::MeshReleaseIntern(AbyssEngine::Engine*, AbyssEngine::Mesh**)
// Tear down a mesh: delete its GL buffer objects (gated on the flag byte at offset 0 and the
// "uploaded" flag at 0x5c), free all CPU-side arrays, destroy the owned Transform, and free
// the mesh. Skips GL/array teardown entirely when it is a shared/aliased mesh (byte@0x38).
namespace AbyssEngine {

extern "C" char *g_Mesh_tangentEnabledFlag; // *(DAT_0007c4cc + 0x7c3de)
extern "C" char *g_Mesh_extraArraysFlag;    // *(DAT_0007c4d0 + 0x7c478)

extern "C" void *AE_Transform_dtor(void *self);

static inline void freeArray(Mesh **slot, unsigned int off)
{
    void *p = pp(*slot, off);
    if (p != 0)
        operator delete[](p);
    pp(*slot, off) = 0;
}

void MeshReleaseIntern(Engine * /*engine*/, Mesh **slot)
{
    Mesh *m = (Mesh *)*slot;
    if (m == 0)
        return;

    if (u8(m, 0x38) == 0) {
        if (u8(m, 0x5c) != 0) {
            glDeleteBuffers(1, (char *)m + 0x60);
            glDeleteBuffers(1, (char *)*slot + 0x64);
            m = (Mesh *)*slot;
            unsigned char flags = u8(m, 0x0);
            if (flags & 2) {
                glDeleteBuffers(1, (char *)m + 0x68);
                m = (Mesh *)*slot;
                flags = u8(m, 0x0);
            }
            if ((flags & 4) != 0) { // bit set when (flags<<0x1d) is negative
                glDeleteBuffers(1, (char *)m + 0x6c);
                if (*g_Mesh_tangentEnabledFlag != 0) {
                    glDeleteBuffers(1, (char *)*slot + 0x70);
                    glDeleteBuffers(1, (char *)*slot + 0x74);
                }
            }
            m = (Mesh *)*slot;
            if (u8(m, 0x0) & 8) { // bit set when (flags<<0x1c) is negative
                glDeleteBuffers(1, (char *)m + 0x78);
                m = (Mesh *)*slot;
            }
        }

        freeArray(slot, 0x2c);
        freeArray(slot, 0x4);
        freeArray(slot, 0x8);
        freeArray(slot, 0xc);
        freeArray(slot, 0x10);

        if (*g_Mesh_extraArraysFlag != 0) {
            freeArray(slot, 0x14);
            freeArray(slot, 0x18);
        }
    }

    void *t = pp(*slot, 0x34);
    if (t != 0) {
        void *freed = AE_Transform_dtor(t);
        operator delete(freed);
    }
    pp(*slot, 0x34) = 0;

    if (*slot != 0)
        operator delete((void *)*slot);
    *slot = 0;
}

} // namespace AbyssEngine
