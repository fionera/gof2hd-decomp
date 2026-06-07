#include "class.h"

typedef int (*TouchHandler)(void *, int, int);

extern "C" int ScrollTouchWindow_OnTouchMove(void *self, int x, int y);
extern "C" int ChoiceWindow_OnTouchMove(void *self, int x, int y);
__attribute__((visibility("hidden"))) extern TouchHandler g_dw_touchButtonOnTouchMove;

extern "C" int DialogueWindow_OnTouchMove(DialogueWindow *self, int x, int y)
{
    if (F<uint8_t>(self, 0x54) != 0) {
        ChoiceWindow_OnTouchMove(F<void *>(self, 0x50), x, y);
    } else {
        ScrollTouchWindow_OnTouchMove(F<void *>(self, 0x40), x, y);
        void *button = F<void *>(self, 0x0);
        TouchHandler fn = g_dw_touchButtonOnTouchMove;
        fn(button, x, y);
        fn(F<void *>(self, 0x4), x, y);
        fn(F<void *>(self, 0x8), x, y);
    }
    return 0;
}
