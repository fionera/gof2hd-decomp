#include "gof2/game/world/StarSystem.h"
#include "gof2/engine/core/AERandom.h"
#include "gof2/game/world/SolarSystem.h"
// AEGeometry.h declares __aeabi_memcpy with a uint32_t length, which clashes with
// Engine.h's unsigned-long version. Rename the loose decl away while parsing AEGeometry.h
// so only one canonical prototype survives.
#define __aeabi_memcpy __aeabi_memcpy_aegeom_unused
#include "gof2/engine/render/AEGeometry.h"
#undef __aeabi_memcpy
#include "gof2/engine/file/FileRead.h"
#include "gof2/engine/render/LensFlare.h"
#include "gof2/game/mission/Status.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/engine/render/Engine.h"
#include "gof2/game/core/PaintCanvasClass.h"
#include "gof2/game/ship/PlayerStatic.h"

// Free AEMath operators used below (declared in AEMath.cpp; not pulling in AEMath.h
// here because it redefines BSphere/Transform which clash in this TU).
namespace AbyssEngine { namespace AEMath {
Vector operator+(const Vector &lhs, const Vector &rhs);
Vector operator*(const Vector &lhs, float rhs);
} }

extern "C" void *LensFlare_dtor(void *flare);
// playerTargets (getPlanetTargets) is read by LevelScript.cpp using the flattened
// {count,data} ABI (P(targets,4)[...]); keep it on the original container helpers so that
// cross-TU layout stays consistent. See REPORT.
extern "C" void ArrayRelease_KIPlayer(void *array);
extern "C" void *Array_KIPlayer_dtor(void *array);
extern "C" void Array_KIPlayer_ctor(void *array);
extern "C" void ArraySetLength_KIPlayer(int length, void *array);
extern "C" uint32_t Station_getTextureIndex(void *station);
extern "C" int Station_getIndex(void *station);
extern "C" void FileRead_ctor(void *self);
extern "C" void *FileRead_dtor(void *self);
extern "C" void Array_Station_release(void *array);
extern "C" void *Array_Station_dtor(void *array);
extern "C" void LensFlare_ctor(void *self, void *canvas);

// ---- Typed array members ----
// The recovered code stored these as flattened void* slots manipulated by Array_<T>_*
// container helpers. They are now real Array<T> (== std::vector<T>) heap-object members:
// class-element arrays hold owning-free element pointers, value arrays hold the values inline.
//   texturesArray    station/planet texture handles
//   planetsArray     sun + planet/station billboard meshes
//   positionsArray   per-mesh world positions
//   stationIdxArray  station indices
// (playerTargets stays on the flattened container helpers because LevelScript.cpp reads it directly.)
void MatrixGetPosition(char *out, void *matrix);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, unsigned long n);
void MatrixGetUp(char *out, const char *matrix);
void MatrixGetLookAt(char *out, const char *from, const char *to, const char *up);
extern "C" void MatrixSetScaling(char *matrix, float x, float y, float z);

void StarSystem::switchSunForSupernovaExpansion() {
    char scaleBytes[12];
    AEGeometry *geom = (*this->planetsArray)[0];
    uint32_t *scale = (uint32_t *)scaleBytes;
    scale[0] = 0x46311048;
    scale[1] = 0x46311048;
    scale[2] = 0x46311048;
    ((AEGeometry *)(geom))->setScaling(*(const Vector *)scaleBytes);
}

__attribute__((visibility("hidden"))) extern void **g_StarSystem_sunstreak_canvas;

void StarSystem::renderSunStreak() {
    void **holder = g_StarSystem_sunstreak_canvas;
    void *canvas = *holder;
    void *slot = (this->supernovaSystem != 0) ? (void *)&this->supernovaSunTexture : (void *)&(*this->texturesArray)[0];
    ((PaintCanvas *)canvas)->SetTexture(*(uint32_t *)slot, (unsigned)-1);
    ((PaintCanvas *)*holder)->SetBlendMode(2);
    return this->renderSunStreak_tail(this->sunStreak);
}

// ---- renderSunStreak_tail ----------------------------------------------------
// The trailing tail-call of renderSunStreak() (a GOT veneer) draws the sun-streak
// billboard geometry held at field +0x40: AEGeometry::render().
void StarSystem::renderSunStreak_tail(void *geom)
{
    if (geom != 0)
        ((AEGeometry *)geom)->render();
}

// ---- getPlanetTargets / getPlanets ----
// Simple accessors for the two planet geometry arrays (playerTargets and planetsArray).
void *StarSystem::getPlanetTargets() {
    return this->playerTargets;
}

void *StarSystem::getPlanets() {
    return this->planetsArray;
}

Vector StarSystem::getLightDirection() {
    return this->lightDirection;
}

StarSystem::~StarSystem()
{
    delete this->planetsArray;
    this->planetsArray = nullptr;

    void *players = this->playerTargets;
    if (players != 0) {
        ArrayRelease_KIPlayer(players);
        players = this->playerTargets;
        if (players != 0) {
            ::operator delete(Array_KIPlayer_dtor(players));
        }
    }
    this->playerTargets = 0;

    void *p = this->lensFlare;
    if (p != 0) {
        ::operator delete(LensFlare_dtor(p));
    }
    this->lensFlare = 0;

    if (this->texturesArray != nullptr) {
        this->texturesArray->clear();
        delete this->texturesArray;
    }
    this->texturesArray = nullptr;

    if (this->stationIdxArray != nullptr) {
        this->stationIdxArray->clear();
        delete this->stationIdxArray;
    }
    this->stationIdxArray = nullptr;

    if (this->positionsArray != nullptr) {
        this->positionsArray->clear();
        delete this->positionsArray;
    }
    this->positionsArray = nullptr;
}

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
    void *app = *g_StarSystem_init_app;
    Engine *engine = (Engine *)((ApplicationManager *)(app))->GetEngine();
    I(engine, 0x32c) = 2;

    void *status = *g_StarSystem_init_status;
    uint32_t baseIndex;
    bool normalSystem;
    if (this->abstractSystem == 0) {
        void *system = (void *)(intptr_t)((Status *)(status))->getSystem();
        baseIndex = ((SolarSystem *)(system))->getTextureIndex() * 3;
        normalSystem = true;
    } else {
        baseIndex = 0x1e;
        normalSystem = false;
    }

    float ambientScale = 0.5f;
    this->sunLightColor.x = g_StarSystem_init_sunColors[baseIndex];
    this->sunLightColor.y = g_StarSystem_init_sunColors[baseIndex + 1];
    this->sunLightColor.z = g_StarSystem_init_sunColors[baseIndex + 2];

    if (normalSystem) {
        void *system = (void *)(intptr_t)((Status *)(status))->getSystem();
        uint32_t tex = ((SolarSystem *)(system))->getTextureIndex();
        int mission = ((Status *)(status))->getCurrentCampaignMission();
        if (tex == 0x0f && (mission == 0x59 || mission > 0x9d)) {
            this->sunLightColor.x *= 0.5f;
            this->sunLightColor.y *= 0.5f;
            this->sunLightColor.z *= 0.5f;
        }
        if (this->abstractSystem == 0) {
            system = (void *)(intptr_t)((Status *)(status))->getSystem();
            tex = ((SolarSystem *)(system))->getTextureIndex();
            ambientScale = 0.5f;
            mission = ((Status *)(status))->getCurrentCampaignMission();
            if (tex == 0x0f && mission != 0x59) {
                ambientScale = mission < 0x9e ? 0.35f : 0.6f;
            }
        }
    }

    uint32_t stationColorIndex = 0x17;
    if (normalSystem && this->abstractSystem == 0) {
        void *station = ((Status *)(status))->getStation();
        stationColorIndex = Station_getTextureIndex(station) * 3;
    }

    float lr = g_StarSystem_init_stationColors[stationColorIndex];
    float lg = g_StarSystem_init_stationColors[stationColorIndex + 1];
    float lb = g_StarSystem_init_stationColors[stationColorIndex + 2];
    if (normalSystem && this->abstractSystem == 0) {
        void *system = (void *)(intptr_t)((Status *)(status))->getSystem();
        if (((SolarSystem *)(system))->getTextureIndex() == 0x0f &&
            ((Status *)(status))->getCurrentCampaignMission() != 0x59) {
            if (((Status *)(status))->getCurrentCampaignMission() < 0x9e) {
                lr = lr;
            }
        }
    }

    EngineGetter getEngine = g_StarSystem_init_getEngine;
    engine = getEngine(app);
    ((Engine *)(engine))->LightSetGlobalSceneColorAmbient(ambientScale * this->sunLightColor.x, ambientScale * this->sunLightColor.y, ambientScale * this->sunLightColor.z);
    engine = getEngine(app);
    ((Engine *)(engine))->LightSetRimColor(g_StarSystem_init_lightColors[baseIndex] * 3.0f, g_StarSystem_init_lightColors[baseIndex + 1] * 3.0f, g_StarSystem_init_lightColors[baseIndex + 2] * 3.0f);
    ((Engine *)(getEngine(app)))->LightSetMaterialColorAmbient(1.0f, 1.0f, 1.0f);
    ((Engine *)(getEngine(app)))->LightSetMaterialColorDiffuse(1.0f, 1.0f, 1.0f);
    ((Engine *)(getEngine(app)))->LightSetMaterialColorSpecular(1.0f, 1.0f, 1.0f);
    ((Engine *)(getEngine(app)))->LightSetMaterialColorShininess(0.7f);
    ((Engine *)(getEngine(app)))->LightSetLightDirection(this->lightDirection.x, this->lightDirection.y, this->lightDirection.z, 0x4000);
    ((Engine *)(getEngine(app)))->LightSetLightColorAmbient(0.0f, 0.0f, 0.0f, 0x4000);
    ((Engine *)(getEngine(app)))->LightSetLightColorDiffuse(this->sunLightColor.x, this->sunLightColor.y, this->sunLightColor.z, 0x4000);
    ((Engine *)(getEngine(app)))->LightSetLightColorSpecular(2.0f, 2.0f, 2.0f, 0x4000);
    ((Engine *)(getEngine(app)))->LightSetLightDirection(0.0f, 0.0f, 1.0f, 0x4001);
    ((Engine *)(getEngine(app)))->LightSetLightColorDiffuse(lr * 1.5f, lg * 1.5f, lb * 1.5f, 0x4001);
    ((Engine *)(getEngine(app)))->LightSetLightColorSpecular(lr * 1.5f, lg * 1.5f, lb * 1.5f, 0x4001);
    ((Engine *)(getEngine(app)))->LightSetParticleAmbient(this->sunLightColor.x, this->sunLightColor.y, this->sunLightColor.z);

    void *canvas = *g_StarSystem_init_canvas;
    ((PaintCanvas *)canvas)->FogEnable(0, 1);
    this->fogEnabled = 0;
    if (this->abstractSystem != 0) {
        return;
    }

    uint32_t tex = ((SolarSystem *)((void *)(intptr_t)((Status *)(status))->getSystem()))->getTextureIndex();
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
        this->fogEnabled = 1;
        for (uint32_t i = 1; i < this->planetsArray->size(); ++i) {
            AEGeometry *geom = (*this->planetsArray)[i];
            void *transform = ((PaintCanvas *)canvas)->TransformGetTransform(I(geom, 0x0c));
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
    this->fogEnabled = 1;
    uint32_t scaled = rgba_scaled(fogColor, 0.65f);
    for (uint32_t i = 1; i < this->planetsArray->size(); ++i) {
        AEGeometry *geom = (*this->planetsArray)[i];
        void *transform = ((PaintCanvas *)canvas)->TransformGetTransform(I(geom, 0x0c));
        U(transform, 0x48) = scaled;
    }
}

__attribute__((visibility("hidden"))) extern void **g_StarSystem_render2d_canvas;

void StarSystem::render2D() {
    char posBytes[12];
    void *flare = this->lensFlare;
    if (flare != 0) {
        void **canvasHolder = g_StarSystem_render2d_canvas;
        void *canvas = *canvasHolder;
        unsigned current = ((PaintCanvas *)canvas)->CameraGetCurrent();
        void *local = ((PaintCanvas *)canvas)->CameraGetLocal(current);
        float *pos = (float *)posBytes;
        pos[0] = FL(local, 0x0c) + this->lightDirection.x * 65536.0f;
        pos[1] = FL(local, 0x1c) + this->lightDirection.y * 65536.0f;
        pos[2] = FL(local, 0x2c) + this->lightDirection.z * 65536.0f;
        ((LensFlare *)(this->lensFlare))->update();
        if (((PaintCanvas *)*canvasHolder)->GetScreenPosition((Vector *)posBytes, (Vector *)posBytes) != 0) {
            ((LensFlare *)(this->lensFlare))->render2D(pos[0], pos[1], pos[2], this->tintColor);
        }
    }
}

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
    char vec[12];
    char usedSlots[0x60];
    for (unsigned i = 0; i < sizeof(usedSlots); ++i) {
        usedSlots[i] = 0;
    }

    this->lightDirection.x = 0.0f;
    this->lightDirection.y = 0.0f;
    this->lightDirection.z = 0.0f;
    this->stationIdxArray = nullptr;
    this->texturesArray = nullptr;
    this->playerTargets = 0;
    this->planetsArray = nullptr;

    void *status = *g_StarSystem_ctor_status;
    this->abstractSystem = ((Status *)(status))->getSystem() == 0;
    this->supernovaSystem = (uint8_t)((Status *)(status))->inSupernovaSystem();

    void *flare = ::operator new(0x14);
    LensFlare_ctor(flare, *g_StarSystem_ctor_canvas);
    this->planetRing = 0;
    this->lensFlare = (LensFlare *)flare;
    this->planetRingIndex = -1;

    if (this->abstractSystem != 0) {
        this->planetsArray = new Array<AEGeometry *>();
        this->planetsArray->resize(2);

        AEGeometry *sun = (AEGeometry *)::operator new(0xc0);
        new ((void*)sun) AEGeometry((uint16_t)0x1a70, (PaintCanvas*)*g_StarSystem_ctor_canvas, false);
        (*this->planetsArray)[0] = sun;
        set_vec(vec, 1000.0f, 1000.0f, 1000.0f);
        ((AEGeometry *)(sun))->setScaling(*(const Vector *)vec);
        ((AEGeometry *)(sun))->moveForward(0x447a0000);
        *(AbyssEngine::AEMath::Vector*)(vec) = ((AEGeometry*)sun)->getDirection();
        this->lightDirection = *(const Vector *)(vec);
        this->lightDirection.x = -this->lightDirection.x;
        this->lightDirection.y = -this->lightDirection.y;
        this->lightDirection.z = -this->lightDirection.z;

        AEGeometry *planet = (AEGeometry *)::operator new(0xc0);
        new ((void*)planet) AEGeometry((uint16_t)0x1a70, (PaintCanvas*)*g_StarSystem_ctor_canvas, false);
        (*this->planetsArray)[1] = planet;
        int rnd = ((AbyssEngine::AERandom *)g_StarSystem_ctor_rng)->nextInt(0x4e20);
        float scale = (float)(rnd + 0x4e20) * 0.001f;
        set_vec(vec, scale, scale, scale);
        ((AEGeometry *)(planet))->setScaling(*(const Vector *)vec);
        set_vec(vec, 0.0f, 90.0f, 0.0f);
        ((AEGeometry *)(planet))->setRotation(*(const Vector *)vec);
        ((AEGeometry *)(planet))->moveForward(0x447a0000);

        this->texturesArray = new Array<uint32_t>();
        this->texturesArray->resize(2);
        ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(0x2739), &(*this->texturesArray)[0], (bool)(0));
        ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(0x2719), &(*this->texturesArray)[1], (bool)(0));

        this->positionsArray = new Array<Vector>();
        this->positionsArray->resize(2);
        ((AEGeometry *)(vec))->getPosition();
        (*this->positionsArray)[0] = *(const Vector *)(vec);
        ((AEGeometry *)(vec))->getPosition();
        (*this->positionsArray)[1] = *(const Vector *)(vec);

        AEGeometry *streak = (AEGeometry *)::operator new(0xc0);
        new ((void*)streak) AEGeometry((uint16_t)0x1a70, (PaintCanvas*)*g_StarSystem_ctor_canvas, false);
        this->sunStreak = streak;
        set_vec(vec, 250.0f, 15.0f, 1000.0f);
        ((AEGeometry *)(streak))->setScaling(*(const Vector *)vec);
        ((StarSystem *)(this))->initLight();
        return;
    }

    void *system = (void *)(intptr_t)((Status *)(status))->getSystem();
    void *stations = ((SolarSystem *)(system))->getStations();
    void *reader = ::operator new(1);
    FileRead_ctor(reader);
    void *stationArray = (void *)(intptr_t)((FileRead *)(reader))->loadStationsBinary();
    ::operator delete(FileRead_dtor(reader));

    this->texturesArray = new Array<uint32_t>();
    this->texturesArray->resize(array_len(stationArray) + 1);

    this->stationIdxArray = new Array<int>();
    this->stationIdxArray->resize(array_len(stations));

    int mission = ((Status *)(status))->getCurrentCampaignMission();
    if ((mission == 0x59 && this->supernovaSystem != 0) ||
        (mission > 0x9d && ((SolarSystem *)((void *)(intptr_t)((Status *)(status))->getSystem()))->getIndex() == 0x1b)) {
        ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(0x2dde), &(*this->texturesArray)[0], (bool)(0));
        this->tintColor = 3;
    } else {
        uint32_t tex = ((SolarSystem *)((void *)(intptr_t)((Status *)(status))->getSystem()))->getTextureIndex();
        ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(g_StarSystem_ctor_planetTextures[tex]), &(*this->texturesArray)[0], (bool)(0));
        this->tintColor = g_StarSystem_ctor_systemColors[((SolarSystem *)((void *)(intptr_t)((Status *)(status))->getSystem()))->getIndex()];
    }

    uint32_t count = array_len(stations);
    for (uint32_t i = 1, off = 0; i < this->texturesArray->size(); ++i, off += 4) {
        int stationIndex = ((int *)array_data(stations))[off / 4];
        int currentStation = Station_getIndex(((Status *)(status))->getStation());
        void *station = ((void **)array_data(stationArray))[off / 4];
        uint32_t stationTex = Station_getTextureIndex(station);
        if (stationIndex == currentStation) {
            if (mode == 3 && ((Status *)(status))->getCurrentCampaignMission() == 0) {
                this->selectedStationSlot = i;
                ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(0x273b), &(*this->texturesArray)[i], (bool)(0));
            } else {
                ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(g_StarSystem_ctor_planetTextures[stationTex]), &(*this->texturesArray)[i], (bool)(0));
            }
        } else {
            ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(g_StarSystem_ctor_stationTextures[stationTex]), &(*this->texturesArray)[i], (bool)(0));
            int idx = Station_getIndex(station);
            if (((Status *)(*g_StarSystem_ctor_status_obj))->orbitHasPlanetRing(idx) != 0) {
                AEGeometry *ring = (AEGeometry *)::operator new(0xc0);
                new ((void*)ring) AEGeometry((uint16_t)0x1a70, (PaintCanvas*)*g_StarSystem_ctor_canvas, false);
                this->planetRing = ring;
                ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(0x7198), &this->planetRingTexture, (bool)(0));
                this->planetRingIndex = i;
            }
        }
        (*this->stationIdxArray)[off / 4] = Station_getIndex(station);
    }

    Array_Station_release(stationArray);
    ::operator delete(Array_Station_dtor(stationArray));

    void *players = ::operator new(0x0c);
    Array_KIPlayer_ctor(players);
    this->playerTargets = players;
    ArraySetLength_KIPlayer(count, players);

    this->planetsArray = new Array<AEGeometry *>();
    this->planetsArray->resize(count + 1);

    this->positionsArray = new Array<Vector>();
    this->positionsArray->resize(count + 1);

    ((AbyssEngine::AERandom *)(g_StarSystem_ctor_rng))->setSeed((long long)Station_getIndex(((Status *)(status))->getStation()) * 300);

    int sunSlot = ((AbyssEngine::AERandom *)g_StarSystem_ctor_rng)->nextInt(14);
    for (uint32_t i = 0; i < this->planetsArray->size(); ++i) {
        AEGeometry *geom = (AEGeometry *)::operator new(0xc0);
        new ((void*)geom) AEGeometry((uint16_t)0x1a70, (PaintCanvas*)*g_StarSystem_ctor_canvas, false);
        (*this->planetsArray)[i] = geom;

        if (i == 0) {
            float sunScale = this->supernovaSystem == 0 ? 1000.0f : 2000.0f;
            set_vec(vec, sunScale, sunScale, sunScale);
            ((AEGeometry *)(geom))->setScaling(*(const Vector *)vec);
            AEGeometry *streak = (AEGeometry *)::operator new(0xc0);
            new ((void*)streak) AEGeometry((uint16_t)0x1a70, (PaintCanvas*)*g_StarSystem_ctor_canvas, false);
            this->sunStreak = streak;
            set_vec(vec, this->supernovaSystem == 0 ? 250.0f : 500.0f,
                    this->supernovaSystem == 0 ? 15.0f : 25.0f,
                    this->supernovaSystem == 0 ? 1000.0f : 2000.0f);
            ((AEGeometry *)(streak))->setScaling(*(const Vector *)vec);
            if (this->supernovaSystem != 0) {
                ((PaintCanvas *)(*g_StarSystem_ctor_canvas))->TextureCreate((unsigned short)(0x2dde), &this->supernovaSunTexture, (bool)(0));
            }
            usedSlots[sunSlot * 4] = 1;
        } else {
            void *player = ::operator new(0x130);
            ((PlayerStatic *)player)->ctor(0, geom, 0.0f, 0.0f, 0.0f);
            ((void **)array_data(this->playerTargets))[i - 1] = player;
            int slot = ((AbyssEngine::AERandom *)g_StarSystem_ctor_rng)->nextInt(11) + 7;
            usedSlots[slot * 4] = 1;
            int dist = ((AbyssEngine::AERandom *)g_StarSystem_ctor_rng)->nextInt(0x4e20) + 0x4e20;
            if (((Status *)(status))->getCurrentCampaignMission() == 0) {
                dist = (int)((float)dist * 0.5f);
            }
            float scale = (float)dist * 0.001f;
            this->planetScale = scale;
            set_vec(vec, scale, scale, scale);
            ((AEGeometry *)(geom))->setScaling(*(const Vector *)vec);
        }

        int angleSlot = (i == 0) ? sunSlot : (int)i + 6;
        set_vec(vec, 0.0f, (float)(angleSlot * 0xaaa) * 0.001f, 0.0f);
        ((AEGeometry *)(geom))->setRotation(*(const Vector *)vec);
        ((AEGeometry *)(geom))->moveForward(0x447a0000);
        if (i == 0) {
            *(AbyssEngine::AEMath::Vector*)(vec) = ((AEGeometry*)geom)->getDirection();
            this->lightDirection = *(const Vector *)(vec);
            this->lightDirection.x = -this->lightDirection.x;
            this->lightDirection.y = -this->lightDirection.y;
            this->lightDirection.z = -this->lightDirection.z;
        }
        ((AEGeometry *)(vec))->getPosition();
        (*this->positionsArray)[i] = *(const Vector *)(vec);
    }

    ((AbyssEngine::AERandom *)(g_StarSystem_ctor_rng))->reset();
    ((StarSystem *)(this))->initLight();
    return;
}

__attribute__((visibility("hidden"))) extern void **g_StarSystem_update_canvas_a;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_update_canvas_b;

void StarSystem::updateSupernova(int dt) {
    void *streak = this->sunStreak;
    if (streak != 0) {
        void *transform = ((PaintCanvas *)*g_StarSystem_update_canvas_a)->TransformGetTransform(I(streak, 0x0c));
        int mode = 0;
        ((AbyssEngine::Transform *)(transform))->Update(mode, dt);
    }
    void *sun = (*this->planetsArray)[0];
    if (sun != 0) {
        void *transform = ((PaintCanvas *)*g_StarSystem_update_canvas_b)->TransformGetTransform(I(sun, 0x0c));
        int mode = 1;
        ((AbyssEngine::Transform *)(transform))->Update(mode, dt);
    }
}

void StarSystem::scaleSunDuringSupernovaIntro(int amount) {
    char scaleBytes[12];
    ((AEGeometry *)(scaleBytes))->getScaling();
    float scale = *(float *)scaleBytes + (float)amount * -9.769497830779909e32f;
    (*this->planetsArray)[0]->setScaling(scale, scale, scale);
}

__attribute__((visibility("hidden"))) extern int *g_StarSystem_planet_guard;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_planet_canvas;

void StarSystem::switchPlanetForIntro() {
    char scaled[12];
    char current[12];
    int flags = 0;
    ((PaintCanvas *)(*g_StarSystem_planet_canvas))->TextureCreate((unsigned short)(0x273a), &(*this->texturesArray)[this->selectedStationSlot], (bool)(flags));
    ((AEGeometry *)(current))->getScaling();
    *(Vector *)(scaled) = *(const Vector *)(current) * (2.0f);
    (*this->planetsArray)[this->selectedStationSlot]->setScaling(*(const Vector *)scaled);
}

__attribute__((visibility("hidden"))) extern void **g_StarSystem_reversal_canvas;

void StarSystem::switchSunForSupernovaReversal() {
    void **canvasHolder = g_StarSystem_reversal_canvas;
    char scaleBytes[12];
    ((PaintCanvas *)(*canvasHolder))->TextureCreate((unsigned short)(0x2734), &(*this->texturesArray)[0], (bool)(0));
    AEGeometry *geom = (*this->planetsArray)[0];
    uint32_t *scale = (uint32_t *)scaleBytes;
    scale[0] = 0x68009815;
    scale[1] = 0x68009815;
    scale[2] = 0x68009815;
    ((AEGeometry *)(geom))->setScaling(*(const Vector *)scaleBytes);
}

typedef void *(*GetTransformFn)(void *canvas, int transform_id);
typedef void (*SetTransformModeFn)(void *transform, int mode, int value);

__attribute__((visibility("hidden"))) extern void **g_StarSystem_intro_canvas;
__attribute__((visibility("hidden"))) extern GetTransformFn g_StarSystem_intro_getTransform;
__attribute__((visibility("hidden"))) extern SetTransformModeFn g_StarSystem_intro_setTransformMode;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_intro_status;
__attribute__((visibility("hidden"))) extern uint32_t g_StarSystem_intro_colors[];

void StarSystem::switchSunForSupernovaIntro() {
    void **canvasHolder = g_StarSystem_intro_canvas;
    char scaled[12];
    char current[12];
    void *canvas = *canvasHolder;
    ((PaintCanvas *)(canvas))->TextureCreate((unsigned short)(0x2df3), &(*this->texturesArray)[0], (bool)(0));
    ((PaintCanvas *)(*canvasHolder))->TextureCreate((unsigned short)(0x2df4), &this->supernovaSunTexture, (bool)(0));

    ((AEGeometry *)((AEGeometry *)this->sunStreak))->setMesh(0x2df2);
    AEGeometry *streak = (AEGeometry *)this->sunStreak;
    ((AEGeometry *)(current))->getScaling();
    *(Vector *)(scaled) = *(const Vector *)(current) * (5.0f);
    ((AEGeometry *)(streak))->setScaling(*(const Vector *)scaled);

    AEGeometry *sun = (*this->planetsArray)[0];
    ((AEGeometry *)(sun))->setMesh(0x2df1);
    {
        uint32_t scaleBits = 0x3f2fc800;
        (*this->planetsArray)[0]->setScaling(*(float *)&scaleBits);
    }

    GetTransformFn getTransform = g_StarSystem_intro_getTransform;
    int transformId = I((*this->planetsArray)[0], 0x0c);
    void *transform = getTransform(*canvasHolder, transformId);
    SetTransformModeFn setTransformMode = g_StarSystem_intro_setTransformMode;
    setTransformMode(transform, 0, 0);
    transformId = I((*this->planetsArray)[0], 0x0c);
    setTransformMode(getTransform(*canvasHolder, transformId), 3, 0);
    transformId = I((*this->planetsArray)[0], 0x0c);
    setTransformMode(getTransform(*canvasHolder, transformId), 1, 0);

    void *system = (void *)(intptr_t)((Status *)(*g_StarSystem_intro_status))->getSystem();
    this->tintColor = g_StarSystem_intro_colors[((SolarSystem *)(system))->getIndex()];
}

__attribute__((visibility("hidden"))) extern void **g_StarSystem_render_canvas;
__attribute__((visibility("hidden"))) extern uint32_t *g_StarSystem_render_station_index;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_render_status;

void StarSystem::render() {
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

    uint32_t count = this->planetsArray->size();
    int posOffset = 0;
    for (uint32_t i = 0; i < count; ++i, posOffset += 0x0c) {
        AEGeometry **geoms = this->planetsArray->data();
        if (count > 1) {
            if (posOffset == 0) {
                void *canvas2 = *canvasHolder;
                unsigned current2 = ((PaintCanvas *)canvas2)->CameraGetCurrent();
                void *local2 = ((PaintCanvas *)canvas2)->CameraGetLocal(current2);
                __aeabi_memcpy(savedCamera, local2, 0x3c);
                *(Vector *)(tempVec) = (*this->positionsArray)[0] + *(const Vector *)(cameraPos);
                if (this->supernovaSystem == 0) {
                    MatrixGetUp(up, savedCamera);
                } else {
                    ((float *)up)[0] = 1.0f;
                    ((float *)up)[1] = 0.0f;
                    ((float *)up)[2] = 0.0f;
                }
                MatrixGetLookAt(lookAt, tempVec, cameraPos, up);
                *(Matrix *)(savedCamera) = *(const Matrix *)(lookAt);
                geoms[0]->setRotation(0.0f, 0.0f, 0.0f);
                ((AEGeometry *)(tempVec))->getScaling();

                float grow = (FL(this->lensFlare, 0) - 10.0f) * 0.001f;
                if (grow < 0.0f) {
                    grow = 0.0f;
                }
                float sx = ((float *)tempVec)[0];
                float sy = ((float *)tempVec)[1];
                float sz = ((float *)tempVec)[2];
                if (this->supernovaSystem == 0) {
                    sx += grow;
                    sy += grow;
                }
                MatrixSetScaling(lookAt, sx, sy, sz);
                ((AEGeometry*)geoms[0])->setMatrix(*(const AbyssEngine::AEMath::Matrix*)(lookAt));

                if (this->supernovaSystem != 0) {
                    *(Vector *)(tempVec) *= (grow);
                    void *canvas3 = *canvasHolder;
                    unsigned current3 = ((PaintCanvas *)canvas3)->CameraGetCurrent();
                    void *local3 = ((PaintCanvas *)canvas3)->CameraGetLocal(current3);
                    *(Matrix *)(savedCamera) = *(const Matrix *)((const char *)local3);
                    *(Vector *)(up) = (*this->positionsArray)[0] + *(const Vector *)(cameraPos);
                    MatrixGetUp(scaleBytes, savedCamera);
                    MatrixGetLookAt(lookAt, up, cameraPos, scaleBytes);
                    *(Matrix *)(savedCamera) = *(const Matrix *)(lookAt);
                    MatrixSetScaling(lookAt, grow + ((float *)tempVec)[0],
                                     ((float *)tempVec)[1],
                                     grow + ((float *)tempVec)[2]);
                    ((AEGeometry*)(AEGeometry *)this->sunStreak)->setMatrix(*(const AbyssEngine::AEMath::Matrix*)(lookAt));
                    ((StarSystem *)(this))->renderSunStreak();
                }

                MatrixSetScaling(lookAt,
                                 grow * (grow + ((float *)tempVec)[0] + 1.0f),
                                 ((float *)tempVec)[1],
                                 ((float *)tempVec)[1] / ((1.0f - grow) * 6.0f + 6.0f));
                ((AEGeometry*)(AEGeometry *)this->sunStreak)->setMatrix(*(const AbyssEngine::AEMath::Matrix*)(lookAt));
            }

            uint32_t stationIndex = *g_StarSystem_render_station_index;
            bool selected = (i - 1) == stationIndex;
            if (selected && this->abstractSystem == 0 &&
                ((Status *)(*g_StarSystem_render_status))->inPlanetRingOrbit() == 0) {
                float ring = ((float *)cameraPos)[2] / 65536.0f;
                float clamped = ring < 0.0f ? ring : 0.0f;
                if (clamped < -1.0f) {
                    clamped = -1.0f;
                }
                this->planetRingScaleOffset = clamped;
                geoms[i]->setScaling(this->planetScale + clamped,
                                     this->planetScale + clamped,
                                     this->planetScale + clamped);
            }

            *(Vector *)(tempMatrix) = (*this->positionsArray)[i] + *(const Vector *)(cameraPos);
            ((AEGeometry *)(geoms[i]))->setPosition(*(const Vector *)tempMatrix);
        }

        ((PaintCanvas *)*canvasHolder)->SetTexture((*this->texturesArray)[i], (unsigned)-1);
        int blend;
        if (posOffset == 0) {
            blend = 2;
        } else {
            blend = this->fogEnabled == 0 ? 1 : 0x15;
        }
        ((PaintCanvas *)*canvasHolder)->SetBlendMode(blend);
        (*this->planetsArray)[i]->render();

        if (i == this->planetRingIndex) {
            ((PaintCanvas *)*canvasHolder)->SetTexture(this->planetRingTexture, (unsigned)-1);
            ((PaintCanvas *)*canvasHolder)->SetBlendMode(1);
            __aeabi_memcpy(savedCamera, &(*this->planetsArray)[i]->getMatrix(), 0x3c);
            MatrixSetScaling(lookAt, 4.0f, 4.0f, 4.0f);
            ((AEGeometry*)(AEGeometry *)this->planetRing)->setMatrix(*(const AbyssEngine::AEMath::Matrix*)(lookAt));
            ((AEGeometry *)((AEGeometry *)this->planetRing))->render();
        }
    }
}
