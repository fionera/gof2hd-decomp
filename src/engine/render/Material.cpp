#include "gof2/engine/render/Material.h"

namespace AbyssEngine {

// AbyssEngine::Material::~Material()
Material::~Material()
{
    delete this->arr_5c;
    delete this->arr_50;
    delete this->meshes;
    delete this->arr_38;
    delete this->arr_2c;
}

} // namespace AbyssEngine

namespace AbyssEngine {

// AbyssEngine::Material::Material(AbyssEngine::Material*) - copy/clone constructor.
Material::Material(Material *other)
{
    this->arr_2c = new Array<Matrix>();
    this->arr_38 = new Array<Matrix>();
    this->meshes = new Array<Mesh *>();
    this->arr_50 = new Array<uint32_t>();
    this->arr_5c = new Array<Matrix>();

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

namespace AbyssEngine {

// AbyssEngine::Material::Material() - default constructor.
Material::Material()
{
    this->f_20 = 0;
    this->arr_2c = new Array<Matrix>();
    this->arr_38 = new Array<Matrix>();
    this->meshes = new Array<Mesh *>();
    this->arr_50 = new Array<uint32_t>();
    this->arr_5c = new Array<Matrix>();

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
