#ifndef GOF2_AESTRING_H
#define GOF2_AESTRING_H
// NOTE: this file is AEString.h, NOT String.h — a header named String.h collides with the C
// library's <string.h> on case-insensitive filesystems (macOS), breaking libc++'s <cstring>.
//
// AbyssEngine::String — the engine's UTF-16 string (backed by std::u16string), kept as a class
// because it carries 43 game-specific methods (transliteration, Split, ReplaceString, ToLowerCase
// with CP-1252 tables, ...). Method bodies live in src/game/core/String.cpp (and AbyssEngine.cpp
// for operator+).
#include <cstdint>
#include <string>

namespace AbyssEngine {
struct String {
    std::u16string s;
    // Default constructor (recovered): empty string. Delegates to the recovered ctor() body,
    // matching the original String::String() which zero-initializes the contents.
    String() { ctor(); }
    // Convenience C++ constructors/operators layered on the recovered engine methods, so call
    // sites read like ordinary string code. The recovered ctor_*/assign/addAssign_* bodies do the
    // actual work.
    String(const char *cstr, bool reverse = false) { ctor_char(cstr, reverse); }
    String(const String &other) { s = other.s; }
    // Real engine overloads (recovered from the Android binary): these mangle to the original
    // String::String(...) / operator+= symbols and delegate to the recovered member bodies.
    // Defined out-of-line in String.cpp so the original symbols are emitted from that TU.
    String(const uint16_t *wstr, bool reverse);
    String(const String &other, bool reverse);
    explicit String(char c);
    explicit String(int v);
    explicit String(float v);
    explicit String(long long v);
    String &operator=(const String &other) { s = other.s; return *this; }
    String &operator=(const char *cstr) { Set_char(cstr); return *this; }
    String &operator+=(const String &other) { s.append(other.s); return *this; }
    String &operator+=(const char &c);
    String &operator+=(const int &v);
    String &operator+=(const float &v);
    String &operator+=(const long long &v);
    // copy(src, reverse): copy-assign from another String, optionally reversing (RTL languages).
    void copy(const String *src, bool reverse) { ctor_copy(const_cast<String *>(src), reverse); }
    const char16_t* text() const { return s.c_str(); }
    uint32_t        size() const { return (uint32_t)s.size(); }

    // ---- methods (converted from String_*/free functions) ----
    unsigned int Compare_char(const char *s);
    int Compare_str(String *other);
    void ConvertFromUTF8();
    char * GetAEChar() const;
    unsigned int IndexOf(const String &needle);
    unsigned int IndexOf(unsigned int start, const String &needle);
    unsigned int IndexOf_from(unsigned int start, const String *needle);
    void PrintOut();
    void ReplaceChar(char from, char to);
    void ReplaceString(String find, String repl);
    void Reverse();
    void Set_char(const char *s);
    void Set_float(float v);
    void Set_longlong(long long v);
    void Set_wchar(const uint16_t *s);
    void * Split(String sep);
    void SplitTags(String tag);
    int StrLen_char(const char *s);
    int StrLen_wchar(const uint16_t *s);
    void SubString(String *self, unsigned int start, unsigned int end);
    String SubString(unsigned int start, unsigned int end);
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

// AbyssEngine::operator+(String const&, String const&) - concatenate (body in AbyssEngine.cpp).
String operator+(const String &a, const String &b);
// AbyssEngine::operator+(int const&, String const&) - render the int, then append (AbyssEngine.cpp).
String operator+(const int &a, const String &b);
} // namespace AbyssEngine

using AbyssEngine::String;

#endif // GOF2_AESTRING_H
