#include "game/world/StarSystem.h"
#include "engine/core/ArrayReleaseClasses.h"
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

static inline void MatrixGetUp(Vector *out, const Matrix *matrix) { *out = AbyssEngine::AEMath::MatrixGetUp(*matrix); }

static inline void MatrixGetLookAt(Matrix *out, const Vector *from, const Vector *to, const Vector *up) { *out = AbyssEngine::AEMath::MatrixGetLookAt(*from, *to, *up); }

void MatrixSetScaling(Matrix *matrix, float x, float y, float z);


// Per-system / per-station render tables (.rodata, original TU order).
static const int kSunTextures[19] = {
    0x272f, 0x2730, 0x2731, 0x2732, 0x2733, 0x2734, 0x2735, 0x2736, 0x2737, 0x2738, 0x2739, 0x2730, 0x2737, 0x2733, 0x272f, 0x2ddd, 0x2736, 0x2731, 0x2735,
};

static const int kSystemTints[34] = {
    3, 1, 3, 2, 0, 3, 1, 4, 2, 4, 1, 3, 1, 3, 2, 3, 3, 0, 2, 1, 2, 3, 2, 0, 3, 1, 2, 5, 3, 3, 3, 3, 3, 3,
};

static const int kCurrentStationTextures[27] = {
    0x273a, 0x273b, 0x273c, 0x273d, 0x273e, 0x273f, 0x2740, 0x2741, 0x2742, 0x2743, 0x2744, 0x2745, 0x2746, 0x2747, 0x2748, 0x2749, 0x274a, 0x274b, 0x274c, 0x274d, 0x2d68, 0x2d69, 0x2d6a, 0x2719, 0x2e14, 0x2e15, 0x2e16,
};

static const int kStationTextures[27] = {
    0x271b, 0x271c, 0x271d, 0x271e, 0x271f, 0x2720, 0x2721, 0x2722, 0x2723, 0x2724, 0x2725, 0x2726, 0x2727, 0x2728, 0x2729, 0x272a, 0x272b, 0x272c, 0x272d, 0x272e, 0x2d65, 0x2d66, 0x2d67, 0x2d67, 0x2e08, 0x2e09, 0x2e0a,
};

static const float kSunLightColors[57] = {
    0.21f, 0.32f, 0.25f,
    0.5f, 0.29f, 0.28f,
    0.24f, 0.4f, 0.37f,
    0.22f, 0.32f, 0.43f,
    0.18f, 0.23f, 0.27f,
    0.38f, 0.13f, 0.11f,
    0.34f, 0.34f, 0.3f,
    0.24f, 0.26f, 0.25f,
    0.21f, 0.23f, 0.27f,
    0.43f, 0.47f, 0.47f,
    0.43f, 0.21f, 0.34f,
    0.85f, 0.41f, 0.14f,
    0.08f, 0.24f, 0.48f,
    0.2f, 0.25f, 0.2f,
    0.3f, 0.35f, 0.15f,
    1.0f, 0.52f, 0.24f,
    0.39f, 0.48f, 0.44f,
    0.41f, 0.52f, 0.58f,
    0.67f, 0.63f, 0.46f,
};

static const float kStationLightColors[81] = {
    0.29f, 0.35f, 0.31f,
    0.25f, 0.22f, 0.14f,
    0.2f, 0.17f, 0.09f,
    0.3f, 0.27f, 0.31f,
    0.25f, 0.34f, 0.41f,
    0.39f, 0.165f, 0.045f,
    0.37f, 0.43f, 0.26f,
    0.42f, 0.22f, 0.11f,
    0.18f, 0.25f, 0.28f,
    0.075f, 0.065f, 0.045f,
    0.2f, 0.43f, 0.35f,
    0.27f, 0.09f, 0.04f,
    0.38f, 0.29f, 0.16f,
    0.06f, 0.05f, 0.03f,
    0.11f, 0.1f, 0.11f,
    0.25f, 0.23f, 0.2f,
    0.25f, 0.19f, 0.135f,
    0.17f, 0.35f, 0.48f,
    0.07f, 0.07f, 0.07f,
    0.26f, 0.29f, 0.31f,
    0.13f, 0.1f, 0.06f,
    0.31f, 0.31f, 0.21f,
    0.39f, 0.32f, 0.25f,
    0.64f, 0.5f, 0.67f,
    0.45f, 0.38f, 0.34f,
    0.48f, 0.52f, 0.58f,
    0.21f, 0.27f, 0.34f,
};

static const float kRimLightColors[57] = {
    0.21f, 0.32f, 0.25f,
    0.5f, 0.29f, 0.28f,
    0.24f, 0.4f, 0.37f,
    0.22f, 0.32f, 0.43f,
    0.18f, 0.23f, 0.27f,
    0.38f, 0.13f, 0.11f,
    0.34f, 0.34f, 0.3f,
    0.24f, 0.26f, 0.25f,
    0.21f, 0.23f, 0.27f,
    0.43f, 0.47f, 0.47f,
    0.43f, 0.21f, 0.34f,
    0.2f, 0.11f, 0.04f,
    0.08f, 0.24f, 0.48f,
    0.2f, 0.25f, 0.2f,
    0.3f, 0.35f, 0.15f,
    0.5f, 0.26f, 0.12f,
    0.39f, 0.48f, 0.44f,
    0.41f, 0.52f, 0.58f,
    0.67f, 0.63f, 0.46f,
};

static inline float clampLightComponent(float v) {
    return v > 2.0f ? 2.0f : (v > 0.0f ? v : 0.0f);
}

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
        Globals::Canvas->TextureCreate(kSunTextures[tex],
                                       (*this->texturesArray)[0], false);
        this->tintColor = kSystemTints[
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
                Globals::Canvas->TextureCreate(kCurrentStationTextures[stationTex],
                                               (*this->texturesArray)[i], false);
            }
        } else {
            Globals::Canvas->TextureCreate(kStationTextures[stationTex],
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

    ArrayReleaseClasses(*stationArray);
    delete stationArray;

    this->playerTargets = new Array<KIPlayer *>();

    ArraySetLength<unsigned int>(static_cast<unsigned int>(count),
                                 *reinterpret_cast<Array<unsigned int> *>(this->playerTargets));

    this->planetsArray = new Array<AEGeometry *>();
    ArraySetLength(count + 1, *(this->planetsArray));

    this->positionsArray = new Array<Vector>();
    ArraySetLength(count + 1, *(this->positionsArray));

    Globals::rnd->setSeed((long long) status->getStation()->getIndex() * 300);

    int sunSlot = Globals::rnd->nextInt(14);
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
            (*this->playerTargets)[i - 1] = player;
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
void StarSystem::initLight() {
    ((Engine *) Globals::appManager->GetEngine())->lightCount = 2;

    int sunIndex;
    bool normalSystem;
    if (this->abstractSystem) {
        sunIndex = 30;
        normalSystem = false;
    } else {
        sunIndex = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() * 3;
        normalSystem = !this->abstractSystem;
    }
    float ambientScale = 0.15f;
    this->sunLightColor.x = kSunLightColors[sunIndex];
    this->sunLightColor.y = kSunLightColors[sunIndex + 1];
    this->sunLightColor.z = kSunLightColors[sunIndex + 2];

    if (normalSystem) {
        if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() == 15 &&
            (Globals::status->getCurrentCampaignMission() == 89 ||
             Globals::status->getCurrentCampaignMission() >= 158)) {
            this->sunLightColor.x *= 0.5f;
            this->sunLightColor.y *= 0.5f;
            this->sunLightColor.z *= 0.5f;
        }
    }
    if (!this->abstractSystem) {
        if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() == 15 &&
            Globals::status->getCurrentCampaignMission() != 89) {
            ambientScale = Globals::status->getCurrentCampaignMission() < 158 ? 0.7f : 0.15f;
        }
    }

    int stationIndex = this->abstractSystem ? 23 : Globals::status->getStation()->getTextureIndex() * 3;
    float stationR = kStationLightColors[stationIndex];
    float stationG = kStationLightColors[stationIndex + 1];
    float stationB = kStationLightColors[stationIndex + 2];

    float sunScale = 15.0f;
    if (!this->abstractSystem &&
        ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() == 15 &&
        Globals::status->getCurrentCampaignMission() != 89) {
        sunScale = Globals::status->getCurrentCampaignMission() < 158 ? 30.0f : 15.0f;
    }

    float sunR = this->sunLightColor.x;
    float sunG = this->sunLightColor.y;
    float sunB = this->sunLightColor.z;
    ((Engine *) Globals::appManager->GetEngine())->LightSetGlobalSceneColorAmbient(
            ambientScale * this->sunLightColor.x, ambientScale * this->sunLightColor.y,
            ambientScale * this->sunLightColor.z);
    ((Engine *) Globals::appManager->GetEngine())->LightSetRimColor(kRimLightColors[sunIndex] * 3.0f,
                                                                    kRimLightColors[sunIndex + 1] * 3.0f,
                                                                    kRimLightColors[sunIndex + 2] * 3.0f);
    ((Engine *) Globals::appManager->GetEngine())->LightSetMaterialColorAmbient(1.0f, 1.0f, 1.0f);
    ((Engine *) Globals::appManager->GetEngine())->LightSetMaterialColorDiffuse(1.0f, 1.0f, 1.0f);
    ((Engine *) Globals::appManager->GetEngine())->LightSetMaterialColorSpecular(1.0f, 1.0f, 1.0f);
    ((Engine *) Globals::appManager->GetEngine())->LightSetMaterialColorShininess(20.0f);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightDirection(
            this->lightDirection.x, this->lightDirection.y, this->lightDirection.z, 0x4000);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorAmbient(0.0f, 0.0f, 0.0f, 0x4000);
    float diffuseR = clampLightComponent(sunScale * sunR);
    float diffuseG = clampLightComponent(sunScale * sunG);
    float diffuseB = clampLightComponent(sunScale * sunB);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorDiffuse(diffuseR, diffuseG, diffuseB, 0x4000);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorSpecular(2.0f, 2.0f, 2.0f, 0x4000);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightDirection(0.0f, 0.0f, -1.0f, 0x4001);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorDiffuse(
            stationR * 1.5f, stationG * 1.5f, stationB * 1.5f, 0x4001);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorSpecular(
            stationR * 1.5f, stationG * 1.5f, stationB * 1.5f, 0x4001);
    ((Engine *) Globals::appManager->GetEngine())->LightSetParticleAmbient(
            this->sunLightColor.x, this->sunLightColor.y, this->sunLightColor.z);

    Globals::Canvas->FogEnable(false, AbyssEngine::FogMode_1);
    this->fogEnabled = 0;
    if (this->abstractSystem) {
        return;
    }

    uint32_t fogColor;
    int texture = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex();
    switch (texture) {
        case 11:
            fogColor = 0xdb6923ff;
            break;
        case 12:
            fogColor = 0x163e7cff;
            break;
        case 15:
            if (Globals::status->getCurrentCampaignMission() == 89) {
                return;
            }
            Globals::Canvas->FogSetParameter(AbyssEngine::FogMode_linear, 0.0f, 100000.0f, 1.0f, 0x82441fff);
            Globals::Canvas->FogEnable(true, AbyssEngine::FogMode_1);
            this->fogEnabled = 1;
            for (uint32_t i = 1; i < this->planetsArray->size(); ++i) {
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->planetsArray)[i]->transform))
                        ->color = (int) 0x82441fff;
            }
            return;
        case 16:
            fogColor = 0x47665eff;
            break;
        case 17:
            fogColor = 0x738d95ff;
            break;
        case 18:
            fogColor = 0xaba075ff;
            break;
        default:
            return;
    }

    Globals::Canvas->FogSetParameter(AbyssEngine::FogMode_linear, 0.0f,
                                     (texture == 16 || texture == 17 || texture == 18) ? 150000.0f : 50000.0f,
                                     1.0f, fogColor);
    Globals::Canvas->FogEnable(true, AbyssEngine::FogMode_1);
    this->fogEnabled = 1;
    int r = (int) ((float) (int) (fogColor >> 24) * 0.7f);
    int g = (int) ((float) (int) ((fogColor >> 16) & 0xff) * 0.7f);
    int b = (int) ((float) (int) ((fogColor >> 8) & 0xff) * 0.7f);
    uint32_t planetFog = (r << 24) | (g << 16) | (b << 8) | 0xff;
    for (uint32_t i = 1; i < this->planetsArray->size(); ++i) {
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->planetsArray)[i]->transform))->color =
                (int) planetFog;
    }
}
StarSystem::~StarSystem() {
    delete this->planetsArray;
    this->planetsArray = nullptr;

    if (this->playerTargets) {
        ArrayReleaseClasses(*this->playerTargets);
        delete this->playerTargets;
    }
    this->playerTargets = nullptr;

    delete this->lensFlare;
    this->lensFlare = nullptr;

    if (this->texturesArray) {
        ArrayRelease(*this->texturesArray);
        delete this->texturesArray;
    }
    this->texturesArray = nullptr;

    if (this->stationIdxArray) {
        ArrayRelease(*this->stationIdxArray);
        delete this->stationIdxArray;
    }
    this->stationIdxArray = nullptr;

    if (this->positionsArray) {
        ArrayRelease(*this->positionsArray);
        delete this->positionsArray;
    }
    this->positionsArray = nullptr;
}
Vector StarSystem::getLightDirection() {
    return this->lightDirection;
}
void StarSystem::switchPlanetForIntro() {
    Globals::Canvas->TextureCreate((uint16_t) 0x273a,
                                   (*this->texturesArray)[this->selectedStationSlot], false);
    Vector scaling = (*this->planetsArray)[this->selectedStationSlot]->getScaling() * 2.0f;
    (*this->planetsArray)[this->selectedStationSlot]->setScaling(scaling);
}
void StarSystem::switchSunForSupernovaReversal() {
    Globals::Canvas->TextureCreate((uint16_t) 0x2734,
                                   (*this->texturesArray)[0], false);
    (*this->planetsArray)[0]->setScaling(Vector{0.2288818359375f, 0.2288818359375f, 0.2288818359375f});
}
void StarSystem::switchSunForSupernovaIntro() {
    Globals::Canvas->TextureCreate((uint16_t) 0x2df3, (*this->texturesArray)[0], false);
    Globals::Canvas->TextureCreate((uint16_t) 0x2df4, this->supernovaSunTexture, false);

    this->sunStreak->setMesh(0x2df2);
    this->sunStreak->setScaling(this->sunStreak->getScaling() * 5.0f);

    (*this->planetsArray)[0]->setMesh(0x2df1);
    (*this->planetsArray)[0]->setScaling(0.6866455078125f);
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->planetsArray)[0]->transform))->SetAnimationState((AbyssEngine::AnimationMode) 0, nullptr);
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->planetsArray)[0]->transform))->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->planetsArray)[0]->transform))->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);

    SolarSystem *system = (SolarSystem *) (intptr_t) Globals::status->getSystem();
    this->tintColor = kSystemTints[system->getIndex()];
}
void StarSystem::switchSunForSupernovaExpansion() {
    (*this->planetsArray)[0]->setScaling(Vector{1.373291015625f, 1.373291015625f, 1.373291015625f});
}
void StarSystem::scaleSunDuringSupernovaIntro(int amount) {
    float scale = (*this->planetsArray)[0]->getScaling().x + (float) amount * 4.0e-5f;
    (*this->planetsArray)[0]->setScaling(scale, scale, scale);
}
void StarSystem::updateSupernova(int dt) {
    AEGeometry *streak = this->sunStreak;
    if (streak != nullptr) {
        AbyssEngine::Transform *transform = (AbyssEngine::Transform *) Globals::Canvas
                ->TransformGetTransform(streak->transform);
        transform->Update((longlong)dt, false);
    }
    AEGeometry *sun = (*this->planetsArray)[0];
    if (sun != nullptr) {
        AbyssEngine::Transform *transform = (AbyssEngine::Transform *) Globals::Canvas
                ->TransformGetTransform(sun->transform);
        transform->Update((longlong)dt, true);
    }
}
void *StarSystem::getPlanets() { // lint: void_ptr (exported signature; mangling must match lib)
    // lint: void_ptr (exported signature; mangling must match lib)
    return this->planetsArray;
}
Array<int> *StarSystem::getStationIndices() {
    return this->stationIdxArray;
}
void *StarSystem::getPlanetTargets() { // lint: void_ptr (exported signature; mangling must match lib)
    // lint: void_ptr (exported signature; mangling must match lib)
    return this->playerTargets;
}
float StarSystem::getPlanetScaleFactor() {
    return this->planetRingScaleOffset;
}
void StarSystem::renderSunStreak() {
    Globals::Canvas->SetTexture(this->supernovaSystem != 0 ? this->supernovaSunTexture : (*this->texturesArray)[0],
                                (unsigned) -1);
    Globals::Canvas->SetBlendMode(AbyssEngine::BlendMode_2);
    this->sunStreak->render();
}
void StarSystem::render() {
    using namespace AbyssEngine::AEMath;
    Vector cameraPos = MatrixGetPosition(*(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent()));
    for (unsigned i = 0; i < this->planetsArray->size(); ++i) {
        if (this->planetsArray->size() >= 2) {
            if (i == 0) {
                Matrix m = *(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
                Vector target = (*this->positionsArray)[0] + cameraPos;
                Vector up;
                if (this->supernovaSystem) {
                    up.x = 1.0f;
                    up.y = 0.0f;
                    up.z = 0.0f;
                } else {
                    up = MatrixGetUp(m);
                }
                m = MatrixGetLookAt(target, cameraPos, up);
                (*this->planetsArray)[0]->setRotation(0.0f, 0.0f, 0.0f);
                Vector scaling = (*this->planetsArray)[0]->getScaling();
                float grow = (this->lensFlare->intensity + -10.0f) * 0.015625f;
                grow = grow > 0.0f ? grow : 0.0f;
                if (this->supernovaSystem) {
                    MatrixSetScaling(m, scaling.x, scaling.y, scaling.z);
                } else {
                    MatrixSetScaling(m, scaling.x + grow, grow + scaling.y, scaling.z);
                }
                (*this->planetsArray)[0]->setMatrix(m);
                if (this->supernovaSystem) {
                    scaling *= 0.3f;
                    m = *(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
                    Vector streakTarget = (*this->positionsArray)[0] + cameraPos;
                    Vector streakUp = MatrixGetUp(m);
                    m = MatrixGetLookAt(streakTarget, cameraPos, streakUp);
                    MatrixSetScaling(m, grow + scaling.x, grow + scaling.y, scaling.z);
                    this->sunStreak->setMatrix(m);
                    this->renderSunStreak();
                }
                MatrixSetScaling(m, grow * (grow + scaling.x + 1.0f),
                                 scaling.y / ((1.0f - grow) * 6.0f + 6.0f), scaling.z);
                this->sunStreak->setMatrix(m);
            }
            if (i - 1 == (unsigned) StarSystem::orbitPlanetIndex && this->abstractSystem == 0 &&
                !Globals::status->inPlanetRingOrbit()) {
                float ring = cameraPos.z / -800000.0f;
                float clamped = ring < 0.2f ? ring : 0.2f;
                if (clamped < -0.2f) {
                    clamped = -0.2f;
                }
                this->planetRingScaleOffset = clamped;
                (*this->planetsArray)[i]->setScaling(this->planetScale + clamped, this->planetScale + clamped,
                                                    this->planetScale + clamped);
            }
            (*this->planetsArray)[i]->setPosition((*this->positionsArray)[i] + cameraPos);
        }
        Globals::Canvas->SetTexture((*this->texturesArray)[i], (unsigned) -1);
        Globals::Canvas->SetBlendMode(i == 0 ? AbyssEngine::BlendMode_2
                                             : (this->fogEnabled ? AbyssEngine::BlendMode_0x15
                                                                 : AbyssEngine::BlendMode_1));
        (*this->planetsArray)[i]->render();
        if (i == (unsigned) this->planetRingIndex) {
            Globals::Canvas->SetTexture(this->planetRingTexture, (unsigned) -1);
            Globals::Canvas->SetBlendMode(AbyssEngine::BlendMode_1);
            Matrix ringMatrix = (*this->planetsArray)[i]->getMatrix();
            MatrixSetScaling(ringMatrix, 4.0f, 4.0f, 4.0f);
            this->planetRing->setMatrix(ringMatrix);
            this->planetRing->render();
        }
    }
}
void StarSystem::render2D() {
    if (this->lensFlare == nullptr)
        return;

    Matrix *local = (Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
    Vector pos;
    pos.x = local->m[3] + this->lightDirection.x * 65536.0f;
    pos.y = local->m[7] + this->lightDirection.y * 65536.0f;
    pos.z = local->m[11] + this->lightDirection.z * 65536.0f;
    this->lensFlare->update(0);
    if (Globals::Canvas->GetScreenPosition(pos, pos)) {
        this->lensFlare->render2D(pos.x, pos.y, pos.z, this->tintColor);
    }
}
void StarSystem::rotate(int x, int y, int z) {
}