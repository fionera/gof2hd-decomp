#include "class.h"

extern "C" int Mission_hasFailed(Mission *m);
extern "C" int Mission_getType(Mission *m);

// Returns the first failed mission, or a type-0xd mission flagged failable (byte at 0xf1).
int Status::missionFailed(bool param_1, int64_t time) {
    for (unsigned i = 0; i < missions->size; i = i + 1) {
        Mission *cur = ((Mission **)missions->data)[i];
        if (Mission_hasFailed(cur) != 0) {
            return 0;
        }
        if (cur != 0 && Mission_getType(cur) == 0xd && param_1 &&
            *((uint8_t *)this + 0xf1) != 0) {
            return (int)(uint32_t)cur;
        }
    }
    return 0;
}
