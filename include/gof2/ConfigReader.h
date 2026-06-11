#ifndef GOF2_CONFIGREADER_H
#define GOF2_CONFIGREADER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
#include <new>
void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

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

// --- thin String helpers (the shared common.h String is minimal; the engine's
//     game-specific String methods are not modeled in this build) ---
inline String StringFromAscii(const char *cstr) {
    String r;
    for (const char *p = cstr; p && *p; ++p) r.s.push_back((char16_t)(unsigned char)*p);
    return r;
}
inline void StringAppendChar(String &str, char c) { str.s.push_back((char16_t)(unsigned char)c); }
inline void StringTrim(String &str) {
    size_t b = str.s.find_first_not_of((char16_t)' ');
    size_t e = str.s.find_last_not_of((char16_t)' ');
    if (b == std::u16string::npos) { str.s.clear(); return; }
    str.s = str.s.substr(b, e - b + 1);
}
inline int32_t StringIndexOf(const String &str, const String &needle) {
    size_t p = str.s.find(needle.s);
    return p == std::u16string::npos ? -1 : (int32_t)p;
}
inline String StringSubString(const String &str, uint32_t start, uint32_t len) {
    String r; r.s = str.s.substr(start, len); return r;
}
inline int32_t StringCompareAscii(const String &str, const char *cstr) {
    return str.s == StringFromAscii(cstr).s ? 0 : 1;
}
inline void StringSetAscii(String &str, const char *cstr) { str.s = StringFromAscii(cstr).s; }
inline uint16_t *StringCharAt(String &str, uint32_t idx) { return (uint16_t *)&str.s[idx]; }

typedef void (*ConfigTokenReadFunction)(ConfigReader *, void *);

struct TokenStruct {
    String name;
    ConfigTokenReadFunction read;
    void *context;
};

class ConfigReader {
public:
    Array<TokenStruct *> tokens; // 0x0  { count@0x0, data@0x4, cap@0x8 }
    Engine *engine;              // 0xc
    uint32_t file_handle;        // 0x10

    ConfigReader(Engine *engine);
    ~ConfigReader();

    void RegisterTokenReadFunction(String name, ConfigTokenReadFunction read, void *context);
    String GetNewLine();
    void ParseFile(String name);
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
#endif
