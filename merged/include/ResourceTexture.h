#ifndef WORK_CLASSES_RESOURCETEXTURE_SRC_CLASS_H
#define WORK_CLASSES_RESOURCETEXTURE_SRC_CLASS_H

// Galaxy on Fire 2 - AbyssEngine::ResourceTexture.
// Layout (from ctors):
//   +0x00 char*  name   (heap copy of a C-string)
//   +0x04 float  value  (param_2)
// Access fields via byte-offset casts from `this`.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

namespace AbyssEngine {

struct String {
    uint16_t *text;     // +0x00
    uint32_t field_04;  // +0x04
    uint32_t size;      // +0x08 char count
};

struct ResourceTexture {
    ResourceTexture(const char *name, float value);
    ResourceTexture(const String &name, float value);
    ~ResourceTexture();
};

} // namespace AbyssEngine

using String = AbyssEngine::String;
using ResourceTexture = AbyssEngine::ResourceTexture;

extern "C" {
void *operator_new__(uint32_t size);
void operator_delete__(void *ptr);
void __aeabi_memcpy(void *dst, const void *src, uint32_t n);

// AbyssEngine::String::GetStringLength(char const*) -- strlen.
uint32_t String_GetStringLength(const char *s);
// AbyssEngine::String::GetAEChar() const -- returns a freshly allocated UTF-8 C-string.
void *String_GetAEChar(const String *self);
}

#endif
