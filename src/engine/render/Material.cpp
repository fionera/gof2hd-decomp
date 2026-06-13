#include "gof2/Material.h"




// ---- _Material_7c8b8.cpp ----
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

// ---- Material_7a3d8.cpp ----
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
    this->f_68 = 0;
    this->f_6c = 0;
    this->f_70 = 0;

    if (other == 0) {
        this->f_20 = 0;
        int zero[3] = {0, 0, 0};
        *(Vector *)vec = *(const Vector *)zero;
        for (int i = 0; i != 8; ++i)
            i32(this, i * 4) = -1;
    } else {
        for (int i = 0; i != 8; ++i)
            i32(this, i * 4) = i32(other, i * 4);
        this->f_20 = other->f_20;
        uint32_t size = other->f_28;
        this->f_28 = size;
        void *buf;
        if (size == 0)
            buf = 0;
        else
            buf = ::operator new[](size | ((int)size >> 31));
        pptr(this, 0x24) = buf;
        *(Vector *)vec = *(const Vector *)((char *)other + 0x68);
    }
}

} // namespace AbyssEngine

// ---- Material_7c188.cpp ----
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
