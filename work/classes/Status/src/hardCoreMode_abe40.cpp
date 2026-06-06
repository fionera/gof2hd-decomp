#include "class.h"
struct HardCoreHolder { char pad[0x2c]; float difficulty; };
__attribute__((visibility("hidden"))) extern HardCoreHolder *g_hardCoreHolder;
bool Status::hardCoreMode() {
    return g_hardCoreHolder->difficulty == 1.5f;
}
