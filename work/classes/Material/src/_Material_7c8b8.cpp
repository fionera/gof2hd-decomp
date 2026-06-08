#include "class.h"

namespace AbyssEngine {

// AbyssEngine::Material::~Material()
Material::~Material()
{
    Array_Matrix_dtor((char *)this + 0x5c);
    Array_uint_dtor((char *)this + 0x50);
    Array_MeshPtr_dtor((char *)this + 0x44);
    Array_Matrix_dtor((char *)this + 0x38);
    Array_Matrix_dtor((char *)this + 0x2c);
}

} // namespace AbyssEngine
