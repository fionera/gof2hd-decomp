#include <new>
#include "gof2/game/core/CheatHandler.h"




// ---- SetCheatFunc_74656.cpp ----
namespace AbyssEngine {

void CheatHandler::SetCheatFunc(CheatFunc f, void *ctx)
{
    func = f;
    context = ctx;
}

} // namespace AbyssEngine

// ---- CheatHandler_74448.cpp ----
namespace AbyssEngine {

CheatHandler::CheatHandler(KeyCode *k)
{
    func = 0;
    context = 0;
    codes = new Array<CheatCode *>();
    keys = k;
}

} // namespace AbyssEngine

// ---- _CheatHandler_7449c.cpp ----
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

// ---- AddCheatCode_74538.cpp ----
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
            ArrayAdd<uint16_t>(*(uint16_t *)((char *)keys + 0x00), *c->keys);
            break;
        case '2':
            ArrayAdd<uint16_t>(*(uint16_t *)((char *)keys + 0x20), *c->keys);
            break;
        case '3':
            ArrayAdd<uint16_t>(*(uint16_t *)((char *)keys + 0x30), *c->keys);
            break;
        case '4':
            ArrayAdd<uint16_t>(*(uint16_t *)((char *)keys + 0x40), *c->keys);
            break;
        case '7':
            ArrayAdd<uint16_t>(*(uint16_t *)((char *)keys + 0x70), *c->keys);
            break;
        case '9':
            ArrayAdd<uint16_t>(*(uint16_t *)((char *)keys + 0x90), *c->keys);
            break;
        case '1':
            ArrayAdd<uint16_t>(*(uint16_t *)((char *)keys + 0x10), *c->keys);
            break;
        case '5':
            ArrayAdd<uint16_t>(*(uint16_t *)((char *)keys + 0x50), *c->keys);
            break;
        case '6':
            ArrayAdd<uint16_t>(*(uint16_t *)((char *)keys + 0x60), *c->keys);
            break;
        case '8':
            ArrayAdd<uint16_t>(*(uint16_t *)((char *)keys + 0x80), *c->keys);
            break;
        default:
            break;
        }
    }
}

} // namespace AbyssEngine

// ---- Update_7465c.cpp ----
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
