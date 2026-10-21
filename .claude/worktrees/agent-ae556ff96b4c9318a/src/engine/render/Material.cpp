#include "engine/render/Material.h"

namespace AbyssEngine {

// Default constructor: untextured material with zeroed ambient colour.
Material::Material()
{
    this->blendMode = 0;
    this->arr_2c = new Array<Matrix>();
    this->arr_38 = new Array<Matrix>();
    this->meshes = new Array<Mesh *>();
    this->arr_50 = new Array<uint32_t>();
    this->arr_5c = new Array<Matrix>();

    this->ambientColor.x = Material_defaultVectorX;
    this->ambientColor.y = 0;
    this->ambientColor.z = 0;

    for (int i = 0; i != 8; ++i)
        this->textures[i] = -1;
    this->textures[0] = 0;

    this->addData = 0;
    this->addDataSize = 0;
}

// Clone constructor: copy material state from another material (null builds an empty one).
Material::Material(Material *other)
{
    this->arr_2c = new Array<Matrix>();
    this->arr_38 = new Array<Matrix>();
    this->meshes = new Array<Mesh *>();
    this->arr_50 = new Array<uint32_t>();
    this->arr_5c = new Array<Matrix>();

    this->ambientColor.x = 0;
    this->ambientColor.y = 0;
    this->ambientColor.z = 0;

    if (other == 0) {
        this->blendMode = 0;
        for (int i = 0; i != 8; ++i)
            this->textures[i] = -1;
    } else {
        for (int i = 0; i != 8; ++i)
            this->textures[i] = other->textures[i];
        this->blendMode = other->blendMode;
        uint32_t size = other->addDataSize;
        this->addDataSize = size;
        this->addData = size == 0 ? 0 : new uint8_t[size];
        this->ambientColor = other->ambientColor;
    }
}

Material::~Material()
{
    delete this->arr_5c;
    delete this->arr_50;
    delete this->meshes;
    delete this->arr_38;
    delete this->arr_2c;
}

} // namespace AbyssEngine
