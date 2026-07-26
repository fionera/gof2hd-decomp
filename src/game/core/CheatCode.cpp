#include "game/core/CheatCode.h"
#include "game/core/CheatHandler.h"
#include "engine/core/KeyCode.h"

namespace AbyssEngine {
    CheatCode::CheatCode() {
        keys = new Array<uint16_t>();
        value = 0;
        pos = 0;
    }

    CheatCode::~CheatCode() {
        delete keys;
        keys = nullptr;
    }

    bool CheatCode::Update(uint16_t key) {
        Array<uint16_t> *a = keys;
        uint32_t p = pos;
        uint16_t *d = a->data();
        uint32_t n = a->size();
        if (p >= n) {
            return false;
        }
        if (d[p] != key) {
            bool first = (d[0] == key);
            uint32_t one = (uint32_t) first;
            pos = (first && n != one);
            return n == one;
        }
        p = p + 1;
        uint32_t np = p;
        if (p == n) {
            np = 0;
        }
        pos = np;
        return p == n;
    }

    CheatHandler::CheatHandler(KeyCode *k) {
        func = 0;
        context = 0;
        codes = new Array<CheatCode *>();
        keys = k;
    }

    CheatHandler::~CheatHandler() {
        if (codes != 0) {
            for (uint32_t i = 0; i < codes->size(); i = i + 1) {
                CheatCode *c = codes->data()[i];
                delete c;
                codes->data()[i] = 0;
            }
            ArrayRemoveAll<CheatCode *>(*codes);
            delete codes;
            codes = 0;
        }
    }

    void CheatHandler::SetCheatFunc(CheatFunc f, void *ctx) { // lint: void_ptr (exported method signature)
        // lint: void_ptr (exported method signature)
        func = f;
        context = ctx;
    }

    void CheatHandler::AddCheatCode(const String &code, int value) {
        CheatCode *c = new CheatCode();
        c->value = value;
        ArrayAdd<CheatCode *>(c, *codes);

        for (uint32_t i = 0; i < code.size(); i = i + 1) {
            uint16_t ch = *(code[(int)i]);
            switch (ch) {
                case '0':
                    ArrayAdd<uint16_t>((uint16_t) keys[0].code, *c->keys);
                    break;
                case '1':
                    ArrayAdd<uint16_t>((uint16_t) keys[1].code, *c->keys);
                    break;
                case '2':
                    ArrayAdd<uint16_t>((uint16_t) keys[2].code, *c->keys);
                    break;
                case '3':
                    ArrayAdd<uint16_t>((uint16_t) keys[3].code, *c->keys);
                    break;
                case '4':
                    ArrayAdd<uint16_t>((uint16_t) keys[4].code, *c->keys);
                    break;
                case '5':
                    ArrayAdd<uint16_t>((uint16_t) keys[5].code, *c->keys);
                    break;
                case '6':
                    ArrayAdd<uint16_t>((uint16_t) keys[6].code, *c->keys);
                    break;
                case '7':
                    ArrayAdd<uint16_t>((uint16_t) keys[7].code, *c->keys);
                    break;
                case '8':
                    ArrayAdd<uint16_t>((uint16_t) keys[8].code, *c->keys);
                    break;
                case '9':
                    ArrayAdd<uint16_t>((uint16_t) keys[9].code, *c->keys);
                    break;
                default:
                    break;
            }
        }
    }

    void CheatHandler::Update(uint16_t key) {
        if (func != 0) {
            for (uint32_t i = 0; i < codes->size(); i = i + 1) {
                if (codes->data()[i]->Update(key)) {
                    func(codes->data()[i]->value, context);
                }
            }
        }
    }
}
