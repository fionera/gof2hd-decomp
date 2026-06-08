#include "class.h"

extern "C" void ChoiceWindow_OnTouchMove(void *choice, int x, int y);
extern "C" void StarMap_OnTouchMove(void *map, int x, int y);
extern "C" void TouchButton_OnTouchMove(void *button, int x, int y);
extern "C" void Layout_OnTouchMove(void *layout, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchMove(void *scroll, int x, int y);
extern "C" void ListItemWindow_OnTouchMove(void *list, int x, int y);

extern "C" void *SpaceLounge_layout_move;

extern "C" int SpaceLounge_OnTouchMove(SpaceLounge *self, int x, int y)
{
    I(self, 0xb4) = x;
    I(self, 0xb8) = y;

    if (UC(self, 0x1b) != 0 || UC(self, 0x19) != 0) {
        ChoiceWindow_OnTouchMove(self->f_8, x, y);
        return 0;
    }

    if (UC(self, 0x34) != 0) {
        StarMap_OnTouchMove(self->f_4, x, y);
        return 0;
    }

    int mode = I(self, 0x14);
    if (mode == 0) {
        void *items = self->f_24;
        if (items != 0) {
            float fy = (float)y;
            float fx = (float)x;
            I(self, 0x88) = -1;
            I(self, 0x20) = -1;
            int i = 0;
            int count = I(items, 0x0);
            for (; count != i; ++i) {
                char *data = (char *)P(self->f_40, 0x4);
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
        void *buttons = self->f_5c;
        TouchButton_OnTouchMove(*(void **)buttons->f_4, x, y);
    } else if (mode == 2) {
        unsigned i = 0;
        for (; i < U(self->f_5c, 0x0); ++i) {
            void **data = (void **)P(self->f_5c, 0x4);
            TouchButton_OnTouchMove(data[i], x, y);
        }
    }

    Layout_OnTouchMove(*(void **)SpaceLounge_layout_move, x, y);
    if (UC(self, 0x1c) != 0) {
        ListItemWindow_OnTouchMove(self->f_c, x, y);
    } else {
        ScrollTouchWindow_OnTouchMove(self->f_60, x, y);
    }
    return 0;
}
