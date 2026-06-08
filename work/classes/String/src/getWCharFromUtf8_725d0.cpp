#include "class.h"

extern "C" void *operator_new__(unsigned int n);

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
