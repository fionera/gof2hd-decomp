#ifndef GOF2_AESTRING_H
#define GOF2_AESTRING_H

#include <cstdint>

#define AESTRING_SHIM inline __attribute__((always_inline))

namespace AbyssEngine {
    class String {
    public:
        // vptr lives at +0x00 (implicit, from the virtual destructor below).
        unsigned short *data; // +0x04  heap UTF-16 buffer, NUL-terminated (or null when empty)
        int length; // +0x08  code-unit count (excludes the terminating NUL)

        // Default constructor: empty (null buffer), exactly as String::String() (0x72398). Out-of-line
        // in String.cpp so the engine's standalone C1Ev/C2Ev symbols are emitted (the original ships
        // them; the compiler also plants the vptr here).
        String();

        // Convenience C++ constructor; out-of-line in String.cpp so the original C1EPKcb/C2EPKcb symbols
        // are emitted. The recovered Set/Reverse bodies do the actual work.
        String(const char *cstr, bool reverse = false);

        // Copy constructor: the original inlines a deep copy (data=0; Set(other.data)) at every call site
        // (no standalone symbol exists), so force-inline here to suppress the out-of-line (weak) variant.
        AESTRING_SHIM String(const String &other) : data(nullptr), length(0) { Set(other.data); }
        // Real engine overloads (recovered from the Android binary): these mangle to the original
        // String::String(...) symbols and are defined out-of-line in String.cpp.
        String(const uint16_t *wstr, bool reverse);

        String(const String &other, bool reverse);

        explicit String(char c);

        explicit String(int v);

        explicit String(float v);

        explicit String(long long v);

        // Destructor: the sole virtual. Frees `data`. Out-of-line in String.cpp so the engine's
        // standalone D0/D1/D2 + vtable/typeinfo are emitted, byte-matching the binary.
        virtual ~String();

        // operator=(String const&): recovered engine assignment (re-Sets from the other's wide buffer
        // only when it is non-empty — matches the original 0x727a8). Out-of-line in String.cpp.
        String &operator=(const String &other);

        String &operator=(const char *cstr) {
            Set(cstr);
            return *this;
        }

        String &operator=(const char16_t *wstr) {
            Set(reinterpret_cast<const unsigned short *>(wstr));
            return *this;
        }

        // Append a single UTF-16 code unit (convenience for char-by-char builders).
        AESTRING_SHIM void push_back(unsigned short c) { appendWide(&c, 1); }

        String &operator+=(const String &other) {
            appendWide(other.data, other.length);
            return *this;
        }

        String &operator+=(const char &c);

        String &operator+=(const int &v);

        String &operator+=(const float &v);

        String &operator+=(const long long &v);

        // copy(src, reverse): copy-assign from another String, optionally reversing (RTL languages).
        void copy(const String *src, bool reverse) { ctor_copy(const_cast<String *>(src), reverse); }
        const char16_t *text() const { return reinterpret_cast<const char16_t *>(data); }
        uint32_t size() const { return (uint32_t) length; }

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

        char *GetAEChar() const;

        unsigned int IndexOf(const String &needle);

        unsigned int IndexOf(unsigned int start, const String &needle);

        // First index >= start where needle occurs, or 0xffffffff if not found. Inline (the original
        // inlined this worker into IndexOf / Split / ReplaceString rather than emitting a symbol).
        AESTRING_SHIM unsigned int IndexOf_from(unsigned int start, const String *needle) {
            unsigned int slen = (unsigned int) this->length;
            unsigned int nlen = (unsigned int) needle->length;
            while (start < slen && nlen <= slen - start) {
                if (needle->data[0] == this->data[start]) {
                    unsigned int k = 0;
                    while (start + k < slen && this->data[start + k] == needle->data[k]) {
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
        AESTRING_SHIM void Set_char(const char *s) { Set(s); }
        AESTRING_SHIM void Set_float(float v) { Set(v); }
        // Forwarder to Set(long long); inline so no standalone symbol is emitted.
        AESTRING_SHIM void Set_longlong(long long v) { Set((long long) v); }
        AESTRING_SHIM void Set_wchar(const uint16_t *s) { Set((const unsigned short *) s); }

        void *Split(String sep);

        void SplitTags(String tag);

        // Length adapters forwarding to the static StrLen overloads; inline (no standalone symbol).
        AESTRING_SHIM int StrLen_char(const char *s) { return StrLen(s); }
        AESTRING_SHIM int StrLen_wchar(const uint16_t *s) { return StrLen((const unsigned short *) s); }
        // out = self[start..end); empty when end <= start. Inline (the original inlined this worker
        // into Split / SplitTags / the public SubString rather than emitting a standalone symbol).
        AESTRING_SHIM void SubString(String *self, unsigned int start, unsigned int end) {
            clear();
            unsigned int slen = (unsigned int) self->length;
            if (start < end && start < slen) {
                unsigned int hi = end > slen ? slen : end;
                assignWide(self->data + start, (int) (hi - start));
            }
        }

        String SubString(unsigned int start, unsigned int end);

        void ToLowerCase();

        void ToUpperCase();

        void Trim();

        int ValueOf();

        // operator+= helpers (one per value type). Each appends the formatted value to this string.
        // Inline: the original inlined these into the operator+= bodies rather than emitting symbols.
        AESTRING_SHIM String *addAssign_char(const char *c) {
            unsigned short ch = (unsigned short) *(const unsigned char *) c;
            appendWide(&ch, 1);
            return this;
        }

        AESTRING_SHIM String *addAssign_float(const float *v) {
            String tmp;
            tmp.ctor_float(*v);
            appendWide(tmp.data, tmp.length);
            return this;
        }

        AESTRING_SHIM String *addAssign_int(const int *v) {
            long long ext = (long long) *v;
            return addAssign_longlong(&ext);
        }

        AESTRING_SHIM String *addAssign_longlong(const long long *v) {
            String tmp;
            tmp.ctor_longlong(*v);
            appendWide(tmp.data, tmp.length);
            return this;
        }

        AESTRING_SHIM String *addAssign_str(String *other) {
            appendWide(other->data, other->length);
            return this;
        }

        // Assign from another String, returning this; inline (the original inlined this into its
        // call sites rather than emitting a standalone symbol).
        AESTRING_SHIM String *assign(String *other) {
            *this = *other;
            return this;
        }

        // ctor helpers (one per value type). Each resets this string and fills it from the argument.
        // Inline: the original inlined these into the matching String(...) constructor bodies.
        AESTRING_SHIM void ctor() { clear(); }
        AESTRING_SHIM String *ctor_char(const char *s, bool reverse) {
            Set_char(s);
            if (reverse)
                Reverse();
            return this;
        }

        AESTRING_SHIM String *ctor_charval(char c) {
            Set_longlong((long long) c);
            return this;
        }

        AESTRING_SHIM String *ctor_copy(String *other, bool reverse) {
            Set(other->data);
            if (reverse)
                Reverse();
            return this;
        }

        AESTRING_SHIM String *ctor_float(float v) {
            Set_float(v);
            return this;
        }

        AESTRING_SHIM String *ctor_int(int v) {
            Set_longlong((long long) v);
            return this;
        }

        AESTRING_SHIM String *ctor_longlong(long long v) {
            Set_longlong(v);
            return this;
        }

        AESTRING_SHIM String *ctor_wchar(const uint16_t *s, bool reverse) {
            Set_wchar(s);
            if (reverse)
                Reverse();
            return this;
        }

        // Bounded element access; inline (no standalone symbol in the original).
        AESTRING_SHIM uint16_t *index(int i) {
            return reinterpret_cast<uint16_t *>((*this)[i]);
        }

        // const-overload element adapter; inline so no standalone symbol is emitted.
        AESTRING_SHIM uint16_t *index_const(int i) {
            return const_cast<uint16_t *>(reinterpret_cast<const uint16_t *>(
                (*const_cast<const String *>(this))[i]));
        }

        // Decode `len` UTF-8 bytes into a freshly allocated NUL-terminated wide buffer (caller frees
        // with operator delete[]); transliterates Cyrillic to Latin approximations.
        static uint16_t *getWCharFromUtf8(char *utf8, int len);

        // ---- native buffer helpers (manual UTF-16 storage; not original symbols, force-inlined) ----
        // Release the heap buffer and reset to the empty (null-buffer) state, as the dtor / Set do.
        AESTRING_SHIM void clear() {
            if (data) delete[] data;
            data = nullptr;
            length = 0;
        }

        // Replace the contents with a copy of `n` code units from `src` (NUL-terminated allocation of
        // n+1), matching the engine's Set buffer convention. `src==nullptr`/`n<=0` -> empty.
        AESTRING_SHIM void assignWide(const unsigned short *src, int n) {
            if (data) delete[] data;
            data = nullptr;
            length = 0;
            if (!src || n < 0) return;
            unsigned short *nd = new unsigned short[n + 1];
            for (int i = 0; i < n; i++) nd[i] = src[i];
            nd[n] = 0;
            data = nd;
            length = n;
        }

        // Append `n` code units from `src`, reallocating the NUL-terminated buffer.
        AESTRING_SHIM void appendWide(const unsigned short *src, int n) {
            if (!src || n <= 0) return;
            int newLen = length + n;
            unsigned short *nd = new unsigned short[newLen + 1];
            for (int i = 0; i < length; i++) nd[i] = data[i];
            for (int i = 0; i < n; i++) nd[length + i] = src[i];
            nd[newLen] = 0;
            if (data) delete[] data;
            data = nd;
            length = newLen;
        }
    };

    String operator+(const String &a, const String &b);

    String operator+(const int &a, const String &b);
} // namespace AbyssEngine

using AbyssEngine::String;

int GetStringLength(AbyssEngine::String str);

#endif // GOF2_AESTRING_H
