#include "engine/render/LODManager.h"

#include <cstdlib>

// The original touch TU (its empty .text starts at 0x950dc, the address of AddTouch): the touch
// globals and the four touch accessors.  Its second .init_array entry allocates a one-entry
// buffer at static-init time (touches = realloc(touches, 16)); that initializer lives in
// LODManager.cpp, which is the next TU.
Touch *touches;
int maxTouchSize = 0;
int curTouchSize = 0;

void AddTouch(int x, int y, int id, int action) {
    int index = curTouchSize;
    curTouchSize = index + 1;

    Touch *buf;
    if (index < maxTouchSize) {
        buf = touches;
    } else {
        maxTouchSize = index + 1;
        buf = static_cast<Touch *>(realloc(touches, (index + 1) * sizeof(Touch)));
        touches = buf;
    }

    // The original computes the x slot before reloading curTouchSize after realloc; keeping the
    // two expressions separate reproduces that instruction schedule.
    buf[curTouchSize - 1].x = x;
    Touch *slot = &buf[curTouchSize - 1];
    slot->y = y;
    slot->id = id;
    slot->action = action;
}

int GetTouchCount() {
    return curTouchSize;
}

void RemoveTouches() {
    curTouchSize = 0;
}

Touch GetTouch(int index) {
    return touches[index];
}
