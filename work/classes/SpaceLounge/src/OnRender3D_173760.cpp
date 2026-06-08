#include "class.h"

extern "C" void SpaceLounge_OnRender3D_map_tail(void *map);
extern "C" int ListItemWindow_shows3DShip(void *item);
extern "C" void SpaceLounge_OnRender3D_cutscene_tail(void *cutscene);

extern "C" void SpaceLounge_OnRender3D(SpaceLounge *self)
{
    if (UC(self, 0x34) != 0) {
        return SpaceLounge_OnRender3D_map_tail(self->f_4);
    }

    void *cutscene = self->f_44;
    if (cutscene == 0) {
        return;
    }

    if (UC(self, 0x1c) != 0) {
        if (ListItemWindow_shows3DShip(self->f_c) != 0) {
            return;
        }
        cutscene = self->f_44;
    }

    return SpaceLounge_OnRender3D_cutscene_tail(cutscene);
}
