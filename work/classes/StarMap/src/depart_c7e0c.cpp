#include "class.h"

extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_status;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_store0_a;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_depart_flag_a;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_depart_jumpFlag_a;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_jumpCost_a;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_targetStation;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_depart_flag_b;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_status2;
extern "C" __attribute__((visibility("hidden"))) uint8_t *g_StarMap_depart_jumpFlag_b;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_jumpCost_b;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_achievements;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_sound;
extern "C" __attribute__((visibility("hidden"))) int *g_StarMap_depart_modstation_flag;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_depart_canvas;

extern "C" void Status_departStation(void *status, void *station);
extern "C" void Level_setInitStreamOut();
extern "C" int Status_jumpgateUsed(void *status);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" void *Status_getStation(void *status);
extern "C" void *Status_getShip(void *status);
extern "C" void *Status_getSystem(void *status);
extern "C" int Ship_hasVolatileGoods(void *ship);
extern "C" int Ship_hasJumpDriveIntegrated(void *ship);
extern "C" int SolarSystem_getIndex(void *system);
extern "C" int Station_getSystem(void *station);
extern "C" int Station_equals(void *a, void *b);
extern "C" void *Station_dtor(void *station);
extern "C" void ArrayReleaseClasses_Station(void *arr);
extern "C" void *Array_Station_dtor(void *arr);
extern "C" void Achievements_resetNewMedals(void *achievements);
extern "C" void FModSound_stop(void *sound, int id);
extern "C" void StarMap_depart_tail(void *canvas, int mode);

void StarMap::depart(bool jump)
{
    int selected = field<int32_t>(this, 0x64);
    if (selected < 0) {
        return;
    }

    void **statusHolder = g_StarMap_depart_status;
    void *status = *statusHolder;
    if (field<uint8_t>(this, 0xaa) != 0) {
        Array<void *> *stations = (Array<void *> *)ptr_field(this, 0x58);
        Status_departStation(status, stations->data[selected]);
        *g_StarMap_depart_store0_a = 0;
        Level_setInitStreamOut();
        int used = Status_jumpgateUsed(status);
        if (jump) {
            used = field<uint8_t>(this, 0xab);
        }
        if (jump && used != 0) {
            int toSystem = Station_getSystem(stations->data[selected]);
            int current = SolarSystem_getIndex(Status_getSystem(status));
            *g_StarMap_depart_jumpFlag_a = (uint8_t)(toSystem != current);
            if (toSystem != current) {
                *g_StarMap_depart_jumpCost_a = field<int32_t>(this, 0x1d0);
            }
        } else {
            *g_StarMap_depart_flag_a = 0;
        }
    } else {
        if (Status_getCurrentCampaignMission(status) == 3) {
            goto cleanup;
        }

        int *statusWords = (int *)status;
        statusWords[0x5c / 4] = -1;
        statusWords[0x60 / 4] = -1;
        statusWords[0x64 / 4] = -1;
        statusWords[0x68 / 4] = -1;
        Status_departStation(status, Status_getStation(status));

        Array<void *> *stations = (Array<void *> *)ptr_field(this, 0x58);
        void *target = stations->data[field<int32_t>(this, 0x64)];
        if (Station_equals(target, Status_getStation(status)) == 0) {
            *g_StarMap_depart_targetStation = (int)target;
        }

        if (jump) {
            void *ship = Status_getShip(status);
            if (Ship_hasVolatileGoods(ship) != 0) {
                goto no_jump;
            }
            if (Ship_hasJumpDriveIntegrated(Status_getShip(status)) == 0 && field<uint8_t>(this, 0xab) == 0) {
                goto no_jump;
            }
            int toSystem = Station_getSystem(*g_StarMap_depart_status2);
            int current = SolarSystem_getIndex(Status_getSystem(status));
            *g_StarMap_depart_jumpFlag_b = (uint8_t)(toSystem != current);
            if (toSystem != current) {
                *g_StarMap_depart_jumpCost_b = field<int32_t>(this, 0x1d0);
            }
        } else {
no_jump:
            *g_StarMap_depart_flag_b = 0;
        }
        Achievements_resetNewMedals(*g_StarMap_depart_achievements);
    }

cleanup:
    {
        Array<void *> *stations = (Array<void *> *)ptr_field(this, 0x58);
        for (uint32_t i = 0; i < stations->length; i++) {
            if (i != (uint32_t)field<int32_t>(this, 0x64)) {
                void *station = stations->data[i];
                if (station != 0) {
                    operator delete(Station_dtor(station));
                }
                stations->data[i] = 0;
            }
        }
        if (stations != 0) {
            operator delete(Array_Station_dtor(stations));
        }
        ptr_field(this, 0x58) = 0;
    }
    FModSound_stop(*g_StarMap_depart_sound, 0x66);
    *g_StarMap_depart_modstation_flag = 1;
    return StarMap_depart_tail(*g_StarMap_depart_canvas, 2);
}
