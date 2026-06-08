#include "class.h"

extern "C" int ListItem_isSelectable(void *item);
extern "C" int ListItem_isTextButton(void *item);
extern "C" int ListItem_isShip(void *item);
extern "C" void Ship_adjustPrice(void *ship);
// FModSound singleton (pc-rel -> holder; *holder is the FModSound object).
__attribute__((visibility("hidden"))) extern void **g_hw_sound;
extern "C" void FModSound_play(void *snd, int id, void *a, void *b, void *c);

extern "C" int HangarWindow_highlightItem(HangarWindow *self, void *item)
{
    if (item != 0 && ListItem_isSelectable(item) != 0) {
        FModSound_play(*g_hw_sound, 0x7c, 0, 0, 0);
        unsigned flag = 0;
        if (self->f_68 != item) {
            flag = ListItem_isTextButton(item) ^ 1;
        }
        self->f_68 = item;
        F<uint8_t>(self, 0xd2) = flag;
        if (ListItem_isShip(item) != 0) {
            Ship_adjustPrice(G<void *>(self->f_68, 0xc));
        }
    }
    return 0;
}
