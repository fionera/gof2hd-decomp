#include "class.h"

extern "C" void Radar_SetColor(void *canvas, int color);
extern "C" int Radar_GetMissionState(void *mission);
extern "C" int Radar_GetMissionType(void *mission);
extern void *gRadarCanvasForDraw;
extern void *gRadarMissionSlot;

long long Radar::draw(Player *, Hud *, int mode)
{
    char scratch[0x120];
    (void)scratch;

    if (F<uint8_t>(this, 0x48) == 0) {
        return 0;
    }

    F<uint8_t>(this, 0x218) = 0;
    F<uint8_t>(this, 0x130) = 0;

    void *canvas = *(void **)gRadarCanvasForDraw;
    Radar_SetColor(canvas, -1);

    void *mission = *(void **)gRadarMissionSlot;
    int missionState = Radar_GetMissionState(mission);
    if (missionState == 0 && Radar_GetMissionType(mission) == 0) {
        F<uint8_t>(this, 0x218) = (uint8_t)mode;
    }

    return 0;
}
