#include "game/core/String.h"
#include "engine/core/GameText.h"

#include <cstdlib>
#include <cstring>
#include "engine/core/AbyssEngine.h"
#include "engine/file/AEFile.h"
#include "engine/file/FileInterfaceAndroid.h"
#include "engine/math/AEMath.h"
#include "engine/math/Transform.h"
#include "engine/render/Mesh.h"
#include "engine/render/Engine.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/FBOContainer.h"
#include "engine/math/Quaternion.h"
#include <GLES2/gl2.h>
#include <cmath>
#include <cstdint>

void String_printImpl(const char *s);


void String_concat(String * out, String * a, String * b);

void String_assign_op(String * self, String * src);

String::~String() {
    if (this->data)
        delete[] this->data;
    this->data = nullptr;
}

char *String::GetAEChar() const {
    if (this->length == 0) {
        char *empty = new char[1];
        empty[0] = 0;
        return empty;
    }
    unsigned int n = this->length + 1;
    char *out = new char[n];
    for (unsigned int i = 0; i < n; i++)
        out[i] = (char) this->data[i];
    return out;
}

void String::ReplaceString(String find, String repl) {
    if (this->data == nullptr || this->length == 0 || find.length == 0)
        return;

    String acc;
    unsigned int pos = 0;
    int idx;
    while ((idx = (int) ((String *) (this))->IndexOf(pos, find)) != -1) {
        acc += this->SubString(pos, (unsigned int) idx) + repl;
        pos = (unsigned int) find.length + idx;
    }

    if (pos != 0 && pos < (unsigned int) this->length)
        acc += this->SubString(pos, (unsigned int) this->length);

    if (acc.length != 0)
        this->Set(acc.data);
}

String *String::ReplaceChar(char from, char to) {
    if (this->data == nullptr)
        return this;
    for (unsigned int i = 0; i < (unsigned int) this->length; i++)
        if (this->data[i] == (unsigned char) from)
            this->data[i] = (unsigned char) to;
    return this;
}

void String::Reverse() {
    if (this->data != nullptr && GameText::getLanguage() == 9) {
        String copy(this->data, false);
        for (unsigned int i = 0; i < (unsigned int) this->length; i++)
            this->data[i] = *copy[this->length - 1 - i];
    }
}

String *String::ToUpperCase() {
    if (this->data == nullptr)
        return this;
    for (unsigned int i = 0; i < (unsigned int) this->length; i++) {
        unsigned short c = this->data[i];
        if ((unsigned short) (c - 'a') < 26 || (unsigned short) (c - 0xe0) < 31) {
            this->data[i] = (unsigned short) (c - 0x20);
        } else {
            switch (c) {
                case 0x81: this->data[i] = 0x9a;
                    break;
                case 0x82: this->data[i] = 0x90;
                    break;
                case 0x84: this->data[i] = 0x8e;
                    break;
                case 0x86: this->data[i] = 0x8f;
                    break;
                case 0x91: this->data[i] = 0x92;
                    break;
                case 0x94: this->data[i] = 0x99;
                    break;
                case 0xa4: this->data[i] = 0xa5;
                    break;
                default:
                    break;
            }
        }
    }
    return this;
}

int String::ValueOf() {
    char *bytes = ((String *) (this))->GetAEChar();
    int v = atoi(bytes);
    delete[] bytes;
    return v;
}

void *String::Split(String sep) { // lint: void_ptr exported String method return type, signature must not change
    // lint: void_ptr exported String method return type, signature must not change
    if (this->data != nullptr && this->length != 0 && sep.length != 0) {
        Array<String *> *arr = new Array<String *>();

        unsigned int pos = 0;
        unsigned int idx;
        while ((idx = IndexOf(pos, sep)) != 0xffffffff) {
            if (pos < idx)
                ArrayAdd(new String(SubString(pos, idx)), *arr);
            pos = (unsigned int) sep.length + idx;
        }
        if (pos != 0 && pos < (unsigned int) this->length)
            ArrayAdd(new String(SubString(pos, (unsigned int) this->length)), *arr);

        if (arr->size() != 0)
            return arr;

        ArrayRemoveAll(*arr);
        delete arr;
    }
    return 0;
}


unsigned short *String::operator[](int i) {
    if (i < 0 || i >= this->length)
        return &termChar;
    return this->data + i;
}

void String::Set(long long v) {
    if (this->data) delete[] this->data;
    this->data = nullptr;
    if (v == 0) {
        Set("");
        return;
    }
    unsigned short *out = new unsigned short[20];
    this->data = out;
    this->length = 0;
    unsigned long long magnitude;
    if (v < 0) {
        *out++ = '-';
        this->length = 1;
        magnitude = (unsigned long long) -v;
    } else {
        magnitude = (unsigned int) v; // binary: non-negative values keep only the low word
    }
    int count = 0;
    unsigned long long previous;
    do {
        unsigned int digit = (unsigned int) (magnitude % 10);
        unsigned char c = (unsigned char) (digit | '0');
        if (digit > 9)
            c = (unsigned char) (digit + 'a' - 10);
        out[count++] = c;
        this->length++;
        previous = magnitude;
        magnitude /= 10;
    } while (previous > 9);
    out[count] = 0;
    unsigned short *lo = out;
    unsigned short *hi = out + count - 1;
    do {
        unsigned short t = *hi;
        *hi = *lo;
        *lo = t;
        lo++;
        hi--;
    } while (lo < hi);
}

int String::StrLen(const char *s) {
    const char *p = s;
    while (*p++ != '\0') {
    }
    return (int) (p - s) - 1;
}

unsigned int String::Compare(const char *s) {
    if (this->length == 0)
        return 1;
    const unsigned short *a = this->data;
    bool reachedEnd = false;
    unsigned short ca;
    unsigned char cb;
    for (int i = 0;; i++) {
        ca = a[i];
        if (ca == 0) {
            cb = (unsigned char) s[i];
            reachedEnd = true;
            break;
        }
        cb = (unsigned char) s[i];
        if (cb == 0 || cb != ca)
            break;
    }
    if (cb == 0)
        return ca != 0;
    int d = ca - cb;
    if (reachedEnd)
        d = -1;
    return (signed char) d;
}

void String::Set(const char *s) {
    if (this->data) delete[] this->data;
    this->data = nullptr;
    if (s == nullptr) {
        Set("");
        return;
    }
    int len = StrLen(s);
    this->length = len;
    if (len == 0) {
        this->data = new unsigned short[1];
        this->data[0] = 0;
    } else if (s[len - 1] != 0) {
        unsigned short *out = new unsigned short[len + 1];
        this->data = out;
        for (unsigned int i = 0; i < (unsigned int) len; i++)
            out[i] = (unsigned char) s[i];
        out[len] = 0;
    } else {
        unsigned short *out = new unsigned short[len];
        this->data = out;
        for (unsigned int i = 0; i < (unsigned int) len; i++)
            out[i] = (unsigned char) s[i];
        this->length = len - 1;
    }
}

signed char String::Compare(const String &other) {
    signed char result;
    if (other.length == this->length) {
        const unsigned short *a = this->data;
        const unsigned short *b = other.data;
        bool reachedEnd = false;
        unsigned short ca, cb;
        while (true) {
            ca = *a;
            cb = *b;
            if (ca == 0) {
                reachedEnd = true;
                break;
            }
            if (cb != ca)
                break;
            b++;
            a++;
        }
        if (cb == 0)
            result = ca != 0;
        else if (reachedEnd)
            result = -1;
        else
            result = (signed char) (ca - cb);
    } else {
        result = -1;
    }
    return result;
}

String *String::Trim() {
    if (this->data != nullptr && this->length != 0) {
        unsigned int start = 0;
        while (start < (unsigned int) this->length &&
               (this->data[start] == ' ' || this->data[start] == '\t'))
            start++;
        unsigned int end = (unsigned int) this->length;
        while (this->data[end - 1] == '\t' || this->data[end - 1] == ' ')
            end--;
        String trimmed = SubString(start, end);
        Set(trimmed.data);
    }
    return this;
}

int String::GetStringLength(const char *s) {
    const char *p = s;
    while (*p++ != '\0') {
    }
    return (int) (p - s) - 1;
}

String *String::ToLowerCase() {
    if (this->data == nullptr)
        return this;
    for (unsigned int i = 0; i < (unsigned int) this->length; i++) {
        unsigned short c = this->data[i];
        if ((unsigned short) (c - 'A') < 26 || (unsigned short) (c - 0xc0) < 31) {
            this->data[i] = (unsigned short) (c + 0x20);
        } else {
            switch (c) {
                case 0x9a: this->data[i] = 0x81;
                    break;
                case 0x90: this->data[i] = 0x82;
                    break;
                case 0x8e: this->data[i] = 0x84;
                    break;
                case 0x8f: this->data[i] = 0x86;
                    break;
                case 0x92: this->data[i] = 0x91;
                    break;
                case 0x99: this->data[i] = 0x94;
                    break;
                case 0xa5: this->data[i] = 0xa4;
                    break;
                default:
                    break;
            }
        }
    }    return this;
}

String &String::operator=(const String &other) {
    if (other.data != nullptr)
        this->Set(other.data);
    return *this;
}

void String::Set(const unsigned short *s) {
    if (this->data) delete[] this->data;
    this->data = nullptr;
    if (s == nullptr) {
        Set("");
        return;
    }
    int len = StrLen(s);
    this->length = len;
    if (len == 0) {
        this->data = new unsigned short[1];
        this->data[0] = 0;
    } else if (s[len - 1] != 0) {
        unsigned short *out = new unsigned short[len + 1];
        this->data = out;
        memcpy(out, s, len * 2);
        out[len] = 0;
    } else {
        unsigned short *out = new unsigned short[len];
        this->data = out;
        memcpy(out, s, len * 2);
        out[len - 1] = 0;
        this->length = len - 1;
    }
}

void String::ConvertFromUTF8() {
    if (this->data == nullptr)
        return;

    char *bytes = GetAEChar();
    uint16_t *wide = String::getWCharFromUtf8(GetAEChar(), this->length);
    Set(wide);
    delete[] bytes;
    delete[] wide;
}

Array<String *> *String::SplitTags(String tag) {
    if (this->data != nullptr && this->length != 0 && tag.length != 0) {
        tag = String("<") + tag + String(">");
        Array<String *> *arr = new Array<String *>();

        unsigned int endPos = 0;
        unsigned int pos = 0;
        unsigned int idx;
        while ((idx = IndexOf(pos, tag)) != 0xffffffff) {
            if (pos <= idx) {
                ArrayAdd(new String(SubString(pos, idx)), *arr);
                endPos = IndexOf(tag.length + idx, String("</"));
                if (endPos == 0xffffffff)
                    return arr;
                ArrayAdd(new String(SubString(tag.length + idx, endPos)), *arr);
            }
            pos = endPos + 1;
        }
        if (pos != 0 && pos < (unsigned int) this->length)
            ArrayAdd(new String(SubString(pos, (unsigned int) this->length)), *arr);

        if (arr->size() != 0)
            return arr;

        ArrayRemoveAll(*arr);
        delete arr;
    }
    return nullptr;
}


void String::Set(float v) {
    int exponent;
    int negative;
    unsigned short *digits = computeFloatString(v, 10, &exponent, &negative, 1);
    String result;
    if (exponent <= 0) {
        result.Set("0.");
        for (int i = 0; i > exponent; i--)
            result += String("0");
    }
    result += String(digits, false);
    if (exponent >= 1)
        result = result.SubString(0, exponent) + String(".") + result.SubString(exponent, result.length);
    if (negative != 0)
        result = String("-") + result;
    delete[] digits;
    Set(result.data);
}

int String::StrLen(const unsigned short *s) {
    if (s == 0)
        return 0;
    const unsigned short *p = s;
    while (*p++ != 0) {
    }
    return (int) (p - s) - 1;
}

String String::SubString(unsigned int start, unsigned int end) {
    String sub;
    if (start < end) {
        unsigned int n = end - start;
        if (sub.data) delete[] sub.data;
        sub.data = nullptr;
        sub.data = new unsigned short[n + 1];
        memcpy(sub.data, this->data + start, n * 2);
        sub.length = (int) n;
        sub.data[n] = 0;
        return sub;
    }
    return String();
}

uint16_t *String::getWCharFromUtf8(char *s, int len) {
    int outCount = 0;
    for (int i = 0; i < len; i = i + 1) {
        if (((unsigned char) s[i] & 0xe0) == 0xc0)
            i = i + 1;
        else if (((unsigned char) s[i] & 0xf0) == 0xe0)
            i = i + 2;
        outCount = outCount + 1;
    }

    uint16_t *out = new uint16_t[outCount + 1];
    uint16_t *w = out;

    for (int i = 0; i < len; i = i + 1) {
        unsigned short cp = (unsigned short) (unsigned char) s[i];
        unsigned short val = cp;
        if ((cp & 0xe0) == 0xc0) {
            unsigned short hi = cp & 0x1f;
            int j = i + 1;
            val = (unsigned short) ((hi << 6) | ((unsigned char) s[j] & 0x3f));
            i = j;
        } else if ((cp & 0xf0) == 0xe0) {
            int j = i + 2;
            unsigned short mid = (unsigned short) (((unsigned char) s[i + 1] & 0x3f) | ((cp & 0xf) << 6));
            val = (unsigned short) ((mid << 6) | ((unsigned char) s[j] & 0x3f));
            i = j;
        }
        *w = val;
        w++;
    }
    out[outCount] = 0;

    for (int i = 0; i != outCount; i = i + 1) {
        unsigned short c = out[i];
        unsigned short r = c;
        switch (c) {
            case 0x410: r = 0x41;
                break;
            case 0x412: r = 0x42;
                break;
            case 0x415: r = 0x45;
                break;
            case 0x41a: r = 0x4b;
                break;
            case 0x41c: r = 0x4d;
                break;
            case 0x41d: r = 0x48;
                break;
            case 0x41e: r = 0x4f;
                break;
            case 0x420: r = 0x50;
                break;
            case 0x421: r = 0x43;
                break;
            case 0x422: r = 0x54;
                break;
            case 0x425: r = 0x58;
                break;
            case 0x411:
            case 0x413:
            case 0x414:
            case 0x416:
            case 0x417:
            case 0x418:
            case 0x419:
            case 0x41b:
            case 0x41f:
            case 0x423:
            case 0x424:
                break;
            case 0x43e:
            case 0xba: r = 0x6f;
                break;
            case 0x440: r = 0x70;
                break;
            case 0x441: r = 0x63;
                break;
            case 0x445: r = 0x78;
                break;
            case 0x43f:
            case 0x442:
            case 0x443:
            case 0x444:
                break;
            case 0x435: r = 0x65;
                break;
            case 0xaa:
            case 0x430: r = 0x61;
                break;
            case 0x60: r = 0x27;
                break;
            default:
                break;
        }
        out[i] = r;
    }
    return out;
}

unsigned int String::IndexOf(const String &needle) {
    return this->IndexOf(0, needle);
}

unsigned int String::IndexOf(unsigned int start, const String &needle) {
    const unsigned short *nd = needle.data;
    unsigned int nlen = (unsigned int) needle.length;
    unsigned int slen = (unsigned int) this->length;
    while (start < slen) {
        if (nlen > slen - start)
            return 0xffffffff;
        if (nd[0] != this->data[start]) {
            start++;
            continue;
        }
        unsigned int k = 0;
        while (nd[k] == this->data[start + k]) {
            if (k + 1 >= nlen)
                return start;
            k++;
        }
        start += k;
    }
    return 0xffffffff;
}

const unsigned short *String::operator[](int i) const {
    if (i < 0 || i >= this->length)
        return &termChar;
    return this->data + i;
}

const unsigned short *String::GetAEWChar() const {
    return this->data;
}

String::operator unsigned short *() {
    return this->data;
}

String::operator const unsigned short *() const {
    return this->data;
}

void String::PrintOut() {
    char *bytes = ((String *) (this))->GetAEChar();
    String_printImpl(bytes);
}

String::String() : data(nullptr), length(0) {
}

String::String(const char *cstr, bool reverse) : data(nullptr) {
    Set(cstr);
    if (reverse) Reverse();
}

String::String(const uint16_t *wstr, bool reverse) : data(nullptr) {
    Set((const unsigned short *) wstr);
    if (reverse) Reverse();
}

String::String(const String &other, bool reverse) : data(nullptr) {
    Set(other.data);
    if (reverse) Reverse();
}

String &String::operator+=(const String &other) {
    if (other.length != 0) {
        String tmp(other);
        int oldLength = this->length;
        this->length = oldLength + tmp.length;
        unsigned short *grown = (unsigned short *) realloc(this->data, this->length * 2 + 2);
        this->data = grown;
        memcpy(grown + oldLength, tmp.data, tmp.length * 2);
        grown[this->length] = 0;
    }
    return *this;
}

String &String::operator+=(const char &c) {
    if (this->length != 0) {
        int oldLength = this->length;
        this->length = oldLength + 1;
        this->data = (unsigned short *) realloc(this->data, (oldLength + 2) * 2);
        this->data[this->length - 1] = (unsigned short) c;
        this->data[this->length] = 0;
    } else {
        this->data = new unsigned short[2];
        this->data[0] = (unsigned short) c;
        this->data[1] = 0;
        this->length = 1;
    }
    return *this;
}

String &String::operator+=(const int &v) {
    long long wide = v;
    return *this += wide;
}

String &String::operator+=(const float &v) {
    String tmp(v);
    if (tmp.length != 0) {
        int oldLength = this->length;
        this->length = oldLength + tmp.length;
        this->data = (unsigned short *) realloc(this->data, this->length * 2 + 2);
        memcpy(this->data + oldLength, tmp.data, tmp.length * 2);
    }
    return *this;
}

String &String::operator+=(const long long &v) {
    String tmp(v);
    if (tmp.length != 0) {
        int oldLength = this->length;
        this->length = oldLength + tmp.length;
        this->data = (unsigned short *) realloc(this->data, this->length * 2 + 2);
        memcpy(this->data + oldLength, tmp.data, tmp.length * 2);
    }
    return *this;
}

unsigned short AbyssEngine::String::termChar;

namespace AbyssEngine {
    String operator+(const String &a, const String &b) {
        String result(a);
        result += b;
        return String(result);
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const long long &b) {
        String result(a);
        result += String(b);
        return String(result);
    }
}

namespace AbyssEngine {
    String operator+(const float &a, const String &b) {
        String result(a);
        result += b;
        return String(result);
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const int &b) {
        String result(a);
        result += String(b);
        return String(result);
    }
}

namespace AbyssEngine {
    String operator+(const String &a) {
        return String(a);
    }
}

namespace AbyssEngine {
    String operator+(const int &a, const String &b) {
        String result(a);
        result += b;
        return String(result);
    }
}

namespace AbyssEngine {
    float MODF(float value, float *intPart) {
        float ip = (float) (int) value;
        *intPart = ip;
        return value - ip;
    }
}

namespace AbyssEngine {
    String operator+(const long long &a, const String &b) {
        String result(a);
        result += b;
        return String(result);
    }
}

namespace AbyssEngine {
    bool operator==(const String &a, const String &b) {
        String copy(a);
        return copy.Compare(b) == 0;
    }
}

namespace AbyssEngine {
    String operator+(const String &a, const float &b) {
        String result(a);
        result += String(b);
        return String(result);
    }
}

namespace AbyssEngine {
    unsigned short *computeFloatString(float value, int precision, int *decimalPoint, int *negative, int fixed) {
        unsigned short *buf = new unsigned short[33];
        if (precision < 0)
            precision = 0;
        if (precision >= 30)
            precision = 30;
        *negative = 0;
        if (value < 0.0f) {
            *negative = 1;
            value = -value;
        }
        float integral;
        int exponent = 0;
        unsigned short *out;

        float fraction = MODF(value, &integral);
        if (integral != 0.0f) {
            unsigned short *end = buf + 32;
            unsigned short *p = buf + 31;
            while (integral != 0.0f) {
                float digit = MODF(integral / 10.0f, &integral);
                exponent++;
                *p-- = (unsigned short) ((int) ((digit + 0.03) * 10.0) + '0');
            }
            unsigned short *src = p + 1;
            out = buf;
            while (src < end)
                *out++ = *src++;
        } else {
            if (fraction > 0.0f) {
                // Scale the fraction up until its first digit is non-zero.
                float prev;
                exponent = 1;
                do {
                    prev = fraction;
                    exponent--;
                    fraction *= 10.0f;
                } while (fraction < 1.0f);
                fraction = prev;
            }
            out = buf;
        }
        *decimalPoint = exponent;

        unsigned short *limit = buf + precision;
        if (fixed == 0)
            limit += exponent;
        if (limit < buf) {
            *buf = 0;
            return buf;
        }
        while (out <= limit && out < buf + 32) {
            float digit;
            fraction = MODF(fraction * 10.0f, &digit);
            *out++ = (unsigned short) ((int) digit + '0');
        }
        if (limit >= buf + 32) {
            buf[31] = 0;
            return buf;
        }

        unsigned short *p = limit;
        unsigned short v = (unsigned short) (*limit + 5);
        *limit = v;
        while (v > '9') {
            *p = '0';
            if (p > buf) {
                p--;
                v = (unsigned short) (*p + 1);
                *p = v;
            } else {
                *p = '1';
                exponent++;
                *decimalPoint = exponent;
                if (fixed == 0) {
                    if (limit > buf)
                        *limit = '0';
                    limit++;
                }
                v = *p;
            }
        }
        *limit = 0;
        return buf;
    }
}
