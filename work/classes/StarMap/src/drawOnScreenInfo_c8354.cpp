#include "class.h"

extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_info_screenW;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_info_screenH;
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_info_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_info_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_info_font;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_info_layout;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_info_text;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_info_isGerman;

extern "C" void Vector_assign(Vector *dst, Vector *src);
extern "C" void PaintCanvas_SetColor(uint32_t canvas, uint32_t color);
extern "C" void PaintCanvas_SetColorRGBA(uint32_t canvas, int r, int g, int b, int a);
extern "C" int PaintCanvas_GetTextWidth(uint32_t canvas, String *font);
extern "C" void PaintCanvas_DrawString(uint32_t canvas, void *font, String *text, int x, int y, bool flag);
extern "C" void PaintCanvas_DrawImage2D(uint32_t canvas, uint32_t image, int x, int y, int anchor);
extern "C" void Station_getName(String *out, void *station);
extern "C" int Station_getIndex(void *station);
extern "C" int Station_isDiscovered(void *station);
extern "C" int Station_getTecLevel(void *station);
extern "C" void SolarSystem_getName(String *out, void *system);
extern "C" int SolarSystem_isFullyDiscovered(void *system);
extern "C" int SolarSystem_getIndex(void *system);
extern "C" int SolarSystem_getRace(void *system);
extern "C" int SolarSystem_hasNoOwner(void *system);
extern "C" int SolarSystem_getSecurityLevel(void *system);
extern "C" int SolarSystem_getStationEnumIndex(void *system, int station);
extern "C" int SolarSystem_getWarpGateIndex(void *system);
extern "C" void *Status_getStation(void *status);
extern "C" void *Status_getSystem(void *status);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" void *Status_getCampaignMission(void *status);
extern "C" void *Status_getFreelanceMission(void *status);
extern "C" void *Status_getPendingProducts(void *status);
extern "C" int Status_isFreighterMissionStation(void *status, int station);
extern "C" int Status_getFreighterMissionStationBit(void *status, int station);
extern "C" String *GameText_getText(void *text, int id);
extern "C" int Mission_isEmpty(void *mission);
extern "C" int Mission_getType(void *mission);
extern "C" int Mission_getTargetStation(void *mission);
extern "C" int Mission_getStatusValue(void *mission);
extern "C" void *Mission_getAgent(void *mission);
extern "C" int Agent_getStation(void *agent);
extern "C" int Ship_hasCargo(void *ship, int cargo);
extern "C" void *Status_getShip(void *status);
extern "C" void String_ctor_char(String *self, const char *text, bool copy);
extern "C" void String_ctor_int(String *self, int value);
extern "C" void String_ctor_copy(String *self, String *src, bool copy);
extern "C" void String_add(String *out, String *a, String *b);
extern "C" void String_dtor(String *self);
extern "C" float Layout_getPulseValue(void *layout, float alpha);

void StarMap::drawOnScreenInfo(int index, bool stationMode)
{
    String name;
    String line;
    String value;

    Array<Vector *> *positions =
        stationMode ? (Array<Vector *> *)ptr_field(this, 0x198) : (Array<Vector *> *)ptr_field(this, 0x194);
    Vector_assign((Vector *)((char *)this + 0x78), positions->data[index]);
    float x = field<float>(this, 0x78);
    float y = field<float>(this, 0x7c);
    if (x < 0.0f || x > (float)(*g_StarMap_info_screenW + 0x32) ||
        y < 0.0f || y > (float)(*g_StarMap_info_screenH + 0x32)) {
        return;
    }

    uint32_t canvas = *g_StarMap_info_canvas;
    PaintCanvas_SetColor(canvas, 0xffffffffu);
    int *icons = (int *)ptr_field(this, 0xfc);
    for (int i = 0; i != 5; i++) {
        icons[i] = -1;
    }

    if (stationMode) {
        void *station = ((Array<void *> *)ptr_field(this, 0x58))->data[index];
        if (Station_isDiscovered(station) != 0) {
            icons[0] = field<int32_t>(this, 0x30);
        }
        int current = Station_getIndex(Status_getStation(*g_StarMap_info_status));
        if (current == Station_getIndex(station)) {
            icons[3] = field<int32_t>(this, 0x2c);
        }
        Station_getName(&name, station);
        int textW = PaintCanvas_GetTextWidth(canvas, (String *)*g_StarMap_info_font);
        int drawX = (int)(x - (float)(textW / 2));
        int drawY = (int)(y + (float)(field<int32_t>(this, 0x1a8) >> 1) - 3.0f);
        if (field<int32_t>(this, 0x64) == index) {
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0x80, 0, field<int32_t>(this, 0x1a4));
            PaintCanvas_DrawString(canvas, *g_StarMap_info_font, &name, drawX, drawY, false);
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4));
            if (Station_getTecLevel(station) > 0) {
                String_ctor_copy(&line, GameText_getText(*g_StarMap_info_text, 0x200), false);
                String_ctor_int(&value, Station_getTecLevel(station));
                String_add(&name, &line, &value);
                PaintCanvas_DrawString(canvas, *g_StarMap_info_font, &name, drawX,
                                       drawY + field<int32_t>(*g_StarMap_info_layout, 4), false);
                String_dtor(&name);
                String_dtor(&value);
                String_dtor(&line);
            }
            PaintCanvas_DrawImage2D(canvas, field<uint32_t>(this, 0x40), (int)x, (int)y, 0x11);
        } else {
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4));
            PaintCanvas_DrawImage2D(canvas, field<uint32_t>(this, 0x44), (int)x, (int)y, 0x11);
            PaintCanvas_DrawString(canvas, *g_StarMap_info_font, &name, drawX, drawY, false);
        }
        String_dtor(&name);
    } else {
        void *system = ((Array<void *> *)ptr_field(this, 0x54))->data[index];
        if (SolarSystem_isFullyDiscovered(system) != 0) {
            icons[0] = field<int32_t>(this, 0x30);
        }
        if (Status_getCurrentCampaignMission(*g_StarMap_info_status) == 0x34 &&
            SolarSystem_getStationEnumIndex(system, 0x4a) >= 0) {
            icons[1] = field<int32_t>(this, 0x24);
        }
        void *mission = Status_getCampaignMission(*g_StarMap_info_status);
        if (mission != 0 && Mission_isEmpty(mission) == 0) {
            int target = Mission_getTargetStation(mission);
            if (SolarSystem_getStationEnumIndex(system, target) >= 0) {
                icons[2] = field<int32_t>(this, 0x28);
            }
        }
        void *freelance = Status_getFreelanceMission(*g_StarMap_info_status);
        if (freelance != 0 && Mission_isEmpty(freelance) == 0) {
            int target = Mission_getTargetStation(freelance);
            if (SolarSystem_getStationEnumIndex(system, target) >= 0) {
                icons[2] = field<int32_t>(this, 0x28);
            }
        }
        SolarSystem_getName(&name, system);
        int textW = PaintCanvas_GetTextWidth(canvas, (String *)*g_StarMap_info_font);
        int drawX = (int)(x - (float)(textW / 2));
        int drawY = (int)(y + (float)(field<int32_t>(this, 0x1a8) >> 1) - 3.0f);
        void *currentSystem = Status_getSystem(*g_StarMap_info_status);
        if (SolarSystem_getIndex(currentSystem) == SolarSystem_getIndex(system)) {
            Layout_getPulseValue(*g_StarMap_info_layout, (float)field<int32_t>(this, 0x1a4));
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4));
            PaintCanvas_DrawImage2D(canvas, field<uint32_t>(this, 0x48), (int)x, (int)y, 0x11);
        }
        if (field<int32_t>(this, 0x60) == index) {
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0x80, 0, field<int32_t>(this, 0x1a4));
        } else if (field<int32_t>(this, 0x60) >= 0) {
            PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4));
        }
        PaintCanvas_DrawString(canvas, *g_StarMap_info_font, &name, drawX, drawY, false);
        String_dtor(&name);
        PaintCanvas_SetColorRGBA(canvas, 0xff, 0xff, 0xff, field<int32_t>(this, 0x1a4));
        if (SolarSystem_hasNoOwner(system) == 0) {
            uint32_t image = field<uint32_t>(this, 0x130);
            int race = SolarSystem_getRace(system);
            if (race == 2) {
                image = field<uint32_t>(this, 0x12c);
            } else if (race == 1) {
                image = field<uint32_t>(this, 0x128);
            } else if (race == 0) {
                image = field<uint32_t>(this, 0x124);
            }
            PaintCanvas_DrawImage2D(canvas, image, drawX, drawY + 0xfd, 0x11);
        }
        if (field<int32_t>(this, 0x60) == index) {
            PaintCanvas_DrawImage2D(canvas, field<uint32_t>(this, 0x40), (int)x, (int)y, 0x11);
        } else {
            PaintCanvas_DrawImage2D(canvas, field<uint32_t>(this, 0x44), (int)x, (int)y, 0x11);
        }
    }

    float iconY = (float)(int)((field<float>(this, 0x7c) - (float)(field<int32_t>(this, 0x1a8) >> 1)) + 10.0f);
    float iconX = (float)(int)(field<float>(this, 0x78) + (float)(field<int32_t>(this, 0x1a8) >> 1) - 7.0f);
    for (int i = 0; i != 5; i++) {
        int image = icons[i];
        if (image != -1) {
            int dx = (i == 0 && *g_StarMap_info_isGerman != 0) ? 0xc : 0x12;
            PaintCanvas_DrawImage2D(canvas, (uint32_t)image, (int)iconX - dx, (int)iconY, 0);
        }
    }
}
