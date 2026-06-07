#include "class.h"

// NOTE: the work-item lists n=36 with a garbled "body 12" decompile, but the real
// target function has a large frame (saves r4-r11, vpush {d8,d9}, sub sp,#80) and a
// body that is not recoverable from the provided Ghidra output. Left as a stub.
extern "C" void Level_createPlayer_impl(Level *self);

void Level::createPlayer() {
    return Level_createPlayer_impl(this);
}
