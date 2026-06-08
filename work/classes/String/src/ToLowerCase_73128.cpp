#include "class.h"

// AbyssEngine::String::ToLowerCase() - lowercase ASCII plus a CP-1252 accented range.
extern "C" void String_ToLowerCase(String *self)
{
    short *p = F<short *>(self, 0x4);
    if (p == 0)
        return;
    int count = F<int>(self, 0x8);
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
