#include "class.h"

extern uint8_t *gRadarDrawCurrentLockFlag;
extern "C" void Radar_DrawImage2D(void *canvas, int image, int x, int y);
extern "C" void Radar_StringText(void *self, void const *text, bool copy);
extern "C" void Radar_StringDefault(void *self);
extern "C" void Radar_StringAssign(void *self, void const *rhs);
extern "C" void Radar_StringDtor(void *self);

void Radar::drawCurrentLock(Hud *)
{
    char text34[12];
    char text40[12];
    char text4c[12];
    char text58[12];
    char text64[12];
    char text70[12];
    char text7c[12];
    char text88[12];

    if (F<uint8_t>(this, 0x48) == 0) {
        return;
    }

    *gRadarDrawCurrentLockFlag = 1;

    if (F<int>(this, 0x14) == 0) {
        void *locked = F<void *>(this, 0x0c);
        if (locked == 0) {
            locked = F<void *>(this, 0x38);
            if (locked == 0) {
                locked = F<void *>(this, 0x04);
                if (locked == 0) {
                    locked = F<void *>(this, 0x24);
                    if (locked == 0) {
                        *gRadarDrawCurrentLockFlag = 0;
                        return;
                    }
                }
            }
        }

        Radar_StringDefault(text40);
        if (locked == F<void *>(this, 0x24)) {
            Radar_StringAssign(text40, text34);
        }
        Radar_StringDtor(text40);
    } else {
        Radar_StringText(text34, F<void *>(this, 0x18c), false);
    }

    Radar_StringDtor(text34);
    (void)text4c;
    (void)text58;
    (void)text64;
    (void)text70;
    (void)text7c;
    (void)text88;
}
