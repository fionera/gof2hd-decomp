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

    this->vec_68.x = 0;
    this->vec_68.y = 0;
    this->vec_68.z = 0;

    if (other == 0) {
        this->f_20 = 0;
        for (int i = 0; i != 8; ++i)
            this->textures[i] = -1;
    } else {
        for (int i = 0; i != 8; ++i)
            this->textures[i] = other->textures[i];
        this->f_20 = other->f_20;
        uint32_t size = other->f_28;
        this->f_28 = size;
        void *buf;
        if (size == 0)
            buf = 0;
        else
            buf = ::operator new[](size | ((int)size >> 31));
        this->f_24 = buf;
        this->vec_68 = other->vec_68;
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

    this->vec_68.x = Material_defaultVectorX;
    this->vec_68.y = 0;
    this->vec_68.z = 0;

    for (int i = 0; i != 8; ++i)
        this->textures[i] = -1;

    this->textures[0] = 0;
    this->f_24 = 0;
    this->f_28 = 0;
}

} // namespace AbyssEngine
