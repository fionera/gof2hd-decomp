#include "class.h"

extern "C" {
void glGenBuffers(int n, void *buffers);
void glBindBuffer(unsigned int target, unsigned int buffer);
void glBufferData(unsigned int target, unsigned int size, const void *data, unsigned int usage);
void glDeleteBuffers(int n, const void *buffers);
int glGetError();
}
void operator delete[](void *ptr) noexcept;

// AbyssEngine::MeshConvertToVBOIntern(AbyssEngine::Mesh*)
// Upload the mesh's CPU vertex arrays into GL buffer objects (position/uv+index/normal
// [+tangent/binormal]/color, selected by the vertex-format flag byte). On success, optionally
// frees the CPU copies and marks the mesh uploaded (byte@0x5c); on GL error, tears the buffers
// back down. Returns 1 / -1 / -4.
namespace AbyssEngine {

extern "C" char *g_Mesh_vboEnabledFlag;     // *(DAT_0007d7cc + 0x7d526)
extern "C" char *g_Mesh_tangentEnabledFlag; // *(DAT_0007d7d0 + 0x7d61c)
extern "C" char *g_Mesh_keepCpuCopyFlag;    // *(DAT_0007d7d8 + 0x7d764)
extern "C" char *g_Mesh_tangentDelFlag;     // *(DAT_0007d7d4 + 0x7d72c)
extern "C" int *g_Mesh_vboByteCounter;      // *(DAT_0007d7dc + 0x7d7be)

static inline void freeIf(Mesh *m, unsigned int off)
{
    if (pp(m, off) != 0)
        operator delete[](pp(m, off));
    pp(m, off) = 0;
}

int MeshConvertToVBOIntern(Mesh *m)
{
    if (m == 0 || *g_Mesh_vboEnabledFlag == 0)
        return -4;
    if (u8(m, 0x5c) != 0 || s16(m, 0x28) == 0)
        return -4;

    void *posArr = pp(m, 0x4);
    void *uvArr = pp(m, 0x8);
    void *colArr = pp(m, 0xc);
    void *normArr = pp(m, 0x10);
    void *tanArr = pp(m, 0x14);
    void *binArr = pp(m, 0x18);

    unsigned int vcount = (unsigned int)(unsigned short)u16(m, 0x2);

    glGenBuffers(1, (char *)m + 0x60);
    glBindBuffer(0x8892, u32(m, 0x60));
    glBufferData(0x8892, vcount * 0xc, posArr, 0x88e4);

    unsigned char flags = u8(m, 0x0);
    if (flags & 2) {
        glGenBuffers(1, (char *)m + 0x68);
        glBindBuffer(0x8892, u32(m, 0x68));
        glBufferData(0x8892, vcount << 3, uvArr, 0x88e4);
        i32(m, 0x7c) += (int)(vcount * 8);

        glGenBuffers(1, (char *)m + 0x64);
        glBindBuffer(0x8893, u32(m, 0x64));
        glBufferData(0x8893, (unsigned int)(unsigned short)u16(m, 0x28) << 1, pp(m, 0x2c), 0x88e4);
        i32(m, 0x7c) += (int)((unsigned int)(unsigned short)u16(m, 0x28) * 2);
        flags = u8(m, 0x0);
    }

    if (flags & 8) {
        glGenBuffers(1, (char *)m + 0x6c);
        glBindBuffer(0x8892, u32(m, 0x6c));
        glBufferData(0x8892, vcount * 0xc, normArr, 0x88e4);
        i32(m, 0x7c) += (int)(vcount * 0xc);
        if (*g_Mesh_tangentEnabledFlag != 0) {
            glGenBuffers(1, (char *)m + 0x70);
            glBindBuffer(0x8892, u32(m, 0x70));
            glBufferData(0x8892, vcount * 0xc, tanArr, 0x88e4);
            i32(m, 0x7c) += (int)(vcount * 0xc);
            glGenBuffers(1, (char *)m + 0x74);
            glBindBuffer(0x8892, u32(m, 0x74));
            glBufferData(0x8892, vcount * 0xc, binArr, 0x88e4);
            i32(m, 0x7c) += (int)(vcount * 0xc);
        }
    }

    if (u8(m, 0x0) & 0x10) {
        glGenBuffers(1, (char *)m + 0x78);
        glBindBuffer(0x8892, u32(m, 0x78));
        glBufferData(0x8892, vcount << 4, colArr, 0x88e4);
        i32(m, 0x7c) += (int)(vcount * 0x10);
    }

    glBindBuffer(0x8892, 0);
    glBindBuffer(0x8893, 0);

    if (glGetError() == 0) {
        if (*g_Mesh_keepCpuCopyFlag == 0) {
            freeIf(m, 0x4);
            freeIf(m, 0x8);
            freeIf(m, 0x2c);
            freeIf(m, 0x10);
            freeIf(m, 0xc);
            freeIf(m, 0x14);
            freeIf(m, 0x18);
        }
        u8(m, 0x5c) = 1;
        *g_Mesh_vboByteCounter += i32(m, 0x7c);
        return 1;
    }

    // GL error path: delete whatever buffers were created.
    if (u8(m, 0x5c) != 0) {
        glDeleteBuffers(1, (char *)m + 0x60);
        glDeleteBuffers(1, (char *)m + 0x64);
        unsigned char f = u8(m, 0x0);
        if (f & 2) {
            glDeleteBuffers(1, (char *)m + 0x68);
            f = u8(m, 0x0);
        }
        if (f & 8) {
            glDeleteBuffers(1, (char *)m + 0x6c);
            if (*g_Mesh_tangentDelFlag != 0) {
                glDeleteBuffers(1, (char *)m + 0x70);
                glDeleteBuffers(1, (char *)m + 0x74);
            }
        }
        if (u8(m, 0x0) & 0x10)
            glDeleteBuffers(1, (char *)m + 0x78);
    }
    i32(m, 0x7c) = 0;
    return -1;
}

} // namespace AbyssEngine
