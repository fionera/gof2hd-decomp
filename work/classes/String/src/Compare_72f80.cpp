#include "class.h"

// AbyssEngine::String::Compare(AbyssEngine::String const&)
// Returns 0 when equal; a small signed value otherwise (0xff sentinel for length mismatch).
extern "C" int String_Compare_str(String *self, String *other)
{
    short result;
    if (F<int>(other, 0x8) == F<int>(self, 0x8)) {
        short *op = F<short *>(other, 0x4);
        short *sp = F<short *>(self, 0x4);
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
