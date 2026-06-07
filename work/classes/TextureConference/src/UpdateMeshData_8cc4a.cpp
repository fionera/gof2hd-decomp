#include "class.h"

extern "C" long long _ZN11AbyssEngine18ApplicationManager20GetElapsedTimeMillisEv(void *self);
extern "C" long long __divdi3_like(int a_lo, int a_hi, int b_lo, int b_hi);

namespace AbyssEngine {

void TextureConference::UpdateMeshData(Mesh *mesh, Engine *engine)
{
    glUniformMatrix4fv(field_i32(this, 0x2c), 1, 0, (char *)engine + 0x104);
    if (field_u8(this, 0x9) != 0) {
        glUniform4fv(field_i32(this, 0x30), 1, (float *)((char *)engine + 0xd0));
        field_u8(this, 0x9) = 0;
    }

    long long elapsed = _ZN11AbyssEngine18ApplicationManager20GetElapsedTimeMillisEv(field_ptr(engine, 0x30));
    long long t = __divdi3_like((int)elapsed, (int)((unsigned long long)elapsed >> 32), 5, 0);
    t = t + *(long long *)((char *)this + 0x38);
    long long clamped = (0xe10 > t) ? t : (t - 0xe10);
    *(long long *)((char *)this + 0x38) = clamped;
    glUniform1i(field_i32(this, 0x28), (int)clamped);

    glEnableVertexAttribArray(field_i32(this, 0x20));
    glEnableVertexAttribArray(field_i32(this, 0x24));
    glVertexAttribPointer(field_i32(this, 0x20), 3, 0x1406, 0, 0, field_ptr(mesh, 0x4));
    if ((field_u8(mesh, 0x0) & 2) != 0)
        glVertexAttribPointer(field_i32(this, 0x24), 2, 0x1406, 0, 0, field_ptr(mesh, 0x8));
}

} // namespace AbyssEngine
