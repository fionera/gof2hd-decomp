#include "gof2/String.h"

// AbyssEngine::String, native re-expression.
// The canonical type is common.h's `AbyssEngine::String { std::u16string s; }`. Every method
// below operates on that std::u16string member instead of the original hand-managed
// {vtable, ushort* data, uint length} layout. Behaviour (CP-1252 case tables, Cyrillic
// transliteration, Compare sentinels, Split/SplitTags semantics) is preserved.

extern "C" {
// External (non-String) helpers kept as opaque imports.
int   GameText_getLanguage();                              // 0x6f544
int   atoi(const char *s);                                 // libc
void  String_printImpl(const char *s);                     // 0x1ab108 platform print
uint16_t *String_computeFloatString(float v, int base, int *outExp, int *outNeg);
void  operator_delete__(void *p);
void *operator_new__(unsigned int n);
void *operator_new(unsigned int n);
void  operator_delete(void *p);

// Array<String*> container helpers used by Split / SplitTags.
void  String_ArrayString_ctor(void *arr);                  // Array<String*>::Array()
void  String_ArrayString_add(void *item, void *arr);       // ArrayAdd<String*>
void  String_ArrayString_removeAll(void *arr);             // ArrayRemoveAll<String*>
void *String_ArrayString_dtor(void *arr);                  // Array<String*>::~Array()

// operator+(String const&, String const&) free helper (implemented elsewhere).
void  String_concat(String *out, String *a, String *b);
// operator=(String const&) used by some call sites under a distinct symbol.
void  String_assign_op(String *self, String *src);
}

// ---- GetAEChar_72752.cpp ----
// AbyssEngine::String::GetAEChar() const - allocate an 8-bit (low-byte) copy of the wide buffer.
extern "C" char *String_GetAEChar(String *self)
{
    unsigned int len = self->s.size();
    unsigned int n = len + 1;
    char *out = (char *)operator_new(n);
    for (unsigned int i = 0; i < len; i++)
        out[i] = (char)(self->s[i] & 0xff);
    out[len] = 0;
    return out;
}

// ---- ReplaceString_73530.cpp ----
// AbyssEngine::String::ReplaceString(find, repl) - replace each occurrence of `find` with `repl`.
extern "C" void String_ReplaceString(String *self, String *find, String *repl)
{
    if (self->s.empty() || find->s.empty())
        return;

    std::u16string acc;
    unsigned int pos = 0;
    int idx;
    while ((idx = (int)String_IndexOf_from(self, pos, find)) != -1) {
        acc.append(self->s, pos, (unsigned int)idx - pos);
        acc.append(repl->s);
        pos = (unsigned int)find->s.size() + idx;
    }

    if (pos != 0 && pos < self->s.size())
        acc.append(self->s, pos, self->s.size() - pos);

    if (!acc.empty())
        self->s = acc;
}

// ---- ReplaceChar_73070.cpp ----
// AbyssEngine::String::ReplaceChar(char from, char to) - replace every matching code unit.
extern "C" void String_ReplaceChar(String *self, char from, char to)
{
    for (size_t i = 0; i < self->s.size(); i++)
        if ((unsigned int)self->s[i] == (unsigned int)(int)from)
            self->s[i] = (char16_t)(short)to;
}

// ---- String_6e484.cpp ----
// AbyssEngine::String::String(char) - build a string from a single character's numeric value.
extern "C" String *String_ctor_charval(String *self, char c)
{
    self->s.clear();
    String_Set_longlong(self, (long long)c);
    return self;
}

// ---- operator___72958.cpp ----
// AbyssEngine::String::operator+=(float const&) - append the formatted form of a float.
extern "C" String *String_addAssign_float(String *self, const float *v)
{
    String tmp;
    String_ctor_float(&tmp, *v);
    self->s.append(tmp.s);
    return self;
}

// ---- Reverse_722e8.cpp ----
// AbyssEngine::String::Reverse() - reverse the code units (only for language id 9 / RTL).
extern "C" void String_Reverse(String *self)
{
    if (!self->s.empty() && GameText_getLanguage() == 9) {
        std::u16string r(self->s.rbegin(), self->s.rend());
        self->s = r;
    }
}

// ---- String_737b4.cpp ----
// AbyssEngine::String::String(int) - decimal string of an int.
extern "C" String *String_ctor_int(String *self, int v)
{
    self->s.clear();
    String_Set_longlong(self, (long long)v);
    return self;
}

// ---- ToUpperCase_730a4.cpp ----
// AbyssEngine::String::ToUpperCase() - uppercase ASCII plus a CP-1252 accented range.
extern "C" void String_ToUpperCase(String *self)
{
    for (size_t i = 0; i < self->s.size(); i++) {
        short c = (short)self->s[i];
        unsigned short u = (unsigned short)(c - 0x61);
        bool above = u > 0x19;
        bool eq1a = u == 0x1a;
        if (above) {
            u = (unsigned short)(c - 0xe0);
            eq1a = u == 0x1e;
        }
        if (!above || u < 0x1e || eq1a) {
            self->s[i] = (char16_t)(c - 0x20);
        } else {
            // CP-1252 accented lowercase -> uppercase mapping.
            switch (c) {
            case 0x81: self->s[i] = 0x9a; break;
            case 0x82: self->s[i] = 0x90; break;
            case 0x84: self->s[i] = 0x8e; break;
            case 0x86: self->s[i] = 0x8f; break;
            case 0x83: case 0x85:
                break;   // no change
            default:
                if (c == 0x91)      self->s[i] = 0x92;
                else if (c == 0x94) self->s[i] = 0x99;
                else if (c == 0xa4) self->s[i] = 0xa5;
                break;
            }
        }
    }
}

// ---- ValueOf_727bc.cpp ----
// AbyssEngine::String::ValueOf() - parse the string as an integer.
extern "C" int String_ValueOf(String *self)
{
    char *bytes = String_GetAEChar(self);
    int v = atoi(bytes);
    operator_delete__(bytes);
    return v;
}

// ---- Split_7322c.cpp ----
// AbyssEngine::String::Split(sep) -> Array<String*>* (null if no splits).
extern "C" void *String_Split(String *self, String *sep)
{
    if (!self->s.empty() && !sep->s.empty()) {
        void *arr = operator_new(0xc);
        String_ArrayString_ctor(arr);

        unsigned int pos = 0;
        unsigned int idx;
        while ((idx = String_IndexOf_from(self, pos, sep)) != 0xffffffff) {
            if (pos < idx) {
                String *piece = (String *)operator_new(0xc);
                String_SubString(piece, self, pos, idx);
                String_ArrayString_add(piece, arr);
            }
            pos = (unsigned int)sep->s.size() + idx;
        }
        if (pos != 0 && pos < self->s.size()) {
            String *piece = (String *)operator_new(0xc);
            String_SubString(piece, self, pos, self->s.size());
            String_ArrayString_add(piece, arr);
        }

        if (((Array<String *> *)arr)->size() != 0)
            return arr;

        // No elements collected: tear the array down and report failure.
        String_ArrayString_removeAll(arr);
        operator_delete(String_ArrayString_dtor(arr));
    }
    return 0;
}

// ---- operator___72378.cpp ----
// Returned for out-of-range indices: a static NUL code unit.
static char16_t g_String_nullChar = 0;

// AbyssEngine::String::operator[](int) - pointer to the index-th code unit, or &NUL on OOB.
extern "C" uint16_t *String_index(String *self, int i)
{
    if (i < 0 || (unsigned int)i >= self->s.size())
        return (uint16_t *)&g_String_nullChar;
    return (uint16_t *)&self->s[i];
}

// ---- String_72488.cpp ----
// AbyssEngine::String::String(char const*, bool reverse)
extern "C" String *String_ctor_char(String *self, const char *s, bool reverse)
{
    self->s.clear();
    String_Set_char(self, s);
    if (reverse)
        String_Reverse(self);
    return self;
}

// ---- Set_72a0c.cpp ----
// AbyssEngine::String::Set(long long) - format a signed 64-bit integer as a decimal string.
extern "C" void String_Set_longlong(String *self, long long v)
{
    self->s.clear();

    if (v == 0) {
        String_Set_char(self, "");
        return;
    }

    bool neg = v < 0;
    unsigned long long mag = neg ? (unsigned long long)(-v) : (unsigned long long)v;

    char16_t tmp[24];
    int n = 0;
    do {
        unsigned int rem = (unsigned int)(mag % 10);
        char16_t ch = (char16_t)(rem | 0x30);
        if (rem > 9)
            ch = (char16_t)(rem + 0x57);
        tmp[n++] = ch;
        mag /= 10;
    } while (mag != 0);

    std::u16string out;
    if (neg)
        out.push_back(u'-');
    for (int i = n - 1; i >= 0; i--)
        out.push_back(tmp[i]);
    self->s = out;
}

// ---- String_72564.cpp ----
// AbyssEngine::String::String(AbyssEngine::String const&, bool reverse) - copy ctor.
extern "C" String *String_ctor_copy(String *self, String *other, bool reverse)
{
    self->s = other->s;
    if (reverse)
        String_Reverse(self);
    return self;
}

// ---- StrLen_729e0.cpp ----
// AbyssEngine::String::StrLen(char const*) - byte length of a NUL-terminated char string.
extern "C" int String_StrLen_char(String *self, const char *s)
{
    int n = 0;
    while (s[n] != '\0')
        n++;
    return n;
}

// ---- operator___72928.cpp ----
// AbyssEngine::String::operator+=(int const&) - sign-extend to 64-bit and append.
extern "C" String *String_addAssign_int(String *self, const int *v)
{
    long long ext = (long long)*v;
    return String_addAssign_longlong(self, &ext);
}

// ---- String_729bc.cpp ----
// AbyssEngine::String::String(float) - formatted string of a float.
extern "C" String *String_ctor_float(String *self, float v)
{
    self->s.clear();
    String_Set_float(self, v);
    return self;
}

// ---- Compare_72fc0.cpp ----
// AbyssEngine::String::Compare(char const*) - compare against an 8-bit string.
extern "C" unsigned int String_Compare_char(String *self, const char *s)
{
    if (self->s.empty())
        return 1;

    bool reachedEnd = false;
    uint16_t cur = 0;
    unsigned int other = 0;
    size_t i = 0;
    for (; i < self->s.size() && (cur = (uint16_t)self->s[i]) != 0; i++) {
        other = (unsigned int)(unsigned char)*s;
        if (other == 0 || other != cur)
            goto done;
        s++;
    }
    other = (unsigned int)(unsigned char)*s;
    reachedEnd = true;
    cur = 0;
done:
    if (other != 0) {
        char d = (char)cur - (char)other;
        if (reachedEnd)
            d = -1;
        return (unsigned int)(int)d;
    }
    return (unsigned int)(cur != 0);
}

// ---- Set_724b4.cpp ----
// AbyssEngine::String::Set(char const*) - replace contents from an 8-bit string (widened to 16-bit).
extern "C" void String_Set_char(String *self, const char *s)
{
    self->s.clear();
    if (s == 0)
        return;
    for (const unsigned char *p = (const unsigned char *)s; *p != 0; p++)
        self->s.push_back((char16_t)*p);
}

// ---- Compare_72f80.cpp ----
// AbyssEngine::String::Compare(AbyssEngine::String const&)
// Returns 0 when equal; a small signed value otherwise (0xff sentinel for length mismatch).
extern "C" int String_Compare_str(String *self, String *other)
{
    short result;
    if (other->s.size() == self->s.size()) {
        size_t i = 0;
        short sc = 0, oc = 0;
        bool reachedEnd = false;
        while (true) {
            sc = (i < self->s.size()) ? (short)self->s[i] : 0;
            oc = (i < other->s.size()) ? (short)other->s[i] : 0;
            if (sc == 0) {
                reachedEnd = true;
                sc = 0;
                break;
            }
            if (oc != sc) {
                reachedEnd = false;
                break;
            }
            i++;
        }
        if (oc == 0) {
            result = sc;
            if (sc != 0)
                result = 1;
        } else {
            result = sc - oc;
            if (reachedEnd)
                result = -1;
        }
    } else {
        result = 0xff;
    }
    return (int)(char)result;
}

// ---- _String_72594.cpp ----
// AbyssEngine::String::~String() - clear contents.
extern "C" String *String_dtor(String *self)
{
    self->s.clear();
    return self;
}

// ---- Trim_731a8.cpp ----
// AbyssEngine::String::Trim() - strip leading/trailing spaces and tabs.
extern "C" void String_Trim(String *self)
{
    size_t len = self->s.size();
    if (len == 0)
        return;

    size_t start = 0;
    while (start < len) {
        char16_t c = self->s[start];
        if (c != 0x20 && c != 9)
            break;
        start++;
    }
    size_t end = len;
    while (end > 0) {
        char16_t c = self->s[end - 1];
        if (c != 0x20 && c != 9)
            break;
        end--;
    }
    if (start < end)
        self->s = self->s.substr(start, end - start);
    else
        self->s.clear();
}

// ---- GetStringLength_73634.cpp ----
// AbyssEngine::String::GetStringLength(char const*) - byte length of a NUL-terminated char string.
extern "C" int String_GetStringLength(const char *s)
{
    int n = 0;
    while (s[n] != '\0')
        n++;
    return n;
}

// ---- ToLowerCase_73128.cpp ----
// AbyssEngine::String::ToLowerCase() - lowercase ASCII plus a CP-1252 accented range.
extern "C" void String_ToLowerCase(String *self)
{
    for (size_t i = 0; i < self->s.size(); i++) {
        short c = (short)self->s[i];
        unsigned short u = (unsigned short)(c - 0x41);
        bool above = u > 0x19;
        bool eq1a = u == 0x1a;
        if (above) {
            u = (unsigned short)(c - 0xc0);
            eq1a = u == 0x1e;
        }
        if (!above || u < 0x1e || eq1a) {
            self->s[i] = (char16_t)(c + 0x20);
        } else {
            // CP-1252 accented uppercase -> lowercase mapping.
            switch (c) {
            case 0x8e: self->s[i] = 0x84; break;
            case 0x8f: self->s[i] = 0x86; break;
            case 0x90: self->s[i] = 0x82; break;
            case 0x92: self->s[i] = 0x91; break;
            case 0x99: self->s[i] = 0x94; break;
            case 0x9a: self->s[i] = 0x81; break;
            case 0x91: case 0x93: case 0x94: case 0x95:
            case 0x96: case 0x97: case 0x98:
                break;   // no change
            default:
                if (c == 0xa5)
                    self->s[i] = 0xa4;
                break;
            }
        }
    }
}

// ---- operator__727a8.cpp ----
// AbyssEngine::String::operator=(AbyssEngine::String const&)
extern "C" String *String_assign(String *self, String *other)
{
    self->s = other->s;
    return self;
}

// ---- String_72398.cpp ----
// AbyssEngine::String::String() - default constructor: empty string.
extern "C" void String_ctor(String *self)
{
    self->s.clear();
}

// ---- Set_723dc.cpp ----
// AbyssEngine::String::Set(unsigned short const*) - replace contents from a wide string.
extern "C" void String_Set_wchar(String *self, const uint16_t *s)
{
    self->s.clear();
    if (s == 0)
        return;
    for (const uint16_t *p = s; *p != 0; p++)
        self->s.push_back((char16_t)*p);
}

// ---- IndexOf_73004.cpp ----
// AbyssEngine::String::IndexOf(unsigned int start, AbyssEngine::String const&)
// Return the first index >= start where needle occurs, or 0xffffffff if not found.
extern "C" unsigned int String_IndexOf_from(String *self, unsigned int start, String *needle)
{
    unsigned int slen = (unsigned int)self->s.size();
    unsigned int nlen = (unsigned int)needle->s.size();
    while (start < slen && nlen <= slen - start) {
        if (needle->s[0] == self->s[start]) {
            unsigned int k = 0;
            while (start + k < slen && self->s[start + k] == needle->s[k]) {
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

// ---- operator___727d8.cpp ----
// AbyssEngine::String::operator+=(AbyssEngine::String const&)
extern "C" String *String_addAssign_str(String *self, String *other)
{
    self->s.append(other->s);
    return self;
}

// ---- ConvertFromUTF8_7270c.cpp ----
// AbyssEngine::String::ConvertFromUTF8() - reinterpret the stored bytes as UTF-8 and re-store.
extern "C" void String_ConvertFromUTF8(String *self)
{
    if (self->s.empty())
        return;

    char *bytes = String_GetAEChar(self);
    uint16_t *wide = String_getWCharFromUtf8(bytes, (int)self->s.size());
    String_Set_wchar(self, wide);
    operator_delete__(bytes);
    operator_delete__(wide);
}

// ---- _String_725b8.cpp ----
// AbyssEngine::String::~String() - deleting destructor: destroy then free the object.
extern "C" void String_dtor_del(String *self)
{
    String_dtor(self);
    operator_delete(self);
}

// ---- SplitTags_73368.cpp ----
// Tag delimiter fragments.
static const char kOpen[]  = "<";
static const char kClose[] = ">";
static const char kSlash[] = "</";

// AbyssEngine::String::SplitTags(AbyssEngine::String tag)
// Wrap `tag` as "<tag>", split this string on it, ending each run at the matching "</".
extern "C" void String_SplitTags(String *self, String *tag)
{
    if (self->s.empty() || tag->s.empty())
        return;

    // tag := "<" + tag + ">"
    {
        std::u16string wrapped;
        wrapped.push_back(u'<');
        wrapped.append(tag->s);
        wrapped.push_back(u'>');
        tag->s = wrapped;
    }

    void *arr = operator_new(0xc);
    String_ArrayString_ctor(arr);

    int endPos = 0;
    unsigned int pos = 0;
    unsigned int idx;
    while ((idx = String_IndexOf_from(self, pos, tag)) != 0xffffffff) {
        if (pos <= idx) {
            String *piece = (String *)operator_new(0xc);
            String_SubString(piece, self, pos, idx);
            String_ArrayString_add(piece, arr);

            unsigned int afterTag = (unsigned int)tag->s.size() + idx;
            String closer;
            String_ctor_char(&closer, kSlash, false);
            endPos = (int)String_IndexOf_from(self, afterTag, &closer);
            if (endPos == -1)
                goto done;

            String *piece2 = (String *)operator_new(0xc);
            String_SubString(piece2, self, afterTag, (unsigned int)endPos);
            String_ArrayString_add(piece2, arr);
        }
        pos = endPos + 1;
    }

    if (pos != 0 && pos < self->s.size()) {
        String *piece = (String *)operator_new(0xc);
        String_SubString(piece, self, pos, self->s.size());
        String_ArrayString_add(piece, arr);
    }

    if (((Array<String *> *)arr)->size() == 0) {
        String_ArrayString_removeAll(arr);
        operator_delete(String_ArrayString_dtor(arr));
    }
done:
    ;
}

// ---- operator___7289c.cpp ----
// AbyssEngine::String::operator+=(long long const&) - append the decimal form of a 64-bit value.
extern "C" String *String_addAssign_longlong(String *self, const long long *v)
{
    String tmp;
    String_ctor_longlong(&tmp, *v);
    self->s.append(tmp.s);
    return self;
}

// ---- Set_72cb0.cpp ----
// Static format fragments.
static const char kZeroDot[] = "0.";
static const char kZero[]    = "0";
static const char kDot[]     = ".";
static const char kExp[]     = "E";

// AbyssEngine::String::Set(float) - format a float into this string.
extern "C" void String_Set_float(String *self, float v)
{
    int exp = 0;
    int neg = 0;
    uint16_t *digitsW = String_computeFloatString(v, 10, &exp, &neg);

    std::u16string acc;

    if (exp < 1) {
        // Leading "0." plus |exp| zeros.
        for (const char *p = kZeroDot; *p; p++)
            acc.push_back((char16_t)(unsigned char)*p);
        for (int i = 0; exp < i; i--)
            for (const char *p = kZero; *p; p++)
                acc.push_back((char16_t)(unsigned char)*p);
    }

    for (const uint16_t *p = digitsW; *p != 0; p++)
        acc.push_back((char16_t)*p);

    if (exp > 0 && (unsigned int)exp <= acc.size()) {
        std::u16string head = acc.substr(0, (unsigned int)exp);
        std::u16string tail = acc.substr((unsigned int)exp);
        for (const char *p = kDot; *p; p++)
            head.push_back((char16_t)(unsigned char)*p);
        acc = head + tail;
    }

    if (neg != 0) {
        std::u16string sign;
        for (const char *p = kExp; *p; p++)
            sign.push_back((char16_t)(unsigned char)*p);
        acc = sign + acc;
    }

    operator_delete__(digitsW);
    self->s = acc;
}

// ---- StrLen_729f0.cpp ----
// AbyssEngine::String::StrLen(unsigned short const*) - length of a NUL-terminated wide string.
extern "C" int String_StrLen_wchar(String *self, const uint16_t *s)
{
    if (s == 0)
        return 0;
    int n = 0;
    while (s[n] != 0)
        n++;
    return n;
}

// ---- String_723b0.cpp ----
// AbyssEngine::String::String(unsigned short const*, bool reverse)
extern "C" String *String_ctor_wchar(String *self, const uint16_t *s, bool reverse)
{
    self->s.clear();
    String_Set_wchar(self, s);
    if (reverse)
        String_Reverse(self);
    return self;
}

// ---- operator___72850.cpp ----
// AbyssEngine::String::operator+=(char const&) - append a single character.
extern "C" String *String_addAssign_char(String *self, const char *c)
{
    self->s.push_back((char16_t)*(const unsigned char *)c);
    return self;
}

// ---- SubString_72ed8.cpp ----
// AbyssEngine::String::SubString(unsigned int start, unsigned int end)
// out = self[start..end); empty string when end <= start.
extern "C" void String_SubString(String *out, String *self, unsigned int start, unsigned int end)
{
    new (&out->s) std::u16string();   // placement-init when out came from raw operator_new
    if (start < end && start < self->s.size()) {
        unsigned int hi = end > self->s.size() ? (unsigned int)self->s.size() : end;
        out->s = self->s.substr(start, hi - start);
    }
}

// ---- getWCharFromUtf8_725d0.cpp ----
// AbyssEngine::String::getWCharFromUtf8(char* utf8, int len)
// Decode `len` UTF-8 bytes into a wide buffer, transliterating Cyrillic to Latin.
extern "C" uint16_t *String_getWCharFromUtf8(char *s, int len)
{
    // First pass: count output code units (continuation bytes collapse multi-byte sequences).
    int outCount = 0;
    for (int i = 0; i < len; i = i + 1) {
        if (((unsigned char)s[i] & 0xe0) == 0xc0)
            i = i + 1;
        else if (((unsigned char)s[i] & 0xf0) == 0xe0)
            i = i + 2;
        outCount = outCount + 1;
    }

    uint16_t *out = (uint16_t *)operator_new__((unsigned int)((outCount + 1) * 2));
    uint16_t *w = out;

    // Second pass: decode each sequence to a code point.
    for (int i = 0; i < len; i = i + 1) {
        unsigned short cp = (unsigned short)(unsigned char)s[i];
        unsigned short val = cp;
        if ((cp & 0xe0) == 0xc0) {
            unsigned short hi = cp & 0x1f;
            int j = i + 1;
            val = (unsigned short)((hi << 6) | ((unsigned char)s[j] & 0x3f));
            i = j;
        } else if ((cp & 0xf0) == 0xe0) {
            int j = i + 2;
            unsigned short mid = (unsigned short)(((unsigned char)s[i + 1] & 0x3f) | ((cp & 0xf) << 6));
            val = (unsigned short)((mid << 6) | ((unsigned char)s[j] & 0x3f));
            i = j;
        }
        *w = val;
        w++;
    }
    out[outCount] = 0;

    // Third pass: transliterate Cyrillic letters to ASCII approximations.
    for (int i = 0; i != outCount; i = i + 1) {
        unsigned short c = out[i];
        unsigned short r = c;
        switch (c) {
        case 0x410: r = 0x41; break;   // A
        case 0x412: r = 0x42; break;   // B
        case 0x415: r = 0x45; break;   // E
        case 0x41a: r = 0x4b; break;   // K
        case 0x41c: r = 0x4d; break;   // M
        case 0x41d: r = 0x48; break;   // H
        case 0x41e: r = 0x4f; break;   // O
        case 0x420: r = 0x50; break;   // P
        case 0x421: r = 0x43; break;   // C
        case 0x422: r = 0x54; break;   // T
        case 0x425: r = 0x58; break;   // X
        case 0x411: case 0x413: case 0x414: case 0x416:
        case 0x417: case 0x418: case 0x419: case 0x41b:
        case 0x41f: case 0x423: case 0x424:
            break;   // no Latin equivalent
        case 0x43e: case 0xba: r = 0x6f; break;   // o
        case 0x440: r = 0x70; break;   // p
        case 0x441: r = 0x63; break;   // c
        case 0x445: r = 0x78; break;   // x
        case 0x43f: case 0x442: case 0x443: case 0x444:
            break;
        case 0x435: r = 0x65; break;   // e
        case 0xaa: case 0x430: r = 0x61; break;   // a
        case 0x60: r = 0x27; break;    // backtick -> apostrophe
        default:
            break;
        }
        out[i] = r;
    }
    return out;
}

// ---- IndexOf_73068.cpp ----
// AbyssEngine::String::IndexOf(AbyssEngine::String const&) - search from offset 0.
extern "C" unsigned int String_IndexOf(String *self, String *needle)
{
    return String_IndexOf_from(self, 0, needle);
}

// ---- String_72904.cpp ----
// AbyssEngine::String::String(long long) - decimal string of a 64-bit value.
extern "C" String *String_ctor_longlong(String *self, long long v)
{
    self->s.clear();
    String_Set_longlong(self, v);
    return self;
}

// ---- operator___72788.cpp ----
// AbyssEngine::String::operator[](int) const - pointer to the index-th code unit, or &NUL on OOB.
extern "C" uint16_t *String_index_const(String *self, int i)
{
    if (i < 0 || (unsigned int)i >= self->s.size())
        return (uint16_t *)&g_String_nullChar;
    return (uint16_t *)&self->s[i];
}

// ---- PrintOut_73644.cpp ----
// AbyssEngine::String::PrintOut() - print the string via the platform helper.
extern "C" void String_PrintOut(String *self)
{
    char *bytes = String_GetAEChar(self);
    String_printImpl(bytes);
}
