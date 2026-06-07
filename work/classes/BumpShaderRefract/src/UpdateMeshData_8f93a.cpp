#include "class.h"

extern "C" void glActiveTexture(uint32_t texture);
extern "C" void glUniform2f(int location, float x, float y);
extern "C" int _ZN11AbyssEngine6Engine14GetDisplayWidthEv(AbyssEngine::Engine *self);
extern "C" int _ZN11AbyssEngine6Engine15GetDisplayHeightEv(AbyssEngine::Engine *self);
extern "C" void _ZN11AbyssEngine6Engine16ActivateRefractFBOEv(AbyssEngine::Engine *self);

namespace AbyssEngine {

void BumpShaderRefract::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    if (field_i32(this, 0x2c) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x2c), 1, 0, (char *)engine + 0x104);
    if (field_i32(this, 0x40) >= 0)
        glUniformMatrix4fv(field_i32(this, 0x40), 1, 0, (char *)engine + 0x1c4);

    if (field_u8(this, 0x9) != 0) {
        if (field_i32(this, 0x44) >= 0)
            glUniform1i(field_i32(this, 0x44), field_u8(mesh, 0x85));
        glUniform4fv(field_i32(this, 0x48), 1, (float *)((char *)engine + 0xd0));
        int loc = field_i32(this, 0x38);
        if (loc >= 0) {
            float w = (float)_ZN11AbyssEngine6Engine14GetDisplayWidthEv(engine);
            float h = (float)_ZN11AbyssEngine6Engine15GetDisplayHeightEv(engine);
            glUniform2f(loc, 1.0f / w, 1.0f / h);
        }
        glActiveTexture(0x84c7);
        _ZN11AbyssEngine6Engine16ActivateRefractFBOEv(engine);
        field_u8(this, 0x9) = 0;
    }

    if (field_i32(this, 0x20) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x20));
    if (field_i32(this, 0x24) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x24));
    if (field_i32(this, 0x28) >= 0)
        glEnableVertexAttribArray(field_i32(this, 0x28));

    int loc0 = field_i32(this, 0x20);
    const void *ptr;
    int last;
    if (field_u8(mesh, 0x5c) == 0) {
        if (loc0 >= 0)
            glVertexAttribPointer(loc0, 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
        if (field_i32(this, 0x24) >= 0)
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
        last = field_i32(this, 0x28);
        if (last < 0)
            return;
        ptr = field_ptr(mesh, 0xc);
    } else {
        if (loc0 >= 0) {
            glBindBuffer(0x8892, field_i32(mesh, 0x60));
            glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, 0);
        }
        if (field_i32(this, 0x24) >= 0) {
            glBindBuffer(0x8892, field_i32(mesh, 0x68));
            glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, 0);
        }
        if (field_i32(this, 0x28) < 0)
            return;
        glBindBuffer(0x8892, field_i32(mesh, 0x78));
        ptr = 0;
        last = field_i32(this, 0x28);
    }
    glVertexAttribPointer(last, 4, 0x1406, 0, 0, ptr);
}

} // namespace AbyssEngine
