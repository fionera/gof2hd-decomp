#include "gof2/StarSystem.h"
// AEGeometry.h declares __aeabi_memcpy with a uint32_t length, which clashes with
// Engine.h's unsigned-long version. Rename the loose decl away while parsing AEGeometry.h
// so only one canonical prototype survives.
#define __aeabi_memcpy __aeabi_memcpy_aegeom_unused
#include "gof2/AEGeometry.h"
#undef __aeabi_memcpy
#include "gof2/FileRead.h"
#include "gof2/LensFlare.h"
#include "gof2/Status.h"
#include "gof2/Transform.h"
#include "gof2/ApplicationManager.h"
#include "gof2/Engine.h"
#include "gof2/PaintCanvasClass.h"


extern "C" void StarSystem_renderSunStreak_tail(void *geom);
extern "C" void *Array_AEGeometry_dtor(void *array);
extern "C" void ArrayRelease_KIPlayer(void *array);
extern "C" void *Array_KIPlayer_dtor(void *array);
extern "C" void *LensFlare_dtor(void *flare);
extern "C" void ArrayRelease_uint(void *array);
extern "C" void *Array_uint_dtor(void *array);
extern "C" void ArrayRelease_int(void *array);
extern "C" void *Array_int_dtor(void *array);
extern "C" void ArrayRelease_Vector(void *array);
extern "C" void *Array_Vector_dtor(void *array);
extern "C" uint32_t SolarSystem_getTextureIndex(void *system);
extern "C" uint32_t SolarSystem_getIndex(void *system);
extern "C" uint32_t Station_getTextureIndex(void *station);
extern "C" void *SolarSystem_getStations(void *system);
extern "C" int Station_getIndex(void *station);
extern "C" void FileRead_ctor(void *self);
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
extern "C" void AEGeometry_getDirection(char *out, AEGeometry *geom);
extern "C" void Vector_assign(char *dst, const char *src);
extern "C" void AERandom_setSeed(int *rng, long long seed);
extern "C" void AERandom_reset(int *rng);
extern "C" void PlayerStatic_ctor(void *self, int mode, AEGeometry *geom, float x, float y, float z);
extern "C" void Vector_scale(char *out, const char *in, float scale);
void MatrixGetPosition(char *out, void *matrix);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, unsigned long n);
extern "C" void Vector_add(char *out, const char *a, const char *b);
void MatrixGetUp(char *out, const char *matrix);
void MatrixGetLookAt(char *out, const char *from, const char *to, const char *up);
extern "C" void Matrix_assign(char *dst, const char *src);
extern "C" void MatrixSetScaling(char *matrix, float x, float y, float z);
extern "C" void AEGeometry_setMatrix(AEGeometry *geom, const char *matrix);
extern "C" void Vector_mul_assign(char *vec, float scale);

// ---- switchSunForSupernovaExpansion_134fc4.cpp ----
void StarSystem::switchSunForSupernovaExpansion() {
    StarSystem *self = this;
    char scaleBytes[12];
    AEGeometry *geom = *(AEGeometry **)P(P(self, 0x1c), 4);
    uint32_t *scale = (uint32_t *)scaleBytes;
    scale[0] = 0x46311048;
    scale[1] = 0x46311048;
    scale[2] = 0x46311048;
    ((AEGeometry *)(geom))->setScaling(*(const Vector *)scaleBytes);
}

// ---- renderSunStreak_1350c8.cpp ----
__attribute__((visibility("hidden"))) extern void **g_StarSystem_sunstreak_canvas;


void StarSystem::renderSunStreak() {
    StarSystem *self = this;
    void **holder = g_StarSystem_sunstreak_canvas;
    void *canvas = *holder;
    void *slot = (B(self, 0x0c) != 0) ? (void *)((char *)self + 0x10) : P(P(self, 0x14), 4);
    ((PaintCanvas *)canvas)->SetTexture(*(uint32_t *)slot, (unsigned)-1);
    ((PaintCanvas *)*holder)->SetBlendMode(2);
    return StarSystem_renderSunStreak_tail(P(self, 0x40));
}

// ---- getLightDirection_134dbe.cpp ----
Vector StarSystem::getLightDirection() {
    StarSystem *self = this;
    return self->lightDirection;
}

// ---- _StarSystem_134d10.cpp ----
StarSystem::~StarSystem()
{
    StarSystem *self = this;
    void *p = P(self, 0x1c);
    if (p != 0) {
        ::operator delete(Array_AEGeometry_dtor(p));
    }
    P(self, 0x1c) = 0;

    p = P(self, 0x18);
    if (p != 0) {
        ArrayRelease_KIPlayer(p);
        p = P(self, 0x18);
        if (p != 0) {
            ::operator delete(Array_KIPlayer_dtor(p));
        }
    }
    P(self, 0x18) = 0;

    p = P(self, 0x2c);
    if (p != 0) {
        ::operator delete(LensFlare_dtor(p));
    }
    P(self, 0x2c) = 0;

    p = P(self, 0x14);
    if (p != 0) {
        ArrayRelease_uint(p);
        p = P(self, 0x14);
        if (p != 0) {
            ::operator delete(Array_uint_dtor(p));
        }
    }
    P(self, 0x14) = 0;

    p = P(self, 0x24);
    if (p != 0) {
        ArrayRelease_int(p);
        p = P(self, 0x24);
        if (p != 0) {
            ::operator delete(Array_int_dtor(p));
        }
    }
    P(self, 0x24) = 0;

    p = P(self, 0x20);
    if (p != 0) {
        ArrayRelease_Vector(p);
        p = P(self, 0x20);
        if (p != 0) {
            ::operator delete(Array_Vector_dtor(p));
        }
    }
    P(self, 0x20) = 0;
}

// ---- initLight_13474c.cpp ----
typedef Engine *(*EngineGetter)(void *app);

__attribute__((visibility("hidden"))) extern void **g_StarSystem_init_app;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_init_status;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_init_canvas;
__attribute__((visibility("hidden"))) extern EngineGetter g_StarSystem_init_getEngine;
__attribute__((visibility("hidden"))) extern float g_StarSystem_init_sunColors[];
__attribute__((visibility("hidden"))) extern float g_StarSystem_init_lightColors[];
__attribute__((visibility("hidden"))) extern float g_StarSystem_init_stationColors[];

static inline uint32_t rgba_scaled(uint32_t color, float scale)
{
    uint32_t r = (uint32_t)((float)((color >> 24) & 0xff) * scale);
    uint32_t g = (uint32_t)((float)((color >> 16) & 0xff) * scale);
    uint32_t b = (uint32_t)((float)((color >> 8) & 0xff) * scale);
    return (r << 24) | (g << 16) | (b << 8) | 0xff;
}

void StarSystem::initLight() {
    StarSystem *self = this;
    void *app = *g_StarSystem_init_app;
    Engine *engine = (Engine *)((ApplicationManager *)(app))->GetEngine();
    I(engine, 0x32c) = 2;

    void *status = *g_StarSystem_init_status;
    uint32_t baseIndex;
    bool normalSystem;
    if (B(self, 0x28) == 0) {
        void *system = (void *)(intptr_t)((Status *)(status))->getSystem();
        baseIndex = SolarSystem_getTextureIndex(system) * 3;
        normalSystem = true;
    } else {
        baseIndex = 0x1e;
        normalSystem = false;
    }

    float ambientScale = 0.5f;
    FL(self, 0x00) = g_StarSystem_init_sunColors[baseIndex];
    FL(self, 0x04) = g_StarSystem_init_sunColors[baseIndex + 1];
    FL(self, 0x08) = g_StarSystem_init_sunColors[baseIndex + 2];

    if (normalSystem) {
        void *system = (void *)(intptr_t)((Status *)(status))->getSystem();
        uint32_t tex = SolarSystem_getTextureIndex(system);
        int mission = ((Status *)(status))->getCurrentCampaignMission();
        if (tex == 0x0f && (mission == 0x59 || mission > 0x9d)) {
            FL(self, 0x00) *= 0.5f;
            FL(self, 0x04) *= 0.5f;
            FL(self, 0x08) *= 0.5f;
        }
        if (B(self, 0x28) == 0) {
            system = (void *)(intptr_t)((Status *)(status))->getSystem();
            tex = SolarSystem_getTextureIndex(system);
            ambientScale = 0.5f;
            mission = ((Status *)(status))->getCurrentCampaignMission();
            if (tex == 0x0f && mission != 0x59) {
                ambientScale = mission < 0x9e ? 0.35f : 0.6f;
            }
        }
    }

    uint32_t stationColorIndex = 0x17;
    if (normalSystem && B(self, 0x28) == 0) {
        void *station = ((Status *)(status))->getStation();
        stationColorIndex = Station_getTextureIndex(station) * 3;
    }

    float lr = g_StarSystem_init_stationColors[stationColorIndex];
    float lg = g_StarSystem_init_stationColors[stationColorIndex + 1];
    float lb = g_StarSystem_init_stationColors[stationColorIndex + 2];
    if (normalSystem && B(self, 0x28) == 0) {
        void *system = (void *)(intptr_t)((Status *)(status))->getSystem();
        if (SolarSystem_getTextureIndex(system) == 0x0f &&
            ((Status *)(status))->getCurrentCampaignMission() != 0x59) {
            if (((Status *)(status))->getCurrentCampaignMission() < 0x9e) {
                lr = lr;
            }
        }
    }

    EngineGetter getEngine = g_StarSystem_init_getEngine;
    engine = getEngine(app);
    ((Engine *)(engine))->LightSetGlobalSceneColorAmbient(ambientScale * FL(self, 0x00), ambientScale * FL(self, 0x04), ambientScale * FL(self, 0x08));
    engine = getEngine(app);
    ((Engine *)(engine))->LightSetRimColor(g_StarSystem_init_lightColors[baseIndex] * 3.0f, g_StarSystem_init_lightColors[baseIndex + 1] * 3.0f, g_StarSystem_init_lightColors[baseIndex + 2] * 3.0f);
    ((Engine *)(getEngine(app)))->LightSetMaterialColorAmbient(1.0f, 1.0f, 1.0f);
    ((Engine *)(getEngine(app)))->LightSetMaterialColorDiffuse(1.0f, 1.0f, 1.0f);
    ((Engine *)(getEngine(app)))->LightSetMaterialColorSpecular(1.0f, 1.0f, 1.0f);
    ((Engine *)(getEngine(app)))->LightSetMaterialColorShininess(0.7f);
    ((Engine *)(getEngine(app)))->LightSetLightDirection(FL(self, 0x30), FL(self, 0x34), FL(self, 0x38), 0x4000);
    ((Engine *)(getEngine(app)))->LightSetLightColorAmbient(0.0f, 0.0f, 0.0f, 0x4000);
    ((Engine *)(getEngine(app)))->LightSetLightColorDiffuse(FL(self, 0x00), FL(self, 0x04), FL(self, 0x08), 0x4000);
    ((Engine *)(getEngine(app)))->LightSetLightColorSpecular(2.0f, 2.0f, 2.0f, 0x4000);
    ((Engine *)(getEngine(app)))->LightSetLightDirection(0.0f, 0.0f, 1.0f, 0x4001);
    ((Engine *)(getEngine(app)))->LightSetLightColorDiffuse(lr * 1.5f, lg * 1.5f, lb * 1.5f, 0x4001);
    ((Engine *)(getEngine(app)))->LightSetLightColorSpecular(lr * 1.5f, lg * 1.5f, lb * 1.5f, 0x4001);
    ((Engine *)(getEngine(app)))->LightSetParticleAmbient(FL(self, 0x00), FL(self, 0x04), FL(self, 0x08));

    void *canvas = *g_StarSystem_init_canvas;
    ((PaintCanvas *)canvas)->FogEnable(0, 1);
    B(self, 0x54) = 0;
    if (B(self, 0x28) != 0) {
        return;
    }

    uint32_t tex = SolarSystem_getTextureIndex((void *)(intptr_t)((Status *)(status))->getSystem());
    uint32_t fogColor = 0;
    switch (tex) {
    case 0x0b:
        fogColor = 0x808080ff;
        break;
    case 0x0c:
        fogColor = 0xa0a0a0ff;
        break;
    case 0x0f:
        if (((Status *)(status))->getCurrentCampaignMission() == 0x59) {
            return;
        }
        fogColor = 0x707070ff;
        ((PaintCanvas *)canvas)->FogSetParameter(0x2601, 0.0f, 0.0f, 1.0f, fogColor);
        ((PaintCanvas *)canvas)->FogEnable(1, 1);
        B(self, 0x54) = 1;
        for (uint32_t i = 1; i < array_len(P(self, 0x1c)); ++i) {
            AEGeometry **geoms = (AEGeometry **)array_data(P(self, 0x1c));
            void *transform = ((PaintCanvas *)canvas)->TransformGetTransform(I(geoms[i], 0x0c));
            U(transform, 0x48) = fogColor;
        }
        return;
    case 0x10:
        fogColor = 0x909090ff;
        break;
    case 0x11:
        fogColor = 0x707090ff;
        break;
    case 0x12:
        fogColor = 0x909070ff;
        break;
    default:
        return;
    }

    ((PaintCanvas *)canvas)->FogSetParameter(0x2601, 0.0f, 0.0f, 1.0f, fogColor);
    ((PaintCanvas *)canvas)->FogEnable(1, 1);
    B(self, 0x54) = 1;
    uint32_t scaled = rgba_scaled(fogColor, 0.65f);
    for (uint32_t i = 1; i < array_len(P(self, 0x1c)); ++i) {
        AEGeometry **geoms = (AEGeometry **)array_data(P(self, 0x1c));
        void *transform = ((PaintCanvas *)canvas)->TransformGetTransform(I(geoms[i], 0x0c));
        U(transform, 0x48) = scaled;
    }
}

// ---- render2D_1354e8.cpp ----
__attribute__((visibility("hidden"))) extern void **g_StarSystem_render2d_canvas;


void StarSystem::render2D() {
    StarSystem *self = this;
    char posBytes[12];
    void *flare = P(self, 0x2c);
    if (flare != 0) {
        void **canvasHolder = g_StarSystem_render2d_canvas;
        void *canvas = *canvasHolder;
        unsigned current = ((PaintCanvas *)canvas)->CameraGetCurrent();
        void *local = ((PaintCanvas *)canvas)->CameraGetLocal(current);
        float *pos = (float *)posBytes;
        pos[0] = FL(local, 0x0c) + FL(self, 0x30) * 65536.0f;
        pos[1] = FL(local, 0x1c) + FL(self, 0x34) * 65536.0f;
        pos[2] = FL(local, 0x2c) + FL(self, 0x38) * 65536.0f;
        ((LensFlare *)(P(self, 0x2c)))->update();
        if (((PaintCanvas *)*canvasHolder)->GetScreenPosition((Vector *)posBytes, (Vector *)posBytes) != 0) {
            ((LensFlare *)(P(self, 0x2c)))->render2D(pos[0], pos[1], pos[2], I(self, 0x3c));
        }
    }
}

// ---- StarSystem_133b68.cpp ----
__attribute__((visibility("hidden"))) extern void **g_StarSystem_ctor_status;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_ctor_canvas;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_ctor_status_obj;
__attribute__((visibility("hidden"))) extern int *g_StarSystem_ctor_rng;
__attribute__((visibility("hidden"))) extern uint16_t g_StarSystem_ctor_planetTextures[];
__attribute__((visibility("hidden"))) extern uint16_t g_StarSystem_ctor_stationTextures[];
__attribute__((visibility("hidden"))) extern uint32_t g_StarSystem_ctor_systemColors[];


static inline void set_vec(char *dst, float x, float y, float z)
{
    ((float *)dst)[0] = x;
    ((float *)dst)[1] = y;
    ((float *)dst)[2] = z;
}

StarSystem::StarSystem(int mode) {
    StarSystem *self = this;
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
    B(self, 0x28) = ((Status *)(status))->getSystem() == 0;
    B(self, 0x0c) = (uint8_t)((Status *)(status))->inSupernovaSystem();

    void *flare = ::operator new(0x14);
    LensFlare_ctor(flare, *g_StarSystem_ctor_canvas);
    P(self, 0x44) = 0;
    P(self, 0x2c) = flare;
    I(self, 0x4c) = -1;

    if (B(self, 0x28) != 0) {
        void *geomArray = ::operator new(0x0c);
        Array_AEGeometry_ctor(geomArray);
        P(self, 0x1c) = geomArray;
        ArraySetLength_AEGeometry(2, geomArray);

        AEGeometry *sun = (AEGeometry *)::operator new(0xc0);
        AEGeometry_ctor(sun, 0x1a70, *g_StarSystem_ctor_canvas, false);
        ((AEGeometry **)array_data(P(self, 0x1c)))[0] = sun;
        set_vec(vec, 1000.0f, 1000.0f, 1000.0f);
        ((AEGeometry *)(sun))->setScaling(*(const Vector *)vec);
        ((AEGeometry *)(sun))->moveForward(0x447a0000);
        AEGeometry_getDirection(vec, sun);
        Vector_assign((char *)self + 0x30, vec);
        FL(self, 0x30) = -FL(self, 0x30);
        FL(self, 0x34) = -FL(self, 0x34);
        FL(self, 0x38) = -FL(self, 0x38);

        AEGeometry *planet = (AEGeometry *)::operator new(0xc0);
        AEGeometry_ctor(planet, 0x1a70, *g_StarSystem_ctor_canvas, false);
        ((AEGeometry **)array_data(P(self, 0x1c)))[1] = planet;
        int rnd = ((AbyssEngine::AERandom *)g_StarSystem_ctor_rng)->nextInt(0x4e20);
        float scale = (float)(rnd + 0x4e20) * 0.001f;
        set_vec(vec, scale, scale, scale);
        ((AEGeometry *)(planet))->setScaling(*(const Vector *)vec);
        set_vec(vec, 0.0f, 90.0f, 0.0f);
        ((AEGeometry *)(planet))->setRotation(*(const Vector *)vec);
        ((AEGeometry *)(planet))->moveForward(0x447a0000);

        void *textures = ::operator new(0x0c);
        Array_uint_ctor(textures);
        P(self, 0x14) = textures;
        ArraySetLength_uint(2, textures);
        ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(0x2739), (unsigned int *)(array_data(P(self, 0x14))), (bool)(0));
        ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(0x2719), (unsigned int *)((char *)array_data(P(self, 0x14)) + 4), (bool)(0));

        void *positions = ::operator new(0x0c);
        Array_Vector_ctor(positions);
        P(self, 0x20) = positions;
        ArraySetLength_Vector(2, positions);
        ((AEGeometry *)(vec))->getPosition();
        Vector_assign((char *)array_data(P(self, 0x20)), vec);
        ((AEGeometry *)(vec))->getPosition();
        Vector_assign((char *)array_data(P(self, 0x20)) + 0x0c, vec);

        AEGeometry *streak = (AEGeometry *)::operator new(0xc0);
        AEGeometry_ctor(streak, 0x1a70, *g_StarSystem_ctor_canvas, false);
        P(self, 0x40) = streak;
        set_vec(vec, 250.0f, 15.0f, 1000.0f);
        ((AEGeometry *)(streak))->setScaling(*(const Vector *)vec);
        ((StarSystem *)(self))->initLight();
        return;
    }

    void *system = (void *)(intptr_t)((Status *)(status))->getSystem();
    void *stations = SolarSystem_getStations(system);
    void *reader = ::operator new(1);
    FileRead_ctor(reader);
    void *stationArray = (void *)(intptr_t)((FileRead *)(reader))->loadStationsBinary();
    ::operator delete(FileRead_dtor(reader));

    void *textures = ::operator new(0x0c);
    Array_uint_ctor(textures);
    P(self, 0x14) = textures;
    ArraySetLength_uint(array_len(stationArray) + 1, textures);

    void *stationIndexes = ::operator new(0x0c);
    Array_int_ctor(stationIndexes);
    P(self, 0x24) = stationIndexes;
    ArraySetLength_int(array_len(stations), stationIndexes);

    int mission = ((Status *)(status))->getCurrentCampaignMission();
    if ((mission == 0x59 && B(self, 0x0c) != 0) ||
        (mission > 0x9d && SolarSystem_getIndex((void *)(intptr_t)((Status *)(status))->getSystem()) == 0x1b)) {
        ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(0x2dde), (unsigned int *)(array_data(P(self, 0x14))), (bool)(0));
        U(self, 0x3c) = 3;
    } else {
        uint32_t tex = SolarSystem_getTextureIndex((void *)(intptr_t)((Status *)(status))->getSystem());
        ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(g_StarSystem_ctor_planetTextures[tex]), (unsigned int *)(array_data(P(self, 0x14))), (bool)(0));
        U(self, 0x3c) = g_StarSystem_ctor_systemColors[SolarSystem_getIndex((void *)(intptr_t)((Status *)(status))->getSystem())];
    }

    uint32_t count = array_len(stations);
    for (uint32_t i = 1, off = 0; i < array_len(P(self, 0x14)); ++i, off += 4) {
        int stationIndex = ((int *)array_data(stations))[off / 4];
        int currentStation = Station_getIndex(((Status *)(status))->getStation());
        void *station = ((void **)array_data(stationArray))[off / 4];
        uint32_t stationTex = Station_getTextureIndex(station);
        if (stationIndex == currentStation) {
            if (mode == 3 && ((Status *)(status))->getCurrentCampaignMission() == 0) {
                U(self, 0x50) = i;
                ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(0x273b), (unsigned int *)((char *)array_data(P(self, 0x14)) + off + 4), (bool)(0));
            } else {
                ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(g_StarSystem_ctor_planetTextures[stationTex]), (unsigned int *)((char *)array_data(P(self, 0x14)) + off + 4), (bool)(0));
            }
        } else {
            ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(g_StarSystem_ctor_stationTextures[stationTex]), (unsigned int *)((char *)array_data(P(self, 0x14)) + off + 4), (bool)(0));
            int idx = Station_getIndex(station);
            if (((Status *)(*g_StarSystem_ctor_status_obj))->orbitHasPlanetRing(idx) != 0) {
                AEGeometry *ring = (AEGeometry *)::operator new(0xc0);
                AEGeometry_ctor(ring, 0x1a70, *g_StarSystem_ctor_canvas, false);
                P(self, 0x44) = ring;
                ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(0x7198), (unsigned int *)((char *)self + 0x48), (bool)(0));
                U(self, 0x4c) = i;
            }
        }
        ((int *)array_data(P(self, 0x24)))[off / 4] = Station_getIndex(station);
    }

    Array_Station_release(stationArray);
    ::operator delete(Array_Station_dtor(stationArray));

    void *players = ::operator new(0x0c);
    Array_KIPlayer_ctor(players);
    P(self, 0x18) = players;
    ArraySetLength_KIPlayer(count, players);

    void *geometries = ::operator new(0x0c);
    Array_AEGeometry_ctor(geometries);
    P(self, 0x1c) = geometries;
    ArraySetLength_AEGeometry(count + 1, geometries);

    void *positions = ::operator new(0x0c);
    Array_Vector_ctor(positions);
    P(self, 0x20) = positions;
    ArraySetLength_Vector(count + 1, positions);

    AERandom_setSeed(g_StarSystem_ctor_rng,
                     (long long)Station_getIndex(((Status *)(status))->getStation()) * 300);

    int sunSlot = ((AbyssEngine::AERandom *)g_StarSystem_ctor_rng)->nextInt(14);
    for (uint32_t i = 0; i < array_len(P(self, 0x1c)); ++i) {
        AEGeometry *geom = (AEGeometry *)::operator new(0xc0);
        AEGeometry_ctor(geom, 0x1a70, *g_StarSystem_ctor_canvas, false);
        ((AEGeometry **)array_data(P(self, 0x1c)))[i] = geom;

        if (i == 0) {
            float sunScale = B(self, 0x0c) == 0 ? 1000.0f : 2000.0f;
            set_vec(vec, sunScale, sunScale, sunScale);
            ((AEGeometry *)(geom))->setScaling(*(const Vector *)vec);
            AEGeometry *streak = (AEGeometry *)::operator new(0xc0);
            AEGeometry_ctor(streak, 0x1a70, *g_StarSystem_ctor_canvas, false);
            P(self, 0x40) = streak;
            set_vec(vec, B(self, 0x0c) == 0 ? 250.0f : 500.0f,
                    B(self, 0x0c) == 0 ? 15.0f : 25.0f,
                    B(self, 0x0c) == 0 ? 1000.0f : 2000.0f);
            ((AEGeometry *)(streak))->setScaling(*(const Vector *)vec);
            if (B(self, 0x0c) != 0) {
                ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(0x2dde), (unsigned int *)((char *)self + 0x10), (bool)(0));
            }
            usedSlots[sunSlot * 4] = 1;
        } else {
            void *player = ::operator new(0x130);
            PlayerStatic_ctor(player, 0, geom, 0.0f, 0.0f, 0.0f);
            ((void **)array_data(P(self, 0x18)))[i - 1] = player;
            int slot = ((AbyssEngine::AERandom *)g_StarSystem_ctor_rng)->nextInt(11) + 7;
            usedSlots[slot * 4] = 1;
            int dist = ((AbyssEngine::AERandom *)g_StarSystem_ctor_rng)->nextInt(0x4e20) + 0x4e20;
            if (((Status *)(status))->getCurrentCampaignMission() == 0) {
                dist = (int)((float)dist * 0.5f);
            }
            float scale = (float)dist * 0.001f;
            FL(self, 0x58) = scale;
            set_vec(vec, scale, scale, scale);
            ((AEGeometry *)(geom))->setScaling(*(const Vector *)vec);
        }

        int angleSlot = (i == 0) ? sunSlot : (int)i + 6;
        set_vec(vec, 0.0f, (float)(angleSlot * 0xaaa) * 0.001f, 0.0f);
        ((AEGeometry *)(geom))->setRotation(*(const Vector *)vec);
        ((AEGeometry *)(geom))->moveForward(0x447a0000);
        if (i == 0) {
            AEGeometry_getDirection(vec, geom);
            Vector_assign((char *)self + 0x30, vec);
            FL(self, 0x30) = -FL(self, 0x30);
            FL(self, 0x34) = -FL(self, 0x34);
            FL(self, 0x38) = -FL(self, 0x38);
        }
        ((AEGeometry *)(vec))->getPosition();
        Vector_assign((char *)array_data(P(self, 0x20)) + i * 0x0c, vec);
    }

    AERandom_reset(g_StarSystem_ctor_rng);
    ((StarSystem *)(self))->initLight();
    return;
}

// ---- updateSupernova_135064.cpp ----
__attribute__((visibility("hidden"))) extern void **g_StarSystem_update_canvas_a;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_update_canvas_b;


void StarSystem::updateSupernova(int dt) {
    StarSystem *self = this;
    void *streak = P(self, 0x40);
    if (streak != 0) {
        void *transform = ((PaintCanvas *)*g_StarSystem_update_canvas_a)->TransformGetTransform(I(streak, 0x0c));
        int mode = 0;
        ((AbyssEngine::Transform *)(transform))->Update(mode, dt);
    }
    void *sun = *(void **)P(P(self, 0x1c), 4);
    if (sun != 0) {
        void *transform = ((PaintCanvas *)*g_StarSystem_update_canvas_b)->TransformGetTransform(I(sun, 0x0c));
        int mode = 1;
        ((AbyssEngine::Transform *)(transform))->Update(mode, dt);
    }
}

// ---- scaleSunDuringSupernovaIntro_134ffc.cpp ----
void StarSystem::scaleSunDuringSupernovaIntro(int amount) {
    StarSystem *self = this;
    char scaleBytes[12];
    ((AEGeometry *)(scaleBytes))->getScaling();
    float scale = *(float *)scaleBytes + (float)amount * -9.769497830779909e32f;
    (*(AEGeometry **)P(P(self, 0x1c), 4))->setScaling(scale, scale, scale);
}

// ---- switchPlanetForIntro_134dcc.cpp ----
__attribute__((visibility("hidden"))) extern int *g_StarSystem_planet_guard;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_planet_canvas;


void StarSystem::switchPlanetForIntro() {
    StarSystem *self = this;
    char scaled[12];
    char current[12];
    int flags = 0;
    ((PaintCanvas *)(*g_StarSystem_planet_canvas))->TextureCreate((unsigned short)(0x273a), (unsigned int *)((char *)P(P(self, 0x14), 4) + U(self, 0x50) * 4), (bool)(flags));
    ((AEGeometry *)(current))->getScaling();
    Vector_scale(scaled, current, 2.0f);
    ((AEGeometry *)(((AEGeometry **)P(P(self, 0x1c), 4))[U(self, 0x50)]))->setScaling(*(const Vector *)scaled);
}

// ---- switchSunForSupernovaReversal_134e4c.cpp ----
__attribute__((visibility("hidden"))) extern void **g_StarSystem_reversal_canvas;


void StarSystem::switchSunForSupernovaReversal() {
    StarSystem *self = this;
    void **canvasHolder = g_StarSystem_reversal_canvas;
    char scaleBytes[12];
    ((PaintCanvas *)(*canvasHolder))->TextureCreate((unsigned short)(0x2734), (unsigned int *)(P(P(self, 0x14), 4)), (bool)(0));
    AEGeometry *geom = *(AEGeometry **)P(P(self, 0x1c), 4);
    uint32_t *scale = (uint32_t *)scaleBytes;
    scale[0] = 0x68009815;
    scale[1] = 0x68009815;
    scale[2] = 0x68009815;
    ((AEGeometry *)(geom))->setScaling(*(const Vector *)scaleBytes);
}

// ---- switchSunForSupernovaIntro_134ea0.cpp ----
typedef void *(*GetTransformFn)(void *canvas, int transform_id);
typedef void (*SetTransformModeFn)(void *transform, int mode, int value);

__attribute__((visibility("hidden"))) extern void **g_StarSystem_intro_canvas;
__attribute__((visibility("hidden"))) extern GetTransformFn g_StarSystem_intro_getTransform;
__attribute__((visibility("hidden"))) extern SetTransformModeFn g_StarSystem_intro_setTransformMode;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_intro_status;
__attribute__((visibility("hidden"))) extern uint32_t g_StarSystem_intro_colors[];


void StarSystem::switchSunForSupernovaIntro() {
    StarSystem *self = this;
    void **canvasHolder = g_StarSystem_intro_canvas;
    char scaled[12];
    char current[12];
    void *canvas = *canvasHolder;
    ((PaintCanvas *)(canvas))->TextureCreate((unsigned short)(0x2df3), (unsigned int *)(P(P(self, 0x14), 4)), (bool)(0));
    ((PaintCanvas *)(*canvasHolder))->TextureCreate((unsigned short)(0x2df4), (unsigned int *)((char *)self + 0x10), (bool)(0));

    ((AEGeometry *)((AEGeometry *)P(self, 0x40)))->setMesh(0x2df2);
    AEGeometry *streak = (AEGeometry *)P(self, 0x40);
    ((AEGeometry *)(current))->getScaling();
    Vector_scale(scaled, current, 5.0f);   // 0x40a00000
    ((AEGeometry *)(streak))->setScaling(*(const Vector *)scaled);

    AEGeometry *sun = *(AEGeometry **)P(P(self, 0x1c), 4);
    ((AEGeometry *)(sun))->setMesh(0x2df1);
    {
        uint32_t scaleBits = 0x3f2fc800;
        (*(AEGeometry **)P(P(self, 0x1c), 4))->setScaling(*(float *)&scaleBits);
    }

    GetTransformFn getTransform = g_StarSystem_intro_getTransform;
    int transformId = I(*(void **)P(P(self, 0x1c), 4), 0x0c);
    void *transform = getTransform(*canvasHolder, transformId);
    SetTransformModeFn setTransformMode = g_StarSystem_intro_setTransformMode;
    setTransformMode(transform, 0, 0);
    transformId = I(*(void **)P(P(self, 0x1c), 4), 0x0c);
    setTransformMode(getTransform(*canvasHolder, transformId), 3, 0);
    transformId = I(*(void **)P(P(self, 0x1c), 4), 0x0c);
    setTransformMode(getTransform(*canvasHolder, transformId), 1, 0);

    void *system = (void *)(intptr_t)((Status *)(*g_StarSystem_intro_status))->getSystem();
    U(self, 0x3c) = g_StarSystem_intro_colors[SolarSystem_getIndex(system)];
}

// ---- render_135104.cpp ----
__attribute__((visibility("hidden"))) extern void **g_StarSystem_render_canvas;
__attribute__((visibility("hidden"))) extern uint32_t *g_StarSystem_render_station_index;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_render_status;


void StarSystem::render() {
    StarSystem *self = this;
    char cameraPos[12];
    char savedCamera[0x3c];
    char lookAt[0x3c];
    char tempMatrix[0x3c];
    char tempVec[12];
    char up[12];
    char scaleBytes[12];

    void **canvasHolder = g_StarSystem_render_canvas;
    void *canvas = *canvasHolder;
    unsigned current = ((PaintCanvas *)canvas)->CameraGetCurrent();
    void *local = ((PaintCanvas *)canvas)->CameraGetLocal(current);
    MatrixGetPosition(cameraPos, local);

    uint32_t count = array_len(P(self, 0x1c));
    int posOffset = 0;
    for (uint32_t i = 0; i < count; ++i, posOffset += 0x0c) {
        AEGeometry **geoms = (AEGeometry **)array_data(P(self, 0x1c));
        if (count > 1) {
            if (posOffset == 0) {
                void *canvas2 = *canvasHolder;
                unsigned current2 = ((PaintCanvas *)canvas2)->CameraGetCurrent();
                void *local2 = ((PaintCanvas *)canvas2)->CameraGetLocal(current2);
                __aeabi_memcpy(savedCamera, local2, 0x3c);
                Vector_add(tempVec, (const char *)array_data(P(self, 0x20)), cameraPos);
                if (B(self, 0x0c) == 0) {
                    MatrixGetUp(up, savedCamera);
                } else {
                    ((float *)up)[0] = 1.0f;
                    ((float *)up)[1] = 0.0f;
                    ((float *)up)[2] = 0.0f;
                }
                MatrixGetLookAt(lookAt, tempVec, cameraPos, up);
                Matrix_assign(savedCamera, lookAt);
                geoms[0]->setRotation(0.0f, 0.0f, 0.0f);
                ((AEGeometry *)(tempVec))->getScaling();

                float grow = (FL(P(self, 0x2c), 0) - 10.0f) * 0.001f;
                if (grow < 0.0f) {
                    grow = 0.0f;
                }
                float sx = ((float *)tempVec)[0];
                float sy = ((float *)tempVec)[1];
                float sz = ((float *)tempVec)[2];
                if (B(self, 0x0c) == 0) {
                    sx += grow;
                    sy += grow;
                }
                MatrixSetScaling(lookAt, sx, sy, sz);
                AEGeometry_setMatrix(geoms[0], lookAt);

                if (B(self, 0x0c) != 0) {
                    Vector_mul_assign(tempVec, grow);
                    void *canvas3 = *canvasHolder;
                    unsigned current3 = ((PaintCanvas *)canvas3)->CameraGetCurrent();
                    void *local3 = ((PaintCanvas *)canvas3)->CameraGetLocal(current3);
                    Matrix_assign(savedCamera, (const char *)local3);
                    Vector_add(up, (const char *)array_data(P(self, 0x20)), cameraPos);
                    MatrixGetUp(scaleBytes, savedCamera);
                    MatrixGetLookAt(lookAt, up, cameraPos, scaleBytes);
                    Matrix_assign(savedCamera, lookAt);
                    MatrixSetScaling(lookAt, grow + ((float *)tempVec)[0],
                                     ((float *)tempVec)[1],
                                     grow + ((float *)tempVec)[2]);
                    AEGeometry_setMatrix((AEGeometry *)P(self, 0x40), lookAt);
                    ((StarSystem *)(self))->renderSunStreak();
                }

                MatrixSetScaling(lookAt,
                                 grow * (grow + ((float *)tempVec)[0] + 1.0f),
                                 ((float *)tempVec)[1],
                                 ((float *)tempVec)[1] / ((1.0f - grow) * 6.0f + 6.0f));
                AEGeometry_setMatrix((AEGeometry *)P(self, 0x40), lookAt);
            }

            uint32_t stationIndex = *g_StarSystem_render_station_index;
            bool selected = (i - 1) == stationIndex;
            if (selected && B(self, 0x28) == 0 &&
                ((Status *)(*g_StarSystem_render_status))->inPlanetRingOrbit() == 0) {
                float ring = ((float *)cameraPos)[2] / 65536.0f;
                float clamped = ring < 0.0f ? ring : 0.0f;
                if (clamped < -1.0f) {
                    clamped = -1.0f;
                }
                FL(self, 0x5c) = clamped;
                geoms[i]->setScaling(FL(self, 0x58) + clamped,
                                     FL(self, 0x58) + clamped,
                                     FL(self, 0x58) + clamped);
            }

            Vector_add(tempMatrix,
                       (const char *)array_data(P(self, 0x20)) + posOffset,
                       cameraPos);
            ((AEGeometry *)(geoms[i]))->setPosition(*(const Vector *)tempMatrix);
        }

        ((PaintCanvas *)*canvasHolder)->SetTexture(((uint32_t *)array_data(P(self, 0x14)))[i], (unsigned)-1);
        int blend;
        if (posOffset == 0) {
            blend = 2;
        } else {
            blend = B(self, 0x54) == 0 ? 1 : 0x15;
        }
        ((PaintCanvas *)*canvasHolder)->SetBlendMode(blend);
        ((AEGeometry *)(((AEGeometry **)array_data(P(self, 0x1c)))[i]))->render();

        if (i == U(self, 0x4c)) {
            ((PaintCanvas *)*canvasHolder)->SetTexture(U(self, 0x48), (unsigned)-1);
            ((PaintCanvas *)*canvasHolder)->SetBlendMode(1);
            __aeabi_memcpy(savedCamera, ((AEGeometry *)(((AEGeometry **)array_data(P(self, 0x1c)))[i]))->getMatrix(), 0x3c);
            MatrixSetScaling(lookAt, 4.0f, 4.0f, 4.0f);
            AEGeometry_setMatrix((AEGeometry *)P(self, 0x44), lookAt);
            ((AEGeometry *)((AEGeometry *)P(self, 0x44)))->render();
        }
    }
}
