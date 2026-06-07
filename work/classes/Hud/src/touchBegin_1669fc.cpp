#include "class.h"

extern "C" unsigned int Hud_touchedElement(Hud *self, unsigned int a, unsigned int b);

extern "C" unsigned int Hud_touchBegin(Hud *self, unsigned int a, void *b, int key)
{
    unsigned int e = Hud_touchedElement(self, a, (unsigned int)b);
    if (e == 0) {
        for (int i = 0; i != 0x19; i = i + 1) {
            int arr = I((void *)I(self, 0x28c), 4);
            if (I((void *)arr, i * 4) == key) {
                U(self, 0x284) = U(self, 0x284) & ~U((void *)I(self, 0x290), i * 4);
                I((void *)I(self, 0x290), i * 4) = 0;
                I((void *)arr, i * 4) = 0;
            }
        }
    } else {
        int arr = I((void *)I(self, 0x28c), 4);
        unsigned int j;
        for (j = 0; j < 0x19; j = j + 1) {
            if (I((void *)arr, j * 4) == key) {
                unsigned int v = U((void *)I(self, 0x290), j * 4);
                if (e == v)
                    v = U(self, 0x284);
                else
                    v = U(self, 0x284) & ~v;
                U(self, 0x284) = v | e;
                U((void *)I(self, 0x290), j * 4) = e;
                goto done;
            }
        }
        for (j = 0; j < 0x19; j = j + 1) {
            if (I((void *)arr, j * 4) == 0) {
                I((void *)arr, j * 4) = key;
                U((void *)I(self, 0x290), j * 4) = e;
                U(self, 0x284) = e | U(self, 0x284);
                break;
            }
        }
    }
done:
    return U(self, 0x284);
}
