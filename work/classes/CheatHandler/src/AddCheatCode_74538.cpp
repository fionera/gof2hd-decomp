#include "class.h"

namespace AbyssEngine {

void CheatHandler::AddCheatCode(const String &code, int value)
{
    CheatCode *c = new CheatCode();
    c->value = value;
    ArrayAdd<CheatCode *>(c, *codes);

    for (uint32_t i = 0; i < code.size; i = i + 1) {
        uint16_t ch = *((String &)code)[i];
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
