#ifndef GOF2_CONFIGREADER_H
#define GOF2_CONFIGREADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
inline void *operator new(__SIZE_TYPE__, void *ptr) noexcept { return ptr; }

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

namespace AbyssEngine {

struct Engine;
struct ConfigReader;





template <class T>
void ArrayAdd(T item, Array<T> &array);

struct AEFile {
    static uint32_t OpenRead(String &path, uint32_t *handle);
    static uint32_t Read(char &value, uint32_t handle);
    static void Close(uint32_t handle);
};

typedef void (*ConfigTokenReadFunction)(ConfigReader *, void *);

struct TokenStruct {
    String name;
    ConfigTokenReadFunction read;
    void *context;
};



static inline uint32_t &config_count(ConfigReader *self)
{
    return *(uint32_t *)((char *)self + 0x0);
}

static inline TokenStruct **&config_tokens(ConfigReader *self)
{
    return *(TokenStruct ***)((char *)self + 0x4);
}

static inline Engine *&config_engine(ConfigReader *self)
{
    return *(Engine **)((char *)self + 0xc);
}

static inline uint32_t &config_file_handle(ConfigReader *self)
{
    return *(uint32_t *)((char *)self + 0x10);
}

} // namespace AbyssEngine

struct ConfigReader { void* _opaque; };  // no offset accesses observed
#endif
