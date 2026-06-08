#include "class.h"

extern "C" void ChoiceWindow_OnTouchBegin(void *choice, int x, int y);
extern "C" void StarMap_OnTouchBegin(void *map, int x, int y);
extern "C" void ListItemWindow_OnTouchBegin(void *list, int x, int y);
extern "C" void TouchButton_OnTouchBegin(void *button, int x, int y);
extern "C" void Layout_OnTouchBegin(void *layout, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchBegin(void *scroll, int x, int y);

extern "C" void *SpaceLounge_layout_begin;

extern "C" int SpaceLounge_OnTouchBegin(SpaceLounge *self, int x, int y)
{
    UC(self, 0xb2) = 1;
    I(self, 0xb4) = x;
    I(self, 0xb8) = y;
    I(self, 0x88) = -1;

    if (UC(self, 0x1b) != 0 || UC(self, 0x19) != 0) {
        ChoiceWindow_OnTouchBegin(P(self, 0x8), x, y);
        return 0;
    }

    if (UC(self, 0x34) != 0) {
        StarMap_OnTouchBegin(P(self, 0x4), x, y);
        return 0;
    }

    if (UC(self, 0x1c) != 0) {
        ListItemWindow_OnTouchBegin(P(self, 0xc), x, y);
        Layout_OnTouchBegin(*(void **)SpaceLounge_layout_begin, x, y);
        ScrollTouchWindow_OnTouchBegin(P(self, 0x60), x, y);
        return 0;
    }

    int mode = I(self, 0x14);
    if (mode == 0) {
        if (UC(self, 0xbd) == 0) {
            return 0;
        }
        void *items = P(self, 0x24);
        if (items != 0) {
            int i = 0;
            float fy = (float)y;
            float fx = (float)x;
            int count = I(items, 0x0);
            for (; count != i; ++i) {
                char *data = (char *)P(P(self, 0x40), 0x4);
                float *a = *(float **)(data + i * 8);
                if (a[0] < fx) {
                    float *b = *(float **)(data + i * 8 + 4);
                    if (fx < b[0] && fy < a[1] && b[1] < fy) {
                        I(self, 0x88) = i;
                    }
                }
            }
        }
    } else if (mode == 3) {
        void *buttons = P(self, 0x5c);
        TouchButton_OnTouchBegin(*(void **)P(buttons, 0x4), x, y);
    } else if (mode == 2) {
        unsigned i = 0;
        for (; i < U(P(self, 0x5c), 0x0); ++i) {
            void **data = (void **)P(P(self, 0x5c), 0x4);
            TouchButton_OnTouchBegin(data[i], x, y);
        }
    }

    Layout_OnTouchBegin(*(void **)SpaceLounge_layout_begin, x, y);
    ScrollTouchWindow_OnTouchBegin(P(self, 0x60), x, y);
    return 0;
}
