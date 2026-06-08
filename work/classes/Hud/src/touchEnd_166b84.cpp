#include "class.h"

extern "C" void TouchButton_OnTouchEnd(int self, unsigned int a, void *b);

extern "C" unsigned int Hud_touchEnd(Hud *self, unsigned int a, void *b, int key)
{
    int i = 0;
    unsigned int ret = 0;
    for (; i != 0x19; i = i + 1) {
        int arr = I((void *)I(self, 0x28c), 4);
        if (I((void *)arr, i * 4) == key) {
            ret = U((void *)I(self, 0x290), i * 4);
            U(self, 0x284) = U(self, 0x284) & ~ret;
            I((void *)arr, i * 4) = 0;
            I((void *)I(self, 0x290), i * 4) = 0;
        }
    }
    if (UC(self, 0x282) != 0) {
        unsigned int *btns = (unsigned int *)P(self, 0x18);
        if (btns != 0) {
            for (unsigned int j = 0; j < btns[0]; j = j + 1) {
                TouchButton_OnTouchEnd(((int *)btns[1])[j], a, b);
                btns = (unsigned int *)P(self, 0x18);
            }
        }
    }
    return ret;
}
