#include "class.h"

extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_system_canvas;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_system_random;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_system_status;
extern "C" __attribute__((visibility("hidden"))) void **g_StarMap_system_engine;

extern "C" void *SolarSystem_getStations(void *system);
extern "C" int SolarSystem_getIndex(void *system);
extern "C" int SolarSystem_getRace(void *system);
extern "C" int SolarSystem_getTextureIndex(void *system);
extern "C" int Station_getTextureIndex(void *station);
extern "C" int Station_getIndex(void *station);
extern "C" void Array_Station_ctor(void *arr);
extern "C" void Array_AEGeometry_ctor(void *arr);
extern "C" void Array_bool_ctor(void *arr);
extern "C" void Array_Vector_ctor(void *arr);
extern "C" void ArraySetLength_Station(uint32_t n, void *arr);
extern "C" void ArraySetLength_AEGeometry(uint32_t n, void *arr);
extern "C" void ArraySetLength_bool(uint32_t n, void *arr);
extern "C" void ArraySetLength_Vector(uint32_t n, void *arr);
extern "C" void FileRead_ctor(void *reader);
extern "C" void *FileRead_dtor(void *reader);
extern "C" void *FileRead_loadStationsBinary(void *reader, void *system);
extern "C" void *AEGeometry_ctor_canvas(void *geometry, uint32_t canvas);
extern "C" void *AEGeometry_ctor_image(void *geometry, uint16_t image, uint32_t canvas, bool flag);
extern "C" void AEGeometry_addChild(void *geometry, uint32_t child);
extern "C" void AEGeometry_setVisible(void *geometry, bool visible);
extern "C" void AEGeometry_setRotation(void *geometry, float x, float y, float z);
extern "C" void AEGeometry_setScaling(void *geometry, float x, float y, float z);
extern "C" void AEGeometry_getPosition(Vector *out, void *geometry);
extern "C" void AEGeometry_setPosition(void *geometry, Vector *pos);
extern "C" void AEGeometry_translate(void *geometry, Vector *pos);
extern "C" void *AEGeometry_dtor(void *geometry);
extern "C" int AERandom_nextInt(void *random, int bound);
extern "C" void AERandom_setSeed(void *random, long long seed);
extern "C" void ArrayReleaseClasses_Vector(void *arr);
extern "C" void *Array_Vector_dtor(void *arr);
extern "C" void Engine_LightSetLightDirection(void *engine, float x, float y, float z, int light);
extern "C" void PaintCanvas_Image2DCreate(uint32_t canvas, uint16_t image, void *out);
extern "C" void PaintCanvas_TextureCreate(uint32_t canvas, uint16_t image, void *out, bool flag);

void StarMap::initStarSystem()
{
    void *system = ((Array<void *> *)ptr_field(this, 0x54))->data[field<int32_t>(this, 0x60)];
    Array<void *> *stationIds = (Array<void *> *)SolarSystem_getStations(system);
    uint32_t count = stationIds->length;

    void *stations = operator new(0xc);
    Array_Station_ctor(stations);
    ptr_field(this, 0x58) = stations;
    ArraySetLength_Station(count, stations);
    void *reader = operator new(1);
    FileRead_ctor(reader);
    ptr_field(this, 0x58) = FileRead_loadStationsBinary(reader, system);
    operator delete(FileRead_dtor(reader));

    uint32_t bytes = count > 0x3fffffffu ? 0xffffffffu : count * 4;
    ptr_field(this, 0x98) = operator new(bytes);
    ptr_field(this, 0x9c) = operator new(bytes);
    field<int32_t>(this, 0x1c4) = -1;
    AERandom_setSeed(*g_StarMap_system_random, (long long)SolarSystem_getIndex(system) * 1000);

    void *stationGeoms = operator new(0xc);
    Array_AEGeometry_ctor(stationGeoms);
    ptr_field(this, 0x90) = stationGeoms;
    ArraySetLength_AEGeometry(count + 1, stationGeoms);

    void *root = operator new(0xc0);
    AEGeometry_ctor_canvas(root, *g_StarMap_system_canvas);
    ptr_field(this, 0xa4) = root;

    void *used = operator new(0xc);
    Array_bool_ctor(used);
    ptr_field(this, 0x100) = used;
    ArraySetLength_bool(((Array<void *> *)stationGeoms)->length, used);
    for (uint32_t i = 0; i < ((Array<uint8_t> *)used)->length; i++) {
        ((Array<uint8_t> *)used)->data[i] = 0;
    }

    Array<void *> *stationArr = (Array<void *> *)ptr_field(this, 0x58);
    Array<void *> *geomArr = (Array<void *> *)ptr_field(this, 0x90);
    for (uint32_t i = 1; i < geomArr->length; i++) {
        uint32_t stationIndex = i - 1;
        int tex = Station_getTextureIndex(stationArr->data[stationIndex]);
        void *geom = operator new(0xc0);
        AEGeometry_ctor_image(geom, (uint16_t)(tex + 0x4704), *g_StarMap_system_canvas, false);
        geomArr->data[i] = geom;
        ((int *)ptr_field(this, 0x98))[stationIndex] =
            AERandom_nextInt(*g_StarMap_system_random, ((Array<uint8_t> *)used)->length) *
            (0x10000 / (int)((Array<uint8_t> *)used)->length);
        int dist = (i == 1) ? 0x1900 : ((int *)ptr_field(this, 0x9c))[i - 2];
        dist += AERandom_nextInt(*g_StarMap_system_random, 0x15e0) + 0x640;
        ((int *)ptr_field(this, 0x9c))[stationIndex] = dist;
        Vector pos = {0.0f, 0.0f, (float)dist};
        AEGeometry_translate(geom, &pos);
        float scale = (float)(tex << 4) * 0.001f;
        AEGeometry_setScaling(geom, scale, scale, scale);
        AEGeometry_addChild(root, field<uint32_t>(geom, 0xc));
        if (ptr_field(this, 0xf8) != 0 && field<int32_t>(this, 0x60) == field<int32_t>(*g_StarMap_system_status, 0x7c) &&
            Station_getIndex(stationArr->data[stationIndex]) == field<int32_t>(*g_StarMap_system_status, 0x80)) {
            field<uint32_t>(this, 0x1c4) = i;
        }
    }

    AEGeometry_setVisible(((Array<void *> *)ptr_field(this, 0x90))->data[1], false);

    void *rings = operator new(0xc);
    Array_AEGeometry_ctor(rings);
    ptr_field(this, 0x94) = rings;
    ArraySetLength_AEGeometry(count, rings);
    for (uint32_t i = 0; i < ((Array<void *> *)rings)->length; i++) {
        void *ring = operator new(0xc0);
        AEGeometry_ctor_image(ring, 0x1a7b, *g_StarMap_system_canvas, false);
        ((Array<void *> *)rings)->data[i] = ring;
        AEGeometry_addChild(root, field<uint32_t>(ring, 0xc));
        float scale = (float)(((int *)ptr_field(this, 0x9c))[i] << 1) * 0.001f;
        AEGeometry_setScaling(ring, scale, scale, scale);
    }

    Vector selected;
    AEGeometry_getPosition(&selected, ((Array<void *> *)ptr_field(this, 0x68))->data[field<int32_t>(this, 0x60)]);
    AEGeometry_setPosition(root, &selected);
    AEGeometry_setScaling(root, 0.0078125f, 0.0078125f, 0.0078125f);
    AEGeometry_setRotation(root, 0.0f, 0.0f, 0.0f);
    field<int32_t>(this, 0x188) = 0x45800000;
    field<float>(this, 0x18c) = 0.0f;
    field<int32_t>(this, 0x64) = -1;
    PaintCanvas_Image2DCreate(*g_StarMap_system_canvas, (uint16_t)(0x4500 + SolarSystem_getRace(system)),
                              (char *)this + 0x34);

    if (ptr_field(this, 0x198) != 0) {
        ArrayReleaseClasses_Vector(ptr_field(this, 0x198));
        if (ptr_field(this, 0x198) != 0) {
            operator delete(Array_Vector_dtor(ptr_field(this, 0x198)));
        }
    }
    ptr_field(this, 0x198) = 0;
    void *stationPositions = operator new(0xc);
    Array_Vector_ctor(stationPositions);
    ptr_field(this, 0x198) = stationPositions;
    ArraySetLength_Vector(((Array<void *> *)ptr_field(this, 0x58))->length, stationPositions);
    for (uint32_t i = 0; i < ((Array<Vector *> *)stationPositions)->length; i++) {
        Vector *p = (Vector *)operator new(0xc);
        p->x = 0.0f;
        p->y = 0.0f;
        p->z = 0.0f;
        ((Array<Vector *> *)stationPositions)->data[i] = p;
    }

    Engine_LightSetLightDirection(*g_StarMap_system_engine, 0.0f, 0.0f, 1.0f, 1);
    if (ptr_field(this, 0x1bc) != 0) {
        operator delete(AEGeometry_dtor(ptr_field(this, 0x1bc)));
    }
    field<uint32_t>(this, 0x178) = 0xffffffffu;
    ptr_field(this, 0x1bc) = 0;
    uint16_t texture = (uint16_t)(0x2700 + SolarSystem_getTextureIndex(system));
    if (field<int32_t>(this, 0x60) == 0x1b) {
        texture = 0x2734;
    }
    PaintCanvas_TextureCreate(*g_StarMap_system_canvas, texture, (char *)this + 0x178, false);
    void *planet = operator new(0xc0);
    AEGeometry_ctor_image(planet, 0x1a70, *g_StarMap_system_canvas, false);
    ptr_field(this, 0x1bc) = planet;
    AEGeometry_setPosition(planet, &selected);
    AEGeometry_setRotation(planet, 0.0f, 0.0f, 0.0f);
    AEGeometry_setScaling(planet, 1.0f, 1.0f, 1.0f);
    AEGeometry_setVisible(((Array<void *> *)ptr_field(this, 0x68))->data[field<int32_t>(this, 0x60)], false);
}
