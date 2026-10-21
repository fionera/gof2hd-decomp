#include "game/menu/MGame.h"
#include "game/menu/ModStation.h"
#include <new>
#include "game/core/Radio.h"
#include "game/core/RadioMessage.h"
#include "game/core/Globals.h"
#include "game/ship/PlayerFighter.h"
#include "game/ship/Ship.h"
#include "game/world/SolarSystem.h"
#include "game/ship/TargetFollowCamera.h"
#include "engine/render/AEGeometry.h"
#include "game/ui/ChoiceWindow.h"
#include "engine/audio/FModSound.h"
#include "game/world/Galaxy.h"
#include "game/mission/Item.h"
#include "game/world/LevelScript.h"
#include "game/ui/MenuTouchWindow.h"
#include "game/mission/Objective.h"
#include "game/ship/PlayerJumpgate.h"
#include "game/world/StarMap.h"
#include "engine/math/Transform.h"
#include "game/mission/Achievements.h"
#include "engine/core/ApplicationManager.h"
#include "game/ui/DialogueWindow.h"
#include "engine/render/Engine.h"
#include "game/mission/GameRecord.h"
#include "engine/core/GameText.h"
#include "game/ui/Hud.h"
#include "engine/render/ImageFactory.h"
#include "game/ui/Layout.h"
#include "game/world/Level.h"
#include "game/mission/Mission.h"
#include "game/ship/Player.h"
#include "game/mission/RecordHandler.h"
#include "game/world/Route.h"
#include "game/world/StarSystem.h"
#include "game/world/Station.h"
#include "game/ship/KIPlayer.h"
#include "game/ship/PlayerEgo.h"
#include "game/ship/PlayerFixedObject.h"
#include "game/ship/Agent.h"
#include "game/mission/Status.h"
#include "engine/render/PaintCanvas.h"
#include "game/weapons/Radar.h"
#include "engine/input/VirtualInput.h"
#include "engine/file/FileRead.h"
#include "engine/core/AERandom.h"
#include "game/world/Standing.h"

namespace AbyssEngine {
    namespace AEMath {
        float VectorLength(const Vector &value);

        Vector operator-(const Vector &lhs, const Vector &rhs);

        Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector);
    }
}

static inline void TFC_setFastForwardMode(TargetFollowCamera *c, int v) { c->setFastForwardMode(v); }


static inline void TFC_setLookAtCam(TargetFollowCamera *c, int v) { c->setLookAtCam(v); }





static int *g_maneuverScale = &Globals::w;

void MGame::maneuverTouchEnd(int a, int b, void *p) { // lint: void_ptr -- exported method signature (Pv)
    // lint: void_ptr -- exported method signature (Pv)
    MGame *self = this;
    (void) b;
    (void) p;
    if (self->maneuverActive != 0 && self->maneuverHoldTime <= 0x258) {
        float f2 = (float) (*g_maneuverScale);
        int d = a - self->maneuverStartX;
        if (d < 0) d = -d;
        float f3 = (float) d;
        if ((f2 / 480.0f) * 70.0f < f3) {
            int dir = 1;
            if (self->maneuverStartX < a) dir = 2;
            self->player->initManeuver(dir);
        }
    }
    self->maneuverActive = 0;
}

void MGame::OnResume() {
    FModSound **holder = &Globals::sound;
    FModSound *sound = *holder;
    if (sound == 0)
        return;
    if (sound->tryToStopMusicForBGMusic() != 0)
        return;
    (*holder)->setVolume(1, *reinterpret_cast<float *>(Globals::options));
}

void MGame::maneuverTouchMove(int a, int b, void *p) { // lint: void_ptr -- exported method signature (Pv)
    // lint: void_ptr -- exported method signature (Pv)
    (void) a;
    (void) p;
    if (this->maneuverActive != 0) {
        float f2 = (float) (*g_maneuverScale);
        int d = b - this->maneuverStartY;
        if (d < 0) d = -d;
        float f3 = (float) d;

        if ((f2 / 320.0f) * 90.0f < f3) {
            this->maneuverActive = 0;
            this->maneuverHoldTime = 0;
        }
    }
}

void MGame::maneuverTouchBegin(int x, int y, void *p) { // lint: void_ptr -- exported method signature (Pv)
    // lint: void_ptr -- exported method signature (Pv)
    (void) p;
    this->maneuverActive = 1;
    this->maneuverStartX = x;
    this->maneuverStartY = y;
    this->maneuverHoldTime = 0;
}

void MGame::OnTouchBegin(int /*p1*/, int /*p2*/) {
}

void MGame::OnTouchEnd(int /*p1*/, int /*p2*/) {
}

void MGame::OnTouchMove(int /*p1*/, int /*p2*/) {
}

void MGame::OnKeyPress(long long /*key*/, long long /*mod*/) {}
void MGame::OnKeyRelease(long long /*key*/, long long /*mod*/) {}

void MGame::showLiteScreen() {
}

int MGame::ShowLoadingScreen() { return 1; }

void MGame::pause() {
}

void MGame::OnRender3D() {
    if (this->active == 0) return;
    Globals::Canvas->ClearBuffer(0);

    uint8_t inMenuLevel = this->pauseOpen;
    uint8_t flag15e = this->freeCamMode;

    if (inMenuLevel == 0) {
        this->level->renderBG(0);
        Globals::Canvas->Begin3d();
        int arg = (flag15e == 0) ? this->deltaTime : 0;
        this->level->render(arg);
        int egoFlag = (this->jumpActive != 0) ? 0 : (this->jumpDriveActive == 0);
        this->player->render(egoFlag);
        if (this->jumpFlash != 0)
            ((AEGeometry *) (this->player))->render();
        this->levelScript->render3D();
        return Globals::Canvas->End3d();
    }

    if (flag15e != 0) {
        this->level->renderBG(0);
        Globals::Canvas->Begin3d();
        this->level->render(this->deltaTime);
        int egoFlag = (this->jumpActive != 0) ? 0 : (this->jumpDriveActive == 0);
        this->player->render(egoFlag);
        if (this->jumpFlash != 0)
            ((AEGeometry *) (this->player))->render();
        this->levelScript->render3D();
        return Globals::Canvas->End3d();
    }

    if (this->menuTouchOpen != 0) {
        Globals::Canvas->Begin3d();
        this->menuWindow->render3D();
        return Globals::Canvas->End3d();
    }

    if (this->starMapOpen != 0) {
        Globals::Canvas->Begin3d();
        this->starMap->render();
        return Globals::Canvas->End3d();
    }

    this->level->renderBG(0);
    Globals::Canvas->Begin3d();
    this->level->render(0);
    int egoFlag = (this->jumpActive != 0) ? 0 : (this->jumpDriveActive == 0);
    this->player->render(egoFlag);
    if (this->jumpFlash != 0)
        ((AEGeometry *) (this->player))->render();
    this->levelScript->render3D();
    return Globals::Canvas->End3d();
}

static inline void TFC_setActive(TargetFollowCamera *c, int v) { c->setActive(v); }

static inline void TFC_setPosition(TargetFollowCamera *c, float x, float y, float z) { c->setPosition(x, y, z); }

static int *g_jsSound;

static int g_jsHudFlag;

static int g_jsFovDefault;

static int g_jsFovAlienA;

static int g_jsFovAlienB;

static int g_jsPostEffect;

static int g_jsOffsetX;

static int g_jsOffsetY;

static int g_jsOffsetZ;

static int g_jsOffsetZ2;

void MGame::startJumpScene() {
    this->player->player->setVulnerable(false);
    this->level->enableFog(false);

    if (this->player->isDockedToDockingPoint() != 0) {
        this->player->dockToDockingPoint(nullptr, this->radar);
        TFC_setActive(this->camera, 1);
        TFC_setLookAtCam(this->camera, 0);
        this->camera->useTargetsUpVector(false);
        this->player->setSpeed(0.0f);
        this->player->setDockingState(0);
    }
    if (this->player->isInTurretMode() != 0)
        this->player->setTurretMode(0);

    Globals::sound->stop(0x23);
    this->switchCamera(0);
    this->field_0x70 = 0x3f9c28f6;
    this->hud->releaseAllKeys();
    this->_b5c = 0;
    this->field_0x110 = 0;

    unsigned cameraId = this->cameraId;
    float perspective = reinterpret_cast<float &>(this->field_0x70);
    PaintCanvas *canvas = Globals::Canvas;
    float fov = 300000.0f;
    if (Globals::status->inAlienOrbit() != 0) {
        int cm = Globals::status->getCurrentCampaignMission();
        fov = (cm < 0x50) ? 450000.0f : 300000.0f;
    }
    canvas->CameraSetPerspective(cameraId, perspective, 20.0f, fov);
    this->player->setAutoPilot(nullptr);
    this->hudMenuOpen = 0;
    this->pauseOpen = 0;
    this->jumpDriveActive = 1;
    this->jumpActive = 1;
    this->player->setCollide(false);
    TFC_setLookAtCam(this->camera, 1);
    this->player->stopBoost();
    Engine *eng = static_cast<Engine *>(this->applicationManager->GetEngine());
    eng->SetPostEffect(0x01400002, false);

    if (this->usingJumpDrive) {
        this->player->resetMovement();
        this->player->setComputerControlled(true);
        this->jumpFlash = new AEGeometry((uint16_t) 0x3ab2, Globals::Canvas, false);
        static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->jumpFlash->transform))
            ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);

        this->egoJumpPos = this->player->getPosition();
        this->egoJumpPos += this->player->geometry->getDirection() * 3000.0f;
        this->jumpFlash->setPosition(this->egoJumpPos);

        Vector offset = this->player->geometry->getDirection();
        this->jumpFlash->setScaling(2.0f, 2.0f, 2.0f);
        this->jumpFlash->setDirection(offset, Vector{0.0f, 1.0f, 0.0f});

        offset = Vector{-2000.0f, 300.0f, -2000.0f};
        offset = AbyssEngine::AEMath::MatrixRotateVector(
            reinterpret_cast<const AbyssEngine::Matrix &>(this->player->player->transform[0]), offset);
        this->egoJumpPos += offset;

        Globals::sound->stop(this->player->field_0x1c);
        Globals::sound->stop(0x23);
        Globals::sound->stop(0x8d5);
        Globals::sound->stop(0x8d4);
        Globals::sound->play(0x20, nullptr, nullptr, 0.0f);
    } else {
        this->egoJumpPos = (*this->level->getLandmarks())[1]->getPosition();
        this->egoJumpPos.z += -10000.0f;
        this->player->setPosition(this->egoJumpPos);
        this->player->setComputerControlled(true);
        this->player->geometry->setRotation(0.0f, 0.0f, 0.0f);
        this->egoJumpPos.x += -2000.0f;
        this->egoJumpPos.y += 300.0f;
        this->egoJumpPos.z += 4000.0f;
    }
    TFC_setPosition(this->camera, this->egoJumpPos.x, this->egoJumpPos.y, this->egoJumpPos.z);
}



void MGame::switchCamera(int id) {
    int prevMode = this->cameraMode;
    if (id == 2) id++;
    this->cameraMode = id;
    if (id == 1) {
        this->turretMode = this->player->isDockingToAsteroid() ? false : this->player->setTurretMode(true);
        if (!this->turretMode)
            this->cameraMode++;
    } else {
        this->turretMode = false;
    }
    if (this->cameraMode == 2)
        this->cameraMode = 3;
    else if (this->cameraMode >= 4)
        this->cameraMode = 0;
    this->field_0x18 = 0;
    this->player->setTurretMode(this->turretMode);

    switch (this->cameraMode) {
        case 0:
            if (this->player->isDockedToDockingPoint()) {
                switchCamera(1);
                return;
            }
            this->levelScript->resetCamera(this->level);
            this->camera->setRotationAroundTarget(false);
            this->player->setFreeLookMode(false);
            static_cast<Engine *>(this->applicationManager->GetEngine())->wheelDelta = 0.0f;
            break;
        case 1:
        case 3:
            if (this->player->isDockedToDockingPoint())
                this->camera->setLookAtCam(false);
            this->levelScript->lookBehind();
            this->camera->setRotationAroundTarget(true);
            this->player->setFreeLookMode(true);
            break;
        case 2:
            this->levelScript->resetCamera(this->level);
            this->camera->setRotationAroundTarget(false);
            this->player->setFreeLookMode(false);
            if (prevMode == 1)
                this->field_0x18 = 1;
            break;
    }
    this->camera->enableFirstPersonCam(this->cameraMode == 2);
    this->player->hideShipForFirstPersonCameraView(this->field_0x18 ? true : this->camera->hideShipForFirstPersonCam());
}

void MGame::freeCamTouchBegin(int x, int y, void *touch) { // lint: void_ptr -- exported method signature (Pv)
    int id = (int) (intptr_t) touch;
    if (this->touch0Id != 0) {
        if (this->menuTime < 1000) {
            this->flCameraRoll = 0;
            this->flCameraRoll = AbyssEngine::AEMath::VectorLength(*this->camera->getCamOffset());
            this->touch1Id = id;
            this->freeCamFinger0 = Vector{(float) x, (float) y, 0.0f};
        }
    } else {
        if (this->touch1Id == 0)
            this->menuTime = 0;
        this->touch0Id = id;
        this->freeCamFinger1 = Vector{(float) x, (float) y, 0.0f};
    }
    this->dragLastX = x;
    this->dragLastY = y;
    this->dragStartX = x;
    this->dragStartY = y;
    this->dragDeltaX = 0;
    this->dragDeltaY = 0;
    this->freeCamDragging = 1;
}

static GameText **g_gameText = &Globals::gameText;

void MGame::useCloak() {
    if (((::PlayerEgo *) (this->player))->toggleCloaking()) return;
    if (this->choiceWindow == 0)
        this->choiceWindow = new ChoiceWindow();
    Item *eq = ((Ship *) (Globals::status->getShip()))->getFirstEquipmentOfSort(0x15);
    int attr = eq == 0 ? 0 : ((Item *) (eq))->getAttribute(0x26);
    ChoiceWindow *cw = this->choiceWindow;
    String *txt = ((GameText *) (*g_gameText))->getText(0x247);
    String s0, s1, s2, s3, s4, s5;
    s2.ctor_char("", false);
    s3 = *(String *) txt + s2;
    s1.Set((long long) (attr));
    s4 = s3 + s1;
    s0.ctor_char("", false);
    s5 = s4 + s0;
    ((ChoiceWindow *) (cw))->set(*(String *) &s5);
    this->pauseOpen = 1;
    this->choiceWindowOpen = 1;
    ((MGame *) (this))->pauseSounds();
}



static DialogueWindow **g_goDlgA;








void MGame::gameOverCheck() {
    if (this->player->getHitpoints() <= 0) {
        if (this->player->tryToStartEmergencySystem()) return;
        if (this->player->isInWormhole()) {
            this->gameOverActive = 1;
            this->gameOverTitle = *Globals::gameText->getText(319);
            this->needsRedraw = 1;
        } else {
            this->player->setTurretMode(false);
            this->levelScript->resetCamera(this->level);
            this->player->setFreeLookMode(false);
            this->camera->enableFirstPersonCam(false);
            this->player->hideShipForFirstPersonCameraView(false);
            this->needsRedraw = 1;
            this->player->explode();
            if (this->player->explosionEnded()) {
                this->gameOverActive = 1;
                this->gameOverTitle = *Globals::gameText->getText(319);
            }
        }
        if (this->gameOverActive) {
            if (Globals::status->getMission() != nullptr && Globals::status->getMission()->isCampaignMission()) {
                int mission = Globals::status->getCurrentCampaignMission();
                if (mission != Globals::lastCampaignMissionFailed)
                    Globals::lastCampaignMissionFailCount = 0;
                Globals::lastCampaignMissionFailed = mission;
                Globals::lastCampaignMissionFailCount++;
            }
            this->gameRecord = Globals::recordHandler->recordStoreRead(Globals::lastRecordWritten);
        }
    }

    if (this->level->checkGameOver((int) this->levelScript->m_nScriptTime)) {
        if (this->dialogueWindow == nullptr) {
            this->dialogueWindow = new DialogueWindow();
            if (this->level != nullptr) this->dialogueWindow->setLevel(this->level);
        } else if (!this->dialogueWindow->hasLevel() && this->level != nullptr) {
            this->dialogueWindow->setLevel(this->level);
        }
        this->dialogueWindow->set(Globals::status->getMission(), 2, -1);
        this->cutsceneActive = 1;
        pauseSounds();
        this->pauseOpen = 1;
    }

    if (this->elapsedTime >= 5001) {
        this->elapsedTime = 0;
        LevelScript *script = this->levelScript;
        if (script->m_nTimeLimit >= 1 &&
            script->m_nTimeLimit < script->m_nScriptTime &&
            (Globals::status->getCurrentCampaignMission() == 0x2a ||
             (this->level->objectivesA != nullptr && !this->level->objectivesA->isSurvivalObjective()))) {
            if (this->dialogueWindow == nullptr) {
                this->dialogueWindow = new DialogueWindow();
                if (this->level != nullptr) this->dialogueWindow->setLevel(this->level);
            } else if (!this->dialogueWindow->hasLevel() && this->level != nullptr) {
                this->dialogueWindow->setLevel(this->level);
            }
            this->dialogueWindow->set(Globals::status->getMission(), 2, -1);
            this->pauseOpen = 1;
            this->cutsceneActive = 1;
            pauseSounds();
        }
    }

    if (this->gameOverActive) {
        this->elapsedTime = 0;
        this->loadingTime = 0;
        Globals::sound->play(0x25, nullptr, nullptr, 0.0f);
    }
}


struct MGameAppData {
    uint8_t _pad0[5];
    uint8_t modalActive;
    uint8_t _pad6[6];
    uint8_t transitionActive;
};
#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(MGameAppData, modalActive) == 5, "MGameAppData::modalActive @ 5");
static_assert(offsetof(MGameAppData, transitionActive) == 0xc, "MGameAppData::transitionActive @ 0xc");
#endif

// Portrait descriptors used by MGame dialogues, 5 ints per face:
// +0 = station greeter, +5 = mission-23 contact, +10 = planet landing refusal / mission 24.
static int gMGameFaces[15] = {4, 2, 0, 2, 2, 0, 2, 3, 3, 10, 10, 0, 0, 1, 0};

static const int gShipRace[64] = {
    3, 0, 8, 3, 2, 0, 3, 0, 9, 1, 0, 8, 2, 0, 0, 0,
    2, 0, 2, 3, 3, 2, 0, 8, 8, 8, 0, 0, 0, 8, 3, 2,
    8, 0, 0, 2, 0, 0, 0, 1, 0, 1, 1, 2, 1, 3, 3, 3,
    3, 1, 1, 0, 8, 1, 1, 0, 3, 2, 0, 0, 8, 1, 3, 1};


void MGame::OnTouchBegin(int x, int y, void *touch) { // lint: void_ptr -- exported override signature (Pv)
    if (this->activeTouchId == nullptr)
        this->activeTouchId = touch;
    if (this->pauseOpen) {
        if (this->starMapOpen) {
            if (Globals::layout->choiceWindowOpen)
                Globals::layout->OnTouchBegin(x, y);
            else
                this->starMapOpen = this->starMap->OnTouchBegin(x, y) ^ 1;
            return;
        }
        if (this->autopilotMenuOpen || this->choiceWindowOpen || this->field_0xc4) {
            this->choiceWindow->OnTouchBegin(x, y);
            return;
        }
        if (this->cutsceneActive) {
            this->dialogueWindow->OnTouchBegin(x, y);
            return;
        }
        if (this->menuTouchOpen) {
            MGameAppData *appData = (MGameAppData *) Globals::appManager->GetApplicationData();
            if (appData->modalActive || appData->transitionActive)
                return;
            this->menuWindow->OnTouchBegin(x, y, touch);
            if (!this->freeCamMode || this->menuWindow->isShowingMessage() || this->menuWindow->isMakingScreenshot())
                return;
            freeCamTouchBegin(x, y, touch);
            return;
        }
    } else if (this->gameOverActive && this->loadingTime >= 4000) {
        if (Globals::status->getCurrentCampaignMission() == 158) {
            this->active = 0;
            this->applicationManager->SetCurrentApplicationModule(2);
            return;
        }
        if (this->gameRecord != nullptr) {
            this->gameRecord->load();
            Globals::globals->playMusicAndFadeOutCurrent(0);
            this->active = 0;
            this->applicationManager->SetCurrentApplicationModule(5);
            return;
        }
        Globals::globals->playMusicAndFadeOutCurrent(2);
        this->active = 0;
        this->applicationManager->SetCurrentApplicationModule(1);
    }

    this->hudTouchFlags = this->hud->touchBegin(x, y, touch);
    unsigned flags = this->hudTouchFlags;
    if ((flags & 0x10) && !this->jumpDriveActive) {
        // Dock / land / auto-pilot button.
        if (this->radar->lockedStation != nullptr && !Globals::status->inAlienOrbit() &&
            !this->player->isDockedToDockingPoint()) {
            if (!this->player->isAutoPilot() && !this->player->isInTurretMode()) {
                this->player->setAutoPilot(this->radar->lockedStation);
                if (this->radar->lockedStation == (*this->level->getLandmarks())[0]) {
                    this->hud->hudEvent(10, this->player, 0);
                } else {
                    this->hud->hudEvent(this->radar->lockedStation == (*this->level->getLandmarks())[3] ? 15 : 12,
                                        this->player, 0);
                }
                this->hudTouchFlags = this->hud->touchEnd(x, y, touch);
                return;
            }
            if (!this->player->isInTurretMode()) {
                this->hud->hudEvent(6, this->player, 0);
                this->player->setAutoPilot(nullptr);
                this->radar->lockedStation = nullptr;
                this->radar->field_0x28 = 0;
                this->player->resetGunDelay();
            }
        } else if (this->radar->field_0x14 && !this->player->isDockingToPlanet() &&
                   !this->player->isDockedToDockingPoint()) {
            if (Globals::status->getCurrentCampaignMission() < 10 ||
                Globals::status->getCurrentCampaignMission() == 48) {
                this->hud->hudEvent(21, this->player, 0);
                return;
            }
            if (Globals::status->getCurrentCampaignMission() == 24 &&
                this->radar->getPlanetDockIndex() ==
                ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation() &&
                (Globals::status->getShip()->getFirstEquipmentOfSort(13) == nullptr ||
                 Globals::status->getShip()->getFirstEquipmentOfSort(17) == nullptr)) {
                this->pauseOpen = 1;
                pauseSounds();
                delete this->dialogueWindow;
                this->dialogueWindow = nullptr;
                this->dialogueWindow = new DialogueWindow(Globals::gameText->getText(532),
                                                          Globals::gameText->getText(1603), gMGameFaces + 10);
                this->cutsceneActive = 1;
                return;
            }
            if (Globals::status->getCurrentCampaignMission() == 135 &&
                this->radar->getPlanetDockIndex() ==
                ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation() &&
                Globals::status->getShip()->getFirstEquipmentOfSort(19) == nullptr) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr)
                    this->choiceWindow = new ChoiceWindow();
                this->choiceWindow->set(*Globals::gameText->getText(3213));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                return;
            }
            if (Globals::status->getCurrentCampaignMission() == 91 &&
                this->radar->getPlanetDockIndex() ==
                ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation() &&
                Globals::status->getShip()->getMaxPassengers() <= 9) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr)
                    this->choiceWindow = new ChoiceWindow();
                this->choiceWindow->set(*Globals::gameText->getText(3214));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                return;
            }
            if (Globals::status->getCurrentCampaignMission() == 94 &&
                this->radar->getPlanetDockIndex() ==
                ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation() &&
                Globals::status->getShip()->getMaxPassengers() == 0) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr)
                    this->choiceWindow = new ChoiceWindow();
                this->choiceWindow->set(*Globals::gameText->getText(3214));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                return;
            }
            if (Globals::status->getCurrentCampaignMission() == 105 &&
                this->radar->getPlanetDockIndex() ==
                ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation() &&
                !Globals::status->getShip()->hasEquipment(206, 1)) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr)
                    this->choiceWindow = new ChoiceWindow();
                this->choiceWindow->set(*Globals::gameText->getText(3217));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                return;
            }
            if (Globals::status->getCurrentCampaignMission() == 139 &&
                this->radar->getPlanetDockIndex() ==
                ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation() &&
                Globals::status->getShip()->getIndex() != 42 &&
                (gShipRace[Globals::status->getShip()->getIndex()] != 1 ||
                 !Globals::status->getShip()->hasEquipment(190, 1))) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr)
                    this->choiceWindow = new ChoiceWindow();
                this->choiceWindow->set(*Globals::gameText->getText(3215));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                return;
            }
            if (Globals::status->getCurrentCampaignMission() == 142 &&
                this->radar->getPlanetDockIndex() ==
                ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation() &&
                (Globals::status->getShip()->getFirstEquipmentOfSort(33) == nullptr ||
                 !Globals::status->getShip()->hasEquipment(197, 15) ||
                 Globals::status->getShip()->getFirstEquipmentOfSort(35) == nullptr)) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr)
                    this->choiceWindow = new ChoiceWindow();
                this->choiceWindow->set(*Globals::gameText->getText(3216));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                return;
            }
            if (Globals::status->getCurrentCampaignMission() == 142 &&
                this->radar->getPlanetDockIndex() ==
                ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation() &&
                Globals::status->getShip()->getFreeSpace() == 0) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr)
                    this->choiceWindow = new ChoiceWindow();
                this->choiceWindow->set(*Globals::gameText->getText(3218));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                return;
            }
            this->camera->setLookAtCam(true);
            this->player->dockToPlanet();
            this->field_0x110 = 0;
            this->_b5c = 0;
            this->hud->enableFireForTutorial(false);
            reinterpret_cast<uint8_t *>(&this->levelScript->m_nFlags)[1] = 1;
            this->jumpActive = 1;
            return;
        } else if (!this->player->isMining() && !this->player->isDockedToDockingPoint()) {
            if (this->radar->getLockedAsteroid() != nullptr && !this->player->isDockingToAsteroid()) {
                if (Globals::status->getShip()->getFreeSpace() <= 0) {
                    this->hud->hudEvent(27, this->player, 0);
                    return;
                }
                this->hud->hudEvent(11, this->player, 0);
                this->player->dockToAsteroid(this->radar->getLockedAsteroid(), this->radar);
                this->hud->releaseAllKeys();
                return;
            }
            if (this->player->isDockingToAsteroid()) {
                this->hud->hudEvent(6, this->player, 0);
                this->player->dockToAsteroid(this->radar->getLockedAsteroid(), this->radar);
                this->hud->releaseAllKeys();
                return;
            }
            this->player->dockToAsteroid(this->radar->getLockedAsteroid(), this->radar);
        }

        if (this->player->isMining()) {
            Globals::status->field_124 = 0;
            this->player->stopMining();
            return;
        }
        if ((this->radar->lockedEnemy != nullptr && this->radar->lockedEnemy->field_0x70) ||
            this->player->isDockedToDockingPoint()) {
            if (this->radar->lockedEnemy != nullptr && !this->player->isDockingToDockingPoint() &&
                !this->player->isDockedToDockingPoint() && this->radar->lockedEnemy->field_0x75) {
                this->hud->hudEvent(34, this->player, 0);
                this->player->dockToDockingPoint(this->radar->lockedEnemy, this->radar);
                this->hud->releaseAllKeys();
            } else if (this->player->isDockingToDockingPoint() ||
                       (this->player->isDockedToDockingPoint() && !this->player->isInTurretMode())) {
                if (this->player->isDockedToDockingPoint()) {
                    this->player->dockToDockingPoint(this->radar->lockedEnemy, this->radar);
                    this->player->setAutoPilot(nullptr);
                    this->player->resetGunDelay();
                    this->hud->releaseAllKeys();
                } else if (this->player->isDockingToDockingPoint() && !this->player->isLandingOrTakingOff()) {
                    this->hud->hudEvent(6, this->player, 0);
                }
            }
            this->hud->enableFireForTutorial(false);
            long long now = Globals::status->getPlayingTime();
            if (now - this->lastTapTime < 250) {
                this->_b5c = 1;
                this->hud->enableFireForTutorial(true);
            } else {
                if (this->_b5c)
                    this->hudTouchFlags = this->hud->touchEnd(x, y, touch);
                this->_b5c = 0;
            }
            this->lastTapTime = now;
            return;
        }
        this->hud->enableFireForTutorial(false);
        long long now = Globals::status->getPlayingTime();
        if (now - this->lastTapTime < 250) {
            this->_b5c = 1;
            this->hud->enableFireForTutorial(true);
        } else {
            if (this->_b5c)
                this->hudTouchFlags = this->hud->touchEnd(x, y, touch);
            this->_b5c = 0;
        }
        this->lastTapTime = now;
        this->field_0x110 = 1;
        return;
    }
    if (flags & 0x20) {
        long long now = Globals::status->getPlayingTime();
        if (now - this->lastAlignTime < 500)
            this->player->alignToHorizon();
        this->lastAlignTime = now;
        if (this->jumpActive || this->activeTouchId == touch || this->hudTouchFlags != 32)
            return;
        goto beginThrust;
    }
    if (flags & 1) {
        Globals::sound->play(124, nullptr, nullptr, 0.0f);
        return;
    }
    if ((flags & 0x100) &&
        (this->player->isAutoPilot() || this->player->isDockingToAsteroid() ||
         this->player->isDockingToDockingPoint()) &&
        !this->player->field_0x14->field_0x54 && !this->player->aboutToReachAutoTarget()) {
        this->flFastForwardWeight = 5.0f;
        this->camera->setFastForwardMode(true);
        return;
    }
    if (this->cloakAttribute >= 1 && this->timeWarpState >= 0 && (flags & 0x100)) {
        if (this->timeWarpState < 1) {
            this->hud->setTimeExtender(true, false, true, true);
            Globals::sound->setDownPitch(true);
            this->timeWarpState = this->cloakAttribute;
            Globals::sound->play(1120, nullptr, nullptr, 0.0f);
        } else {
            this->timeWarpState = -1;
            Globals::sound->setDownPitch(false);
            this->hud->setTimeExtender(true, false, false, false);
            Globals::sound->play(1119, nullptr, nullptr, 0.0f);
        }
        return;
    }
    if (this->cameraMode == 3 && !this->player->isMining() && !this->jumpActive) {
        freeCamTouchBegin(x, y, touch);
        return;
    }
    if ((unsigned) this->cameraMode > 1)
        return;
    this->maneuverActive = 1;
    this->maneuverStartX = x;
    this->maneuverStartY = y;
    this->maneuverHoldTime = 0;
    if (this->jumpActive || this->hudTouchFlags != 0)
        return;
beginThrust:
    this->_b1b8 = 1;
    this->thrustStartX = (float) x;
    this->thrustStartY = (float) y;
}

// Fake touch id used when keyboard / mouse input drives the free camera.
static inline void *keyboardTouchId() { return reinterpret_cast<void *>(12345); } // lint: void_ptr -- OnTouch* touch handle type

// Campaign missions after which the "your wingmen are leaving" radio messages are no longer due.
static const int gWingmenRadioMissions[3] = {93, 111, 143};

void MGame::OnUpdate() {
    int delta = ((int) this->applicationManager->GetElapsedTimeMillis() > 150
                     ? 150
                     : (int) this->applicationManager->GetElapsedTimeMillis()) < 0
                    ? 0
                    : ((int) this->applicationManager->GetElapsedTimeMillis() > 150
                           ? 150
                           : (int) this->applicationManager->GetElapsedTimeMillis());
    this->deltaTime = delta;
    this->frameTime += delta;
    reinterpret_cast<long long &>(this->field_0x38) += delta;

    // Keyboard control of the free camera (arrow keys emulate a finger drag).
    if (this->freeCamMode && GetKeyState((char *) "Right")) {
        freeCamTouchBegin(400, 400, keyboardTouchId());
        freeCamTouchMove(396, 400, keyboardTouchId());
        freeCamTouchEnd(400, 400, keyboardTouchId());
    } else if (this->freeCamMode && GetKeyState((char *) "Left")) {
        freeCamTouchBegin(400, 400, keyboardTouchId());
        freeCamTouchMove(404, 400, keyboardTouchId());
        freeCamTouchEnd(400, 400, keyboardTouchId());
    }
    if (this->freeCamMode && GetKeyState((char *) "Down")) {
        freeCamTouchBegin(400, 400, keyboardTouchId());
        freeCamTouchMove(400, 404, keyboardTouchId());
        freeCamTouchEnd(400, 400, keyboardTouchId());
    } else if (this->freeCamMode && GetKeyState((char *) "Up")) {
        freeCamTouchBegin(400, 400, keyboardTouchId());
        freeCamTouchMove(400, 396, keyboardTouchId());
        freeCamTouchEnd(400, 400, keyboardTouchId());
    }

    // Mouse drag of the free camera.
    int mouseX = Globals::mouseDeltaX;
    int mouseY = Globals::mouseDeltaY;
    if (this->cameraMode == 3 && Globals::mouseCursorActivated && !Globals::is_hacking_visible &&
        (mouseX | mouseY) != 0) {
        int lastX = this->lastMouseX;
        int lastY = this->lastMouseY;
        freeCamTouchBegin(400, 400, keyboardTouchId());
        freeCamTouchMove(400 + (mouseX - lastX) / 2, 400 + (mouseY - lastY) / 2, keyboardTouchId());
        freeCamTouchEnd(400, 400, keyboardTouchId());
        mouseX = 0;
        mouseY = 0;
        Globals::mouseDeltaX = 0;
        Globals::mouseDeltaY = 0;
    }
    this->lastMouseX = mouseX;
    this->lastMouseY = mouseY;
    if (this->player->isDockingToAsteroid()) {
        Globals::mouseDeltaX = 0;
        Globals::mouseDeltaY = 0;
    }

    if (Globals::showWingmanMenu) {
        if (!this->player->isMining() && Globals::status->getWingmen() != 0) {
            if (this->hudMenuOpen) {
                this->hudMenuOpen = 0;
                this->hud->closeHudMenu();
                this->pauseOpen = 0;
                resumeSounds();
            } else {
                this->hudMenuOpen = 1;
                this->pauseOpen = !this->pauseOpen;
                pauseSounds();
                this->hud->initHudMenu(2, this->level);
            }
        }
        Globals::showWingmanMenu = 0;
    }

    if (this->pauseOpen != this->pauseSnapshot) {
        if (this->pauseOpen) {
            this->player->PauseEngineSound();
            Array<KIPlayer *> *enemies = this->level->getEnemies();
            if (enemies != nullptr) {
                for (unsigned i = 0; i < enemies->size(); i++) {
                    (*enemies)[i]->PauseEngineSound();
                }
            }
        } else {
            this->player->ResumeEngineSound();
            Array<KIPlayer *> *enemies = this->level->getEnemies();
            if (enemies != nullptr) {
                for (unsigned i = 0; i < enemies->size(); i++) {
                    (*enemies)[i]->ResumeEngineSound();
                }
            }
        }
        this->pauseSnapshot = this->pauseOpen;
    }

    // Mouse-wheel zoom of the free camera.
    if ((!this->pauseOpen ||
         (this->freeCamMode && !this->menuWindow->isShowingMessage() && !this->menuWindow->isMakingScreenshot())) &&
        this->cameraMode == 3) {
        if (this->freeCamMode) {
            this->level->update(0, this->jumpActive);
        }
        float wheel = static_cast<Engine *>(this->applicationManager->GetEngine())->wheelDelta;
        if ((wheel > 0.0f ? wheel : -wheel) > 0.2f) {
            this->flCameraRoll += wheel * -50.0f;
            if (this->flCameraRoll > 20000.0f) {
                this->flCameraRoll = 20000.0f;
            } else if (this->flCameraRoll < 1500.0f) {
                this->flCameraRoll = 1500.0f;
            }
            this->camera->zoomTarget(this->flCameraRoll);
        }
        static_cast<Engine *>(this->applicationManager->GetEngine())->wheelDelta *= 0.9f;
    }

    if (!this->pauseOpen) {
        // Time extender (slow motion) countdown.
        if (this->timeWarpState != 0) {
            if (this->timeWarpState >= 0) {
                if (this->timeWarpState <= this->deltaTime) {
                    Globals::sound->setDownPitch(false);
                    this->hud->setTimeExtender(true, false, false, false);
                    Globals::sound->play(1119, nullptr, nullptr, 0.0f);
                }
                this->timeWarpState -= this->deltaTime;
                if (this->timeWarpState == 0) {
                    this->timeWarpState = -1;
                } else if (this->timeWarpState > 0) {
                    int realDelta = this->deltaTime;
                    this->deltaTime = (int) ((float) realDelta * 0.3f);
                    this->field_0x44 = (int) ((float) realDelta * 0.7f);
                    this->hud->setTimeExtender(true, false, true, true);
                }
            } else {
                this->timeWarpState -= this->deltaTime;
                if (this->timeWarpState < -this->cloakAttributeMax) {
                    this->timeWarpState = 0;
                    this->hud->setTimeExtender(true, true, true, false);
                }
            }
        }

        Globals::status->incPlayingTime(this->deltaTime);

        if (this->radar->field_0x54 || this->player->aboutToReachAutoTarget() || this->radio->isShowingMessage()) {
            this->flFastForwardWeight = 1.0f;
            this->camera->setFastForwardMode(false);
            this->player->field_0x84 = 1;
        } else {
            this->deltaTime = (int) (this->flFastForwardWeight * (float) this->deltaTime);
        }

        int scriptTimeBefore = (int) this->levelScript->m_nScriptTime;
        this->levelScript->m_nScriptTime += this->deltaTime;
        if (scriptTimeBefore <= 5000 && this->levelScript->m_nScriptTime > 5000 && this->player->hasVolatileGoods()) {
            this->hud->hudEvent(45, this->player, 0);
        }

        if (!this->jumpActive) {
            this->elapsedTime += this->deltaTime;
            if (this->cameraMode != 3 && !this->player->isDockingToAsteroid() && !this->player->isDockedToAsteroid()) {
                float wheelThrust = static_cast<Engine *>(this->applicationManager->GetEngine())->wheelDelta;
                if (wheelThrust >= 1.0f || wheelThrust <= -1.0f) {
                    this->field_0x1b4 += wheelThrust;
                    static_cast<Engine *>(this->applicationManager->GetEngine())->wheelDelta = 0.0f;
                    float capped = this->field_0x1b4 < 100.0f ? this->field_0x1b4 : 100.0f;
                    this->field_0x1b4 = capped < 0.0f ? 0.0f : (this->field_0x1b4 < 100.0f ? this->field_0x1b4 : 100.0f);
                    this->player->setThrust(this->field_0x1b4 / 100.0f);
                    reinterpret_cast<float &>(this->field_0x1b0) = 1.0f - this->field_0x1b4 / 100.0f;
                    this->player->throttleChanged();
                }
            }
            if ((this->player->isDockingToAsteroid() || this->player->isDockedToAsteroid()) &&
                this->player->getThrust() < 1.0f) {
                this->field_0x1b4 = 100.0f;
                this->player->setThrust(1.0f);
                static_cast<Engine *>(this->applicationManager->GetEngine())->wheelDelta = 0.0f;
            }
            if (this->player->isDockedToAsteroid() && this->cameraMode != 0) {
                switchCamera(0);
            }
        }

        Globals::layout->update(this->deltaTime);
        Globals::status->fadeValue -= this->deltaTime;
        if (!Globals::options[15]) {
            Globals::sound->updateAll(nullptr, nullptr, nullptr, nullptr);
        }

        bool wasDockedToMiningPlant = this->player->isDockedToMiningPlant();
        if (this->loadingTime < 4000) {
            this->player->update(this->timeWarpState > 0 ? this->field_0x44 : this->deltaTime, this->radar, this->hud,
                                 this->radio, this->levelScript, this->field_0xd8, this->jumpActive, this->cameraMode);
        }
        bool wasInRocketControl = this->player->isInRocketControl();
        this->level->update(this->deltaTime, this->jumpActive);
        if (wasInRocketControl && !this->player->isInRocketControl()) {
            switchCamera(0);
        }

        // Mining plant reached: offer to sell the mined ore.
        if (!wasDockedToMiningPlant && this->player->isDockedToMiningPlant() && !this->choiceWindowOpen) {
            if (this->choiceWindow == nullptr) {
                this->choiceWindow = new ChoiceWindow();
            }
            String text = *Globals::gameText->getText(290) + String("\n");
            int fee = Globals::status->hardCoreMode() ? 100 : 30;
            text = Globals::status->replaceHash(String(text, false), String(fee), String("#"));
            Array<Item *> *cargo = Globals::status->getShip()->getCargo();
            bool hasOre = false;
            if (cargo != nullptr) {
                for (unsigned i = 0; i < cargo->size(); i++) {
                    Item *item = (*cargo)[i];
                    if (item != nullptr && item->getSort() == 23) {
                        text += String("\n") + String(item->getAmount()) + String("t ") +
                                *Globals::gameText->getText(1274 + item->getIndex());
                        hasOre = true;
                    }
                }
            }
            if (!hasOre) {
                text += String("\n") + *Globals::gameText->getText(286);
            }
            this->choiceWindow->set(text, true);
            this->needsRedraw = 1;
            this->pauseOpen = 1;
            pauseSounds();
            this->_bca = 1;
            this->choiceWindowOpen = 1;
        }
    }

    // 3D sound listener follows the camera.
    if (Globals::options[15]) {
        AbyssEngine::AEMath::Matrix camMatrix = *reinterpret_cast<AbyssEngine::AEMath::Matrix *>(
            this->paintCanvas->CameraGetLocal(this->paintCanvas->CameraGetCurrent()));
        Vector &lastCameraPos = reinterpret_cast<Vector &>(this->field_0x198);
        Vector velocity = (float) Player::velocity * (AbyssEngine::AEMath::MatrixGetPosition(camMatrix) - lastCameraPos) /
                          (float) this->deltaTime;
        Vector forward = -AbyssEngine::AEMath::MatrixGetDir(camMatrix);
        Vector position = AbyssEngine::AEMath::MatrixGetPosition(camMatrix);
        Vector up = AbyssEngine::AEMath::MatrixGetUp(camMatrix);
        Globals::sound->updateAll(&position, &forward, &up, &velocity);
        lastCameraPos = AbyssEngine::AEMath::MatrixGetPosition(camMatrix);
    }

    if (!this->gameOverActive && this->player->getHitpoints() > 0) {
        if (this->pauseOpen) {
            if (this->menuWindow != nullptr && !this->hudMenuOpen && !this->cutsceneActive && !this->_bc8 &&
                !this->autopilotMenuOpen && !this->field_0xc6 && !this->choiceWindowOpen && !this->_bca) {
                this->menuWindow->update(this->deltaTime);
                if (this->freeCamMode) {
                    this->camera->setRumblePercentage(0.0f, 0);
                    this->menuTime += this->deltaTime;
                    if (!this->freeCamDragging) {
                        this->flShakePhaseX = this->flShakeAmpX * this->flShakePhaseX;
                        if ((this->flShakePhaseX > 0.0f ? this->flShakePhaseX : -this->flShakePhaseX) > 1.0f) {
                            this->flShakeX += this->flShakePhaseX;
                        }
                        this->flShakePhaseY = this->flShakeAmpY * this->flShakePhaseY;
                        if ((this->flShakePhaseY > 0.0f ? this->flShakePhaseY : -this->flShakePhaseY) > 1.0f) {
                            this->flShakeY += this->flShakePhaseY;
                            if (this->flShakeY > 200.0f) {
                                this->flShakeY = 200.0f;
                            } else if (this->flShakeY < -200.0f) {
                                this->flShakeY = -200.0f;
                            }
                        }
                    }
                    this->camera->rotateAroundTarget(this->flShakeY * -0.005f, this->flShakeX * -0.005f, 0.0f);
                    this->camera->update(this->deltaTime);
                }
                if (this->menuWindow->isInMissionScreen()) {
                    Globals::status->incPlayingTime(this->deltaTime);
                }
            }
            if (this->active && !this->_bc8 && !this->autopilotMenuOpen && !this->field_0xc6) {
                if (this->choiceWindowOpen) {
                    this->choiceWindow->update(this->deltaTime);
                } else if (!this->hudMenuOpen) {
                    if (this->starMapOpen) {
                        Globals::status->incPlayingTime(this->deltaTime);
                        this->starMap->update(this->deltaTime);
                    } else if (this->cutsceneActive) {
                        Globals::layout->update(this->deltaTime);
                        this->dialogueWindow->update(this->deltaTime);
                    }
                }
            }
            return;
        }

        // Landing on the planet of mission 24 without the required equipment.
        if (this->player->isDockingToPlanet() && Globals::status->getCurrentCampaignMission() == 24 &&
            this->radar->getPlanetDockIndex() ==
            ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation() &&
            (Globals::status->getShip()->getFirstEquipmentOfSort(13) == nullptr ||
             Globals::status->getShip()->getFirstEquipmentOfSort(17) == nullptr)) {
            this->pauseOpen = 1;
            pauseSounds();
            if (this->dialogueWindow != nullptr) {
                delete this->dialogueWindow;
                this->dialogueWindow = nullptr;
            }
            this->dialogueWindow = new DialogueWindow(Globals::gameText->getText(532), Globals::gameText->getText(1603),
                                                      gMGameFaces + 10);
            this->cutsceneActive = 1;
            this->player->stopPlanetDock();
            this->player->setAutoPilot(nullptr);
            return;
        }

        if (this->player->isDockedToPlanet()) {
            Globals::status->departStation(
                (Station *) (intptr_t) Globals::galaxy->getStation(this->radar->getPlanetDockIndex()));
            Level::setInitStreamOut();
            Globals::status->field_64 = this->player->player->getHitpoints();
            Globals::status->field_5c = this->player->player->getShieldHP();
            Globals::status->field_60 = this->player->player->getArmorHP();
            Globals::status->field_68 = this->player->player->getGammaHP();
            Globals::status->field_f4 = this->player->getCurrentSecondaryWeaponIndex();
            Globals::switch_to_target_setting = 1;
            this->active = 0;
            this->applicationManager->SetCurrentApplicationModule(2);
            return;
        }

        if (this->player->shouldSwitchToStandardCam()) {
            switchCamera(0);
        }
        if (this->player->shouldSwitchToFreeLookCam()) {
            switchCamera(3);
            this->flShakeX = -98.0f;
            this->flShakeY = 35.0f;
            this->flCameraRoll = 3800.0f;
            this->camera->rotateAroundTarget(-0.175f, 0.49f, 0.0f);
            this->camera->zoomTarget(this->flCameraRoll);
        }

        if (this->jumpDriveActive) {
            if (updateJumpScene()) {
                return;
            }
        } else if (this->player->isChargingDrive() && this->player->driveReady()) {
            startJumpScene();
            return;
        }

        if ((this->levelScript->startSequenceOver() || !this->levelScript->startSequence()) &&
            !this->autopilotMenuOpen && !this->field_0xc6) {
            if (dockEvent(0, 0) || this->starMapOpen) {
                return;
            }
        }

        if (this->elapsedTime > 5000 && this->levelScript->m_nScriptTime > 5000 && !this->jumpDriveActive &&
            !this->cutsceneActive) {
            // Story contacts that call in when the player idles for an hour.
            if (Globals::status->getCurrentCampaignMission() == 23 && !this->player->isMining() &&
                Globals::status->getMission()->isEmpty() &&
                Globals::status->getPlayingTime() - reinterpret_cast<long long &>(Globals::status->field_0x100) >
                3600000) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->dialogueWindow != nullptr) {
                    delete this->dialogueWindow;
                    this->dialogueWindow = nullptr;
                }
                int variant = Globals::rnd->nextInt(2);
                this->dialogueWindow = new DialogueWindow(Globals::gameText->getText(variant == 0 ? 533 : 534),
                                                          Globals::gameText->getText(1602), gMGameFaces + 5);
                Globals::sound->play(variant == 0 ? 464 : 465, nullptr, nullptr, 0.0f);
                this->needsRedraw = 1;
                this->cutsceneActive = 1;
                reinterpret_cast<long long &>(Globals::status->field_0x100) = Globals::status->getPlayingTime();
            } else if (Globals::status->getCurrentCampaignMission() == 24 && !this->player->isMining() &&
                       Globals::status->getMission()->isEmpty() &&
                       Globals::status->getPlayingTime() - reinterpret_cast<long long &>(Globals::status->field_0x100) >
                       3600000) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->dialogueWindow != nullptr) {
                    delete this->dialogueWindow;
                    this->dialogueWindow = nullptr;
                }
                int variant = Globals::rnd->nextInt(2);
                this->dialogueWindow = new DialogueWindow(Globals::gameText->getText(variant == 0 ? 535 : 536),
                                                          Globals::gameText->getText(1603), gMGameFaces + 10);
                Globals::sound->play(variant == 0 ? 467 : 468, nullptr, nullptr, 0.0f);
                this->needsRedraw = 1;
                this->cutsceneActive = 1;
                reinterpret_cast<long long &>(Globals::status->field_0x100) = Globals::status->getPlayingTime();
            } else if (Globals::status->getCurrentCampaignMission() >= 122 &&
                       Globals::status->getCurrentCampaignMission() <= 124 && !this->player->isMining() &&
                       Globals::status->getMission()->isEmpty() &&
                       Globals::status->getPlayingTime() - reinterpret_cast<long long &>(Globals::status->field_0x100) >
                       3600000 &&
                       !this->jumpDriveActive && !this->usingJumpDrive) {
                this->level->createRadioMessage(
                    28, Globals::rnd->nextInt(2) + (Globals::status->getCurrentCampaignMission() == 122 ? 3164 : 3166));
                reinterpret_cast<long long &>(Globals::status->field_0x100) = Globals::status->getPlayingTime();
            } else if (!Globals::status->inAlienOrbit() && Globals::status->field_0x17c &&
                       Globals::status->getPlayingTime() - reinterpret_cast<long long &>(Globals::status->field_0x100) >
                       12000 &&
                       !this->jumpDriveActive && !this->usingJumpDrive) {
                int messageIndex = -1;
                for (int i = 0; i < 3; i++) {
                    if (Globals::status->getCurrentCampaignMission() >= gWingmenRadioMissions[i]) {
                        messageIndex++;
                    }
                }
                if (messageIndex >= 0 && messageIndex < 3) {
                    Globals::status->field_0x17c = 0;
                    this->level->createRadioMessage(27, messageIndex);
                }
            }

            // One-time tutorial hints.
            if (!Globals::hints[23] && Globals::status->getShip()->hasBooster() &&
                Globals::status->getCurrentCampaignMission() >= 2) {
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                String hint = Globals::globals->replaceKeyBindingTokens(*Globals::gameText->getText(596));
                this->choiceWindow->set(hint);
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                this->pauseOpen = 1;
                pauseSounds();
                Globals::hints[23] = 1;
                return;
            }
            if (!Globals::hints[33] && Globals::status->getShip()->hasJumpDrive()) {
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                String hint = Globals::globals->replaceKeyBindingTokens(*Globals::gameText->getText(590));
                this->choiceWindow->set(hint);
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                this->pauseOpen = 1;
                pauseSounds();
                Globals::hints[33] = 1;
                return;
            }
            if (!Globals::hints[34] && Globals::status->getShip()->hasCloak()) {
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                String hint = Globals::globals->replaceKeyBindingTokens(*Globals::gameText->getText(593));
                this->choiceWindow->set(hint);
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                this->pauseOpen = 1;
                pauseSounds();
                Globals::hints[34] = 1;
                return;
            }
            if (!Globals::hints[32] && !this->player->isAutoPilot() &&
                Globals::status->getCurrentCampaignMission() >= 10 && Globals::status->getMission()->isEmpty()) {
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                String hint = Globals::globals->replaceKeyBindingTokens(*Globals::gameText->getText(584));
                this->choiceWindow->set(hint);
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                this->pauseOpen = 1;
                pauseSounds();
                Globals::hints[32] = 1;
                return;
            }
            if (Globals::status->wingmen != nullptr && !Globals::hints[21]) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                String hint = Globals::globals->replaceKeyBindingTokens(*Globals::gameText->getText(637));
                this->choiceWindow->set(hint);
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                Globals::hints[21] = 1;
                return;
            }
            if (!Globals::hints[28] &&
                ((Standing *) (intptr_t) Globals::status->getStanding())->isEnemyWithAnyone()) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                this->choiceWindow->set(*Globals::gameText->getText(648));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                Globals::hints[28] = 1;
                return;
            }
            if (!this->field_0x1e6 && Globals::status->getCurrentCampaignMission() == 5) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                this->choiceWindow->set(*Globals::gameText->getText(619));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                this->field_0x1e6 = 1;
                return;
            }
            if (!Globals::hints[17] && !this->player->isMining() && !this->field_0x1e5 &&
                Globals::status->getCurrentCampaignMission() == 2 &&
                (this->player->player->getHitpoints() < this->player->player->getMaxHitpoints() ||
                 this->levelScript->m_nScriptTime > 40000)) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                String hint(*Globals::gameText->getText(1725), false);
                hint = Globals::globals->replaceKeyBindingTokens(hint);
                this->choiceWindow->set(hint);
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                this->field_0x1e5 = 1;
                return;
            }
            if (!Globals::hints[17] && this->player->isMining() && !Globals::hints[17]) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                this->choiceWindow->set(*Globals::gameText->getText(620));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                Globals::hints[17] = 1;
                return;
            }
            if (!Globals::hints[18] && this->player->isMining() && Globals::status->getCurrentCampaignMission() >= 4) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                this->choiceWindow->set(*Globals::gameText->getText(621));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                Globals::hints[18] = 1;
                return;
            }
            if (Globals::status->getCurrentCampaignMission() == 2 && !Globals::hints[55] &&
                this->player->lostMiningGame()) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                this->choiceWindow->set(*Globals::gameText->getText(617));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                Globals::hints[55] = 1;
                return;
            }
            if (!Globals::hints[20] && this->hud->cargoFull() && Globals::status->getCurrentCampaignMission() >= 7) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                this->choiceWindow->set(*Globals::gameText->getText(636));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                Globals::hints[20] = 1;
                return;
            }
            if (!Globals::hints[35] && !Globals::status->inAlienOrbit() &&
                ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 2) {
                this->level->createRadioMessage(5, 0);
                Globals::hints[35] = 1;
                this->field_0xe0 = 1;
                return;
            }
            if (Globals::hints[35] && !Globals::hints[36] && !this->field_0xe0 && !Globals::status->inAlienOrbit() &&
                ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 2) {
                this->level->createRadioMessage(6, 0);
                Globals::hints[36] = 1;
                return;
            }
            if (!Globals::hints[37] && Globals::status->getShip()->getFirstEquipmentOfSort(13) != nullptr &&
                !this->player->isMining() && Globals::status->getMission()->isEmpty() &&
                Globals::status->getCurrentCampaignMission() == 24) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->dialogueWindow != nullptr) {
                    delete this->dialogueWindow;
                    this->dialogueWindow = nullptr;
                }
                this->dialogueWindow = new DialogueWindow(Globals::gameText->getText(1912),
                                                          Globals::gameText->getText(1603), gMGameFaces + 10);
                Globals::sound->play(463, nullptr, nullptr, 0.0f);
                this->needsRedraw = 1;
                this->cutsceneActive = 1;
                Globals::hints[37] = 1;
                return;
            }
            if (!Globals::hints[40] && this->player->isHacking()) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                this->choiceWindow->set(*Globals::gameText->getText(599));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                Globals::hints[40] = 1;
                return;
            }
            if (!Globals::hints[41] &&
                ((Globals::status->getCurrentCampaignMission() == 91 &&
                  Globals::status->getStation()->getIndex() == 110 &&
                  (*this->level->getMessages())[4]->isOver()) ||
                 (Globals::status->getCurrentCampaignMission() != 91 &&
                  Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
                                                              Globals::status->getCurrentCampaignMission()) > 0.0f))) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                this->choiceWindow->set(*Globals::gameText->getText(600));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                Globals::hints[41] = 1;
                return;
            }
            if (!Globals::hints[44] && this->player->hasVolatileGoods()) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                this->choiceWindow->set(*Globals::gameText->getText(602));
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                Globals::hints[44] = 1;
                return;
            }
            if (!Globals::hints[46] && Globals::status->getCurrentCampaignMission() == 91 &&
                Globals::status->getStation()->getIndex() == 110 && (*this->level->getMessages())[4]->isOver()) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                String hint = Globals::globals->replaceKeyBindingTokens(*Globals::gameText->getText(603));
                this->choiceWindow->set(hint);
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                Globals::hints[46] = 1;
                return;
            }
            if (!Globals::hints[47] && Globals::status->getCurrentCampaignMission() == 91 &&
                Globals::status->getStation()->getIndex() == 110 && this->player->isDockedToDockingPoint() &&
                Globals::status->field_178 == 10 && (*this->level->getMessages())[6]->isOver()) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                String hint = Globals::globals->replaceKeyBindingTokens(*Globals::gameText->getText(606));
                this->choiceWindow->set(hint);
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                Globals::hints[47] = 1;
                return;
            }
            if (!Globals::hints[48] && Globals::status->getCurrentCampaignMission() == 92 &&
                Globals::status->getStation()->getIndex() == 113 && this->player->isDockedToDockingPoint() &&
                Globals::status->field_178 == 0) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                String hint = Globals::globals->replaceKeyBindingTokens(*Globals::gameText->getText(609));
                this->choiceWindow->set(hint);
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                Globals::hints[48] = 1;
                return;
            }
            // Campaign finished: hand out the follow-up missions.
            if (Globals::options[53] && !Globals::hints[38] && !this->cutsceneActive && !this->player->isMining() &&
                Globals::status->getMission()->isEmpty() && !this->player->isAutoPilot() &&
                Globals::status->gameWon()) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->dialogueWindow != nullptr) {
                    delete this->dialogueWindow;
                    this->dialogueWindow = nullptr;
                }
                this->dialogueWindow = new DialogueWindow();
                this->dialogueWindow->set(new Mission(160, 0, -1), 1, 46);
                this->needsRedraw = 1;
                this->cutsceneActive = 1;
                Globals::hints[38] = 1;
                Globals::status->nextCampaignMission(true);
                Globals::status->nextCampaignMission(true);
                return;
            }
            if (Globals::options[55] && !Globals::hints[49] && !this->cutsceneActive && !this->player->isMining() &&
                Globals::status->getMission()->isEmpty() && !this->player->isAutoPilot() &&
                Globals::status->dlc1Won()) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->dialogueWindow != nullptr) {
                    delete this->dialogueWindow;
                    this->dialogueWindow = nullptr;
                }
                this->dialogueWindow = new DialogueWindow();
                this->dialogueWindow->set(new Mission(160, 0, -1), 1, 85);
                this->needsRedraw = 1;
                this->cutsceneActive = 1;
                Globals::hints[49] = 1;
                Globals::status->nextCampaignMission(true);
                Globals::status->nextCampaignMission(true);
                return;
            }
            // Black market: offer to buy the whole cargo.
            if (Globals::status->inBlackMarketSystem() && !Globals::status->field_110 && !Globals::status->field_0x111 &&
                !this->field_0xd4 && this->level->getMessages() != nullptr &&
                (*this->level->getMessages())[0]->isOver()) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->choiceWindow == nullptr) {
                    this->choiceWindow = new ChoiceWindow();
                }
                this->choiceItemCount = 0;
                Array<Item *> *cargo = Globals::status->getShip()->getCargo();
                if (cargo != nullptr) {
                    for (unsigned i = 0; i < cargo->size(); i++) {
                        if ((*cargo)[i] != nullptr) {
                            this->choiceItemCount += (*cargo)[i]->getAmount() * (*cargo)[i]->getSinglePrice();
                        }
                    }
                }
                if (this->choiceItemCount == 0) {
                    this->choiceItemCount = 100;
                }
                float difficulty = reinterpret_cast<const float &>(Globals::options[44]);
                float share;
                if (difficulty <= 0.0f) {
                    share = 0.02f;
                } else if (difficulty == 0.5f) {
                    share = 0.05f;
                } else {
                    share = difficulty == 1.0f ? 0.1f : 0.2f;
                }
                this->choiceItemCount = (int) (share * (float) this->choiceItemCount);
                String offer = Globals::status->replaceHash(String(*Globals::gameText->getText(448), false),
                                                            Globals::layout->formatCredits(this->choiceItemCount),
                                                            String("#C"));
                offer = Globals::status->replaceHash(String(offer, false), String((int) (share * 100.0f)),
                                                     String("#P"));
                this->choiceWindow->set(offer, true);
                this->needsRedraw = 1;
                this->choiceWindowOpen = 1;
                this->field_0xcf = 1;
                this->field_0xd4 = 1;
            } else if (!Globals::status->field_114 && !Globals::status->inAlienOrbit() &&
                       Globals::status->getStation()->getIndex() == 108) {
                this->pauseOpen = 1;
                pauseSounds();
                if (this->dialogueWindow != nullptr) {
                    delete this->dialogueWindow;
                    this->dialogueWindow = nullptr;
                }
                this->dialogueWindow = new DialogueWindow(Globals::gameText->getText(457),
                                                          Globals::gameText->getText(1601), gMGameFaces);
                Globals::sound->play(1468, nullptr, nullptr, 0.0f);
                this->needsRedraw = 1;
                this->cutsceneActive = 1;
                Globals::status->field_114 = 1;
                return;
            }
        }

        if (this->elapsedTime > 5000 && !this->cutsceneActive && !Globals::status->getMission()->hasFailed() &&
            !Globals::status->getMission()->hasWon()) {
            dialogueEvent();
        }
    }

    // Boost: radial blur around the ship and widened field of view.
    if (this->player->boosting() && !this->player->isDockingToPlanet()) {
        static_cast<Engine *>(this->applicationManager->GetEngine())->SetPostEffect(0x1400002, true);
        float boost = this->player->getBoostPercentage();
        static_cast<Engine *>(this->applicationManager->GetEngine())->blurStrength = boost > 0.0f ? boost : 0.0f;
        Vector screen = {0.0f, 0.0f, 0.0f};
        Globals::Canvas->GetScreenPosition(this->player->getPosition(), screen);
        screen[0] /= (float) (int) static_cast<Engine *>(this->applicationManager->GetEngine())->GetDisplayWidth();
        screen[1] = 1.0f - screen[1] / (float) (int) static_cast<Engine *>(this->applicationManager->GetEngine())->GetDisplayHeight();
        static_cast<Engine *>(this->applicationManager->GetEngine())->field_0x3cc = screen;
        float fov = 1.22f + this->player->getBoostPercentage() * 0.35f;
        reinterpret_cast<float &>(this->field_0x70) = fov;
        Globals::Canvas->CameraSetPerspective(
            this->cameraId, fov, 20.0f,
            Globals::status->inAlienOrbit() ? (Globals::status->getCurrentCampaignMission() < 80 ? 450000.0f : 300000.0f)
                                            : 300000.0f);
        this->camera->setBoostPercentage(
            this->player->getBoostPercentage() < 0.0f ? 0.0f : this->player->getBoostPercentage(),
            this->player->getBoostSpeed());
    }

    if (this->gameOverActive) {
        if (this->elapsedTime > 3000) {
            this->loadingTime += this->deltaTime;
        }
    } else {
        if (!this->player->isDead() && !Globals::status->getMission()->hasFailed() && successCheck()) {
            return;
        }
        gameOverCheck();
    }

    bool wasCinematic = this->jumpActive;
    this->jumpActive = this->levelScript->process(this->deltaTime);
    if (!wasCinematic && this->jumpActive) {
        // Cinematic started: reset input state and the boost effects.
        this->field_0x1dd = AbyssEngine::Engine::UseAdvancedShader;
        AbyssEngine::Engine::UseAdvancedShader = 1;
        if (this->timeWarpState > 0) {
            Globals::sound->setDownPitch(false);
            this->hud->setTimeExtender(this->hud->field_0x0, false, false, false);
        }
        this->touch0Id = 0;
        this->touch1Id = 0;
        this->menuTime = 0;
        this->freeCamDragging = 0;
        this->_b5c = 0;
        this->field_0x110 = 0;
        this->hud->enableFireForTutorial(false);
        if (this->player->boosting()) {
            this->player->stopBoost();
        }
        reinterpret_cast<float &>(this->field_0x70) = 1.22f;
        Globals::Canvas->CameraSetPerspective(
            this->cameraId, 1.22f, 20.0f,
            Globals::status->inAlienOrbit() ? (Globals::status->getCurrentCampaignMission() < 80 ? 450000.0f : 300000.0f)
                                            : 300000.0f);
        this->camera->setBoostPercentage(0.0f, 0);
        static_cast<Engine *>(this->applicationManager->GetEngine())->SetPostEffect(0x1400002, false);
    } else if (wasCinematic && !this->jumpActive) {
        AbyssEngine::Engine::UseAdvancedShader = this->field_0x1dd;
    }

    uint8_t *scriptFlags = reinterpret_cast<uint8_t *>(&this->levelScript->field_0x12);
    if (scriptFlags[1]) {
        return;
    }
    if (scriptFlags[0]) {
        this->elapsedTime = 5001;
    }

    if (this->cameraMode == 3) {
        if (!this->freeCamDragging) {
            this->flShakePhaseX = this->flShakeAmpX * this->flShakePhaseX;
            if ((this->flShakePhaseX > 0.0f ? this->flShakePhaseX : -this->flShakePhaseX) > 1.0f) {
                this->flShakeX += this->flShakePhaseX;
            }
            this->flShakePhaseY = this->flShakeAmpY * this->flShakePhaseY;
            if ((this->flShakePhaseY > 0.0f ? this->flShakePhaseY : -this->flShakePhaseY) > 1.0f) {
                this->flShakeY += this->flShakePhaseY;
            }
            if (this->flShakeY > 200.0f) {
                this->flShakeY = 200.0f;
            } else if (this->flShakeY < -200.0f) {
                this->flShakeY = -200.0f;
            }
        }
        this->camera->rotateAroundTarget(this->flShakeY * -0.005f, this->flShakeX * -0.005f, 0.0f);
    }

    if (this->flFastForwardWeight > 1.0f) {
        for (int i = 0; i < 5; i++) {
            this->camera->update(
                (int) (this->timeWarpState > 0 ? this->field_0x44 : this->deltaTime / this->flFastForwardWeight));
        }
    } else {
        this->camera->update(this->timeWarpState > 0 ? this->field_0x44 : this->deltaTime);
    }

    if (this->maneuverActive) {
        this->maneuverHoldTime += this->deltaTime;
    }

    uint8_t &reverbOn = reinterpret_cast<uint8_t *>(&this->field_0x1a4)[1];
    if (this->cameraMode == 2) {
        this->camera->setFirstPersonMatrix(
            reinterpret_cast<AbyssEngine::AEMath::Matrix &>(this->player->player->transform));
        if (!reverbOn && this->camera->hideShipForFirstPersonCam()) {
            reverbOn = 1;
            Globals::sound->enableReverb(1);
        }
    } else if (reverbOn && !this->camera->hideShipForFirstPersonCam()) {
        Globals::sound->disableReverb();
        reverbOn = 0;
    }
    this->player->hideShipForFirstPersonCameraView(this->field_0x18 ? true
                                                                    : this->camera->hideShipForFirstPersonCam());

    if (!this->gameOverActive && !this->jumpActive && !this->player->isDead()) {
        if (Level::doInstantJump && this->levelScript->m_nScriptTime > 5000) {
            Level::doInstantJump = 0;
            this->usingJumpDrive = 1;
            startChargingJumpDrive();
        }
        if ((this->_b5c || this->field_0x110 || this->hud->firePressed()) && !this->player->isDockingToPlanet()) {
            this->player->shoot(this->deltaTime, 0);
            this->menuOpen = 1;
        } else if (this->menuOpen) {
            this->menuOpen = 0;
            this->player->stopShooting(0);
        }
        if (Globals::status->getCurrentCampaignMission() != 48) {
            if (Globals::options[17]) {
                if (this->hud->getAnalogX() < 0.0f) {
                    this->player->left(this->timeWarpState > 0 ? this->field_0x44 : this->deltaTime,
                                       this->hud->getAnalogX() * this->hud->getAnalogX());
                } else if (this->hud->getAnalogX() > 0.0f) {
                    this->player->right(this->timeWarpState > 0 ? this->field_0x44 : this->deltaTime,
                                        this->hud->getAnalogX() * this->hud->getAnalogX());
                }
                if (Globals::options[16]) {
                    if (this->hud->getAnalogY() < 0.0f) {
                        this->player->down(this->timeWarpState > 0 ? this->field_0x44 : this->deltaTime,
                                           this->hud->getAnalogY() * this->hud->getAnalogY());
                    } else if (this->hud->getAnalogY() > 0.0f) {
                        this->player->up(this->timeWarpState > 0 ? this->field_0x44 : this->deltaTime,
                                         this->hud->getAnalogY() * this->hud->getAnalogY());
                    }
                } else {
                    if (this->hud->getAnalogY() < 0.0f) {
                        this->player->up(this->timeWarpState > 0 ? this->field_0x44 : this->deltaTime,
                                         this->hud->getAnalogY() * this->hud->getAnalogY());
                    } else if (this->hud->getAnalogY() > 0.0f) {
                        this->player->down(this->timeWarpState > 0 ? this->field_0x44 : this->deltaTime,
                                           this->hud->getAnalogY() * this->hud->getAnalogY());
                    }
                }
            } else {
                handleAccelerometer();
            }
        }
    }
    this->field_0x110 = 0;

    // Flying into a wormhole ends the mission and leaves the system.
    if (this->player->isInWormhole()) {
        if (Globals::status->getMission()->isCampaignMission()) {
            int mission = Globals::status->getCurrentCampaignMission();
            if (mission == 29 || mission == 41 || (mission == 40 && this->levelScript->getEvent() <= 3)) {
                this->player->player->setHitpoints(0);
                return;
            }
            if (mission != 42) {
                Globals::status->nextCampaignMission(true);
            }
            if (mission == 40) {
                Level::lastMissionFreighterHitpoints = (*this->level->getEnemies())[0]->player->getHitpoints();
            }
            this->level->removeObjectives();
            Globals::status->setMission(Mission::empty);
        }
        Globals::status->setMission(Mission::empty);
        if (Globals::status->inAlienOrbit()) {
            if (Globals::status->getCurrentCampaignMission() == 42) {
                return;
            }
            Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(Globals::status->field_84));
        } else {
            Globals::status->setStation(Globals::status->playerStation);
            Globals::status->departStation(Globals::status->playerStation);
        }
        Globals::status->field_64 = this->player->player->getHitpoints();
        Globals::status->field_5c = this->player->player->getShieldHP();
        Globals::status->field_60 = this->player->player->getArmorHP();
        Globals::status->field_68 = this->player->player->getGammaHP();
        Globals::status->field_f4 = this->player->getCurrentSecondaryWeaponIndex();
        Level::comingFromAlienWorld = 1;
        Level::initStreamOutPosition = 1;
        Level::programmedStation = nullptr;
        Globals::switch_to_target_setting = 1;
        this->applicationManager->SetCurrentApplicationModule(2);
        this->active = 0;
    }
}

static FModSound **g_fmod = &Globals::sound;

void MGame::OnSuspend() {
    RecordHandler *record = Globals::recordHandler;
    if (record != 0)
        record->saveOptions();
    ((MGame *) (this))->pauseSounds();
    if (this->pauseOpen == 0) {
        if (this->menuWindow == 0)
            this->menuWindow = new MenuTouchWindow(1);
        this->pauseOpen = 1;
        this->pauseSnapshot = 1;
        ((FModSound *) (*g_fmod))->pauseAllPlaying();
        this->player->PauseEngineSound();
        Array<KIPlayer *> *e = this->level->getEnemies();
        if (e != nullptr) {
            for (uint32_t i = 0; i < e->size(); i++)
                ((KIPlayer *) ((*e)[i]))->PauseEngineSound();
        }
        MenuTouchWindow *w = this->menuWindow;
        int mode = 1;
        if (this->jumpActive == 0)
            mode = this->player->isDead();
        ((MenuTouchWindow *) (w))->setCutsceneMode(mode);
        this->menuTouchOpen = 1;
    }
    this->hud->releaseAllKeys();
}

static inline int Station_getIndex(Station *s) { return s->getIndex(); }

static int *g_deAutoFlag;

static int g_dePostEffect;

static int g_deTextA;

static int g_deLitA0;

static int g_deLitA1;

static int g_deTextB;

static int g_deLitB0;

static int g_deLitB1;

static int **g_deAlienFlag;

bool MGame::dockEvent(int p1, int p2) {
    (void) p1;
    (void) p2;
    this->touchesStream = this->level->collideStream(this->player->getPosition());
    this->touchesStation = this->level->collideStation(this->player->getPosition());

    if (!Globals::status->getMission()->isEmpty() && Globals::status->getMission()->getType() != 11 &&
        Globals::status->getMission()->getType() != 0 && Globals::status->getMission()->getType() != 189 &&
        Globals::status->getMission()->getType() != 171 && Globals::status->getMission()->getType() != 172) {
        // Docking is not allowed during this mission.
        if ((this->touchesStation || this->touchesStream) && this->player->isAutoPilot()) {
            this->hud->hudEvent(21, this->player, 0);
        }
        return false;
    }

    if (this->touchesStream) {
        if (this->player->goingToStream() && !this->player->isDockingToStream() &&
            !this->player->isDockedToStream()) {
            this->player->dockToStream(true);
            this->freeCamDragging = 0;
            this->needsRedraw = 1;
            return false;
        }
    }
    if (this->touchesStream) {
        Globals::status->field_64 = this->player->player->getHitpoints();
        Globals::status->field_5c = this->player->player->getShieldHP();
        Globals::status->field_60 = this->player->player->getArmorHP();
        Globals::status->field_68 = this->player->player->getGammaHP();
        Globals::status->field_f4 = this->player->getCurrentSecondaryWeaponIndex();
        bool autoPilot = this->player->isAutoPilot();
        if (Level::programmedStation != nullptr && autoPilot) {
            // Auto pilot reached a jump stream: confirm the programmed destination.
            if (this->autopilotMenuOpen) {
                return false;
            }
            this->freeCamDragging = 0;
            this->needsRedraw = 1;
            if (this->choiceWindow == nullptr) {
                this->choiceWindow = new ChoiceWindow();
            }
            this->choiceWindow->set(*Globals::gameText->getText(574) + String(": ") +
                                    Level::programmedStation->getName() + String("\n") +
                                    *Globals::gameText->getText(421), true);
            this->choiceWindow->left();
            this->pauseOpen = 1;
            this->autopilotMenuOpen = 1;
            pauseSounds();
            this->player->setAutoPilot(nullptr);
            return false;
        }
        if (this->starMapOpen) {
            this->frameTime = 0;
            return true;
        }
        this->player->isAutoPilot();
        if (!this->player->goingToStream()) {
            return false;
        }
        if (Level::programmedStation == nullptr) {
            // No destination programmed yet: open the jump map.
            this->freeCamDragging = 0;
            this->needsRedraw = 1;
            if (this->starMap == nullptr) {
                this->starMap = new StarMap(false, nullptr, false, -1);
            }
            static_cast<Engine *>(this->applicationManager->GetEngine())->SetPostEffect(0x1400002, false);
            this->starMap->initLights();
            this->starMap->setJumpMapMode(true, false);
            this->player->setAutoPilot(nullptr);
            this->pauseOpen = 1;
            this->starMapOpen = 1;
            pauseSounds();
            this->needsRedraw = 1;
            return true;
        }
        if (this->autopilotMenuOpen) {
            return false;
        }
        this->freeCamDragging = 0;
        this->needsRedraw = 1;
        if (this->choiceWindow == nullptr) {
            this->choiceWindow = new ChoiceWindow();
        }
        this->choiceWindow->set(*Globals::gameText->getText(574) + String(": ") +
                                Level::programmedStation->getName() + String("\n") +
                                *Globals::gameText->getText(421), true);
        this->choiceWindow->left();
        this->pauseOpen = 1;
        this->autopilotMenuOpen = 1;
        pauseSounds();
        this->player->setAutoPilot(nullptr);
        return false;
    }

    if (!this->touchesStation) {
        if (this->player->getAutoPilotTarget() != (int) (intptr_t) (*this->level->getLandmarks())[0] ||
            !this->player->collidesWithStation()) {
            return false;
        }
    }
    if (Globals::status->getCurrentCampaignMission() >= 49 && Globals::status->getCurrentCampaignMission() <= 54 &&
        Globals::status->getStation()->getIndex() != 74) {
        this->hud->hudEvent(21, this->player, 0);
        return false;
    }
    if (!this->player->goingToStation() || Globals::status->inAlienOrbit()) {
        return false;
    }
    if (Globals::status->inEmptyOrbit()) {
        return false;
    }
    Globals::achievements->checkForNewMedal(this->player);
    Globals::switch_to_target_setting = 0;
    Globals::status->field_64 = this->player->player->getHitpoints();
    Globals::status->field_5c = this->player->player->getShieldHP();
    Globals::status->field_60 = this->player->player->getArmorHP();
    Globals::status->field_68 = this->player->player->getGammaHP();
    this->applicationManager->SetCurrentApplicationModule(5);
    this->active = 0;
    return true;
}

void MGame::freeCamTouchEnd(int p1, int p2, void *idPtr) { // lint: void_ptr -- exported method signature (Pv)
    // lint: void_ptr -- exported method signature (Pv)
    int id = (int) (intptr_t) idPtr;
    (void) p1;
    (void) p2;
    if (this->touch0Id == id) {
        this->touch0Id = 0;
        this->menuTime = 0;
    } else if (this->touch1Id == id) {
        this->touch1Id = 0;
        this->menuTime = 0;
    }
    {
        int ix = this->dragDeltaX;
        int iy = this->dragDeltaY;
        float fx = (float) ix;
        float fy = (float) iy;
        float nx = this->flShakeX + fx;
        float ny = this->flShakeY + fy;
        if (iy < 0) iy = -iy;
        if (ix < 0) ix = -ix;

        float ox = 0.001f;
        if (ix > 3) ox = fx;
        float oy = 0.001f;
        if (iy > 3) oy = fy;
        this->needsRedraw = 1;
        this->flShakeAmpX = 0.002f;
        this->flShakeAmpY = 0.002f;
        this->freeCamDragging = 0;
        this->flShakePhaseX = ox;
        this->flShakePhaseY = oy;
        this->flShakeX = nx;
        this->flShakeY = ny;
        this->dragRotIntX = (int) nx;
        this->dragRotIntY = (int) ny;
    }
}





void MGame::UseKhadorDrive() {
    if (this->player->isChargingDrive()) return;

    Mission *m = Globals::status->getMission();
    if (m->isEmpty() || Globals::status->getCurrentCampaignMission() == 0x4e || m->getType() == 0xb ||
        m->getType() == 0 || m->getType() == 0xbd || m->getType() == 0xd || m->getType() == 0xab ||
        m->getType() == 0xac) {
        if (Globals::status->getCurrentCampaignMission() != 0x41 || Globals::status->inAlienOrbit() ||
            Globals::status->getStation()->getIndex() !=
                ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStation()) {
            goto jump;
        }
    }
    this->hud->hudEvent(0x15, this->level->getPlayer(), 0);
    return;

jump:
    this->player->resetGunDelay();
    if (Globals::status->getCurrentCampaignMission() == 0x4e) {
        Level::programmedStation = Globals::status->playerStation;
        this->usingJumpDrive = 1;
        startChargingJumpDrive();
        this->pauseOpen = 0;
        resumeSounds();
        this->hudMenuOpen = 0;
        this->hud->closeHudMenu();
        Globals::status->nextCampaignMission(true);
        return;
    }

    if (Globals::status->inAlienOrbit()) {
        if (Globals::status->getCurrentCampaignMission() == 0x50)
            Globals::status->field_84 = 100;
        Level::programmedStation = (Station *) (intptr_t) Globals::galaxy->getStation(Globals::status->field_84);
        this->usingJumpDrive = 1;
        startChargingJumpDrive();
        this->pauseOpen = 0;
        resumeSounds();
        this->hudMenuOpen = 0;
        this->hud->closeHudMenu();
        return;
    }

    if (this->player->hasVolatileGoods()) {
        this->choiceWindow->set(*Globals::gameText->getText(0x264));
        this->choiceWindowOpen = 1;
        this->pauseOpen = 1;
        this->needsRedraw = 1;
        this->hudMenuOpen = 0;
        this->hud->closeHudMenu();
        pauseSounds();
        this->maneuverActive = 0;
        return;
    }
    if (this->starMap == nullptr)
        this->starMap = new StarMap(false, nullptr, false, -1);
    static_cast<Engine *>(this->applicationManager->GetEngine())->SetPostEffect(0x01400002, false);
    this->starMap->initLights();
    this->usingJumpDrive = 1;
    this->starMap->setJumpMapMode(true, true);
    if (!Globals::status->inAlienOrbit())
        this->starMap->askForJumpIntoAlienWorld();
    this->starMapOpen = 1;
    this->pauseOpen = 1;
    pauseSounds();
    this->hudMenuOpen = 0;
    this->hud->closeHudMenu();
}

static int g_initParticleFlag;

struct ParticleSystemGlobal {
    uint8_t _pad0[0xf];
    uint8_t particlesEnabled;
};
#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(ParticleSystemGlobal, particlesEnabled) == 0xf,
              "ParticleSystemGlobal::particlesEnabled @ 0xf");
#endif

static int *g_initEngineSnd;

static int *g_initMusicArmed;

static int *g_initMusicTrack;

static int **g_initFmod;

static unsigned g_initPostEffect;

static int g_initStationMask;

static String **g_initInfoFont;

static int g_initInfoTextKey;

static int *g_initInfoWidth;

// Globals::options as read by MGame (engine sound toggle @0x0f, effect quality @0x28).
struct GameSettingsView {
    uint8_t _pad0[0x0f];
    uint8_t engineSoundEnabled;
    uint8_t _pad10[0x28 - 0x10];
    float effectQuality;
};

int MGame::OnInitialize() {
    this->loadProgress = 100;
    this->field_0x1dc = 0;
    if (this->level == nullptr) {
        unsigned int cubeTexture;
        if (Globals::status->inAlienOrbit())
            this->paintCanvas->TextureCreate(12040, cubeTexture, false);
        else
            this->paintCanvas->TextureCreate(
                (unsigned short) (((SolarSystem *) (intptr_t) Globals::status->getSystem())->getTextureIndex() + 12030),
                cubeTexture, false);
        this->paintCanvas->ChangeCubeTexture(cubeTexture);

        Globals::globals->startNewSoundResourceList();
        Globals::globals->addSoundResourceToList(102);
        Globals::globals->addSoundResourceToList(104);
        Globals::globals->addSoundResourceToList(105);
        Globals::globals->addSoundResourceToList(106);
        Globals::globals->addSoundResourceToList(107);
        Globals::globals->addSoundResourceToList(103);
        Globals::globals->addSoundResourceToList(126);
        Globals::globals->addSoundResourceToList(5);
        Globals::globals->addSoundResourceToList(24);
        Globals::globals->addSoundResourceToList(21);
        Globals::globals->addSoundResourceToList(18);
        Globals::globals->addSoundResourceToList(19);
        Globals::globals->addSoundResourceToList(20);
        Globals::globals->addSoundResourceToList(28);
        Globals::globals->addSoundResourceToList(29);
        Globals::globals->addSoundResourceToList(27);
        Globals::globals->addSoundResourceToList(37);
        Globals::globals->addSoundResourceToList(26);
        Globals::globals->addSoundResourceToList(46);
        Globals::globals->addSoundResourceToList(47);
        if (Globals::status->getWingmen())
            Globals::globals->addSoundResourceToList(48);
        Globals::globals->addSoundResourceToList(62);
        Globals::globals->addSoundResourceToList(61);
        Globals::globals->addSoundResourceToList(36);
        if (Globals::status->getCurrentCampaignMission() <= 1) {
            Globals::globals->addSoundResourceToList(156);
            Globals::globals->addSoundResourceToList(157);
        }
        if (!Globals::status->inAlienOrbit() &&
            ((SolarSystem *) (intptr_t) Globals::status->getSystem())->currentOrbitHasWarpGate())
            Globals::globals->addSoundResourceToList(31);
        if (Globals::status->getCurrentCampaignMission() == 0) {
            Globals::globals->addSoundResourceToList(143);
            Globals::globals->addSoundResourceToList(157);
            Globals::globals->addSoundResourceToList(158);
            Globals::globals->addSoundResourceToList(161);
            Globals::globals->addSoundResourceToList(160);
            Globals::globals->addSoundResourceToList(159);
        } else if (Globals::status->getCurrentCampaignMission() == 14) {
            Globals::globals->addSoundResourceToList(15);
        } else if (Globals::status->getCurrentCampaignMission() == 24) {
            Globals::globals->addSoundResourceToList(34);
        } else if (Globals::status->getCurrentCampaignMission() == 29) {
            Globals::globals->addSoundResourceToList(14);
        } else if (Globals::status->getCurrentCampaignMission() == 41) {
            Globals::globals->addSoundResourceToList(155);
            Globals::globals->addSoundResourceToList(153);
            Globals::globals->addSoundResourceToList(154);
        }
        Globals::status->checkForLevelUp();
        this->level = new Level(3);
    }
    if (!this->level->init())
        return 100;

    reset();
    if (Globals::status->field_64 >= 0)
        this->player->player->setHitpoints(Globals::status->field_64);
    if (Globals::status->field_5c >= 0)
        this->player->player->setShieldHP(Globals::status->field_5c);
    if (Globals::status->field_60 >= 0)
        this->player->player->setArmorHP(Globals::status->field_60);
    if (Globals::status->field_68 >= 0)
        this->player->player->setGammaHP(Globals::status->field_68);
    this->player->resetLastHP();
    if (Globals::status->getCurrentCampaignMission() != 95) {
        Globals::status->field_64 = Globals::status->getShip()->getMaxHP();
        Globals::status->field_5c = Globals::status->getShip()->getMaxShieldHP();
        Globals::status->field_60 = Globals::status->getShip()->getMaxArmorHP();
        Globals::status->field_68 = 100;
        if (Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
                                                        Globals::status->getCurrentCampaignMission()) == 0.0f)
            this->player->player->setGammaHP(100);
    }
    if (!Globals::status->inAlienOrbit())
        Globals::status->field_84 = Globals::status->getStation()->getIndex();

    this->startTime = this->lastTime = Globals::appManager->GetCurrentTimeMillis();
    this->timeWarpState = 0;
    this->cloakAttribute = 0;
    if (this->radar->hasScanner())
        Globals::status->field_11c = 0;
    Globals::status->field_134 = 0;
    Globals::status->field_12c = 0;
    Globals::status->field_13c = 0;
    Globals::status->field_144 = 0;
    Item *timeExtender = Globals::status->getShip()->getFirstEquipmentOfSort(26);
    if (timeExtender != nullptr) {
        this->cloakAttribute = timeExtender->getAttribute(42);
        this->cloakAttributeMax = timeExtender->getAttribute(43);
        this->hud->setTimeExtender(true, false, true, false);
    }

    if (Globals::status->dlc1Won() && Globals::status->inAlienOrbit() &&
        Globals::status->getCurrentCampaignMission() <= 146) {
        if (this->player != nullptr && this->radio != nullptr)
            this->player->radioRef = this->radio;
        this->level->createRadioMessage(8, 0);
    }
    if (Globals::status->inBlackMarketSystem()) {
        if (this->player != nullptr && this->radio != nullptr)
            this->player->radioRef = this->radio;
        if (Globals::status->field_110) {
            Array<KIPlayer *> *enemies = this->level->getEnemies();
            if (enemies != nullptr) {
                for (unsigned i = 0; i < enemies->size(); i++) {
                    if ((*enemies)[i]->shipGroup == 8)
                        (*enemies)[i]->field_0x25 = 0;
                }
            }
        } else if (Globals::status->field_0x111) {
            this->level->createRadioMessage(13, 8);
        } else {
            Array<KIPlayer *> *enemies = this->level->getEnemies();
            if (enemies != nullptr) {
                for (unsigned i = 0; i < enemies->size(); i++) {
                    if ((*enemies)[i]->shipGroup == 8)
                        (*enemies)[i]->field_0x25 = 0;
                }
            }
            this->level->createRadioMessage(9, 8);
        }
    } else {
        Globals::status->field_110 = 0;
        Globals::status->field_0x111 = 0;
    }

    if (!Globals::status->inAlienOrbit() && Globals::status->getCurrentCampaignMission() == 125 &&
        Globals::status->isFreighterMissionStation(Globals::status->getStation()->getIndex()) &&
        !(((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue() &
          (1 << Globals::status->getFreighterMissionStationBit(Globals::status->getStation()->getIndex())))) {
        Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
        campaign->setStatusValue(
            ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue() |
            (1 << Globals::status->getFreighterMissionStationBit(Globals::status->getStation()->getIndex())));
        if (this->player != nullptr && this->radio != nullptr)
            this->player->radioRef = this->radio;
        this->level->createRadioMessage(19, 0);
    }

    Array<Item *> *secondaries = Globals::status->getShip()->getEquipment(1);
    if (secondaries != nullptr) {
        if (Globals::status->getShip()->hasEquipment(Globals::status->field_f4, 1)) {
            for (unsigned i = 0; i < secondaries->size(); i++) {
                Item *weapon = (*secondaries)[i];
                if (weapon != nullptr && weapon->getIndex() == Globals::status->field_f4) {
                    this->level->getPlayer()->setCurrentSecondaryWeaponIndex(weapon->getIndex());
                    this->hud->setCurrentSecondaryWeapon(weapon);
                    break;
                }
            }
        } else if ((*secondaries)[0] != nullptr) {
            this->player->setCurrentSecondaryWeaponIndex((*secondaries)[0]->getIndex());
            this->hud->setCurrentSecondaryWeapon((*secondaries)[0]);
        }
    }

    this->field_0x1a4 = 0;
    GameSettingsView *settings = reinterpret_cast<GameSettingsView *>(Globals::options);
    if (settings->engineSoundEnabled) {
        this->player->PlayEngineSound();
        Array<KIPlayer *> *enemies = this->level->getEnemies();
        if (enemies != nullptr) {
            for (unsigned i = 0; i < enemies->size(); i++)
                (*enemies)[i]->PlayEngineSound();
        }
    } else if (Globals::status->getCurrentCampaignMission() >= 2) {
        Vector position = this->player->getPosition();
        Globals::sound->play(this->player->field_0x1c, &position, nullptr, 0.0f);
    }

    this->loadingTime = 0;
    this->level->getStarSystem()->initLight();
    this->level->enableParticleEffects(settings->effectQuality >= 0.25f, settings->effectQuality > 0.7f);
    float pitch = 1.0f - Globals::status->getShip()->getFireRateFactor();
    if (pitch >= 0.0f)
        this->player->pitchAllPrimaryGuns(pitch);

    if (!Globals::status->inAlienOrbit()) {
        int station = Globals::status->getStation()->getIndex();
        unsigned slot = (unsigned) (station - 109);
        bool visit;
        if (slot <= 25 && ((1u << slot) & 0x3800003)) // stations 109, 110, 132, 133, 134
            visit = true;
        else if (slot == 2) // station 111
            visit = Globals::status->getCurrentCampaignMission() >= 94;
        else
            visit = (unsigned) (station - 102) < 3;
        if (visit)
            Globals::status->getStation()->visit();
    }

    if (Globals::switch_to_target_setting != -1)
        Globals::globals->playMusicAndFadeOutCurrent(Globals::switch_to_target_setting);
    Globals::switch_to_target_setting = -1;
    Globals::sound->setDownPitch(false);
    static_cast<Engine *>(this->applicationManager->GetEngine())->SetPostEffect(0x01400002, false);
    if (Globals::status->getCurrentCampaignMission() == 158 && this->level->getEnemies() != nullptr)
        ((PlayerFighter *) (*this->level->getEnemies())[0])->cloak(1, true);
    if (this->menuWindow == nullptr)
        this->menuWindow = new MenuTouchWindow(1);
    this->missionInfoLines = new Array<String *>();
    Globals::globals->getLineArray(reinterpret_cast<unsigned int &>(Globals::font), *Globals::gameText->getText(196),
                                   Globals::w - Globals::layout->buttonInsetX * 2, this->missionInfoLines);
    this->active = 1;
    return 0;
}

static inline void TFC_zoomTarget(TargetFollowCamera *cam, float z) { cam->zoomTarget(z); }

static float g_fcRotScale;

static float g_fcZoomMax;

static float g_fcZoomMin;

void MGame::freeCamTouchMove(int x, int y, void *touch) { // lint: void_ptr -- exported method signature (Pv)
    int id = (int) (intptr_t) touch;
    if (this->player->isMining()) {
        this->needsRedraw = 1;
        freeCamTouchEnd(x, y, touch);
        return;
    }
    this->needsRedraw = 0;
    if (this->touch0Id != 0 && this->touch1Id != 0) {
    float oldLen = AbyssEngine::AEMath::VectorLength(this->freeCamFinger1 - this->freeCamFinger0);
    float newLen = oldLen;
    if (this->touch0Id == id) {
        float fx = (float) x;
        float fy = (float) y;
        newLen = AbyssEngine::AEMath::VectorLength(Vector{fx, fy, 0.0f} - this->freeCamFinger0);
        this->freeCamFinger1 = Vector{fx, fy, 0.0f};
    } else if (this->touch1Id == id) {
        float fx = (float) x;
        float fy = (float) y;
        newLen = AbyssEngine::AEMath::VectorLength(Vector{fx, fy, 0.0f} - this->freeCamFinger1);
        this->freeCamFinger0 = Vector{fx, fy, 0.0f};
    }
    this->flCameraRoll += (newLen - oldLen) * -50.0f;
    if (this->flCameraRoll > 20000.0f)
        this->flCameraRoll = 20000.0f;
    else if (this->flCameraRoll < 1500.0f)
        this->flCameraRoll = 1500.0f;
    this->camera->zoomTarget(this->flCameraRoll);
    } else {
        int dx = x - this->dragLastX;
        int dy = y - this->dragLastY;
        this->dragLastX = x;
        this->dragLastY = y;
        this->dragDeltaX = dx;
        this->dragDeltaY = dy;
        this->flShakeAmpX = 1.0f;
        this->flShakeAmpY = 1.0f;
        this->flShakeX += (float) dx;
        this->flShakeY += (float) dy;
        if (this->touch0Id == 0)
            this->freeCamFinger0 = Vector{(float) x, (float) y, 0.0f};
        else if (this->touch1Id == 0)
            this->freeCamFinger1 = Vector{(float) x, (float) y, 0.0f};
    }
}

void MGame::OnTouchEnd(int x, int y, void *touchId) { // lint: void_ptr -- exported override signature (Pv)
    if (this->activeTouchId == touchId) {
        this->activeTouchId = nullptr;
    }

    bool wasAutoPilot = this->player->isAutoPilot();
    this->flFastForwardWeight = 1.0f;
    this->camera->setFastForwardMode(false);
    this->player->field_0x84 = 1;

    if (this->pauseOpen) {
        if (this->choiceWindowOpen) {
            if (this->field_0xcf) {
                // Black market: sell the whole cargo?
                int choice = this->choiceWindow->OnTouchEnd(x, y);
                switch (choice) {
                    case 0: {
                    if (Globals::status->getCredits() < this->choiceItemCount) {
                        this->choiceWindow->set(
                            Globals::status->replaceHash(
                                String(*Globals::gameText->getText(203), false),
                                String(Globals::layout->formatCredits(this->choiceItemCount), false), String("#C")),
                            false);
                        this->field_0xcf = 0;
                        this->level->createRadioMessage(11, 8);
                        Array<KIPlayer *> *enemies = this->level->getEnemies();
                        if (enemies != nullptr) {
                            for (unsigned i = 0; i < enemies->size(); i++) {
                                KIPlayer *enemy = (*enemies)[i];
                                if (enemy->shipGroup == 8) {
                                    enemy->field_0x25 = 1;
                                }
                            }
                        }
                        Globals::status->field_0x111 = 1;
                        return;
                    }
                    Array<KIPlayer *> *enemies = this->level->getEnemies();
                    if (enemies != nullptr) {
                        for (unsigned i = 0; i < enemies->size(); i++) {
                            KIPlayer *enemy = (*enemies)[i];
                            if (enemy->shipGroup == 8) {
                                enemy->field_0x25 = 0;
                            }
                        }
                    }
                    Globals::status->changeCredits(-this->choiceItemCount);
                    this->level->createRadioMessage(10, 8);
                    Globals::status->field_110 = 1;
                    this->pauseOpen = 0;
                    this->choiceWindowOpen = 0;
                    this->field_0xcf = 0;
                    resumeSounds();
                        break;
                    }
                    case 1: {
                    Array<KIPlayer *> *enemies = this->level->getEnemies();
                    if (enemies != nullptr) {
                        for (unsigned i = 0; i < enemies->size(); i++) {
                            KIPlayer *enemy = (*enemies)[i];
                            if (enemy->shipGroup == 8) {
                                enemy->field_0x25 = 1;
                            }
                        }
                    }
                    Globals::status->field_0x111 = 1;
                    this->level->createRadioMessage(11, 8);
                    this->pauseOpen = 0;
                    this->choiceWindowOpen = 0;
                    this->field_0xcf = 0;
                    resumeSounds();
                        break;
                    }
                    default:
                        break;
                }
            } else if (this->_bca) {
                // Mining plant: refine the ore in the cargo hold.
                int choice = this->choiceWindow->OnTouchEnd(x, y);
                switch (choice) {
                    case 0: {
                    String report;
                    Ship *ship = Globals::status->getShip();
                    bool refinedAny = false;
                    for (int ore = 154; ore < 166; ore++) {
                        int refined = ore + 11;
                        if (ore == 165) {
                            refined = 218;
                            ore = 217;
                        }
                        int fee = Globals::status->hardCoreMode() ? 100 : 30;
                        int amount = 0;
                        while (ship->hasCargo(ore, fee)) {
                            ship->removeCargo(ore, fee);
                            ship->addCargo((*Globals::items)[refined]->makeItem(1));
                            amount--;
                            refinedAny = true;
                        }
                        if (amount != 0) {
                            report += String("\n") + String(-amount) + String("t ") +
                                      *Globals::gameText->getText(1274 + refined);
                        }
                    }
                    this->_bca = 0;
                    if (refinedAny) {
                        this->choiceWindow->set(*Globals::gameText->getText(291) + String("\n") + report);
                    } else {
                        this->choiceWindow->set(*Globals::gameText->getText(292));
                    }
                        break;
                    }
                    case 1: {
                    this->choiceWindowOpen = 0;
                    this->_bca = 0;
                    this->pauseOpen = 0;
                    resumeSounds();
                        break;
                    }
                    default:
                        break;
                }
            } else {
                bool supernovaChallenge = this->field_0x1e4;
                int choice = this->choiceWindow->OnTouchEnd(x, y);
                if (!supernovaChallenge) {
                    if (choice == 0) {
                        this->pauseOpen = 0;
                        this->choiceWindowOpen = 0;
                        resumeSounds();
                    }
                } else {
                    switch (choice) {
                        case 0:
                            if (this->menuWindow == nullptr) {
                                this->menuWindow = new MenuTouchWindow(1);
                            }
                            this->menuWindow->startSupernovaChallenge();
                            return;
                        case 1:
                            Globals::globals->playMusicAndFadeOutCurrent(2);
                            this->active = 0;
                            this->applicationManager->SetCurrentApplicationModule(1);
                            Globals::status->resetGame();
                            return;
                        case 2:
                            return;
                        default:
                            break;
                    }
                }
            }
        } else if (this->autopilotMenuOpen) {
            switch (this->choiceWindow->OnTouchEnd(x, y)) {
                case 0:
                    this->autopilotMenuOpen = 0;
                    if (this->player->isInTurretMode()) {
                        this->player->setTurretMode(false);
                    }
                    this->usingJumpDrive = 0;
                    startJumpScene();
                    this->player->resetGunDelay();
                    return;
                case 1:
                    // Pick another destination on the jump map.
                    this->autopilotMenuOpen = 0;
                    if (this->starMap == nullptr) {
                        this->starMap = new StarMap(false, nullptr, false, -1);
                    }
                    static_cast<Engine *>(this->applicationManager->GetEngine())->SetPostEffect(0x1400002, false);
                    this->starMap->initLights();
                    this->starMap->setJumpMapMode(true, false);
                    this->starMapOpen = 1;
                    this->pauseOpen = 1;
                    pauseSounds();
                    return;
                default:
                    return;
            }
        } else if (!this->field_0xc4) {
            if (this->starMapOpen) {
                if (Globals::layout->choiceWindowOpen) {
                    if (Globals::layout->OnTouchEnd(x, y)) {
                        Globals::layout->choiceWindowOpen = 0;
                    }
                    return;
                }
                if (!this->starMap->OnTouchEnd(x, y)) {
                    return;
                }
                this->level->getStarSystem()->initLight();
                this->pauseOpen = 0;
                this->starMapOpen = 0;
                resumeSounds();
                if (this->starMap->exitRequested) {
                    if (this->touchesStream) {
                        this->usingJumpDrive = 0;
                        startJumpScene();
                    } else if (!Level::doInstantJump) {
                        this->levelScript->setAutoPilotToProgrammedStation();
                    }
                } else if (this->touchesStream) {
                    // Jump cancelled while inside the stream: push the ship back out.
                    this->player->dockToStream(false);
                    this->player->setAutoPilot(nullptr);
                    this->player->field_0x4->setDirection((*this->level->getLandmarks())[1]->geometry->getDirection(),
                                                          Vector{0.0f, 1.0f, 0.0f});
                    this->player->setPosition((*this->level->getLandmarks())[1]->geometry->getPosition() +
                                              Vector{0.0f, 0.0f, 8000.0f});
                }
                delete this->starMap;
                this->starMap = nullptr;
                return;
            }
            if (this->cutsceneActive) {
                if (!this->dialogueWindow->OnTouchEnd(x, y)) {
                    return;
                }
                this->pauseOpen = 0;
                resumeSounds();
                this->cutsceneActive = 0;
                if (Globals::status->getMission()->hasFailed()) {
                    if (Globals::status->getMission()->isCampaignMission() ||
                        Globals::status->getCurrentCampaignMission() == 42) {
                        Globals::globals->playMusicAndFadeOutCurrent(2);
                        this->active = 0;
                        this->applicationManager->SetCurrentApplicationModule(1);
                        Globals::status->resetGame();
                        return;
                    }
                    // Freelance mission failed: take back what was handed out.
                    if (Globals::status->getMission()->getType() == 12) {
                        Globals::status->changeCredits(-Globals::status->getMission()->getReward());
                    } else if (Globals::status->getMission()->getType() == 3 ||
                               Globals::status->getMission()->getType() == 5 ||
                               Globals::status->getMission()->getType() == 11) {
                        Array<Item *> *cargo = Globals::status->getShip()->getCargo();
                        if (cargo != nullptr) {
                            for (unsigned i = 0; i < cargo->size(); i++) {
                                if ((*cargo)[i]->isUnsaleable() &&
                                    ((*cargo)[i]->getIndex() == 116 || (*cargo)[i]->getIndex() == 117)) {
                                    Globals::status->getShip()->removeCargo((*cargo)[i]);
                                    break;
                                }
                            }
                        }
                    }
                    Globals::status->setFreelanceMission(Mission::empty);
                    this->level->removeObjectives();
                    this->levelScript->m_nTimeLimit = 0;
                    Globals::status->setMission(Mission::empty);
                    this->player->setRoute(nullptr);
                    if (this->player->goingToWaypoint()) {
                        this->player->setAutoPilot(nullptr);
                    }
                    this->player->removeRoute();
                    this->level->setPlayerRoute(nullptr);
                } else if (Globals::status->getMission()->hasWon() ||
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon()) {
                    bool keepObjectives = ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon()
                                              ? !Globals::status->getMission()->isCampaignMission()
                                              : false;
                    Mission *mission = Globals::status->getMission()->hasWon() ? Globals::status->getMission()
                                                                                : ((Mission *) (intptr_t) Globals::status->getCampaignMission());
                    if (mission->isInstantActionMission()) {
                        Globals::switch_to_target_setting = 2;
                        this->active = 0;
                        this->applicationManager->SetCurrentApplicationModule(1);
                        return;
                    }
                    if (mission->isCampaignMission()) {
                        Globals::status->nextCampaignMission(true);
                    } else {
                        Globals::status->setFreelanceMission(Mission::empty);
                        Globals::layout->showMissionRewardMessage(mission->getReward() + mission->getBonus(), false);
                    }
                    Globals::status->changeCredits(mission->getReward() + mission->getBonus());
                    this->levelScript->m_nTimeLimit = 0;

                    if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 15) {
                        FileRead *reader = new FileRead();
                        Globals::status->setStation(reader->loadStation(98));
                        delete reader;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 22) {
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 43) {
                        FileRead *reader = new FileRead();
                        Globals::status->setStation(reader->loadStation(10));
                        delete reader;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 42) {
                        delete this->level->objectivesB;
                        this->level->objectivesB = nullptr;
                        delete this->level->objectivesA;
                        this->level->objectivesA = nullptr;
                        goto finishMission;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 65) {
                        Globals::switch_to_target_setting = 1;
                        Globals::status->field_64 = this->player->player->getHitpoints();
                        Globals::status->field_5c = this->player->player->getShieldHP();
                        Globals::status->field_60 = this->player->player->getArmorHP();
                        Globals::status->field_68 = this->player->player->getGammaHP();
                        Level::initStreamOutPosition = 1;
                        FileRead *reader = new FileRead();
                        Globals::status->departStation(reader->loadStation(100));
                        delete reader;
                        this->active = 0;
                        this->applicationManager->SetCurrentApplicationModule(2);
                        return;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 81) {
                        Globals::switch_to_target_setting = 1;
                        Globals::status->field_64 = this->player->player->getHitpoints();
                        Globals::status->field_5c = this->player->player->getShieldHP();
                        Globals::status->field_60 = this->player->player->getArmorHP();
                        Globals::status->field_68 = this->player->player->getGammaHP();
                        Level::initStreamOutPosition = 1;
                        Globals::status->setStation(Globals::status->playerStation);
                        Globals::status->departStation(Globals::status->playerStation);
                        this->active = 0;
                        this->applicationManager->SetCurrentApplicationModule(2);
                        return;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 74) {
                        FileRead *reader = new FileRead();
                        Globals::status->setStation(reader->loadStation(100));
                        delete reader;
                        Globals::sound->stopAll();
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 95) {
                        Globals::switch_to_target_setting = 1;
                        Globals::status->field_64 = this->player->player->getHitpoints();
                        Globals::status->field_5c = this->player->player->getShieldHP();
                        Globals::status->field_60 = this->player->player->getArmorHP();
                        Globals::status->field_68 = this->player->player->getGammaHP();
                        Level::initStreamOutPosition = 1;
                        Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(10));
                        this->active = 0;
                        this->applicationManager->SetCurrentApplicationModule(2);
                        return;
                    } else if (mission->isCampaignMission() && (Globals::status->getCurrentCampaignMission() == 96 ||
                                                               Globals::status->getCurrentCampaignMission() == 127)) {
                        Globals::switch_to_target_setting = 1;
                        Globals::status->field_64 = this->player->player->getHitpoints();
                        Globals::status->field_5c = this->player->player->getShieldHP();
                        Globals::status->field_60 = this->player->player->getArmorHP();
                        Globals::status->field_68 = this->player->player->getGammaHP();
                        Level::initStreamOutPosition = 1;
                        Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(98));
                        this->active = 0;
                        this->applicationManager->SetCurrentApplicationModule(2);
                        return;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 100) {
                        Globals::status->field_64 = this->player->player->getHitpoints();
                        Globals::status->field_5c = this->player->player->getShieldHP();
                        Globals::status->field_60 = this->player->player->getArmorHP();
                        Globals::status->field_68 = this->player->player->getGammaHP();
                        Globals::switch_to_target_setting = 0;
                        Level::initStreamOutPosition = 0;
                        Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(120));
                        Globals::appManager->SetCurrentApplicationModule(5);
                        this->active = 0;
                        return;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 110) {
                        Globals::status->field_64 = this->player->player->getHitpoints();
                        Globals::status->field_5c = this->player->player->getShieldHP();
                        Globals::status->field_60 = this->player->player->getArmorHP();
                        Globals::status->field_68 = this->player->player->getGammaHP();
                        Globals::switch_to_target_setting = 0;
                        Level::initStreamOutPosition = 0;
                        Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(10));
                        Globals::appManager->SetCurrentApplicationModule(5);
                        Globals::enterSpaceLounge = 1;
                        this->active = 0;
                        return;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 120) {
                        Globals::switch_to_target_setting = 1;
                        Globals::status->field_64 = this->player->player->getHitpoints();
                        Globals::status->field_5c = this->player->player->getShieldHP();
                        Globals::status->field_60 = this->player->player->getArmorHP();
                        Globals::status->field_68 = this->player->player->getGammaHP();
                        Level::initStreamOutPosition = 0;
                        Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(126));
                        this->active = 0;
                        this->applicationManager->SetCurrentApplicationModule(5);
                        return;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 126) {
                        Globals::switch_to_target_setting = 1;
                        Globals::status->field_64 = this->player->player->getHitpoints();
                        Globals::status->field_5c = this->player->player->getShieldHP();
                        Globals::status->field_60 = this->player->player->getArmorHP();
                        Globals::status->field_68 = this->player->player->getGammaHP();
                        Level::initStreamOutPosition = 0;
                        Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(120));
                        this->active = 0;
                        this->applicationManager->SetCurrentApplicationModule(2);
                        return;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 134) {
                        Globals::switch_to_target_setting = 0;
                        Globals::status->field_64 = this->player->player->getHitpoints();
                        Globals::status->field_5c = this->player->player->getShieldHP();
                        Globals::status->field_60 = this->player->player->getArmorHP();
                        Globals::status->field_68 = this->player->player->getGammaHP();
                        Level::initStreamOutPosition = 0;
                        Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(112));
                        this->active = 0;
                        this->applicationManager->SetCurrentApplicationModule(5);
                        return;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 144) {
                        Globals::switch_to_target_setting = 1;
                        Globals::status->field_64 = this->player->player->getHitpoints();
                        Globals::status->field_5c = this->player->player->getShieldHP();
                        Globals::status->field_60 = this->player->player->getArmorHP();
                        Globals::status->field_68 = this->player->player->getGammaHP();
                        Level::initStreamOutPosition = 0;
                        Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(112));
                        this->active = 0;
                        this->applicationManager->SetCurrentApplicationModule(2);
                        return;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 155) {
                        Level::initStreamOutPosition = 1;
                        Globals::switch_to_target_setting = 1;
                        Globals::status->departStation(
                            (Station *) (intptr_t) Globals::galaxy->getStation(Globals::status->field_84));
                        this->active = 0;
                        this->applicationManager->SetCurrentApplicationModule(2);
                        return;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 161) {
                        Globals::status->field_64 = this->player->player->getHitpoints();
                        Globals::status->field_5c = this->player->player->getShieldHP();
                        Globals::status->field_60 = this->player->player->getArmorHP();
                        Globals::status->field_68 = this->player->player->getGammaHP();
                        Globals::switch_to_target_setting = 0;
                        Level::initStreamOutPosition = 0;
                        Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(93));
                        this->active = 0;
                        this->applicationManager->SetCurrentApplicationModule(2);
                        return;
                    } else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 162) {
                        Globals::status->field_64 = this->player->player->getHitpoints();
                        Globals::status->field_5c = this->player->player->getShieldHP();
                        Globals::status->field_60 = this->player->player->getArmorHP();
                        Globals::status->field_68 = this->player->player->getGammaHP();
                        Globals::switch_to_target_setting = 0;
                        Level::initStreamOutPosition = 0;
                        Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(93));
                        this->active = 0;
                        this->applicationManager->SetCurrentApplicationModule(5);
                        return;
                    } else {
                        goto finishMission;
                    }
                    // Back to the station.
                    Globals::switch_to_target_setting = 0;
                    Globals::status->field_64 = this->player->player->getHitpoints();
                    Globals::status->field_5c = this->player->player->getShieldHP();
                    Globals::status->field_60 = this->player->player->getArmorHP();
                    Globals::status->field_68 = this->player->player->getGammaHP();
                    this->active = 0;
                    this->applicationManager->SetCurrentApplicationModule(5);
                    return;

                finishMission:
                    if (!keepObjectives) {
                        this->level->removeObjectives();
                        Globals::status->setMission(Mission::empty);
                        this->player->setRoute(nullptr);
                        if (this->player->goingToWaypoint()) {
                            this->player->setAutoPilot(nullptr);
                        }
                        this->player->removeRoute();
                        this->level->setPlayerRoute(nullptr);
                    }
                    if (!mission->isCampaignMission() && mission->getType() == 183) {
                        // Escort job handed out right after the previous one.
                        this->field_0x1e0 = this->level->killCountB;
                        Globals::status->getShip()->setEquipment((*Globals::items)[216]->makeItem(10), 0);
                        Mission *escort = new Mission(183, String("Client"), Globals::imageFactory->createChar(true, 0), 0,
                                                      0, Globals::status->getStation()->getIndex(), 1);
                        Globals::status->setMission(escort);
                        Globals::status->setFreelanceMission(escort);
                        this->active = 0;
                        Globals::appManager->SetCurrentApplicationModule(2);
                        return;
                    }
                } else {
                    this->levelScript->resetStartSequenceOver();
                }
                this->frameTime = 0;
                this->hud->resetAnalogStick();
                this->hud->releaseAllKeys();
                return;
            }
            if (this->menuTouchOpen) {
                if (this->freeCamMode) {
                    MGameAppData *appData = (MGameAppData *) Globals::appManager->GetApplicationData();
                    if (appData->modalActive || appData->transitionActive) {
                        return;
                    }
                    if (!this->menuWindow->isShowingMessage() && !this->menuWindow->isMakingScreenshot()) {
                        freeCamTouchEnd(x, y, touchId);
                    }
                }
                uint8_t &soundsMuted = reinterpret_cast<uint8_t *>(&this->field_0x1a4)[0];
                if (this->menuWindow->OnTouchEnd(x, y, touchId)) {
                    // Pause menu closed.
                    this->pauseSnapshot = 0;
                    this->pauseOpen = 0;
                    resumeSounds();
                    if (Globals::sound->IsCategoryEnabled(2)) {
                        if (soundsMuted) {
                            this->player->PlayEngineSound();
                            Array<KIPlayer *> *enemies = this->level->getEnemies();
                            if (enemies != nullptr) {
                                for (unsigned i = 0; i < enemies->size(); i++) {
                                    (*enemies)[i]->PlayEngineSound();
                                }
                            }
                        } else {
                            this->player->ResumeEngineSound();
                            Array<KIPlayer *> *enemies = this->level->getEnemies();
                            if (enemies != nullptr) {
                                for (unsigned i = 0; i < enemies->size(); i++) {
                                    (*enemies)[i]->ResumeEngineSound();
                                }
                            }
                        }
                    } else if (soundsMuted) {
                        this->player->PlayEngineSound();
                        Array<KIPlayer *> *enemies = this->level->getEnemies();
                        if (enemies != nullptr) {
                            for (unsigned i = 0; i < enemies->size(); i++) {
                                (*enemies)[i]->PlayEngineSound();
                            }
                        }
                    } else {
                        if (this->player != nullptr) {
                            this->player->StopEngineSound();
                        }
                        Array<KIPlayer *> *enemies = this->level->getEnemies();
                        if (enemies != nullptr) {
                            for (unsigned i = 0; i < enemies->size(); i++) {
                                (*enemies)[i]->StopEngineSound();
                            }
                        }
                    }
                    this->menuTouchOpen = 0;
                    this->touch0Id = 0;
                    this->touch1Id = 0;
                    this->activeTouchId = nullptr;
                    this->level->getStarSystem()->initLight();
                    float particleQuality = reinterpret_cast<const float &>(Globals::options[40]);
                    this->level->enableParticleEffects(particleQuality > 0.0f, particleQuality > 0.7f);
                    if (this->menuWindow->cinematicSteerActive) {
                        // "Skip" pressed in the pause menu.
                        this->menuWindow->cinematicSteerActive = 0;
                        switch (Globals::status->getCurrentCampaignMission()) {
                            case 154:
                            case 157:
                            case 158:
                                this->levelScript->skipCutscene();
                                break;
                            case 1:
                                Globals::switch_to_target_setting = 0;
                                this->active = 0;
                                Globals::appManager->SetCurrentApplicationModule(5);
                                break;
                            case 0:
                                Globals::status->nextCampaignMission(true);
                                Globals::status->setKills(3);
                                Globals::switch_to_target_setting = 1;
                                this->active = 0;
                                Globals::appManager->SetCurrentApplicationModule(2);
                                Level::initStreamOutPosition = 0;
                                break;
                            default:
                                break;
                        }
                    }
                    this->freeCamMode = 0;
                }
                if (!this->freeCamMode && this->menuWindow->inCinematicMode()) {
                    setCinematicMode(true);
                    this->hudTouchFlags = 0;
                    this->level->getStarSystem()->initLight();
                    return;
                }
                if (this->freeCamMode && !this->menuWindow->inCinematicMode()) {
                    setCinematicMode(false);
                    this->hudTouchFlags = 0;
                }
                return;
            }
        }
    }

    this->hudTouchFlags = this->hud->touchEnd(x, y, touchId);
    if (this->hudTouchFlags != 0) {
        this->touch0Id = 0;
        this->touch1Id = 0;
        if (this->hudTouchFlags & 1) {
            // Pause button.
            pauseSounds();
            if (!this->pauseOpen) {
                if (this->menuWindow == nullptr) {
                    this->menuWindow = new MenuTouchWindow(1);
                }
                this->menuWindow->setSkipButtonVisible(this->levelScript->canSkipCutsceneNow());
                this->pauseOpen = 1;
                pauseSounds();
                this->pauseSnapshot = this->pauseOpen;
                reinterpret_cast<uint8_t *>(&this->field_0x1a4)[0] = !Globals::sound->IsCategoryEnabled(2);
                Globals::sound->pauseAllPlaying();
                this->player->PauseEngineSound();
                Array<KIPlayer *> *enemies = this->level->getEnemies();
                if (enemies != nullptr) {
                    for (unsigned i = 0; i < enemies->size(); i++) {
                        (*enemies)[i]->PauseEngineSound();
                    }
                }
                this->menuWindow->setCutsceneMode(this->jumpActive ? true : this->player->isDead());
                this->menuTouchOpen = 1;
                Globals::sound->play(123, nullptr, nullptr, 0.0f);
            }
            this->hud->releaseAllKeys();
            return;
        }
    }
    if (this->levelScript->startSequence() || this->player->isDead()) {
        this->levelScript->skipSequence();
        return;
    }
    if (this->jumpActive || this->jumpDriveActive) {
        return;
    }

    if ((this->hudTouchFlags & 2) && this->player->isBoostRefreshed() && !this->player->boosting() &&
        !this->player->isMining() && !this->player->isDockedToDockingPoint()) {
        this->player->setThrust(1.0f);
        reinterpret_cast<float &>(this->field_0x1b0) = 1.0f;
        this->field_0x1b4 = 100.0f;
        static_cast<Engine *>(this->applicationManager->GetEngine())->wheelDelta = 0.0f;
        this->player->boost();
    }
    if ((this->hudTouchFlags & 8) && this->player->player->gunAvailable(1) && !this->player->isMining() &&
        !this->player->isInTurretMode() && !this->player->isDockedToDockingPoint() &&
        !this->player->isLandingOrTakingOff()) {
        this->player->shoot(this->deltaTime, 1);
        this->hud->checkIfQuickMenuIsEmpty();
    }
    if (this->player->isHacking()) {
        if (this->hudTouchFlags & 0x200) {
            this->player->hackingRotateLCW();
        }
        if (this->hudTouchFlags & 0x400) {
            this->player->hackingRotateRCW();
        }
    }
    if (this->player->isInRocketControl()) {
        return;
    }
    if (this->player->hasAutoTurret() && (this->hudTouchFlags & 0x20000000)) {
        this->player->setAutoTurret(!this->player->autoTurretIsEnabled());
        this->hud->hudEvent(this->player->autoTurretIsEnabled() ? 32 : 33, this->player, 0);
    }

    if (this->hudTouchFlags & 0x40) {
        // Dock / auto pilot button.
        if (Globals::status->inAlienOrbit() && this->player->isDockingToAsteroid()) {
            this->player->dockToAsteroid(nullptr, this->radar);
        }
        if ((!Globals::status->inAlienOrbit() ||
             (Globals::status->inAlienOrbit() && Globals::status->getCurrentCampaignMission() == 154 &&
              this->level->getNumDockingTargets() >= 1)) &&
            Globals::status->getCurrentCampaignMission() >= 2 && Globals::status->getCurrentCampaignMission() != 48 &&
            !this->player->isDockedToDockingPoint() && !this->player->isLandingOrTakingOff()) {
            if (!this->_bc8 && !this->player->isMining()) {
                if (this->player->isAutoPilot()) {
                    this->player->setAutoPilot(nullptr);
                    this->hud->hudEvent(6, this->player, 0);
                } else if (!this->_bc8) {
                    if (this->player->isDockingToAsteroid()) {
                        this->player->dockToAsteroid(this->radar->getLockedAsteroid(), this->radar);
                        this->hud->hudEvent(6, this->player, 0);
                    } else if (this->player->isDockingToDockingPoint()) {
                        this->radar->lockedEnemy = nullptr;
                        this->player->dockToDockingPoint(nullptr, this->radar);
                        this->hud->hudEvent(6, this->player, 0);
                    } else if (this->player->isDockingToStream()) {
                        this->player->dockToAsteroid(this->radar->lockedStation, this->radar);
                        this->hud->hudEvent(6, this->player, 0);
                    } else if ((!Globals::status->inAlienOrbit() ||
                                (Globals::status->inAlienOrbit() &&
                                 Globals::status->getCurrentCampaignMission() == 154)) &&
                               (Globals::status->getMission() == nullptr ||
                                Globals::status->getMission()->getType() != 183)) {
                        // Open the auto pilot target menu.
                        this->pauseOpen = 1;
                        this->hudMenuOpen = 1;
                        pauseSounds();
                        this->hud->initHudMenu(3, this->level);
                        this->_bc8 = 1;
                    }
                }
            } else if (this->_bc8) {
                this->hudMenuOpen = 0;
                this->_bc8 = 0;
                this->pauseOpen = 0;
                resumeSounds();
                this->hud->closeHudMenu();
                if (this->player != nullptr) {
                    this->player->resetGunDelay();
                }
            }
        }
    } else if (this->_bc8) {
        // Auto pilot target menu entries.
        if (this->hudTouchFlags & 0x200000) {
            this->levelScript->setAutoPilotToProgrammedStation();
        }
        if (this->hudTouchFlags & 0x400000) {
            this->player->setAutoPilot((*this->level->getLandmarks())[1]);
            this->hud->hudEvent(12, this->player, 0);
        }
        if (this->hudTouchFlags & 0x800000) {
            this->player->setAutoPilot((*this->level->getLandmarks())[0]);
            this->hud->hudEvent(10, this->player, 0);
        }
        if (this->hudTouchFlags & 0x1000000) {
            this->player->setAutoPilot(this->level->getAsteroidWaypoint());
            this->hud->hudEvent(14, this->player, 0);
        }
        if ((this->hudTouchFlags & 0x2000000) && this->level->getPlayerRoute() != nullptr) {
            this->player->setAutoPilot(this->level->getPlayerRoute()->getWaypoint());
            this->hud->hudEvent(13, this->player, 0);
        }
        for (int i = 0; i < this->level->getNumDockingTargets(); i++) {
            if ((this->hudTouchFlags & (0x4000000 << i)) && this->level->getDockingTarget(i) != 0) {
                this->radar->lockedEnemy = (KIPlayer *) (intptr_t) this->level->getDockingTarget(i);
                this->hud->hudEvent(34, this->player, 0);
                this->player->dockToDockingPoint((KIPlayer *) (intptr_t) this->level->getDockingTarget(i), this->radar);
                this->radar->dockNavPtr = nullptr;
                this->radar->lockedAsteroid = nullptr;
                this->radar->field_0x10 = 0;
                this->hud->releaseAllKeys();
            }
        }
        this->hudMenuOpen = 0;
        this->_bc8 = 0;
        this->pauseOpen = 0;
        resumeSounds();
        this->hud->closeHudMenu();
        if (this->player != nullptr) {
            this->player->resetGunDelay();
        }
    }

    if ((this->hudTouchFlags & 4) && !this->player->isMining()) {
        // Quick menu button.
        this->hudMenuOpen = !this->hudMenuOpen;
        this->pauseOpen = !this->pauseOpen;
        if (this->hudMenuOpen) {
            pauseSounds();
            this->hud->initHudMenu(0, this->level);
        }
    }
    if ((this->hudTouchFlags & 0x80) && !this->player->isMining()) {
        switchCamera(this->cameraMode + 1);
    }

    if (this->hudMenuOpen) {
        int weaponSlot = -1;
        int wingmanCommand = -1;
        if (this->hudTouchFlags & 0x200) {
            this->hud->initHudMenu(1, this->level);
        } else if (this->hudTouchFlags & 0x800) {
            this->hudMenuOpen = 0;
            this->hud->closeHudMenu();
            this->pauseOpen = 0;
            resumeSounds();
            useCloak();
        } else if (this->hudTouchFlags & 0x400) {
            this->hud->initHudMenu(2, this->level);
        } else if (this->hudTouchFlags & 0x2000) {
            weaponSlot = 0;
        } else if (this->hudTouchFlags & 0x4000) {
            weaponSlot = 1;
        } else if (this->hudTouchFlags & 0x8000) {
            weaponSlot = 2;
        } else if (this->hudTouchFlags & 0x10000) {
            weaponSlot = 3;
        } else if (this->hudTouchFlags & 0x20000) {
            wingmanCommand = 1;
        } else if (this->hudTouchFlags & 0x40000) {
            wingmanCommand = 3;
        } else if (this->hudTouchFlags & 0x80000) {
            wingmanCommand = 2;
        } else if (this->hudTouchFlags & 0x100000) {
            uint8_t &wingmenFollow = reinterpret_cast<uint8_t &>(Globals::status->field_f8);
            wingmenFollow = !wingmenFollow;
            wingmanCommand = 0;
        } else if (this->hudTouchFlags & 0x1000) {
            UseKhadorDrive();
        }
        if (weaponSlot >= 0) {
            // Select a secondary weapon.
            Array<Item *> *secondaries = Globals::status->getShip()->getEquipment(1);
            if (secondaries != nullptr) {
                this->level->getPlayer()->setCurrentSecondaryWeaponIndex((*secondaries)[weaponSlot]->getIndex());
                this->hud->setCurrentSecondaryWeapon((*secondaries)[weaponSlot]);
            }
            this->pauseOpen = 0;
            this->hudMenuOpen = 0;
            resumeSounds();
            this->hud->closeHudMenu();
        } else {
            if (wingmanCommand >= 0) {
                // Order the wingmen.
                Array<KIPlayer *> *enemies = this->level->getEnemies();
                if (enemies != nullptr) {
                    for (unsigned i = 0; i < enemies->size(); i++) {
                        KIPlayer *wingman = (*enemies)[i];
                        if (wingman->isWingMan() && !wingman->isDead()) {
                            wingman->setWingmanCommand(wingmanCommand,
                                                       wingmanCommand == 3 ? this->radar->getLockedEnemy() : nullptr);
                        }
                    }
                }
                this->hudMenuOpen = 0;
                this->hud->closeHudMenu();
                this->pauseOpen = 0;
                resumeSounds();
            }
            if (this->hudTouchFlags == 0) {
                this->hud->closeHudMenu();
                this->hudMenuOpen = 0;
                this->touch0Id = 0;
                this->touch1Id = 0;
                this->pauseOpen = 0;
                resumeSounds();
                if (this->_bc8) {
                    this->_bc8 = 0;
                    if (this->player != nullptr) {
                        this->player->resetGunDelay();
                    }
                }
            }
        }
    } else if (this->cameraMode == 0) {
        maneuverTouchEnd(x, y, touchId);
        reinterpret_cast<uint16_t &>(this->_b1b8) = 0;
    } else if (this->cameraMode == 3) {
        freeCamTouchEnd(x, y, touchId);
    }

    if (this->player->isAutoPilot() && !wasAutoPilot) {
        reinterpret_cast<float &>(this->field_0x1b0) = 1.0f;
        this->field_0x1b4 = 100.0f;
    }
}

bool MGame::successCheck() {
    bool success = false;
    if (!(this->elapsedTime >= 5001 && !this->jumpDriveActive) &&
        ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getType() != 0xaa)
        return success;

    Mission *completed = Globals::status->missionCompleted(false, false, this->levelScript->m_nScriptTime);
    if (!completed && !this->level->checkObjective((int) this->levelScript->m_nScriptTime))
        return success;

    if (Globals::status->getMission()->getType() == 5 || Globals::status->getMission()->getType() == 3) {
        if (this->dialogueWindow == nullptr) {
            this->dialogueWindow = new DialogueWindow();
            if (this->level != nullptr) this->dialogueWindow->setLevel(this->level);
        } else if (!this->dialogueWindow->hasLevel() && this->level != nullptr) {
            this->dialogueWindow->setLevel(this->level);
        }
        Globals::status->getMission()->setTargetStation(Globals::status->getMission()->getAgent()->getStation());
        this->dialogueWindow->set(Globals::status->getMission(), 1, -1);
        Globals::status->getMission()->setType(0xb);

        this->player->setTurretMode(false);
        this->levelScript->resetCamera(this->level);
        this->player->setFreeLookMode(false);
        this->camera->enableFirstPersonCam(false);
        this->player->hideShipForFirstPersonCameraView(false);
        this->needsRedraw = 1;

        Globals::status->getMission()->setStatusValue(-1);
        Globals::status->getMission()->setWon(false);

        String text(*Globals::gameText->getText(803), false);
        text = Globals::status->replaceHash(String(text, false), Globals::status->getMission()->getTargetStationName(),
                                            String("#S", false));
        Globals::status->getMission()->getAgent()->setMissionString(String(text, false));
        Globals::status->setMission(Mission::empty);

        this->player->setRoute(nullptr);
        if (this->player->goingToWaypoint())
            this->player->setAutoPilot(nullptr);
        this->player->removeRoute();
        this->level->setPlayerRoute(nullptr);
        delete this->level->objectivesA;
        this->level->objectivesA = nullptr;
        delete this->level->objectivesB;
        this->level->objectivesB = nullptr;

        this->pauseOpen = 1;
        this->cutsceneActive = 1;
        pauseSounds();
        success = true;
        return success;
    }

    if (!Globals::status->getMission()->isCampaignMission())
        Globals::status->incMissionCount();

    if (Globals::status->getMission()->isCampaignMission() &&
        !(Globals::status->getMission()->isCampaignMission() &&
          DialogueWindow::hasSuccessDialogue(Globals::status->getCurrentCampaignMission()))) {
        if (Globals::status->getCurrentCampaignMission() >= 46 && Globals::status->getMission()->isCampaignMission() &&
            !DialogueWindow::hasSuccessDialogue(Globals::status->getCurrentCampaignMission())) {
            Globals::status->nextCampaignMission(true);
            this->level->removeObjectives();
            Globals::status->setMission(Mission::empty);
        }
        return success;
    }

    if (this->dialogueWindow == nullptr) {
        this->dialogueWindow = new DialogueWindow();
        if (this->level != nullptr) this->dialogueWindow->setLevel(this->level);
    } else if (!this->dialogueWindow->hasLevel() && this->level != nullptr) {
        this->dialogueWindow->setLevel(this->level);
    }
    this->dialogueWindow->set(completed ? completed : Globals::status->getMission(), 1, -1);
    this->pauseOpen = 1;
    this->cutsceneActive = 1;
    pauseSounds();

    if (Globals::status->getMission()->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 38) {
        Array<KIPlayer *> *enemies = this->level->getEnemies();
        for (unsigned i = 0; i < enemies->size(); i++) {
            KIPlayer *enemy = (*enemies)[i];
            if (enemy->field_0x40 && !enemy->isDead())
                enemy->player->setHitpoints(9999999);
        }
    } else if (Globals::status->getMission()->isCampaignMission() &&
               Globals::status->getCurrentCampaignMission() == 56) {
        Array<AEGeometry *> *planets = static_cast<Array<AEGeometry *> *>(this->level->getStarSystem()->getPlanets());
        Vector target = (*planets)[2]->getPosition() * 100.0f;
        int coords[3] = {(int) target.x, (int) target.y, (int) target.z};
        Route *route = new Route(coords, 3);
        Array<KIPlayer *> *enemies = this->level->getEnemies();
        for (unsigned i = 0; i < enemies->size(); i++) {
            KIPlayer *enemy = (*enemies)[i];
            if (enemy->shipGroup == 1)
                enemy->setRoute(route->clone());
        }
        delete route;
    } else if (Globals::status->getMission()->isCampaignMission() &&
               Globals::status->getCurrentCampaignMission() == 63) {
        Array<KIPlayer *> *enemies = this->level->getEnemies();
        for (unsigned i = 0; i < enemies->size(); i++) {
            KIPlayer *enemy = (*enemies)[i];
            if (enemy->shipGroup == 8)
                enemy->player->removeAllGuns();
        }
    } else if (Globals::status->getMission()->isCampaignMission() &&
               Globals::status->getCurrentCampaignMission() == 73) {
        Array<KIPlayer *> *enemies = this->level->getEnemies();
        for (unsigned i = 0; i < enemies->size(); i++) {
            KIPlayer *enemy = (*enemies)[i];
            if (enemy->field_0x40 && !enemy->isDead()) {
                enemy->player->setHitpoints(9999999);
                static_cast<PlayerFixedObject *>(enemy)->setMoving(true);
            }
        }
    } else if (Globals::status->getStation()->getIndex() == 112 &&
               Globals::status->getCurrentCampaignMission() == 143) {
        Level::initStreamOutPositionAfterCutscene = 1;
    }
    success = true;
    return success;
}

MGame::~MGame() {
    OnRelease();
}

void MGame::resumeSounds() {
    ((FModSound *) (*g_fmod))->resumeAll();
    this->player->ResumeEngineSound();
    Array<KIPlayer *> *e = this->level->getEnemies();
    if (e == nullptr) return;
    for (uint32_t i = 0; i < e->size(); i++)
        ((KIPlayer *) ((*e)[i]))->ResumeEngineSound();
}

static int *g_jumpFlag;

static int **g_alienAmt;

static int **g_jumpCost;

static int **g_alienCost;

void MGame::startChargingJumpDrive() {
    if (this->usingJumpDrive == 0) return;
    int needed = 1;
    if (((Ship *) (Globals::status->getShip()))->hasCargo(0x7a, 1) == 0) {
        ChoiceWindow *w = this->choiceWindow;
        if (w == 0) {
            w = new ChoiceWindow();
            this->choiceWindow = w;
        }
        String *txt = ((GameText *) (*(int *) Globals::status))->getText(0x243);
        ((ChoiceWindow *) (w))->set(*(String *) txt);
        this->pauseOpen = 1;
        this->choiceWindowOpen = 1;
        ((MGame *) (this))->pauseSounds();
        **g_jumpCost = 0;
        return;
    }
    int hc = Globals::status->hardCoreMode();
    int *jf = g_jumpFlag;
    if (hc != 0) needed = 2;
    int cost;
    if (*jf == (int) (intptr_t) Globals::status->playerStation) {
        cost = needed << 1;
    } else {
        cost = **g_alienAmt;
        if (Globals::status->inAlienOrbit() != 0) cost = needed;
    }
    ((Ship *) (Globals::status->getShip()))->getCargo(0x7a);
    if (((Item *) (0))->getAmount() < cost) {
        ChoiceWindow *w = this->choiceWindow;
        if (w == 0) {
            w = new ChoiceWindow();
            this->choiceWindow = w;
        }
        int hc2 = Globals::status->hardCoreMode();
        String *txt = ((GameText *) (*(int *) Globals::status))->getText(hc2 != 0 ? 0x243 : 0x244);
        ((ChoiceWindow *) (w))->set(*(String *) txt);
        this->pauseOpen = 1;
        this->choiceWindowOpen = 1;
        ((MGame *) (this))->pauseSounds();
        *jf = 0;
        return;
    }
    this->player->startJumpDrive();
    if (*jf != (int) (intptr_t) Globals::status->playerStation) {
        if (Globals::status->inAlienOrbit() == 0) needed = **g_alienCost;
    }
    this->hud->hudEvent(0x1e, this->player, needed);
    ((Ship *) (Globals::status->getShip()))->removeCargo(0x7a, needed);
}

void MGame::pauseSounds() {
    this->pauseSnapshot = this->pauseOpen;
    ((FModSound *) (*g_fmod))->pauseAllPlayingSoundFXEvents();
    this->player->PauseEngineSound();
    Array<KIPlayer *> *e = this->level->getEnemies();
    if (e != nullptr) {
        for (uint32_t i = 0; i < e->size(); i++)
            ((KIPlayer *) ((*e)[i]))->PauseEngineSound();
    }
}

void MGame::reset() {
    this->dragLastY = 0;
    this->dragDeltaX = 0;
    this->activeTouchId = nullptr;
    this->touch1Id = 0;
    this->dragLastX = 0;
    this->menuTime = 0;
    this->dragRotIntY = 0;
    this->flShakeX = 25.0f;
    this->flCameraRoll = 0;
    this->touch0Id = 0;
    this->flShakeY = -50.0f;
    this->dragStartX = 0;
    this->dragStartY = 0;
    this->dragDeltaY = 0;
    this->freeCamDragging = 0;
    this->field_0x120 = 2500.0f;
    this->dragRotIntX = 0;
    this->player = this->level->getPlayer();
    this->hud = new Hud();
    this->radio = new Radio();
    this->radio->setMessages(this->level->getMessages());

    Globals::Canvas->CameraCreate(this->cameraId);
    Globals::Canvas->CameraSetPerspective(this->cameraId, 1.22f, 20.0f,
                                          Globals::status->inAlienOrbit()
                                              ? (Globals::status->getCurrentCampaignMission() < 0x50 ? 450000.0f
                                                                                                     : 300000.0f)
                                              : 300000.0f);
    if (this->camera != nullptr) {
        delete this->camera;
        this->camera = nullptr;
    }
    this->camera = new TargetFollowCamera(this->cameraId, this->player->geometry, Vector{0.0f, 0.0f, 0.0f},
                                          Vector{0.0f, 0.0f, 0.0f});
    Globals::Canvas->CameraSetCurrent(this->cameraId);
    this->player->setTargetFollowCamera(this->camera);
    this->camera->resetShipHandling();
    this->radar = new Radar(this->level);
    if (Globals::status->getMission() != nullptr)
        this->campaignMission = Globals::status->getMission()->isCampaignMission();
    this->levelScript = new LevelScript(this->level, this->hud, this->radar, this->camera);
    this->levelScript->resetCamera(this->level);
    this->level->initParticleSystems();
    this->choiceWindow = new ChoiceWindow();

    this->elapsedTime = 0;
    this->cameraMode = 0;
    this->field_0x18 = 0;
    this->_b5c = 0;
    this->pauseOpen = 0;
    this->jumpActive = 0;
    this->gameOverActive = 0;
    this->field_0xc4 = 0;
    this->autopilotMenuOpen = 0;
    this->field_0xc6 = 0;
    this->_bc8 = 0;
    this->menuTouchOpen = 0;
    this->field_0xe0 = 0;
    this->freeCamMode = 0;
    this->field_0x110 = 0;
    this->lastTapTime = 0;
    this->lastAlignTime = 0;
    this->flFastForwardWeight = 1.0f;
    this->field_0x1b0 = 0;
    this->field_0x1b4 = 100.0f;
    this->_b1b8 = 0;
    this->thrustEngaged = 0;
    this->thrustThreshold = Globals::layout->field_0x2f4;
    this->thrustBase = 0;
    this->gameRecord = nullptr;
    this->maneuverHoldTime = 0;
    this->maneuverActive = 0;
    this->maneuverStartX = 0;
    this->maneuverStartY = 0;
    this->startTime = this->lastTime = Globals::appManager->GetCurrentTimeMillis();
    this->needsRedraw = 1;
    this->field_0x112 = 1;
    this->field_0xcf = 0;
    this->choiceItemCount = 0;
    this->field_0xd4 = 0;
    this->field_0x1e4 = 0;
    this->field_0x1e5 = 0;
    this->field_0x1e6 = 0;
    this->field_0x1dd = AbyssEngine::Engine::UseAdvancedShader;
    Globals::status->field_0x184 = 0;
    Globals::status->field_0x188 = 1;
    Globals::status->field_0x18c = 1;
}

static int g_accelTune;

struct AccelTune {
    uint8_t _pad0[0x10];
    char invertSign;
    uint8_t _pad11[0xb];
    float refValue;
    float baseValue;
};

struct AccelCtxValue {
    double _v0;
    double _v1;
    double comp2;
};
#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(AccelTune, invertSign) == 0x10, "AccelTune::invertSign @ 0x10");
static_assert(offsetof(AccelTune, refValue) == 0x1c, "AccelTune::refValue @ 0x1c");
static_assert(offsetof(AccelTune, baseValue) == 0x20, "AccelTune::baseValue @ 0x20");
static_assert(offsetof(AccelCtxValue, comp2) == 0x10, "AccelCtxValue::comp2 @ 0x10");
#endif

void MGame::handleAccelerometer() {
    Engine *eng = (Engine *) this->applicationManager->GetEngine();
    double *acc = ((Engine *) (eng))->GetAccelValue();

    float yaw = (float) (acc[1] * 2.5);
    float steer = 1.0f;
    int *shipField = (this->timeWarpState > 0) ? &this->field_0x44 : &this->deltaTime;
    if (yaw <= 1.0f) {
        steer = -1.0f;
        if (yaw < -1.0f) {
            this->player->left(*shipField, steer * steer);
            goto afterYaw;
        }
        steer = yaw;
        if (yaw < 0.0f) {
            this->player->left(*shipField, steer * steer);
            goto afterYaw;
        }
        if (yaw == 0.0f)
            goto afterYaw;
    }
    this->player->right(*shipField, steer * steer);

afterYaw: {
        AbyssEngine::ApplicationManager *appMgr = this->applicationManager;

        double *v1 = ((Engine *) appMgr->GetEngine())->GetAccelValue();
        double d0 = *v1;
        AccelCtxValue *c2 =
                (AccelCtxValue *) ((Engine *) appMgr->GetEngine())->GetAccelValue();

        float base = (float) d0;
        AccelTune *tune = (AccelTune *) (intptr_t) g_accelTune;
        double dz = c2->comp2;
        float ref = tune->refValue;
        char sign = tune->invertSign;

        AccelCtxValue *c3 =
                (AccelCtxValue *) ((Engine *) appMgr->GetEngine())->GetAccelValue();

        float a, b;
        if (sign == 0) {
            float t = base;
            if (c3->comp2 > 0.0) {
                t = 1.0f;
                if (base <= 1.0f) t = (1.0f - base) + 1.0f;
            }
            a = ref - t;
            b = tune->baseValue - (float) dz;
        } else {
            float t = base;
            if (c3->comp2 > 0.0) {
                t = 1.0f;
                if (base <= 1.0f) t = (1.0f - base) + 1.0f;
            }
            a = t - ref;
            b = (float) dz - tune->baseValue;
        }

        float a3 = a * 3.0f;
        float b3 = b * 3.0f;
        float aAbs = a * -3.0f;
        if (a3 > 0.0f) aAbs = a3;
        float bAbs = b * -3.0f;
        if (b3 > 0.0f) bAbs = b3;
        float roll = (aAbs < bAbs) ? b3 : a3;

        float rollMag = 1.0f;
        shipField = (this->timeWarpState > 0) ? &this->field_0x44 : &this->deltaTime;
        if (roll <= 1.0f) {
            rollMag = -1.0f;
            if (roll < -1.0f) {
                return this->player->turnHorizontal(*shipField, rollMag * rollMag);
            }
            rollMag = roll;
            if (roll < 0.0f) {
                return this->player->turnHorizontal(*shipField, rollMag * rollMag);
            }
            if (roll == 0.0f) return;
        }
        return this->player->turnHorizontal(*shipField, rollMag * rollMag);
    }
}

static int **g_tmShipTune;

static int **g_tmStarMap;

static int **g_tmAppData;

void MGame::OnTouchMove(int p1, int y, void *touch) { // lint: void_ptr -- exported override signature (Pv)
    // lint: void_ptr -- exported override signature (Pv)
    MGame *self = this;
    int handledFree = 0;
    if (self->pauseOpen != 0) {
        uint8_t fc = self->freeCamMode;
        int allowFree = (fc != 0);
        if (fc != 0 &&
            self->menuWindow->isShowingMessage() == 0 &&
            self->menuWindow->isMakingScreenshot() == 0) {
            allowFree = 1;
        } else {
            allowFree = 0;
        }
        (void) allowFree;
    }

    if (self->freeCamDragging != 0 && self->hudTouchFlags == 0 &&
        self->cameraMode == 3) {
        ((MGame *) (self))->freeCamTouchMove(p1, y, touch);
        handledFree = 1;
    }

    if (!handledFree) {
        if (self->pauseOpen == 0) {
            int hh = self->hud->touchMove(p1, y, touch);
            self->hudTouchFlags = hh;
            unsigned mode = (unsigned) self->cameraMode;
            if (mode <= 1) {
                ((MGame *) (self))->maneuverTouchMove(mode, y, touch);
                if (reinterpret_cast<uint16_t &>(self->_b1b8) != 0 && self->jumpActive == 0) {
                    int f8 = self->hudTouchFlags;
                    int ok = (f8 == 0) ||
                             (f8 == 0x20 && self->activeTouchId != touch);
                    if (ok) {
                        bool apply = false;
                        if (self->thrustEngaged == 0) {
                            float fy = (float) y;
                            float start = (float) self->thrustStartY;
                            float thresh = (float) self->thrustThreshold;
                            float d = fy - start;
                            float ad = (d > 0.0f) ? d : -d;
                            if (ad > thresh) {
                                int dir = (start > fy) ? 1 : -1;
                                int ny = dir + y;
                                self->thrustResetX = 0;
                                self->thrustStartY = ny;
                                self->thrustBase = self->player->getThrust();
                                apply = true;
                            }
                        } else {
                            apply = true;
                        }
                        if (apply) {
                            self->thrustEngaged = 1;
                            float divisor = Globals::layout->field_0x2f8;
                            float t = ((float) self->thrustStartY - (float) y -
                                       (float) self->thrustResetX) / divisor +
                                      self->thrustBase;
                            if (t < 0.0f) t = 0.0f;
                            if (t > 1.0f) t = 1.0f;
                            self->player->setThrust(t);
                            self->player->throttleChanged();
                        }
                    }
                }
            }
        }
    }

    if (self->pauseOpen == 0) return;

    if (self->gameOverActive != 0 || self->autopilotMenuOpen != 0 ||
        self->choiceWindowOpen != 0 || reinterpret_cast<int &>(self->field_0xc4) != 0) {
        self->choiceWindow->OnTouchMove(p1, y);
        return;
    }
    if (self->starMapOpen != 0) {
        int sel = **g_tmStarMap;
        if (*(uint8_t *) sel == 0)
            self->starMap->OnTouchMove(p1, y);
        else
            ((Layout *) (**g_tmStarMap))->OnTouchMove(p1, y);
        return;
    }
    if (self->cutsceneActive != 0) {
        self->dialogueWindow->OnTouchMove(p1, y);
        return;
    }
    if (self->menuTouchOpen != 0) {
        MGameAppData *ad = (MGameAppData *) (intptr_t) * *g_tmAppData;
        if (ad->modalActive == 0 && ad->transitionActive == 0)
            self->menuWindow->OnTouchMove(p1, y, touch);
    }
}

void MGame::setCinematicMode(bool on) {
    this->freeCamMode = on;
    Globals::isCinematicModeActive = on;
    if (on) {
        this->field_0x1dd = AbyssEngine::Engine::UseAdvancedShader;
        AbyssEngine::Engine::UseAdvancedShader = 1;
        if (this->jumpDriveActive == 0 && this->jumpActive == 0) {
            this->cinematicPrevCamMode = this->cameraMode;
            this->cinematicPrevLookAt = this->camera->isInLookAtMode();
            this->camera->setLookAtCam(false);
            switchCamera(3);
            this->level->lodManager->forceUpdate(this->deltaTime, true);
        }
    } else {
        AbyssEngine::Engine::UseAdvancedShader = this->field_0x1dd;
        switchCamera(this->cinematicPrevCamMode);
        this->camera->setLookAtCam(this->cinematicPrevLookAt);
    }
}

static inline void TFC_translate(TargetFollowCamera *cam, int x, int y, int z) { cam->translate(x, y, z); }


static int g_ujZNear;

static int g_ujZSound;

static int g_ujSpeed;

static int *g_ujSound;

static int g_ujPos0;

static int g_ujPos1;

static int g_ujPos2;

static int **g_ujStation;

static uint8_t **g_ujFlagA;

static uint8_t **g_ujFlagB;

static int **g_ujFlagC;

bool MGame::updateJumpScene() {
    MGame *self = this;
    bool fadeOut = true;

    if (self->usingJumpDrive != 0 && self->jumpFlash != 0) {
        AbyssEngine::Transform *tr = static_cast<AbyssEngine::Transform *>(
            Globals::Canvas->TransformGetTransform(self->jumpFlash->transform));
        long long timeRemaining = 1700LL - tr->currentTime;
        if (timeRemaining < 0) goto afterCam;
        goto camMove;
    } else {
        Array<KIPlayer *> *lm = self->level->getLandmarks();
        if ((*lm)[1] == nullptr) goto afterCam;
        lm = self->level->getLandmarks();
        if (static_cast<PlayerJumpgate *>((*lm)[1])->timeToJump() != 0) goto afterCam;
    }

camMove:
    {
        if (self->usingJumpDrive != 0) {
            self->egoJumpPos = AbyssEngine::AEMath::MatrixRotateVector(
                reinterpret_cast<const AbyssEngine::Matrix &>(self->player->player->transform[0]),
                Vector{(float) (self->deltaTime * 5), (float) (self->deltaTime * 2), (float) -(self->deltaTime * 5)});
        } else {
            self->egoJumpPos = AbyssEngine::AEMath::MatrixRotateVector(
                reinterpret_cast<const AbyssEngine::Matrix &>(self->player->player->transform[0]),
                Vector{(float) (self->deltaTime * 5), (float) (self->deltaTime * 2), (float) (self->deltaTime * -3)});
        }
        self->camera->translate(
            self->egoJumpPos.x, self->egoJumpPos.y, self->egoJumpPos.z);
        self->egoJumpPos = self->usingJumpDrive != 0 ? self->jumpFlash->getPosition()
                                                     : (*self->level->getLandmarks())[1]->getPosition();
        fadeOut = false;
    }

afterCam:
    if (self->usingJumpDrive != 0) {
        AbyssEngine::Transform *tr = static_cast<AbyssEngine::Transform *>(
            Globals::Canvas->TransformGetTransform(self->jumpFlash->transform));
        tr->Update(self->deltaTime, false);
    }

    Vector *camPos = self->camera->getPosition();
    float threshold = self->egoJumpPos.z + -10000.0f;
    if (camPos->z < threshold && self->usingJumpDrive == 0) {
        Array<KIPlayer *> *lm = self->level->getLandmarks();
        static_cast<PlayerJumpgate *>((*lm)[1])->activate();
        Vector playerPosition = self->player->getPosition();
        float soundThreshold = self->egoJumpPos.z + -16500.0f;
        if (playerPosition.z >= soundThreshold && self->jumpGateSoundStarted == 0) {
            Globals::sound->stop(self->player->field_0x1c);
            Globals::sound->stop(0x8d5);
            Globals::sound->stop(0x8d4);
            Globals::sound->stop(0x23);
            Globals::sound->play(0x1f, nullptr, nullptr, 0.0f);
            self->jumpGateSoundStarted = 1;
        }
    }

    if (fadeOut) {
        self->player->setSpeed(90.0f);
        self->player->setVisible(false);
        self->player->setExhaustVisible(false);
    }

    bool ended;
    if (self->usingJumpDrive != 0) {
        AbyssEngine::Transform *tr = static_cast<AbyssEngine::Transform *>(
            Globals::Canvas->TransformGetTransform(self->jumpFlash->transform));
        ended = tr->animating == 0;
    } else {
        Array<KIPlayer *> *lm = self->level->getLandmarks();
        ended = static_cast<PlayerJumpgate *>((*lm)[1])->animationEnded() != 0;
    }
    if (!ended) goto done;

    {
        if (Globals::status->getCurrentCampaignMission() == 0x2a && Globals::status->inAlienOrbit() != 0) {
            self->levelScript->setEvent(6);
            self->player->setSpeed(0.0f);
            Array<KIPlayer *> *lm = self->level->getLandmarks();
            KIPlayer *node = (*lm)[3];
            node->setPosition(25000.0f, 20000.0f, -55000.0f);
            lm = self->level->getLandmarks();
            KIPlayer *node2 = (*lm)[3];
            self->egoJumpPos = node2->getPosition();
            self->player->setPosition(self->egoJumpPos);
            PlayerEgo *p = self->player;
            p->inWormhole = 1;
            self->jumpDriveActive = 0;
            p->resetChargingDrive();
        } else {
            Globals::status->departStation(Level::programmedStation);
            self->level->setInitStreamOut();
            if (self->usingJumpDrive == 0)
                Globals::status->jumpgateUsed();
            if (Level::programmedStation->equals(Globals::status->playerStation) != 0) {
                Level::initStreamOutPosition = 1;
                Level::comingFromAlienWorld = 1;
                Globals::status->setStation(Globals::status->playerStation);
            }
            Level::programmedStation = nullptr;
            Globals::status->field_64 = self->player->player->getHitpoints();
            Globals::status->field_5c = self->player->player->getShieldHP();
            Globals::status->field_60 = self->player->player->getArmorHP();
            Globals::status->field_68 = self->player->player->getGammaHP();
            Globals::status->field_f4 = self->player->getCurrentSecondaryWeaponIndex();
            Globals::switch_to_target_setting = 1;
            self->active = 0;
            self->applicationManager->SetCurrentApplicationModule(2);
        }
    }

    return true;

done:
    return false;
}

MGame::MGame() {
    this->freeCamFinger1.x = 0.0f;
    this->freeCamFinger1.y = 0.0f;
    this->freeCamFinger1.z = 0.0f;
    this->freeCamFinger0.x = 0.0f;
    this->flShakeAmpX = 0;
    this->flShakeAmpY = 0;
    this->field_0x144 = 0;
    this->flShakePhaseX = 0;
    this->field_0x18c = 0;
    this->field_0x190 = 0;
    this->field_0x194 = 0;
    this->field_0x198 = 0;
    this->elapsedTime = 0;
    this->egoJumpPos.x = 0.0f;
    this->egoJumpPos.y = 0.0f;
    this->egoJumpPos.z = 0.0f;
    this->thrustStartX = 0;
    this->thrustStartY = 0;
    this->field_0x1c4 = 0;
    this->freeCamFinger0.y = 0.0f;
    this->freeCamFinger0.z = 0.0f;
    this->flShakePhaseY = 0;
    this->field_0x150 = 0;
    this->field_0x19c = 0;
    this->field_0x1a0 = 0;
    this->loadProgress = 100;
    this->loadingImage = -1;
    this->cameraMode = 0;
    this->frameTime = 0;
    this->field_0x38 = 0;
    this->field_0x3c = 0;
    this->startTime = 0;
    this->lastTime = 0;
    this->radar = nullptr;
    this->radio = nullptr;
    this->menuWindow = nullptr;
    this->dialogueWindow = nullptr;
    this->field_0x70 = 0;
    this->hud = nullptr;
    this->level = nullptr;
    this->levelScript = nullptr;
    this->active = 0;
    this->pauseSnapshot = 0;
    this->turretMode = 0;
    this->hudMenuOpen = 0;
    this->jumpFlash = nullptr;
    this->camera = nullptr;
    this->field_0x1e4 = 0;
    this->field_0x1d4 = 0;
    this->deltaTime = 0;
    this->player = nullptr;
    this->_b5c = 0;
    this->pauseOpen = 0;
    this->cutsceneActive = 0;
    this->jumpActive = 0;
    this->gameOverActive = 0;
    this->campaignMission = 0;
    this->starMap = nullptr;
    this->choiceWindow = nullptr;
    this->_bc8 = 0;
    this->field_0xc4 = 0;
    this->autopilotMenuOpen = 0;
    this->field_0xc6 = 0;
    this->starMapOpen = 0;
    this->choiceWindowOpen = 0;
    this->field_0xcf = 0;
    this->_bca = 0;
    this->touchesStream = 0;
    this->touchesStation = 0;
    this->jumpGateSoundStarted = 0;
    this->field_0xd8 = 0;
    this->jumpDriveActive = 0;
    this->usingJumpDrive = 0;
    this->field_0xde = 0;
    this->field_0xdf = 0;
    this->field_0x1d8 = 100.0f;
    this->field_0x1e0 = 0;
}

void MGame::OnRelease() {
    static_cast<Engine *>(this->applicationManager->GetEngine())->SetPostEffect(0x01400002, false);
    if (Globals::sound != nullptr) {
        Globals::sound->setDownPitch(false);
        Globals::sound->disableReverb();
        Globals::sound->stopAllSoundFXEvents();
    }

    delete this->level;
    this->level = nullptr;
    this->player = nullptr;
    this->cameraMode = 0;
    this->active = 0;
    this->field_0x70 = 0;
    this->deltaTime = 0;
    this->elapsedTime = 0;
    this->_b5c = 0;
    this->pauseOpen = 0;
    this->cutsceneActive = 0;
    this->jumpActive = 0;
    this->gameOverActive = 0;
    this->campaignMission = 0;
    this->startTime = 0;
    this->lastTime = 0;
    this->frameTime = 0;
    this->field_0x38 = 0;
    this->field_0x3c = 0;

    delete this->jumpFlash;
    this->jumpFlash = nullptr;
    delete this->hud;
    this->hud = nullptr;
    delete this->levelScript;
    this->levelScript = nullptr;
    delete this->radar;
    this->radar = nullptr;
    delete this->radio;
    this->radio = nullptr;

    ModStation *station = reinterpret_cast<ModStation *>(Globals::appManager->GetApplicationModule(5));
    if (station->starMap != nullptr)
        delete reinterpret_cast<ModStation *>(Globals::appManager->GetApplicationModule(5))->starMap;
    reinterpret_cast<ModStation *>(Globals::appManager->GetApplicationModule(5))->starMap = nullptr;

    delete this->menuWindow;
    this->menuWindow = nullptr;
    delete this->dialogueWindow;
    this->dialogueWindow = nullptr;
    delete this->starMap;
    this->starMap = nullptr;
    delete this->choiceWindow;
    this->choiceWindow = nullptr;

    this->autopilotMenuOpen = 0;
    this->field_0xc6 = 0;
    this->starMapOpen = 0;
    this->_bc8 = 0;
    this->touchesStream = 0;
    this->touchesStation = 0;
    this->jumpGateSoundStarted = 0;
    this->choiceWindowOpen = 0;
    this->turretMode = 0;
    this->hudMenuOpen = 0;
    this->jumpFlash = nullptr;
    this->field_0xd8 = 0;
    this->jumpDriveActive = 0;
    this->usingJumpDrive = 0;
    this->field_0xde = 0;
    this->field_0xdf = 0;

    delete this->camera;
    this->camera = nullptr;
    delete this->gameRecord;
    this->gameRecord = nullptr;

    Globals::Canvas->ReleaseAllResources();
    Globals::globals->loadFont(GameText::getLanguage());
    if (Globals::layout != nullptr) {
        Globals::layout->reload();
        Globals::imageFactory->reload();
        Globals::layout->initTip();
    }

    ArrayReleaseClasses(*this->missionInfoLines);
    delete this->missionInfoLines;
    this->missionInfoLines = nullptr;

    if (Globals::sound != nullptr)
        Globals::sound->freeAllEvents();
}

static Layout ***g_r2dHelpLayout;

static int **g_r2dPauseFlag;

static int **g_r2dRewardLayout;

static Layout ***g_r2dFadeLayout;

void MGame::OnRender2D() {
    if (!this->active)
        return;
    Globals::Canvas->Begin2d();
    if (this->pauseOpen && this->menuTouchOpen) {
        if (!this->freeCamMode || !this->freeCamDragging || this->menuWindow->pendingActivate) {
            this->menuWindow->draw();
            if (!this->freeCamMode)
                goto resetFocus;
        }
        this->level->getStarSystem()->render2D();
    resetFocus:
        static_cast<Engine *>(this->applicationManager->GetEngine())->field_0x3cc = Vector{0.5f, 0.5f, 0.0f};
        Globals::Canvas->End2d();
        return;
    }
    if (this->needsRedraw) {
        if (this->starMapOpen) {
            this->starMap->draw();
            if (Globals::layout->choiceWindowOpen)
                Globals::layout->drawHelpWindow();
            Globals::Canvas->End2d();
            return;
        }
    } else {
        this->level->getStarSystem()->render2D();
        if (this->levelScript->startSequenceOver() || !this->levelScript->startSequence())
            this->radio->draw(this->levelScript->m_nScriptTime, this->player, this->levelScript);
        Globals::Canvas->End2d();
        return;
    }

    this->level->render2D();
    if (!Globals::mouseCursorActivated)
        this->hud->drawPauseButton();
    if (((Mission *) (intptr_t) Globals::status->getCampaignMission())->getType() == 170) {
        if (!this->levelScript->getEvent())
            this->hud->drawOrbitInformation();
        this->radio->draw(this->levelScript->m_nScriptTime, this->player, this->levelScript);
        if (this->cutsceneActive)
            this->dialogueWindow->draw();
        this->field_0x110 = 0;
        this->_b5c = 0;
    } else if (this->jumpActive || this->jumpDriveActive) {
        if (Globals::status->getCurrentCampaignMission() >= 8 && !(uint8_t) this->levelScript->m_nFlags &&
            !this->jumpDriveActive && !this->player->isDockingToPlanet() && !this->levelScript->getEvent())
            this->hud->drawOrbitInformation();
        if (this->levelScript->startSequenceOver() || !this->levelScript->startSequence())
            this->radio->draw(this->levelScript->m_nScriptTime, this->player, this->levelScript);
        if (this->cutsceneActive)
            this->dialogueWindow->draw();
        this->field_0x110 = 0;
        this->_b5c = 0;
    } else if (this->gameOverActive) {
        if (this->elapsedTime >= 3001) {
            this->paintCanvas->SetColor(this->loadingTime < 4000
                                            ? (int) ((float) this->loadingTime / 4000.0f * 255.0f) - 256
                                            : -1);
            Globals::Canvas->DrawImage2D(this->loadingImage, 0, 0, 68, 68);
            if (this->loadingTime >= 4000) {
                float wave = AbyssEngine::AEMath::Sinf(
                    (float) (long long) Globals::appManager->GetSystemTimeMillis() * 0.003f);
                float pulse = AbyssEngine::AEMath::Sinf(
                    (float) (long long) Globals::appManager->GetSystemTimeMillis() * 0.003f);
                Globals::Canvas->SetColor(255, 255, 255, (unsigned char) ((wave > 0.0f ? pulse : -pulse) * 255.0f));
                String text(*Globals::gameText->getText(199), false);
                if (this->gameRecord != nullptr) {
                    Globals::globals->drawLines(reinterpret_cast<unsigned int &>(Globals::font), this->missionInfoLines,
                                                Globals::w >> 1,
                                                (Globals::h >> 1) +
                                                (Globals::Canvas->GetImage2DHeight(this->loadingImage) >> 1) + 10,
                                                true);
                } else {
                    Globals::Canvas->DrawString(
                        reinterpret_cast<unsigned int &>(Globals::font), text,
                        (Globals::w >> 1) -
                        (Globals::Canvas->GetTextWidth(reinterpret_cast<unsigned int &>(Globals::font), text) >> 1),
                        (Globals::h >> 1) + (Globals::Canvas->GetImage2DHeight(this->loadingImage) >> 1) + 10,
                        false);
                }
            }
        }
    } else {
        this->player->draw(true);
        if (!this->player->isMining() && !this->starMapOpen && (!this->player->isHacking() || this->turretMode))
            this->radar->draw(this->player->player, this->hud, this->pauseOpen ? 0 : this->deltaTime);
        if (this->cutsceneActive) {
            this->dialogueWindow->draw();
        } else {
            this->hud->draw(this->pauseOpen ? 0 : this->deltaTime,
                            this->levelScript->m_nTimeLimit - this->levelScript->m_nScriptTime, this->player,
                            this->_b5c, this->cameraMode, nextCamId(this->cameraMode));
            this->radio->draw(this->levelScript->m_nScriptTime, this->player, this->levelScript);
            this->radar->drawCurrentLock(this->hud);
            Globals::layout->drawMissionRewardMessage(false);
        }
        if (this->autopilotMenuOpen || this->field_0xc6 || this->choiceWindowOpen || this->field_0xc4)
            this->choiceWindow->draw();
        if (this->hudMenuOpen)
            this->hud->drawMenu(this->deltaTime);
    }
    Globals::layout->drawFade();
    Globals::Canvas->End2d();
}

void MGame::dialogueEvent() {
    if (this->levelScript->startSequenceOver() == 0) return;
    if (DialogueWindow::hasBriefingDialogue(Globals::status->getCurrentCampaignMission()) == 0) {
        if (((Mission *) (Globals::status->getMission()))->isCampaignMission() != 0) return;
    }
    if (((Mission *) (Globals::status->getMission()))->isEmpty() != 0) return;
    if (((Mission *) (Globals::status->getMission()))->getType() == 8) return;
    if (((Mission *) (Globals::status->getMission()))->getType() == 0xa6) return;
    if (((Mission *) (Globals::status->getMission()))->getType() == 0) return;
    if (((Mission *) (Globals::status->getMission()))->getType() == 0xb7) return;
    if (((Mission *) (Globals::status->getMission()))->isVisible() == 0) return;
    if (((Mission *) (Globals::status->getMission()))->isCampaignMission() == 0) {
        if (((Mission *) (Globals::status->getMission()))->getType() == 0xb) return;
    }
    if (this->dialogueWindow == 0) {
        this->dialogueWindow = new DialogueWindow(
            (Mission *) (intptr_t) Globals::status->getMission(), this->level, 0);
    }
    this->player->setTurretMode(0);
    this->levelScript->resetCamera((Level *) (this->level) /* level: arg lost in decomp */);
    this->player->setFreeLookMode(0);
    this->camera->enableFirstPersonCam(false);
    this->player->hideShipForFirstPersonCameraView(0);
    LevelScript *cam = this->levelScript;
    this->needsRedraw = 1;
    cam->m_nScriptTime = 0;
    this->pauseOpen = 1;
    ((MGame *) (this))->pauseSounds();
    this->cutsceneActive = 1;
}

int MGame::nextCamId(int cur) {
    int id = cur + 1;
    if (id == 2) id++;
    if (id == 1) {
        id = (Globals::status->getShip()->getFirstEquipmentOfSort(8) != 0 ||
              Globals::status->getShip()->getFirstEquipmentOfSort(0x23) != 0) &&
             !this->player->hasAutoTurret() ? 1 : 2;
    }
    if (id == 2) id++;
    if (id >= 4) {
        if (!this->player->isDockedToDockingPoint()) return 0;
        if (Globals::status->getShip()->getFirstEquipmentOfSort(8) == 0 &&
            Globals::status->getShip()->getFirstEquipmentOfSort(0x23) == 0) {
            return 3;
        }
        id = 1;
        if (this->player->hasAutoTurret()) id = 3;
    }
    return id;
}
