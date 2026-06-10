#include "gof2/String.h"


extern "C" void *operator_new__(unsigned int n);
extern "C" void String_Set_longlong(String *self, long long v);
extern "C" String *String_addAssign_longlong(String *self, const long long *v);
extern "C" void String_Set_float(String *self, float v);
extern "C" void operator_delete__(void *p);
extern "C" void String_Set_wchar(String *self, const uint16_t *s);
extern "C" unsigned int String_IndexOf_from(String *self, unsigned int start, String *needle);

// ---- GetAEChar_72752.cpp ----
// AbyssEngine::String::GetAEChar() const - allocate an 8-bit (low-byte) copy of the wide buffer.
extern "C" char *String_GetAEChar(String *self)
{
    unsigned int len = self->field_0x8;
    if (len != 0) {
        unsigned int n = len + 1;
        char *out = (char *)operator_new__(n);
        for (unsigned int i = 0; i != n; i++)
            out[i] = *(char *)((char *)self->field_0x4 + i * 2);
        return out;
    }
    char *out = (char *)operator_new__(1);
    out[0] = 0;
    return out;
}

// ---- ReplaceString_73530.cpp ----
__attribute__((visibility("hidden"))) extern void *g_String_vtable;

extern "C" {
unsigned int String_IndexOf_from(String *self, unsigned int start, String *needle);
void String_SubString(String *out, String *self, unsigned int start, unsigned int end);
void String_concat(String *out, String *a, String *b);
void String_addAssign_str(String *self, String *other);
void String_Set_wchar(String *self, const uint16_t *s);
void String_dtor(String *self);
}

struct SBuf { uint32_t a, b, c; };

// AbyssEngine::String::ReplaceString(AbyssEngine::String find, AbyssEngine::String repl)
// Replace each occurrence of `find` with `repl` (here named param2=repl) in this string.
extern "C" void String_ReplaceString(String *self, String *find, String *repl)
{
    if (self->field_0x4 == 0 || self->field_0x8 == 0 || find->field_0x8 == 0)
        return;

    SBuf acc;
    acc.a = (uint32_t)(unsigned long)g_String_vtable;
    acc.b = 0;
    acc.c = 0;

    unsigned int pos = 0;
    int idx;
    while ((idx = (int)String_IndexOf_from(self, pos, find)) != -1) {
        SBuf head, piece;
        String_SubString((String *)&head, self, pos, (unsigned int)idx);
        String_concat((String *)&piece, (String *)&head, repl);
        String_addAssign_str((String *)&acc, (String *)&piece);
        String_dtor((String *)&piece);
        String_dtor((String *)&head);
        pos = find->field_0x8 + idx;
    }

    if (pos != 0 && pos < self->field_0x8) {
        SBuf tail;
        String_SubString((String *)&tail, self, pos, self->field_0x8);
        String_addAssign_str((String *)&acc, (String *)&tail);
        String_dtor((String *)&tail);
    }

    if (acc.c != 0)   // accumulator has a non-zero length field
        String_Set_wchar(self, (uint16_t *)(unsigned long)acc.b);
    String_dtor((String *)&acc);
}

// ---- ReplaceChar_73070.cpp ----
// AbyssEngine::String::ReplaceChar(char from, char to) - replace every matching code unit.
extern "C" void String_ReplaceChar(String *self, char from, char to)
{
    uint16_t *p = self->field_0x4;
    if (p == 0)
        return;
    int count = self->field_0x8;
    while (count != 0) {
        count = count - 1;
        if ((unsigned int)*p == (unsigned int)(int)from)
            *p = (uint16_t)(short)to;
        p++;
    }
}

// ---- String_6e484.cpp ----
__attribute__((visibility("hidden"))) extern void *g_String_vtable;

// Set(long long): the char ctor forwards a sign/zero-extended value into the 64-bit Set.

// AbyssEngine::String::String(char) - build a string from a single character's numeric value.
extern "C" String *String_ctor_charval(String *self, char c)
{
    self->field_0x0 = g_String_vtable;
    self->field_0x4 = 0;
    String_Set_longlong(self, (long long)c);
    return self;
}

// ---- operator___72958.cpp ----
extern "C" {
String *String_ctor_float(String *self, float v);
void String_dtor(String *self);
void *realloc(void *p, unsigned int n);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
}

struct SBuf { uint32_t a, b, c; };

// AbyssEngine::String::operator+=(float const&) - append the formatted form of a float.
extern "C" String *String_addAssign_float(String *self, const float *v)
{
    SBuf tmp;
    String_ctor_float((String *)&tmp, *v);
    unsigned int addLen = *(uint32_t *)((char *)&tmp + 0x8);
    if (addLen != 0) {
        void *addData = *(void **)((char *)&tmp + 0x4);
        int oldLen = self->field_0x8;
        int newLen = addLen + oldLen;
        self->field_0x8 = newLen;
        void *buf = realloc(self->field_0x4, newLen * 2 + 2);
        self->field_0x4 = buf;
        __aeabi_memcpy((char *)buf + oldLen * 2, addData, addLen << 1);
    }
    String_dtor((String *)&tmp);
    return self;
}

// ---- Reverse_722e8.cpp ----
extern "C" {
int GameText_getLanguage();                             // 0x6f544
String *String_ctor_wchar(String *self, const uint16_t *s, bool rev);
void String_dtor(String *self);
}

// Sentinel code unit returned by operator[] for out-of-range reads (PC-rel static NUL).
__attribute__((visibility("hidden"))) extern uint16_t g_String_nullChar;

struct SBuf { uint32_t a, b, c; };

// AbyssEngine::String::Reverse() - reverse the code units (only for language id 9 / RTL).
extern "C" void String_Reverse(String *self)
{
    if (self->field_0x4 != 0 && GameText_getLanguage() == 9) {
        SBuf src;
        String_ctor_wchar((String *)&src, self->field_0x4, false);
        uint16_t *srcData = *(uint16_t **)((char *)&src + 0x4);
        int srcLen = *(int *)((char *)&src + 0x8);

        unsigned int idx = self->field_0x8 - 1;
        uint16_t *tail = srcData + idx;
        uint16_t *nul = &g_String_nullChar;

        int dst = 0;
        for (; idx != 0xffffffff; idx = idx - 1) {
            uint16_t *pick = nul;
            if ((int)idx < srcLen)
                pick = tail;
            if (idx > 0x7fffffff)
                pick = nul;
            tail = tail - 1;
            *(uint16_t *)((char *)self->field_0x4 + dst) = *pick;
            dst = dst + 2;
        }
        String_dtor((String *)&src);
    }
}

// ---- String_737b4.cpp ----
__attribute__((visibility("hidden"))) extern void *g_String_vtable;


// AbyssEngine::String::String(int) - decimal string of an int (sign-extended into Set(long long)).
extern "C" String *String_ctor_int(String *self, int v)
{
    self->field_0x0 = g_String_vtable;
    self->field_0x4 = 0;
    String_Set_longlong(self, (long long)v);
    return self;
}

// ---- ToUpperCase_730a4.cpp ----
// AbyssEngine::String::ToUpperCase() - uppercase ASCII plus a CP-1252 accented range.
extern "C" void String_ToUpperCase(String *self)
{
    short *p = self->field_0x4;
    if (p == 0)
        return;
    int count = self->field_0x8;
    while (count != 0) {
        short c = *p;
        unsigned short u = (unsigned short)(c - 0x61);
        bool above = u > 0x19;
        bool eq1a = u == 0x1a;
        if (above) {
            u = (unsigned short)(c - 0xe0);
            eq1a = u == 0x1e;
        }
        short out;
        if (!above || u < 0x1e || eq1a) {
            out = c - 0x20;
            *p = out;
        } else {
            // CP-1252 accented lowercase -> uppercase mapping.
            out = 0x9a;
            switch (c) {
            case 0x81: *p = out; break;
            case 0x82: out = 0x90; *p = out; break;
            case 0x84: out = 0x8e; *p = out; break;
            case 0x86: out = 0x8f; *p = out; break;
            case 0x83: case 0x85:
                break;   // no change
            default:
                if (c == 0x91) {
                    out = 0x92;
                    *p = out;
                } else if (c == 0x94) {
                    out = 0x99;
                    *p = out;
                } else if (c == 0xa4) {
                    out = 0xa5;
                    *p = out;
                }
                break;
            }
        }
        p++;
        count--;
    }
}

// ---- ValueOf_727bc.cpp ----
extern "C" {
char *String_GetAEChar(String *self);   // 0x6ef68 - allocates an 8-bit copy
int atoi(const char *s);                 // 0x6f5a4
void operator_delete__(void *p);
}

// AbyssEngine::String::ValueOf() - parse the string as an integer.
extern "C" int String_ValueOf(String *self)
{
    char *bytes = String_GetAEChar(self);
    int v = atoi(bytes);
    operator_delete__(bytes);
    return v;
}

// ---- Split_7322c.cpp ----
extern "C" {
void *operator_new(unsigned int n);
void operator_delete(void *p);
void String_ArrayString_ctor(void *arr);                       // Array<String*>::Array()
unsigned int String_IndexOf_from(String *self, unsigned int start, String *needle);
void String_SubString(String *out, String *self, unsigned int start, unsigned int end);
void String_ArrayString_add(void *item, void *arr);            // ArrayAdd<String*>
void String_ArrayString_removeAll(void *arr);                  // ArrayRemoveAll<String*>
void *String_ArrayString_dtor(void *arr);                      // Array<String*>::~Array()
}

// AbyssEngine::String::Split(AbyssEngine::String sep) -> Array<String*>* (null if no splits).
extern "C" void *String_Split(String *self, String *sep)
{
    if (self->field_0x4 != 0 && self->field_0x8 != 0 && sep->field_0x8 != 0) {
        void *arr = operator_new(0xc);
        String_ArrayString_ctor(arr);

        unsigned int pos = 0;
        unsigned int idx;
        while ((idx = String_IndexOf_from(self, pos, sep)) != 0xffffffff) {
            if (pos < idx) {
                void *piece = operator_new(0xc);
                String_SubString((String *)piece, self, pos, idx);
                String_ArrayString_add(piece, arr);
            }
            pos = sep->field_0x8 + idx;
        }
        if (pos != 0 && pos < self->field_0x8) {
            void *piece = operator_new(0xc);
            String_SubString((String *)piece, self, pos, self->field_0x8);
            String_ArrayString_add(piece, arr);
        }

        if (*(int *)arr != 0)
            return arr;

        // No elements collected: tear the array down and report failure.
        String_ArrayString_removeAll(arr);
        operator_delete(String_ArrayString_dtor(arr));
    }
    return 0;
}

// ---- operator___72378.cpp ----
// Returned for out-of-range indices: a PC-relative pointer to a static NUL code unit.
__attribute__((visibility("hidden"))) extern uint16_t g_String_nullChar;

// AbyssEngine::String::operator[](int) - pointer to the index-th code unit, or &NUL on OOB.
extern "C" uint16_t *String_index(String *self, int i)
{
    uint16_t *fallback = &g_String_nullChar;
    if (i < 0)
        return fallback;
    if (i < self->field_0x8)
        return (uint16_t *)((char *)self->field_0x4 + i * 2);
    return fallback;
}

// ---- String_72488.cpp ----
__attribute__((visibility("hidden"))) extern void *g_String_vtable;

extern "C" {
void String_Set_char(String *self, const char *s);
void String_Reverse(String *self);
}

// AbyssEngine::String::String(char const*, bool reverse)
extern "C" String *String_ctor_char(String *self, const char *s, bool reverse)
{
    self->field_0x0 = g_String_vtable;
    self->field_0x4 = 0;
    String_Set_char(self, s);
    if (reverse)
        String_Reverse(self);
    return self;
}

// ---- Set_72a0c.cpp ----
extern "C" {
void operator_delete__(void *p);
void *operator_new__(unsigned int n);
unsigned long long __aeabi_uldivmod(unsigned long long num, unsigned long long den);
void String_Set_char(String *self, const char *s);   // virtual Set(char const*) fallback
}

// AbyssEngine::String::Set(long long) - format a signed 64-bit integer as a decimal string.
extern "C" void String_Set_longlong(String *self, long long v)
{
    void *old = self->field_0x4;
    if (old != 0)
        operator_delete__(old);
    self->field_0x4 = 0;

    if (v == 0) {
        String_Set_char(self, "");
        return;
    }

    uint16_t *buf = (uint16_t *)operator_new__(0x28);
    self->field_0x4 = buf;
    self->field_0x8 = 0;

    unsigned long long mag;
    uint16_t *digits;       // where the first generated digit is written
    unsigned int count;     // running digit count (1 if a sign was emitted)
    if (v < 0) {
        buf[0] = '-';
        digits = buf + 1;
        mag = (unsigned long long)(-v);
        count = 1;
        self->field_0x8 = 1;
    } else {
        digits = buf;
        mag = (unsigned long long)v;
        count = 0;
    }

    // Emit decimal digits least-significant first, just past the sign slot.
    uint16_t *w = digits;
    do {
        unsigned long long q = __aeabi_uldivmod(mag, 10);
        unsigned int rem = (unsigned int)(mag - q * 10);
        unsigned char ch = (unsigned char)rem | 0x30;
        if (rem > 9)
            ch = (unsigned char)rem + 0x57;
        *w = ch;
        w++;
        count++;
        mag = q;
    } while (mag != 0);
    self->field_0x8 = count;
    *w = 0;

    // Reverse the generated digit run in place so it reads most-significant first.
    uint16_t *lo = digits;
    uint16_t *hi = w - 1;
    while (lo < hi) {
        uint16_t t = *lo;
        *lo = *hi;
        *hi = t;
        lo++;
        hi--;
    }
}

// ---- String_72564.cpp ----
__attribute__((visibility("hidden"))) extern void *g_String_vtable;

extern "C" {
void String_Set_wchar(String *self, const uint16_t *s);
void String_Reverse(String *self);
}

// AbyssEngine::String::String(AbyssEngine::String const&, bool reverse) - copy ctor.
extern "C" String *String_ctor_copy(String *self, String *other, bool reverse)
{
    self->field_0x0 = g_String_vtable;
    self->field_0x4 = 0;
    String_Set_wchar(self, other->field_0x4);
    if (reverse)
        String_Reverse(self);
    return self;
}

// ---- StrLen_729e0.cpp ----
// AbyssEngine::String::StrLen(char const*) - byte length of a NUL-terminated char string.
extern "C" int String_StrLen_char(String *self, const char *s)
{
    const char *p = s;
    char c;
    do {
        c = *p;
        p++;
    } while (c != '\0');
    return (int)(p + ~(unsigned int)s - (char *)0);
}

// ---- operator___72928.cpp ----
// Appends via the long long overload after sign-extending the int.

// AbyssEngine::String::operator+=(int const&) - sign-extend to 64-bit and append.
extern "C" String *String_addAssign_int(String *self, const int *v)
{
    long long ext = (long long)*v;
    return String_addAssign_longlong(self, &ext);
}

// ---- String_729bc.cpp ----
__attribute__((visibility("hidden"))) extern void *g_String_vtable;


// AbyssEngine::String::String(float) - formatted string of a float.
extern "C" String *String_ctor_float(String *self, float v)
{
    self->field_0x0 = g_String_vtable;
    self->field_0x4 = 0;
    String_Set_float(self, v);
    return self;
}

// ---- Compare_72fc0.cpp ----
// AbyssEngine::String::Compare(char const*) - compare against an 8-bit string.
extern "C" unsigned int String_Compare_char(String *self, const char *s)
{
    if (self->field_0x8 == 0)
        return 1;

    bool reachedEnd = false;
    uint16_t cur;
    unsigned int other;
    uint16_t *p = self->field_0x4;
    for (; (cur = *p) != 0; p++) {
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
extern "C" {
void operator_delete__(void *p);
void *operator_new__(unsigned int n);
}

// Empty-string literal substituted when a null pointer is passed.
static const char kEmpty[] = "";

// AbyssEngine::String::Set(char const*) - replace contents from an 8-bit string (widened to 16-bit).
extern "C" void String_Set_char(String *self, const char *s)
{
    void *old = self->field_0x4;
    if (old != 0)
        operator_delete__(old);
    self->field_0x4 = 0;

    // Null input falls back to the empty literal (matching the target's retry loop).
    if (s == 0)
        s = kEmpty;

    const unsigned char *in = (const unsigned char *)s;
    const unsigned char *p = in;
    unsigned char c;
    do {
        c = *p;
        p++;
    } while (c != 0);
    unsigned int len = (unsigned int)(p - in) - 1;   // code units excluding terminator
    self->field_0x8 = len;

    if (len == 0) {
        uint16_t *buf = (uint16_t *)operator_new__(2);
        self->field_0x4 = buf;
        buf[0] = 0;
        return;
    }

    if (in[len - 1] == 0) {
        // Last byte already NUL: copy len units, terminate at len-1.
        unsigned int bytes = len * 2;
        if (len + len < len)
            bytes = 0xffffffff;
        uint16_t *buf = (uint16_t *)operator_new__(bytes);
        self->field_0x4 = buf;
        for (unsigned int i = 0; i < len; i++)
            buf[i] = (uint16_t)in[i];
        self->field_0x8 = len - 1;
    } else {
        unsigned int count = len + 1;
        unsigned int bytes = count * 2;
        if (count + count < count)
            bytes = 0xffffffff;
        uint16_t *buf = (uint16_t *)operator_new__(bytes);
        self->field_0x4 = buf;
        for (unsigned int i = 0; i < len; i++)
            buf[i] = (uint16_t)in[i];
        buf[len] = 0;
    }
}

// ---- Compare_72f80.cpp ----
// AbyssEngine::String::Compare(AbyssEngine::String const&)
// Returns 0 when equal; a small signed value otherwise (0xff sentinel for length mismatch).
extern "C" int String_Compare_str(String *self, String *other)
{
    short result;
    if (other->field_0x8 == self->field_0x8) {
        short *op = other->field_0x4;
        short *sp = self->field_0x4;
        short sc, oc;
        bool reachedEnd;
        while (true) {
            sc = *sp;
            oc = *op;
            if (sc == 0) {
                reachedEnd = true;
                sc = 0;
                break;
            }
            if (oc != sc) {
                reachedEnd = false;
                break;
            }
            sp++;
            op++;
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
__attribute__((visibility("hidden"))) extern void *g_String_vtable;

// AbyssEngine::String::~String() - reset vtable, free the data buffer.
extern "C" String *String_dtor(String *self)
{
    self->field_0x0 = g_String_vtable;
    void *data = self->field_0x4;
    if (data != 0)
        operator_delete__(data);
    self->field_0x4 = 0;
    return self;
}

// ---- Trim_731a8.cpp ----
extern "C" {
void String_SubString(String *out, String *self, unsigned int start, unsigned int end);
void String_Set_wchar(String *self, const uint16_t *s);
void String_dtor(String *self);
}

struct SBuf { uint32_t a, b, c; };

// AbyssEngine::String::Trim() - strip leading/trailing spaces and tabs.
extern "C" void String_Trim(String *self)
{
    int data = self->field_0x4;
    unsigned int len;
    if (data != 0 && (len = self->field_0x8) != 0) {
        short *base = (short *)(unsigned int)data;
        unsigned int start = 0;
        while (start < len) {
            short c = base[start];
            if (c != 0x20 && c != 9)
                break;
            start = start + 1;
        }
        // Walk back over trailing whitespace.
        unsigned int end = len;
        short c;
        do {
            do {
                c = base[end - 1];
                end = end - 1;
            } while (c == 9);
        } while (c == 0x20);

        SBuf tmp;
        String_SubString((String *)&tmp, self, start, end);
        String_Set_wchar(self, *(uint16_t **)((char *)&tmp + 0x4));
        String_dtor((String *)&tmp);
    }
}

// ---- GetStringLength_73634.cpp ----
// AbyssEngine::String::GetStringLength(char const*) - byte length of a NUL-terminated char string.
extern "C" int String_GetStringLength(const char *s)
{
    const char *p = s;
    char c;
    do {
        c = *p;
        p++;
    } while (c != '\0');
    return (int)(p + ~(unsigned int)s - (char *)0);
}

// ---- ToLowerCase_73128.cpp ----
// AbyssEngine::String::ToLowerCase() - lowercase ASCII plus a CP-1252 accented range.
extern "C" void String_ToLowerCase(String *self)
{
    short *p = self->field_0x4;
    if (p == 0)
        return;
    int count = self->field_0x8;
    while (count != 0) {
        short c = *p;
        unsigned short u = (unsigned short)(c - 0x41);
        bool above = u > 0x19;
        bool eq1a = u == 0x1a;
        if (above) {
            u = (unsigned short)(c - 0xc0);
            eq1a = u == 0x1e;
        }
        short out;
        if (!above || u < 0x1e || eq1a) {
            out = c + 0x20;
            *p = out;
        } else {
            // CP-1252 accented uppercase -> lowercase mapping.
            out = 0x81;
            switch (c) {
            case 0x8e: out = 0x84; *p = out; break;
            case 0x8f: out = 0x86; *p = out; break;
            case 0x90: out = 0x82; *p = out; break;
            case 0x92: out = 0x91; *p = out; break;
            case 0x99: out = 0x94; *p = out; break;
            case 0x9a: *p = out; break;
            case 0x91: case 0x93: case 0x94: case 0x95:
            case 0x96: case 0x97: case 0x98:
                break;   // no change
            default:
                if (c == 0xa5) {
                    out = 0xa4;
                    *p = out;
                }
                break;
            }
        }
        p++;
        count--;
    }
}

// ---- operator__727a8.cpp ----
// AbyssEngine::String::operator=(AbyssEngine::String const&)
extern "C" String *String_assign(String *self, String *other)
{
    uint16_t *data = other->field_0x4;
    if (data != 0)
        String_Set_wchar(self, data);
    return self;
}

// ---- String_72398.cpp ----
// Vtable pointer stored at +0 (target loads a PC-relative GOT slot and adds 8).
__attribute__((visibility("hidden"))) extern void *g_String_vtable;

// AbyssEngine::String::String() - default constructor: empty string.
extern "C" void String_ctor(String *self)
{
    self->field_0x0 = g_String_vtable;
    self->field_0x4 = 0;
    self->field_0x8 = 0;
}

// ---- Set_723dc.cpp ----
extern "C" {
void operator_delete__(void *p);
void *operator_new__(unsigned int n);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
void String_Set_char(String *self, const char *s);   // virtual Set(char const*) fallback
}

// AbyssEngine::String::Set(unsigned short const*) - replace contents from a wide string.
extern "C" void String_Set_wchar(String *self, const uint16_t *s)
{
    void *old = self->field_0x4;
    if (old != 0)
        operator_delete__(old);
    self->field_0x4 = 0;

    if (s == 0) {
        // Null -> empty string via the char-Set path (tail call in the target).
        String_Set_char(self, "");
        return;
    }

    const uint16_t *p = s;
    uint16_t c;
    do {
        c = *p;
        p++;
    } while (c != 0);
    unsigned int count = (unsigned int)(p - s);     // includes the terminator
    unsigned int len = count - 1;
    self->field_0x8 = len;

    if (len == 0) {
        uint16_t *buf = (uint16_t *)operator_new__(2);
        self->field_0x4 = buf;
        buf[0] = 0;
        return;
    }

    unsigned int last = count - 2;
    if (s[last] == 0) {
        unsigned int bytes = len * 2;
        if (len + len < len)
            bytes = 0xffffffff;
        void *buf = operator_new__(bytes);
        self->field_0x4 = buf;
        __aeabi_memcpy(buf, s, len * 2);
        *(uint16_t *)((char *)buf + last * 2) = 0;
        self->field_0x8 = last;
    } else {
        unsigned int bytes = count * 2;
        if (count + count < count)
            bytes = 0xffffffff;
        void *buf = operator_new__(bytes);
        self->field_0x4 = buf;
        __aeabi_memcpy(buf, s, len * 2);
        *(uint16_t *)((char *)buf + len * 2) = 0;
    }
}

// ---- IndexOf_73004.cpp ----
// AbyssEngine::String::IndexOf(unsigned int start, AbyssEngine::String const&)
// Return the first index >= start where needle occurs, or 0xffffffff if not found.
extern "C" unsigned int String_IndexOf_from(String *self, unsigned int start, String *needle)
{
    while (start < self->field_0x8 &&
           needle->field_0x8 <= self->field_0x8 - start) {
        short first = *needle->field_0x4;
        if (first == *(short *)((char *)self->field_0x4 + start * 2)) {
            unsigned int k = 0;
            while (first == *(short *)((char *)self->field_0x4 + start * 2 + k * 2)) {
                if (needle->field_0x8 <= k + 1)
                    return start;
                first = needle->field_0x4[k + 1];
                k = k + 1;
            }
            start = start + k;
        } else {
            start = start + 1;
        }
    }
    return 0xffffffff;
}

// ---- operator___727d8.cpp ----
extern "C" {
String *String_ctor_copy(String *self, String *other, bool rev);
void String_dtor(String *self);
void *realloc(void *p, unsigned int n);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
}

struct SBuf { uint32_t a, b, c; };

// AbyssEngine::String::operator+=(AbyssEngine::String const&)
extern "C" String *String_addAssign_str(String *self, String *other)
{
    if (other->field_0x8 != 0) {
        SBuf tmp;
        String_ctor_copy((String *)&tmp, other, false);
        unsigned int addLen = *(uint32_t *)((char *)&tmp + 0x8);
        void *addData = *(void **)((char *)&tmp + 0x4);

        int oldLen = self->field_0x8;
        int newLen = addLen + oldLen;
        self->field_0x8 = newLen;
        void *buf = realloc(self->field_0x4, newLen * 2 + 2);
        self->field_0x4 = buf;
        __aeabi_memcpy((char *)buf + oldLen * 2, addData, addLen << 1);
        *(uint16_t *)((char *)buf + self->field_0x8 * 2) = 0;
        String_dtor((String *)&tmp);
    }
    return self;
}

// ---- ConvertFromUTF8_7270c.cpp ----
extern "C" {
char *String_GetAEChar(String *self);                       // 0x6ef68
uint16_t *String_getWCharFromUtf8(char *s, int len);        // 0x6f598
void String_Set_wchar(String *self, const uint16_t *s);
void operator_delete__(void *p);
void String_freeWide(void *p);                              // 0x1ab108 (tail-called delete[])
}

// AbyssEngine::String::ConvertFromUTF8() - reinterpret the stored bytes as UTF-8 and re-store.
extern "C" void String_ConvertFromUTF8(String *self)
{
    if (self->field_0x4 == 0)
        return;

    char *bytes = String_GetAEChar(self);
    char *bytes2 = String_GetAEChar(self);
    uint16_t *wide = String_getWCharFromUtf8(bytes2, self->field_0x8);
    String_Set_wchar(self, wide);
    operator_delete__(bytes);
    String_freeWide(wide);
}

// ---- _String_725b8.cpp ----
extern "C" {
String *String_dtor(String *self);   // 0x6f580 - the non-deallocating destructor
void operator_delete(void *p);        // 0x1ab098 - deallocating operator delete
}

// AbyssEngine::String::~String() - deleting destructor: destroy then free the object.
extern "C" void String_dtor_del(String *self)
{
    String_dtor(self);
    operator_delete(self);
}

// ---- SplitTags_73368.cpp ----
extern "C" {
void *operator_new(unsigned int n);
void operator_delete(void *p);
String *String_ctor_char(String *self, const char *s, bool rev);
void String_concat(String *out, String *a, String *b);
void String_assign_op(String *self, String *src);
void String_dtor(String *self);
void String_ArrayString_ctor(void *arr);
unsigned int String_IndexOf_from(String *self, unsigned int start, String *needle);
void String_SubString(String *out, String *self, unsigned int start, unsigned int end);
void String_ArrayString_add(void *item, void *arr);
void String_ArrayString_removeAll(void *arr);
void *String_ArrayString_dtor(void *arr);
}

struct SBuf { uint32_t a, b, c; };

// Tag delimiter fragments (PC-relative literals in the target).
static const char kOpen[]  = "<";
static const char kClose[] = ">";
static const char kSlash[] = "</";

// AbyssEngine::String::SplitTags(AbyssEngine::String tag)
// Wrap `tag` as "<tag>", split this string on it, ending each run at the matching "</".
extern "C" void String_SplitTags(String *self, String *tag)
{
    if (self->field_0x4 == 0 || self->field_0x8 == 0 || tag->field_0x8 == 0)
        return;

    // tag := "<" + tag + ">"
    {
        SBuf open, lhs, close, full;
        String_ctor_char((String *)&open, kOpen, false);
        String_concat((String *)&lhs, (String *)&open, tag);
        String_ctor_char((String *)&close, kClose, false);
        String_concat((String *)&full, (String *)&lhs, (String *)&close);
        String_assign_op(tag, (String *)&full);
        String_dtor((String *)&full);
        String_dtor((String *)&close);
        String_dtor((String *)&lhs);
        String_dtor((String *)&open);
    }

    void *arr = operator_new(0xc);
    String_ArrayString_ctor(arr);

    int endPos = 0;
    unsigned int pos = 0;
    unsigned int idx;
    while ((idx = String_IndexOf_from(self, pos, tag)) != 0xffffffff) {
        if (pos <= idx) {
            void *piece = operator_new(0xc);
            String_SubString((String *)piece, self, pos, idx);
            String_ArrayString_add(piece, arr);

            int afterTag = tag->field_0x8 + idx;
            SBuf closer;
            String_ctor_char((String *)&closer, kSlash, false);
            endPos = (int)String_IndexOf_from(self, afterTag, (String *)&closer);
            String_dtor((String *)&closer);
            if (endPos == -1)
                goto done;

            void *piece2 = operator_new(0xc);
            String_SubString((String *)piece2, self, afterTag, (unsigned int)endPos);
            String_ArrayString_add(piece2, arr);
        }
        pos = endPos + 1;
    }

    if (pos != 0 && pos < self->field_0x8) {
        void *piece = operator_new(0xc);
        String_SubString((String *)piece, self, pos, self->field_0x8);
        String_ArrayString_add(piece, arr);
    }

    if (*(int *)arr == 0) {
        String_ArrayString_removeAll(arr);
        operator_delete(String_ArrayString_dtor(arr));
    }
done:
    ;
}

// ---- operator___7289c.cpp ----
extern "C" {
String *String_ctor_longlong(String *self, long long v);
void String_dtor(String *self);
void *realloc(void *p, unsigned int n);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
}

struct SBuf { uint32_t a, b, c; };

// AbyssEngine::String::operator+=(long long const&) - append the decimal form of a 64-bit value.
extern "C" String *String_addAssign_longlong(String *self, const long long *v)
{
    SBuf tmp;
    String_ctor_longlong((String *)&tmp, *v);
    unsigned int addLen = *(uint32_t *)((char *)&tmp + 0x8);
    if (addLen != 0) {
        void *addData = *(void **)((char *)&tmp + 0x4);
        int oldLen = self->field_0x8;
        int newLen = addLen + oldLen;
        self->field_0x8 = newLen;
        void *buf = realloc(self->field_0x4, newLen * 2 + 2);
        self->field_0x4 = buf;
        __aeabi_memcpy((char *)buf + oldLen * 2, addData, addLen << 1);
    }
    String_dtor((String *)&tmp);
    return self;
}

// ---- Set_72cb0.cpp ----
// Local 12-byte String storage used for temporaries on the stack.
struct SBuf { uint32_t a, b, c; };

extern "C" {
void operator_delete__(void *p);
// computeFloatString(float v, int base, int *outExp, int *outNeg) -> wide buffer.
uint16_t *String_computeFloatString(float v, int base, int *outExp, int *outNeg);
void String_Set_wchar(String *self, const uint16_t *s);
void String_Set_char(String *self, const char *s);
String *String_ctor_char(String *self, const char *s, bool rev);
void String_dtor(String *self);
// operator+(String const&, String const&) - the free-function concatenation helper.
void String_concat(String *out, String *a, String *b);
void String_addAssign_str(String *self, String *other);
void String_assign_op(String *self, String *src);
// SubString(uint, uint) -> String*  (out, this, start, end via r2/r3 in target)
void String_SubString(String *out, String *self, unsigned int start, unsigned int end);
}

// Static format fragments (PC-relative literals in the target).
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

    // Accumulator string built up on the stack; start empty.
    SBuf acc;
    String_Set_char((String *)&acc, "");

    if (exp < 1) {
        // Leading "0." plus |exp| zeros.
        String_Set_char((String *)&acc, kZeroDot);
        for (int i = 0; exp < i; i--) {
            SBuf tmp;
            String_ctor_char((String *)&tmp, kZero, false);
            String_addAssign_str((String *)&acc, (String *)&tmp);
            String_dtor((String *)&tmp);
        }
    }

    {
        SBuf tmp;
        String_ctor_char((String *)&tmp, (const char *)digitsW, false);
        String_addAssign_str((String *)&acc, (String *)&tmp);
        String_dtor((String *)&tmp);
    }

    if (exp > 0) {
        SBuf head, dot, left, tail, full;
        String_SubString((String *)&head, (String *)&acc, 0, (unsigned int)exp);
        String_ctor_char((String *)&dot, kDot, false);
        String_concat((String *)&left, (String *)&head, (String *)&dot);
        String_SubString((String *)&tail, (String *)&acc, (unsigned int)exp, 0);
        String_concat((String *)&full, (String *)&left, (String *)&tail);
        String_assign_op((String *)&acc, (String *)&full);
        String_dtor((String *)&full);
        String_dtor((String *)&tail);
        String_dtor((String *)&left);
        String_dtor((String *)&dot);
        String_dtor((String *)&head);
    }

    if (neg != 0) {
        SBuf sign, full;
        String_ctor_char((String *)&sign, kExp, false);
        String_concat((String *)&full, (String *)&sign, (String *)&acc);
        String_assign_op((String *)&acc, (String *)&full);
        String_dtor((String *)&full);
        String_dtor((String *)&sign);
    }

    operator_delete__(digitsW);
    String_Set_wchar(self, *(uint16_t **)((char *)&acc + 0x4));
    String_dtor((String *)&acc);
}

// ---- StrLen_729f0.cpp ----
// AbyssEngine::String::StrLen(unsigned short const*) - length of a NUL-terminated wide string.
extern "C" int String_StrLen_wchar(String *self, const uint16_t *s)
{
    if (s == 0)
        return 0;
    const uint16_t *p = s;
    uint16_t c;
    do {
        c = *p;
        p++;
    } while (c != 0);
    return (int)((p - s) - 1);
}

// ---- String_723b0.cpp ----
__attribute__((visibility("hidden"))) extern void *g_String_vtable;

extern "C" {
void String_Set_wchar(String *self, const uint16_t *s);
void String_Reverse(String *self);
}

// AbyssEngine::String::String(unsigned short const*, bool reverse)
extern "C" String *String_ctor_wchar(String *self, const uint16_t *s, bool reverse)
{
    self->field_0x0 = g_String_vtable;
    self->field_0x4 = 0;
    String_Set_wchar(self, s);
    if (reverse)
        String_Reverse(self);
    return self;
}

// ---- operator___72850.cpp ----
extern "C" {
void *operator_new__(unsigned int n);
void *realloc(void *p, unsigned int n);
}

// AbyssEngine::String::operator+=(char const&) - append a single character.
extern "C" String *String_addAssign_char(String *self, const char *c)
{
    int len = self->field_0x8;
    if (len == 0) {
        uint16_t *buf = (uint16_t *)operator_new__(4);
        self->field_0x4 = buf;
        unsigned char ch = *(const unsigned char *)c;
        buf[1] = 0;
        buf[0] = (uint16_t)ch;
        self->field_0x8 = 1;
    } else {
        self->field_0x8 = len + 1;
        char *buf = (char *)realloc(self->field_0x4, len * 2 + 4);
        self->field_0x4 = buf;
        int nlen = self->field_0x8;
        unsigned char ch = *(const unsigned char *)c;
        *(uint16_t *)(buf + nlen * 2) = 0;
        *(uint16_t *)(buf + nlen * 2 - 2) = (uint16_t)ch;
    }
    return self;
}

// ---- SubString_72ed8.cpp ----
__attribute__((visibility("hidden"))) extern void *g_String_vtable;

extern "C" {
void *operator_new__(unsigned int n);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
String *String_ctor_wchar(String *self, const uint16_t *s, bool rev);
}

// AbyssEngine::String::SubString(unsigned int start, unsigned int end)
// out = self[start..end); empty string when end <= start.
extern "C" void String_SubString(String *out, String *self, unsigned int start, unsigned int end)
{
    if (start < end) {
        int n = (int)(end - start);
        unsigned int count = n + 1;
        unsigned int bytes = count * 2;
        if (count + count < count)
            bytes = 0xffffffff;
        void *buf = operator_new__(bytes);
        __aeabi_memcpy(buf, (char *)self->field_0x4 + start * 2, n * 2);
        *(uint16_t *)((char *)buf + n * 2) = 0;
        // Construct the result in-place from the freshly built wide buffer.
        String_ctor_wchar(out, (uint16_t *)buf, false);
    } else {
        // Empty substring: vtable pointer + null data + zero length.
        out->field_0x0 = g_String_vtable;
        out->field_0x4 = 0;
        out->field_0x8 = 0;
    }
}

// ---- getWCharFromUtf8_725d0.cpp ----
// AbyssEngine::String::getWCharFromUtf8(char* utf8, int len)
// Decode `len` UTF-8 bytes into a wide buffer, transliterating Cyrillic to Latin.
extern "C" uint16_t *String_getWCharFromUtf8(char *s, int len)
{
    // First pass: count output code units (continuation bytes collapse multi-byte sequences).
    int alloc = -2;     // grows by -2 per emitted unit; negated for the byte count
    int outCount = 0;
    for (int i = 0; i < len; i = i + 1) {
        if (((unsigned char)s[i] & 0xe0) == 0xc0)
            i = i + 1;
        else if (((unsigned char)s[i] & 0xf0) == 0xe0)
            i = i + 2;
        alloc = alloc - 2;
        outCount = outCount + 1;
    }

    uint16_t *out = (uint16_t *)operator_new__((unsigned int)(-alloc));
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
// IndexOf(uint start, String const&) - the 3-arg overload at 0x73004.

// AbyssEngine::String::IndexOf(AbyssEngine::String const&) - search from offset 0.
extern "C" unsigned int String_IndexOf(String *self, String *needle)
{
    return String_IndexOf_from(self, 0, needle);
}

// ---- String_72904.cpp ----
__attribute__((visibility("hidden"))) extern void *g_String_vtable;


// AbyssEngine::String::String(long long) - decimal string of a 64-bit value.
extern "C" String *String_ctor_longlong(String *self, long long v)
{
    self->field_0x0 = g_String_vtable;
    self->field_0x4 = 0;
    String_Set_longlong(self, v);
    return self;
}

// ---- operator___72788.cpp ----
// Returned for out-of-range indices: a PC-relative pointer to a static NUL code unit.
__attribute__((visibility("hidden"))) extern uint16_t g_String_nullChar;

// AbyssEngine::String::operator[](int) const - pointer to the index-th code unit, or &NUL on OOB.
extern "C" uint16_t *String_index_const(String *self, int i)
{
    uint16_t *fallback = &g_String_nullChar;
    if (i < 0)
        return fallback;
    if (i < self->field_0x8)
        return (uint16_t *)((char *)self->field_0x4 + i * 2);
    return fallback;
}

// ---- PrintOut_73644.cpp ----
extern "C" {
char *String_GetAEChar(String *self);   // 0x6ef68 - allocates an 8-bit copy
void String_printImpl(const char *s);    // 0x1ab108 - tail-called print helper
}

// AbyssEngine::String::PrintOut() - print the string via the platform helper.
extern "C" void String_PrintOut(String *self)
{
    char *bytes = String_GetAEChar(self);
    String_printImpl(bytes);
}
