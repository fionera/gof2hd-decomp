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

// The thin Compare/StrLen/index/ctor_*/addAssign_* adapters below are inlined into their call
// sites in the original binary (no standalone symbol exists). always_inline guarantees the same
// here so they never emit a (weak) out-of-line definition.
#define AESTRING_SHIM inline __attribute__((always_inline))

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
    // Copy constructor: the original inlined it at every call site (no standalone symbol exists),
    // so force-inline here to suppress the out-of-line (weak) variant other TUs would emit.
    AESTRING_SHIM String(const String &other) { s = other.s; }
    // Real engine overloads (recovered from the Android binary): these mangle to the original
    // String::String(...) / operator+= symbols and delegate to the recovered member bodies.
    // Defined out-of-line in String.cpp so the original symbols are emitted from that TU.
    String(const uint16_t *wstr, bool reverse);
    String(const String &other, bool reverse);
    explicit String(char c);
    explicit String(int v);
    explicit String(float v);
    explicit String(long long v);
    // operator=(String const&): recovered engine assignment (re-Sets from the other's wide buffer).
    // Defined out-of-line in String.cpp so the original mangled symbol is emitted from that TU.
    String &operator=(const String &other);
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

    // ---- recovered engine methods (mangle to the original AbyssEngine::String symbols) ----
    // Replace this string's contents from various sources.
    void Set(const char *s);
    void Set(const unsigned short *s);
    void Set(float v);
    void Set(long long v);
    // Length helpers (static): byte length of a char string / code-unit length of a wide string.
    static int StrLen(const char *s);
    static int StrLen(const unsigned short *s);
    static int GetStringLength(const char *s);
    // Compare against another String / an 8-bit string. 0 == equal.
    int Compare(const String &other);
    unsigned int Compare(const char *s);
    // Raw access to the wide backing buffer.
    const unsigned short *GetAEWChar() const;
    operator unsigned short *();
    operator const unsigned short *() const;
    // Bounded element access: pointer to the index-th code unit, or &NUL on out-of-range.
    unsigned short *operator[](int i);
    const unsigned short *operator[](int i) const;

    // ---- methods (converted from String_*/free functions) ----
    // Thin wide-vs-narrow / String-ptr Compare adapters used at various call sites; both forward
    // to the real Compare overloads. Inline so they emit no standalone symbol (the original inlined
    // them at the call site).
    AESTRING_SHIM unsigned int Compare_char(const char *s) { return Compare(s); }
    AESTRING_SHIM int Compare_str(String *other) { return Compare(*other); }
    void ConvertFromUTF8();
    char * GetAEChar() const;
    unsigned int IndexOf(const String &needle);
    unsigned int IndexOf(unsigned int start, const String &needle);
    // First index >= start where needle occurs, or 0xffffffff if not found. Inline (the original
    // inlined this worker into IndexOf / Split / ReplaceString rather than emitting a symbol).
    AESTRING_SHIM unsigned int IndexOf_from(unsigned int start, const String *needle) {
        unsigned int slen = (unsigned int)this->s.size();
        unsigned int nlen = (unsigned int)needle->s.size();
        while (start < slen && nlen <= slen - start) {
            if (needle->s[0] == this->s[start]) {
                unsigned int k = 0;
                while (start + k < slen && this->s[start + k] == needle->s[k]) {
                    if (nlen <= k + 1)
                        return start;
                    k++;
                }
                start += k;
            } else {
                start++;
            }
        }
        return 0xffffffff;
    }
    void PrintOut();
    void ReplaceChar(char from, char to);
    void ReplaceString(String find, String repl);
    void Reverse();
    // Set_* forwarders to the matching Set(...) overload; inline so no standalone symbol is
    // emitted (the original inlined these into their call sites).
    AESTRING_SHIM void Set_char(const char *s)  { Set(s); }
    AESTRING_SHIM void Set_float(float v)        { Set(v); }
    // Forwarder to Set(long long); inline so no standalone symbol is emitted.
    AESTRING_SHIM void Set_longlong(long long v) { Set((long long)v); }
    AESTRING_SHIM void Set_wchar(const uint16_t *s) { Set((const unsigned short *)s); }
    void * Split(String sep);
    void SplitTags(String tag);
    // Length adapters forwarding to the static StrLen overloads; inline (no standalone symbol).
    AESTRING_SHIM int StrLen_char(const char *s) { return StrLen(s); }
    AESTRING_SHIM int StrLen_wchar(const uint16_t *s) { return StrLen((const unsigned short *)s); }
    // out = self[start..end); empty when end <= start. Inline (the original inlined this worker
    // into Split / SplitTags / the public SubString rather than emitting a standalone symbol).
    AESTRING_SHIM void SubString(String *self, unsigned int start, unsigned int end) {
        this->s.clear();
        if (start < end && start < self->s.size()) {
            unsigned int hi = end > self->s.size() ? (unsigned int)self->s.size() : end;
            this->s = self->s.substr(start, hi - start);
        }
    }
    String SubString(unsigned int start, unsigned int end);
    void ToLowerCase();
    void ToUpperCase();
    void Trim();
    int ValueOf();
    // operator+= helpers (one per value type). Each appends the formatted value to this string.
    // Inline: the original inlined these into the operator+= bodies rather than emitting symbols.
    AESTRING_SHIM String * addAssign_char(const char *c) {
        this->s.push_back((char16_t)*(const unsigned char *)c);
        return this;
    }
    AESTRING_SHIM String * addAssign_float(const float *v) {
        String tmp; tmp.ctor_float(*v);
        this->s.append(tmp.s);
        return this;
    }
    AESTRING_SHIM String * addAssign_int(const int *v) {
        long long ext = (long long)*v;
        return addAssign_longlong(&ext);
    }
    AESTRING_SHIM String * addAssign_longlong(const long long *v) {
        String tmp; tmp.ctor_longlong(*v);
        this->s.append(tmp.s);
        return this;
    }
    AESTRING_SHIM String * addAssign_str(String *other) {
        this->s.append(other->s);
        return this;
    }
    // Assign from another String, returning this; inline (the original inlined this into its
    // call sites rather than emitting a standalone symbol).
    AESTRING_SHIM String * assign(String *other) { *this = *other; return this; }
    // ctor helpers (one per value type). Each resets this string and fills it from the argument.
    // Inline: the original inlined these into the matching String(...) constructor bodies.
    AESTRING_SHIM void ctor() { this->s.clear(); }
    AESTRING_SHIM String * ctor_char(const char *s, bool reverse) {
        this->s.clear();
        Set_char(s);
        if (reverse)
            Reverse();
        return this;
    }
    AESTRING_SHIM String * ctor_charval(char c) {
        this->s.clear();
        Set_longlong((long long)c);
        return this;
    }
    AESTRING_SHIM String * ctor_copy(String *other, bool reverse) {
        this->s = other->s;
        if (reverse)
            Reverse();
        return this;
    }
    AESTRING_SHIM String * ctor_float(float v) {
        this->s.clear();
        Set_float(v);
        return this;
    }
    AESTRING_SHIM String * ctor_int(int v) {
        this->s.clear();
        Set_longlong((long long)v);
        return this;
    }
    AESTRING_SHIM String * ctor_longlong(long long v) {
        this->s.clear();
        Set_longlong(v);
        return this;
    }
    AESTRING_SHIM String * ctor_wchar(const uint16_t *s, bool reverse) {
        this->s.clear();
        Set_wchar(s);
        if (reverse)
            Reverse();
        return this;
    }
    // Destructor body / deleting-destructor; inline (the original inlined these into the real
    // ~String() and the delete expression rather than emitting standalone symbols).
    AESTRING_SHIM String * dtor() { this->s.clear(); return this; }
    AESTRING_SHIM void dtor_del() { dtor(); delete this; }
    // Bounded element access; inline (no standalone symbol in the original).
    AESTRING_SHIM uint16_t * index(int i) {
        return reinterpret_cast<uint16_t *>((*this)[i]);
    }
    // const-overload element adapter; inline so no standalone symbol is emitted.
    AESTRING_SHIM uint16_t * index_const(int i) {
        return const_cast<uint16_t *>(reinterpret_cast<const uint16_t *>(
            (*const_cast<const String *>(this))[i]));
    }
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

// Free helper (global namespace): number of UTF-16 code units before the terminating NUL in the
// string's wide backing buffer. Walks GetAEWChar() to the NUL and returns the code-unit count.
inline int GetStringLength(AbyssEngine::String str) {
    const unsigned short *p = str.GetAEWChar();
    while (*p)
        ++p;
    return static_cast<int>(p - str.GetAEWChar());
}

#endif // GOF2_AESTRING_H
