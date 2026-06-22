#include "game/core/String.h"
#include "engine/core/GameText.h"
#include <string>   // local scratch buffers only; String's storage is the native data/length

// AbyssEngine::String, native re-expression.
// String is the binary's native 12-byte polymorphic string { vptr@0, unsigned short* data@4,
// int length@8 } (see docs/STRING_REMODEL.md). `data` is a heap UTF-16 buffer managed with
// operator new[]/delete[] and kept NUL-terminated; `length` is the code-unit count. The methods
// below operate directly on data/length; a few building methods assemble into a local std::u16string
// scratch and then copy into the native buffer with assignWide(). Behaviour (CP-1252 case tables,
// Cyrillic transliteration, Compare sentinels, Split/SplitTags semantics) is preserved.

extern "C" {
// External (non-String) helpers kept as opaque imports.
int   atoi(const char *s);                                 // libc
void  String_printImpl(const char *s);                     // 0x1ab108 platform print
uint16_t *String_computeFloatString(float v, int base, int *outExp, int *outNeg);

// operator+(String const&, String const&) free helper (implemented elsewhere).
void  String_concat(String *out, String *a, String *b);
// operator=(String const&) used by some call sites under a distinct symbol.
void  String_assign_op(String *self, String *src);
}

// AbyssEngine::String::~String() — the sole virtual. Frees the heap buffer; defined out-of-line so
// this TU emits the engine's standalone vtable/typeinfo + D0 (deleting) / D1 / D2 destructor symbols,
// byte-matching the binary (vtable @0x21CC8C: [0][typeinfo][D1 0x82594][D0 0x825b8]).
String::~String() {
    if (this->data)
        delete[] this->data;
    this->data = nullptr;
}

// AbyssEngine::String::GetAEChar() const - allocate an 8-bit (low-byte) copy of the wide buffer.
char * String::GetAEChar() const {
    unsigned int len = (unsigned int)this->length;
    unsigned int n = len + 1;
    char *out = new char[n];
    for (unsigned int i = 0; i < len; i++)
        out[i] = (char)(this->data[i] & 0xff);
    out[len] = 0;
    return out;
}

// AbyssEngine::String::ReplaceString(find, repl) - replace each occurrence of `find` with `repl`.
void String::ReplaceString(String find, String repl) {
    if (this->length == 0 || find.length == 0)
        return;

    std::u16string acc;
    unsigned int pos = 0;
    int idx;
    while ((idx = (int)((String *)(this))->IndexOf_from(pos, &find)) != -1) {
        acc.append(reinterpret_cast<const char16_t *>(this->data) + pos, (unsigned int)idx - pos);
        if (repl.length)
            acc.append(reinterpret_cast<const char16_t *>(repl.data), (unsigned int)repl.length);
        pos = (unsigned int)find.length + idx;
    }

    if (pos != 0 && pos < (unsigned int)this->length)
        acc.append(reinterpret_cast<const char16_t *>(this->data) + pos, (unsigned int)this->length - pos);

    if (!acc.empty())
        assignWide(reinterpret_cast<const unsigned short *>(acc.data()), (int)acc.size());
}

// AbyssEngine::String::ReplaceChar(char from, char to) - replace every matching code unit.
void String::ReplaceChar(char from, char to) {
    for (int i = 0; i < this->length; i++)
        if ((unsigned int)this->data[i] == (unsigned int)(int)from)
            this->data[i] = (unsigned short)(short)to;
}

// AbyssEngine::String::Reverse() - reverse the code units (only for language id 9 / RTL).
void String::Reverse() {
    if (this->length > 0 && GameText::getLanguage() == 9) {
        for (int i = 0, j = this->length - 1; i < j; i++, j--) {
            unsigned short t = this->data[i];
            this->data[i] = this->data[j];
            this->data[j] = t;
        }
    }
}

// AbyssEngine::String::ToUpperCase() - uppercase ASCII plus a CP-1252 accented range.
void String::ToUpperCase() {
    for (int i = 0; i < this->length; i++) {
        short c = (short)this->data[i];
        unsigned short u = (unsigned short)(c - 0x61);
        bool above = u > 0x19;
        bool eq1a = u == 0x1a;
        if (above) {
            u = (unsigned short)(c - 0xe0);
            eq1a = u == 0x1e;
        }
        if (!above || u < 0x1e || eq1a) {
            this->data[i] = (unsigned short)(c - 0x20);
        } else {
            // CP-1252 accented lowercase -> uppercase mapping.
            switch (c) {
            case 0x81: this->data[i] = 0x9a; break;
            case 0x82: this->data[i] = 0x90; break;
            case 0x84: this->data[i] = 0x8e; break;
            case 0x86: this->data[i] = 0x8f; break;
            case 0x83: case 0x85:
                break;   // no change
            default:
                if (c == 0x91)      this->data[i] = 0x92;
                else if (c == 0x94) this->data[i] = 0x99;
                else if (c == 0xa4) this->data[i] = 0xa5;
                break;
            }
        }
    }
}

// AbyssEngine::String::ValueOf() - parse the string as an integer.
int String::ValueOf() {
    char *bytes = ((String *)(this))->GetAEChar();
    int v = atoi(bytes);
    delete[] bytes;
    return v;
}

// AbyssEngine::String::Split(sep) -> Array<String*>* (null if no splits).
void * String::Split(String sep) {
    if (this->length != 0 && sep.length != 0) {
        Array<String *> *arr = new Array<String *>();

        unsigned int pos = 0;
        unsigned int idx;
        while ((idx = ((String *)(this))->IndexOf_from(pos, &sep)) != 0xffffffff) {
            if (pos < idx) {
                String *piece = new String();
                piece->SubString(this, pos, idx);
                arr->push_back(piece);
            }
            pos = (unsigned int)sep.length + idx;
        }
        if (pos != 0 && pos < (unsigned int)this->length) {
            String *piece = new String();
            piece->SubString(this, pos, (unsigned int)this->length);
            arr->push_back(piece);
        }

        if (arr->size() != 0)
            return arr;

        // No elements collected: tear the array down and report failure.
        arr->clear();
        delete arr;
    }
    return 0;
}

// Returned for out-of-range indices: a static NUL code unit.
static unsigned short g_String_nullChar = 0;

// AbyssEngine::String::operator[](int) - pointer to the index-th code unit, or &NUL on OOB.
unsigned short *String::operator[](int i) {
    if (i < 0 || i >= this->length)
        return &g_String_nullChar;
    return this->data + i;
}

// AbyssEngine::String::Set(long long) - format a signed 64-bit integer as a decimal string.
void String::Set(long long v) {
    clear();

    if (v == 0) {
        this->Set("");
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
    assignWide(reinterpret_cast<const unsigned short *>(out.data()), (int)out.size());
}

// AbyssEngine::String::StrLen(char const*) - byte length of a NUL-terminated char string.
int String::StrLen(const char *s) {
    const char *p = s;
    while (*p != '\0')
        p++;
    return (int)(p - s);
}

// AbyssEngine::String::Compare(char const*) - compare against an 8-bit string.
unsigned int String::Compare(const char *s) {
    if (this->length == 0)
        return 1;

    bool reachedEnd = false;
    uint16_t cur = 0;
    unsigned int other = 0;
    int i = 0;
    for (; i < this->length && (cur = (uint16_t)this->data[i]) != 0; i++) {
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

// AbyssEngine::String::Set(char const*) - replace contents from an 8-bit string (widened to 16-bit).
void String::Set(const char *s) {
    clear();
    if (s == 0)
        return;
    int n = 0;
    for (const unsigned char *p = (const unsigned char *)s; *p != 0; p++)
        n++;
    if (n == 0)
        return;
    unsigned short *nd = new unsigned short[n + 1];
    const unsigned char *p = (const unsigned char *)s;
    for (int i = 0; i < n; i++)
        nd[i] = (unsigned short)p[i];
    nd[n] = 0;
    this->data = nd;
    this->length = n;
}

// AbyssEngine::String::Compare(AbyssEngine::String const&)
// Returns 0 when equal; a small signed value otherwise (0xff sentinel for length mismatch).
int String::Compare(const String &otherRef) {
    const String *other = &otherRef;
    short result;
    if (other->length == this->length) {
        int i = 0;
        short sc = 0, oc = 0;
        bool reachedEnd = false;
        while (true) {
            sc = (i < this->length) ? (short)this->data[i] : 0;
            oc = (i < other->length) ? (short)other->data[i] : 0;
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

// AbyssEngine::String::Trim() - strip leading/trailing spaces and tabs.
void String::Trim() {
    int len = this->length;
    if (len == 0)
        return;

    int start = 0;
    while (start < len) {
        unsigned short c = this->data[start];
        if (c != 0x20 && c != 9)
            break;
        start++;
    }
    int end = len;
    while (end > 0) {
        unsigned short c = this->data[end - 1];
        if (c != 0x20 && c != 9)
            break;
        end--;
    }
    if (start < end) {
        std::u16string sub(reinterpret_cast<const char16_t *>(this->data) + start, end - start);
        assignWide(reinterpret_cast<const unsigned short *>(sub.data()), (int)sub.size());
    } else {
        clear();
    }
}

// AbyssEngine::String::GetStringLength(char const*) - byte length of a NUL-terminated char string.
int String::GetStringLength(const char *s)
{
    const char *p = s;
    while (*p != '\0')
        p++;
    return (int)(p - s);
}

// ABI-stable free entry point kept for existing call sites; forwards to the real static method.
int String_GetStringLength(const char *s)
{
    return String::GetStringLength(s);
}

// AbyssEngine::String::ToLowerCase() - lowercase ASCII plus a CP-1252 accented range.
void String::ToLowerCase() {
    for (int i = 0; i < this->length; i++) {
        short c = (short)this->data[i];
        unsigned short u = (unsigned short)(c - 0x41);
        bool above = u > 0x19;
        bool eq1a = u == 0x1a;
        if (above) {
            u = (unsigned short)(c - 0xc0);
            eq1a = u == 0x1e;
        }
        if (!above || u < 0x1e || eq1a) {
            this->data[i] = (unsigned short)(c + 0x20);
        } else {
            // CP-1252 accented uppercase -> lowercase mapping.
            switch (c) {
            case 0x8e: this->data[i] = 0x84; break;
            case 0x8f: this->data[i] = 0x86; break;
            case 0x90: this->data[i] = 0x82; break;
            case 0x92: this->data[i] = 0x91; break;
            case 0x99: this->data[i] = 0x94; break;
            case 0x9a: this->data[i] = 0x81; break;
            case 0x91: case 0x93: case 0x94: case 0x95:
            case 0x96: case 0x97: case 0x98:
                break;   // no change
            default:
                if (c == 0xa5)
                    this->data[i] = 0xa4;
                break;
            }
        }
    }
}

// AbyssEngine::String::operator=(AbyssEngine::String const&)
// Re-Set from the other's wide buffer; an empty source leaves this string untouched (matches 0x727a8).
String &String::operator=(const String &other) {
    if (other.data != nullptr)
        this->Set(other.data);
    return *this;
}

// AbyssEngine::String::Set(unsigned short const*) - replace contents from a wide string.
void String::Set(const unsigned short *s) {
    clear();
    if (s == 0)
        return;
    int n = 0;
    while (s[n] != 0)
        n++;
    if (n == 0)
        return;
    unsigned short *nd = new unsigned short[n + 1];
    for (int i = 0; i < n; i++)
        nd[i] = s[i];
    nd[n] = 0;
    this->data = nd;
    this->length = n;
}

// AbyssEngine::String::ConvertFromUTF8() - reinterpret the stored bytes as UTF-8 and re-store.
void String::ConvertFromUTF8() {
    if (this->length == 0)
        return;

    char *bytes = ((String *)(this))->GetAEChar();
    uint16_t *wide = String::getWCharFromUtf8(bytes, this->length);
    ((String *)(this))->Set_wchar(wide);
    delete[] bytes;
    delete[] wide;
}

// Tag delimiter fragments.
static const char kOpen[]  = "<";
static const char kClose[] = ">";
static const char kSlash[] = "</";

// AbyssEngine::String::SplitTags(AbyssEngine::String tag)
// Wrap `tag` as "<tag>", split this string on it, ending each run at the matching "</".
void String::SplitTags(String tag) {
    if (this->length == 0 || tag.length == 0)
        return;

    // tag := "<" + tag + ">"
    {
        std::u16string wrapped;
        wrapped.push_back(u'<');
        if (tag.length)
            wrapped.append(reinterpret_cast<const char16_t *>(tag.data), (unsigned int)tag.length);
        wrapped.push_back(u'>');
        tag.assignWide(reinterpret_cast<const unsigned short *>(wrapped.data()), (int)wrapped.size());
    }

    Array<String *> *arr = new Array<String *>();

    int endPos = 0;
    unsigned int pos = 0;
    unsigned int idx;
    while ((idx = ((String *)(this))->IndexOf_from(pos, &tag)) != 0xffffffff) {
        if (pos <= idx) {
            String *piece = new String();
            piece->SubString(this, pos, idx);
            arr->push_back(piece);

            unsigned int afterTag = (unsigned int)tag.length + idx;
            String closer;
            ((String *)(&closer))->ctor_char(kSlash, false);
            endPos = (int)((String *)(this))->IndexOf_from(afterTag, &closer);
            if (endPos == -1)
                goto done;

            String *piece2 = new String();
            piece2->SubString(this, afterTag, (unsigned int)endPos);
            arr->push_back(piece2);
        }
        pos = endPos + 1;
    }

    if (pos != 0 && pos < (unsigned int)this->length) {
        String *piece = new String();
        piece->SubString(this, pos, (unsigned int)this->length);
        arr->push_back(piece);
    }

    if (arr->size() == 0) {
        arr->clear();
        delete arr;
    }
done:
    ;
}

// Static format fragments.
static const char kZeroDot[] = "0.";
static const char kZero[]    = "0";
static const char kDot[]     = ".";
static const char kExp[]     = "E";

// AbyssEngine::String::Set(float) - format a float into this string.
void String::Set(float v) {
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

    delete[] digitsW;
    assignWide(reinterpret_cast<const unsigned short *>(acc.data()), (int)acc.size());
}

// AbyssEngine::String::StrLen(unsigned short const*) - length of a NUL-terminated wide string.
int String::StrLen(const unsigned short *s) {
    if (s == 0)
        return 0;
    const unsigned short *p = s;
    while (*p != 0)
        p++;
    return (int)(p - s);
}

// AbyssEngine::String::SubString(unsigned int start, unsigned int end)
// Return self[start..end) as a new String; empty when end <= start.
String String::SubString(unsigned int start, unsigned int end) {
    String out;
    out.SubString(this, start, end);
    return out;
}

// AbyssEngine::String::getWCharFromUtf8(char* utf8, int len)
// Decode `len` UTF-8 bytes into a wide buffer, transliterating Cyrillic to Latin.
uint16_t *String::getWCharFromUtf8(char *s, int len)
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

    uint16_t *out = new uint16_t[outCount + 1];
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

// AbyssEngine::String::IndexOf(AbyssEngine::String const&) - search from offset 0.
unsigned int String::IndexOf(const String &needle) {
    return ((String *)(this))->IndexOf_from(0, &needle);
}

// AbyssEngine::String::IndexOf(unsigned int start, AbyssEngine::String const&)
// Return the first index >= start where needle occurs, or 0xffffffff if not found.
unsigned int String::IndexOf(unsigned int start, const String &needle) {
    return this->IndexOf_from(start, &needle);
}

// AbyssEngine::String::operator[](int) const - pointer to the index-th code unit, or &NUL on OOB.
const unsigned short *String::operator[](int i) const {
    if (i < 0 || i >= this->length)
        return &g_String_nullChar;
    return this->data + i;
}

// AbyssEngine::String::GetAEWChar() const - the wide backing buffer.
const unsigned short *String::GetAEWChar() const {
    return this->data;
}

// AbyssEngine::String::operator unsigned short*() - implicit access to the wide backing buffer.
String::operator unsigned short *() {
    return this->data;
}

// AbyssEngine::String::operator unsigned short const*() const - implicit wide-buffer access.
String::operator const unsigned short *() const {
    return this->data;
}

// AbyssEngine::String::PrintOut() - print the string via the platform helper.
void String::PrintOut() {
    char *bytes = ((String *)(this))->GetAEChar();
    String_printImpl(bytes);
}

// ---- Real engine constructor / operator+= overloads ----
// These carry the original binary's mangled symbols and delegate to the recovered member bodies.

// AbyssEngine::String::String() — default (empty); out-of-line so the standalone C1Ev/C2Ev symbols
// are emitted (the compiler plants the vptr + zeroes data/length), matching the binary (0x72398).
String::String() : data(nullptr), length(0) {}
// AbyssEngine::String::String(char const*, bool)
String::String(const char *cstr, bool reverse) : data(nullptr), length(0) {
    Set(cstr);
    if (reverse) Reverse();
}
// AbyssEngine::String::String(unsigned short const*, bool)
String::String(const uint16_t *wstr, bool reverse) : data(nullptr), length(0) { ctor_wchar(wstr, reverse); }
// AbyssEngine::String::String(AbyssEngine::String const&, bool)
String::String(const String &other, bool reverse) : data(nullptr), length(0) { ctor_copy(const_cast<String *>(&other), reverse); }
// AbyssEngine::String::String(char)
String::String(char c) : data(nullptr), length(0) { ctor_charval(c); }
// AbyssEngine::String::String(int)
String::String(int v) : data(nullptr), length(0) { ctor_int(v); }
// AbyssEngine::String::String(float)
String::String(float v) : data(nullptr), length(0) { ctor_float(v); }
// AbyssEngine::String::String(long long)
String::String(long long v) : data(nullptr), length(0) { ctor_longlong(v); }

// AbyssEngine::String::operator+=(char const&)
String &String::operator+=(const char &c) { addAssign_char(&c); return *this; }
// AbyssEngine::String::operator+=(int const&)
String &String::operator+=(const int &v) { addAssign_int(&v); return *this; }
// AbyssEngine::String::operator+=(float const&)
String &String::operator+=(const float &v) { addAssign_float(&v); return *this; }
// AbyssEngine::String::operator+=(long long const&)
String &String::operator+=(const long long &v) { addAssign_longlong(&v); return *this; }
