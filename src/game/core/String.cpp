#include "game/core/String.h"

// AbyssEngine::String, native re-expression.
// The canonical type is common.h's `AbyssEngine::String { std::u16string s; }`. Every method
// below operates on that std::u16string member instead of the original hand-managed
// {vtable, ushort* data, uint length} layout. Behaviour (CP-1252 case tables, Cyrillic
// transliteration, Compare sentinels, Split/SplitTags semantics) is preserved.

extern "C" {
// External (non-String) helpers kept as opaque imports.
int   GameText_getLanguage();
int   atoi(const char *s);                                 // libc
void  String_printImpl(const char *s);                     // 0x1ab108 platform print
uint16_t *String_computeFloatString(float v, int base, int *outExp, int *outNeg);

// operator+(String const&, String const&) free helper (implemented elsewhere).
void  String_concat(String *out, String *a, String *b);
// operator=(String const&) used by some call sites under a distinct symbol.
void  String_assign_op(String *self, String *src);
}

// AbyssEngine::String::GetAEChar() const - allocate an 8-bit (low-byte) copy of the wide buffer.
char * String::GetAEChar() {
    unsigned int len = this->s.size();
    unsigned int n = len + 1;
    char *out = new char[n];
    for (unsigned int i = 0; i < len; i++)
        out[i] = (char)(this->s[i] & 0xff);
    out[len] = 0;
    return out;
}

// AbyssEngine::String::ReplaceString(find, repl) - replace each occurrence of `find` with `repl`.
void String::ReplaceString(String *find, String *repl) {
    if (this->s.empty() || find->s.empty())
        return;

    std::u16string acc;
    unsigned int pos = 0;
    int idx;
    while ((idx = (int)((String *)(this))->IndexOf_from(pos, find)) != -1) {
        acc.append(this->s, pos, (unsigned int)idx - pos);
        acc.append(repl->s);
        pos = (unsigned int)find->s.size() + idx;
    }

    if (pos != 0 && pos < this->s.size())
        acc.append(this->s, pos, this->s.size() - pos);

    if (!acc.empty())
        this->s = acc;
}

// AbyssEngine::String::ReplaceChar(char from, char to) - replace every matching code unit.
void String::ReplaceChar(char from, char to) {
    for (size_t i = 0; i < this->s.size(); i++)
        if ((unsigned int)this->s[i] == (unsigned int)(int)from)
            this->s[i] = (char16_t)(short)to;
}

// AbyssEngine::String::String(char) - build a string from a single character's numeric value.
String * String::ctor_charval(char c) {
    this->s.clear();
    ((String *)(this))->Set_longlong((long long)c);
    return this;
}

// AbyssEngine::String::operator+=(float const&) - append the formatted form of a float.
String * String::addAssign_float(const float *v) {
    String tmp;
    ((String *)(&tmp))->ctor_float(*v);
    this->s.append(tmp.s);
    return this;
}

// AbyssEngine::String::Reverse() - reverse the code units (only for language id 9 / RTL).
void String::Reverse() {
    if (!this->s.empty() && GameText_getLanguage() == 9) {
        std::u16string r(this->s.rbegin(), this->s.rend());
        this->s = r;
    }
}

// AbyssEngine::String::String(int) - decimal string of an int.
String * String::ctor_int(int v) {
    this->s.clear();
    ((String *)(this))->Set_longlong((long long)v);
    return this;
}

// AbyssEngine::String::ToUpperCase() - uppercase ASCII plus a CP-1252 accented range.
void String::ToUpperCase() {
    for (size_t i = 0; i < this->s.size(); i++) {
        short c = (short)this->s[i];
        unsigned short u = (unsigned short)(c - 0x61);
        bool above = u > 0x19;
        bool eq1a = u == 0x1a;
        if (above) {
            u = (unsigned short)(c - 0xe0);
            eq1a = u == 0x1e;
        }
        if (!above || u < 0x1e || eq1a) {
            this->s[i] = (char16_t)(c - 0x20);
        } else {
            // CP-1252 accented lowercase -> uppercase mapping.
            switch (c) {
            case 0x81: this->s[i] = 0x9a; break;
            case 0x82: this->s[i] = 0x90; break;
            case 0x84: this->s[i] = 0x8e; break;
            case 0x86: this->s[i] = 0x8f; break;
            case 0x83: case 0x85:
                break;   // no change
            default:
                if (c == 0x91)      this->s[i] = 0x92;
                else if (c == 0x94) this->s[i] = 0x99;
                else if (c == 0xa4) this->s[i] = 0xa5;
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
void * String::Split(String *sep) {
    if (!this->s.empty() && !sep->s.empty()) {
        Array<String *> *arr = new Array<String *>();

        unsigned int pos = 0;
        unsigned int idx;
        while ((idx = ((String *)(this))->IndexOf_from(pos, sep)) != 0xffffffff) {
            if (pos < idx) {
                String *piece = new String();
                piece->SubString(this, pos, idx);
                arr->push_back(piece);
            }
            pos = (unsigned int)sep->s.size() + idx;
        }
        if (pos != 0 && pos < this->s.size()) {
            String *piece = new String();
            piece->SubString(this, pos, this->s.size());
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
static char16_t g_String_nullChar = 0;

// AbyssEngine::String::operator[](int) - pointer to the index-th code unit, or &NUL on OOB.
uint16_t * String::index(int i) {
    String *self = this;
    if (i < 0 || (unsigned int)i >= self->s.size())
        return (uint16_t *)&g_String_nullChar;
    return (uint16_t *)&self->s[i];
}

// AbyssEngine::String::String(char const*, bool reverse)
String * String::ctor_char(const char *s, bool reverse) {
    this->s.clear();
    ((String *)(this))->Set_char(s);
    if (reverse)
        ((String *)(this))->Reverse();
    return this;
}

// AbyssEngine::String::Set(long long) - format a signed 64-bit integer as a decimal string.
void String::Set_longlong(long long v) {
    this->s.clear();

    if (v == 0) {
        ((String *)(this))->Set_char("");
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
    this->s = out;
}

// AbyssEngine::String::String(AbyssEngine::String const&, bool reverse) - copy ctor.
String * String::ctor_copy(String *other, bool reverse) {
    this->s = other->s;
    if (reverse)
        ((String *)(this))->Reverse();
    return this;
}

// AbyssEngine::String::StrLen(char const*) - byte length of a NUL-terminated char string.
int String::StrLen_char(const char *s) {
    int n = 0;
    while (s[n] != '\0')
        n++;
    return n;
}

// AbyssEngine::String::operator+=(int const&) - sign-extend to 64-bit and append.
String * String::addAssign_int(const int *v) {
    long long ext = (long long)*v;
    return ((String *)(this))->addAssign_longlong(&ext);
}

// AbyssEngine::String::String(float) - formatted string of a float.
String * String::ctor_float(float v) {
    this->s.clear();
    ((String *)(this))->Set_float(v);
    return this;
}

// AbyssEngine::String::Compare(char const*) - compare against an 8-bit string.
unsigned int String::Compare_char(const char *s) {
    if (this->s.empty())
        return 1;

    bool reachedEnd = false;
    uint16_t cur = 0;
    unsigned int other = 0;
    size_t i = 0;
    for (; i < this->s.size() && (cur = (uint16_t)this->s[i]) != 0; i++) {
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
void String::Set_char(const char *s) {
    this->s.clear();
    if (s == 0)
        return;
    for (const unsigned char *p = (const unsigned char *)s; *p != 0; p++)
        this->s.push_back((char16_t)*p);
}

// AbyssEngine::String::Compare(AbyssEngine::String const&)
// Returns 0 when equal; a small signed value otherwise (0xff sentinel for length mismatch).
int String::Compare_str(String *other) {
    short result;
    if (other->s.size() == this->s.size()) {
        size_t i = 0;
        short sc = 0, oc = 0;
        bool reachedEnd = false;
        while (true) {
            sc = (i < this->s.size()) ? (short)this->s[i] : 0;
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

// AbyssEngine::String::~String() - clear contents.
String * String::dtor() {
    this->s.clear();
    return this;
}

// AbyssEngine::String::Trim() - strip leading/trailing spaces and tabs.
void String::Trim() {
    size_t len = this->s.size();
    if (len == 0)
        return;

    size_t start = 0;
    while (start < len) {
        char16_t c = this->s[start];
        if (c != 0x20 && c != 9)
            break;
        start++;
    }
    size_t end = len;
    while (end > 0) {
        char16_t c = this->s[end - 1];
        if (c != 0x20 && c != 9)
            break;
        end--;
    }
    if (start < end)
        this->s = this->s.substr(start, end - start);
    else
        this->s.clear();
}

// AbyssEngine::String::GetStringLength(char const*) - byte length of a NUL-terminated char string.
int String_GetStringLength(const char *s)
{
    int n = 0;
    while (s[n] != '\0')
        n++;
    return n;
}

// AbyssEngine::String::ToLowerCase() - lowercase ASCII plus a CP-1252 accented range.
void String::ToLowerCase() {
    for (size_t i = 0; i < this->s.size(); i++) {
        short c = (short)this->s[i];
        unsigned short u = (unsigned short)(c - 0x41);
        bool above = u > 0x19;
        bool eq1a = u == 0x1a;
        if (above) {
            u = (unsigned short)(c - 0xc0);
            eq1a = u == 0x1e;
        }
        if (!above || u < 0x1e || eq1a) {
            this->s[i] = (char16_t)(c + 0x20);
        } else {
            // CP-1252 accented uppercase -> lowercase mapping.
            switch (c) {
            case 0x8e: this->s[i] = 0x84; break;
            case 0x8f: this->s[i] = 0x86; break;
            case 0x90: this->s[i] = 0x82; break;
            case 0x92: this->s[i] = 0x91; break;
            case 0x99: this->s[i] = 0x94; break;
            case 0x9a: this->s[i] = 0x81; break;
            case 0x91: case 0x93: case 0x94: case 0x95:
            case 0x96: case 0x97: case 0x98:
                break;   // no change
            default:
                if (c == 0xa5)
                    this->s[i] = 0xa4;
                break;
            }
        }
    }
}

// AbyssEngine::String::operator=(AbyssEngine::String const&)
String * String::assign(String *other) {
    this->s = other->s;
    return this;
}

// AbyssEngine::String::String() - default constructor: empty string.
void String::ctor() {
    this->s.clear();
}

// AbyssEngine::String::Set(unsigned short const*) - replace contents from a wide string.
void String::Set_wchar(const uint16_t *s) {
    this->s.clear();
    if (s == 0)
        return;
    for (const uint16_t *p = s; *p != 0; p++)
        this->s.push_back((char16_t)*p);
}

// AbyssEngine::String::IndexOf(unsigned int start, AbyssEngine::String const&)
// Return the first index >= start where needle occurs, or 0xffffffff if not found.
unsigned int String::IndexOf_from(unsigned int start, String *needle) {
    String *self = this;
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

// AbyssEngine::String::operator+=(AbyssEngine::String const&)
String * String::addAssign_str(String *other) {
    this->s.append(other->s);
    return this;
}

// AbyssEngine::String::ConvertFromUTF8() - reinterpret the stored bytes as UTF-8 and re-store.
void String::ConvertFromUTF8() {
    if (this->s.empty())
        return;

    char *bytes = ((String *)(this))->GetAEChar();
    uint16_t *wide = String::getWCharFromUtf8(bytes, (int)this->s.size());
    ((String *)(this))->Set_wchar(wide);
    delete[] bytes;
    delete[] wide;
}

// AbyssEngine::String::~String() - deleting destructor: destroy then free the object.
void String::dtor_del() {
    ((String *)(this))->dtor();
    delete this;
}

// Tag delimiter fragments.
static const char kOpen[]  = "<";
static const char kClose[] = ">";
static const char kSlash[] = "</";

// AbyssEngine::String::SplitTags(AbyssEngine::String tag)
// Wrap `tag` as "<tag>", split this string on it, ending each run at the matching "</".
void String::SplitTags(String *tag) {
    if (this->s.empty() || tag->s.empty())
        return;

    // tag := "<" + tag + ">"
    {
        std::u16string wrapped;
        wrapped.push_back(u'<');
        wrapped.append(tag->s);
        wrapped.push_back(u'>');
        tag->s = wrapped;
    }

    Array<String *> *arr = new Array<String *>();

    int endPos = 0;
    unsigned int pos = 0;
    unsigned int idx;
    while ((idx = ((String *)(this))->IndexOf_from(pos, tag)) != 0xffffffff) {
        if (pos <= idx) {
            String *piece = new String();
            piece->SubString(this, pos, idx);
            arr->push_back(piece);

            unsigned int afterTag = (unsigned int)tag->s.size() + idx;
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

    if (pos != 0 && pos < this->s.size()) {
        String *piece = new String();
        piece->SubString(this, pos, this->s.size());
        arr->push_back(piece);
    }

    if (arr->size() == 0) {
        arr->clear();
        delete arr;
    }
done:
    ;
}

// AbyssEngine::String::operator+=(long long const&) - append the decimal form of a 64-bit value.
String * String::addAssign_longlong(const long long *v) {
    String tmp;
    ((String *)(&tmp))->ctor_longlong(*v);
    this->s.append(tmp.s);
    return this;
}

// Static format fragments.
static const char kZeroDot[] = "0.";
static const char kZero[]    = "0";
static const char kDot[]     = ".";
static const char kExp[]     = "E";

// AbyssEngine::String::Set(float) - format a float into this string.
void String::Set_float(float v) {
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
    this->s = acc;
}

// AbyssEngine::String::StrLen(unsigned short const*) - length of a NUL-terminated wide string.
int String::StrLen_wchar(const uint16_t *s) {
    if (s == 0)
        return 0;
    int n = 0;
    while (s[n] != 0)
        n++;
    return n;
}

// AbyssEngine::String::String(unsigned short const*, bool reverse)
String * String::ctor_wchar(const uint16_t *s, bool reverse) {
    this->s.clear();
    ((String *)(this))->Set_wchar(s);
    if (reverse)
        ((String *)(this))->Reverse();
    return this;
}

// AbyssEngine::String::operator+=(char const&) - append a single character.
String * String::addAssign_char(const char *c) {
    this->s.push_back((char16_t)*(const unsigned char *)c);
    return this;
}

// AbyssEngine::String::SubString(unsigned int start, unsigned int end)
// out = self[start..end); empty string when end <= start.
void String::SubString(String *self, unsigned int start, unsigned int end) {
    String *out = this;
    out->s.clear();
    if (start < end && start < self->s.size()) {
        unsigned int hi = end > self->s.size() ? (unsigned int)self->s.size() : end;
        out->s = self->s.substr(start, hi - start);
    }
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
unsigned int String::IndexOf(String *needle) {
    return ((String *)(this))->IndexOf_from(0, needle);
}

// AbyssEngine::String::String(long long) - decimal string of a 64-bit value.
String * String::ctor_longlong(long long v) {
    this->s.clear();
    ((String *)(this))->Set_longlong(v);
    return this;
}

// AbyssEngine::String::operator[](int) const - pointer to the index-th code unit, or &NUL on OOB.
uint16_t * String::index_const(int i) {
    String *self = this;
    if (i < 0 || (unsigned int)i >= self->s.size())
        return (uint16_t *)&g_String_nullChar;
    return (uint16_t *)&self->s[i];
}

// AbyssEngine::String::PrintOut() - print the string via the platform helper.
void String::PrintOut() {
    char *bytes = ((String *)(this))->GetAEChar();
    String_printImpl(bytes);
}
