#include "class.h"

extern "C" void SpaceLounge_OnRender3D_map_tail(void *map);
extern "C" int ListItemWindow_shows3DShip(void *item);
extern "C" void SpaceLounge_OnRender3D_cutscene_tail(void *cutscene);

extern "C" void SpaceLounge_OnRender3D(SpaceLounge *self)
{
    if (UC(self, 0x34) != 0) {
        return SpaceLounge_OnRender3D_map_tail(P(self, 0x4));
    }

    void *cutscene = P(self, 0x44);
    if (cutscene == 0) {
        return;
    }

    if (UC(self, 0x1c) != 0) {
        if (ListItemWindow_shows3DShip(P(self, 0xc)) != 0) {
            return;
        }
        cutscene = P(self, 0x44);
    }

    return SpaceLounge_OnRender3D_cutscene_tail(cutscene);
}
