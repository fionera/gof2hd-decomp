#include "game/world/StarSystem.h"
#include "game/core/Globals.h"
#include "engine/math/Transform.h"
#include "engine/render/PaintCanvas.h"
#include "engine/core/AERandom.h"
#include "game/world/SolarSystem.h"
#include "game/world/Station.h"
#include "engine/render/AEGeometry.h"
#include "engine/file/FileRead.h"
#include "game/mission/Status.h"
#include "engine/core/ApplicationManager.h"
#include "engine/render/Engine.h"
#include "game/ship/PlayerStatic.h"
#include "engine/render/LensFlare.h"

int StarSystem::orbitPlanetIndex;

namespace AbyssEngine {
    namespace AEMath {
        Vector operator+(const Vector &lhs, const Vector &rhs);

        Vector operator*(const Vector &lhs, float rhs);
    }
}

void ArrayRelease_KIPlayer(void *array); // lint: void_ptr (external symbol; mangling must match lib)

void *Array_KIPlayer_dtor(void *array); // lint: void_ptr (external symbol; mangling must match lib)

void Array_KIPlayer_ctor(void *array); // lint: void_ptr (external symbol; mangling must match lib)

void Array_Station_release(void *array); // lint: void_ptr (external symbol; mangling must match lib)

void *Array_Station_dtor(void *array); // lint: void_ptr (external symbol; mangling must match lib)

void MatrixGetPosition(Vector *out, void *matrix); // lint: void_ptr (external symbol; mangling must match lib)

static inline void MatrixGetUp(Vector *out, const Matrix *matrix) { *out = AbyssEngine::AEMath::MatrixGetUp(*matrix); }

static inline void MatrixGetLookAt(Matrix *out, const Vector *from, const Vector *to, const Vector *up) { *out = AbyssEngine::AEMath::MatrixGetLookAt(*from, *to, *up); }

void MatrixSetScaling(Matrix *matrix, float x, float y, float z);

void StarSystem::switchSunForSupernovaExpansion() {
    AEGeometry *sun = (*this->planetsArray)[0];
    sun->setScaling(11332.0703125f, 11332.0703125f, 11332.0703125f);
}

void StarSystem::renderSunStreak() {
    uint32_t texture = (this->supernovaSystem != 0)
                           ? this->supernovaSunTexture
                           : (*this->texturesArray)[0];
    Globals::Canvas->SetTexture(texture, (unsigned) -1);
    Globals::Canvas->SetBlendMode(AbyssEngine::BlendMode_2);
    this->sunStreak->render();
}

void *StarSystem::getPlanetTargets() {
    // lint: void_ptr (exported signature; mangling must match lib)
    return this->playerTargets;
}

void *StarSystem::getPlanets() {
    // lint: void_ptr (exported signature; mangling must match lib)
    return this->planetsArray;
}

Array<int> *StarSystem::getStationIndices() {
    return this->stationIdxArray;
}

float StarSystem::getPlanetScaleFactor() {
    return this->planetRingScaleOffset;
}

void StarSystem::rotate(int x, int y, int z) {
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

static float g_StarSystem_init_sunColors[256] = {0};

static float g_StarSystem_init_lightColors[256] = {0};

static float g_StarSystem_init_stationColors[256] = {0};

static inline uint32_t rgba_scaled(uint32_t color, float scale) {
    uint32_t r = (uint32_t)((float) ((color >> 24) & 0xff) * scale);
    uint32_t g = (uint32_t)((float) ((color >> 16) & 0xff) * scale);
    uint32_t b = (uint32_t)((float) ((color >> 8) & 0xff) * scale);
    return (r << 24) | (g << 16) | (b << 8) | 0xff;
}

void StarSystem::initLight() {
    Engine *engine = (Engine *) Globals::appManager->GetEngine();
    engine->lightCount = 2;

    Status *status = Globals::status;
    uint32_t baseIndex;
    bool normalSystem;
    if (this->abstractSystem == 0) {
        SolarSystem *system = (SolarSystem *) (intptr_t) status->getSystem();
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
        SolarSystem *system = (SolarSystem *) (intptr_t) status->getSystem();
        uint32_t tex = system->getTextureIndex();
        int mission = status->getCurrentCampaignMission();
        if (tex == 0x0f && (mission == 0x59 || mission > 0x9d)) {
            this->sunLightColor.x *= 0.5f;
            this->sunLightColor.y *= 0.5f;
            this->sunLightColor.z *= 0.5f;
        }
        if (this->abstractSystem == 0) {
            system = (SolarSystem *) (intptr_t) status->getSystem();
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
    engine = ((Engine *) Globals::appManager->GetEngine());
    engine->LightSetGlobalSceneColorAmbient(ambientScale * this->sunLightColor.x,
                                            ambientScale * this->sunLightColor.y,
                                            ambientScale * this->sunLightColor.z);
    engine = ((Engine *) Globals::appManager->GetEngine());
    engine->LightSetRimColor(g_StarSystem_init_lightColors[baseIndex] * 3.0f,
                             g_StarSystem_init_lightColors[baseIndex + 1] * 3.0f,
                             g_StarSystem_init_lightColors[baseIndex + 2] * 3.0f);
    ((Engine *) Globals::appManager->GetEngine())->LightSetMaterialColorAmbient(1.0f, 1.0f, 1.0f);
    ((Engine *) Globals::appManager->GetEngine())->LightSetMaterialColorDiffuse(1.0f, 1.0f, 1.0f);
    ((Engine *) Globals::appManager->GetEngine())->LightSetMaterialColorSpecular(1.0f, 1.0f, 1.0f);
    ((Engine *) Globals::appManager->GetEngine())->LightSetMaterialColorShininess(0.7f);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightDirection(this->lightDirection.x, this->lightDirection.y,
                                                           this->lightDirection.z, 0x4000);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorAmbient(0.0f, 0.0f, 0.0f, 0x4000);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorDiffuse(this->sunLightColor.x, this->sunLightColor.y,
                                                              this->sunLightColor.z, 0x4000);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorSpecular(2.0f, 2.0f, 2.0f, 0x4000);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightDirection(0.0f, 0.0f, 1.0f, 0x4001);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorDiffuse(lr * 1.5f, lg * 1.5f, lb * 1.5f, 0x4001);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorSpecular(lr * 1.5f, lg * 1.5f, lb * 1.5f, 0x4001);
    ((Engine *) Globals::appManager->GetEngine())->LightSetParticleAmbient(this->sunLightColor.x, this->sunLightColor.y,
                                                            this->sunLightColor.z);

    Globals::Canvas->FogEnable(0, AbyssEngine::FogMode_1);
    this->fogEnabled = 0;
    if (this->abstractSystem != 0) {
        return;
    }

    uint32_t tex = ((SolarSystem *) (intptr_t) status->getSystem())->getTextureIndex();
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
            Globals::Canvas->FogSetParameter(AbyssEngine::FogMode_linear, 0.0f, 0.0f, 1.0f, fogColor);
            Globals::Canvas->FogEnable(1, AbyssEngine::FogMode_1);
            this->fogEnabled = 1;
            for (uint32_t i = 1; i < this->planetsArray->size(); ++i) {
                AEGeometry *geom = (*this->planetsArray)[i];
                AbyssEngine::Transform *transform =
                        (AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(geom->transform);

                transform->id = (int) fogColor;
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

    Globals::Canvas->FogSetParameter(AbyssEngine::FogMode_linear, 0.0f, 0.0f, 1.0f, fogColor);
    Globals::Canvas->FogEnable(1, AbyssEngine::FogMode_1);
    this->fogEnabled = 1;
    uint32_t scaled = rgba_scaled(fogColor, 0.65f);
    for (uint32_t i = 1; i < this->planetsArray->size(); ++i) {
        AEGeometry *geom = (*this->planetsArray)[i];
        AbyssEngine::Transform *transform =
                (AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(geom->transform);
        transform->id = (int) scaled;
    }
}

void StarSystem::render2D() {
    if (this->lensFlare == nullptr)
        return;

    unsigned current = Globals::Canvas->CameraGetCurrent();
    Matrix *local = (Matrix *) Globals::Canvas->CameraGetLocal(current);
    Vector pos;
    pos.x = local->m[3] + this->lightDirection.x * 65536.0f;
    pos.y = local->m[7] + this->lightDirection.y * 65536.0f;
    pos.z = local->m[11] + this->lightDirection.z * 65536.0f;
    this->lensFlare->update(0);
    Globals::Canvas->GetScreenPosition(pos, pos);
    this->lensFlare->render2D(pos.x, pos.y, pos.z, this->tintColor);
}

static uint16_t g_StarSystem_ctor_planetTextures[256] = {0};

static uint16_t g_StarSystem_ctor_stationTextures[256] = {0};

static uint32_t g_StarSystem_ctor_systemColors[256] = {0};

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

    Status *status = Globals::status;
    this->abstractSystem = status->getSystem() == 0;
    this->supernovaSystem = (uint8_t) status->inSupernovaSystem();

    this->lensFlare = new LensFlare(Globals::Canvas);
    this->planetRing = nullptr;
    this->planetRingIndex = -1;

    if (this->abstractSystem != 0) {
        this->planetsArray = new Array<AEGeometry *>();
        ArraySetLength(2, *(this->planetsArray));

        AEGeometry *sun = new AEGeometry((uint16_t) 0x1a70, Globals::Canvas, false);
        (*this->planetsArray)[0] = sun;
        sun->setScaling(Vector{1000.0f, 1000.0f, 1000.0f});
        sun->moveForward(0x447a0000);
        this->lightDirection = sun->getDirection();
        this->lightDirection.x = -this->lightDirection.x;
        this->lightDirection.y = -this->lightDirection.y;
        this->lightDirection.z = -this->lightDirection.z;

        AEGeometry *planet = new AEGeometry((uint16_t) 0x1a70, Globals::Canvas, false);
        (*this->planetsArray)[1] = planet;
        int rnd = Globals::rnd->nextInt(0x4e20);
        float scale = (float) (rnd + 0x4e20) * 0.001f;
        planet->setScaling(Vector{scale, scale, scale});
        planet->setRotation(Vector{0.0f, 90.0f, 0.0f});
        planet->moveForward(0x447a0000);

        this->texturesArray = new Array<uint32_t>();
        ArraySetLength(2, *(this->texturesArray));
        Globals::Canvas->TextureCreate((uint16_t) 0x2739, (*this->texturesArray)[0], false);
        Globals::Canvas->TextureCreate((uint16_t) 0x2719, (*this->texturesArray)[1], false);

        this->positionsArray = new Array<Vector>();
        ArraySetLength(2, *(this->positionsArray));
        (*this->positionsArray)[0] = sun->getPosition();
        (*this->positionsArray)[1] = planet->getPosition();

        AEGeometry *streak = new AEGeometry((uint16_t) 0x1a70, Globals::Canvas, false);
        this->sunStreak = streak;
        streak->setScaling(Vector{250.0f, 15.0f, 1000.0f});
        initLight();
        return;
    }

    SolarSystem *system = (SolarSystem *) (intptr_t) status->getSystem();
    uint32_t *stations = system->getStations();
    FileRead reader;
    Array<Station *> *stationArray = (Array<Station *> *) (intptr_t) reader.loadStationsBinary();

    this->texturesArray = new Array<uint32_t>();
    ArraySetLength(stationArray->size() + 1, *(this->texturesArray));

    this->stationIdxArray = new Array<int>();
    ArraySetLength(reinterpret_cast<Array<int> *>(stations)->size(), *(this->stationIdxArray));

    int mission = status->getCurrentCampaignMission();
    if ((mission == 0x59 && this->supernovaSystem != 0) ||
        (mission > 0x9d &&
         ((SolarSystem *) (intptr_t) status->getSystem())->getIndex() == 0x1b)) {
        Globals::Canvas->TextureCreate((uint16_t) 0x2dde, (*this->texturesArray)[0], false);
        this->tintColor = 3;
    } else {
        uint32_t tex = ((SolarSystem *) (intptr_t) status->getSystem())->getTextureIndex();
        Globals::Canvas->TextureCreate(g_StarSystem_ctor_planetTextures[tex],
                                       (*this->texturesArray)[0], false);
        this->tintColor = g_StarSystem_ctor_systemColors[
            ((SolarSystem *) (intptr_t) status->getSystem())->getIndex()];
    }

    uint32_t count = reinterpret_cast<Array<int> *>(stations)->size();
    int *stationIds = reinterpret_cast<Array<int> *>(stations)->data();
    Station **loadedStations = stationArray->data();
    for (uint32_t i = 1; i < this->texturesArray->size(); ++i) {
        int stationIndex = stationIds[i - 1];
        int currentStation = status->getStation()->getIndex();
        Station *station = loadedStations[i - 1];
        uint32_t stationTex = station->getTextureIndex();
        if (stationIndex == currentStation) {
            if (mode == 3 && status->getCurrentCampaignMission() == 0) {
                this->selectedStationSlot = i;
                Globals::Canvas->TextureCreate((uint16_t) 0x273b,
                                               (*this->texturesArray)[i], false);
            } else {
                Globals::Canvas->TextureCreate(g_StarSystem_ctor_planetTextures[stationTex],
                                               (*this->texturesArray)[i], false);
            }
        } else {
            Globals::Canvas->TextureCreate(g_StarSystem_ctor_stationTextures[stationTex],
                                           (*this->texturesArray)[i], false);
            if (Globals::status->orbitHasPlanetRing(station->getIndex()) != 0) {
                this->planetRing = new AEGeometry((uint16_t) 0x1a70, Globals::Canvas, false);
                Globals::Canvas->TextureCreate((uint16_t) 0x7198,
                                               this->planetRingTexture, false);
                this->planetRingIndex = i;
            }
        }
        (*this->stationIdxArray)[i - 1] = station->getIndex();
    }

    Array_Station_release(stationArray);
    ::operator delete(Array_Station_dtor(stationArray));

    this->playerTargets = (Array<KIPlayer *> *) ::operator new(0x0c);
    Array_KIPlayer_ctor(this->playerTargets);

    ArraySetLength<unsigned int>(static_cast<unsigned int>(count),
                                 *reinterpret_cast<Array<unsigned int> *>(this->playerTargets));

    this->planetsArray = new Array<AEGeometry *>();
    ArraySetLength(count + 1, *(this->planetsArray));

    this->positionsArray = new Array<Vector>();
    ArraySetLength(count + 1, *(this->positionsArray));

    Globals::rnd->setSeed((long long) status->getStation()->getIndex() * 300);

    int sunSlot = Globals::rnd->nextInt(14);
    PlayerStatic **targets = (PlayerStatic **) this->playerTargets->data();
    for (uint32_t i = 0; i < this->planetsArray->size(); ++i) {
        AEGeometry *geom = new AEGeometry((uint16_t) 0x1a70, Globals::Canvas, false);
        (*this->planetsArray)[i] = geom;

        if (i == 0) {
            float sunScale = this->supernovaSystem == 0 ? 1000.0f : 2000.0f;
            geom->setScaling(Vector{sunScale, sunScale, sunScale});
            AEGeometry *streak = new AEGeometry((uint16_t) 0x1a70, Globals::Canvas, false);
            this->sunStreak = streak;
            streak->setScaling(Vector{
                this->supernovaSystem == 0 ? 250.0f : 500.0f,
                this->supernovaSystem == 0 ? 15.0f : 25.0f,
                this->supernovaSystem == 0 ? 1000.0f : 2000.0f
            });
            if (this->supernovaSystem != 0) {
                Globals::Canvas->TextureCreate((uint16_t) 0x2dde,
                                               this->supernovaSunTexture, false);
            }
            usedSlots[sunSlot * 4] = 1;
        } else {
            PlayerStatic *player = new PlayerStatic(0, geom, 0.0f, 0.0f, 0.0f);
            targets[i - 1] = player;
            int slot = Globals::rnd->nextInt(11) + 7;
            usedSlots[slot * 4] = 1;
            int dist = Globals::rnd->nextInt(0x4e20) + 0x4e20;
            if (status->getCurrentCampaignMission() == 0) {
                dist = (int) ((float) dist * 0.5f);
            }
            float scale = (float) dist * 0.001f;
            this->planetScale = scale;
            geom->setScaling(Vector{scale, scale, scale});
        }

        int angleSlot = (i == 0) ? sunSlot : (int) i + 6;
        geom->setRotation(Vector{0.0f, (float) (angleSlot * 0xaaa) * 0.001f, 0.0f});
        geom->moveForward(0x447a0000);
        if (i == 0) {
            this->lightDirection = geom->getDirection();
            this->lightDirection.x = -this->lightDirection.x;
            this->lightDirection.y = -this->lightDirection.y;
            this->lightDirection.z = -this->lightDirection.z;
        }
        (*this->positionsArray)[i] = geom->getPosition();
    }

    Globals::rnd->reset();
    initLight();
}

void StarSystem::updateSupernova(int dt) {
    AEGeometry *streak = this->sunStreak;
    if (streak != nullptr) {
        AbyssEngine::Transform *transform = (AbyssEngine::Transform *) Globals::Canvas
                ->TransformGetTransform(streak->transform);
        transform->Update(0, dt);
    }
    AEGeometry *sun = (*this->planetsArray)[0];
    if (sun != nullptr) {
        AbyssEngine::Transform *transform = (AbyssEngine::Transform *) Globals::Canvas
                ->TransformGetTransform(sun->transform);
        transform->Update(1, dt);
    }
}

void StarSystem::scaleSunDuringSupernovaIntro(int amount) {
    AEGeometry *sun = (*this->planetsArray)[0];
    float scale = sun->getScaling().x + (float) amount * -9.769497830779909e32f;
    sun->setScaling(scale, scale, scale);
}

void StarSystem::switchPlanetForIntro() {
    Globals::Canvas->TextureCreate((uint16_t) 0x273a,
                                   (*this->texturesArray)[this->selectedStationSlot], false);
    AEGeometry *planet = (*this->planetsArray)[this->selectedStationSlot];
    planet->setScaling(planet->getScaling() * 2.0f);
}

void StarSystem::switchSunForSupernovaReversal() {
    Globals::Canvas->TextureCreate((uint16_t) 0x2734,
                                   (*this->texturesArray)[0], false);
    AEGeometry *sun = (*this->planetsArray)[0];
    sun->setScaling(2.429073312463973e24f, 2.429073312463973e24f, 2.429073312463973e24f);
}

static uint32_t g_StarSystem_intro_colors[256] = {0};

void StarSystem::switchSunForSupernovaIntro() {
    Globals::Canvas->TextureCreate((uint16_t) 0x2df3, (*this->texturesArray)[0], false);
    Globals::Canvas->TextureCreate((uint16_t) 0x2df4, this->supernovaSunTexture, false);

    AEGeometry *streak = this->sunStreak;
    streak->setMesh(0x2df2);
    streak->setScaling(streak->getScaling() * 5.0f);

    AEGeometry *sun = (*this->planetsArray)[0];
    sun->setMesh(0x2df1);
    sun->setScaling(0.6866455078125f);
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(sun->transform))->SetAnimationState((AbyssEngine::AnimationMode) 0, nullptr);
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(sun->transform))->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(sun->transform))->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);

    SolarSystem *system = (SolarSystem *) (intptr_t) Globals::status->getSystem();
    this->tintColor = g_StarSystem_intro_colors[system->getIndex()];
}

static uint32_t g_StarSystem_render_station_index_storage = 0;
static uint32_t *g_StarSystem_render_station_index =
        &g_StarSystem_render_station_index_storage;

void StarSystem::render() {
    Vector cameraPos;
    Matrix savedCamera;
    Matrix lookAt;
    Vector tempMatrix;
    Vector tempVec;
    Vector up;
    Vector scaleBytes;

    PaintCanvas *canvas = Globals::Canvas;
    unsigned current = canvas->CameraGetCurrent();
    Matrix *local = (Matrix *) canvas->CameraGetLocal(current);
    MatrixGetPosition(&cameraPos, local);

    uint32_t count = this->planetsArray->size();
    AEGeometry **geoms = this->planetsArray->data();
    for (uint32_t i = 0; i < count; ++i) {
        if (count > 1) {
            if (i == 0) {
                unsigned current2 = canvas->CameraGetCurrent();
                Matrix *local2 = (Matrix *) canvas->CameraGetLocal(current2);
                savedCamera = *local2;
                tempVec = (*this->positionsArray)[0] + cameraPos;
                if (this->supernovaSystem == 0) {
                    MatrixGetUp(&up, &savedCamera);
                } else {
                    up.x = 1.0f;
                    up.y = 0.0f;
                    up.z = 0.0f;
                }
                MatrixGetLookAt(&lookAt, &tempVec, &cameraPos, &up);
                savedCamera = lookAt;
                geoms[0]->setRotation(0.0f, 0.0f, 0.0f);
                tempVec = geoms[0]->getScaling();

                float grow = (this->lensFlare->intensity - 10.0f) * 0.001f;
                if (grow < 0.0f) {
                    grow = 0.0f;
                }
                float sx = tempVec.x;
                float sy = tempVec.y;
                float sz = tempVec.z;
                if (this->supernovaSystem == 0) {
                    sx += grow;
                    sy += grow;
                }
                MatrixSetScaling(&lookAt, sx, sy, sz);
                geoms[0]->setMatrix(lookAt);

                if (this->supernovaSystem != 0) {
                    tempVec *= grow;
                    unsigned current3 = canvas->CameraGetCurrent();
                    Matrix *local3 = (Matrix *) canvas->CameraGetLocal(current3);
                    savedCamera = *local3;
                    up = (*this->positionsArray)[0] + cameraPos;
                    MatrixGetUp(&scaleBytes, &savedCamera);
                    MatrixGetLookAt(&lookAt, &up, &cameraPos, &scaleBytes);
                    savedCamera = lookAt;
                    MatrixSetScaling(&lookAt, grow + tempVec.x,
                                     tempVec.y,
                                     grow + tempVec.z);
                    this->sunStreak->setMatrix(lookAt);
                    renderSunStreak();
                }

                MatrixSetScaling(&lookAt,
                                 grow * (grow + tempVec.x + 1.0f),
                                 tempVec.y,
                                 tempVec.y / ((1.0f - grow) * 6.0f + 6.0f));
                this->sunStreak->setMatrix(lookAt);
            }

            uint32_t stationIndex = *g_StarSystem_render_station_index;
            bool selected = (i - 1) == stationIndex;
            if (selected && this->abstractSystem == 0 &&
                Globals::status->inPlanetRingOrbit() == 0) {
                float ring = cameraPos.z / 65536.0f;
                float clamped = ring < 0.0f ? ring : 0.0f;
                if (clamped < -1.0f) {
                    clamped = -1.0f;
                }
                this->planetRingScaleOffset = clamped;
                geoms[i]->setScaling(this->planetScale + clamped,
                                     this->planetScale + clamped,
                                     this->planetScale + clamped);
            }

            tempMatrix = (*this->positionsArray)[i] + cameraPos;
            geoms[i]->setPosition(tempMatrix);
        }

        canvas->SetTexture((*this->texturesArray)[i], (unsigned) -1);
        AbyssEngine::BlendMode blend;
        if (i == 0) {
            blend = AbyssEngine::BlendMode_2;
        } else {
            blend = this->fogEnabled == 0
                        ? AbyssEngine::BlendMode_1
                        : AbyssEngine::BlendMode_0x15;
        }
        canvas->SetBlendMode(blend);
        geoms[i]->render();

        if ((int) i == this->planetRingIndex) {
            canvas->SetTexture(this->planetRingTexture, (unsigned) -1);
            canvas->SetBlendMode(AbyssEngine::BlendMode_1);
            savedCamera = geoms[i]->getMatrix();
            MatrixSetScaling(&lookAt, 4.0f, 4.0f, 4.0f);
            this->planetRing->setMatrix(lookAt);
            this->planetRing->render();
        }
    }
}
