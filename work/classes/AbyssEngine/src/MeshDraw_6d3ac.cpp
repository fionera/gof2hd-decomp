#include "class.h"

extern "C" {
void glBindBuffer(unsigned int target, unsigned int buffer);
void glVertexPointer(int size, unsigned int type, int stride, const void *ptr);
void glTexCoordPointer(int size, unsigned int type, int stride, const void *ptr);
void glNormalPointer(unsigned int type, int stride, const void *ptr);
void glColorPointer(int size, unsigned int type, int stride, const void *ptr);
void glDrawElements(unsigned int mode, int count, unsigned int type, const void *indices);
int __aeabi_uidiv(int, int);
}

// AbyssEngine::MeshDraw(AbyssEngine::Engine*, AbyssEngine::Mesh*)
// Fixed-function draw path for a VBO-backed mesh. Binds the position/uv/normal/color buffers
// according to the mesh's vertex-format flag byte (offset 0), issues glDrawElements, and
// accumulates triangle/draw-call statistics into the engine. Falls back to Engine::RenderMesh
// for the shader path. Returns 1 on draw, -4 when the mesh is empty/disabled.
namespace AbyssEngine {

extern "C" char *g_Mesh_shaderPathFlag; // *(DAT_0007d50c + 0x7d3cc)

extern "C" void AE_Engine_AEClientState(Engine *self, unsigned int cap, bool on);
extern "C" void AE_Engine_RenderMesh(Engine *self, Mesh *mesh);

int MeshDraw(Engine *engine, Mesh *mesh)
{
    if (mesh == 0 || s16(mesh, 0x2) == 0 || (u8(mesh, 0x0) & 1) == 0)
        return -4;

    unsigned char flags = u8(mesh, 0x0);
    if (*g_Mesh_shaderPathFlag == 0 && u8(mesh, 0x5c) != 0) {
        glBindBuffer(0x8892, u32(mesh, 0x60));
        AE_Engine_AEClientState(engine, 0x8074, true);
        glVertexPointer(3, 0x1406, 0, 0);
        glBindBuffer(0x8893, u32(mesh, 0x64));

        if (flags & 4) { // (flags<<0x1e) negative -> uv present
            glBindBuffer(0x8892, u32(mesh, 0x68));
            AE_Engine_AEClientState(engine, 0x8078, true);
            glTexCoordPointer(2, 0x1406, 0, 0);
        } else {
            AE_Engine_AEClientState(engine, 0x8078, false);
        }

        if (flags & 8) { // (flags<<0x1d) negative -> normals present
            glBindBuffer(0x8892, u32(mesh, 0x6c));
            AE_Engine_AEClientState(engine, 0x8075, true);
            glNormalPointer(0x1406, 0, 0);
        } else {
            AE_Engine_AEClientState(engine, 0x8075, false);
        }

        if (flags & 0x10) { // (flags<<0x1c) negative -> colors present
            glBindBuffer(0x8892, u32(mesh, 0x78));
            AE_Engine_AEClientState(engine, 0x8076, true);
            glColorPointer(4, 0x1406, 0, 0);
        } else {
            AE_Engine_AEClientState(engine, 0x8076, false);
        }

        glDrawElements(4, (int)(unsigned short)u16(mesh, 0x28), 0x1403, 0);

        if (u8(engine, 0xfe) != 0) {
            int tris = __aeabi_uidiv((int)(unsigned short)u16(mesh, 0x28), 3);
            if (u8(engine, 0xfd) == 0) {
                i32(engine, 0x64) += 1;
                i32(engine, 0x68) += tris;
            } else {
                i32(engine, 0x6c) += tris;
                i32(engine, 0x60) += 1;
            }
        }

        glBindBuffer(0x8892, 0);
        glBindBuffer(0x8893, 0);
    } else {
        AE_Engine_RenderMesh(engine, mesh);
    }
    return 1;
}

} // namespace AbyssEngine
