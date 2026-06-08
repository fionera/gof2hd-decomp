#include "class.h"

namespace AbyssEngine {

// AbyssEngine::Material::Material() - default constructor.
Material::Material()
{
    i32(this, 0x20) = 0;
    Array_Matrix_ctor((char *)this + 0x2c);
    Array_Matrix_ctor((char *)this + 0x38);
    Array_MeshPtr_ctor((char *)this + 0x44);
    Array_uint_ctor((char *)this + 0x50);
    Array_Matrix_ctor((char *)this + 0x5c);

    f32(this, 0x68) = Material_defaultVectorX;
    i32(this, 0x6c) = 0;
    i32(this, 0x70) = 0;

    for (int i = 0; i != 8; ++i)
        i32(this, i * 4) = -1;

    i32(this, 0x0) = 0;
    i32(this, 0x24) = 0;
    i32(this, 0x28) = 0;
}

} // namespace AbyssEngine
