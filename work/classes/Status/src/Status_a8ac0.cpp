#include "class.h"

extern "C" void Array_Mission_ctor(Array<Mission *> *a);
extern "C" void Array_Station_ctor(Array<Station *> *a);
extern "C" void Array_bool_ctor(Array<bool> *a);
extern "C" void Array_int_ctor(void *a);
extern "C" void ArraySetLength_Mission(uint32_t n, Array<Mission *> *a);
extern "C" void ArraySetLength_Station(uint32_t n, Array<Station *> *a);
extern "C" void ArraySetLength_bool(uint32_t n, Array<bool> *a);
extern "C" void Station_ctor(Station *s);

// Allocates the mission/station/visibility containers, sizes them, and zero-initializes the
// persistent player state to its new-game defaults.
Status::Status() {
    Array<Mission *> *m = (Array<Mission *> *)operator new(0xc);
    Array_Mission_ctor(m);
    missions = m;
    Array<Station *> *ss = (Array<Station *> *)operator new(0xc);
    Array_Station_ctor(ss);
    stationStack = ss;
    Array<bool> *a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); systemVisibilities = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_94 = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_98 = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_ac = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_b4 = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_4c = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_50 = a;
    void *ai = operator new(0xc); Array_int_ctor(ai); field_90 = (int32_t)(uint32_t)ai;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_54 = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_58 = a;
    ArraySetLength_Mission(2, missions);
    ArraySetLength_Station(3, stationStack);
    ArraySetLength_bool(0x22, systemVisibilities);
    ArraySetLength_bool(0xb, field_94);
    ArraySetLength_bool(0xb, field_98);
    ArraySetLength_bool(0x16, field_ac);
    ArraySetLength_bool(0x22, field_b4);
    ArraySetLength_bool(4, field_4c);
    ArraySetLength_bool(0xf, field_50);
    ArraySetLength_bool(0xe9, field_54);
    ArraySetLength_bool(5, field_58);
    stationsVisited = 0;
    currentCampaignMission = 0;
    passengers = 0;
    field_10c = 0;
    field_110 = 0;
    kills = 0;
    missionCount = 0;
    level = 1;
    credits = 0;
    rating = 0;
    playingTime = 0;
    Station *st = (Station *)operator new(0x34);
    Station_ctor(st);
    playerStation = st;
    field_8c = 0;
    ship = 0;
    mission = 0;
    station = 0;
    system = 0;
    planetNames = 0;
    planetTextures = 0;
    field_14c = 0;
    wanted = 0;
    wingmen = 0;
    *(int32_t *)((char *)this + 0x28) = 0;
    standing = 0;
    bluePrints = 0;
    pendingProducts = 0;
    agents = 0;
    *(int32_t *)((char *)this + 0x3c) = 0;
    *(int32_t *)((char *)this + 0x40) = 0;
    *(int32_t *)((char *)this + 0x44) = 0;
    *(int32_t *)((char *)this + 0x48) = 0;
    for (int i = 1; i != 5; i = i + 1) {
        *(int32_t *)((char *)this + i * 4) = 0;
    }
    *(uint8_t *)((char *)this + 0x17c) = 0;
    field_178 = 0;
}
