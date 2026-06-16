// gof2/common.h — unified shared types for the native (macOS) build of Galaxy on Fire 2.
//
// The byte-matching decomp redefined containers/math in every per-class header. For ONE coherent
// program they must be defined exactly once. We are NOT byte-matching, so we use natural 64-bit
// layout and the C++ standard library where the engine type IS a standard type:
//   * AbyssEngine::Array<T>   ==  std::vector<T>      (dynamic array: size/data/capacity)
//   * AbyssEngine::String     ==  UTF-16 string backed by std::u16string (+ game-specific methods)
//   * Vector/Matrix/Quaternion ARE 3D MATH (not std::vector) -> custom, see gof2/math.h
#ifndef GOF2_COMMON_H
#define GOF2_COMMON_H
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <new>          // global operator new/delete (replaces the decomp's hand-written redeclarations)
#include "mathtypes.h"

// ---- Array<T> == std::vector<T> -------------------------------------------------------------
// The recovered code used `Array<T>` with public {size,data,capacity}. We alias it to std::vector
// and provide the engine's container free-functions over it, so call sites (ArrayAdd/ArraySetLength/
// ArrayRemoveAll) are unchanged. Raw field reads (`a->data`, `a->size`) are rewritten to a.data()/
// a.size() during the merge.
template <class T> using Array = std::vector<T>;

template <class T> inline void ArrayAdd(T item, std::vector<T>& a)            { a.push_back(item); }
template <class T> inline void ArraySetLength(unsigned int n, std::vector<T>& a){ a.resize(n); }
template <class T> inline void ArrayRemoveAll(std::vector<T>& a)              { a.clear(); }

// AELabelObject(glIdentifier, name, label): in the shipped binary this is the GLES debug-object
// labelling stub (KHR_debug). On the production build it is compiled out to a no-op that simply
// returns its first argument, so callers stay byte-identical without any GL side effect.
unsigned int AELabelObject(unsigned int glIdentifier, unsigned int name, const char *label);

// ---- String == game UTF-16 string (backed by std::u16string) -------------------------------
// Kept as a class because the engine's String has 43 game-specific methods (transliteration, Split,
// ReplaceString, ToLowerCase with CP-1252 tables, ...). Storage is std::u16string; method bodies
// come from the merged native/src/String.cpp.
namespace AbyssEngine {
struct String {
    std::u16string s;
    String() {}
    // Convenience C++ constructors/operators layered on the recovered engine methods, so call
    // sites read like ordinary string code. The recovered ctor_*/assign/addAssign_* bodies in
    // src/String.cpp do the actual work.
    String(const char *cstr, bool reverse = false) { ctor_char(cstr, reverse); }
    String(const String &other) { s = other.s; }
    String &operator=(const String &other) { s = other.s; return *this; }
    String &operator=(const char *cstr) { Set_char(cstr); return *this; }
    String &operator+=(const String &other) { s.append(other.s); return *this; }
    // copy(src, reverse): copy-assign from another String, optionally reversing (RTL languages).
    void copy(const String *src, bool reverse) { ctor_copy(const_cast<String *>(src), reverse); }
    // (method declarations are folded in from the recovered String header during the merge)
    const char16_t* text() const { return s.c_str(); }
    uint32_t        size() const { return (uint32_t)s.size(); }

    // ---- methods (converted from String_*/free functions) ----
    unsigned int Compare_char(const char *s);
    int Compare_str(String *other);
    void ConvertFromUTF8();
    char * GetAEChar();
    unsigned int IndexOf(String *needle);
    unsigned int IndexOf_from(unsigned int start, String *needle);
    void PrintOut();
    void ReplaceChar(char from, char to);
    void ReplaceString(String *find, String *repl);
    void Reverse();
    void Set_char(const char *s);
    void Set_float(float v);
    void Set_longlong(long long v);
    void Set_wchar(const uint16_t *s);
    void * Split(String *sep);
    void SplitTags(String *tag);
    int StrLen_char(const char *s);
    int StrLen_wchar(const uint16_t *s);
    void SubString(String *self, unsigned int start, unsigned int end);
    void ToLowerCase();
    void ToUpperCase();
    void Trim();
    int ValueOf();
    String * addAssign_char(const char *c);
    String * addAssign_float(const float *v);
    String * addAssign_int(const int *v);
    String * addAssign_longlong(const long long *v);
    String * addAssign_str(String *other);
    String * assign(String *other);
    void ctor();
    String * ctor_char(const char *s, bool reverse);
    String * ctor_charval(char c);
    String * ctor_copy(String *other, bool reverse);
    String * ctor_float(float v);
    String * ctor_int(int v);
    String * ctor_longlong(long long v);
    String * ctor_wchar(const uint16_t *s, bool reverse);
    String * dtor();
    void dtor_del();
    uint16_t * index(int i);
    uint16_t * index_const(int i);
    // Decode `len` UTF-8 bytes into a freshly allocated NUL-terminated wide buffer (caller frees
    // with operator delete[]); transliterates Cyrillic to Latin approximations.
    static uint16_t * getWCharFromUtf8(char *utf8, int len);
};

// AbyssEngine::operator+(String const&, String const&) - concatenate (body in src/AbyssEngine.cpp).
String operator+(const String &a, const String &b);
} // namespace AbyssEngine

using AbyssEngine::String;

// ---- global aliases so recovered code's short type names resolve --------------------------
namespace AEMath = AbyssEngine::AEMath;   // so global `AEMath::Vector` resolves
using AbyssEngine::AEMath::Vector;        // recovered structs use bare Vector/Matrix
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::Quaternion;
typedef unsigned char  byte;
typedef unsigned int   uint;
typedef unsigned short ushort;
struct u32x4 { uint32_t v[4]; };          // NEON 4x u32 (best-effort, non-matching)

// ---- GOF2 byte-offset accessor helpers (single definition; were duplicated per header) ----
static inline bool&           BL(void* p, int off) { return *(bool*)((char*)p + off); }
static inline char&           C (void* p, int off) { return *(char*)((char*)p + off); }
static inline double&         D (void* p, int off) { return *(double*)((char*)p + off); }
template <class T> static inline T &F(void *p, uint32_t off) {
    return *(T *)((char *)p + off);
}
static inline float &FL(void *self, uint32_t off)
{
    return *(float *)((char *)self + off);
}
template <class T> static inline T &G(void *p, uint32_t off) { return *(T *)((char *)p + off); }
static inline short&          S (void* p, int off) { return *(short*)((char*)p + off); }
static inline uint32_t &U(void *self, uint32_t off)
{
    return *(uint32_t *)((char *)self + off);
}
static inline uint8_t &UC(void *self, int offset) { return *(uint8_t *)((char *)self + offset); }
static inline uint16_t &US(void *self, int offset) { return *(uint16_t *)((char *)self + offset); }
static inline float &f32(void *self, unsigned int offset)
{
    return *(float *)((char *)self + offset);
}
template <class T>
static inline T &field(void *self, uint32_t off)
{
    return *(T *)((char *)self + off);
}
static inline float &field_f32(void *self, uint32_t offset)
{
    return *(float *)((char *)self + offset);
}
static inline int &field_i32(void *self, uint32_t offset)
{
    return *(int *)((char *)self + offset);
}
static inline void *field_ptr(void *self, uint32_t offset)
{
    return *(void **)((char *)self + offset);
}
static inline uint8_t &field_u8(void *self, uint32_t offset)
{
    return *(uint8_t *)((char *)self + offset);
}
static inline short    &i16(void *self, uint32_t off) { return *(short    *)((char *)self + off); }
static inline int &i32(void *self, unsigned int offset)
{
    return *(int *)((char *)self + offset);
}
static inline void    *&pp (void *self, uint32_t off) { return *(void    **)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t offset)
{
    return *(uint32_t *)((char *)self + offset);
}
static inline uint8_t &u8(void *self, unsigned int offset)
{
    return *(uint8_t *)((char *)self + offset);
}

#endif // GOF2_COMMON_H
