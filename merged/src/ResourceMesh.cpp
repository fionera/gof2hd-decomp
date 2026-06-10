#include "ResourceMesh.h"




// ---- _ResourceMesh_77ed0.cpp ----
namespace AbyssEngine {

// AbyssEngine::ResourceMesh::~ResourceMesh()
ResourceMesh::~ResourceMesh()
{
    operator_delete__(*(void **)this);
    *(void **)this = 0;
}

} // namespace AbyssEngine

// ---- ResourceMesh_112cf0.cpp ----
namespace AbyssEngine {

// AbyssEngine::ResourceMesh::ResourceMesh(char const*, unsigned short, bool)
ResourceMesh::ResourceMesh(const char *name, unsigned short id, bool flag)
{
    *(uint8_t *)((char *)this + 0x6) = (uint8_t)flag;
    *(uint16_t *)((char *)this + 0x4) = id;
    uint32_t len = String_GetStringLength(name);
    void *buf = operator_new__(len + 1U);
    *(void **)this = buf;
    __aeabi_memcpy(buf, name, len + 1U);
}

} // namespace AbyssEngine
