#ifndef WORK_CLASSES_RESOURCEMESH_SRC_CLASS_H
#define WORK_CLASSES_RESOURCEMESH_SRC_CLASS_H

// Galaxy on Fire 2 - AbyssEngine::ResourceMesh.
// Layout (from ctor):
//   +0x00 char*          name   (heap copy of a C-string)
//   +0x04 unsigned short id     (param_2)
//   +0x06 bool           flag   (param_3)
// Access fields via byte-offset casts from `this`.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace AbyssEngine {

struct ResourceMesh {
    ResourceMesh(const char *name, unsigned short id, bool flag);
    ~ResourceMesh();
};

} // namespace AbyssEngine

using ResourceMesh = AbyssEngine::ResourceMesh;

extern "C" {
void *operator_new__(uint32_t size);
void operator_delete__(void *ptr);
void __aeabi_memcpy(void *dst, const void *src, uint32_t n);

// AbyssEngine::String::GetStringLength(char const*) -- strlen.
uint32_t String_GetStringLength(const char *s);
}

#endif
