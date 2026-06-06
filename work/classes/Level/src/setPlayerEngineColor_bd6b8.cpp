#include "class.h"

__attribute__((visibility("hidden"))) extern int *g_engineColorBase;

void Level::setPlayerEngineColor(short color) {
    int c = color;
    if (player != 0 && field_a4 != 0) {
        unsigned int *p = (unsigned int *)((char *)g_engineColorBase + 0x1254);
        for (int i = *(int *)field_a4; i != 0; i = i - 1) {
            *p = c << 0x10 | c << 0x18 | c << 8 | 0xff;
            p = p + 0x28;
        }
    }
}
