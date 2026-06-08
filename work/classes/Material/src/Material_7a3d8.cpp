#include "class.h"

namespace AbyssEngine {

// AbyssEngine::Material::Material(AbyssEngine::Material*) - copy/clone constructor.
Material::Material(Material *other)
{
    Array_Matrix_ctor((char *)this + 0x2c);
    Array_Matrix_ctor((char *)this + 0x38);
    Array_MeshPtr_ctor((char *)this + 0x44);
    Array_uint_ctor((char *)this + 0x50);
    Array_Matrix_ctor((char *)this + 0x5c);

    Vector *vec = (Vector *)((char *)this + 0x68);
    i32(this, 0x68) = 0;
    i32(this, 0x6c) = 0;
    i32(this, 0x70) = 0;

    if (other == 0) {
        i32(this, 0x20) = 0;
        int zero[3] = {0, 0, 0};
        Vector_assign(vec, (Vector *)zero);
        for (int i = 0; i != 8; ++i)
            i32(this, i * 4) = -1;
    } else {
        for (int i = 0; i != 8; ++i)
            i32(this, i * 4) = i32(other, i * 4);
        i32(this, 0x20) = i32(other, 0x20);
        uint32_t size = u32(other, 0x28);
        u32(this, 0x28) = size;
        void *buf;
        if (size == 0)
            buf = 0;
        else
            buf = operator_new_array(size | ((int)size >> 31));
        pptr(this, 0x24) = buf;
        Vector_assign(vec, (Vector *)((char *)other + 0x68));
    }
}

} // namespace AbyssEngine
