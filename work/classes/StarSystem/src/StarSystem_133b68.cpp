#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_StarSystem_ctor_status;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_ctor_canvas;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_ctor_status_obj;
__attribute__((visibility("hidden"))) extern int *g_StarSystem_ctor_rng;
__attribute__((visibility("hidden"))) extern uint16_t g_StarSystem_ctor_planetTextures[];
__attribute__((visibility("hidden"))) extern uint16_t g_StarSystem_ctor_stationTextures[];
__attribute__((visibility("hidden"))) extern uint32_t g_StarSystem_ctor_systemColors[];

extern "C" void *operator_new(unsigned size);
extern "C" void operator_delete(void *ptr);
extern "C" void *Status_getSystem(void *status);
extern "C" int Status_inSupernovaSystem(void *status);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" void *Status_getStation(void *status);
extern "C" int Status_orbitHasPlanetRing(void *status, int station_index);
extern "C" uint32_t SolarSystem_getTextureIndex(void *system);
extern "C" uint32_t SolarSystem_getIndex(void *system);
extern "C" void *SolarSystem_getStations(void *system);
extern "C" int Station_getIndex(void *station);
extern "C" int Station_getTextureIndex(void *station);
extern "C" void FileRead_ctor(void *self);
extern "C" void *FileRead_loadStationsBinary(void *self, void *system);
extern "C" void *FileRead_dtor(void *self);
extern "C" void Array_uint_ctor(void *array);
extern "C" void Array_int_ctor(void *array);
extern "C" void Array_KIPlayer_ctor(void *array);
extern "C" void Array_AEGeometry_ctor(void *array);
extern "C" void Array_Vector_ctor(void *array);
extern "C" void Array_Station_release(void *array);
extern "C" void *Array_Station_dtor(void *array);
extern "C" void ArraySetLength_uint(int length, void *array);
extern "C" void ArraySetLength_int(int length, void *array);
extern "C" void ArraySetLength_KIPlayer(int length, void *array);
extern "C" void ArraySetLength_AEGeometry(int length, void *array);
extern "C" void ArraySetLength_Vector(int length, void *array);
extern "C" void LensFlare_ctor(void *self, void *canvas);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint32_t mesh, void *canvas, bool dynamic);
extern "C" void AEGeometry_setScaling(AEGeometry *geom, const char *scale);
extern "C" void AEGeometry_setRotation(AEGeometry *geom, const char *rot);
extern "C" void AEGeometry_moveForward(AEGeometry *geom, uint32_t distance_bits);
extern "C" void AEGeometry_getDirection(char *out, AEGeometry *geom);
extern "C" void AEGeometry_getPosition(char *out, AEGeometry *geom);
extern "C" void AEGeometry_rotate(AEGeometry *geom, const char *rot);
extern "C" void Vector_assign(char *dst, const char *src);
extern "C" void PaintCanvas_TextureCreate(void *canvas, uint32_t texture, void *slot, int flags);
extern "C" int AERandom_nextInt(int *rng, int max);
extern "C" void AERandom_setSeed(int *rng, long long seed);
extern "C" void AERandom_reset(int *rng);
extern "C" void *ApplicationManager_GetEngine(void *app);
extern "C" int Engine_IsPostEffectActivated(void *engine);
extern "C" void *PaintCanvas_MeshGetPointer(void *canvas, uint32_t mesh_id);
extern "C" void PlayerStatic_ctor(void *self, int mode, AEGeometry *geom, float x, float y, float z);
extern "C" void StarSystem_initLight(StarSystem *self);

static inline void set_vec(char *dst, float x, float y, float z)
{
    ((float *)dst)[0] = x;
    ((float *)dst)[1] = y;
    ((float *)dst)[2] = z;
}

extern "C" StarSystem *StarSystem_StarSystem(StarSystem *self, int mode)
{
    char vec[12];
    char usedSlots[0x60];
    for (unsigned i = 0; i < sizeof(usedSlots); ++i) {
        usedSlots[i] = 0;
    }

    FL(self, 0x30) = 0.0f;
    FL(self, 0x34) = 0.0f;
    FL(self, 0x38) = 0.0f;
    P(self, 0x24) = 0;
    P(self, 0x14) = 0;
    P(self, 0x18) = 0;
    P(self, 0x1c) = 0;

    void *status = *g_StarSystem_ctor_status;
    B(self, 0x28) = Status_getSystem(status) == 0;
    B(self, 0x0c) = (uint8_t)Status_inSupernovaSystem(status);

    void *flare = operator_new(0x14);
    LensFlare_ctor(flare, *g_StarSystem_ctor_canvas);
    P(self, 0x44) = 0;
    P(self, 0x2c) = flare;
    I(self, 0x4c) = -1;

    if (B(self, 0x28) != 0) {
        void *geomArray = operator_new(0x0c);
        Array_AEGeometry_ctor(geomArray);
        P(self, 0x1c) = geomArray;
        ArraySetLength_AEGeometry(2, geomArray);

        AEGeometry *sun = (AEGeometry *)operator_new(0xc0);
        AEGeometry_ctor(sun, 0x1a70, *g_StarSystem_ctor_canvas, false);
        ((AEGeometry **)array_data(P(self, 0x1c)))[0] = sun;
        set_vec(vec, 1000.0f, 1000.0f, 1000.0f);
        AEGeometry_setScaling(sun, vec);
        AEGeometry_moveForward(sun, 0x447a0000);
        AEGeometry_getDirection(vec, sun);
        Vector_assign((char *)self + 0x30, vec);
        FL(self, 0x30) = -FL(self, 0x30);
        FL(self, 0x34) = -FL(self, 0x34);
        FL(self, 0x38) = -FL(self, 0x38);

        AEGeometry *planet = (AEGeometry *)operator_new(0xc0);
        AEGeometry_ctor(planet, 0x1a70, *g_StarSystem_ctor_canvas, false);
        ((AEGeometry **)array_data(P(self, 0x1c)))[1] = planet;
        int rnd = AERandom_nextInt(g_StarSystem_ctor_rng, 0x4e20);
        float scale = (float)(rnd + 0x4e20) * 0.001f;
        set_vec(vec, scale, scale, scale);
        AEGeometry_setScaling(planet, vec);
        set_vec(vec, 0.0f, 90.0f, 0.0f);
        AEGeometry_setRotation(planet, vec);
        AEGeometry_moveForward(planet, 0x447a0000);

        void *textures = operator_new(0x0c);
        Array_uint_ctor(textures);
        P(self, 0x14) = textures;
        ArraySetLength_uint(2, textures);
        PaintCanvas_TextureCreate(*g_StarSystem_ctor_canvas, 0x2739, array_data(P(self, 0x14)), 0);
        PaintCanvas_TextureCreate(*g_StarSystem_ctor_canvas, 0x2719,
                                  (char *)array_data(P(self, 0x14)) + 4, 0);

        void *positions = operator_new(0x0c);
        Array_Vector_ctor(positions);
        P(self, 0x20) = positions;
        ArraySetLength_Vector(2, positions);
        AEGeometry_getPosition(vec, sun);
        Vector_assign((char *)array_data(P(self, 0x20)), vec);
        AEGeometry_getPosition(vec, planet);
        Vector_assign((char *)array_data(P(self, 0x20)) + 0x0c, vec);

        AEGeometry *streak = (AEGeometry *)operator_new(0xc0);
        AEGeometry_ctor(streak, 0x1a70, *g_StarSystem_ctor_canvas, false);
        P(self, 0x40) = streak;
        set_vec(vec, 250.0f, 15.0f, 1000.0f);
        AEGeometry_setScaling(streak, vec);
        StarSystem_initLight(self);
        return self;
    }

    void *system = Status_getSystem(status);
    void *stations = SolarSystem_getStations(system);
    void *reader = operator_new(1);
    FileRead_ctor(reader);
    void *stationArray = FileRead_loadStationsBinary(reader, system);
    operator_delete(FileRead_dtor(reader));

    void *textures = operator_new(0x0c);
    Array_uint_ctor(textures);
    P(self, 0x14) = textures;
    ArraySetLength_uint(array_len(stationArray) + 1, textures);

    void *stationIndexes = operator_new(0x0c);
    Array_int_ctor(stationIndexes);
    P(self, 0x24) = stationIndexes;
    ArraySetLength_int(array_len(stations), stationIndexes);

    int mission = Status_getCurrentCampaignMission(status);
    if ((mission == 0x59 && B(self, 0x0c) != 0) ||
        (mission > 0x9d && SolarSystem_getIndex(Status_getSystem(status)) == 0x1b)) {
        PaintCanvas_TextureCreate(*g_StarSystem_ctor_canvas, 0x2dde, array_data(P(self, 0x14)), 0);
        U(self, 0x3c) = 3;
    } else {
        uint32_t tex = SolarSystem_getTextureIndex(Status_getSystem(status));
        PaintCanvas_TextureCreate(*g_StarSystem_ctor_canvas, g_StarSystem_ctor_planetTextures[tex],
                                  array_data(P(self, 0x14)), 0);
        U(self, 0x3c) = g_StarSystem_ctor_systemColors[SolarSystem_getIndex(Status_getSystem(status))];
    }

    uint32_t count = array_len(stations);
    for (uint32_t i = 1, off = 0; i < array_len(P(self, 0x14)); ++i, off += 4) {
        int stationIndex = ((int *)array_data(stations))[off / 4];
        int currentStation = Station_getIndex(Status_getStation(status));
        void *station = ((void **)array_data(stationArray))[off / 4];
        uint32_t stationTex = Station_getTextureIndex(station);
        if (stationIndex == currentStation) {
            if (mode == 3 && Status_getCurrentCampaignMission(status) == 0) {
                U(self, 0x50) = i;
                PaintCanvas_TextureCreate(*g_StarSystem_ctor_canvas, 0x273b,
                                          (char *)array_data(P(self, 0x14)) + off + 4, 0);
            } else {
                PaintCanvas_TextureCreate(*g_StarSystem_ctor_canvas,
                                          g_StarSystem_ctor_planetTextures[stationTex],
                                          (char *)array_data(P(self, 0x14)) + off + 4, 0);
            }
        } else {
            PaintCanvas_TextureCreate(*g_StarSystem_ctor_canvas,
                                      g_StarSystem_ctor_stationTextures[stationTex],
                                      (char *)array_data(P(self, 0x14)) + off + 4, 0);
            int idx = Station_getIndex(station);
            if (Status_orbitHasPlanetRing(*g_StarSystem_ctor_status_obj, idx) != 0) {
                AEGeometry *ring = (AEGeometry *)operator_new(0xc0);
                AEGeometry_ctor(ring, 0x1a70, *g_StarSystem_ctor_canvas, false);
                P(self, 0x44) = ring;
                PaintCanvas_TextureCreate(*g_StarSystem_ctor_canvas, 0x7198, (char *)self + 0x48, 0);
                U(self, 0x4c) = i;
            }
        }
        ((int *)array_data(P(self, 0x24)))[off / 4] = Station_getIndex(station);
    }

    Array_Station_release(stationArray);
    operator_delete(Array_Station_dtor(stationArray));

    void *players = operator_new(0x0c);
    Array_KIPlayer_ctor(players);
    P(self, 0x18) = players;
    ArraySetLength_KIPlayer(count, players);

    void *geometries = operator_new(0x0c);
    Array_AEGeometry_ctor(geometries);
    P(self, 0x1c) = geometries;
    ArraySetLength_AEGeometry(count + 1, geometries);

    void *positions = operator_new(0x0c);
    Array_Vector_ctor(positions);
    P(self, 0x20) = positions;
    ArraySetLength_Vector(count + 1, positions);

    AERandom_setSeed(g_StarSystem_ctor_rng,
                     (long long)Station_getIndex(Status_getStation(status)) * 300);

    int sunSlot = AERandom_nextInt(g_StarSystem_ctor_rng, 14);
    for (uint32_t i = 0; i < array_len(P(self, 0x1c)); ++i) {
        AEGeometry *geom = (AEGeometry *)operator_new(0xc0);
        AEGeometry_ctor(geom, 0x1a70, *g_StarSystem_ctor_canvas, false);
        ((AEGeometry **)array_data(P(self, 0x1c)))[i] = geom;

        if (i == 0) {
            float sunScale = B(self, 0x0c) == 0 ? 1000.0f : 2000.0f;
            set_vec(vec, sunScale, sunScale, sunScale);
            AEGeometry_setScaling(geom, vec);
            AEGeometry *streak = (AEGeometry *)operator_new(0xc0);
            AEGeometry_ctor(streak, 0x1a70, *g_StarSystem_ctor_canvas, false);
            P(self, 0x40) = streak;
            set_vec(vec, B(self, 0x0c) == 0 ? 250.0f : 500.0f,
                    B(self, 0x0c) == 0 ? 15.0f : 25.0f,
                    B(self, 0x0c) == 0 ? 1000.0f : 2000.0f);
            AEGeometry_setScaling(streak, vec);
            if (B(self, 0x0c) != 0) {
                PaintCanvas_TextureCreate(*g_StarSystem_ctor_canvas, 0x2dde, (char *)self + 0x10, 0);
            }
            usedSlots[sunSlot * 4] = 1;
        } else {
            void *player = operator_new(0x130);
            PlayerStatic_ctor(player, 0, geom, 0.0f, 0.0f, 0.0f);
            ((void **)array_data(P(self, 0x18)))[i - 1] = player;
            int slot = AERandom_nextInt(g_StarSystem_ctor_rng, 11) + 7;
            usedSlots[slot * 4] = 1;
            int dist = AERandom_nextInt(g_StarSystem_ctor_rng, 0x4e20) + 0x4e20;
            if (Status_getCurrentCampaignMission(status) == 0) {
                dist = (int)((float)dist * 0.5f);
            }
            float scale = (float)dist * 0.001f;
            FL(self, 0x58) = scale;
            set_vec(vec, scale, scale, scale);
            AEGeometry_setScaling(geom, vec);
        }

        int angleSlot = (i == 0) ? sunSlot : (int)i + 6;
        set_vec(vec, 0.0f, (float)(angleSlot * 0xaaa) * 0.001f, 0.0f);
        AEGeometry_setRotation(geom, vec);
        AEGeometry_moveForward(geom, 0x447a0000);
        if (i == 0) {
            AEGeometry_getDirection(vec, geom);
            Vector_assign((char *)self + 0x30, vec);
            FL(self, 0x30) = -FL(self, 0x30);
            FL(self, 0x34) = -FL(self, 0x34);
            FL(self, 0x38) = -FL(self, 0x38);
        }
        AEGeometry_getPosition(vec, geom);
        Vector_assign((char *)array_data(P(self, 0x20)) + i * 0x0c, vec);
    }

    AERandom_reset(g_StarSystem_ctor_rng);
    StarSystem_initLight(self);
    return self;
}
