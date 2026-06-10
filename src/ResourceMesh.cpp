#include "gof2/ResourceMesh.h"




// ---- _ResourceMesh_77ed0.cpp ----
namespace AbyssEngine {

// AbyssEngine::ResourceMesh::~ResourceMesh()
ResourceMesh::~ResourceMesh()
{
    operator_delete__(field_0x0);
    field_0x0 = 0;
}

} // namespace AbyssEngine

// ---- ResourceMesh_112cf0.cpp ----
namespace AbyssEngine {

// AbyssEngine::ResourceMesh::ResourceMesh(char const*, unsigned short, bool)
ResourceMesh::ResourceMesh(const char *name, unsigned short id, bool flag)
{
    this->field_0x6 = (uint8_t)flag;
    this->field_0x4 = id;
    uint32_t len = String_GetStringLength(name);
    void *buf = operator_new__(len + 1U);
    field_0x0 = (char *)buf;
    __aeabi_memcpy(buf, name, len + 1U);
}

} // namespace AbyssEngine
