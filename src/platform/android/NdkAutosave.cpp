#include "engine/render/LODManager.h"

#include <cstdlib>

// The original touch TU (its empty .text starts at 0x950dc, the address of AddTouch): the touch
// globals, the four touch accessors and the second .init_array entry, which allocates a
// one-entry buffer at static-init time (touches = realloc(touches, 16)).  The initializer must
// live in this TU: its `.ARM.extab.text.startup` record is emitted after AddTouch's record and
// before the next TU's, which is what places the record at the original 0x1a768c (see
// _work/reconstructions/exidx_reconcile_w103_wm201.md).
Touch *touches;
int maxTouchSize = 0;
int curTouchSize = 0;

namespace {
struct TouchBufferInit {
    TouchBufferInit() { touches = static_cast<Touch *>(realloc(touches, sizeof(Touch))); }
};
TouchBufferInit touchBufferInit;
}

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
