#include "class.h"

extern "C" void __aeabi_memclr4(void *dst, unsigned int n);

__attribute__((visibility("hidden"))) extern void (**g_levelSubCtor)(void *);

static inline void zero16(void *p) {
    ((int *)p)[0] = 0;
    ((int *)p)[1] = 0;
    ((int *)p)[2] = 0;
    ((int *)p)[3] = 0;
}

Level::Level(int mission) {
    void (*ctor)(void *) = *g_levelSubCtor;
    field_b4 = 0;
    field_b8 = 0;
    field_bc = 0;
    field_c8 = 0;
    field_cc = 0;
    field_d0 = 0;
    field_18c = 0;
    field_190 = 0;
    field_194 = 0;
    ctor((char *)this + 0x1d0);
    ctor((char *)this + 0x20c);
    ctor((char *)this + 0x248);
    zero16(&field_1c);
    zero16((char *)this + 0x90);
    zero16((char *)this + 0x84);
    zero16((char *)this + 0x74);
    skyboxMesh = -1;
    field_08 = -1;
    skyboxTexture = -1;
    field_10 = -1;
    missionPtr = mission;
    collisionVolume = 0;
    field_b0 = 0;
    flashColor.x = 0;
    flashColor.y = 0;
    flashColor.z = 0;
    flashDurationA = 0;
    flashDurationB = 0;
    flashActive = 0;
    *(int *)((char *)this + 0x2d) = 0;
    *(int *)((char *)this + 0x29) = 0;
    __aeabi_memclr4((char *)this + 0xd8, 0x65);
    zero16((char *)this + 0x16c);
    zero16((char *)this + 0x15c);
    field_18a = 0;
    field_188 = 0;
    movingStarsIndex = -1;
    field_284 = -1;
    field_34 = -1;
    field_38 = -1;
    field_3c = -1;
    field_48 = -1;
    field_4c = -1;
    field_17c = 0;
    field_180 = 0;
    field_184 = 0;
    field_69 = 0;
    field_6c = 0;
    field_288 = 0;
    field_a0 = 0;
    field_a4 = 0;
    field_a8 = 0;
    miningPlantIndex = -1;
    miningPlant = 0;
    numDeliveredOre = 0;
    numDeliveredPassengers = 0;
    *(short *)((char *)this + 0x29c) = 0;
    field_29e = 0;
    field_1b4 = -1;
    field_1b8 = -1;
    field_1bc = -1;
    field_1c0 = -1;
}
