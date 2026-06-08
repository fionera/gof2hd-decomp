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

    if (this->f_48 == 0) {
        return 0;
    }

    this->f_218 = 0;
    this->f_130 = 0;

    void *canvas = *(void **)gRadarCanvasForDraw;
    Radar_SetColor(canvas, -1);

    void *mission = *(void **)gRadarMissionSlot;
    int missionState = Radar_GetMissionState(mission);
    if (missionState == 0 && Radar_GetMissionType(mission) == 0) {
        this->f_218 = (uint8_t)mode;
    }

    return 0;
}
