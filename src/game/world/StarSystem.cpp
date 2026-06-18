#include "game/world/StarSystem.h"
#include "engine/math/Transform.h"
#include "game/core/PaintCanvasClass.h"
#include "engine/core/AERandom.h"
#include "game/world/SolarSystem.h"
#include "game/world/Station.h"
#include "engine/render/AEGeometry.h"
#include "engine/file/FileRead.h"
#include "game/mission/Status.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/Engine.h"
#include "game/ship/PlayerStatic.h"
#include "platform/libc.h"

using AbyssEngine::AERandom;

// PaintCanvas.h cannot be included here (its self-contained Mesh/Transform
// definitions clash with the leaner PaintCanvasClass.h used by this TU), so
// declare the canonical render-canvas singleton locally; it matches the
// definition in PaintCanvas.h.
extern PaintCanvas* gCanvas;

// LensFlare's own header (engine/render/LensFlare.h) forward-declares PaintCanvas
// as a class *inside* namespace AbyssEngine, which clashes with the canonical
// `using ::PaintCanvas;` brought in by PaintCanvasClass.h above. Re-declare the
// LensFlare interface here against the canonical `::PaintCanvas` so the only
// PlanetSystem use (new LensFlare(canvas)) resolves without pulling in that
// conflicting forward declaration. Layout/signature are identical to LensFlare.h.
class LensFlare {
public:
    float intensity;                 // current flare alpha
    PaintCanvas *canvas;
    int width;
    int height;
    uint32_t *images;                // three Image2D sprite handles

    explicit LensFlare(PaintCanvas *canvas);
    ~LensFlare();

    void render2D(float srcX, float srcY, float alpha, int colorIndex);
    void update();
};

// Free AEMath operators used below (defined in AEMath.cpp).
namespace AbyssEngine { namespace AEMath {
Vector operator+(const Vector &lhs, const Vector &rhs);
Vector operator*(const Vector &lhs, float rhs);
} }

// playerTargets and the loaded station list are flattened {count,data} engine
// containers (Array<KIPlayer*> / Array<Station*>) with no clean C++ type in this
// TU. LevelScript.cpp reads playerTargets directly through the same ABI, so the
// container helpers below operate on their raw memory.
extern "C" void ArrayRelease_KIPlayer(void *array);
extern "C" void *Array_KIPlayer_dtor(void *array);
extern "C" void Array_KIPlayer_ctor(void *array);
extern "C" void ArraySetLength_KIPlayer(int length, void *array);
extern "C" void Array_Station_release(void *array);
extern "C" void *Array_Station_dtor(void *array);

static inline uint32_t flat_count(void *array) {
    return *(uint32_t *)array;
}
static inline void *flat_data(void *array) {
    return *(void **)((char *)array + 4);
}

// Raw matrix helpers (engine 60-byte transform matrices); declared in AEMath.cpp.
void MatrixGetPosition(char *out, void *matrix);
void MatrixGetUp(char *out, const char *matrix);
void MatrixGetLookAt(char *out, const char *from, const char *to, const char *up);
void MatrixSetScaling(char *matrix, float x, float y, float z);

void StarSystem::switchSunForSupernovaExpansion() {
    AEGeometry *sun = (*this->planetsArray)[0];
    sun->setScaling(11332.0703125f, 11332.0703125f, 11332.0703125f);
}

void StarSystem::renderSunStreak() {
    uint32_t texture = (this->supernovaSystem != 0) ? this->supernovaSunTexture
                                                     : (*this->texturesArray)[0];
    gCanvas->SetTexture(texture, (unsigned)-1);
    gCanvas->SetBlendMode(AbyssEngine::BlendMode_2);
    renderSunStreak_tail(this->sunStreak);
}

// Draws the sun-streak billboard geometry held in sunStreak.
void StarSystem::renderSunStreak_tail(AEGeometry *geom) {
    if (geom != nullptr)
        geom->render();
}

void *StarSystem::getPlanetTargets() {
    return this->playerTargets;
}

void *StarSystem::getPlanets() {
    return this->planetsArray;
}

Vector StarSystem::getLightDirection() {
    return this->lightDirection;
}

StarSystem::~StarSystem() {
    delete this->planetsArray;
    this->planetsArray = nullptr;

    if (this->playerTargets != nullptr) {
        ArrayRelease_KIPlayer(this->playerTargets);
        if (this->playerTargets != nullptr)
            ::operator delete(Array_KIPlayer_dtor(this->playerTargets));
    }
    this->playerTargets = nullptr;

    delete this->lensFlare;
    this->lensFlare = nullptr;

    delete this->texturesArray;
    this->texturesArray = nullptr;

    delete this->stationIdxArray;
    this->stationIdxArray = nullptr;

    delete this->positionsArray;
    this->positionsArray = nullptr;
}

typedef Engine *(*EngineGetter)(ApplicationManager *app);
__attribute__((visibility("hidden"))) extern EngineGetter g_StarSystem_init_getEngine;
__attribute__((visibility("hidden"))) extern float g_StarSystem_init_sunColors[];
__attribute__((visibility("hidden"))) extern float g_StarSystem_init_lightColors[];
__attribute__((visibility("hidden"))) extern float g_StarSystem_init_stationColors[];

static inline uint32_t rgba_scaled(uint32_t color, float scale) {
    uint32_t r = (uint32_t)((float)((color >> 24) & 0xff) * scale);
    uint32_t g = (uint32_t)((float)((color >> 16) & 0xff) * scale);
    uint32_t b = (uint32_t)((float)((color >> 8) & 0xff) * scale);
    return (r << 24) | (g << 16) | (b << 8) | 0xff;
}

void StarSystem::initLight() {
    Engine *engine = (Engine *)gAppManager->GetEngine();
    engine->field_0x32c = 2;

    Status *status = gStatus;
    uint32_t baseIndex;
    bool normalSystem;
    if (this->abstractSystem == 0) {
        SolarSystem *system = (SolarSystem *)(intptr_t)status->getSystem();
        baseIndex = system->getTextureIndex() * 3;
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
        SolarSystem *system = (SolarSystem *)(intptr_t)status->getSystem();
        uint32_t tex = system->getTextureIndex();
        int mission = status->getCurrentCampaignMission();
        if (tex == 0x0f && (mission == 0x59 || mission > 0x9d)) {
            this->sunLightColor.x *= 0.5f;
            this->sunLightColor.y *= 0.5f;
            this->sunLightColor.z *= 0.5f;
        }
        if (this->abstractSystem == 0) {
            system = (SolarSystem *)(intptr_t)status->getSystem();
            tex = system->getTextureIndex();
            ambientScale = 0.5f;
            mission = status->getCurrentCampaignMission();
            if (tex == 0x0f && mission != 0x59) {
                ambientScale = mission < 0x9e ? 0.35f : 0.6f;
            }
        }
    }

    uint32_t stationColorIndex = 0x17;
    if (normalSystem && this->abstractSystem == 0) {
        Station *station = status->getStation();
        stationColorIndex = station->getTextureIndex() * 3;
    }

    float lr = g_StarSystem_init_stationColors[stationColorIndex];
    float lg = g_StarSystem_init_stationColors[stationColorIndex + 1];
    float lb = g_StarSystem_init_stationColors[stationColorIndex + 2];

    EngineGetter getEngine = g_StarSystem_init_getEngine;
    engine = getEngine(gAppManager);
    engine->LightSetGlobalSceneColorAmbient(ambientScale * this->sunLightColor.x,
                                            ambientScale * this->sunLightColor.y,
                                            ambientScale * this->sunLightColor.z);
    engine = getEngine(gAppManager);
    engine->LightSetRimColor(g_StarSystem_init_lightColors[baseIndex] * 3.0f,
                             g_StarSystem_init_lightColors[baseIndex + 1] * 3.0f,
                             g_StarSystem_init_lightColors[baseIndex + 2] * 3.0f);
    getEngine(gAppManager)->LightSetMaterialColorAmbient(1.0f, 1.0f, 1.0f);
    getEngine(gAppManager)->LightSetMaterialColorDiffuse(1.0f, 1.0f, 1.0f);
    getEngine(gAppManager)->LightSetMaterialColorSpecular(1.0f, 1.0f, 1.0f);
    getEngine(gAppManager)->LightSetMaterialColorShininess(0.7f);
    getEngine(gAppManager)->LightSetLightDirection(this->lightDirection.x, this->lightDirection.y,
                                           this->lightDirection.z, 0x4000);
    getEngine(gAppManager)->LightSetLightColorAmbient(0.0f, 0.0f, 0.0f, 0x4000);
    getEngine(gAppManager)->LightSetLightColorDiffuse(this->sunLightColor.x, this->sunLightColor.y,
                                              this->sunLightColor.z, 0x4000);
    getEngine(gAppManager)->LightSetLightColorSpecular(2.0f, 2.0f, 2.0f, 0x4000);
    getEngine(gAppManager)->LightSetLightDirection(0.0f, 0.0f, 1.0f, 0x4001);
    getEngine(gAppManager)->LightSetLightColorDiffuse(lr * 1.5f, lg * 1.5f, lb * 1.5f, 0x4001);
    getEngine(gAppManager)->LightSetLightColorSpecular(lr * 1.5f, lg * 1.5f, lb * 1.5f, 0x4001);
    getEngine(gAppManager)->LightSetParticleAmbient(this->sunLightColor.x, this->sunLightColor.y,
                                            this->sunLightColor.z);

    gCanvas->FogEnable(0, AbyssEngine::FogMode_1);
    this->fogEnabled = 0;
    if (this->abstractSystem != 0) {
        return;
    }

    uint32_t tex = ((SolarSystem *)(intptr_t)status->getSystem())->getTextureIndex();
    uint32_t fogColor = 0;
    switch (tex) {
    case 0x0b:
        fogColor = 0x808080ff;
        break;
    case 0x0c:
        fogColor = 0xa0a0a0ff;
        break;
    case 0x0f:
        if (status->getCurrentCampaignMission() == 0x59) {
            return;
        }
        fogColor = 0x707070ff;
        gCanvas->FogSetParameter(AbyssEngine::FogMode_linear, 0.0f, 0.0f, 1.0f, fogColor);
        gCanvas->FogEnable(1, AbyssEngine::FogMode_1);
        this->fogEnabled = 1;
        for (uint32_t i = 1; i < this->planetsArray->size(); ++i) {
            AEGeometry *geom = (*this->planetsArray)[i];
            void *transform = gCanvas->TransformGetTransform(geom->transform);
            // fog-color field of the opaque engine transform object.
            *(uint32_t *)((char *)transform + 0x48) = fogColor;
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

    gCanvas->FogSetParameter(AbyssEngine::FogMode_linear, 0.0f, 0.0f, 1.0f, fogColor);
    gCanvas->FogEnable(1, AbyssEngine::FogMode_1);
    this->fogEnabled = 1;
    uint32_t scaled = rgba_scaled(fogColor, 0.65f);
    for (uint32_t i = 1; i < this->planetsArray->size(); ++i) {
        AEGeometry *geom = (*this->planetsArray)[i];
        void *transform = gCanvas->TransformGetTransform(geom->transform);
        *(uint32_t *)((char *)transform + 0x48) = scaled;
    }
}

void StarSystem::render2D() {
    if (this->lensFlare == nullptr)
        return;

    unsigned current = gCanvas->CameraGetCurrent();
    char *local = (char *)gCanvas->CameraGetLocal(current);
    Vector pos;
    pos.x = *(float *)(local + 0x0c) + this->lightDirection.x * 65536.0f;
    pos.y = *(float *)(local + 0x1c) + this->lightDirection.y * 65536.0f;
    pos.z = *(float *)(local + 0x2c) + this->lightDirection.z * 65536.0f;
    this->lensFlare->update();
    if (gCanvas->GetScreenPosition(&pos, &pos) != nullptr) {
        this->lensFlare->render2D(pos.x, pos.y, pos.z, this->tintColor);
    }
}

__attribute__((visibility("hidden"))) extern uint16_t g_StarSystem_ctor_planetTextures[];
__attribute__((visibility("hidden"))) extern uint16_t g_StarSystem_ctor_stationTextures[];
__attribute__((visibility("hidden"))) extern uint32_t g_StarSystem_ctor_systemColors[];

StarSystem::StarSystem(int mode) {
    char usedSlots[0x60];
    for (unsigned i = 0; i < sizeof(usedSlots); ++i) {
        usedSlots[i] = 0;
    }

    this->lightDirection.x = 0.0f;
    this->lightDirection.y = 0.0f;
    this->lightDirection.z = 0.0f;
    this->stationIdxArray = nullptr;
    this->texturesArray = nullptr;
    this->playerTargets = nullptr;
    this->planetsArray = nullptr;

    Status *status = gStatus;
    this->abstractSystem = status->getSystem() == 0;
    this->supernovaSystem = (uint8_t)status->inSupernovaSystem();

    this->lensFlare = new LensFlare(gCanvas);
    this->planetRing = nullptr;
    this->planetRingIndex = -1;

    if (this->abstractSystem != 0) {
        this->planetsArray = new Array<AEGeometry *>();
        this->planetsArray->resize(2);

        AEGeometry *sun = new AEGeometry((uint16_t)0x1a70, gCanvas, false);
        (*this->planetsArray)[0] = sun;
        sun->setScaling(Vector{1000.0f, 1000.0f, 1000.0f});
        sun->moveForward(0x447a0000);
        this->lightDirection = sun->getDirection();
        this->lightDirection.x = -this->lightDirection.x;
        this->lightDirection.y = -this->lightDirection.y;
        this->lightDirection.z = -this->lightDirection.z;

        AEGeometry *planet = new AEGeometry((uint16_t)0x1a70, gCanvas, false);
        (*this->planetsArray)[1] = planet;
        int rnd = gRandom->nextInt(0x4e20);
        float scale = (float)(rnd + 0x4e20) * 0.001f;
        planet->setScaling(Vector{scale, scale, scale});
        planet->setRotation(Vector{0.0f, 90.0f, 0.0f});
        planet->moveForward(0x447a0000);

        this->texturesArray = new Array<uint32_t>();
        this->texturesArray->resize(2);
        gCanvas->TextureCreate((uint16_t)0x2739, (*this->texturesArray)[0], false);
        gCanvas->TextureCreate((uint16_t)0x2719, (*this->texturesArray)[1], false);

        this->positionsArray = new Array<Vector>();
        this->positionsArray->resize(2);
        (*this->positionsArray)[0] = sun->getPosition();
        (*this->positionsArray)[1] = planet->getPosition();

        AEGeometry *streak = new AEGeometry((uint16_t)0x1a70, gCanvas, false);
        this->sunStreak = streak;
        streak->setScaling(Vector{250.0f, 15.0f, 1000.0f});
        initLight();
        return;
    }

    SolarSystem *system = (SolarSystem *)(intptr_t)status->getSystem();
    uint32_t *stations = system->getStations();
    FileRead reader;
    void *stationArray = (void *)(intptr_t)reader.loadStationsBinary();

    this->texturesArray = new Array<uint32_t>();
    this->texturesArray->resize(flat_count(stationArray) + 1);

    this->stationIdxArray = new Array<int>();
    this->stationIdxArray->resize(flat_count(stations));

    int mission = status->getCurrentCampaignMission();
    if ((mission == 0x59 && this->supernovaSystem != 0) ||
        (mission > 0x9d &&
         ((SolarSystem *)(intptr_t)status->getSystem())->getIndex() == 0x1b)) {
        gCanvas->TextureCreate((uint16_t)0x2dde, (*this->texturesArray)[0], false);
        this->tintColor = 3;
    } else {
        uint32_t tex = ((SolarSystem *)(intptr_t)status->getSystem())->getTextureIndex();
        gCanvas->TextureCreate(g_StarSystem_ctor_planetTextures[tex],
                                                   (*this->texturesArray)[0], false);
        this->tintColor = g_StarSystem_ctor_systemColors[
            ((SolarSystem *)(intptr_t)status->getSystem())->getIndex()];
    }

    uint32_t count = flat_count(stations);
    int *stationIds = (int *)flat_data(stations);
    Station **loadedStations = (Station **)flat_data(stationArray);
    for (uint32_t i = 1; i < this->texturesArray->size(); ++i) {
        int stationIndex = stationIds[i - 1];
        int currentStation = status->getStation()->getIndex();
        Station *station = loadedStations[i - 1];
        uint32_t stationTex = station->getTextureIndex();
        if (stationIndex == currentStation) {
            if (mode == 3 && status->getCurrentCampaignMission() == 0) {
                this->selectedStationSlot = i;
                gCanvas->TextureCreate((uint16_t)0x273b,
                                                           (*this->texturesArray)[i], false);
            } else {
                gCanvas->TextureCreate(g_StarSystem_ctor_planetTextures[stationTex],
                                                           (*this->texturesArray)[i], false);
            }
        } else {
            gCanvas->TextureCreate(g_StarSystem_ctor_stationTextures[stationTex],
                                                       (*this->texturesArray)[i], false);
            if (gStatus->orbitHasPlanetRing(station->getIndex()) != 0) {
                this->planetRing = new AEGeometry((uint16_t)0x1a70, gCanvas, false);
                gCanvas->TextureCreate((uint16_t)0x7198,
                                                           this->planetRingTexture, false);
                this->planetRingIndex = i;
            }
        }
        (*this->stationIdxArray)[i - 1] = station->getIndex();
    }

    Array_Station_release(stationArray);
    ::operator delete(Array_Station_dtor(stationArray));

    this->playerTargets = ::operator new(0x0c);
    Array_KIPlayer_ctor(this->playerTargets);
    ArraySetLength_KIPlayer(count, this->playerTargets);

    this->planetsArray = new Array<AEGeometry *>();
    this->planetsArray->resize(count + 1);

    this->positionsArray = new Array<Vector>();
    this->positionsArray->resize(count + 1);

    gRandom->setSeed((long long)status->getStation()->getIndex() * 300);

    int sunSlot = gRandom->nextInt(14);
    PlayerStatic **targets = (PlayerStatic **)flat_data(this->playerTargets);
    for (uint32_t i = 0; i < this->planetsArray->size(); ++i) {
        AEGeometry *geom = new AEGeometry((uint16_t)0x1a70, gCanvas, false);
        (*this->planetsArray)[i] = geom;

        if (i == 0) {
            float sunScale = this->supernovaSystem == 0 ? 1000.0f : 2000.0f;
            geom->setScaling(Vector{sunScale, sunScale, sunScale});
            AEGeometry *streak = new AEGeometry((uint16_t)0x1a70, gCanvas, false);
            this->sunStreak = streak;
            streak->setScaling(Vector{this->supernovaSystem == 0 ? 250.0f : 500.0f,
                                      this->supernovaSystem == 0 ? 15.0f : 25.0f,
                                      this->supernovaSystem == 0 ? 1000.0f : 2000.0f});
            if (this->supernovaSystem != 0) {
                gCanvas->TextureCreate((uint16_t)0x2dde,
                                                           this->supernovaSunTexture, false);
            }
            usedSlots[sunSlot * 4] = 1;
        } else {
            PlayerStatic *player = new PlayerStatic(0, geom, 0.0f, 0.0f, 0.0f);
            targets[i - 1] = player;
            int slot = gRandom->nextInt(11) + 7;
            usedSlots[slot * 4] = 1;
            int dist = gRandom->nextInt(0x4e20) + 0x4e20;
            if (status->getCurrentCampaignMission() == 0) {
                dist = (int)((float)dist * 0.5f);
            }
            float scale = (float)dist * 0.001f;
            this->planetScale = scale;
            geom->setScaling(Vector{scale, scale, scale});
        }

        int angleSlot = (i == 0) ? sunSlot : (int)i + 6;
        geom->setRotation(Vector{0.0f, (float)(angleSlot * 0xaaa) * 0.001f, 0.0f});
        geom->moveForward(0x447a0000);
        if (i == 0) {
            this->lightDirection = geom->getDirection();
            this->lightDirection.x = -this->lightDirection.x;
            this->lightDirection.y = -this->lightDirection.y;
            this->lightDirection.z = -this->lightDirection.z;
        }
        (*this->positionsArray)[i] = geom->getPosition();
    }

    gRandom->reset();
    initLight();
}

void StarSystem::updateSupernova(int dt) {
    AEGeometry *streak = this->sunStreak;
    if (streak != nullptr) {
        AbyssEngine::Transform *transform = (AbyssEngine::Transform *)gCanvas
                                   ->TransformGetTransform(streak->transform);
        transform->Update(0, dt);
    }
    AEGeometry *sun = (*this->planetsArray)[0];
    if (sun != nullptr) {
        AbyssEngine::Transform *transform = (AbyssEngine::Transform *)gCanvas
                                   ->TransformGetTransform(sun->transform);
        transform->Update(1, dt);
    }
}

void StarSystem::scaleSunDuringSupernovaIntro(int amount) {
    AEGeometry *sun = (*this->planetsArray)[0];
    float scale = sun->getScaling().x + (float)amount * -9.769497830779909e32f;
    sun->setScaling(scale, scale, scale);
}

void StarSystem::switchPlanetForIntro() {
    gCanvas->TextureCreate((uint16_t)0x273a,
        (*this->texturesArray)[this->selectedStationSlot], false);
    AEGeometry *planet = (*this->planetsArray)[this->selectedStationSlot];
    planet->setScaling(planet->getScaling() * 2.0f);
}

void StarSystem::switchSunForSupernovaReversal() {
    gCanvas->TextureCreate((uint16_t)0x2734,
                                                   (*this->texturesArray)[0], false);
    AEGeometry *sun = (*this->planetsArray)[0];
    sun->setScaling(2.429073312463973e24f, 2.429073312463973e24f, 2.429073312463973e24f);
}

typedef void *(*GetTransformFn)(PaintCanvas *canvas, int transform_id);
typedef void (*SetTransformModeFn)(void *transform, int mode, int value);

__attribute__((visibility("hidden"))) extern GetTransformFn g_StarSystem_intro_getTransform;
__attribute__((visibility("hidden"))) extern SetTransformModeFn g_StarSystem_intro_setTransformMode;
__attribute__((visibility("hidden"))) extern uint32_t g_StarSystem_intro_colors[];

void StarSystem::switchSunForSupernovaIntro() {
    gCanvas->TextureCreate((uint16_t)0x2df3, (*this->texturesArray)[0], false);
    gCanvas->TextureCreate((uint16_t)0x2df4, this->supernovaSunTexture, false);

    AEGeometry *streak = this->sunStreak;
    streak->setMesh(0x2df2);
    streak->setScaling(streak->getScaling() * 5.0f);

    AEGeometry *sun = (*this->planetsArray)[0];
    sun->setMesh(0x2df1);
    sun->setScaling(0.6866455078125f);

    GetTransformFn getTransform = g_StarSystem_intro_getTransform;
    SetTransformModeFn setTransformMode = g_StarSystem_intro_setTransformMode;
    setTransformMode(getTransform(gCanvas, sun->transform), 0, 0);
    setTransformMode(getTransform(gCanvas, sun->transform), 3, 0);
    setTransformMode(getTransform(gCanvas, sun->transform), 1, 0);

    SolarSystem *system = (SolarSystem *)(intptr_t)gStatus->getSystem();
    this->tintColor = g_StarSystem_intro_colors[system->getIndex()];
}

__attribute__((visibility("hidden"))) extern uint32_t *g_StarSystem_render_station_index;

void StarSystem::render() {
    char cameraPos[12];
    char savedCamera[0x3c];
    char lookAt[0x3c];
    char tempMatrix[0x3c];
    char tempVec[12];
    char up[12];
    char scaleBytes[12];

    PaintCanvas *canvas = gCanvas;
    unsigned current = canvas->CameraGetCurrent();
    void *local = canvas->CameraGetLocal(current);
    MatrixGetPosition(cameraPos, local);

    uint32_t count = this->planetsArray->size();
    AEGeometry **geoms = this->planetsArray->data();
    for (uint32_t i = 0; i < count; ++i) {
        if (count > 1) {
            if (i == 0) {
                unsigned current2 = canvas->CameraGetCurrent();
                void *local2 = canvas->CameraGetLocal(current2);
                memcpy(savedCamera, local2, 0x3c);
                *(Vector *)tempVec = (*this->positionsArray)[0] + *(const Vector *)cameraPos;
                if (this->supernovaSystem == 0) {
                    MatrixGetUp(up, savedCamera);
                } else {
                    ((float *)up)[0] = 1.0f;
                    ((float *)up)[1] = 0.0f;
                    ((float *)up)[2] = 0.0f;
                }
                MatrixGetLookAt(lookAt, tempVec, cameraPos, up);
                memcpy(savedCamera, lookAt, 0x3c);
                geoms[0]->setRotation(0.0f, 0.0f, 0.0f);
                *(Vector *)tempVec = geoms[0]->getScaling();

                float grow = (this->lensFlare->intensity - 10.0f) * 0.001f;
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
                geoms[0]->setMatrix(*(const Matrix *)lookAt);

                if (this->supernovaSystem != 0) {
                    *(Vector *)tempVec *= grow;
                    unsigned current3 = canvas->CameraGetCurrent();
                    void *local3 = canvas->CameraGetLocal(current3);
                    memcpy(savedCamera, local3, 0x3c);
                    *(Vector *)up = (*this->positionsArray)[0] + *(const Vector *)cameraPos;
                    MatrixGetUp(scaleBytes, savedCamera);
                    MatrixGetLookAt(lookAt, up, cameraPos, scaleBytes);
                    memcpy(savedCamera, lookAt, 0x3c);
                    MatrixSetScaling(lookAt, grow + ((float *)tempVec)[0],
                                     ((float *)tempVec)[1],
                                     grow + ((float *)tempVec)[2]);
                    this->sunStreak->setMatrix(*(const Matrix *)lookAt);
                    renderSunStreak();
                }

                MatrixSetScaling(lookAt,
                                 grow * (grow + ((float *)tempVec)[0] + 1.0f),
                                 ((float *)tempVec)[1],
                                 ((float *)tempVec)[1] / ((1.0f - grow) * 6.0f + 6.0f));
                this->sunStreak->setMatrix(*(const Matrix *)lookAt);
            }

            uint32_t stationIndex = *g_StarSystem_render_station_index;
            bool selected = (i - 1) == stationIndex;
            if (selected && this->abstractSystem == 0 &&
                gStatus->inPlanetRingOrbit() == 0) {
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

            *(Vector *)tempMatrix = (*this->positionsArray)[i] + *(const Vector *)cameraPos;
            geoms[i]->setPosition(*(const Vector *)tempMatrix);
        }

        canvas->SetTexture((*this->texturesArray)[i], (unsigned)-1);
        AbyssEngine::BlendMode blend;
        if (i == 0) {
            blend = AbyssEngine::BlendMode_2;
        } else {
            blend = this->fogEnabled == 0 ? AbyssEngine::BlendMode_1
                                          : AbyssEngine::BlendMode_0x15;
        }
        canvas->SetBlendMode(blend);
        geoms[i]->render();

        if ((int)i == this->planetRingIndex) {
            canvas->SetTexture(this->planetRingTexture, (unsigned)-1);
            canvas->SetBlendMode(AbyssEngine::BlendMode_1);
            memcpy(savedCamera, &geoms[i]->getMatrix(), 0x3c);
            MatrixSetScaling(lookAt, 4.0f, 4.0f, 4.0f);
            this->planetRing->setMatrix(*(const Matrix *)lookAt);
            this->planetRing->render();
        }
    }
}
