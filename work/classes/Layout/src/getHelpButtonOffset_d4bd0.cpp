#include "class.h"

__attribute__((visibility("hidden"))) extern int **gTouchButtonHolder;  // ldr [0xe4bec]
extern "C" int TouchButton_getWidth(int id);   // blx 0x74da0

// Layout::getHelpButtonOffset() -> TouchButton::getWidth(self->f3bc) - (**holder)[0x38/4]
extern "C" int Layout_getHelpButtonOffset(Layout *self) {
    int w = TouchButton_getWidth(F<int>(self, 0x3bc));
    return w - (*gTouchButtonHolder)[0x38 / 4];
}
