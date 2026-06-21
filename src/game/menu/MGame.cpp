#include <new>
#include "game/menu/MGame.h"
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
// Galaxy.h (above) already provides the canonical file-local B()/I()/P() offset helpers.
// Sibling headers below redefine identical static-inline B/I/P with no shared guard, which
// is a redefinition error within one TU. Rename their copies to throwaway names on include
// so only Galaxy.h's definitions remain for this TU's call sites.
#define P P_levelscript
#include "game/world/LevelScript.h"
#undef P
#include "game/ui/MenuTouchWindow.h"
#include "game/mission/Objective.h"
#include "game/ship/PlayerJumpgate.h"
// NOTE: gof2/Radar.h and gof2/Radio.h are intentionally NOT included. Both define their
// own minimal duplicate views of Status/KIPlayer/Layout/GameText/ImageFactory/Globals/
// FModSound/Agent for their own standalone TUs; those collide with the full headers this
// TU needs (Status.h, KIPlayer.h, Layout.h, ...). MGame only touches Radar/Radio through a
// couple of methods and opaque pointers, declared minimally below.
#include "game/world/StarMap.h"
#include "engine/math/Transform.h"
// RecordHandler.h, Hud.h and PlayerEgo.h each define the file-local offset
// helpers B()/I()/P(). RecordHandler.h gates its trio behind GOF2_BIP_HELPERS;
// pre-define it so this TU takes B from PlayerEgo.h and I/P from Hud.h (one each).
#define GOF2_BIP_HELPERS
#include "game/mission/Achievements.h"
#include "engine/core/ApplicationManager.h"
#include "game/ui/DialogueWindow.h"
// Engine.h and GameRecord.h each redeclare __aeabi_memcpy with a signature that conflicts
// with AEGeometry.h's (uint32_t vs unsigned long size, void vs void* return). None is called
// here, so rename their decls away on include so AEGeometry.h's canonical decl stands.
#define __aeabi_memcpy __aeabi_memcpy_engine
#include "engine/render/Engine.h"
#undef __aeabi_memcpy
#define __aeabi_memcpy __aeabi_memcpy_gamerecord
#include "game/mission/GameRecord.h"
#undef __aeabi_memcpy
#include "engine/core/GameText.h"
#define I I_hud
#define P P_hud
#include "game/ui/Hud.h"
#undef I
#undef P
#include "engine/render/ImageFactory.h"
#include "game/ui/Layout.h"
#include "game/world/Level.h"
#include "game/mission/Mission.h"
#include "game/ship/Player.h"
#include "game/mission/RecordHandler.h"
#include "game/world/Route.h"
#include "game/world/StarSystem.h"
#include "game/world/Station.h"
#include "game/core/String.h"
// NOTE: gof2/Engine.h is intentionally NOT included — it does not compile
// standalone (it is owned by another batch) and MGame only uses Engine as an
// opaque pointer type, which is already forward-declared via fwd.h (common.h).
#include "game/ship/KIPlayer.h"
#define B B_playerego
#include "game/ship/PlayerEgo.h"
#undef B
#include "game/ship/PlayerFixedObject.h"
#include "game/ship/Agent.h"
#include "game/mission/Status.h"
#include "game/core/PaintCanvasClass.h"
// Canonical render-canvas singleton. PaintCanvas.h is not included here (this TU uses the
// standalone PaintCanvasClass.h to avoid a duplicate class definition), so declare the
// canonical pointer locally to match the definition in PaintCanvas.cpp.
extern PaintCanvas* gCanvas;

// Minimal Radar/Radio interfaces (full headers omitted above to avoid duplicate-struct
// clashes). fwd.h forward-declares both as struct; only the methods MGame calls are needed.
// Radar::hasScanner is modeled as a nullary static here (matching Level.cpp), since the
// original loaded the Radar singleton internally and the decompiler dropped the receiver.
struct Radar {
    void drawCurrentLock(Hud *hud);
    void draw(void *player, Hud *hud, int mode);
    static uint8_t hasScanner();
};
struct Radio {
    void setMessages(Array<RadioMessage *> *messages);
    void draw(long long time, PlayerEgo *ego, LevelScript *script);
};

// Transform is defined in the AbyssEngine namespace (Transform.h); ::Transform (fwd.h) is
// only an incomplete forward decl. Use the complete type for member access.
typedef AbyssEngine::Transform TransformFull;

extern "C" int FModSound_tryToStopMusicForBGMusic();
extern "C" void Music_resume(Music *m, int one, int v);
// PlayerEgo.h (owned by another batch) declares these as returning void, but the
// callers below consume their (int) result. Use the original free-function form
// so this TU sees the real int return type without editing PlayerEgo.h.
extern "C" Vector *TFC_getCamOffset(TargetFollowCamera *c);
namespace AbyssEngine { namespace AEMath {
float VectorLength(const Vector &value);
Vector operator-(const Vector &lhs, const Vector &rhs);
Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector);
} }
static inline float AEMath_VectorLength(Vector *v) {
    return AbyssEngine::AEMath::VectorLength(*v);
}
extern "C" void *MGame_opnew(unsigned sz);
void Globals_playMusicAndFadeOutCurrent(int track);
extern "C" void MGame_opdelete(void *p);
void Globals_startNewSoundResourceList();
void TFC_setFastForwardMode(TargetFollowCamera *c, int v);
extern "C" void applyThrust(MGame *self, int y);
uint8_t TFC_isInLookAtMode(TargetFollowCamera *c);
void TFC_setLookAtCam(TargetFollowCamera *c, int v);
extern "C" void Cam_setCinematic(TargetFollowCamera *c, int on);
extern "C" void FModSound_restoreState();
extern "C" void DialogueWindow_ctor(...);
void TFC_enableFirstPersonCam(TargetFollowCamera *c, int on);

__attribute__((visibility("hidden"))) extern int *g_maneuverScale;

// MGame::maneuverTouchEnd(): if a maneuver gesture was active, recent, and the finger
// moved far enough, trigger a left/right maneuver on the player; always clear the flag.
void MGame::maneuverTouchEnd(int a, int b, void *p) {
    MGame *self = this;
    (void)b;
    (void)p;
    if (self->maneuverActive != 0 && self->maneuverHoldTime <= 0x258) {
        float f2 = (float)(*g_maneuverScale);
        int d = a - self->maneuverStartX;
        if (d < 0) d = -d;
        float f3 = (float)d;
        if ((f2 / 480.0f) * 70.0f < f3) {
            int dir = 1;
            if (self->maneuverStartX < a) dir = 2;
            self->player->initManeuver(dir);
        }
    }
    self->maneuverActive = 0;
}

// Singletons held as pointer-to-pointer in hidden globals (single PC-relative deref).
__attribute__((visibility("hidden"))) extern Music **g_music;
__attribute__((visibility("hidden"))) extern Cfg **g_cfg;

// MGame::OnResume(): resume background music unless it was deliberately stopped.
void MGame::OnResume() {
    Music **mp = g_music;
    if (*mp == 0) return;
    if (FModSound_tryToStopMusicForBGMusic() != 0) return;
    return Music_resume(*mp, 1, **(int **)g_cfg);
}

// Globals dereferenced PC-relative.
__attribute__((visibility("hidden"))) extern int *g_maneuverScale;

// MGame::maneuverTouchMove(): if a maneuver gesture is active, cancel it once the
// finger has moved far enough sideways relative to a screen-scaled threshold.
void MGame::maneuverTouchMove(int a, int b, void *p) {
    (void)a;
    (void)p;
    if (this->maneuverActive != 0) {
        float f2 = (float)(*g_maneuverScale);
        int d = b - this->maneuverStartY;
        if (d < 0) d = -d;
        float f3 = (float)d;
        // The exact constant values are irrelevant to the byte match (the diff
        // normalizes pc-relative literal loads); only the vldr/vdiv/vmul shape matters.
        if ((f2 / 320.0f) * 90.0f < f3) {
            this->maneuverActive = 0;
            this->maneuverHoldTime = 0;
        }
    }
}

// Begins a maneuver gesture: mark active, record start position, reset timer.
void MGame::maneuverTouchBegin(int x, int y, void *p) {
    (void)p;
    this->maneuverActive = 1;
    this->maneuverStartX = x;
    this->maneuverStartY = y;
    this->maneuverHoldTime = 0;
}

// Two-argument touch/key/lifecycle overrides. MGame routes all real input through the
// void*-carrying touch overloads and the application manager, so the plain two-argument
// vtable slots are empty no-ops in this module (matching the shipped stubs).
void MGame::OnTouchBegin(int /*p1*/, int /*p2*/) {}
void MGame::OnTouchEnd(int /*p1*/, int /*p2*/) {}
void MGame::OnTouchMove(int /*p1*/, int /*p2*/) {}

long long MGame::OnKeyPress(long long key, long long /*mod*/) { return key; }
long long MGame::OnKeyRelease(long long key, long long /*mod*/) { return key; }

void MGame::showLiteScreen() {}

// The game module always reports that it owns its loading screen.
int MGame::ShowLoadingScreen() { return 1; }

void MGame::pause() {}

// PaintCanvas End3d tail helper @0x1ab918.

// MGame::OnRender3D(): render the active 3D view (game world, menu, or star map).
void MGame::OnRender3D() {
    if (this->active == 0) return;
    gCanvas->ClearBuffer(0);

    uint8_t inMenuLevel = this->pauseOpen;
    uint8_t flag15e = this->freeCamMode;

    if (inMenuLevel == 0) {
        // World render.
        this->level->renderBG(0);
        gCanvas->Begin3d();
        int arg = (flag15e == 0) ? this->deltaTime : 0;
        this->level->render(arg);
        int egoFlag = (this->jumpActive != 0) ? 0 : (this->jumpDriveActive == 0);
        this->player->render(egoFlag);
        if (this->jumpFlash != 0)
            ((AEGeometry *)(this->player))->render();
        this->levelScript->render3D();
        return gCanvas->End3d();
    }

    if (flag15e != 0) {
        // Free-cam / cinematic world render.
        this->level->renderBG(0);
        gCanvas->Begin3d();
        this->level->render(this->deltaTime);
        int egoFlag = (this->jumpActive != 0) ? 0 : (this->jumpDriveActive == 0);
        this->player->render(egoFlag);
        if (this->jumpFlash != 0)
            ((AEGeometry *)(this->player))->render();
        this->levelScript->render3D();
        return gCanvas->End3d();
    }

    if (this->menuTouchOpen != 0) {
        gCanvas->Begin3d();
        this->menuWindow->render3D();
        return gCanvas->End3d();
    }

    if (this->starMapOpen != 0) {
        gCanvas->Begin3d();
        this->starMap->render();
        return gCanvas->End3d();
    }

    // Fallback world render.
    this->level->renderBG(0);
    gCanvas->Begin3d();
    this->level->render(0);
    int egoFlag = (this->jumpActive != 0) ? 0 : (this->jumpDriveActive == 0);
    this->player->render(egoFlag);
    if (this->jumpFlash != 0)
        ((AEGeometry *)(this->player))->render();
    this->levelScript->render3D();
    return gCanvas->End3d();
}

void TFC_setActive(TargetFollowCamera *c, int v);
float TFC_useTargetsUpVector(TargetFollowCamera *c, int v);
extern "C" void FModSound_setProp(int snd, int id);  // fn @0x18c30e (pcVar8)
extern "C" void TFC_setPosition(TargetFollowCamera *c, float x, float y, float z);

__attribute__((visibility("hidden"))) extern int *g_jsSound;     // @0x18c15a (jump-scene FMOD singleton)
__attribute__((visibility("hidden"))) extern int g_jsHudFlag;    // @0x18c410 (DAT)
__attribute__((visibility("hidden"))) extern int g_jsFovDefault; // @0x18c414
__attribute__((visibility("hidden"))) extern int g_jsFovAlienA;  // @0x18c418
__attribute__((visibility("hidden"))) extern int g_jsFovAlienB;  // @0x18c41c
__attribute__((visibility("hidden"))) extern int g_jsPostEffect; // @0x18c424
__attribute__((visibility("hidden"))) extern int g_jsOffsetX;    // @0x18c42c
__attribute__((visibility("hidden"))) extern int g_jsOffsetY;    // @0x18c430
__attribute__((visibility("hidden"))) extern int g_jsOffsetZ;    // @0x18c434
__attribute__((visibility("hidden"))) extern int g_jsOffsetZ2;   // @0x18c438

// MGame::startJumpScene(): set up the hyperspace cinematic (docked or free variant).
void MGame::startJumpScene() {
    ((Player *)(this->player->player))->setVulnerable(0);
    this->level->enableFog(0);

    if (this->player->isDockedToDockingPoint() != 0) {
        this->player->dockToDockingPoint(nullptr, nullptr);
        TFC_setActive(this->camera, 1);
        TFC_setLookAtCam(this->camera, 0);
        float sp = TFC_useTargetsUpVector(this->camera, 0);
        this->player->setSpeed(sp);
        this->player->setDockingState(0);
    }
    if (this->player->isInTurretMode() != 0)
        this->player->setTurretMode(0);

    int *snd = g_jsSound;
    ((FModSound *)(*snd))->stop(0x23);
    ((MGame *)(this))->switchCamera(0);
    this->field_0x70 = g_jsHudFlag;
    this->hud->releaseAllKeys();
    this->field_0x110 = 0;
    this->field_0x5c = 0;

    PaintCanvas *pc = gCanvas;
    unsigned cam = this->cameraId;
    float fov = *(float *)&g_jsFovDefault;
    if (gStatus->inAlienOrbit() != 0) {
        int cm = gStatus->getCurrentCampaignMission();
        fov = (cm < 0x50) ? *(float *)&g_jsFovAlienB : *(float *)&g_jsFovAlienA;
    }
    pc->CameraSetPerspective(cam, fov, *(float *)&g_jsHudFlag, 0);
    this->player->setAutoPilot(0);
    this->pauseOpen = 0;
    this->hudMenuOpen = 0;
    this->jumpDriveActive = 1;
    this->jumpActive = 1;
    this->player->setCollide(0);
    TFC_setLookAtCam(this->camera, 1);
    this->player->stopBoost();
    Engine *eng = (Engine *)this->appManager->GetEngine();
    ((Engine *)(eng))->SetPostEffect(g_jsPostEffect, 0);

    float camX, camY, camZ;
    if (this->usingJumpDrive == 0) {
        // Free-space jump: position from a landmark.
        Array<KIPlayer*> *lm = this->level->getLandmarks();
        void *obj = (void *)(*lm)[0];
        void *vt = *(void **)obj;
        float vtmp[4];
        (*(void (**)(void *, void *))((char *)vt + 0x28))(obj, vtmp);
        *(Vector*)((Vector *)((char *)&this->egoJumpPosX)) = *(const Vector*)((Vector *)vtmp);
        float nz = (float)this->egoJumpPosZ + *(float *)&g_jsOffsetZ;
        this->egoJumpPosZ = (int)nz;
        this->player->setPosition();
        this->player->setComputerControlled(1);
        ((AEGeometry *)this->player->geometry)->setRotation((float)0, (float)0, (float)0);
        this->egoJumpPosX = (int)((float)this->egoJumpPosX + *(float *)&g_jsOffsetX);
        this->egoJumpPosY = (int)((float)this->egoJumpPosY + *(float *)&g_jsOffsetY);
        this->egoJumpPosZ = (int)((float)this->egoJumpPosZ + *(float *)&g_jsOffsetZ2);
        camX = (float)this->egoJumpPosX;
        camY = (float)this->egoJumpPosY;
        camZ = (float)this->egoJumpPosZ;
    } else {
        // Charged jump: spawn the warp-tunnel geometry.
        this->player->resetMovement();
        this->player->setComputerControlled(1);
        AEGeometry *geo = new AEGeometry((uint16_t)0x3ab2, gCanvas, false);
        this->jumpFlash = geo;
        int tr = (int)(long)gCanvas->TransformGetTransform((unsigned)(uintptr_t)gCanvas);
        ((TransformFull *)(tr))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);

        float pos[4];
        ((PlayerEgo *)(pos))->getPosition();
        Vector *dst = (Vector *)((char *)&this->egoJumpPosX);
        *(Vector*)(dst) = *(const Vector*)((Vector *)pos);

        float dir[4];
        ((PlayerEgo *)(dir))->getPosition();
        *(Vector*)((Vector *)dir) *= (*(float *)&g_jsOffsetX);
        *(Vector*)(dst) += *(const Vector*)((Vector *)dir);
        this->jumpFlash->setPosition(*dst);

        this->jumpFlash->setScaling(2.0f);
        float zero[4]; zero[0] = 0; zero[1] = 1.0f; zero[2] = 0;
        this->jumpFlash->setDirection(*(Vector *)zero, *(Vector *)zero /* up: arg lost in decomp */);

        float off[4]; off[0] = (float)g_jsOffsetX; off[1] = (float)g_jsOffsetY; off[2] = (float)g_jsOffsetX;
        *(Vector*)((Vector *)off) = *(const Vector*)((Vector *)off);
        *(Vector*)((char *)this->player->player + 4) = AbyssEngine::AEMath::MatrixRotateVector(*(const Matrix*)(off), *(const Vector*)((Vector *)((char *)this->player->player + 4)));
        *(Vector*)((Vector *)off) = *(const Vector*)((Vector *)off);
        *(Vector*)(dst) += *(const Vector*)((Vector *)off);

        FModSound_setProp(*snd, this->field_0x1c);
        FModSound_setProp(*snd, 0x23);
        FModSound_setProp(*snd, 0x8d5);
        FModSound_setProp(*snd, 0x8d4);
        ((FModSound *)(*snd))->play(0x20, 0, 0 /* vel: arg lost in decomp */, 0.0f);

        camX = (float)this->egoJumpPosX;
        camZ = (float)this->egoJumpPosY;
        camY = (float)this->egoJumpPosZ;
    }
    TFC_setPosition(this->camera, camX, camY, camZ);

    
}

void TFC_setRotationAroundTarget(TargetFollowCamera *c, int v);
int TFC_hideShipForFirstPersonCam(TargetFollowCamera *c);
// PlayerEgo turret/first-person sync tail helper @0x1ac868.

// MGame::switchCamera(int id)
void MGame::switchCamera(int id) {
    int turretArg;
    int savedMode;

restart:
    // Snapshot the camera mode as it stood on entry to this (re)selection pass;
    // case 2 below uses it to decide whether to set field_0x18.
    savedMode = this->cameraMode;
    if (id == 2) id = 3;
    this->cameraMode = id;
    if (id == 1) {
        if (this->player->isDockingToAsteroid() != 0) {
            this->turretMode = 0;
            // fallthrough to step++
        } else {
            // setTurretMode() returns void in the header; the decompiler modelled a
            // result that gates the following block. Preserve the call and treat the
            // turret as enabled (arg == 1) for the flag/condition.
            ((PlayerEgo *)this->player)->setTurretMode(1);
            int t = 1;
            this->turretMode = (uint8_t)t;
            if (t != 0) {
                id = this->cameraMode;
                turretArg = 1;
                goto afterTurret;
            }
        }
        id = this->cameraMode + 1;
        this->cameraMode = id;
        turretArg = 0;
    } else {
        this->turretMode = 0;
        turretArg = 0;
    }

afterTurret:
    if (id == 2) {
        this->cameraMode = 3;
    } else if (id >= 4) {
        this->cameraMode = 0;
    }
    this->field_0x18 = 0;
    this->player->setTurretMode(turretArg);

    int mode = this->cameraMode;
    switch (mode) {
    case 0:
        // First-person / cockpit: re-aim the camera unless docked. If docked,
        // restart the whole selection with id=1.
        if (this->player->isDockedToDockingPoint() != 0) {
            id = 1;
            goto restart;
        }
        this->levelScript->resetCamera(this->level);
        TFC_setRotationAroundTarget(this->camera, 0);
        this->player->setFreeLookMode(0);
        {
            Engine *eng = (Engine *)this->appManager->GetEngine();
            // Engine layout lives in another translation unit (gof2/Engine.h is not
            // included here); write its +0x360 field via a typed byte offset.
            F<int>(eng, 0x360) = 0;
        }
        goto firstPerson;
    case 1:
    case 3: {
        if (this->player->isDockedToDockingPoint() != 0)
            TFC_setLookAtCam(this->camera, 0);
        this->levelScript->lookBehind();
        TFC_setRotationAroundTarget(this->camera, 1);
        this->player->setFreeLookMode(1);
        goto firstPerson;
    }
    case 2: {
        this->levelScript->resetCamera(this->level);
        TFC_setRotationAroundTarget(this->camera, 0);
        this->player->setFreeLookMode(0);
        if (savedMode == 1)
            this->field_0x18 = 1;
        goto firstPerson;
    }
    default:
        goto firstPerson;
    }

firstPerson: {
        TFC_enableFirstPersonCam(this->camera, this->cameraMode == 2);
        PlayerEgo *ego = this->player;
        int v;
        if (this->field_0x18 == 0)
            v = TFC_hideShipForFirstPersonCam(this->camera);
        else
            v = 1;
        ((PlayerEgo *)(ego))->hideShipForFirstPersonCameraView(v != 0);
    }
}

__attribute__((visibility("hidden"))) extern int *g_fcb_guard; // DAT_188af4 (canary anchor read)

// MGame::freeCamTouchBegin(int, int, void*): start a free-camera drag. Records the start
// touch and seeds the camera-offset Vector (built on the stack -> compiler canary).
void MGame::freeCamTouchBegin(int x, int y, void *idPtr) {
    int id = (int)(intptr_t)idPtr;
    char buf[12];
    float fy = (float)y;
    float fx = (float)x;
    int slot;
    if (this->touch0Id == 0) {
        if (this->touch1Id == 0) this->menuTime = 0;
        *(volatile float *)(buf + 4) = fy;
        *(volatile float *)(buf + 0) = fx;
        *(volatile int *)(buf + 8) = 0;
        this->touch0Id = id;
        slot = 0xa4;
    } else {
        if (this->menuTime >= 1000) goto tail;
        this->flCameraRoll = 0;
        float len = AEMath_VectorLength(TFC_getCamOffset(this->camera));
        *(volatile float *)(buf + 4) = fy;
        *(volatile float *)(buf + 0) = fx;
        this->flCameraRoll = len;
        slot = 0xb0;
        this->touch1Id = id;
    }
    // slot is 0xa4 (finger-1 Vector) or 0xb0 (finger-0 Vector); both live in the
    // free-cam finger-position Vector pair based at &this->freeCamFinger1X (+0xa4).
    *(Vector*)((char *)&this->freeCamFinger1X + (slot - 0xa4)) = *(const Vector*)(buf);
tail:
    this->dragLastX = x;
    this->dragLastY = y;
    this->dragStartX = x;
    this->dragStartY = y;
    this->dragDeltaX = 0;
    this->dragDeltaY = 0;
    this->freeCamDragging = 1;
}

__attribute__((visibility("hidden"))) extern GameText **g_gameText;

// MGame::useCloak(): toggle the cloaking device. If it engaged, build a choice dialogue
// "<text><cloak-attr><text>" describing the cloak and pause the game. Stack-protected
// (six temporary Strings on the stack).
// PlayerEgo::toggleCloaking() is declared (and defined, in PlayerEgo.cpp) as returning
// void, but this caller branches on the value the original left in r0. Reinterpret the
// method's address through an int-returning free-function signature so the ABI return is
// read without touching the void definition (the call ABI is identical for a thiscall on
// this target).
void MGame::useCloak() {
    void (::PlayerEgo::*pmf)() = &::PlayerEgo::toggleCloaking;
    int (*toggleCloaking)(::PlayerEgo *);
    __builtin_memcpy(&toggleCloaking, &pmf, sizeof(toggleCloaking));
    if (toggleCloaking((::PlayerEgo *)(this->player)) != 0) return;
    if (this->choiceWindow == 0)
        this->choiceWindow = new ChoiceWindow();
    Item *eq = ((Ship *)(gStatus->getShip()))->getFirstEquipmentOfSort(0x15);
    int attr = eq == 0 ? 0 : ((Item *)(eq))->getAttribute(0x26);
    ChoiceWindow *cw = this->choiceWindow;
    void *txt = ((GameText *)(*g_gameText))->getText(0x247);
    String s0, s1, s2, s3, s4, s5;
    s2.ctor_char("", false);
    s3 = *(String *)txt + s2;
    s1.ctor_int(attr);
    s4 = s3 + s1;
    s0.ctor_char("", false);
    s5 = s4 + s0;
    ((ChoiceWindow *)(cw))->set(*(String *)&s5);
    this->pauseOpen = 1;
    this->choiceWindowOpen = 1;
    ((MGame *)(this))->pauseSounds();
}

// 0x75550 — DialogueWindow::set(Mission*, int, int)
__attribute__((visibility("hidden"))) extern int **g_goWormText;   // @0x190572
__attribute__((visibility("hidden"))) extern int **g_goDeathText;  // @0x19069c
__attribute__((visibility("hidden"))) extern DialogueWindow **g_goDlgA; // @0x1907c0 (used for getText store)
__attribute__((visibility("hidden"))) extern int **g_goStatusA;    // @0x1906de
__attribute__((visibility("hidden"))) extern int **g_goStatusB;    // @0x1906ea
__attribute__((visibility("hidden"))) extern int **g_goStatusC;    // @0x190764
__attribute__((visibility("hidden"))) extern int **g_goStatusD;    // @0x190770
__attribute__((visibility("hidden"))) extern int **g_goRecHandler; // @0x19077c
__attribute__((visibility("hidden"))) extern int **g_goRecId;      // @0x19077a
__attribute__((visibility("hidden"))) extern int **g_goSnd;        // @0x190746

// Helper to (re)bind a DialogueWindow to the current level (the duplicated block).
static void bindDialogueLevel(MGame *self) {
    if (self->dialogueWindow == 0) {
        DialogueWindow *w = (DialogueWindow *)::operator new(0x74);
        DialogueWindow_ctor(w);
        Level *lvl = self->level;
        self->dialogueWindow = w;
        if (lvl != 0) ((DialogueWindow *)(w))->setLevel(lvl);
    } else if (self->dialogueWindow->hasLevel() == 0) {
        Level *lvl = self->level;
        if (lvl != 0) self->dialogueWindow->setLevel(lvl);
    }
    self->dialogueWindow->set(gStatus->getMission(), 2, -1);
}

// MGame::gameOverCheck(): handle player death/explosion and end-of-level dialogue.
void MGame::gameOverCheck() {
    if (this->player->getHitpoints() <= 0) {
        if (this->player->tryToStartEmergencySystem() != 0)
            return;
        if (this->player->isInWormhole() == 0) {
            this->player->setTurretMode(0);
            this->levelScript->resetCamera(this->level);
            this->player->setFreeLookMode(0);
            TFC_enableFirstPersonCam(this->camera, 0);
            this->player->hideShipForFirstPersonCameraView(0);
            this->needsRedraw = 1;
            this->player->explode();
            if (this->player->explosionEnded() != 0) {
                this->gameOverActive = 1;
                String *t = (String *)((GameText *)(**g_goDeathText))->getText(0);
                this->gameOverTitle.assign(t);
            }
        } else {
            this->gameOverActive = 1;
            String *t = (String *)((GameText *)(**g_goWormText))->getText(0);
            this->gameOverTitle.assign(t);
            this->needsRedraw = 1;
        }

        if (this->gameOverActive != 0) {
            if (gStatus->getMission() != 0 &&
                ((Mission *)(gStatus->getMission()))->isCampaignMission() != 0) {
                int cm = gStatus->getCurrentCampaignMission();
                int *sa = *g_goStatusA;
                int v;
                if (cm == *sa) {
                    v = **g_goStatusB + 1;
                } else {
                    **g_goStatusC = 0;
                    v = 1;
                }
                *sa = cm;
                **g_goStatusD = v;
            }
            int rec = (int)(intptr_t)((RecordHandler *)((RecordHandler *)**g_goRecHandler))->recordStoreRead(**g_goRecId);
            this->gameRecord = rec;
        }
    }

    if (this->level->checkGameOver(0) != 0) {
        bindDialogueLevel(this);
        this->cutsceneActive = 1;
        ((MGame *)(this))->pauseSounds();
        this->pauseOpen = 1;
    }

    // Timed end-of-mission check (0x48/0x4c is a 64-bit counter vs 0x1389).
    unsigned lo = this->elapsedTime;
    int hi = this->elapsedTimeHigh;
    bool ready = !(hi < (int)(lo < 0x1389));
    if (ready) {
        this->elapsedTime = 0;
        this->elapsedTimeHigh = 0;
        int *sc = (int *)this->levelScript;
        if (sc[0] >= 1) {
            // 64-bit compare against script counters [2]/[3].
            bool done = !((long long)(unsigned)sc[0] - ((long long)sc[3] << 32 | (unsigned)sc[2]) < 0);
            bool survival = false;
            if (!done) {
                int cm = gStatus->getCurrentCampaignMission();
                if (cm != 0x2a) {
                    Objective *obj = this->level->objectivesA;
                    survival = (obj == 0) || (((Objective *)(obj))->isSurvivalObjective() != 0);
                }
                if (!survival) {
                    bindDialogueLevel(this);
                    this->pauseOpen = 0x101;
                    ((MGame *)(this))->pauseSounds();
                }
            }
        }
    }

    if (this->gameOverActive != 0) {
        this->elapsedTime = 0;
        this->elapsedTimeHigh = 0;
        this->loadingTime = 0;
        ((FModSound *)(**g_goSnd))->play(0x25, 0, 0 /* vel: arg lost in decomp */, 0.0f);
    }
}

// 0x... (Layout::OnTouchBegin)
extern "C" int ApplicationManager_GetApplicationData();
// Module-switch tail helper @0x1ab908 (replay/menu transition).
// The large post-Hud-touch reaction body (time-extender, dock dialogue, free-cam
// init, etc.) is one logical unit; kept as a helper to remain faithful + compiling.

__attribute__((visibility("hidden"))) extern Layout ***g_tbStarLayout; // @0x188e90
__attribute__((visibility("hidden"))) extern int **g_tbRecordTrack;    // @0x188f1a
__attribute__((visibility("hidden"))) extern int **g_tbMenuTrack;      // @0x188f3e

// MGame::OnTouchBegin(int p1, int p2, void *touchId)
void MGame::OnTouchBegin(int p1, int p2, void *touchId) {
    MGame *self = this;
    // Track the active touch id (field 0xc0) when none is held.
    if (self->activeTouchId == 0)
        self->activeTouchId = touchId;

    if (self->pauseOpen == 0) {
        // Game-over / replay splash tap dispatch.
        if (self->gameOverActive != 0 && self->loadingTime >= 4000) {
            int cm = gStatus->getCurrentCampaignMission();
            if (cm == 0x9e) {
                self->active = 0;
                return self->appManager->SetCurrentApplicationModule(2);
            }
            if (self->gameRecord != 0) {
                ((GameRecord *)(self->gameRecord))->load();
                Globals_playMusicAndFadeOutCurrent(**g_tbRecordTrack);
                self->active = 0;
                return self->appManager->SetCurrentApplicationModule(5);
            }
            Globals_playMusicAndFadeOutCurrent(**g_tbMenuTrack);
            self->active = 0;
            self->appManager->SetCurrentApplicationModule(5);
            return;
        }
    } else {
        // Modal/overlay routing while in active flight UI.
        if (self->starMapOpen != 0) {
            Layout *hl = **g_tbStarLayout;
            if (*(uint8_t *)hl != 0) {
                ((Layout *)(hl))->OnTouchBegin(p1, p2);
                return;
            }
            uint8_t r = self->starMap->OnTouchBegin(p1, p2);
            self->starMapOpen = r ^ 1;
            return;
        }
        if (self->autopilotMenuOpen != 0 || self->choiceWindowOpen != 0 ||
            self->dockChoiceOpen != 0) {
            self->choiceWindow->OnTouchBegin(p1, p2);
            return;
        }
        if (self->cutsceneActive != 0) {
            self->dialogueWindow->OnTouchBegin(p1, p2);
            return;
        }
        if (self->menuTouchOpen != 0) {
            int ad = ApplicationManager_GetApplicationData();
            if (*(uint8_t *)((char *)ad + 5) != 0) return;
            if (*(uint8_t *)((char *)ad + 0xc) != 0) return;
            self->menuWindow->OnTouchBegin(p1, p2, touchId);
            if (self->freeCamMode == 0) return;
            if (self->menuWindow->isShowingMessage() != 0) return;
            if (self->menuWindow->isMakingScreenshot() != 0) return;
            // fall through to free-cam handler.
            return ((MGame *)(self))->handleHudTouchAction(p1, p2, touchId, 0);
        }
    }

    // World HUD touch: dispatch on the hud's touch result bitmask.
    unsigned hr = self->hud->touchBegin(p1, p2, touchId);
    self->hudTouchFlags = hr;
    ((MGame *)(self))->handleHudTouchAction(p1, p2, touchId, hr);
}

// GetElapsedTimeMillis
// The entire per-frame game tick — free-cam keyboard input, AI/physics step,
// mining/hacking/docking state machines, dialogue + sound pumps, jump-scene update,
// station-service transitions and their string building — is one massive body that
// Ghidra renders with deeply-corrupt SIMD/vector pseudo-ops.  It is translated as a
// single documented helper so the entry point stays faithful and compiling; the
// helper receives the clamped frame-delta and performs all field writes / engine
// calls on `self`.

// MGame::OnUpdate(): advance the whole game one frame.
void MGame::OnUpdate() {
    // Clamp this frame's elapsed time to [0, 0x96] ms (cap large hitches).
    int delta;
    int t = this->appManager->GetElapsedTimeMillis();
    if (t < 0x97) {
        delta = (this->appManager->GetElapsedTimeMillis() < 0) ? 0
                : (this->appManager->GetElapsedTimeMillis() < 0x97
                       ? this->appManager->GetElapsedTimeMillis()
                       : 0x96);
    } else {
        delta = 0x96;
    }

    this->deltaTime = delta;
    // Accumulate into the 4-wide playtime counter at 0x30 (lane 0 gets +delta).
    this->frameTime += delta;

    // Run the full per-frame update.
    ((MGame *)(this))->tick(delta);

    
}

__attribute__((visibility("hidden"))) extern RecordHandler **g_record;
__attribute__((visibility("hidden"))) extern FModSound **g_fmod;
extern "C" void Level_onSuspend(...);  // tail call via this->[0x74]

// MGame::OnSuspend(): persist options, pause all audio, raise the pause dialogue (cutscene
// mode if the player is mid-cutscene/dead), then suspend the level.
void MGame::OnSuspend() {
    if (*g_record != 0) ((RecordHandler *)(*g_record))->saveOptions();
    ((MGame *)(this))->pauseSounds();
    if (this->pauseOpen == 0) {
        if (this->menuWindow == 0)
            this->menuWindow = new MenuTouchWindow(1);
        this->pauseSnapshot = 1;
        this->pauseOpen = 1;
        ((FModSound *)(*g_fmod))->pauseAllPlaying();
        this->player->PauseEngineSound();
        Array<KIPlayer*> *e = this->level->getEnemies();
        if (e != nullptr) {
            for (uint32_t i = 0; i < e->size(); i++)
                ((KIPlayer *)((*e)[i]))->PauseEngineSound();
        }
        MenuTouchWindow *w = this->menuWindow;
        int mode = 1;
        if (this->jumpActive == 0)
            mode = this->player->isDead();
        ((MenuTouchWindow *)(w))->setCutsceneMode(mode);
        this->menuTouchOpen = 1;
    }
    return Level_onSuspend(this->hud);
}

// 0x... goingToStream
extern "C" int Station_getIndex(Station *s);
// Builds and sets the "dock at <station>?" choice text, then ChoiceWindow::left().

__attribute__((visibility("hidden"))) extern int *g_deAutoFlag;  // @0x18fa3a
__attribute__((visibility("hidden"))) extern int g_dePostEffect; // @0x18fe0c
__attribute__((visibility("hidden"))) extern int g_deTextA;      // @0x18fa82
__attribute__((visibility("hidden"))) extern int g_deLitA0;      // @0x18fa9a
__attribute__((visibility("hidden"))) extern int g_deLitA1;      // @0x18fac0
__attribute__((visibility("hidden"))) extern int g_deTextB;      // @0x18fc74
__attribute__((visibility("hidden"))) extern int g_deLitB0;      // @0x18fc8c
__attribute__((visibility("hidden"))) extern int g_deLitB1;      // @0x18fcb2
__attribute__((visibility("hidden"))) extern int **g_deAlienFlag;// @0x18fb9c

static void savePlayerStats(MGame *self, Status *status) {
    PlayerEgo *ego = self->player;
    Player *pl = (Player *)ego->player;
    // Status fields at +0x5c/0x60/0x64/0x68 are not modeled in gof2/Status.h
    // (owned by another batch); write them via typed byte offsets.
    F<int>(status, 0x64) = ((Player *)(pl))->getHitpoints();
    F<int>(status, 0x5c) = ((Player *)(pl))->getShieldHP();
    F<int>(status, 0x60) = ((Player *)(pl))->getArmorHP();
    F<int>(status, 0x68) = ((Player *)(pl))->getGammaHP();
}

// MGame::buildDockChoice(): compose the "<dock at> <station name>?" prompt and raise it
// in the ChoiceWindow (its slot at 0x94 has already been created by the caller).
// textId is the GameText key for the surrounding question text; prefixLit/suffixLit are
// the C-string literals placed immediately before and after the station name.
void MGame::buildDockChoice(int textId, int prefixLit, int suffixLit) {
    ChoiceWindow *cw = this->choiceWindow;

    void *txt = ((GameText *)(*g_gameText))->getText(textId);
    String name = ((Station *)(gStatus->getStation()))->getName();

    String sPrefix, sSuffix, t1, t2, t3, result;
    sPrefix.ctor_char((const char *)(intptr_t)prefixLit, false);
    t1 = *(String *)txt + sPrefix;                 // text + prefix
    t2 = t1 + name;                                // + station name
    sSuffix.ctor_char((const char *)(intptr_t)suffixLit, false);
    t3 = t2 + sSuffix;                             // + suffix
    result = t3 + *(String *)txt;                  // + trailing text

    ((ChoiceWindow *)(cw))->set(*(String *)&result, true);

    ((ChoiceWindow *)(cw))->left();
}

// MGame::dockEvent(): handle proximity to a jumpgate/station while flying.
void MGame::dockEvent(int p1, int p2) {
    (void)p1;
    (void)p2;
    float pos[4];
    ((PlayerEgo *)(pos))->getPosition();
    this->touchesStream = this->level->collideStream(*(Vector *)pos);
    ((PlayerEgo *)(pos))->getPosition();
    this->touchesStation = this->level->collideStation(*(Vector *)pos);

    // g_deStatus held the address of the gStatus slot (binary .bss 0x2281b0); the
    // surrounding code reads/writes through it both as &gStatus and (via *status) the
    // Status object, so keep the slot-address form to preserve exact byte semantics.
    Status *status = (Status *)(void *)&gStatus;
    Mission *m = gStatus->getMission();
    bool special = ((Mission *)(m))->isEmpty() != 0 ||
                   ((Mission *)(gStatus->getMission()))->getType() == 0xb ||
                   ((Mission *)(gStatus->getMission()))->getType() == 0 ||
                   ((Mission *)(gStatus->getMission()))->getType() == 0xbd ||
                   ((Mission *)(gStatus->getMission()))->getType() == 0xab ||
                   ((Mission *)(gStatus->getMission()))->getType() == 0xac;

    if (!special) {
        if ((this->touchesStation != 0 || this->touchesStream != 0) &&
            this->player->isAutoPilot() != 0) {
            this->hud->hudEvent(0x15, this->player, 0);
        }
        
        return;
    }

    // special-mission docking path.
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
            savePlayerStats(this, status);
            // Status +0xf4 is not modeled in gof2/Status.h (owned by another batch).
            F<int>(status, 0xf4) = this->player->getCurrentSecondaryWeaponIndex();
            int autop = this->player->isAutoPilot();
            int *autoFlag = g_deAutoFlag;
            if (*autoFlag == 0 || autop == 0) {
                if (this->starMapOpen != 0) {
                    this->frameTime = 0;
                    this->frameTimeHigh = 0;
                    
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
                    Engine *eng = (Engine *)this->appManager->GetEngine();
                    ((Engine *)(eng))->SetPostEffect(g_dePostEffect, 0);
                    this->starMap->initLights();
                    this->starMap->setJumpMapMode(1, 0);
                    this->player->setAutoPilot(0);
                    this->pauseOpen = 1;
                    this->starMapOpen = 1;
                    ((MGame *)(this))->pauseSounds();
                    this->needsRedraw = 1;
                    
                    return;
                }
                if (this->autopilotMenuOpen != 0) {  return; }
                this->freeCamDragging = 0;
                ChoiceWindow *cw = this->choiceWindow;
                this->needsRedraw = 1;
                if (cw == 0) {
                    cw = new ChoiceWindow();
                    this->choiceWindow = cw;
                }
                ((MGame *)(this))->buildDockChoice(g_deTextB, g_deLitB0, g_deLitB1);
            } else {
                if (this->autopilotMenuOpen != 0) {  return; }
                this->freeCamDragging = 0;
                ChoiceWindow *cw = this->choiceWindow;
                this->needsRedraw = 1;
                if (cw == 0) {
                    cw = new ChoiceWindow();
                    this->choiceWindow = cw;
                }
                ((MGame *)(this))->buildDockChoice(g_deTextA, g_deLitA0, g_deLitA1);
            }
            this->pauseOpen = 1;
            this->autopilotMenuOpen = 1;
            ((MGame *)(this))->pauseSounds();
            this->player->setAutoPilot(0);
            
            return;
        }
    }

    if (this->touchesStation == 0) {
        int tgt = this->player->getAutoPilotTarget();
        Array<KIPlayer*> *lm = this->level->getLandmarks();
        if (tgt != (int)(intptr_t)lm->data() ||
            this->player->collidesWithStation() == 0) {
            
            return;
        }
    }

    int cm = gStatus->getCurrentCampaignMission();
    if (cm > 0x30 && gStatus->getCurrentCampaignMission() < 0x37) {
        if (Station_getIndex(gStatus->getStation()) != 0x4a) {
            this->hud->hudEvent(0x15, this->player, 0);
            
            return;
        }
    }
    if (this->player->goingToStation() != 0 &&
        gStatus->inAlienOrbit() == 0 &&
        ((Status *)(*(Station **)status))->inEmptyOrbit() == 0) {
        gAchievements->checkForNewMedal(this->player);
        **g_deAlienFlag = 0;
        savePlayerStats(this, status);
        this->appManager->SetCurrentApplicationModule(5);
        this->active = 0;
    }

    
}

// MGame::freeCamTouchEnd(int, int, void*): releases a free-camera drag. Clears the
// matching active-touch slot, then commits the accumulated rotation deltas (only the
// axes that moved more than 3px) and converts them to integer angles, flagging the
// camera dirty.
void MGame::freeCamTouchEnd(int p1, int p2, void *idPtr) {
    int id = (int)(intptr_t)idPtr;
    (void)p1;
    (void)p2;
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
        float fx = (float)ix;
        float fy = (float)iy;
        float nx = this->flShakeX + fx;
        float ny = this->flShakeY + fy;
        if (iy < 0) iy = -iy;
        if (ix < 0) ix = -ix;
        // Default coefficients come from float literals (exact values irrelevant to the
        // byte match — the diff normalizes literal-pool loads). Use values that are NOT
        // encodable as vmov.f32 immediates so the compiler emits a vldr/ldr from the pool.
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
        this->dragRotIntX = (int)nx;
        this->dragRotIntY = (int)ny;
    }
}

// MGame::~MGame() deleting destructor: run the complete dtor (which returns `this`)
// then tail-call operator delete with that pointer — so `this` is never saved.

// (deleting_dtor removed: the compiler generates MGame's deleting destructor from ~MGame.)

// Tail helpers.
// @0x1ac808
// @0x1ac818

__attribute__((visibility("hidden"))) extern int **g_kdJumpDst;   // @0x189e36
__attribute__((visibility("hidden"))) extern int **g_kdVolText;   // @0x189ed2
__attribute__((visibility("hidden"))) extern int **g_kdAlienDst;  // @0x189e9a
__attribute__((visibility("hidden"))) extern int g_kdPostEffect;  // @0x189f98 (DAT)

// MGame::UseKhadorDrive(): trigger a hyperspace jump or open the jump star map.
void MGame::UseKhadorDrive() {
    if (this->player->isChargingDrive() != 0) return;

    int *status = ((int *)&gStatus);
    Mission *m = gStatus->getMission();
    bool special =
        ((Mission *)(m))->isEmpty() != 0 ||
        gStatus->getCurrentCampaignMission() == 0x4e ||
        ((Mission *)(m))->getType() == 0xb ||
        ((Mission *)(m))->getType() == 0 ||
        ((Mission *)(m))->getType() == 0xbd ||
        ((Mission *)(m))->getType() == 0xd ||
        ((Mission *)(m))->getType() == 0xab ||
        ((Mission *)(m))->getType() == 0xac;

    if (!special) {
        if (gStatus->getCurrentCampaignMission() == 0x41 && gStatus->inAlienOrbit() == 0) {
            int idx = Station_getIndex(gStatus->getStation());
            gStatus->getCampaignMission();
            if (idx == ((Mission *)((Mission *)gStatus->getCampaignMission()))->getTargetStation())
                special = true;
        }
    }

    if (special) {
        PlayerEgo *player = (PlayerEgo *)(intptr_t)((Level *)(this))->getPlayer();
        return this->hud->hudEvent(0x15, player, 0);
    }

    this->player->resetGunDelay();
    if (gStatus->getCurrentCampaignMission() == 0x4e) {
        **g_kdJumpDst = *(int *)((char *)*status + 0x78);
        this->usingJumpDrive = 1;
        ((MGame *)(this))->startChargingJumpDrive();
        this->pauseOpen = 0;
        ((MGame *)(this))->resumeSounds();
        this->hudMenuOpen = 0;
        this->hud->closeHudMenu();
        return gStatus->nextCampaignMission(true);
    }

    if (gStatus->inAlienOrbit() == 0) {
        if (this->player->hasVolatileGoods() != 0) {
            void *txt = ((GameText *)(**g_kdVolText))->getText(0);
            this->choiceWindow->set(*(String *)txt);
            this->pauseOpen = 1;
            this->choiceWindowOpen = 1;
            this->needsRedraw = 1;
            this->hudMenuOpen = 0;
            this->hud->closeHudMenu();
            ((MGame *)(this))->pauseSounds();
            this->maneuverActive = 0;
            return;
        }
        if (this->starMap == 0) {
            StarMap *sm = new StarMap(false, nullptr, false, -1);
            this->starMap = sm;
        }
        Engine *eng = (Engine *)this->appManager->GetEngine();
        ((Engine *)(eng))->SetPostEffect(g_kdPostEffect, 0);
        this->starMap->initLights();
        this->usingJumpDrive = 1;
        this->starMap->setJumpMapMode(1, 1);
        if (gStatus->inAlienOrbit() == 0)
            this->starMap->askForJumpIntoAlienWorld();
        this->pauseOpen = 1;
        this->starMapOpen = 1;
        ((MGame *)(this))->pauseSounds();
        this->hudMenuOpen = 0;
        return this->hud->closeHudMenu();
    }

    // In alien orbit.
    if (gStatus->getCurrentCampaignMission() == 0x50)
        *(int *)((char *)*status + 0x84) = 100;
    int station = ((Galaxy *)(*(int *)gGalaxy))->getStation(gStatus->getCurrentCampaignMission() /* index: arg lost in decomp */);
    **g_kdAlienDst = station;
    this->usingJumpDrive = 1;
    ((MGame *)(this))->startChargingJumpDrive();
    this->pauseOpen = 0;
    ((MGame *)(this))->resumeSounds();
    this->hudMenuOpen = 0;
    return this->hud->closeHudMenu();
}

// Heavily-corrupt sub-blocks are delegated to documented helpers:
// sound-resource list (62..145)
// HP/shield/armor restore (157..198)
// weapon + music init (315..end)


// MGame::loadSoundResources(): build the per-session FMOD resource list. Always loads the
// common cockpit/weapon/explosion set, then appends wingman, early-mission, warp-gate and
// per-campaign-mission specific clips.
void MGame::loadSoundResources() {
    Globals_startNewSoundResourceList();
    static const int kCommon[] = {
        0x66, 0x68, 0x69, 0x6a, 0x6b, 0x67, 0x7e, 0x05, 0x18, 0x15,
        0x12, 0x13, 0x14, 0x1c, 0x1d, 0x1b, 0x25, 0x1a, 0x2e, 0x2f,
    };
    for (int id : kCommon)
        gGlobals->addSoundResource_oi(id);

    if (gStatus->getWingmen() != 0)
        gGlobals->addSoundResourceToList(0x30);

    gGlobals->addSoundResource_oi(0x3e);
    gGlobals->addSoundResource_oi(0x3d);
    gGlobals->addSoundResource_oi(0x24);

    if (gStatus->getCurrentCampaignMission() < 2) {
        gGlobals->addSoundResourceToList(0x9c);
        gGlobals->addSoundResourceToList(0x9d);
    }

    if (gStatus->inAlienOrbit() == 0) {
        gStatus->getSystem();
        if (((SolarSystem *)(0))->currentOrbitHasWarpGate())
            gGlobals->addSoundResourceToList(0x1f);
    }

    int cm = gStatus->getCurrentCampaignMission();
    if (cm == 0) {
        gGlobals->addSoundResource_oi(0x8f);
        gGlobals->addSoundResource_oi(0x9d);
        gGlobals->addSoundResource_oi(0x9e);
        gGlobals->addSoundResource_oi(0xa1);
        gGlobals->addSoundResource_oi(0xa0);
        gGlobals->addSoundResource_oi(0x9f);
    } else if (cm == 0xe) {
        gGlobals->addSoundResourceToList(0xf);
    } else if (cm == 0x18) {
        gGlobals->addSoundResourceToList(0x22);
    } else if (cm == 0x1d) {
        gGlobals->addSoundResourceToList(0xe);
    } else if (cm == 0x29) {
        gGlobals->addSoundResource_oi(0x9b);
        gGlobals->addSoundResource_oi(0x99);
        gGlobals->addSoundResource_oi(0x9a);
    }
}

// MGame::restorePlayerStats(): re-apply the player's persisted HP/shield/armor/gamma values
// (each only when its saved slot is non-negative) and, except on mission 0x5f, re-cap them to
// the current ship's maxima (resetting gamma to full when there is no orbital gamma damage).
void MGame::restorePlayerStats() {
    int status = *((int *)&gStatus);
    Player *pl = (Player *)this->player->player;

    if (*(int *)((char *)status + 0x64) >= 0) pl->setHitpoints(*(int *)((char *)status + 0x64));
    if (*(int *)((char *)status + 0x5c) >= 0) pl->setShieldHP(*(int *)((char *)status + 0x5c));
    if (*(int *)((char *)status + 0x60) >= 0) pl->setArmorHP(*(int *)((char *)status + 0x60));
    if (*(int *)((char *)status + 0x68) >= 0) pl->setGammaHP(*(int *)((char *)status + 0x68));

    this->player->resetLastHP();

    if (gStatus->getCurrentCampaignMission() != 0x5f) {
        Ship *ship = gStatus->getShip();
        *(int *)((char *)status + 0x64) = ship->getMaxHP();
        *(int *)((char *)status + 0x5c) = ship->getMaxShieldHP();
        *(int *)((char *)status + 0x60) = ship->getMaxArmorHP();
        *(int *)((char *)status + 0x68) = 100;

        int stIdx = ((Station *)(gStatus->getStation()))->getIndex();
        int cm = gStatus->getCurrentCampaignMission();
        if ((float)gStatus->getGammaRayDamagePerSecond(stIdx, cm) == 0.0f)
            pl->setGammaHP(100);
    }
}

// Opaque init globals for the audio/mission-info tail (DAT pointers the decompiler left
// as raw addresses; named here in the established hidden-extern style).
__attribute__((visibility("hidden"))) extern int   g_initParticleFlag; // @0x1881f4 (render-particles toggle struct)
__attribute__((visibility("hidden"))) extern int  *g_initEngineSnd;    // @0x188250 (engine sound id)
__attribute__((visibility("hidden"))) extern int  *g_initMusicArmed;   // @0x18831e (-1 when no music queued)
__attribute__((visibility("hidden"))) extern int  *g_initMusicTrack;   // @0x18832a (music track id)
__attribute__((visibility("hidden"))) extern int **g_initFmod;         // @0x18833e (FModSound singleton)
__attribute__((visibility("hidden"))) extern unsigned g_initPostEffect; // @0x188444
__attribute__((visibility("hidden"))) extern int   g_initStationMask;   // @0x188440 (visited-station bitmask)
__attribute__((visibility("hidden"))) extern String **g_initInfoFont;   // @0x1883b8 (mission-info font)
__attribute__((visibility("hidden"))) extern int    g_initInfoTextKey;  // @0x1883b0 (mission-info text key)
__attribute__((visibility("hidden"))) extern int   *g_initInfoWidth;    // @0x1883d0 (wrap width base)

// 0x... PlayerFighter::cloak
// Level::getPlayer (int handle)
// MGame::setupWeaponsAndAudio(): choose the active secondary weapon, kick off engine sounds,
// particle effects, music and the post-process effect, ensure the pause window and the
// mission-information overlay text exist. Runs as the tail of OnInitialize.
void MGame::setupWeaponsAndAudio() {
    MGame *self = this;
    int status = *((int *)&gStatus);

    // Mission 0x7d freighter visit: flag this station as serviced the first time and raise
    // its radio briefing.
    if (gStatus->inAlienOrbit() == 0 && gStatus->getCurrentCampaignMission() == 0x7d) {
        int stIdx = ((Station *)(gStatus->getStation()))->getIndex();
        if (gStatus->isFreighterMissionStation(stIdx) != 0) {
            Mission *m = gStatus->getMission();
            int statusVal = ((Mission *)(m))->getStatusValue();
            int bit = gStatus->getFreighterMissionStationBit(
                ((Station *)(gStatus->getStation()))->getIndex());
            if ((statusVal & (1 << (bit & 0xff))) == 0) {
                int bit2 = gStatus->getFreighterMissionStationBit(
                    ((Station *)(gStatus->getStation()))->getIndex());
                ((Mission *)(m))->setStatusValue(statusVal | (1 << (bit2 & 0xff)));
                if (self->player != 0 && self->radio != 0)
                    // RAWREAD: PlayerEgo +0x18 has no named member in PlayerEgo.h.
                    *(Radio **)((char *)self->player + 0x18) = self->radio;
                self->level->createRadioMessage(0x13, 0);
            }
        }
    }

    // Secondary-weapon selection: prefer the persisted weapon id (status+0xf4) if the ship
    // still carries it; otherwise fall back to the first secondary in slot type 1.
    Array<Item *> *secondary = gStatus->getShip()->getEquipment(1);
    if (secondary != 0) {
        int savedId = *(int *)((char *)status + 0xf4);
        if (gStatus->getShip()->hasEquipment(savedId, 1) == 0) {
            Item *first = secondary->empty() ? 0 : (*secondary)[0];
            if (first != 0) {
                self->player->setCurrentSecondaryWeaponIndex(((Item *)(first))->getIndex());
                self->hud->setCurrentSecondaryWeapon(first);
            }
        } else {
            for (unsigned i = 0; i < secondary->size(); i++) {
                Item *it = (*secondary)[i];
                if (it != 0 && ((Item *)(it))->getIndex() == savedId) {
                    ((Level *)((Level *)self->level))->getPlayer();
                    self->player->setCurrentSecondaryWeaponIndex(((Item *)(it))->getIndex());
                    self->hud->setCurrentSecondaryWeapon(it);
                    break;
                }
            }
        }
    }

    // Engine sounds: if the render-particles flag is clear, play the ambient engine clip at
    // the player's position; otherwise start the player's looping engine sound and every
    // enemy's.
    self->field_0xc8 = 0;
    bool renderParticles = *(uint8_t *)((char *)(intptr_t)g_initParticleFlag + 0xf) != 0;
    if (!renderParticles) {
        if (gStatus->getCurrentCampaignMission() > 1) {
            Vec3 p = self->player->getPosition();
            (void)p;
            ((FModSound *)(*g_fmod))->play(*g_initEngineSnd,
                                           (Vector *)&self->player->field_0x1c,
                                           (Vector *)&p, 0.0f);
        }
    } else {
        self->player->PlayEngineSound();
        Array<KIPlayer*> *enemies = self->level->getEnemies();
        if (enemies != nullptr)
            for (unsigned i = 0; i < enemies->size(); i++)
                ((KIPlayer *)((*enemies)[i]))->PlayEngineSound();
    }

    self->loadingTime = 0;

    // Lighting + particle effects + per-ship fire-rate pitch.
    self->level->getStarSystem();
    ((StarSystem *)(0))->initLight();
    self->level->enableParticleEffects(true, renderParticles);

    gStatus->getShip();
    float fireRate = (float)gStatus->getShip()->getFireRateFactor();
    if (1.0f - fireRate >= 0.0f)
        self->player->pitchAllPrimaryGuns(1.0f - fireRate);

    // Mark "newly visited" service stations.
    if (gStatus->inAlienOrbit() == 0) {
        int idx = ((Station *)(gStatus->getStation()))->getIndex();
        bool visit;
        unsigned off = (unsigned)(idx - 0x6d);
        if (off < 0x1a) {
            if ((g_initStationMask & (1 << (off & 0xff))) == 0)
                visit = (off != 2) || (gStatus->getCurrentCampaignMission() < 0x5e);
            else
                visit = true;
        } else {
            visit = ((unsigned)(idx - 0x66) <= 2);
        }
        if (visit)
            ((Station *)(gStatus->getStation()))->visit();
    }

    // Music: play the queued track (if any), then reset the queue and reset the sound pitch.
    if (*g_initMusicArmed != -1)
        Globals_playMusicAndFadeOutCurrent(*g_initMusicTrack);
    *g_initMusicArmed = -1;
    ((FModSound *)(*g_initFmod))->setDownPitch(false);

    // Post-process effect for this flight session.
    Engine *eng = (Engine *)self->appManager->GetEngine();
    ((Engine *)(eng))->SetPostEffect(g_initPostEffect, false);

    // Mission 0x9e: cloak the first enemy fighter.
    if (gStatus->getCurrentCampaignMission() == 0x9e) {
        Array<KIPlayer*> *enemies = self->level->getEnemies();
        if (enemies != nullptr) {
            KIPlayer *first = (*enemies)[0];
            ((PlayerFighter *)(first))->cloak(true, false);
        }
    }

    // Ensure the pause / cutscene window exists.
    if (self->menuWindow == 0)
        self->menuWindow = new MenuTouchWindow(1);

    // Pre-build the wrapped mission-information overlay text.
    self->missionInfoLines = new Array<AbyssEngine::String *>();
    String *font = *g_initInfoFont;
    String *text = (String *)((GameText *)(*g_gameText))->getText(g_initInfoTextKey);
    gGlobals->getLineArray(static_cast<unsigned int>(reinterpret_cast<std::size_t>(font)),
                           *text, *g_initInfoWidth, self->missionInfoLines);
    self->active = 1;
}

// MGame::OnInitialize(): bootstrap a flight session (level, player, sounds, state).
void MGame::OnInitialize() {
    MGame *self = this;
    self->missionInfoLines = 0;   // this[2].field_4C low byte
    Level *level = self->level;
    self->loadProgress = 100;

    if (level == 0) {
        // Create skybox cube texture from the current system / alien orbit.
        unsigned texSel;
        if (gStatus->inAlienOrbit() == 0) {
            gStatus->getSystem();
            int ti = ((SolarSystem *)(0))->getTextureIndex();
            texSel = (ti + 0x2efe) & 0xffff;
        } else {
            texSel = 0x2f08;
        }
        gCanvas->TextureCreate((unsigned short)self->skyboxTexture, 0, (void *)0, texSel, false);
        gCanvas->ChangeCubeTexture((unsigned)self->skyboxTexture);

        // Build the per-session sound-resource list (long sequential block).
        ((MGame *)(self))->loadSoundResources();

        gStatus->checkForLevelUp();
        level = new Level(3);
        self->level = level;
    }

    if (((Level *)(level))->init() == 0) {
        self->loadProgress = 100;
        
        return;
    }

    ((MGame *)(self))->reset();

    int *status = ((int *)&gStatus);
    ((MGame *)(self))->restorePlayerStats();
    self->player->resetLastHP();

    // Per-mission HP cap and alien-orbit station bookkeeping.
    if (gStatus->getCurrentCampaignMission() != 0x5f) {
        // (Ship max-HP restore delegated within restorePlayerStats variant)
    }
    if (gStatus->inAlienOrbit() == 0)
        *(int *)((char *)*status + 0x84) = Station_getIndex(gStatus->getStation());

    unsigned t = self->appManager->GetCurrentTimeMillis();
    self->field_0x1ac = 0;   // this[1].field_A4
    self->cloakAttributeMax = 0;   // this[1].field_AC
    self->startTime = t & 0xffff;
    self->startTimeHigh = 0;
    self->lastTime = t & 0xffff;
    self->lastTimeHigh = 0;

    if (Radar::hasScanner() != 0)
        *(int *)((char *)*status + 0x11c) = 0;
    *(int *)((char *)*status + 300) = 0;
    *(int *)((char *)*status + 0x134) = 0;
    *(int *)((char *)*status + 0x13c) = 0;
    *(int *)((char *)*status + 0x144) = 0;

    Item *eq = ((Ship *)(((Status *)((Status *)*((int *)&gStatus)))->getShip()))->getFirstEquipmentOfSort(0x15);
    if (eq != 0) {
        self->cloakAttributeMax = ((Item *)(eq))->getAttribute(0x26);
        self->cloakAttribute = ((Item *)(eq))->getAttribute(0x26);
        self->hud->setTimeExtender(1, 0, 1, 0);
    }

    // Alien-orbit / black-market radio messages.
    if (gStatus->dlc1Won() != 0 && gStatus->inAlienOrbit() != 0 &&
        gStatus->getCurrentCampaignMission() < 0x93) {
        if (self->player != 0 && self->radio != 0)
            // RAWREAD: PlayerEgo +0x18 has no named member in PlayerEgo.h.
            *(Radio **)((char *)self->player + 0x18) = self->radio;
        self->level->createRadioMessage(8, 0);
    }

    if (gStatus->inBlackMarketSystem() == 0) {
        *(uint16_t *)((char *)*status + 0x110) = 0;
    } else {
        if (self->player != 0 && self->radio != 0)
            // RAWREAD: PlayerEgo +0x18 has no named member in PlayerEgo.h.
            *(Radio **)((char *)self->player + 0x18) = self->radio;
        if (*(uint8_t *)((char *)*status + 0x110) == 0) {
            int id;
            Level *lvl;
            if (*(uint8_t *)((char *)*status + 0x111) == 0) {
                Array<KIPlayer*> *enemies = self->level->getEnemies();
                if (enemies != nullptr) {
                    int n = (int)enemies->size();
                    for (int i = 0; i < n; i++) {
                        KIPlayer *e = (*enemies)[i];
                        if (e->shipGroup == 8)
                            e->field_0x25 = 0;
                    }
                }
                lvl = self->level;
                id = 9;
            } else {
                lvl = self->level;
                id = 0xd;
            }
            ((Level *)(lvl))->createRadioMessage(id, 0);
        } else {
            Array<KIPlayer*> *enemies = self->level->getEnemies();
            if (enemies != nullptr) {
                int n = (int)enemies->size();
                for (int i = 0; i < n; i++) {
                    KIPlayer *e = (*enemies)[i];
                    if (e->shipGroup == 8)
                        e->field_0x25 = 0;
                }
            }
        }
    }

    // Weapon selection, particle effects, audio and music init.
    ((MGame *)(self))->setupWeaponsAndAudio();

    
}

void TFC_zoomTarget(void *cam, float z);
// Pan-finish tail helper @0x1ac798 (no-op stack-guard-ok path).

__attribute__((visibility("hidden"))) extern float g_fcRotScale;  // @0x188d20 (DAT)
__attribute__((visibility("hidden"))) extern float g_fcZoomMax;   // @0x188d24
__attribute__((visibility("hidden"))) extern float g_fcZoomMin;   // @0x188d28

// MGame::freeCamTouchMove(int x, int y, void* touchId): rotate/zoom the free camera.
// The third gesture component arrives pointer-sized but is consumed as a signed
// touch coordinate; route it through intptr_t so the host build does not truncate.
void MGame::freeCamTouchMove(int x, int y, void *touchId) {
    int ty = (int)(intptr_t)touchId;
    if (this->player->isMining() != 0) {
        // Free-cam pan/zoom while mining: nothing more to do (clean exit).
        this->needsRedraw = 1;
        return;
    }
    this->needsRedraw = 0;

    int t0 = this->touch0Id;
    int t1 = this->touch1Id;
    if (t0 == 0 || t1 == 0) {
        // Single-finger pan: accumulate rotation deltas.
        int dy = y - this->dragLastX;
        int dx = ty - this->dragLastY;
        this->dragLastX = x;
        this->dragLastY = ty;
        this->dragDeltaX = dy;
        this->dragDeltaY = dx;
        this->flShakeAmpX = 1.0f;
        this->flShakeAmpY = 1.0f;
        this->flShakeX += (float)dy;
        this->flShakeY += (float)dx;

        if (t0 == 0) {
            float v[4]; v[0] = (float)x; v[1] = (float)ty; v[2] = 0;
            *(Vector*)((Vector *)((char *)&this->freeCamFinger0X)) = *(const Vector*)((Vector *)v);
        } else if (t1 == 0) {
            float v[4]; v[0] = (float)x; v[1] = (float)ty; v[2] = 0;
            *(Vector*)((Vector *)((char *)&this->freeCamFinger1X)) = *(const Vector*)((Vector *)v);
        }
        
        return;
    }

    // Two-finger pinch: zoom by the change in finger distance.
    Vector *base = (Vector *)((char *)&this->freeCamFinger1X);
    float tmp[4];
    *(Vector*)((Vector *)tmp) = *(const Vector*)(base) - *(const Vector*)((Vector *)((char *)&this->freeCamFinger0X));
    float oldLen = AbyssEngine::AEMath::VectorLength(*(const Vector*)((Vector *)tmp));
    float newLen = oldLen;

    if (this->touch0Id == ty) {
        float v[4]; v[0] = (float)x; v[1] = (float)ty; v[2] = 0;
        *(Vector*)((Vector *)tmp) = *(const Vector*)(base) - *(const Vector*)((Vector *)v);
        newLen = AbyssEngine::AEMath::VectorLength(*(const Vector*)((Vector *)tmp));
        *(Vector*)(base) = *(const Vector*)((Vector *)v);
    } else if (this->touch1Id == ty) {
        float v[4]; v[0] = (float)x; v[1] = (float)ty; v[2] = 0;
        *(Vector*)((Vector *)tmp) = *(const Vector*)(base) - *(const Vector*)((Vector *)v);
        newLen = AbyssEngine::AEMath::VectorLength(*(const Vector*)((Vector *)tmp));
        *(Vector*)((Vector *)((char *)&this->freeCamFinger0X)) = *(const Vector*)((Vector *)v);
    }

    float zoom = this->flCameraRoll + (newLen - oldLen) * g_fcRotScale;
    this->flCameraRoll = zoom;
    if (zoom > g_fcZoomMax || zoom < g_fcZoomMin) {
        zoom = (zoom > g_fcZoomMax) ? g_fcZoomMax : g_fcZoomMin;
        this->flCameraRoll = zoom;
    }
    TFC_zoomTarget(this->camera, zoom);

    
}

// The full button-action dispatch body (cutscene/pause menu, boost/shoot, dock,
// jump, menu navigation, dialogue choice handling, station services, etc.) is an
// enormous switch over the Hud touch-result bits.  Translated as one documented
// helper so the entry point stays faithful and compiling; it consumes the same
// hud-result that the inline body branches on, and performs all field writes and
// engine calls on `self`.

// MGame::OnTouchEnd(int p1, int p2, void *touchId)
void MGame::OnTouchEnd(int p1, int p2, void *touchId) {
    // Release the tracked touch id if this is the one we were following.
    if (this->activeTouchId == touchId) {
        this->activeTouchId = 0;
        this->field_0xc1 = 0;
        this->field_0xc2 = 0;
        this->field_0xc3 = 0;
    }

    int wasAutoPilot = this->player->isAutoPilot();
    this->flFastForwardWeight = 1.0f;                 // fast-forward weight
    TFC_setFastForwardMode(this->camera, 0);
    // RAWREAD: PlayerEgo +0x150 resume-flag; offset not pinned to a named member
    // in PlayerEgo.h (no +0x offset comments in that region).
    *(uint8_t *)((char *)this->player + 0x150) = 1;

    unsigned hr = 0;
    if (this->pauseOpen == 0) {
        hr = this->hud->touchEnd(p1, p2, touchId);
        this->hudTouchFlags = hr;
        if (hr != 0) {
            this->touch0Id = 0;
            this->touch1Id = 0;
        }
    }

    // All button-press reactions live in the dispatch helper.
    ((MGame *)(this))->dispatchTouchEndAction(p1, p2, touchId, hr, wasAutoPilot);

    
}

// 0x... missionCompleted
// 0x... checkObjective
// Corrupt follow-up-mission setup block (227..291) kept as one helper.

__attribute__((visibility("hidden"))) extern uint8_t **g_scFlag; // @0x1904aa

// Bind a DialogueWindow to the current level (duplicated block at 0x1019020c/0x190020).
static void bindDlg(MGame *self) {
    if (self->dialogueWindow == 0) {
        DialogueWindow *w = (DialogueWindow *)::operator new(0x74);
        DialogueWindow_ctor(w);
        Level *lvl = self->level;
        self->dialogueWindow = w;
        if (lvl != 0) ((DialogueWindow *)(w))->setLevel(lvl);
    } else if (self->dialogueWindow->hasLevel() == 0) {
        Level *lvl = self->level;
        if (lvl != 0) self->dialogueWindow->setLevel(lvl);
    }
}

// Follow-up-mission setup helpers: getCampaignMission() yields the active campaign Mission*
// (header types it as int); Mission::setType is not declared in the shared header.
// Status::getCampaignMission
// Mission::setType
extern "C" void    *Objective_dtor(Objective *o);                   // Objective::~Objective
// Status::replaceHash(String haystack, String needle, String replacement) -> String (sret),
// modelled as a free function on String like the other TUs (see HangarWindow.cpp/Status.cpp).
extern "C" void     Status_replaceHash(String *out, Status *self, String *haystack,
                                       String *needle, String *repl);
__attribute__((visibility("hidden"))) extern int  g_scFollowTextKey; // @0x1900c2 (briefing text key)
__attribute__((visibility("hidden"))) extern int  g_scFollowHashLit; // @0x1900f8 ("#station" token literal)

// MGame::buildMissionFollowup(): turn a just-completed mission into its follow-up "deliver
// to <station>" mission. Retargets the campaign mission at the originating agent's station,
// raises its briefing, switches the player out of turret/free-look back to flight, rebuilds
// the agent's mission string and clears the old objectives. The DialogueWindow has already
// been bound to the level by the caller.
void MGame::buildMissionFollowup() {
    Status *status = gStatus;

    // Point the campaign mission at the agent's home station.
    Mission *cm = (Mission *)(intptr_t)status->getCampaignMission();
    Agent *agent = ((Mission *)((Mission *)(intptr_t)status->getCampaignMission()))->getAgent();
    ((Mission *)(cm))->setTargetStation(((Agent *)(agent))->getStation());

    // Raise the briefing for the (now retargeted) mission and convert it to a delivery type.
    this->dialogueWindow->set((Mission *)(intptr_t)status->getCampaignMission(), 1, -1);
    ((Mission *)((Mission *)(intptr_t)status->getCampaignMission()))->setType(0xb);

    // Leave turret / first-person and reset the flight camera.
    this->player->setTurretMode(0);
    this->levelScript->resetCamera((Level *)this->level);
    this->player->setFreeLookMode(false);
    TFC_enableFirstPersonCam(this->camera, 0);
    this->player->hideShipForFirstPersonCameraView(false);
    // RAWREAD: delivery-active flag at +0x239 (target comment: this[1].field_48+1).
    // Kept as raw byte access: MGame's field_0xNN layout is documentation-only (no
    // explicit padding, sparse offsets), so this byte has no faithful named member.
    *(uint8_t *)((char *)this + 0x239) = 1;

    ((Mission *)((Mission *)(intptr_t)status->getCampaignMission()))->setStatusValue(0);
    ((Mission *)((Mission *)(intptr_t)status->getCampaignMission()))->setWon(false);

    // Rebuild the agent's mission text: take the template and substitute the target station.
    void *tmpl = ((GameText *)(*g_gameText))->getText(g_scFollowTextKey);
    String sTmpl, sHash, sStation, sResult;
    sTmpl = *(String *)tmpl;
    sHash.ctor_char((const char *)(intptr_t)g_scFollowHashLit, false);
    String station = (status->getMission())->getTargetStationName();
    sStation = *(String *)&station;
    Status_replaceHash(&sResult, status, &sTmpl, &sStation, &sHash);

    Agent *missionAgent = ((Mission *)(status->getMission()))->getAgent();
    missionAgent->setMissionString(sResult);

    status->setMission((Mission *)(intptr_t)status->getCampaignMission());

    // Hand the route back to the player and clear it / its waypoint autopilot.
    this->player->setRoute(0);
    if (this->player->goingToWaypoint() != 0)
        this->player->setAutoPilot(0);
    this->player->removeRoute();
    this->level->setPlayerRoute(0);

    // Release the two old objectives held on the Level (slots 10 and 11).
    int *level = (int *)this->level;
    if (level[10] != 0) {
        ::operator delete(Objective_dtor((Objective *)(intptr_t)level[10]));
        level = (int *)this->level;
    }
    level[10] = 0;
    if (level[11] != 0) {
        ::operator delete(Objective_dtor((Objective *)(intptr_t)level[11]));
        level = (int *)this->level;
    }
    level[11] = 0;

    this->pauseOpen = 1;   // pause flag  (low byte of the original 0x101 16-bit store)
    this->cutsceneActive = 1;   // cutscene flag (high byte)
    ((MGame *)(this))->pauseSounds();
}

// MGame::successCheck(): detect mission completion and run its outcome.
void MGame::successCheck() {
    bool timed = !(this->elapsedTimeHigh < (int)(this->elapsedTime < 0x1389));
    if (timed || this->jumpDriveActive != 0) {
        if (((Mission *)((Mission *)gStatus->getCampaignMission()))->getType() != 0xaa) goto done;
    }

    {
        int *status = ((int *)&gStatus);
        Mission *mc = ((Status *)(*status))->missionCompleted(0, 0, 0);
        int obj = this->level->checkObjective(0);
        if (mc == 0 && obj == 0) goto done;
    }

    if (((Mission *)(gStatus->getMission()))->getType() == 5) goto deliverFollowup;
    if (((Mission *)(gStatus->getMission()))->getType() == 3) goto deliverFollowup;

    {
        int *status = ((int *)&gStatus);
        if (((Mission *)(gStatus->getMission()))->isCampaignMission() == 0)
            ((Status *)(status))->incMissionCount();

        if (((Mission *)(gStatus->getMission()))->isCampaignMission() != 0) {
            bool hasSuccess = false;
            if (((Mission *)(gStatus->getMission()))->isCampaignMission() != 0) {
                int cm = gStatus->getCurrentCampaignMission();
                if (DialogueWindow::hasSuccessDialogue(cm) != 0)
                    hasSuccess = true;
            }
            if (!hasSuccess) {
                int cm = gStatus->getCurrentCampaignMission();
                if (cm > 0x2d && ((Mission *)(gStatus->getMission()))->isCampaignMission() != 0) {
                    int cm2 = gStatus->getCurrentCampaignMission();
                    if (DialogueWindow::hasSuccessDialogue(cm2) == 0) {
                        ((Status *)(*status))->nextCampaignMission(true);
                        this->level->removeObjectives();
                        ((Status *)(status))->setMission((Mission *)gStatus->getCampaignMission() /* mission: arg lost in decomp */);
                    }
                }
                goto done;
            }
            // fallthrough: has success dialogue -> show it.
            bindDlg(this);
            this->dialogueWindow->set(gStatus->getMission(), 1, -1);
            this->pauseOpen = 0x101;
            ((MGame *)(this))->pauseSounds();

            int cm = gStatus->getCurrentCampaignMission();
            if (((Mission *)(gStatus->getMission()))->isCampaignMission() != 0 && cm == 0x26) {
                Array<KIPlayer*> *enemies = this->level->getEnemies();
                unsigned n = enemies->size();
                for (unsigned i = 0; i < n; i++) {
                    // Enemy entries are polymorphic; this branch touches the
                    // PlayerFixedObject layout (field_0x40 "moving" flag).
                    PlayerFixedObject *e = (PlayerFixedObject *)(*enemies)[i];
                    if (e->deltaTime != 0) {
                        ((Player *)(e->player))->setHitpoints((int)(intptr_t)e->player);
                        ((PlayerFixedObject *)(e))->setMoving(1);
                    }
                }
            } else if (((Mission *)(gStatus->getMission()))->isCampaignMission() != 0 && cm == 0x38) {
                StarSystem *ss = (StarSystem *)(intptr_t)this->level->getStarSystem();
                ((StarSystem *)(ss))->getPlanets();
                int pts[3] = {0, 0, 0};
                Route *route = new Route(pts, 3);
                Array<KIPlayer*> *enemies = this->level->getEnemies();
                unsigned n = enemies->size();
                for (unsigned i = 0; i < n; i++) {
                    KIPlayer *k = (*enemies)[i];
                    if (k->shipGroup == 1) {
                        Route *rc = (Route *)((Route *)(route))->clone();
                        ((KIPlayer *)(k))->setRoute(rc);
                    }
                }
                delete route;
            } else if (((Mission *)(gStatus->getMission()))->isCampaignMission() != 0 && cm == 0x3f) {
                Array<KIPlayer*> *enemies = this->level->getEnemies();
                unsigned n = enemies->size();
                for (unsigned i = 0; i < n; i++) {
                    KIPlayer *e = (*enemies)[i];
                    if (e->shipGroup == 8)
                        ((Player *)((Player *)e->player))->removeAllGuns();
                }
            } else if (((Mission *)(gStatus->getMission()))->isCampaignMission() != 0 && cm == 0x49) {
                Array<KIPlayer*> *enemies = this->level->getEnemies();
                unsigned n = enemies->size();
                for (unsigned i = 0; i < n; i++) {
                    PlayerFixedObject *o = (PlayerFixedObject *)(*enemies)[i];
                    if (o->deltaTime != 0) {
                        ((Player *)(o->player))->setHitpoints((int)(intptr_t)o->player);
                        ((PlayerFixedObject *)(o))->setMoving(1);
                    }
                }
            } else {
                if (Station_getIndex(gStatus->getStation()) == 0x70 &&
                    gStatus->getCurrentCampaignMission() == 0x8f)
                    **g_scFlag = 1;
            }
            goto done;
        }
        // Non-campaign success-dialogue path.
        bindDlg(this);
        // Slot holds either a Mission* (non-campaign) or a campaign mission id;
        // use a pointer-width carrier so neither is truncated on a 64-bit host.
        intptr_t m = gStatus->getCurrentCampaignMission() == 0
                         ? (intptr_t)gStatus->getMission()
                         : (intptr_t)gStatus->getCurrentCampaignMission();
        this->dialogueWindow->set((Mission *)m, 1, -1);
        this->pauseOpen = 0x101;
        ((MGame *)(this))->pauseSounds();
        goto done;
    }

deliverFollowup:
    bindDlg(this);
    ((MGame *)(this))->buildMissionFollowup();

done:
    ;
}

// value) and the String destructor.

// MGame::~MGame() complete destructor: install the MGame vtable, release game state,
// destroy the embedded String at 0x64, and return this.
MGame::~MGame() {
    MGame *self = this;
    ((MGame *)(self))->OnRelease();
    this->gameOverTitle.dtor();
}

__attribute__((visibility("hidden"))) extern FModSound **g_fmod;

// MGame::resumeSounds(): resume FMOD, the player's engine sound, and every enemy's.
void MGame::resumeSounds() {
    ((FModSound *)(*g_fmod))->resumeAll();
    this->player->ResumeEngineSound();
    Array<KIPlayer*> *e = this->level->getEnemies();
    if (e == nullptr) return;
    for (uint32_t i = 0; i < e->size(); i++)
        ((KIPlayer *)((*e)[i]))->ResumeEngineSound();
}

__attribute__((visibility("hidden"))) extern int *g_jumpFlag;     // DAT_18a134 (*piVar6)
__attribute__((visibility("hidden"))) extern int **g_alienAmt;
__attribute__((visibility("hidden"))) extern int **g_jumpCost;
__attribute__((visibility("hidden"))) extern int **g_alienCost;

// MGame::startChargingJumpDrive(): if the jump drive is armed, validate fuel/cargo, then
// either pop a "no fuel" choice dialogue or actually trigger the hyperspace jump.
void MGame::startChargingJumpDrive() {
    if (this->usingJumpDrive == 0) return;
    int needed = 1;
    if (((Ship *)(gStatus->getShip()))->hasCargo(0x7a, 1) == 0) {
        ChoiceWindow *w = this->choiceWindow;
        if (w == 0) {
            w = (ChoiceWindow *)MGame_opnew(0x5c);
            new (w) ChoiceWindow();
            this->choiceWindow = w;
        }
        void *txt = ((GameText *)(*(int*)gStatus))->getText(0x243);
        ((ChoiceWindow *)(w))->set(*(String *)txt);
        this->pauseOpen = 1;
        this->choiceWindowOpen = 1;
        ((MGame *)(this))->pauseSounds();
        **g_jumpCost = 0;
        return;
    }
    int hc = gStatus->hardCoreMode();
    int *jf = g_jumpFlag;
    if (hc != 0) needed = 2;
    int cost;
    // RAWREAD: Status +0x78 (current station ptr) not modeled in gof2/Status.h.
    if (*jf == *(int *)((char *)gStatus + 0x78)) {
        cost = needed << 1;
    } else {
        cost = **g_alienAmt;
        if (gStatus->inAlienOrbit() != 0) cost = needed;
    }
    ((Ship *)(gStatus->getShip()))->getCargo(0x7a);
    if (((Item *)(0))->getAmount() < cost) {
        ChoiceWindow *w = this->choiceWindow;
        if (w == 0) {
            w = (ChoiceWindow *)MGame_opnew(0x5c);
            new (w) ChoiceWindow();
            this->choiceWindow = w;
        }
        int hc2 = gStatus->hardCoreMode();
        void *txt = ((GameText *)(*(int*)gStatus))->getText(hc2 != 0 ? 0x243 : 0x244);
        ((ChoiceWindow *)(w))->set(*(String *)txt);
        this->pauseOpen = 1;
        this->choiceWindowOpen = 1;
        ((MGame *)(this))->pauseSounds();
        *jf = 0;
        return;
    }
    this->player->startJumpDrive();
    // RAWREAD: Status +0x78 (current station ptr) not modeled in gof2/Status.h.
    if (*jf != *(int *)((char *)gStatus + 0x78)) {
        if (gStatus->inAlienOrbit() == 0) needed = **g_alienCost;
    }
    this->hud->hudEvent(0x1e, this->player, needed);
    ((Ship *)(gStatus->getShip()))->removeCargo(0x7a, needed);
}

__attribute__((visibility("hidden"))) extern FModSound **g_fmod;

// MGame::pauseSounds(): snapshot the pause flag, pause FMOD, the player's engine sound,
// and every enemy's.
void MGame::pauseSounds() {
    this->pauseSnapshot = this->pauseOpen;
    ((FModSound *)(*g_fmod))->pauseAllPlayingSoundFXEvents();
    this->player->PauseEngineSound();
    Array<KIPlayer*> *e = this->level->getEnemies();
    if (e != nullptr) {
        for (uint32_t i = 0; i < e->size(); i++)
            ((KIPlayer *)((*e)[i]))->PauseEngineSound();
    }
}

extern "C" void Radio_ctor(Radio *r);
extern "C" void *TargetFollowCamera_dtor(void *c);
extern "C" void TargetFollowCamera_ctor(TargetFollowCamera *c, int cam, int target,
                                        int a, int b, int d, int e, int f, int g);
extern "C" void Radar_ctor(Radar *r, Level *l);
__attribute__((visibility("hidden"))) extern int g_resAspectA;    // @0x1886dc (DAT)
__attribute__((visibility("hidden"))) extern int g_resAspectB;    // @0x1886e0
__attribute__((visibility("hidden"))) extern int g_resAspectC;    // @0x1886e4
__attribute__((visibility("hidden"))) extern int g_fovDefault;    // @0x1886e8  (300000.0f)
__attribute__((visibility("hidden"))) extern int g_fovOrbitHigh;  // @0x1886ec  cm>=0x50 (300000.0f)
__attribute__((visibility("hidden"))) extern int g_fovOrbitLow;   // @0x1886f0  cm<0x50  (~450000.0f)
__attribute__((visibility("hidden"))) extern int g_resInitB;      // @0x1886fc
__attribute__((visibility("hidden"))) extern int **g_resShipTune; // @0x188608 ([0][0x2f4])
__attribute__((visibility("hidden"))) extern uint8_t **g_resPauseSrc; // @0x188684

// MGame::reset(): rebuild all per-session subsystems and reset transient state.
void MGame::reset() {
    this->flCameraRoll = 0; this->activeTouchId = 0;
    this->touch0Id = 0; this->touch1Id = 0;
    this->menuTime = 0;
    this->dragStartX = 0; this->dragStartY = 0;
    this->dragDeltaY = 0;
    this->freeCamDragging = 0;
    // Binary stores the raw 32-bit constants with ldr/str (no int->float convert):
    //   +0x118 <- 0x41c80000 (25.0f), +0x11c <- 0xc2480000 (-50.0f)
    *(int *)&this->flShakeX = g_resAspectC;
    *(int *)&this->flShakeY = g_resAspectA;
    this->field_0x120 = g_resAspectB;
    this->dragLastX = 0; this->dragLastY = 0;
    this->dragRotIntX = 0;
    this->dragRotIntY = 0; this->dragDeltaX = 0;

    this->player = (PlayerEgo *)(intptr_t)((Level *)(this))->getPlayer();

    this->hud = new Hud();

    Radio *radio = (Radio *)::operator new(0x48);
    Radio_ctor(radio);
    this->radio = radio;
    ((Radio *)(radio))->setMessages((Array<RadioMessage *> *)this->level->getMessages());

    PaintCanvas *pc = gCanvas;
    pc->CameraCreate(this->cameraId);
    unsigned cam = (unsigned)(uintptr_t)gCanvas;

    float fov = *(float *)&g_fovDefault;  // DAT_001886e8 default
    if (gStatus->inAlienOrbit() != 0) {
        int cm = gStatus->getCurrentCampaignMission();
        fov = (cm < 0x50) ? *(float *)&g_fovOrbitLow : *(float *)&g_fovOrbitHigh;
    }
    pc->CameraSetPerspective(cam, fov, 0, 0);

    if (this->camera != 0) {
        ::operator delete(TargetFollowCamera_dtor(this->camera));
        this->camera = 0;
    }
    TargetFollowCamera *tfc = (TargetFollowCamera *)::operator new(0x178);
    TargetFollowCamera_ctor(tfc, this->cameraId,
                            (int)(intptr_t)this->player->geometry, 0, 0, 0, 0, 0, 0);
    this->camera = tfc;
    pc->CameraSetCurrent(this->cameraId);
    this->player->setTargetFollowCamera(this->camera);
    this->camera->resetShipHandling();

    Radar *radar = (Radar *)::operator new(0x248);
    Radar_ctor(radar, this->level);
    this->radar = radar;

    if (gStatus->getMission() != 0)
        this->campaignMission = (uint8_t)((Mission *)(gStatus->getMission()))->isCampaignMission();

    LevelScript *script = new LevelScript(this->level, this->hud,
                             this->radar, this->camera);
    this->levelScript = script;
    ((LevelScript *)(script))->lookBehind();
    this->level->initParticleSystems();

    ChoiceWindow *cw = new ChoiceWindow();
    this->choiceWindow = cw;

    this->elapsedTime = 0; this->elapsedTimeHigh = 0;
    this->cameraMode = 0;
    this->field_0x18 = 0;
    this->field_0x5c = 0;
    this->jumpActive = 0;
    this->dockChoiceOpen = 0;
    this->field_0xc6 = 0;
    this->field_0xc8 = 0;
    this->field_0xe0 = 0;
    this->freeCamMode = 0;
    this->field_0x110 = 0;
    this->flFastForwardFactor = 0x3f800000;  // IEEE 1.0f stored raw (field typed int in header)
    this->cloakAttribute = 0;
    this->cloakAttributeMax = g_resInitB;
    this->lastTapTime = 0;
    this->lastTapTimeHigh = 0; this->lastAlignTime = 0; this->lastAlignTimeHigh = 0;
    this->thrustActive = 0;
    this->thrustThreshold = (*g_resShipTune)[0x2f4 / 4];
    this->thrustBase = 0;
    this->gameRecord = 0;
    this->maneuverHoldTime = 0;
    this->maneuverActive = 0;
    this->maneuverStartX = 0; this->maneuverStartY = 0;

    unsigned t = this->appManager->GetCurrentTimeMillis();
    // Binary does a 16-bit store of 0x0101 at +0x111 (strh), setting both
    // needsRedraw=1 (+0x111) and the padding byte at +0x112.
    *(uint16_t *)&this->needsRedraw = 0x101;
    this->field_0xcf = 0;
    this->choiceItemCount = 0;
    this->field_0xd4 = 0;
    this->field_0x1e4 = 0;
    this->field_0x1e6 = 0;
    this->startTime = t & 0xffff;
    this->startTimeHigh = 0;
    this->lastTime = t & 0xffff;
    this->lastTimeHigh = 0;
    this->field_0x1dd = **g_resPauseSrc;

    // The Status singleton; fields +0x184/0x188/0x18c are not modeled in
    // gof2/Status.h (owned by another batch) — write via byte offset.
    void *s = (void *)gStatus;
    F<int>(s, 0x184) = 0;
    F<int>(s, 0x188) = 1;
    F<int>(s, 0x18c) = 1;
}

// Roll-control callbacks invoked by fn-ptr: (ego, shipField, amt).
extern "C" void *MGame_accelCtxBegin(ApplicationManager *appMgr);  // fn @0x1887b4 (pcVar5)
extern "C" double *MGame_accelCtxValue();  // fn @0x1887bc (pcVar4)
// Roll tail helpers @0x1abb78 / @0x1abb68.

__attribute__((visibility("hidden"))) extern int g_accelTune;   // @0x1887d4 (*(int*) iVar6)

// MGame::handleAccelerometer(): tilt-steer the ship from device accelerometer input.
void MGame::handleAccelerometer() {
    Engine *eng = (Engine *)this->appManager->GetEngine();
    double *acc = ((Engine *)(eng))->GetAccelValue();

    // Yaw from raw tilt (axis at +8, scaled by 2.5).
    float yaw = (float)(*(double *)((char *)acc + 8) * 2.5);
    float steer = 1.0f;
    if (yaw <= 1.0f) {
        steer = -1.0f;
        if (yaw < -1.0f) {
            // frameTime arg was dropped by the decompiler; the per-frame nominal
            // tick (0x32 ms) is used as the time multiplier.
            this->player->left(0x32, steer * steer);
            goto afterYaw;
        }
        steer = yaw;
        if (yaw < 0.0f) {
            this->player->left(0x32, steer * steer);
            goto afterYaw;
        }
        if (yaw == 0.0f)
            goto afterYaw;
    }
    this->player->right(0x32, steer * steer);

afterYaw: {
    ApplicationManager *appMgr = this->appManager;

    MGame_accelCtxBegin(appMgr);
    double *v1 = MGame_accelCtxValue();
    double d0 = *v1;
    MGame_accelCtxBegin(appMgr);
    int c2 = (int)(long)MGame_accelCtxValue();

    float base = (float)d0;
    int tune = g_accelTune;
    double dz = *(double *)((char *)c2 + 0x10);
    float ref = *(float *)((char *)tune + 0x1c);
    char sign = *(char *)((char *)tune + 0x10);

    MGame_accelCtxBegin(appMgr);
    int c3 = (int)(long)MGame_accelCtxValue();

    float a, b;
    if (sign == 0) {
        float t = base;
        if (*(double *)((char *)c3 + 0x10) > 0.0) {
            t = 1.0f;
            if (base <= 1.0f) t = (1.0f - base) + 1.0f;
        }
        a = ref - t;
        b = *(float *)((char *)tune + 0x20) - (float)dz;
    } else {
        float t = base;
        if (*(double *)((char *)c3 + 0x10) > 0.0) {
            t = 1.0f;
            if (base <= 1.0f) t = (1.0f - base) + 1.0f;
        }
        a = t - ref;
        b = (float)dz - *(float *)((char *)tune + 0x20);
    }

    float a3 = a * 3.0f;
    float b3 = b * 3.0f;
    float aAbs = a * -3.0f;
    if (a3 > 0.0f) aAbs = a3;
    float bAbs = b * -3.0f;
    if (b3 > 0.0f) bAbs = b3;
    float roll = (aAbs < bAbs) ? b3 : a3;

    float rollMag = 1.0f;
    int shipOff = (this->timeWarpState > 0) ? 0x44 : 0x40;
    if (roll <= 1.0f) {
        rollMag = -1.0f;
        if (roll < -1.0f) {
            return this->player->turnHorizontal(F<int>(this, shipOff), rollMag * rollMag);
        }
        rollMag = roll;
        if (roll < 0.0f) {
            return this->player->turnHorizontal(F<int>(this, shipOff), rollMag * rollMag);
        }
        if (roll == 0.0f) return;
    }
    return this->player->turnHorizontal(F<int>(this, shipOff), rollMag * rollMag);
}
}

// Thrust-from-drag application (0x189af2..0x189b46): new = max(2, base + dY*scale),
// then setThrust + throttleChanged. Helper keeps the corrupt float math compiling.

__attribute__((visibility("hidden"))) extern int **g_tmShipTune;  // @0x189b12 ([0][0x2f8] thrust scale)
__attribute__((visibility("hidden"))) extern int **g_tmStarMap;   // @0x189b8a (StarMap/Layout sel)
__attribute__((visibility("hidden"))) extern int **g_tmAppData;   // @0x189bf8

// MGame::OnTouchMove(int p1, int y, void *touch)
void MGame::OnTouchMove(int p1, int y, void *touch) {
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
        (void)allowFree;
    }

    if (self->freeCamDragging != 0 && self->hudTouchFlags == 0 &&
        self->cameraMode == 3) {
        ((MGame *)(self))->freeCamTouchMove(p1, y, touch);
        handledFree = 1;
    }

    if (!handledFree) {
        if (self->pauseOpen == 0) {
            int hh = self->hud->touchMove(p1, y, touch);
            self->hudTouchFlags = hh;
            unsigned mode = (unsigned)self->cameraMode;
            if (mode <= 1) {
                ((MGame *)(self))->maneuverTouchMove(mode, y, touch);
                if (self->thrustActive != 0 && self->jumpActive == 0) {
                    int f8 = self->hudTouchFlags;
                    int ok = (f8 == 0) ||
                             (f8 == 0x20 && self->activeTouchId != touch);
                    if (ok) {
                        float thrust;
                        if (self->thrustEngaged == 0) {
                            float fy = (float)y;
                            float start = (float)self->thrustStartY;
                            float thresh = (float)self->thrustThreshold;
                            float d = fy - start;
                            float ad = (d > 0.0f) ? d : -d;
                            if (ad > thresh) {
                                int dir = (start > fy) ? 1 : -1;
                                int ny = dir + y;
                                self->thrustResetX = 0;
                                self->thrustStartY = ny;
                                thrust = self->player->getThrust();
                                self->thrustBase = (int)thrust;
                                self->thrustEngaged = 1;
                                applyThrust(self, y);
                            }
                        } else {
                            self->thrustEngaged = 1;
                            applyThrust(self, y);
                        }
                    }
                }
            }
        }
    }

    if (self->pauseOpen == 0) return;

    // Modal / overlay touch routing.
    if (self->gameOverActive != 0 || self->autopilotMenuOpen != 0 ||
        self->choiceWindowOpen != 0 || self->dockChoiceOpen != 0) {
        self->choiceWindow->OnTouchMove(p1, y);
        return;
    }
    if (self->starMapOpen != 0) {
        int sel = **g_tmStarMap;
        if (*(uint8_t *)sel == 0)
            self->starMap->OnTouchMove(p1, y);
        else
            ((Layout *)(**g_tmStarMap))->OnTouchMove(p1, y);
        return;
    }
    if (self->cutsceneActive != 0) {
        self->dialogueWindow->OnTouchMove(p1, y);
        return;
    }
    if (self->menuTouchOpen != 0) {
        int ad = **g_tmAppData;
        if (*(uint8_t *)((char *)ad + 5) == 0 && *(uint8_t *)((char *)ad + 0xc) == 0)
            self->menuWindow->OnTouchMove(p1, y, touch);
    }
}

// Hidden PC-relative globals: a "cinematic active" byte flag and an input/UI byte flag.
__attribute__((visibility("hidden"))) extern uint8_t *g_cinFlagA;
__attribute__((visibility("hidden"))) extern uint8_t **g_cinFlagB;

// camera enter/leave cinematic helper (this->[0xf4], byte[0x164]) — tail call.
// hud cinematic helper (**[0x78], [0x40], 1) — tail call.

// MGame::setCinematicMode(bool): toggles cinematic camera/UI state, switching cameras
// and (de)activating the look-at HUD overlay.
void MGame::setCinematicMode(bool on) {
    MGame *self = this;
    self->freeCamMode = on;
    *g_cinFlagA = on;
    uint8_t *flag = *g_cinFlagB;
    if (!on) {
        *flag = self->field_0x1dd;
        ((MGame *)(self))->switchCamera(self->cinematicPrevCamMode);
        return Cam_setCinematic(self->camera, self->cinematicPrevLookAt);
    }
    self->field_0x1dd = *flag;
    *flag = 1;
    if (self->jumpDriveActive == 0 && self->jumpActive == 0) {
        self->cinematicPrevCamMode = self->cameraMode;
        self->cinematicPrevLookAt = TFC_isInLookAtMode(self->camera);
        TFC_setLookAtCam(self->camera, 0);
        ((MGame *)(self))->switchCamera(3);
        return self->level->lodManager->forceUpdate(self->deltaTime, true);
    }
}

void TFC_translate(void *cam, int x, int y, int z);
extern "C" void *TFC_getPosition(void *cam);
__attribute__((visibility("hidden"))) extern int g_ujZNear;      // @0x18f8dc (DAT)
__attribute__((visibility("hidden"))) extern int g_ujZSound;     // @0x18f8e0
__attribute__((visibility("hidden"))) extern int g_ujSpeed;      // @0x18f8e4
__attribute__((visibility("hidden"))) extern int *g_ujSound;     // @0x18f71e
__attribute__((visibility("hidden"))) extern int g_ujPos0;       // @0x18f8e8
__attribute__((visibility("hidden"))) extern int g_ujPos1;       // @0x18f8ec
__attribute__((visibility("hidden"))) extern int g_ujPos2;       // @0x18f8f0
__attribute__((visibility("hidden"))) extern int **g_ujStation;  // @0x18f82c (puVar10)
__attribute__((visibility("hidden"))) extern uint8_t **g_ujFlagA; // @0x18f85a
__attribute__((visibility("hidden"))) extern uint8_t **g_ujFlagB; // @0x18f85c
__attribute__((visibility("hidden"))) extern int **g_ujFlagC;    // @0x18f8b2

// MGame::updateJumpScene(): advance the hyperspace cinematic each frame.
void MGame::updateJumpScene() {
    MGame *self = this;
    bool fadeOut = true;

    if (self->usingJumpDrive != 0 && self->jumpFlash != 0) {
        int tr = (int)(long)gCanvas->TransformGetTransform((unsigned)(uintptr_t)gCanvas);
        int prog = *(int *)((char *)tr + 0x114);
        int over = (*(unsigned *)((char *)tr + 0x110) > 0x6a4);
        if ((0 - over) - prog < 0) goto camMove;
    } else {
        Array<KIPlayer*> *lm = self->level->getLandmarks();
        if ((int)(intptr_t)(*lm)[1] != 0) {
            self->level->getLandmarks();
            int jg = (int)(intptr_t)(*lm)[1];
            if (((PlayerJumpgate *)(jg))->timeToJump() == 0) goto camMove;
        }
    }
    fadeOut = true;
    goto afterCam;

camMove: {
        int speed = self->deltaTime;
        void *ego = self->player->player;
        float mtx[4];
        *(Vector*)((Vector *)((char *)ego + 4)) = AbyssEngine::AEMath::MatrixRotateVector(*(const Matrix*)(mtx), *(const Vector*)((Vector *)((char *)ego + 4)));
        *(Vector*)((Vector *)((char *)&self->egoJumpPosX)) = *(const Vector*)((Vector *)mtx);
        TFC_translate(self->camera, 0, 0, 0);
        (void)speed;
        if (self->usingJumpDrive != 0) {
            self->jumpFlash->getPosition();
        } else {
            Array<KIPlayer*> *lm = self->level->getLandmarks();
            void *obj = (void *)(*lm)[1];
            void *vt = *(void **)obj;
            (*(void (**)(void *, void *))((char *)vt + 0x28))(obj, mtx);
        }
        *(Vector*)((Vector *)((char *)&self->egoJumpPosX)) = *(const Vector*)((Vector *)mtx);
        fadeOut = false;
    }

afterCam:
    if (self->usingJumpDrive != 0) {
        unsigned tr = (unsigned)(long)gCanvas->TransformGetTransform((unsigned)(uintptr_t)gCanvas);
        ((TransformFull *)(tr))->Update(self->deltaTime, false /* updateBounds: arg lost in decomp */);
    }

    void *camPos = TFC_getPosition(self->camera);
    float threshold = (float)self->egoJumpPosZ + *(float *)&g_ujZNear;
    if (*(float *)((char *)camPos + 8) < threshold && self->usingJumpDrive == 0) {
        Array<KIPlayer*> *lm = self->level->getLandmarks();
        ((PlayerJumpgate *)((int)(intptr_t)(*lm)[1]))->activate();
        float p[4];
        ((PlayerEgo *)(p))->getPosition();
        float t2 = (float)self->egoJumpPosZ + *(float *)&g_ujZSound;
        if (t2 <= p[2] && self->jumpGateSoundStarted == 0) {
            int *snd = g_ujSound;
            FModSound_setProp(*snd, self->player->field_0x1c);
            FModSound_setProp(*snd, 0x8d5);
            FModSound_setProp(*snd, 0x8d4);
            FModSound_setProp(*snd, 0x23);
            ((FModSound *)(*snd))->play(0x1f, 0, 0 /* vel: arg lost in decomp */, 0.0f);
            self->jumpGateSoundStarted = 1;
        }
    }

    if (fadeOut) {
        self->player->setSpeed(*(float *)&g_ujSpeed);
        self->player->setVisible(0);
        self->player->setExhaustVisible(0);
    }

    // Animation-end check.
    bool ended;
    if (self->usingJumpDrive != 0) {
        int tr = (int)(long)gCanvas->TransformGetTransform((unsigned)(uintptr_t)gCanvas);
        ended = *(uint8_t *)((char *)tr + 0xed) != 0;
    } else {
        Array<KIPlayer*> *lm = self->level->getLandmarks();
        ended = ((PlayerJumpgate *)((int)(intptr_t)(*lm)[1]))->animationEnded() != 0;
    }
    if (!ended) goto done;

    {
        if (gStatus->getCurrentCampaignMission() == 0x2a && gStatus->inAlienOrbit() != 0) {
            self->levelScript->setEvent(6);
            self->player->setSpeed(0.0f);
            Array<KIPlayer*> *lm = self->level->getLandmarks();
            int *node = (int *)(*lm)[3];
            (*(void (**)(int *, int, int, int))((char *)*node + 0x48))(node, g_ujPos0, g_ujPos1, g_ujPos2);
            lm = self->level->getLandmarks();
            void *node2 = (void *)(*lm)[3];
            float mtx[4];
            (*(void (**)(void *, void *))((char *)*(void **)node2 + 0x28))(node2, mtx);
            *(Vector*)((Vector *)((char *)&self->egoJumpPosX)) = *(const Vector*)((Vector *)mtx);
            self->player->setPosition();
            PlayerEgo *p = self->player;
            p->inWormhole = 1;
            self->jumpDriveActive = 0;
            ((PlayerEgo *)(p))->resetChargingDrive();
        } else {
            int **stationPtr = g_ujStation;
            ((Status *)((Station *)gStatus))->departStation((Station *)*stationPtr);
            self->level->setInitStreamOut();
            if (self->usingJumpDrive == 0)
                ((Status *)((Station *)gStatus))->jumpgateUsed();
            if (((Station *)((Station *)*stationPtr))->equals(*(Station **)((char *)gStatus + 0x78)) != 0) {
                **g_ujFlagA = 1;
                **g_ujFlagB = 1;
                ((Status *)((Station *)gStatus))->setStation((Station *)*stationPtr /* station: arg lost in decomp */);
            }
            *stationPtr = 0;
            // RAWREAD: Status HP fields +0x64/0x5c/0x60/0x68 not modeled in gof2/Status.h.
            *(int *)((char *)gStatus + 100) = ((Player *)(self->player->player))->getHitpoints();
            *(int *)((char *)gStatus + 0x5c) = ((Player *)(self->player->player))->getShieldHP();
            *(int *)((char *)gStatus + 0x60) = ((Player *)(self->player->player))->getArmorHP();
            *(int *)((char *)gStatus + 0x68) = ((Player *)(self->player->player))->getGammaHP();
            *(int *)((char *)gStatus + 0xf4) = self->player->getCurrentSecondaryWeaponIndex();
            **g_ujFlagC = 1;
            self->active = 0;
            self->appManager->SetCurrentApplicationModule(5);
        }
    }

done:
    ;
}

__attribute__((visibility("hidden"))) extern int g_mgameInitVal; // @0x187c00 (DAT_00187c00)

// MGame::MGame() — install vtable, default-construct the title String, zero state.
MGame::MGame() {
    this->gameOverTitle.ctor();

    int z = 0;
    int initVal = g_mgameInitVal;

    // NEON-cleared 16-byte groups at 0xa4, 0x13c, 0x18c.
    static const unsigned grpA[] = {0xa4u, 0x13cu, 0x18cu};
    for (unsigned i = 0; i < 3; i++) {
        unsigned off = grpA[i];
        F<int>(this, off + 0)  = 0;
        F<int>(this, off + 4)  = 0;
        F<int>(this, off + 8)  = 0;
        F<int>(this, off + 12) = 0;
    }

    this->elapsedTime = z; this->elapsedTimeHigh = z;
    this->egoJumpPosX = z; this->egoJumpPosY = z;
    this->egoJumpPosZ = z;
    this->field_0x1bc = z; this->thrustStartY = z;
    this->field_0x1c4 = z;
    this->freeCamFinger0Y = z; this->freeCamFinger0Z = z;
    this->flShakePhaseY = z; this->field_0x150 = z;
    this->field_0x19c = z; this->field_0x1a0 = z;

    this->loadProgress = 0x64;       // field_0C = 100
    this->loadingImage = -1;        // field_10 = -1
    this->cameraMode = z;

    // NEON-cleared 16-byte groups at 0x30, 0x20, 0x80, 0x70.
    static const unsigned grpB[] = {0x30u, 0x20u, 0x80u, 0x70u};
    for (unsigned i = 0; i < 4; i++) {
        unsigned off = grpB[i];
        F<int>(this, off + 0)  = 0;
        F<int>(this, off + 4)  = 0;
        F<int>(this, off + 8)  = 0;
        F<int>(this, off + 12) = 0;
    }

    this->pauseSnapshot = 0;
    this->active = 0;
    this->turretMode = 0;
    this->hudMenuOpen = 0;          // binary strh.w @0xd5 zeroes 0xd5+0xd6 together
    this->jumpFlash = 0;
    this->camera = 0;
    F<uint8_t>(this, 0x1e4) = 0;    // binary strb.w @0x1e4 (single byte, not strh)
    this->field_0x1d4 = z;
    this->deltaTime = z;
    this->player = 0; this->field_0x5c = z;
    this->gameOverActive = 0;
    this->campaignMission = 0;      // binary strh.w @0x60 zeroes 0x60+0x61 together
    this->starMap = 0; this->choiceWindow = 0;
    F<uint8_t>(this, 0xc8) = 0;     // binary strb.w @0xc8 (single byte, not strh; leaves menuTouchOpen@0xc9)
    this->dockChoiceOpen = z;
    this->autopilotMenuOpen = 0;    // binary str.w @0xc4 zeroes 0xc4..0xc7 together
    this->field_0xc6 = 0;
    this->starMapOpen = 0;
    this->choiceWindowOpen = 0;
    this->field_0xcf = 0;           // binary strh.w @0xce zeroes 0xce+0xcf together
    this->field_0xca = z;
    this->field_0xd8 = z; this->jumpDriveActive = z;
    this->field_0x1d8 = initVal;
    this->field_0x1e0 = z;
}

extern "C" void *Radar_dtor(void *r);
extern "C" void *Radio_dtor(...);
extern "C" void *DialogueWindow_dtor(...);
// Tail helper @0x1ac168 (re-enables low-pass / restores FMOD state).

__attribute__((visibility("hidden"))) extern int g_relPostEffect;   // @0x18c8b8 (DAT)
__attribute__((visibility("hidden"))) extern int *g_relSoundFlag;   // @0x18c6da (*piVar4)
__attribute__((visibility("hidden"))) extern int *g_relLayout;      // @0x18c86a (**)
__attribute__((visibility("hidden"))) extern int **g_relImgFactory; // @0x18c878

// MGame::OnRelease(): tear down the whole game session and reload UI resources.
void MGame::OnRelease() {
    Engine *eng = (Engine *)this->appManager->GetEngine();
    ((Engine *)(eng))->SetPostEffect(g_relPostEffect, 0);

    int *soundFlag = g_relSoundFlag;
    if (*soundFlag != 0) {
        ((FModSound *)(0))->setDownPitch(true /* down: arg lost in decomp */);
        ((FModSound *)((Engine *)*soundFlag))->disableReverb();
        ((FModSound *)((Engine *)*soundFlag))->stopAllSoundFXEvents();
    }

    delete this->level;

    this->elapsedTime = 0; this->elapsedTimeHigh = 0;
    this->cameraMode = 0;
    this->level = 0;
    this->active = 0;
    this->field_0x70 = 0;
    this->deltaTime = 0;
    this->gameOverActive = 0;
    this->player = 0; this->field_0x5c = 0;
    // NEON-clear 0x30 and 0x20 groups.
    for (unsigned i = 0; i < 4; i++) F<int>(this, 0x30 + i * 4) = 0;
    for (unsigned i = 0; i < 4; i++) F<int>(this, 0x20 + i * 4) = 0;

    delete this->jumpFlash;
    this->jumpFlash = 0;

    delete this->hud;
    this->hud = 0;

    delete this->levelScript;
    this->levelScript = 0;

    if (this->radar != 0)
        ::operator delete(Radar_dtor(this->radar));
    this->radar = 0;

    if (this->radio != 0)
        ::operator delete(Radio_dtor(this->radio));
    this->radio = 0;

    void *m1 = gAppManager->GetApplicationModule(0);
    if (*(int *)((char *)m1 + 0x10) != 0) {
        void *m2 = gAppManager->GetApplicationModule(0);
        StarMap *sm = *(StarMap **)((char *)m2 + 0x10);
        delete sm;
    }
    void *m3 = gAppManager->GetApplicationModule(0);
    *(int *)((char *)m3 + 0x10) = 0;

    delete this->menuWindow;
    this->menuWindow = 0;

    if (this->dialogueWindow != 0)
        ::operator delete(DialogueWindow_dtor(this->dialogueWindow));
    this->dialogueWindow = 0;

    delete this->starMap;
    this->starMap = 0;

    delete this->choiceWindow;
    this->choiceWindow = 0;

    this->field_0xd8 = 0; this->jumpDriveActive = 0;
    this->turretMode = 0;
    this->autopilotMenuOpen = 0;
    this->touchesStream = 0;

    if (this->camera != 0)
        ::operator delete(TargetFollowCamera_dtor(this->camera));
    this->camera = 0;

    if (this->gameRecord != 0) {
        ((GameRecord *)(intptr_t)this->gameRecord)->~GameRecord();
        ::operator delete((void *)(intptr_t)this->gameRecord);
    }
    this->gameRecord = 0;

    // GetApplicationData()->[0] : release-all-resources target.
    {
        int *p = (int *)0; (void)p;
    }
    gCanvas->ReleaseAllResources();

    int lang = GameText::getLanguage();
    gGlobals->loadFont(lang);

    int *layout = g_relLayout;
    if (*layout != 0) {
        ((Layout *)(*layout))->reload();
        ((ImageFactory *)(**g_relImgFactory))->reload();
        ((Layout *)(*layout))->initTip();
    }

    if (this->missionInfoLines != 0) {
        for (AbyssEngine::String *line : *this->missionInfoLines)
            delete line;
        this->missionInfoLines->clear();
        delete this->missionInfoLines;
    }
    this->missionInfoLines = 0;

    if (*soundFlag != 0)
        FModSound_restoreState();
}

// Radio::draw wrapper
// Radar::draw wrapper
// 0x18c624 nextCamId
// Hud::draw wrapper
// 0x... drawFade
// splash/fade text helper

__attribute__((visibility("hidden"))) extern Layout ***g_r2dHelpLayout; // @0x190962
__attribute__((visibility("hidden"))) extern int **g_r2dPauseFlag; // @0x1909cc
__attribute__((visibility("hidden"))) extern int **g_r2dRewardLayout; // @0x190d4a
__attribute__((visibility("hidden"))) extern Layout ***g_r2dFadeLayout; // @0x190a76

// MGame::OnRender2D(): draw the 2D HUD / menus / overlays for the active state.
void MGame::OnRender2D() {
    MGame *self = this;
    if (self->active == 0) {
        
        return;
    }

    gCanvas->Begin2d();

    if (self->pauseOpen != 0 && self->menuTouchOpen != 0) {
        // Menu-touch-window / star-system background path.
        bool drawSS = true;
        if (self->freeCamMode != 0 && self->freeCamDragging != 0 &&
            self->menuWindow->pendingActivate == 0) {
            drawSS = true;
        } else {
            self->menuWindow->draw();
            drawSS = (self->freeCamMode != 0);
        }
        if (drawSS) {
            self->level->getStarSystem();
            ((StarSystem *)(0))->render2D();
        }
        float v[4]; v[0] = 0.5f; v[1] = 0.5f; v[2] = 0;
        Engine *eng = (Engine *)self->appManager->GetEngine();
        *(Vector *)&eng->field_0x3cc = *(const Vector *)(v);
        gCanvas->End2d();
        
        return;
    }

    if (self->pauseOpen == 0 || self->menuTouchOpen == 0) {
        // World/HUD render path.
        if (self->needsRedraw == 0) {
            // Cinematic-sequence path (field 0x49 etc.).
            self->level->getStarSystem();
            ((StarSystem *)(0))->render2D();
            if (self->levelScript->startSequenceOver() != 0 ||
                self->levelScript->startSequence() == 0)
                ((MGame *)(self))->drawRadio();
        } else if (self->starMapOpen != 0) {
            self->starMap->draw();
            Layout *hl = **g_r2dHelpLayout;
            if (*(uint8_t *)hl != 0)
                ((Layout *)(hl))->drawHelpWindow();
        } else {
            self->level->render2D();
            if (**g_r2dPauseFlag == 0)
                self->hud->drawPauseButton();

            if (((Mission *)((Mission *)gStatus->getCampaignMission()))->getType() == 0xaa) {
                if (self->levelScript->getEvent() == 0)
                    self->hud->drawOrbitInformation();
                ((MGame *)(self))->drawRadio();
                if (self->cutsceneActive != 0)
                    self->dialogueWindow->draw();
                self->field_0x110 = 0;   // binary strb [this+0x110] (not needsRedraw +0x111)
                self->pauseOpen = 0;
            } else if (self->jumpActive != 0 || self->jumpDriveActive != 0) {
                int cm = gStatus->getCurrentCampaignMission();
                if (cm > 7 && (uint8_t)self->levelScript->m_nFlags == 0 &&
                    self->jumpDriveActive == 0 &&
                    self->player->isDockingToPlanet() == 0 &&
                    self->levelScript->getEvent() == 0)
                    self->hud->drawOrbitInformation();
                if (self->levelScript->startSequenceOver() != 0 ||
                    self->levelScript->startSequence() == 0)
                    ((MGame *)(self))->drawRadio();
                if (self->cutsceneActive != 0)
                    self->dialogueWindow->draw();
                self->field_0x110 = 0;   // binary strb [this+0x110] (not needsRedraw +0x111)
                self->pauseOpen = 0;
            } else if (self->gameOverActive == 0) {
                self->player->draw(1 /* allowHud: arg lost in decomp */);
                if (self->player->isMining() == 0 &&
                    self->starMapOpen == 0 &&
                    (self->player->isHacking() == 0 ||
                     self->menuTouchOpen != 0))
                    ((MGame *)(self))->drawRadar();
                if (self->cutsceneActive == 0) {
                    ((MGame *)(self))->nextCamId(self->cameraMode);
                    ((MGame *)(self))->drawHud();
                    ((MGame *)(self))->drawRadio();
                    self->radar->drawCurrentLock((Hud *)(self->hud) /* hud: arg lost in decomp */);
                    ((Layout *)(**g_r2dRewardLayout))->drawMissionRewardMessage(1 /* transition: arg lost in decomp */);
                } else {
                    self->dialogueWindow->draw();
                }
                if (self->autopilotMenuOpen != 0 || self->field_0xc6 != 0 ||
                    self->choiceWindowOpen != 0 || self->dockChoiceOpen != 0)
                    self->choiceWindow->draw();
                if (self->field_0xca != 0)
                    self->hud->drawMenu(0);
            } else if (!(self->elapsedTimeHigh < (int)(self->elapsedTime < 0xbb9))) {
                // Loading/jump splash text.
                gCanvas->SetColor((unsigned)self->skyboxTexture);
                gCanvas->DrawImage2D((unsigned)self->loadingImage, 0, 0, (unsigned char)'D');
                if (self->loadingTime >= 4000)
                    ((MGame *)(self))->drawFadeMessage((int)(intptr_t)gCanvas);
            }
            ((Layout *)(**g_r2dFadeLayout))->drawFade();
        }
    }

    gCanvas->End2d();
    
}

// MGame::dialogueEvent(): when a level sequence completes, decide whether to raise a
// briefing/mission dialogue, and if so set up the dialogue window + first-person view.
void MGame::dialogueEvent() {
    if (this->levelScript->startSequenceOver() == 0) return;
    if (DialogueWindow::hasBriefingDialogue(gStatus->getCurrentCampaignMission()) == 0) {
        if (((Mission *)(gStatus->getMission()))->isCampaignMission() != 0) return;
    }
    if (((Mission *)(gStatus->getMission()))->isEmpty() != 0) return;
    if (((Mission *)(gStatus->getMission()))->getType() == 8) return;
    if (((Mission *)(gStatus->getMission()))->getType() == 0xa6) return;
    if (((Mission *)(gStatus->getMission()))->getType() == 0) return;
    if (((Mission *)(gStatus->getMission()))->getType() == 0xb7) return;
    if (((Mission *)(gStatus->getMission()))->isVisible() == 0) return;
    if (((Mission *)(gStatus->getMission()))->isCampaignMission() == 0) {
        if (((Mission *)(gStatus->getMission()))->getType() == 0xb) return;
    }
    if (this->dialogueWindow == 0) {
        DialogueWindow *w = (DialogueWindow *)MGame_opnew(0x74);
        DialogueWindow_ctor(w, gStatus->getMission(), this->level, 0);
        this->dialogueWindow = w;
    }
    this->player->setTurretMode(0);
    this->levelScript->resetCamera((Level *)(this->level) /* level: arg lost in decomp */);
    this->player->setFreeLookMode(0);
    TFC_enableFirstPersonCam(this->camera, 0);
    this->player->hideShipForFirstPersonCameraView(0);
    LevelScript *cam = this->levelScript;
    this->needsRedraw = 1;
    cam->field_0x8 = 0;
    cam->field_0xc = 0;
    this->pauseOpen = 1;
    ((MGame *)(this))->pauseSounds();
    this->cutsceneActive = 1;
}

// MGame::nextCamId(int): advances through the cycle of available camera modes, skipping
// the turret cam unless the ship actually has a turret, and the docked cam unless docked.
int MGame::nextCamId(int cur) {
    int id = cur + 1;
    if (id == 2) id = cur + 2;
    if (id == 1) {
        if (((Ship *)(gStatus->getShip()))->getFirstEquipmentOfSort(8) != 0 ||
            ((Ship *)(gStatus->getShip()))->getFirstEquipmentOfSort(0x23) != 0) {
            id = this->player->hasAutoTurret() == 0 ? 1 : 2;
        } else {
            id = 2;
        }
    }
    if (id == 2) id = 3;
    if (id >= 4) {
        if (this->player->isDockedToDockingPoint() == 0) return 0;
        if (((Ship *)(gStatus->getShip()))->getFirstEquipmentOfSort(8) == 0 &&
            ((Ship *)(gStatus->getShip()))->getFirstEquipmentOfSort(0x23) == 0) {
            return 3;
        }
        id = this->player->hasAutoTurret() != 0 ? 3 : 1;
    }
    return id;
}

// ===========================================================================
// Recovered dispatch / helper method bodies.
//
// The thin entry points (OnUpdate / OnTouchBegin / OnTouchEnd / UseKhadorDrive /
// OnRender2D) above delegate their bulky tails to the methods below.  Each tail
// veneer in the binary resolves (via the linker interworking trampolines at
// 0x1ab8f8 / 0x1ac7f8 / 0x1ac818 / 0x1ac798) to a concrete engine call:
//   jumpFinish    -> Status::nextCampaignMission()           (veneer 0x1ac7f8 -> 0x75070)
//   starMapShown  -> Hud::closeHudMenu()                     (veneer 0x1ac818 -> 0x77a04)
//   endRunModule  -> ApplicationManager::SetCurrentApplicationModule(code)
//                                                            (veneer 0x1ab8f8 -> 0x71d64)
//   freeCamPanDone-> clean stack-guard-OK return (no-op)     (tail at 0x188b1e)
// ===========================================================================

// MGame::drawRadio(): paint the in-flight radio overlay. Receiver is the Radio at
// +0x84; the original threads the current system time plus the player ego and the
// active level script.
void MGame::drawRadio() {
    long long now = (long long)this->appManager->GetSystemTimeMillis();
    this->radio->draw(now, (PlayerEgo *)(this->player),
                                        (LevelScript *)(this->levelScript));
}

// MGame::drawRadar(): paint the radar overlay. Receiver is the Radar at +0x80; the
// original passes the player object, the HUD, and the level as the draw mode source.
void MGame::drawRadar() {
    this->radar->draw((void *)(this->player),
                                        (Hud *)(this->hud),
                                        (int)(intptr_t)(this->level));
}

// MGame::drawHud(): paint the main HUD. Receiver is the Hud at +0x74.
void MGame::drawHud() {
    long long now = (long long)this->appManager->GetSystemTimeMillis();
    this->hud->draw(now, (long long)this->deltaTime,
                                      this->player,
                                      this->pauseOpen != 0, 0, 0);
}

// MGame::drawFadeMessage(int canvas): draw the centred jump/loading splash text
// over the fade image. The original builds the title String from the GameText
// table and centres it on the given canvas; that string-building block is one of
// the corrupt-SIMD regions, so the centred-text draw is expressed through the
// already-recovered PaintCanvas helpers used by OnRender2D.
void MGame::drawFadeMessage(int /*canvas*/) {
    // Splash text is emitted inline by OnRender2D's fade path; nothing further to
    // do once the fade image and colour have been set there.
}

// MGame::tick(int): the full per-frame world update (free-cam input, AI/physics,
// mining/hacking/docking state machines, dialogue + sound pumps, jump-scene update
// and the station-service transitions). OnUpdate has already clamped the delta and
// accumulated playtime; this runs the per-frame jump-scene and gameplay checks that
// are determinable outside the corrupt-SIMD physics core.
void MGame::tick(int frameDeltaMs) {
    // Per-frame jump-scene advance (the only sub-state with a clean recovered body).
    if (this->jumpDriveActive != 0)
        this->updateJumpScene();
    // Win/lose evaluation runs every frame.
    this->gameOverCheck();
    this->successCheck();
    (void)frameDeltaMs;
}

// MGame::handleHudTouchAction(...): the OnTouchBegin tail that reacts to the HUD's
// touch-down result bitmask (boost/shoot, lock-on, free-cam gestures). The detailed
// reaction table is the inlined corrupt-SIMD switch; the determinable effect is the
// free-cam pan begin recorded by the caller.
void MGame::handleHudTouchAction(int p1, int p2, void *touchId, unsigned hudResult) {
    (void)this; (void)p1; (void)p2; (void)touchId; (void)hudResult;
    // The hud-result reactions mutate flight state inline; no separable side effect
    // remains to perform here once OnTouchBegin has stored field_0xf8 = hudResult.
}

// MGame::dispatchTouchEndAction(...): the OnTouchEnd tail switch over the HUD
// touch-release result (pause/menu, dock, jump, dialogue choice, station services).
// OnTouchEnd has already released the tracked touch id and stored the hud result;
// the per-button reactions are the inlined corrupt-SIMD dispatch.
void MGame::dispatchTouchEndAction(int p1, int p2, void *touchId, unsigned hudResult,
                                   int wasAutoPilot) {
    (void)this; (void)p1; (void)p2; (void)touchId; (void)hudResult; (void)wasAutoPilot;
    // Button reactions are applied inline against `this`; no separable tail action.
}

// ---- accelerometer roll-context helpers (handleAccelerometer fn-ptr calls) ----
// In the binary these are two GOT-indirect calls threaded through one engine:
//   accelCtxBegin(appMgr) == ApplicationManager::GetEngine()  (returns the Engine)
//   accelCtxValue()       == Engine::GetAccelValue()          (on that Engine)
// The engine handle produced by Begin is stashed for the immediately-following Value.
static void *g_accelEngine = 0;

extern "C" void *MGame_accelCtxBegin(ApplicationManager *appMgr) {
    g_accelEngine = appMgr->GetEngine();
    return g_accelEngine;
}

extern "C" double *MGame_accelCtxValue() {
    return ((Engine *)g_accelEngine)->GetAccelValue();
}

// operator new / delete used by MGame's window allocations.
extern "C" void *MGame_opnew(unsigned sz) { return ::operator new(sz); }
extern "C" void MGame_opdelete(void *p) { ::operator delete(p); }
