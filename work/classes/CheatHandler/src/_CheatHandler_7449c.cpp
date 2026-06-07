#include "class.h"

namespace AbyssEngine {

CheatHandler::~CheatHandler()
{
    Array<CheatCode *> *a = codes;
    if (a != 0) {
        uint32_t i = 0;
        int off = 0;
        while (i < a->size) {
            CheatCode *c = *(CheatCode **)((int)a->data + off);
            if (c != 0) {
                delete c;
                *(uint32_t *)((char *)codes->data + off) = 0;
            } else {
                a->data[i] = 0;
            }
            off = off + 4;
            i = i + 1;
            a = codes;
        }
        ArrayRemoveAll<CheatCode *>(*a);
        if (codes != 0) {
            delete codes;
        }
        codes = 0;
    }
}

} // namespace AbyssEngine
