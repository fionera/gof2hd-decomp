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
#include <cstdlib>

int StarSystem::orbitPlanetIndex;

namespace AbyssEngine {
    namespace AEMath {
        Vector operator+(const Vector &lhs, const Vector &rhs);

        Vector operator*(const Vector &lhs, float rhs);
    }
}

static inline void MatrixGetUp(Vector *out, const Matrix *matrix) { *out = AbyssEngine::AEMath::MatrixGetUp(*matrix); }

static inline void MatrixGetLookAt(Matrix *out, const Vector *from, const Vector *to, const Vector *up) { *out = AbyssEngine::AEMath::MatrixGetLookAt(*from, *to, *up); }



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

// Keep the operands expanded: introducing a temporary changes the original
// clang min/max lowering in initLight and render.
#define STAR_MIN(a, b) ((a) > (b) ? (b) : (a))
#define STAR_MAX(a, b) ((a) > (b) ? (a) : (b))

StarSystem::StarSystem(int mode) {
    this->stationIdxArray = nullptr;
    this->texturesArray = nullptr;
    this->playerTargets = nullptr;
    this->planetsArray = nullptr;
    this->abstractSystem = Globals::status->getSystem() == 0;
    this->supernovaSystem = Globals::status->inSupernovaSystem();
    this->lensFlare = new LensFlare(Globals::Canvas);
    this->planetRing = nullptr;
    this->planetRingIndex = -1;

    if (this->abstractSystem) {
        // no real system: a sun and one planet as backdrop
        this->planetsArray = new Array<AEGeometry *>();
        ArraySetLength(2, *this->planetsArray);
        (*this->planetsArray)[0] = new AEGeometry((uint16_t) 6768, Globals::Canvas, false);
        (*this->planetsArray)[0]->setScaling(Vector{15000.0f / 65536.0f, 15000.0f / 65536.0f, 15000.0f / 65536.0f});
        (*this->planetsArray)[0]->moveForward(-20000.0f);
        this->lightDirection = (*this->planetsArray)[0]->getDirection();
        this->lightDirection.x = -this->lightDirection.x;
        this->lightDirection.y = -this->lightDirection.y;
        this->lightDirection.z = -this->lightDirection.z;
        (*this->planetsArray)[1] = new AEGeometry((uint16_t) 6768, Globals::Canvas, false);
        float scale = (float) (Globals::rnd->nextInt(20000) + 20000) * (1.0f / 65536.0f);
        (*this->planetsArray)[1]->setScaling(Vector{scale, scale, scale});
        (*this->planetsArray)[1]->setRotation(Vector{0.0f, 5.234709739685059f, 0.0f});
        (*this->planetsArray)[1]->moveForward(-20000.0f);

        this->texturesArray = new Array<uint32_t>();
        ArraySetLength(2, *this->texturesArray);
        Globals::Canvas->TextureCreate((uint16_t) 0x2739, (*this->texturesArray)[0], false);
        Globals::Canvas->TextureCreate((uint16_t) 0x2719, (*this->texturesArray)[1], false);

        this->positionsArray = new Array<Vector>();
        ArraySetLength(2, *this->positionsArray);
        (*this->positionsArray)[0] = (*this->planetsArray)[0]->getPosition();
        (*this->positionsArray)[1] = (*this->planetsArray)[1]->getPosition();

        this->sunStreak = new AEGeometry((uint16_t) 6768, Globals::Canvas, false);
        this->sunStreak->setScaling(Vector{30000.0f / 65536.0f, 3000.0f / 65536.0f, 15000.0f / 65536.0f});
    } else {
        Array<int> *stations = (Array<int> *) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStations();
        FileRead *reader = new FileRead();
        Array<Station *> *stationArray = reader->loadStationsBinary((SolarSystem *) (intptr_t) Globals::status->getSystem());
        delete reader;

        this->texturesArray = new Array<uint32_t>();
        ArraySetLength(stationArray->size() + 1, *this->texturesArray);
        this->stationIdxArray = new Array<int>();
        ArraySetLength(stations->size(), *this->stationIdxArray);

        int mission = Globals::status->getCurrentCampaignMission();
        if ((mission == 89 && this->supernovaSystem) ||
            (mission > 157 && ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex() == 27)) {
            Globals::Canvas->TextureCreate((uint16_t) 0x2dde, (*this->texturesArray)[0], false);
            this->tintColor = 3;
        } else {
            Globals::Canvas->TextureCreate(
                    kSunTextures[((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex()],
                    (*this->texturesArray)[0], false);
            this->tintColor = kSystemTints[((SolarSystem *) (intptr_t) Globals::status->getSystem())->getIndex()];
        }

        uint32_t i;
        for (i = 1; i < this->texturesArray->size(); ++i) {
            if (stations->data()[i - 1] == Globals::status->getStation()->getIndex()) {
                if (Globals::status->getCurrentCampaignMission() == 0 && mode == 3) {
                    this->selectedStationSlot = i;
                    Globals::Canvas->TextureCreate((uint16_t) 0x273b, (*this->texturesArray)[i], false);
                } else {
                    Globals::Canvas->TextureCreate(kCurrentStationTextures[stationArray->data()[i - 1]->getTextureIndex()],
                                                   (*this->texturesArray)[i], false);
                }
            } else {
                Globals::Canvas->TextureCreate(kStationTextures[stationArray->data()[i - 1]->getTextureIndex()],
                                               (*this->texturesArray)[i], false);
                if (Globals::status->orbitHasPlanetRing(stationArray->data()[i - 1]->getIndex())) {
                    this->planetRing = new AEGeometry((uint16_t) 6768, Globals::Canvas, false);
                    Globals::Canvas->TextureCreate((uint16_t) 0x7198, this->planetRingTexture, false);
                    this->planetRingIndex = i;
                }
            }
            this->stationIdxArray->data()[i - 1] = stationArray->data()[i - 1]->getIndex();
        }
        ArrayReleaseClasses(*stationArray);
        delete stationArray;

        this->playerTargets = new Array<KIPlayer *>();
        ArraySetLength(stations->size(), *this->playerTargets);
        this->planetsArray = new Array<AEGeometry *>();
        ArraySetLength(stations->size() + 1, *this->planetsArray);
        this->positionsArray = new Array<Vector>();
        ArraySetLength(stations->size() + 1, *this->positionsArray);

        // planets are placed on 24 orbit slots of 2730/65536 turns each
        Globals::rnd->setSeed((long long) (Globals::status->getStation()->getIndex() * 300));
        int usedSlots[24];
        for (int k = 0; k < 24; ++k) {
            usedSlots[k] = 0;
        }
        int slot = 0;
        int sunSlot = 0;
        for (uint32_t i = 0; i < this->planetsArray->size() && (int) i >= 0; ++i) {
            (*this->planetsArray)[i] = new AEGeometry((uint16_t) 6768, Globals::Canvas, false);
            if (!this->abstractSystem &&
                ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() >= 15 &&
                ((AbyssEngine::Engine *) Globals::appManager->GetEngine())->IsPostEffectActivated()) {
                reinterpret_cast<float &>(Globals::Canvas->MeshGetPointer((*this->planetsArray)[i]->meshId)->materialId) =
                        0.22f;
            }

            int flip;
            if (i == 0) {
                int randomSlot = Globals::rnd->nextInt(14);
                switch (Globals::status->getStation()->getTextureIndex()) {
                    case 9:
                    case 13:
                    case 14:
                    case 18:
                    case 20:
                        slot = 6;
                        break;
                    case 21:
                        slot = 4;
                        break;
                    case 22:
                        slot = 16;
                        break;
                    default:
                        slot = randomSlot + 5;
                        break;
                }
                if (this->supernovaSystem) {
                    float sunScale = Globals::status->getCurrentCampaignMission() < 106 ? 65000.0f / 65536.0f
                                                                                         : 90000.0f / 65536.0f;
                    (*this->planetsArray)[0]->setScaling(Vector{sunScale, sunScale, sunScale});
                    this->sunStreak = new AEGeometry((uint16_t) 6768, Globals::Canvas, false);
                    this->sunStreak->setScaling(Vector{130000.0f / 65536.0f, 13000.0f / 65536.0f, 65000.0f / 65536.0f});
                    if (((AbyssEngine::Engine *) Globals::appManager->GetEngine())->IsPostEffectActivated()) {
                        reinterpret_cast<float &>(Globals::Canvas->MeshGetPointer(this->sunStreak->meshId)->materialId) =
                                0.22f;
                    }
                    Globals::Canvas->TextureCreate((uint16_t) 0x2dde, this->supernovaSunTexture, false);
                } else {
                    (*this->planetsArray)[0]->setScaling(
                            Vector{15000.0f / 65536.0f, 15000.0f / 65536.0f, 15000.0f / 65536.0f});
                    this->sunStreak = new AEGeometry((uint16_t) 6768, Globals::Canvas, false);
                    this->sunStreak->setScaling(Vector{30000.0f / 65536.0f, 3000.0f / 65536.0f, 15000.0f / 65536.0f});
                }
                usedSlots[slot] = 1;
                flip = 0x8000;
                sunSlot = slot;
            } else {
                if (stations->data()[i - 1] == Globals::status->getStation()->getIndex()) {
                    (*this->playerTargets)[i - 1] = new PlayerStatic(0, (*this->planetsArray)[i], 0.0f, 0.0f, 0.0f);
                    // the orbited planet keeps slot 0; the slot search is a leftover of the J2ME placement loop
                    bool placed = false;
                    while (!placed) {
                        slot = 0;
                        placed = true;
                    }
                    flip = sunSlot < 12 ? 0 : 0x8000;
                    StarSystem::orbitPlanetIndex = i - 1;
                    int size = Globals::rnd->nextInt(20000) + 20000;
                    if (Globals::status->getCurrentCampaignMission() == 0) {
                        size = (int) ((float) size * 0.5f);
                    }
                    int texture = Globals::status->getStation()->getTextureIndex();
                    if (Globals::status->inPlanetRingOrbit()) {
                        size = 26000;
                    } else {
                        switch (texture) {
                            case 9:
                            case 16:
                                size = Globals::rnd->nextInt(13000) + 32500;
                                break;
                            case 6:
                            case 11:
                            case 12:
                            case 17:
                                size = Globals::rnd->nextInt(15000) + 35000;
                                break;
                            default:
                                break;
                        }
                    }
                    this->planetScale = (float) size * (1.0f / 65536.0f);
                    (*this->planetsArray)[i]->setScaling(Vector{this->planetScale, this->planetScale, this->planetScale});
                } else {
                    (*this->playerTargets)[i - 1] = new PlayerStatic(0, (*this->planetsArray)[i], 0.0f, 0.0f, 0.0f);
                    bool placed = false;
                    while (!placed) {
                        slot = Globals::rnd->nextInt(11) + 7;
                        placed = false;
                        if (abs(slot - sunSlot) >= 3) {
                            placed = usedSlots[slot] == 0;
                        }
                    }
                    float scale = (float) ((0.8f + 0.01 * Globals::rnd->nextInt(40)) * (2300.0 / 65536.0));
                    (*this->planetsArray)[i]->setScaling(Vector{scale, scale, scale});
                    flip = sunSlot - slot < 12 ? 0 : 0x8000;
                    if (sunSlot <= slot) {
                        flip = 0x8000;
                    }
                }
                usedSlots[slot] = 1;
            }

            const float kTwoPi = 6.2831855f;
            if (slot == sunSlot && i != 0) {
                (*this->planetsArray)[i]->setRotation(Vector{0.0f, (float) (slot * 2730) * (1.0f / 65536.0f) * kTwoPi, 0.0f});
            } else if (i > 0 && stations->data()[i - 1] == Globals::status->getStation()->getIndex()) {
                (*this->planetsArray)[i]->setRotation(Vector{0.0f, (float) (slot * 2730) * (1.0f / 65536.0f) * kTwoPi, 0.0f});
            } else {
                AEGeometry *planet = (*this->planetsArray)[i];
                float tilt = (float) (Globals::rnd->nextInt(4096) - 2048);
                tilt *= (1.0f / 65536.0f);
                tilt *= kTwoPi;
                Vector rotation{tilt, (float) (slot * 2730) * (1.0f / 65536.0f) * kTwoPi, 0.0f};
                planet->setRotation(rotation);
            }
            (*this->planetsArray)[i]->moveForward(-20000.0f);
            if (i == 0) {
                this->lightDirection = (*this->planetsArray)[0]->getDirection();
                this->lightDirection.x = -this->lightDirection.x;
                this->lightDirection.y = -this->lightDirection.y;
                this->lightDirection.z = -this->lightDirection.z;
            } else if (flip != 0) {
                (*this->planetsArray)[i]->rotate(Vector{0.0f, (float) flip * (1.0f / 65536.0f) * kTwoPi, 0.0f});
            }
            (*this->positionsArray)[i] = (*this->planetsArray)[i]->getPosition();
        }
        Globals::rnd->reset();
    }
    this->initLight();
}
void StarSystem::initLight() {
    ((Engine *) Globals::appManager->GetEngine())->lightCount = 2;

    float ambientScale = 0.15f;
    int sunIndex;
    bool normalSystem;
    if (this->abstractSystem) {
        sunIndex = 30;
        normalSystem = false;
    } else {
        sunIndex = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() * 3;
        normalSystem = !this->abstractSystem;
    }
    this->sunLightColor[0] = kSunLightColors[sunIndex];
    this->sunLightColor[1] = kSunLightColors[sunIndex + 1];
    this->sunLightColor[2] = kSunLightColors[sunIndex + 2];

    int stationIndex;
    uint8_t abstractStation;
    if (normalSystem) {
        if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() == 15 &&
            (Globals::status->getCurrentCampaignMission() == 89 ||
             Globals::status->getCurrentCampaignMission() >= 158)) {
            this->sunLightColor[0] *= 0.5f;
            this->sunLightColor[1] *= 0.5f;
            this->sunLightColor[2] *= 0.5f;
        }
        if (!this->abstractSystem) {
            if (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() == 15 &&
                Globals::status->getCurrentCampaignMission() != 89) {
                ambientScale = Globals::status->getCurrentCampaignMission() < 158 ? 0.7f : 0.15f;
            }
        }
        if (this->abstractSystem) {
            stationIndex = 23;
            abstractStation = this->abstractSystem;
        } else {
            stationIndex = Globals::status->getStation()->getTextureIndex() * 3;
            abstractStation = this->abstractSystem;
        }
    } else {
        stationIndex = 23;
        abstractStation = 1;
    }
    float stationR = kStationLightColors[stationIndex];
    float stationG = kStationLightColors[stationIndex + 1];
    float stationB = kStationLightColors[stationIndex + 2];

    float sunScale = 15.0f;
    if (!abstractStation &&
        ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() == 15 &&
        Globals::status->getCurrentCampaignMission() != 89) {
        sunScale = Globals::status->getCurrentCampaignMission() < 158 ? 30.0f : 15.0f;
    }

    float sunR = this->sunLightColor[0];
    float sunG = this->sunLightColor[1];
    float sunB = this->sunLightColor[2];
    ((Engine *) Globals::appManager->GetEngine())->LightSetGlobalSceneColorAmbient(
            ambientScale * this->sunLightColor[0], ambientScale * this->sunLightColor[1],
            ambientScale * this->sunLightColor[2]);
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
    float diffuseR = STAR_MIN(STAR_MAX(sunScale * sunR, 0.0), 2.0);
    float diffuseG = STAR_MIN(STAR_MAX(sunScale * sunG, 0.0), 2.0);
    float diffuseB = STAR_MIN(STAR_MAX(sunScale * sunB, 0.0), 2.0);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorDiffuse(diffuseR, diffuseG, diffuseB, 0x4000);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorSpecular(2.0f, 2.0f, 2.0f, 0x4000);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightDirection(0.0f, 0.0f, -1.0f, 0x4001);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorDiffuse(
            stationR * 1.5f, stationG * 1.5f, stationB * 1.5f, 0x4001);
    ((Engine *) Globals::appManager->GetEngine())->LightSetLightColorSpecular(
            stationR * 1.5f, stationG * 1.5f, stationB * 1.5f, 0x4001);
    ((Engine *) Globals::appManager->GetEngine())->LightSetParticleAmbient(
            this->sunLightColor[0], this->sunLightColor[1], this->sunLightColor[2]);

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
    for (int i = 0; i < this->planetsArray->size(); ++i) {
        if (this->planetsArray->size() >= 2) {
            if (i == 0) {
                Matrix m = *(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
                m = MatrixGetLookAt((*this->positionsArray)[0] + cameraPos, cameraPos,
                                    this->supernovaSystem ? Vector(1.0f, 0.0f, 0.0f) : MatrixGetUp(m));
                (*this->planetsArray)[0]->setRotation(0.0f, 0.0f, 0.0f);
                Vector scaling = (*this->planetsArray)[0]->getScaling();
                float grow = (this->lensFlare->intensity + -10.0f) * 0.015625f;
                grow = grow < 0.0f ? 0.0f : grow;
                if (this->supernovaSystem) {
                    MatrixSetScaling(m, scaling.x, scaling.y, scaling.z);
                } else {
                    MatrixSetScaling(m, scaling.x + grow, grow + scaling.y, scaling.z);
                }
                (*this->planetsArray)[0]->setMatrix(m);
                if (this->supernovaSystem) {
                    scaling *= 0.3f;
                    m = *(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
                    {
                        Vector streakTarget = (*this->positionsArray)[0] + cameraPos;
                        Vector streakUp = MatrixGetUp(m);
                        m = MatrixGetLookAt(streakTarget, cameraPos, streakUp);
                    }
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
                float clamped = STAR_MAX(-0.2f, STAR_MIN(0.2f, cameraPos.z / -800000.0f));
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
#undef STAR_MIN
#undef STAR_MAX

void StarSystem::render2D() {
    if (this->lensFlare == nullptr)
        return;

    Matrix local = *(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
    Vector pos(local.m[3] + this->lightDirection.x * 65536.0f, local.m[7] + this->lightDirection.y * 65536.0f,
               local.m[11] + this->lightDirection.z * 65536.0f);
    this->lensFlare->update(0);
    if (Globals::Canvas->GetScreenPosition(pos, pos)) {
        this->lensFlare->render2D(pos.x, pos.y, pos.z, this->tintColor);
    }
}
void StarSystem::rotate(int x, int y, int z) {
}