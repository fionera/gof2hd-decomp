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
            static_cast<Engine *>(this->applicationManager->GetEngine())->autoPilotEngaged = 0;
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

// Portrait descriptors used by MGame dialogues ([1] = planet landing refusal).
static int gMGameFaces[2][10] = {{4, 2, 0, 2, 2, 0, 2, 3, 3, 10}, {10, 0, 0, 1, 0}};

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
                                                          Globals::gameText->getText(1603), gMGameFaces[1]);
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

void MGame::OnUpdate() {
    int delta;
    int t = this->applicationManager->GetElapsedTimeMillis();
    if (t < 0x97) {
        delta = (this->applicationManager->GetElapsedTimeMillis() < 0)
                    ? 0
                    : (this->applicationManager->GetElapsedTimeMillis() < 0x97
                           ? this->applicationManager->GetElapsedTimeMillis()
                           : 0x96);
    } else {
        delta = 0x96;
    }

    this->deltaTime = delta;

    this->frameTime += delta;

    if (this->jumpDriveActive != 0)
        this->updateJumpScene();
    this->gameOverCheck();
    this->successCheck();
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

void MGame::dockEvent(int p1, int p2) {
    (void) p1;
    (void) p2;
    float pos[4];
    ((PlayerEgo *) (pos))->getPosition();
    this->touchesStream = this->level->collideStream(*(Vector *) pos);
    ((PlayerEgo *) (pos))->getPosition();
    this->touchesStation = this->level->collideStation(*(Vector *) pos);

    Status *status = (Status *) &Globals::status;
    Mission *m = Globals::status->getMission();
    bool special = ((Mission *) (m))->isEmpty() != 0 ||
                   ((Mission *) (Globals::status->getMission()))->getType() == 0xb ||
                   ((Mission *) (Globals::status->getMission()))->getType() == 0 ||
                   ((Mission *) (Globals::status->getMission()))->getType() == 0xbd ||
                   ((Mission *) (Globals::status->getMission()))->getType() == 0xab ||
                   ((Mission *) (Globals::status->getMission()))->getType() == 0xac;

    if (!special) {
        if ((this->touchesStation != 0 || this->touchesStream != 0) &&
            this->player->isAutoPilot() != 0) {
            this->hud->hudEvent(0x15, this->player, 0);
        }

        return;
    }

    if (this->touchesStream != 0) {
        if (this->player->goingToStream() != 0 &&
            this->player->isDockingToStream() == 0 &&
            this->player->isDockedToStream() == 0) {
            this->player->dockToStream(0);
            this->freeCamDragging = 0;
            this->needsRedraw = 1;

            return;
        }
        if (this->touchesStream != 0) {
            {
                Player *pl = (Player *) this->player->player;
                Globals::status->field_64 = pl->getHitpoints();
                Globals::status->field_5c = pl->getShieldHP();
                Globals::status->field_60 = pl->getArmorHP();
                Globals::status->field_68 = pl->getGammaHP();
            }
            Globals::status->field_f4 = this->player->getCurrentSecondaryWeaponIndex();
            int autop = this->player->isAutoPilot();
            int *autoFlag = g_deAutoFlag;
            if (*autoFlag == 0 || autop == 0) {
                if (this->starMapOpen != 0) {
                    this->frameTime = 0;

                    return;
                }
                this->player->isAutoPilot();
                if (this->player->goingToStream() == 0) {
                    return;
                }
                if (*autoFlag == 0) {
                    this->freeCamDragging = 0;
                    this->needsRedraw = 1;
                    if (this->starMap == 0) {
                        StarMap *sm = new StarMap(false, nullptr, false, -1);
                        this->starMap = sm;
                    }
                    Engine *eng = (Engine *) this->applicationManager->GetEngine();
                    ((Engine *) (eng))->SetPostEffect(g_dePostEffect, 0);
                    this->starMap->initLights();
                    this->starMap->setJumpMapMode(1, 0);
                    this->player->setAutoPilot(0);
                    this->pauseOpen = 1;
                    this->starMapOpen = 1;
                    ((MGame *) (this))->pauseSounds();
                    this->needsRedraw = 1;

                    return;
                }
                if (this->autopilotMenuOpen != 0) { return; }
                this->freeCamDragging = 0;
                ChoiceWindow *cw = this->choiceWindow;
                this->needsRedraw = 1;
                if (cw == 0) {
                    cw = new ChoiceWindow();
                    this->choiceWindow = cw;
                }
                {
                    String *txt = ((GameText *) (*g_gameText))->getText(g_deTextB);
                    String name = ((Station *) (Globals::status->getStation()))->getName();
                    String sPrefix, sSuffix, t1, t2, t3, result;
                    sPrefix.ctor_char((const char *) (intptr_t) g_deLitB0, false);
                    t1 = *(String *) txt + sPrefix;
                    t2 = t1 + name;
                    sSuffix.ctor_char((const char *) (intptr_t) g_deLitB1, false);
                    t3 = t2 + sSuffix;
                    result = t3 + *(String *) txt;
                    ((ChoiceWindow *) (cw))->set(*(String *) &result, true);
                    ((ChoiceWindow *) (cw))->left();
                }
            } else {
                if (this->autopilotMenuOpen != 0) { return; }
                this->freeCamDragging = 0;
                ChoiceWindow *cw = this->choiceWindow;
                this->needsRedraw = 1;
                if (cw == 0) {
                    cw = new ChoiceWindow();
                    this->choiceWindow = cw;
                }
                {
                    String *txt = ((GameText *) (*g_gameText))->getText(g_deTextA);
                    String name = ((Station *) (Globals::status->getStation()))->getName();
                    String sPrefix, sSuffix, t1, t2, t3, result;
                    sPrefix.ctor_char((const char *) (intptr_t) g_deLitA0, false);
                    t1 = *(String *) txt + sPrefix;
                    t2 = t1 + name;
                    sSuffix.ctor_char((const char *) (intptr_t) g_deLitA1, false);
                    t3 = t2 + sSuffix;
                    result = t3 + *(String *) txt;
                    ((ChoiceWindow *) (cw))->set(*(String *) &result, true);
                    ((ChoiceWindow *) (cw))->left();
                }
            }
            this->pauseOpen = 1;
            this->autopilotMenuOpen = 1;
            ((MGame *) (this))->pauseSounds();
            this->player->setAutoPilot(0);

            return;
        }
    }

    if (this->touchesStation == 0) {
        int tgt = this->player->getAutoPilotTarget();
        Array<KIPlayer *> *lm = this->level->getLandmarks();
        if (tgt != (int) (intptr_t) lm->data() ||
            this->player->collidesWithStation() == 0) {
            return;
        }
    }

    int cm = Globals::status->getCurrentCampaignMission();
    if (cm > 0x30 && Globals::status->getCurrentCampaignMission() < 0x37) {
        if (Station_getIndex(Globals::status->getStation()) != 0x4a) {
            this->hud->hudEvent(0x15, this->player, 0);

            return;
        }
    }
    if (this->player->goingToStation() != 0 &&
        Globals::status->inAlienOrbit() == 0 &&
        ((Status *) (*(Station **) status))->inEmptyOrbit() == 0) {
        Globals::achievements->checkForNewMedal(this->player);
        **g_deAlienFlag = 0;

        {
            Player *pl = (Player *) this->player->player;
            Globals::status->field_64 = pl->getHitpoints();
            Globals::status->field_5c = pl->getShieldHP();
            Globals::status->field_60 = pl->getArmorHP();
            Globals::status->field_68 = pl->getGammaHP();
        }
        this->applicationManager->SetCurrentApplicationModule(5);
        this->active = 0;
    }
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

void MGame::OnTouchEnd(int p1, int p2, void *touchId) { // lint: void_ptr -- exported override signature (Pv)
    // lint: void_ptr -- exported override signature (Pv)
    if (this->activeTouchId == touchId) {
        this->activeTouchId = 0;
        this->field_0xc4 = 0;
        this->autopilotMenuOpen = 0;
        this->field_0xc6 = 0;
    }

    int wasAutoPilot = this->player->isAutoPilot();
    this->flFastForwardWeight = 1.0f;
    TFC_setFastForwardMode(this->camera, 0);
    reinterpret_cast<uint8_t &>(this->player->miningUpVec) = 1; // TODO(decomp): byte @0x1c8 of PlayerEgo, unverified

    unsigned hr = 0;
    if (this->pauseOpen == 0) {
        hr = this->hud->touchEnd(p1, p2, touchId);
        this->hudTouchFlags = hr;
        if (hr != 0) {
            this->touch0Id = 0;
            this->touch1Id = 0;
        }
    }

    (void) wasAutoPilot;
    (void) hr;
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
                                self->thrustBase = (int) self->player->getThrust();
                                apply = true;
                            }
                        } else {
                            apply = true;
                        }
                        if (apply) {
                            self->thrustEngaged = 1;
                            float divisor = reinterpret_cast<float &>(Globals::layout->field_0x2f8);
                            float t = ((float) self->thrustStartY - (float) y -
                                       (float) self->thrustResetX) / divisor +
                                      (float) self->thrustBase;
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
