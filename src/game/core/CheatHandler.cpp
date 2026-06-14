#include <new>
#include "gof2/game/core/CheatHandler.h"
#include "gof2/engine/core/KeyCode.h"

namespace AbyssEngine {

void CheatHandler::SetCheatFunc(CheatFunc f, void *ctx)
{
    func = f;
    context = ctx;
}

} // namespace AbyssEngine

namespace AbyssEngine {

CheatHandler::CheatHandler(KeyCode *k)
{
    func = 0;
    context = 0;
    codes = new Array<CheatCode *>();
    keys = k;
}

} // namespace AbyssEngine

namespace AbyssEngine {

CheatHandler::~CheatHandler()
{
    Array<CheatCode *> *a = codes;
    if (a != 0) {
        uint32_t i = 0;
        int off = 0;
        while (i < a->size()) {
            CheatCode *c = *(CheatCode **)((char *)a->data() + off);
            if (c != 0) {
                delete c;
                *(uint32_t *)((char *)codes->data() + off) = 0;
            } else {
                a->data()[i] = 0;
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

namespace AbyssEngine {

void CheatHandler::AddCheatCode(const String &code, int value)
{
    CheatCode *c = new CheatCode();
    c->value = value;
    ArrayAdd<CheatCode *>(c, *codes);

    for (uint32_t i = 0; i < code.size(); i = i + 1) {
        uint16_t ch = (uint16_t)code.s[i];
        switch (ch) {
        case '0':
            ArrayAdd<uint16_t>((uint16_t)keys[0].code, *c->keys);
            break;
        case '2':
            ArrayAdd<uint16_t>((uint16_t)keys[2].code, *c->keys);
            break;
        case '3':
            ArrayAdd<uint16_t>((uint16_t)keys[3].code, *c->keys);
            break;
        case '4':
            ArrayAdd<uint16_t>((uint16_t)keys[4].code, *c->keys);
            break;
        case '7':
            ArrayAdd<uint16_t>((uint16_t)keys[7].code, *c->keys);
            break;
        case '9':
            ArrayAdd<uint16_t>((uint16_t)keys[9].code, *c->keys);
            break;
        case '1':
            ArrayAdd<uint16_t>((uint16_t)keys[1].code, *c->keys);
            break;
        case '5':
            ArrayAdd<uint16_t>((uint16_t)keys[5].code, *c->keys);
            break;
        case '6':
            ArrayAdd<uint16_t>((uint16_t)keys[6].code, *c->keys);
            break;
        case '8':
            ArrayAdd<uint16_t>((uint16_t)keys[8].code, *c->keys);
            break;
        default:
            break;
        }
    }
}

} // namespace AbyssEngine

namespace AbyssEngine {

void CheatHandler::Update(uint16_t key)
{
    if (func != 0) {
        for (uint32_t i = 0; i < codes->size(); i = i + 1) {
            if (codes->data()[i]->Update(key)) {
                func(codes->data()[i]->value, context);
            }
        }
    }
}

} // namespace AbyssEngine

// Free-function entries used by callers that hold the handler storage opaquely
// (placement-construct into `cheats` with the digit->keycode table; the dtor runs
// the in-place teardown that releases the cheat-code array).
extern "C" void CheatHandler_ctor(void *cheats, void *keys)
{
    new (cheats) AbyssEngine::CheatHandler((AbyssEngine::KeyCode *)keys);
}

extern "C" void CheatHandler_dtor(void *cheats)
{
    ((AbyssEngine::CheatHandler *)cheats)->~CheatHandler();
}
