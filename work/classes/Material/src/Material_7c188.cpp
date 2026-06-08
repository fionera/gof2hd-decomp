#include "class.h"

namespace AbyssEngine {

// AbyssEngine::Material::Material() - default constructor.
Material::Material()
{
    this->f_20 = 0;
    Array_Matrix_ctor((char *)this + 0x2c);
    Array_Matrix_ctor((char *)this + 0x38);
    Array_MeshPtr_ctor((char *)this + 0x44);
    Array_uint_ctor((char *)this + 0x50);
    Array_Matrix_ctor((char *)this + 0x5c);

    this->f_68 = Material_defaultVectorX;
    this->f_6c = 0;
    this->f_70 = 0;

    for (int i = 0; i != 8; ++i)
        i32(this, i * 4) = -1;

    this->f_0 = 0;
    this->f_24 = 0;
    this->f_28 = 0;
}

} // namespace AbyssEngine
