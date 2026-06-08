#include "class.h"

// AbyssEngine::String::ToUpperCase() - uppercase ASCII plus a CP-1252 accented range.
extern "C" void String_ToUpperCase(String *self)
{
    short *p = F<short *>(self, 0x4);
    if (p == 0)
        return;
    int count = F<int>(self, 0x8);
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
