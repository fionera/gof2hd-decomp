#include "gof2/engine/render/ResourceMesh.h"
#include "gof2/game/core/String.h"
#include "gof2/platform/libc.h"

namespace AbyssEngine {

ResourceMesh::ResourceMesh(const char *name, unsigned short id, bool flag)
    : wResourceId(id), flag((uint8_t)flag)
{
    uint32_t len = (uint32_t)String_GetStringLength(name);
    this->name = new char[len + 1];
    memcpy(this->name, name, len + 1);
}

ResourceMesh::~ResourceMesh()
{
    delete[] name;
    name = nullptr;
}

} // namespace AbyssEngine
