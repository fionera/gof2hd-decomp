#include "class.h"

extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_ctor_vecCtor)(void *);
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_ctor_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_ctor_galaxy;
extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_ctor_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_ctor_random;

extern "C" void *Galaxy_getSystems(void *galaxy);
extern "C" void *AEGeometry_ctor_canvas(void *geometry, uint32_t canvas);
extern "C" void *AEGeometry_ctor_image(void *geometry, uint16_t image, uint32_t canvas, bool flag);
extern "C" void AEGeometry_setScaling(void *geometry, float x, float y, float z);
extern "C" void AEGeometry_setPosition(void *geometry, float x, float y, float z);
extern "C" void AEGeometry_addChild(void *geometry, uint32_t child);
extern "C" void AEGeometry_getPosition(Vector *out, void *geometry);
extern "C" void AEGeometry_setRotation(void *geometry, float x, float y, float z);
extern "C" void Array_AEGeometry_ctor(void *arr);
extern "C" void Array_Vector_ctor(void *arr);
extern "C" void ArraySetLength_AEGeometry(uint32_t n, void *arr);
extern "C" void ArraySetLength_Vector(uint32_t n, void *arr);
extern "C" void AERandom_reset(void *random);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" int SolarSystem_getTextureIndex(void *system);
extern "C" int SolarSystem_getX(void *system);
extern "C" int SolarSystem_getY(void *system);
extern "C" int SolarSystem_getZ(void *system);
extern "C" void *PaintCanvas_TransformGetTransform(uint32_t canvas);
extern "C" void Transform_SetAnimationState(void *transform, int state, int frame);

StarMap::StarMap(bool jumpMapMode, Mission *mission, bool param3, int param4)
{
    void (*vecCtor)(void *) = g_StarMap_ctor_vecCtor;
    Vector zero = {0.0f, 0.0f, 0.0f};
    *(Vector *)((char *)this + 0x78) = zero;
    field<int32_t>(this, 0x88) = 0;
    field<int32_t>(this, 0x8c) = 0;
    vecCtor((char *)this + 0xac);
    vecCtor((char *)this + 0xbc);
    vecCtor((char *)this + 0xcc);
    field<Vector>(this, 0x154) = zero;
    field<Vector>(this, 0x144) = zero;
    field<int32_t>(this, 0x164) = 0;
    field<int32_t>(this, 0x188) = 0;
    field<int32_t>(this, 0x18c) = 0;
    field<int32_t>(this, 0x190) = 0;

    field<int32_t>(this, 4) = 0;
    field<int32_t>(this, 0x1c8) = field<int32_t>(*g_StarMap_ctor_status, 0x88);
    field<int32_t>(this, 0x60) = -1;
    ptr_field(this, 0xa4) = 0;
    ptr_field(this, 0x58) = 0;
    ptr_field(this, 0x194) = 0;
    ptr_field(this, 0x198) = 0;
    field<uint8_t>(this, 0x1e4) = 0;
    field<uint8_t>(this, 1) = 0;
    field<uint8_t>(this, 0x120) = 0;
    ptr_field(this, 0x17c) = 0;
    ptr_field(this, 0x180) = 0;
    ptr_field(this, 0x184) = 0;
    field<int32_t>(this, 0x168) = 0;
    field<int32_t>(this, 0x16c) = 0;
    field<int32_t>(this, 0x170) = 0;

    ptr_field(this, 0xfc) = operator new(0x14);
    ptr_field(this, 0x54) = Galaxy_getSystems(*g_StarMap_ctor_galaxy);
    field<int32_t>(this, 0x10) = 500;
    field<int32_t>(this, 0x14) = 500;

    void *root = operator new(0xc0);
    AEGeometry_ctor_canvas(root, *g_StarMap_ctor_canvas);
    ptr_field(this, 0x6c) = root;

    void *systemsGeom = operator new(0xc);
    Array_AEGeometry_ctor(systemsGeom);
    ptr_field(this, 0x68) = systemsGeom;
    void *systemPositions = operator new(0xc);
    Array_Vector_ctor(systemPositions);
    ptr_field(this, 0x194) = systemPositions;
    ArraySetLength_AEGeometry(0x22, systemsGeom);
    ArraySetLength_Vector(0x22, systemPositions);

    Array<void *> *systems = (Array<void *> *)ptr_field(this, 0x54);
    Array<void *> *geoms = (Array<void *> *)ptr_field(this, 0x68);
    Array<Vector *> *positions = (Array<Vector *> *)ptr_field(this, 0x194);
    for (uint32_t i = 0; i < geoms->length; i++) {
        void *sys = systems->data[i];
        int tex = SolarSystem_getTextureIndex(sys);
        uint16_t image = (uint16_t)(tex + 0x4696);
        if (i == 0x1b && Status_getCurrentCampaignMission(*g_StarMap_ctor_status) > 0x9d) {
            image = 0x469b;
        }
        void *geom = operator new(0xc0);
        AEGeometry_ctor_image(geom, image, *g_StarMap_ctor_canvas, false);
        geoms->data[i] = geom;
        AEGeometry_setScaling(geom, 1.0f, 1.0f, 1.0f);
        float px = (float)((int)(((100 - SolarSystem_getX(sys)) / 100.0f) * 20000.0f) - 10000);
        float py = (float)((int)(((100 - SolarSystem_getY(sys)) / 100.0f) * 18000.0f) - 9000);
        float pz = (float)((int)(((100 - SolarSystem_getZ(sys)) / 100.0f) * 9000.0f) + 1000);
        AEGeometry_setPosition(geom, pz, px, py);
        AEGeometry_addChild(root, field<uint32_t>(geom, 0xc));
        Vector *pos = (Vector *)operator new(0xc);
        *pos = zero;
        positions->data[i] = pos;
    }

    AERandom_reset(*g_StarMap_ctor_random);
    ptr_field(this, 0xf8) = 0;
    field<uint8_t>(this, 0xa9) = 0;
    if (Status_getCurrentCampaignMission(*g_StarMap_ctor_status) > 0x1f &&
        field<int32_t>(*g_StarMap_ctor_status, 0x7c) >= 0) {
        void *marker = operator new(0xc0);
        AEGeometry_ctor_image(marker, 0x4262, *g_StarMap_ctor_canvas, false);
        ptr_field(this, 0xf8) = marker;
        Vector p;
        AEGeometry_getPosition(&p, geoms->data[field<int32_t>(*g_StarMap_ctor_status, 0x7c)]);
        AEGeometry_setPosition(marker, p.x, p.y, p.z);
        Transform_SetAnimationState(PaintCanvas_TransformGetTransform(*g_StarMap_ctor_canvas), 2, 0);
        AEGeometry_setRotation(marker, 0.0f, 0.0f, 0.0f);
    }

    init(jumpMapMode, mission, param3, param4);
}
