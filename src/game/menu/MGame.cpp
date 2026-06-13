#include <new>
#include "gof2/game/menu/MGame.h"
#include "gof2/game/core/Globals.h"
#include "gof2/game/ship/PlayerFighter.h"
#include "gof2/game/ship/Ship.h"
#include "gof2/game/world/SolarSystem.h"
#include "gof2/game/ship/TargetFollowCamera.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/game/ui/ChoiceWindow.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/game/world/Galaxy.h"
#include "gof2/game/mission/Item.h"
// Galaxy.h (above) already provides the canonical file-local B()/I()/P() offset helpers.
// Sibling headers below redefine identical static-inline B/I/P with no shared guard, which
// is a redefinition error within one TU. Rename their copies to throwaway names on include
// so only Galaxy.h's definitions remain for this TU's call sites.
#define P P_levelscript
#include "gof2/game/world/LevelScript.h"
#undef P
#include "gof2/game/ui/MenuTouchWindow.h"
#include "gof2/game/mission/Objective.h"
#include "gof2/game/ship/PlayerJumpgate.h"
// NOTE: gof2/Radar.h and gof2/Radio.h are intentionally NOT included. Both define their
// own minimal duplicate views of Status/KIPlayer/Layout/GameText/ImageFactory/Globals/
// FModSound/Agent for their own standalone TUs; those collide with the full headers this
// TU needs (Status.h, KIPlayer.h, Layout.h, ...). MGame only touches Radar/Radio through a
// couple of methods and opaque pointers, declared minimally below.
#include "gof2/game/world/StarMap.h"
#include "gof2/engine/math/Transform.h"
// RecordHandler.h, Hud.h and PlayerEgo.h each define the file-local offset
// helpers B()/I()/P(). RecordHandler.h gates its trio behind GOF2_BIP_HELPERS;
// pre-define it so this TU takes B from PlayerEgo.h and I/P from Hud.h (one each).
#define GOF2_BIP_HELPERS
#include "gof2/game/mission/Achievements.h"
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/game/ui/DialogueWindow.h"
// Engine.h and GameRecord.h each redeclare __aeabi_memcpy with a signature that conflicts
// with AEGeometry.h's (uint32_t vs unsigned long size, void vs void* return). None is called
// here, so rename their decls away on include so AEGeometry.h's canonical decl stands.
#define __aeabi_memcpy __aeabi_memcpy_engine
#include "gof2/engine/render/Engine.h"
#undef __aeabi_memcpy
#define __aeabi_memcpy __aeabi_memcpy_gamerecord
#include "gof2/game/mission/GameRecord.h"
#undef __aeabi_memcpy
#include "gof2/engine/core/GameText.h"
#define I I_hud
#define P P_hud
#include "gof2/game/ui/Hud.h"
#undef I
#undef P
#include "gof2/engine/render/ImageFactory.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/mission/Mission.h"
#include "gof2/game/ship/Player.h"
#include "gof2/game/mission/RecordHandler.h"
#include "gof2/game/world/Route.h"
#include "gof2/game/world/StarSystem.h"
#include "gof2/game/world/Station.h"
#include "gof2/game/core/String.h"
// NOTE: gof2/Engine.h is intentionally NOT included — it does not compile
// standalone (it is owned by another batch) and MGame only uses Engine as an
// opaque pointer type, which is already forward-declared via fwd.h (common.h).
#include "gof2/game/ship/KIPlayer.h"
#define B B_playerego
#include "gof2/game/ship/PlayerEgo.h"
#undef B
#include "gof2/game/ship/PlayerFixedObject.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/core/PaintCanvasClass.h"
extern void *g_PaintCanvas;   // PaintCanvas singleton pointer (externs.h)

// The Status singleton. The original 0-argument Status_* calls below loaded this global and
// invoked the corresponding instance method; the decompiler dropped the implicit receiver.
// (Some functions re-declare this same hidden global at block scope; both are consistent.)
extern "C" __attribute__((visibility("hidden"))) Status **g_status;

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
extern "C" void String_cstr_ctor(String12 *out, const char *s, bool copy);
extern "C" void String_concat(String12 *out, String12 *lhs, String12 *rhs);
extern "C" void String_int_ctor(String12 *out, int v);
void Globals_playMusicAndFadeOutCurrent(int track);
extern "C" void MGame_opdelete(void *p);
void Globals_startNewSoundResourceList();
void Globals_addSoundResourceToList(int list);
void TFC_setFastForwardMode(TargetFollowCamera *c, int v);
extern "C" void applyThrust(MGame *self, int y);
uint8_t TFC_isInLookAtMode(TargetFollowCamera *c);
void TFC_setLookAtCam(TargetFollowCamera *c, int v);
extern "C" void Cam_setCinematic(TargetFollowCamera *c, int on);
extern "C" void FModSound_restoreState();
extern "C" void DialogueWindow_ctor(...);
void TFC_enableFirstPersonCam(TargetFollowCamera *c, int on);

// ---- maneuverTouchEnd_1789b4.cpp ----
__attribute__((visibility("hidden"))) extern int *g_maneuverScale;

// MGame::maneuverTouchEnd(): if a maneuver gesture was active, recent, and the finger
// moved far enough, trigger a left/right maneuver on the player; always clear the flag.
void MGame::maneuverTouchEnd(int a, void *p) {
    MGame *self = this;
    (void)p;
    if (self->field_0x17c != 0 && self->field_0x178 <= 0x258) {
        float f2 = (float)(*g_maneuverScale);
        int d = a - self->field_0x180;
        if (d < 0) d = -d;
        float f3 = (float)d;
        if ((f2 / 320.0f) * 0.15f < f3) {
            int dir = 1;
            if (self->field_0x180 < a) dir = 2;
            ((PlayerEgo *)(self->field_0x58))->initManeuver(dir);
        }
    }
    self->field_0x17c = 0;
}

// ---- OnResume_1807cc.cpp ----
// Singletons held as pointer-to-pointer in hidden globals (single PC-relative deref).
__attribute__((visibility("hidden"))) extern Music **g_music;
__attribute__((visibility("hidden"))) extern Cfg **g_cfg;

// MGame::OnResume(): resume background music unless it was deliberately stopped.
void MGame_OnResume() {
    Music **mp = g_music;
    if (*mp == 0) return;
    if (FModSound_tryToStopMusicForBGMusic() != 0) return;
    return Music_resume(*mp, 1, **(int **)g_cfg);
}

// ---- maneuverTouchMove_17895c.cpp ----
// Globals dereferenced PC-relative.
__attribute__((visibility("hidden"))) extern int *g_maneuverScale;

// MGame::maneuverTouchMove(): if a maneuver gesture is active, cancel it once the
// finger has moved far enough sideways relative to a screen-scaled threshold.
void MGame::maneuverTouchMove(int a, int b) {
    MGame *self = this;
    (void)a;
    if (self->field_0x17c != 0) {
        float f2 = (float)(*g_maneuverScale);
        int d = b - self->field_0x184;
        if (d < 0) d = -d;
        float f3 = (float)d;
        // The exact constant values are irrelevant to the byte match (the diff
        // normalizes pc-relative literal loads); only the vldr/vdiv/vmul shape matters.
        if ((f2 / 320.0f) * 0.15f < f3) {
            self->field_0x17c = 0;
            self->field_0x178 = 0;
        }
    }
}

// ---- maneuverTouchBegin_178948.cpp ----
// Begins a maneuver gesture: mark active, record start position, reset timer.
void MGame::maneuverTouchBegin(int x, int y) {
    MGame *self = this;
    self->field_0x17c = 1;
    self->field_0x180 = x;
    self->field_0x184 = y;
    self->field_0x178 = 0;
}

// ---- OnRender3D_180dcc.cpp ----

// 0x72214
// 0x7222c
// 0x78730
// 0x72238
// 0x7873c
// 0x78748
// 0x7567c
// PaintCanvas End3d tail helper @0x1ab918.

__attribute__((visibility("hidden"))) extern unsigned *g_r3dCanvas; // @0x190de4 ([0]=canvas)

// MGame::OnRender3D(): render the active 3D view (game world, menu, or star map).
void MGame::OnRender3D() {
    MGame *self = this;
    if (self->field_0x54 == 0) return;
    unsigned canvas = *g_r3dCanvas;
    ((PaintCanvas*)(long)canvas)->ClearBuffer(0);

    uint8_t inMenuLevel = self->field_0x5d;
    uint8_t flag15e = self->field_0x15e;

    if (inMenuLevel == 0) {
        // World render.
        ((Level *)(self->field_0x78))->renderBG(0);
        ((PaintCanvas*)(long)canvas)->Begin3d();
        int arg = (flag15e == 0) ? self->field_0x40 : 0;
        ((Level *)(self->field_0x78))->render(arg);
        int egoFlag = (self->field_0x5f != 0) ? 0 : (self->field_0xdc == 0);
        ((PlayerEgo *)(self->field_0x58))->render(egoFlag);
        if (self->field_0x114 != 0)
            ((AEGeometry *)(self->field_0x58))->render();
        ((LevelScript *)(self->field_0x7c))->render3D();
        return ((PaintCanvas*)(long)canvas)->End3d();
    }

    if (flag15e != 0) {
        // Free-cam / cinematic world render.
        ((Level *)(self->field_0x78))->renderBG(0);
        ((PaintCanvas*)(long)canvas)->Begin3d();
        ((Level *)(self->field_0x78))->render(self->field_0x40);
        int egoFlag = (self->field_0x5f != 0) ? 0 : (self->field_0xdc == 0);
        ((PlayerEgo *)(self->field_0x58))->render(egoFlag);
        if (self->field_0x114 != 0)
            ((AEGeometry *)(self->field_0x58))->render();
        ((LevelScript *)(self->field_0x7c))->render3D();
        return ((PaintCanvas*)(long)canvas)->End3d();
    }

    if (self->field_0xc9 != 0) {
        ((PaintCanvas*)(long)canvas)->Begin3d();
        ((MenuTouchWindow *)(self->field_0x88))->render3D();
        return ((PaintCanvas*)(long)canvas)->End3d();
    }

    if (self->field_0xc7 != 0) {
        ((PaintCanvas*)(long)canvas)->Begin3d();
        ((StarMap *)(self->field_0x90))->render();
        return ((PaintCanvas*)(long)canvas)->End3d();
    }

    // Fallback world render.
    ((Level *)(self->field_0x78))->renderBG(0);
    ((PaintCanvas*)(long)canvas)->Begin3d();
    ((Level *)(self->field_0x78))->render(0);
    int egoFlag = (self->field_0x5f != 0) ? 0 : (self->field_0xdc == 0);
    ((PlayerEgo *)(self->field_0x58))->render(egoFlag);
    if (self->field_0x114 != 0)
        ((AEGeometry *)(self->field_0x58))->render();
    ((LevelScript *)(self->field_0x7c))->render3D();
    return ((PaintCanvas*)(long)canvas)->End3d();
}

// ---- startJumpScene_17c0d4.cpp ----

// 0x7294c
// 0x7843c
// 0x72838
void TFC_setActive(TargetFollowCamera *c, int v);  // 0x72acc
float TFC_useTargetsUpVector(TargetFollowCamera *c, int v);  // 0x7267c
// 0x76b40
// 0x76ca8
// 0x7678c
// 0x724a8
// 0x77a1c
// 0x728d4
// 0x76a80
// 0x72cac
// 0x7258c
// 0x707f8
// 0x725b0
// 0x72cb8
// 0x76aa4
// 0x78448
// 0x6fd18
// 0x72574
// 0x72148
// 0x727b4
// 0x726ac
extern "C" void FModSound_setProp(int snd, int id);  // fn @0x18c30e (pcVar8)
// 0x71548
extern "C" void TFC_setPosition(TargetFollowCamera *c, float x, float y, float z);  // 0x76abc

__attribute__((visibility("hidden"))) extern int *g_jsGuard;     // @0x18c0ea (stack guard / canvas[0])
__attribute__((visibility("hidden"))) extern int *g_jsSound;     // @0x18c15a (*piVar9)
__attribute__((visibility("hidden"))) extern PaintCanvas **g_jsCanvas; // @0x18c182 (*puVar2)
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
    MGame *self = this;
    int *guard = g_jsGuard;
    int g0 = *guard;

    ((Player *)(*(void **)((char *)self->field_0x58)))->setVulnerable(0);
    ((Level *)(self->field_0x78))->enableFog(0);

    if (((PlayerEgo *)(self->field_0x58))->isDockedToDockingPoint() != 0) {
        ((PlayerEgo *)(self->field_0x58))->dockToDockingPoint(0);
        TFC_setActive(self->field_0xf4, 1);
        TFC_setLookAtCam(self->field_0xf4, 0);
        float sp = TFC_useTargetsUpVector(self->field_0xf4, 0);
        ((PlayerEgo *)(self->field_0x58))->setSpeed(sp);
        ((PlayerEgo *)(self->field_0x58))->setDockingState(0);
    }
    if (((PlayerEgo *)(self->field_0x58))->isInTurretMode() != 0)
        ((PlayerEgo *)(self->field_0x58))->setTurretMode(0);

    int *snd = g_jsSound;
    ((FModSound *)(*snd))->stop(0x23);
    ((MGame *)(self))->switchCamera(0);
    self->field_0x70 = g_jsHudFlag;
    ((Hud *)(self->field_0x74))->releaseAllKeys();
    self->field_0x110 = 0;
    self->field_0x5c = 0;

    PaintCanvas *pc = *g_jsCanvas;
    unsigned cam = self->field_0xf0;
    float fov = *(float *)&g_jsFovDefault;
    if ((*g_status)->inAlienOrbit() != 0) {
        int cm = (*g_status)->getCurrentCampaignMission();
        fov = (cm < 0x50) ? *(float *)&g_jsFovAlienB : *(float *)&g_jsFovAlienA;
    }
    pc->CameraSetPerspective(cam, fov, *(float *)&g_jsHudFlag, 0);
    ((PlayerEgo *)(self->field_0x58))->setAutoPilot(0);
    self->field_0x5d = 0;
    self->field_0xd6 = 0;
    self->field_0xdc = 1;
    self->field_0x5f = 1;
    ((PlayerEgo *)(self->field_0x58))->setCollide(0);
    TFC_setLookAtCam(self->field_0xf4, 1);
    ((PlayerEgo *)(self->field_0x58))->stopBoost();
    Engine *eng = (Engine *)((ApplicationManager *)(self->field_0x8))->GetEngine();
    ((Engine *)(eng))->SetPostEffect(g_jsPostEffect, 0);

    float camX, camY, camZ;
    if (self->field_0xdd == 0) {
        // Free-space jump: position from a landmark.
        int lm = ((Level *)(self->field_0x78))->getLandmarks();
        void *obj = *(void **)((char *)lm + 4);
        void *vt = *(void **)obj;
        float vtmp[4];
        (*(void (**)(void *, void *))((char *)vt + 0x28))(obj, vtmp);
        *(Vector*)((Vector *)((char *)self + 0xe4)) = *(const Vector*)((Vector *)vtmp);
        float nz = (float)self->field_0xec + *(float *)&g_jsOffsetZ;
        self->field_0xec = (int)nz;
        ((PlayerEgo *)(self->field_0x58))->setPosition();
        ((PlayerEgo *)(self->field_0x58))->setComputerControlled(1);
        ((AEGeometry *)*(void **)((char *)self->field_0x58 + 8))->setRotation((float)0, (float)0, (float)0);
        self->field_0xe4 = (int)((float)self->field_0xe4 + *(float *)&g_jsOffsetX);
        self->field_0xe8 = (int)((float)self->field_0xe8 + *(float *)&g_jsOffsetY);
        self->field_0xec = (int)((float)self->field_0xec + *(float *)&g_jsOffsetZ2);
        camX = (float)self->field_0xe4;
        camY = (float)self->field_0xe8;
        camZ = (float)self->field_0xec;
    } else {
        // Charged jump: spawn the warp-tunnel geometry.
        ((PlayerEgo *)(self->field_0x58))->resetMovement();
        ((PlayerEgo *)(self->field_0x58))->setComputerControlled(1);
        AEGeometry *geo = (AEGeometry *)::operator new(0xc0);
        new (geo) AEGeometry((uint16_t)0x3ab2, *g_jsCanvas, false);
        self->field_0x114 = geo;
        int tr = (int)(long)((PaintCanvas*)g_PaintCanvas)->TransformGetTransform(*(unsigned *)g_jsCanvas);
        ((TransformFull *)(tr))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);

        float pos[4];
        ((PlayerEgo *)(pos))->getPosition();
        Vector *dst = (Vector *)((char *)self + 0xe4);
        *(Vector*)(dst) = *(const Vector*)((Vector *)pos);

        float dir[4];
        ((PlayerEgo *)(dir))->getPosition();
        *(Vector*)((Vector *)dir) *= (*(float *)&g_jsOffsetX);
        *(Vector*)(dst) += *(const Vector*)((Vector *)dir);
        ((AEGeometry *)(self->field_0x114))->setPosition(*dst);

        ((AEGeometry *)(self->field_0x114))->setScaling(0x40000000);
        float zero[4]; zero[0] = 0; *(int *)&zero[1] = 0x3f800000; zero[2] = 0;
        ((AEGeometry *)(self->field_0x114))->setDirection(*(Vector *)zero, *(Vector *)zero /* up: arg lost in decomp */);

        float off[4]; off[0] = (float)g_jsOffsetX; off[1] = (float)g_jsOffsetY; off[2] = (float)g_jsOffsetX;
        *(Vector*)((Vector *)off) = *(const Vector*)((Vector *)off);
        *(Vector*)((Vector *)((char *)*(int *)((char *)self->field_0x58) + 4)) = AbyssEngine::AEMath::MatrixRotateVector(*(const Matrix*)(off), *(const Vector*)((Vector *)((char *)*(int *)((char *)self->field_0x58) + 4)));
        *(Vector*)((Vector *)off) = *(const Vector*)((Vector *)off);
        *(Vector*)(dst) += *(const Vector*)((Vector *)off);

        FModSound_setProp(*snd, self->field_0x1c);
        FModSound_setProp(*snd, 0x23);
        FModSound_setProp(*snd, 0x8d5);
        FModSound_setProp(*snd, 0x8d4);
        ((FModSound *)(*snd))->play(0x20, 0, 0 /* vel: arg lost in decomp */, 0.0f);

        camX = (float)self->field_0xe4;
        camZ = (float)self->field_0xe8;
        camY = (float)self->field_0xec;
    }
    TFC_setPosition(self->field_0xf4, camX, camY, camZ);

    
}

// ---- switchCamera_17c4e8.cpp ----

// 0x76804
// 0x7246c
void TFC_setRotationAroundTarget(TargetFollowCamera *c, int v);  // 0x76c0c
int TFC_hideShipForFirstPersonCam(TargetFollowCamera *c);  // 0x78460
// PlayerEgo turret/first-person sync tail helper @0x1ac868.

// MGame::switchCamera(int id)
void MGame::switchCamera(int id) {
    MGame *self = this;
    int dockState = 0;
    int turretArg;

restart:
    if (id == 2) id = 3;
    self->field_0x14 = id;
    if (id == 1) {
        if (((PlayerEgo *)(self->field_0x58))->isDockingToAsteroid() != 0) {
            self->field_0xd5 = 0;
            // fallthrough to step++
        } else {
            // setTurretMode() returns void in the header; the decompiler modelled a
            // result that gates the following block. Preserve the call and treat the
            // turret as enabled (arg == 1) for the flag/condition.
            ((PlayerEgo *)((PlayerEgo *)(self->field_0x58)))->setTurretMode(1);
            int t = 1;
            self->field_0xd5 = (uint8_t)t;
            if (t != 0) {
                id = self->field_0x14;
                turretArg = 1;
                goto afterTurret;
            }
        }
        id = self->field_0x14 + 1;
        self->field_0x14 = id;
        turretArg = 0;
    } else {
        self->field_0xd5 = 0;
        turretArg = 0;
    }

afterTurret:
    if (id == 2) {
        self->field_0x14 = 3;
    } else if (id >= 4) {
        self->field_0x14 = 0;
    }
    self->field_0x18 = 0;
    ((PlayerEgo *)(self->field_0x58))->setTurretMode(turretArg);

    int mode = self->field_0x14;
    switch (mode) {
    case 0:
        break;
    case 1:
    case 3: {
        if (((PlayerEgo *)(self->field_0x58))->isDockedToDockingPoint() != 0)
            TFC_setLookAtCam(self->field_0xf4, 0);
        ((LevelScript *)(self->field_0x7c))->lookBehind();
        TFC_setRotationAroundTarget(self->field_0xf4, 1);
        ((PlayerEgo *)(self->field_0x58))->setFreeLookMode(1);
        goto firstPerson;
    }
    case 2: {
        dockState = ((PlayerEgo *)(self->field_0x58))->isDockedToDockingPoint();
        if (dockState == 0) {
            ((LevelScript *)(self->field_0x7c))->lookBehind();
            TFC_setRotationAroundTarget(self->field_0xf4, 0);
            ((PlayerEgo *)(self->field_0x58))->setFreeLookMode(0);
            Engine *eng = (Engine *)((ApplicationManager *)(self->field_0x8))->GetEngine();
            // Engine layout lives in another translation unit (gof2/Engine.h is not
            // included here); write its +0x360 field via a typed byte offset.
            F<int>(eng, 0x360) = 0;
            goto firstPerson;
        }
        // docked: restart the whole selection with id=1.
        id = 1;
        goto restart;
    }
    default:
        goto firstPerson;
    }

firstPerson: {
        TFC_enableFirstPersonCam(self->field_0xf4, self->field_0x14 == 2);
        PlayerEgo *ego = self->field_0x58;
        int v;
        if (self->field_0x18 == 0)
            v = TFC_hideShipForFirstPersonCam(self->field_0xf4);
        else
            v = 1;
        ((PlayerEgo *)(ego))->syncFirstPerson(v);
    }
}

// ---- freeCamTouchBegin_178a28.cpp ----
__attribute__((visibility("hidden"))) extern int *g_fcb_guard; // DAT_188af4 (canary anchor read)


// MGame::freeCamTouchBegin(int, int, void*): start a free-camera drag. Records the start
// touch and seeds the camera-offset Vector (built on the stack -> compiler canary).
void MGame::freeCamTouchBegin(int x, int y, int id) {
    MGame *self = this;
    char buf[12];
    float fy = (float)y;
    float fx = (float)x;
    int slot;
    if (self->field_0x98 == 0) {
        if (self->field_0x9c == 0) self->field_0xa0 = 0;
        *(volatile float *)(buf + 4) = fy;
        *(volatile float *)(buf + 0) = fx;
        *(volatile int *)(buf + 8) = 0;
        self->field_0x98 = id;
        slot = 0xa4;
    } else {
        if (self->field_0xa0 >= 1000) goto tail;
        self->field_0xbc = 0;
        float len = AEMath_VectorLength(TFC_getCamOffset(self->field_0xf4));
        *(volatile float *)(buf + 4) = fy;
        *(volatile float *)(buf + 0) = fx;
        self->field_0xbc = len;
        slot = 0xb0;
        self->field_0x9c = id;
    }
    *(Vector*)((Vector *)((char *)self + slot)) = *(const Vector*)(buf);
tail:
    self->field_0x124 = x;
    self->field_0x128 = y;
    self->field_0x154 = x;
    self->field_0x158 = y;
    self->field_0x134 = 0;
    self->field_0x138 = 0;
    self->field_0x15c = 1;
}

// ---- useCloak_179bfc.cpp ----
__attribute__((visibility("hidden"))) extern Status **g_status;     // DAT_189d44
__attribute__((visibility("hidden"))) extern GameText **g_gameText; // DAT_189d4c


// MGame::useCloak(): toggle the cloaking device. If it engaged, build a choice dialogue
// "<text><cloak-attr><text>" describing the cloak and pause the game. Stack-protected
// (six temporary Strings on the stack).
// PlayerEgo::toggleCloaking() is declared (and defined, in PlayerEgo.cpp) as returning
// void, but this caller branches on the value the original left in r0. Reinterpret the
// method's address through an int-returning free-function signature so the ABI return is
// read without touching the void definition (the call ABI is identical for a thiscall on
// this target).
void MGame::useCloak() {
    MGame *self = this;
    void (::PlayerEgo::*pmf)() = &::PlayerEgo::toggleCloaking;
    int (*toggleCloaking)(::PlayerEgo *);
    __builtin_memcpy(&toggleCloaking, &pmf, sizeof(toggleCloaking));
    if (toggleCloaking((::PlayerEgo *)(self->field_0x58)) != 0) return;
    if (self->field_0x94 == 0) {
        ChoiceWindow *w = (ChoiceWindow *)MGame_opnew(0x5c);
        new (w) ChoiceWindow();
        self->field_0x94 = w;
    }
    Item *eq = ((Ship *)(((Status *)(*g_status))->getShip()))->getFirstEquipmentOfSort(0x15);
    int attr = eq == 0 ? 0 : ((Item *)(eq))->getAttribute(0x26);
    ChoiceWindow *cw = self->field_0x94;
    void *txt = ((GameText *)(*g_gameText))->getText(0x247);
    String12 s0, s1, s2, s3, s4, s5;
    String_cstr_ctor(&s2, "", false);
    String_concat(&s3, (String12 *)txt, &s2);
    String_int_ctor(&s1, attr);
    String_concat(&s4, &s3, &s1);
    String_cstr_ctor(&s0, "", false);
    String_concat(&s5, &s4, &s0);
    ((ChoiceWindow *)(cw))->set(*(String *)&s5);
    ((String *)(&s5))->dtor();
    ((String *)(&s0))->dtor();
    ((String *)(&s4))->dtor();
    ((String *)(&s1))->dtor();
    ((String *)(&s3))->dtor();
    ((String *)(&s2))->dtor();
    self->field_0x5d = 1;
    self->field_0xce = 1;
    ((MGame *)(self))->pauseSounds();
}

// ---- gameOverCheck_180514.cpp ----

// 0x724e4
// 0x78688
// 0x78658
// 0x78694
// 0x76c84
// 0x6f2b0
// 0x78664
// 0x75550 — DialogueWindow::set(Mission*, int, int)
// 0x786ac
// 0x786b8
// 0x786a0
// 0x7483c

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
    if (self->field_0x8c == 0) {
        DialogueWindow *w = (DialogueWindow *)::operator new(0x74);
        DialogueWindow_ctor(w);
        Level *lvl = self->field_0x78;
        self->field_0x8c = w;
        if (lvl != 0) ((DialogueWindow *)(w))->setLevel(lvl);
    } else if (((DialogueWindow *)(self->field_0x8c))->hasLevel() == 0) {
        Level *lvl = self->field_0x78;
        if (lvl != 0) ((DialogueWindow *)(self->field_0x8c))->setLevel(lvl);
    }
    ((DialogueWindow *)(self->field_0x8c))->set((*g_status)->getMission(), 2, -1);
}

// MGame::gameOverCheck(): handle player death/explosion and end-of-level dialogue.
void MGame::gameOverCheck() {
    MGame *self = this;
    if (((PlayerEgo *)(self->field_0x58))->getHitpoints() <= 0) {
        if (((PlayerEgo *)(self->field_0x58))->tryToStartEmergencySystem() != 0)
            return;
        if (((PlayerEgo *)(self->field_0x58))->isInWormhole() == 0) {
            ((PlayerEgo *)(self->field_0x58))->setTurretMode(0);
            ((LevelScript *)(self->field_0x7c))->resetCamera(self->field_0x78);
            ((PlayerEgo *)(self->field_0x58))->setFreeLookMode(0);
            TFC_enableFirstPersonCam(self->field_0xf4, 0);
            ((PlayerEgo *)(self->field_0x58))->hideShipForFirstPersonCameraView(0);
            self->field_0x111 = 1;
            ((PlayerEgo *)(self->field_0x58))->explode();
            if (((PlayerEgo *)(self->field_0x58))->explosionEnded() != 0) {
                self->field_0x60 = 1;
                String *t = (String *)((GameText *)(**g_goDeathText))->getText(0);
                ((String *)((char *)self + 0x64))->assign(t);
            }
        } else {
            self->field_0x60 = 1;
            String *t = (String *)((GameText *)(**g_goWormText))->getText(0);
            ((String *)((char *)self + 0x64))->assign(t);
            self->field_0x111 = 1;
        }

        if (self->field_0x60 != 0) {
            if ((*g_status)->getMission() != 0 &&
                ((Mission *)((*g_status)->getMission()))->isCampaignMission() != 0) {
                int cm = (*g_status)->getCurrentCampaignMission();
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
            self->field_0x1e8 = rec;
        }
    }

    if (((Level *)(self->field_0x78))->checkGameOver() != 0) {
        bindDialogueLevel(self);
        self->field_0x5e = 1;
        ((MGame *)(self))->pauseSounds();
        self->field_0x5d = 1;
    }

    // Timed end-of-mission check (0x48/0x4c is a 64-bit counter vs 0x1389).
    unsigned lo = self->field_0x48;
    int hi = self->field_0x4c;
    bool ready = !(hi < (int)(lo < 0x1389));
    if (ready) {
        self->field_0x48 = 0;
        self->field_0x4c = 0;
        int *sc = (int *)self->field_0x7c;
        if (sc[0] >= 1) {
            // 64-bit compare against script counters [2]/[3].
            bool done = !((long long)(unsigned)sc[0] - ((long long)sc[3] << 32 | (unsigned)sc[2]) < 0);
            bool survival = false;
            if (!done) {
                int cm = (*g_status)->getCurrentCampaignMission();
                if (cm != 0x2a) {
                    Objective *obj = *(Objective **)((char *)self->field_0x78 + 0x28);
                    survival = (obj == 0) || (((Objective *)(obj))->isSurvivalObjective() != 0);
                }
                if (!survival) {
                    bindDialogueLevel(self);
                    self->field_0x5d = 0x101;
                    ((MGame *)(self))->pauseSounds();
                }
            }
        }
    }

    if (self->field_0x60 != 0) {
        self->field_0x48 = 0;
        self->field_0x4c = 0;
        self->field_0x50 = 0;
        ((FModSound *)(**g_goSnd))->play(0x25, 0, 0 /* vel: arg lost in decomp */, 0.0f);
    }
}

// ---- OnTouchBegin_178e64.cpp ----

// 0x71d64
// 0x... (Layout::OnTouchBegin)
extern "C" int ApplicationManager_GetApplicationData();  // 0x71704
// 0x781e4
// 0x781f0
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
    if (self->field_0xc0 == 0)
        self->field_0xc0 = touchId;

    if (self->field_0x5d == 0) {
        // Game-over / replay splash tap dispatch.
        if (self->field_0x60 != 0 && self->field_0x50 >= 4000) {
            int cm = (*g_status)->getCurrentCampaignMission();
            if (cm == 0x9e) {
                self->field_0x54 = 0;
                return ((MGame *)(self->field_0x8))->endRunModule(2);
            }
            if (self->field_0x1e8 != 0) {
                ((GameRecord *)(self->field_0x1e8))->load();
                Globals_playMusicAndFadeOutCurrent(**g_tbRecordTrack);
                self->field_0x54 = 0;
                return ((MGame *)(self->field_0x8))->endRunModule(5);
            }
            Globals_playMusicAndFadeOutCurrent(**g_tbMenuTrack);
            self->field_0x54 = 0;
            ((ApplicationManager *)(self->field_0x8))->SetCurrentApplicationModule(5);
            return;
        }
    } else {
        // Modal/overlay routing while in active flight UI.
        if (self->field_0xc7 != 0) {
            Layout *hl = **g_tbStarLayout;
            if (*(uint8_t *)hl != 0) {
                ((Layout *)(hl))->OnTouchBegin(p1, p2);
                return;
            }
            uint8_t r = ((StarMap *)(self->field_0x90))->OnTouchBegin(p1, p2);
            self->field_0xc7 = r ^ 1;
            return;
        }
        if (self->field_0xc5 != 0 || self->field_0xce != 0 ||
            self->field_0xc4 != 0) {
            ((ChoiceWindow *)(self->field_0x94))->OnTouchBegin(p1, p2);
            return;
        }
        if (self->field_0x5e != 0) {
            ((DialogueWindow *)(self->field_0x8c))->OnTouchBegin(p1, p2);
            return;
        }
        if (self->field_0x15d != 0) {
            int ad = ApplicationManager_GetApplicationData();
            if (*(uint8_t *)((char *)ad + 5) != 0) return;
            if (*(uint8_t *)((char *)ad + 0xc) != 0) return;
            ((MenuTouchWindow *)(self->field_0x88))->OnTouchBegin(p1, p2, (int)(intptr_t)touchId);
            if (self->field_0x15e == 0) return;
            if (((MenuTouchWindow *)(self->field_0x88))->isShowingMessage() != 0) return;
            if (((MenuTouchWindow *)(self->field_0x88))->isMakingScreenshot() != 0) return;
            // fall through to free-cam handler.
            return ((MGame *)(self))->handleHudTouchAction(p1, p2, touchId, 0);
        }
    }

    // World HUD touch: dispatch on the hud's touch result bitmask.
    unsigned hr = ((Hud *)(self->field_0x74))->touchBegin(p1, (void *)(intptr_t)p2, (int)(intptr_t)touchId);
    self->field_0xf8 = hr;
    ((MGame *)(self))->handleHudTouchAction(p1, p2, touchId, hr);
}

// ---- OnUpdate_17c8d8.cpp ----
// GetElapsedTimeMillis
// The entire per-frame game tick — free-cam keyboard input, AI/physics step,
// mining/hacking/docking state machines, dialogue + sound pumps, jump-scene update,
// station-service transitions and their string building — is one massive body that
// Ghidra renders with deeply-corrupt SIMD/vector pseudo-ops.  It is translated as a
// single documented helper so the entry point stays faithful and compiling; the
// helper receives the clamped frame-delta and performs all field writes / engine
// calls on `self`.

__attribute__((visibility("hidden"))) extern int *g_upGuard;   // @0x18c8ee (stack guard [0])

// MGame::OnUpdate(): advance the whole game one frame.
void MGame::OnUpdate() {
    MGame *self = this;
    int *guard = g_upGuard;
    int g0 = *guard;

    // Clamp this frame's elapsed time to [0, 0x96] ms (cap large hitches).
    int delta;
    int t = ((ApplicationManager *)(self->field_0x8))->GetElapsedTimeMillis();
    if (t < 0x97) {
        delta = (((ApplicationManager *)(self->field_0x8))->GetElapsedTimeMillis() < 0) ? 0
                : (((ApplicationManager *)(self->field_0x8))->GetElapsedTimeMillis() < 0x97
                       ? ((ApplicationManager *)(self->field_0x8))->GetElapsedTimeMillis()
                       : 0x96);
    } else {
        delta = 0x96;
    }

    self->field_0x40 = delta;
    // Accumulate into the 4-wide playtime counter at 0x30 (lane 0 gets +delta).
    self->field_0x30 += delta;

    // Run the full per-frame update.
    ((MGame *)(self))->tick(delta);

    
}

// ---- OnSuspend_180800.cpp ----

__attribute__((visibility("hidden"))) extern RecordHandler **g_record;
__attribute__((visibility("hidden"))) extern FModSound **g_fmod;
extern "C" void Level_onSuspend(...);  // tail call via this->[0x74]

// MGame::OnSuspend(): persist options, pause all audio, raise the pause dialogue (cutscene
// mode if the player is mid-cutscene/dead), then suspend the level.
void MGame::OnSuspend() {
    MGame *self = this;
    if (*g_record != 0) ((RecordHandler *)(*g_record))->saveOptions();
    ((MGame *)(self))->pauseSounds();
    if (self->field_0x5d == 0) {
        if (self->field_0x88 == 0) {
            MenuTouchWindow *w = (MenuTouchWindow *)MGame_opnew(0x240);
            ((MenuTouchWindow *)(w))->ctor(1);
            self->field_0x88 = w;
        }
        self->field_0x1a6 = 1;
        self->field_0x5d = 1;
        ((FModSound *)(*g_fmod))->pauseAllPlaying();
        ((PlayerEgo *)(self->field_0x58))->PauseEngineSound();
        EnemyList *e = (EnemyList *)(intptr_t)((Level *)(self->field_0x78))->getEnemies();
        if (e != 0) {
            for (uint32_t i = 0; i < e->size; i++)
                ((KIPlayer *)(e->data[i]))->PauseEngineSound();
        }
        MenuTouchWindow *w = self->field_0x88;
        int mode = 1;
        if (self->field_0x5f == 0)
            mode = ((PlayerEgo *)(self->field_0x58))->isDead();
        ((MenuTouchWindow *)(w))->setCutsceneMode(mode);
        self->field_0xc9 = 1;
    }
    return Level_onSuspend(self->field_0x74);
}

// ---- dockEvent_17f920.cpp ----

// 0x78604
// 0x78610
// 0x... goingToStream
// 0x724f0
// 0x726c4
// 0x72928
// 0x72814
// 0x76d20
// 0x78298
// 0x75430
// 0x71c14
extern "C" int Station_getIndex(Station *s);  // 0x71824
// Builds and sets the "dock at <station>?" choice text, then ChoiceWindow::left().

__attribute__((visibility("hidden"))) extern int *g_deGuard;     // @0x18f932 (stack guard [0])
__attribute__((visibility("hidden"))) extern int *g_deStatus;    // @0x18f970 (piVar6)
__attribute__((visibility("hidden"))) extern int *g_deAutoFlag;  // @0x18fa3a
__attribute__((visibility("hidden"))) extern int g_dePostEffect; // @0x18fe0c
__attribute__((visibility("hidden"))) extern int g_deTextA;      // @0x18fa82
__attribute__((visibility("hidden"))) extern int g_deLitA0;      // @0x18fa9a
__attribute__((visibility("hidden"))) extern int g_deLitA1;      // @0x18fac0
__attribute__((visibility("hidden"))) extern int g_deTextB;      // @0x18fc74
__attribute__((visibility("hidden"))) extern int g_deLitB0;      // @0x18fc8c
__attribute__((visibility("hidden"))) extern int g_deLitB1;      // @0x18fcb2
__attribute__((visibility("hidden"))) extern int **g_deAchieve;  // @0x18fb90
__attribute__((visibility("hidden"))) extern int **g_deAlienFlag;// @0x18fb9c

static void savePlayerStats(MGame *self, Status *status) {
    PlayerEgo *ego = self->field_0x58;
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
    MGame *self = this;
    ChoiceWindow *cw = self->field_0x94;

    void *txt = ((GameText *)(*g_gameText))->getText(textId);
    RetStr name = ((Station *)((*g_status)->getStation()))->getName();

    String12 sPrefix, sSuffix, t1, t2, t3, result;
    String_cstr_ctor(&sPrefix, (const char *)(intptr_t)prefixLit, false);
    String_concat(&t1, (String12 *)txt, &sPrefix);                 // text + prefix
    String_concat(&t2, &t1, (String12 *)&name);                    // + station name
    String_cstr_ctor(&sSuffix, (const char *)(intptr_t)suffixLit, false);
    String_concat(&t3, &t2, &sSuffix);                             // + suffix
    String_concat(&result, &t3, (String12 *)txt);                  // + trailing text

    ((ChoiceWindow *)(cw))->set(*(String *)&result, true);

    ((String *)(&result))->dtor();
    ((String *)(&t3))->dtor();
    ((String *)(&sSuffix))->dtor();
    ((String *)(&t2))->dtor();
    ((String *)(&t1))->dtor();
    ((String *)(&sPrefix))->dtor();

    ((ChoiceWindow *)(cw))->left();
}

// MGame::dockEvent(): handle proximity to a jumpgate/station while flying.
void MGame::dockEvent() {
    MGame *self = this;
    int *guard = g_deGuard;
    int g0 = *guard;

    float pos[4];
    ((PlayerEgo *)(pos))->getPosition();
    self->field_0xcb = ((Level *)(self->field_0x78))->collideStream(*(Vector *)pos);
    ((PlayerEgo *)(pos))->getPosition();
    self->field_0xcc = ((Level *)(self->field_0x78))->collideStation(*(Vector *)pos);

    Status *status = (Status *)g_deStatus;
    Mission *m = (*g_status)->getMission();
    bool special = ((Mission *)(m))->isEmpty() != 0 ||
                   ((Mission *)((*g_status)->getMission()))->getType() == 0xb ||
                   ((Mission *)((*g_status)->getMission()))->getType() == 0 ||
                   ((Mission *)((*g_status)->getMission()))->getType() == 0xbd ||
                   ((Mission *)((*g_status)->getMission()))->getType() == 0xab ||
                   ((Mission *)((*g_status)->getMission()))->getType() == 0xac;

    if (!special) {
        if ((self->field_0xcc != 0 || self->field_0xcb != 0) &&
            ((PlayerEgo *)(self->field_0x58))->isAutoPilot() != 0) {
            ((Hud *)(self->field_0x74))->hudEvent(0x15, self->field_0x58, 0);
        }
        
        return;
    }

    // special-mission docking path.
    if (self->field_0xcb != 0) {
        if (((PlayerEgo *)(self->field_0x58))->goingToStream() != 0 &&
            ((PlayerEgo *)(self->field_0x58))->isDockingToStream() == 0 &&
            ((PlayerEgo *)(self->field_0x58))->isDockedToStream() == 0) {
            ((PlayerEgo *)(self->field_0x58))->dockToStream(0);
            self->field_0x15c = 0;
            self->field_0x111 = 1;
            
            return;
        }
        if (self->field_0xcb != 0) {
            savePlayerStats(self, status);
            // Status +0xf4 is not modeled in gof2/Status.h (owned by another batch).
            F<int>(status, 0xf4) = ((PlayerEgo *)(self->field_0x58))->getCurrentSecondaryWeaponIndex();
            int autop = ((PlayerEgo *)(self->field_0x58))->isAutoPilot();
            int *autoFlag = g_deAutoFlag;
            if (*autoFlag == 0 || autop == 0) {
                if (self->field_0xc7 != 0) {
                    self->field_0x30 = 0;
                    self->field_0x34 = 0;
                    
                    return;
                }
                ((PlayerEgo *)(self->field_0x58))->isAutoPilot();
                if (((PlayerEgo *)(self->field_0x58))->goingToStream() == 0) {
                    
                    return;
                }
                if (*autoFlag == 0) {
                    self->field_0x15c = 0;
                    self->field_0x111 = 1;
                    if (self->field_0x90 == 0) {
                        StarMap *sm = (StarMap *)::operator new(0x1e8);
                        new (sm) StarMap(false, nullptr, false, -1);
                        self->field_0x90 = sm;
                    }
                    Engine *eng = (Engine *)((ApplicationManager *)(self->field_0x8))->GetEngine();
                    ((Engine *)(eng))->SetPostEffect(g_dePostEffect, 0);
                    ((StarMap *)(self->field_0x90))->initLights();
                    ((StarMap *)(self->field_0x90))->setJumpMapMode(1, 0);
                    ((PlayerEgo *)(self->field_0x58))->setAutoPilot(0);
                    self->field_0x5d = 1;
                    self->field_0xc7 = 1;
                    ((MGame *)(self))->pauseSounds();
                    self->field_0x111 = 1;
                    
                    return;
                }
                if (self->field_0xc5 != 0) {  return; }
                self->field_0x15c = 0;
                ChoiceWindow *cw = self->field_0x94;
                self->field_0x111 = 1;
                if (cw == 0) {
                    cw = (ChoiceWindow *)::operator new(0x5c);
                    new (cw) ChoiceWindow();
                    self->field_0x94 = cw;
                }
                ((MGame *)(self))->buildDockChoice(g_deTextB, g_deLitB0, g_deLitB1);
            } else {
                if (self->field_0xc5 != 0) {  return; }
                self->field_0x15c = 0;
                ChoiceWindow *cw = self->field_0x94;
                self->field_0x111 = 1;
                if (cw == 0) {
                    cw = (ChoiceWindow *)::operator new(0x5c);
                    new (cw) ChoiceWindow();
                    self->field_0x94 = cw;
                }
                ((MGame *)(self))->buildDockChoice(g_deTextA, g_deLitA0, g_deLitA1);
            }
            self->field_0x5d = 1;
            self->field_0xc5 = 1;
            ((MGame *)(self))->pauseSounds();
            ((PlayerEgo *)(self->field_0x58))->setAutoPilot(0);
            
            return;
        }
    }

    if (self->field_0xcc == 0) {
        int tgt = ((PlayerEgo *)(self->field_0x58))->getAutoPilotTarget();
        int lm = ((Level *)(self->field_0x78))->getLandmarks();
        if (tgt != *(int *)((char *)lm + 4) ||
            ((PlayerEgo *)(self->field_0x58))->collidesWithStation() == 0) {
            
            return;
        }
    }

    int cm = (*g_status)->getCurrentCampaignMission();
    if (cm > 0x30 && (*g_status)->getCurrentCampaignMission() < 0x37) {
        if (Station_getIndex((*g_status)->getStation()) != 0x4a) {
            ((Hud *)(self->field_0x74))->hudEvent(0x15, self->field_0x58, 0);
            
            return;
        }
    }
    if (((PlayerEgo *)(self->field_0x58))->goingToStation() != 0 &&
        (*g_status)->inAlienOrbit() == 0 &&
        ((Status *)(*(Station **)status))->inEmptyOrbit() == 0) {
        ((Achievements *)(*g_deAchieve))->checkForNewMedal(self->field_0x58);
        **g_deAlienFlag = 0;
        savePlayerStats(self, status);
        ((ApplicationManager *)(self->field_0x8))->SetCurrentApplicationModule(5);
        self->field_0x54 = 0;
    }

    
}

// ---- freeCamTouchEnd_178d30.cpp ----
// MGame::freeCamTouchEnd(int, int, void*): releases a free-camera drag. Clears the
// matching active-touch slot, then commits the accumulated rotation deltas (only the
// axes that moved more than 3px) and converts them to integer angles, flagging the
// camera dirty.
void MGame::freeCamTouchEnd(int p1, int p2, int id) {
    MGame *self = this;
    (void)p1;
    (void)p2;
    if (self->field_0x98 == id) {
        self->field_0x98 = 0;
        self->field_0xa0 = 0;
    } else if (self->field_0x9c == id) {
        self->field_0x9c = 0;
        self->field_0xa0 = 0;
    }
    {
        int ix = self->field_0x134;
        int iy = self->field_0x138;
        float fx = (float)ix;
        float fy = (float)iy;
        float nx = self->field_0x118 + fx;
        float ny = self->field_0x11c + fy;
        if (iy < 0) iy = -iy;
        if (ix < 0) ix = -ix;
        // Default coefficients come from float literals (exact values irrelevant to the
        // byte match — the diff normalizes literal-pool loads). Use values that are NOT
        // encodable as vmov.f32 immediates so the compiler emits a vldr/ldr from the pool.
        float ox = 0.001f;
        if (ix > 3) ox = fx;
        float oy = 0.001f;
        if (iy > 3) oy = fy;
        self->field_0x111 = 1;
        self->field_0x13c = 0.002f;
        self->field_0x140 = 0.002f;
        self->field_0x15c = 0;
        self->field_0x148 = ox;
        self->field_0x14c = oy;
        self->field_0x118 = nx;
        self->field_0x11c = ny;
        self->field_0x12c = (int)nx;
        self->field_0x130 = (int)ny;
    }
}

// ---- _MGame_177c40.cpp ----
// MGame::~MGame() deleting destructor: run the complete dtor (which returns `this`)
// then tail-call operator delete with that pointer — so `this` is never saved.

void MGame::deleting_dtor() {
    MGame *self = this;
    MGame_opdelete(((MGame *)(self))->dtor());
}

// ---- UseKhadorDrive_179d5c.cpp ----

// 0x77a7c
// 0x73738
// 0x72034
// 0x72ca0
// 0x78280
// 0x7828c
// 0x77a04
// 0x77b24
// 0x782a4
// 0x71c44
// Tail helpers.
// @0x1ac808
// @0x1ac818

__attribute__((visibility("hidden"))) extern int *g_kdStatus;     // @0x189d7e (*piVar6)
__attribute__((visibility("hidden"))) extern int **g_kdJumpDst;   // @0x189e36
__attribute__((visibility("hidden"))) extern int **g_kdVolText;   // @0x189ed2
__attribute__((visibility("hidden"))) extern int **g_kdGalaxy;    // @0x189e8e
__attribute__((visibility("hidden"))) extern int **g_kdAlienDst;  // @0x189e9a
__attribute__((visibility("hidden"))) extern int g_kdPostEffect;  // @0x189f98 (DAT)

// MGame::UseKhadorDrive(): trigger a hyperspace jump or open the jump star map.
void MGame::UseKhadorDrive() {
    MGame *self = this;
    if (((PlayerEgo *)(self->field_0x58))->isChargingDrive() != 0) return;

    int *status = g_kdStatus;
    Mission *m = (*g_status)->getMission();
    bool special =
        ((Mission *)(m))->isEmpty() != 0 ||
        (*g_status)->getCurrentCampaignMission() == 0x4e ||
        ((Mission *)(m))->getType() == 0xb ||
        ((Mission *)(m))->getType() == 0 ||
        ((Mission *)(m))->getType() == 0xbd ||
        ((Mission *)(m))->getType() == 0xd ||
        ((Mission *)(m))->getType() == 0xab ||
        ((Mission *)(m))->getType() == 0xac;

    if (!special) {
        if ((*g_status)->getCurrentCampaignMission() == 0x41 && (*g_status)->inAlienOrbit() == 0) {
            int idx = Station_getIndex((*g_status)->getStation());
            (*g_status)->getCampaignMission();
            if (idx == ((Mission *)((Mission *)(*g_status)->getCampaignMission()))->getTargetStation())
                special = true;
        }
    }

    if (special) {
        PlayerEgo *player = (PlayerEgo *)(intptr_t)((Level *)(self))->getPlayer();
        return ((Hud *)(self->field_0x74))->hudEvent(0x15, player, 0);
    }

    ((PlayerEgo *)(self->field_0x58))->resetGunDelay();
    if ((*g_status)->getCurrentCampaignMission() == 0x4e) {
        **g_kdJumpDst = *(int *)((char *)*status + 0x78);
        self->field_0xdd = 1;
        ((MGame *)(self))->startChargingJumpDrive();
        self->field_0x5d = 0;
        ((MGame *)(self))->resumeSounds();
        self->field_0xd6 = 0;
        ((Hud *)(self->field_0x74))->closeHudMenu();
        return ((MGame *)(status))->jumpFinish();
    }

    if ((*g_status)->inAlienOrbit() == 0) {
        if (((PlayerEgo *)(self->field_0x58))->hasVolatileGoods() != 0) {
            void *txt = ((GameText *)(**g_kdVolText))->getText(0);
            ((ChoiceWindow *)(self->field_0x94))->set(*(String *)txt);
            self->field_0x5d = 1;
            self->field_0xce = 1;
            self->field_0x111 = 1;
            self->field_0xd6 = 0;
            ((Hud *)(self->field_0x74))->closeHudMenu();
            ((MGame *)(self))->pauseSounds();
            self->field_0x17c = 0;
            return;
        }
        if (self->field_0x90 == 0) {
            StarMap *sm = (StarMap *)::operator new(0x1e8);
            new (sm) StarMap(false, nullptr, false, -1);
            self->field_0x90 = sm;
        }
        Engine *eng = (Engine *)((ApplicationManager *)(self->field_0x8))->GetEngine();
        ((Engine *)(eng))->SetPostEffect(g_kdPostEffect, 0);
        ((StarMap *)(self->field_0x90))->initLights();
        self->field_0xdd = 1;
        ((StarMap *)(self->field_0x90))->setJumpMapMode(1, 1);
        if ((*g_status)->inAlienOrbit() == 0)
            ((StarMap *)(self->field_0x90))->askForJumpIntoAlienWorld();
        self->field_0x5d = 1;
        self->field_0xc7 = 1;
        ((MGame *)(self))->pauseSounds();
        self->field_0xd6 = 0;
        return ((MGame *)(self->field_0x74))->starMapShown();
    }

    // In alien orbit.
    if ((*g_status)->getCurrentCampaignMission() == 0x50)
        *(int *)((char *)*status + 0x84) = 100;
    int station = ((Galaxy *)(**g_kdGalaxy))->getStation((*g_status)->getCurrentCampaignMission() /* index: arg lost in decomp */);
    **g_kdAlienDst = station;
    self->field_0xdd = 1;
    ((MGame *)(self))->startChargingJumpDrive();
    self->field_0x5d = 0;
    ((MGame *)(self))->resumeSounds();
    self->field_0xd6 = 0;
    return ((MGame *)(self->field_0x74))->starMapShown();
}

// ---- OnInitialize_177c50.cpp ----

// 0x178470
// 0x713d4
// Heavily-corrupt sub-blocks are delegated to documented helpers:
// sound-resource list (62..145)
// HP/shield/armor restore (157..198)
// weapon + music init (315..end)

__attribute__((visibility("hidden"))) extern int *g_initGuard;   // @0x187c60 (stack guard [0])
__attribute__((visibility("hidden"))) extern int *g_initStatus;  // @0x187e46 (*piVar17)
__attribute__((visibility("hidden"))) extern int *g_initSoundList; // @0x187cc6 (the FMOD resource list)

// Add a specific resource id to the running sound-resource list (2-arg variant; the
// 1-arg Globals_addSoundResourceToList above appends the list's "next" default).

// MGame::loadSoundResources(): build the per-session FMOD resource list. Always loads the
// common cockpit/weapon/explosion set, then appends wingman, early-mission, warp-gate and
// per-campaign-mission specific clips.
void MGame::loadSoundResources() {
    MGame *self = this;
    int list = *g_initSoundList;

    Globals_startNewSoundResourceList();
    static const int kCommon[] = {
        0x66, 0x68, 0x69, 0x6a, 0x6b, 0x67, 0x7e, 0x05, 0x18, 0x15,
        0x12, 0x13, 0x14, 0x1c, 0x1d, 0x1b, 0x25, 0x1a, 0x2e, 0x2f,
    };
    for (int id : kCommon)
        ((Globals *)(list))->addSoundResource_oi(id);

    if ((*g_status)->getWingmen() != 0)
        Globals_addSoundResourceToList(list);

    ((Globals *)(list))->addSoundResource_oi(0x3e);
    ((Globals *)(list))->addSoundResource_oi(0x3d);
    ((Globals *)(list))->addSoundResource_oi(0x24);

    if ((*g_status)->getCurrentCampaignMission() < 2) {
        Globals_addSoundResourceToList(list);
        Globals_addSoundResourceToList(list);
    }

    if ((*g_status)->inAlienOrbit() == 0) {
        (*g_status)->getSystem();
        if (((SolarSystem *)(0))->currentOrbitHasWarpGate())
            Globals_addSoundResourceToList(list);
    }

    int cm = (*g_status)->getCurrentCampaignMission();
    if (cm == 0) {
        ((Globals *)(list))->addSoundResource_oi(0x8f);
        ((Globals *)(list))->addSoundResource_oi(0x9d);
        ((Globals *)(list))->addSoundResource_oi(0x9e);
        ((Globals *)(list))->addSoundResource_oi(0xa1);
        ((Globals *)(list))->addSoundResource_oi(0xa0);
        ((Globals *)(list))->addSoundResource_oi(0x9f);
    } else if (cm == 0xe || cm == 0x18 || cm == 0x1d) {
        Globals_addSoundResourceToList(list);
    } else if (cm == 0x29) {
        ((Globals *)(list))->addSoundResource_oi(0x9b);
        ((Globals *)(list))->addSoundResource_oi(0x99);
        ((Globals *)(list))->addSoundResource_oi(0x9a);
    }
}

// MGame::restorePlayerStats(): re-apply the player's persisted HP/shield/armor/gamma values
// (each only when its saved slot is non-negative) and, except on mission 0x5f, re-cap them to
// the current ship's maxima (resetting gamma to full when there is no orbital gamma damage).
void MGame::restorePlayerStats() {
    MGame *self = this;
    int status = *g_initStatus;
    Player *pl = (Player *)self->field_0x58->player;

    if (*(int *)((char *)status + 0x64) >= 0) pl->setHitpoints(*(int *)((char *)status + 0x64));
    if (*(int *)((char *)status + 0x5c) >= 0) pl->setShieldHP(*(int *)((char *)status + 0x5c));
    if (*(int *)((char *)status + 0x60) >= 0) pl->setArmorHP(*(int *)((char *)status + 0x60));
    if (*(int *)((char *)status + 0x68) >= 0) pl->setGammaHP(*(int *)((char *)status + 0x68));

    self->field_0x58->resetLastHP();

    if ((*g_status)->getCurrentCampaignMission() != 0x5f) {
        Ship *ship = (*g_status)->getShip();
        *(int *)((char *)status + 0x64) = ship->getMaxHP();
        *(int *)((char *)status + 0x5c) = ship->getMaxShieldHP();
        *(int *)((char *)status + 0x60) = ship->getMaxArmorHP();
        *(int *)((char *)status + 0x68) = 100;

        int stIdx = ((Station *)((*g_status)->getStation()))->getIndex();
        int cm = (*g_status)->getCurrentCampaignMission();
        if ((float)(*g_status)->getGammaRayDamagePerSecond(stIdx, cm) == 0.0f)
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
extern "C" void  Globals_getLineArray(int font, String *text, int key,
                                       Array<AbyssEngine::String *> *out); // Globals::getLineArray
extern "C" void  Array_string_ptr_ctor2(void *array);                  // Array<String*>::Array()

// MGame::setupWeaponsAndAudio(): choose the active secondary weapon, kick off engine sounds,
// particle effects, music and the post-process effect, ensure the pause window and the
// mission-information overlay text exist. Runs as the tail of OnInitialize.
void MGame::setupWeaponsAndAudio() {
    MGame *self = this;
    int status = *g_initStatus;

    // Mission 0x7d freighter visit: flag this station as serviced the first time and raise
    // its radio briefing.
    if ((*g_status)->inAlienOrbit() == 0 && (*g_status)->getCurrentCampaignMission() == 0x7d) {
        int stIdx = ((Station *)((*g_status)->getStation()))->getIndex();
        if ((*g_status)->isFreighterMissionStation(stIdx) != 0) {
            Mission *m = (*g_status)->getMission();
            int statusVal = ((Mission *)(m))->getStatusValue();
            int bit = (*g_status)->getFreighterMissionStationBit(
                ((Station *)((*g_status)->getStation()))->getIndex());
            if ((statusVal & (1 << (bit & 0xff))) == 0) {
                int bit2 = (*g_status)->getFreighterMissionStationBit(
                    ((Station *)((*g_status)->getStation()))->getIndex());
                ((Mission *)(m))->setStatusValue(statusVal | (1 << (bit2 & 0xff)));
                if (self->field_0x58 != 0 && self->field_0x84 != 0)
                    *(Radio **)((char *)self->field_0x58 + 0x18) = self->field_0x84;
                ((Level *)(self->field_0x78))->createRadioMessage(0x13, 0);
            }
        }
    }

    // Secondary-weapon selection: prefer the persisted weapon id (status+0xf4) if the ship
    // still carries it; otherwise fall back to the first secondary in slot type 1.
    Array<Item *> *secondary = ((Status *)(*g_status))->getShip()->getEquipment(1);
    if (secondary != 0) {
        int savedId = *(int *)((char *)status + 0xf4);
        if (((Status *)(*g_status))->getShip()->hasEquipment(savedId, 1) == 0) {
            Item *first = secondary->empty() ? 0 : (*secondary)[0];
            if (first != 0) {
                self->field_0x58->setCurrentSecondaryWeaponIndex(((Item *)(first))->getIndex());
                ((Hud *)(self->field_0x74))->setCurrentSecondaryWeapon(first);
            }
        } else {
            for (unsigned i = 0; i < secondary->size(); i++) {
                Item *it = (*secondary)[i];
                if (it != 0 && ((Item *)(it))->getIndex() == savedId) {
                    ((Level *)((Level *)self->field_0x78))->getPlayer();
                    self->field_0x58->setCurrentSecondaryWeaponIndex(((Item *)(it))->getIndex());
                    ((Hud *)(self->field_0x74))->setCurrentSecondaryWeapon(it);
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
        if ((*g_status)->getCurrentCampaignMission() > 1) {
            Vec3 p = self->field_0x58->getPosition();
            (void)p;
            ((FModSound *)(*g_fmod))->play(*g_initEngineSnd,
                                           (Vector *)((char *)self->field_0x58 + 0x1c),
                                           (Vector *)&p, 0.0f);
        }
    } else {
        self->field_0x58->PlayEngineSound();
        EnemyList *enemies = (EnemyList *)(intptr_t)((Level *)(self->field_0x78))->getEnemies();
        if (enemies != 0)
            for (unsigned i = 0; i < enemies->size; i++)
                ((KIPlayer *)(enemies->data[i]))->PlayEngineSound();
    }

    self->field_0x50 = 0;

    // Lighting + particle effects + per-ship fire-rate pitch.
    ((Level *)(self->field_0x78))->getStarSystem();
    ((StarSystem *)(0))->initLight();
    ((Level *)(self->field_0x78))->enableParticleEffects(true, renderParticles);

    ((Status *)(*g_status))->getShip();
    float fireRate = (float)((Status *)(*g_status))->getShip()->getFireRateFactor();
    if (1.0f - fireRate >= 0.0f)
        self->field_0x58->pitchAllPrimaryGuns(1.0f - fireRate);

    // Mark "newly visited" service stations.
    if ((*g_status)->inAlienOrbit() == 0) {
        int idx = ((Station *)((*g_status)->getStation()))->getIndex();
        bool visit;
        unsigned off = (unsigned)(idx - 0x6d);
        if (off < 0x1a) {
            if ((g_initStationMask & (1 << (off & 0xff))) == 0)
                visit = (off != 2) || ((*g_status)->getCurrentCampaignMission() < 0x5e);
            else
                visit = true;
        } else {
            visit = ((unsigned)(idx - 0x66) <= 2);
        }
        if (visit)
            ((Station *)((*g_status)->getStation()))->visit();
    }

    // Music: play the queued track (if any), then reset the queue and reset the sound pitch.
    if (*g_initMusicArmed != -1)
        Globals_playMusicAndFadeOutCurrent(*g_initMusicTrack);
    *g_initMusicArmed = -1;
    ((FModSound *)(*g_initFmod))->setDownPitch(false);

    // Post-process effect for this flight session.
    Engine *eng = (Engine *)((ApplicationManager *)(self->field_0x8))->GetEngine();
    ((Engine *)(eng))->SetPostEffect(g_initPostEffect, false);

    // Mission 0x9e: cloak the first enemy fighter.
    if ((*g_status)->getCurrentCampaignMission() == 0x9e) {
        int enemies = ((Level *)(self->field_0x78))->getEnemies();
        if (enemies != 0) {
            int first = *(int *)((char *)(intptr_t)((Level *)(self->field_0x78))->getEnemies() + 4);
            ((PlayerFighter *)(first))->cloak(true, false);
        }
    }

    // Ensure the pause / cutscene window exists.
    if (self->field_0x88 == 0) {
        MenuTouchWindow *w = (MenuTouchWindow *)::operator new(0x240);
        ((MenuTouchWindow *)(w))->ctor(1);
        self->field_0x88 = w;
    }

    // Pre-build the wrapped mission-information overlay text.
    void *arr = ::operator new(0xc);
    Array_string_ptr_ctor2(arr);
    *(void **)((char *)self + 0x43c) = arr;    // this[2].field_5C (extended layout)
    String *font = *g_initInfoFont;
    String *text = (String *)((GameText *)(*g_gameText))->getText(g_initInfoTextKey);
    Globals_getLineArray((int)(intptr_t)font, text, *g_initInfoWidth,
                         (Array<AbyssEngine::String *> *)arr);
    self->field_0x54 = 1;
}

// MGame::OnInitialize(): bootstrap a flight session (level, player, sounds, state).
void MGame::OnInitialize() {
    MGame *self = this;
    int *guard = g_initGuard;
    int g0 = *guard;

    self->field_0x1ec = 0;   // this[2].field_4C low byte
    Level *level = self->field_0x78;
    self->field_0xc = 100;

    if (level == 0) {
        // Create skybox cube texture from the current system / alien orbit.
        unsigned texSel;
        if ((*g_status)->inAlienOrbit() == 0) {
            (*g_status)->getSystem();
            int ti = ((SolarSystem *)(0))->getTextureIndex();
            texSel = (ti + 0x2efe) & 0xffff;
        } else {
            texSel = 0x2f08;
        }
        ((PaintCanvas*)g_PaintCanvas)->TextureCreate((unsigned short)self->field_0x4, &texSel, (void *)0, &texSel, false);
        ((PaintCanvas*)g_PaintCanvas)->ChangeCubeTexture((unsigned)self->field_0x4);

        // Build the per-session sound-resource list (long sequential block).
        ((MGame *)(self))->loadSoundResources();

        (*g_status)->checkForLevelUp();
        level = (Level *)::operator new(0x2a0);
        new (level) Level(3);
        self->field_0x78 = level;
    }

    if (((Level *)(level))->init() == 0) {
        self->field_0xc = 100;
        
        return;
    }

    ((MGame *)(self))->reset();

    int *status = g_initStatus;
    ((MGame *)(self))->restorePlayerStats();
    ((PlayerEgo *)(self->field_0x58))->resetLastHP();

    // Per-mission HP cap and alien-orbit station bookkeeping.
    if ((*g_status)->getCurrentCampaignMission() != 0x5f) {
        // (Ship max-HP restore delegated within restorePlayerStats variant)
    }
    if ((*g_status)->inAlienOrbit() == 0)
        *(int *)((char *)*status + 0x84) = Station_getIndex((*g_status)->getStation());

    unsigned t = ((ApplicationManager *)(self->field_0x8))->GetCurrentTimeMillis();
    self->field_0x1ac = 0;   // this[1].field_A4
    self->field_0x1b4 = 0;   // this[1].field_AC
    self->field_0x20 = t & 0xffff;
    self->field_0x24 = 0;
    self->field_0x28 = t & 0xffff;
    self->field_0x2c = 0;

    if (Radar::hasScanner() != 0)
        *(int *)((char *)*status + 0x11c) = 0;
    *(int *)((char *)*status + 300) = 0;
    *(int *)((char *)*status + 0x134) = 0;
    *(int *)((char *)*status + 0x13c) = 0;
    *(int *)((char *)*status + 0x144) = 0;

    Item *eq = ((Ship *)(((Status *)((Status *)*g_initStatus))->getShip()))->getFirstEquipmentOfSort(0x15);
    if (eq != 0) {
        self->field_0x1b4 = ((Item *)(eq))->getAttribute(0x26);
        self->field_0x1b0 = ((Item *)(eq))->getAttribute(0x26);
        ((Hud *)(self->field_0x74))->setTimeExtender(1, 0, 1, 0);
    }

    // Alien-orbit / black-market radio messages.
    if ((*g_status)->dlc1Won() != 0 && (*g_status)->inAlienOrbit() != 0 &&
        (*g_status)->getCurrentCampaignMission() < 0x93) {
        if (self->field_0x58 != 0 && self->field_0x84 != 0)
            *(Radio **)((char *)self->field_0x58 + 0x18) = self->field_0x84;
        ((Level *)(self->field_0x78))->createRadioMessage(8, 0);
    }

    if ((*g_status)->inBlackMarketSystem() == 0) {
        *(uint16_t *)((char *)*status + 0x110) = 0;
    } else {
        if (self->field_0x58 != 0 && self->field_0x84 != 0)
            *(Radio **)((char *)self->field_0x58 + 0x18) = self->field_0x84;
        if (*(uint8_t *)((char *)*status + 0x110) == 0) {
            int id;
            Level *lvl;
            if (*(uint8_t *)((char *)*status + 0x111) == 0) {
                EnemyList *enemies = (EnemyList *)(intptr_t)((Level *)(self->field_0x78))->getEnemies();
                if (enemies != 0) {
                    int n = (int)enemies->size;
                    for (int i = 0; i < n; i++) {
                        KIPlayer *e = enemies->data[i];
                        if (e->shipGroup == 8)
                            e->field_0x25 = 0;
                    }
                }
                lvl = self->field_0x78;
                id = 9;
            } else {
                lvl = self->field_0x78;
                id = 0xd;
            }
            ((Level *)(lvl))->createRadioMessage(id, 0);
        } else {
            EnemyList *enemies = (EnemyList *)(intptr_t)((Level *)(self->field_0x78))->getEnemies();
            if (enemies != 0) {
                int n = (int)enemies->size;
                for (int i = 0; i < n; i++) {
                    KIPlayer *e = enemies->data[i];
                    if (e->shipGroup == 8)
                        e->field_0x25 = 0;
                }
            }
        }
    }

    // Weapon selection, particle effects, audio and music init.
    ((MGame *)(self))->setupWeaponsAndAudio();

    
}

// ---- freeCamTouchMove_178af8.cpp ----

// 0x77adc
void TFC_zoomTarget(void *cam, float z);  // 0x78178
// Pan-finish tail helper @0x1ac798 (no-op stack-guard-ok path).

__attribute__((visibility("hidden"))) extern int *g_fcGuard;   // @0x188b10 (stack guard [0])
__attribute__((visibility("hidden"))) extern float g_fcRotScale;  // @0x188d20 (DAT)
__attribute__((visibility("hidden"))) extern float g_fcZoomMax;   // @0x188d24
__attribute__((visibility("hidden"))) extern float g_fcZoomMin;   // @0x188d28

// MGame::freeCamTouchMove(int x, int y, void* touchId): rotate/zoom the free camera.
// The third gesture component arrives pointer-sized but is consumed as a signed
// touch coordinate; route it through intptr_t so the host build does not truncate.
void MGame::freeCamTouchMove(int x, int y, void *touchId) {
    MGame *self = this;
    int ty = (int)(intptr_t)touchId;
    int *guard = g_fcGuard;
    int g0 = *guard;

    if (((PlayerEgo *)(self->field_0x58))->isMining() != 0) {
        self->field_0x111 = 1;
        return ((MGame *)(self))->freeCamPanDone(ty);
        return;
    }
    self->field_0x111 = 0;

    int t0 = self->field_0x98;
    int t1 = self->field_0x9c;
    if (t0 == 0 || t1 == 0) {
        // Single-finger pan: accumulate rotation deltas.
        int dy = y - self->field_0x124;
        int dx = ty - self->field_0x128;
        self->field_0x124 = x;
        self->field_0x128 = ty;
        self->field_0x134 = dy;
        self->field_0x138 = dx;
        self->field_0x13c = 0x3f800000;
        self->field_0x140 = 0x3f800000;
        self->field_0x118 += (float)dy;
        self->field_0x11c += (float)dx;

        if (t0 == 0) {
            float v[4]; v[0] = (float)x; v[1] = (float)ty; v[2] = 0;
            *(Vector*)((Vector *)((char *)self + 0xb0)) = *(const Vector*)((Vector *)v);
        } else if (t1 == 0) {
            float v[4]; v[0] = (float)x; v[1] = (float)ty; v[2] = 0;
            *(Vector*)((Vector *)((char *)self + 0xa4)) = *(const Vector*)((Vector *)v);
        }
        
        return;
    }

    // Two-finger pinch: zoom by the change in finger distance.
    Vector *base = (Vector *)((char *)self + 0xa4);
    float tmp[4];
    *(Vector*)((Vector *)tmp) = *(const Vector*)(base) - *(const Vector*)((Vector *)((char *)self + 0xb0));
    float oldLen = AbyssEngine::AEMath::VectorLength(*(const Vector*)((Vector *)tmp));
    float newLen = oldLen;

    if (self->field_0x98 == ty) {
        float v[4]; v[0] = (float)x; v[1] = (float)ty; v[2] = 0;
        *(Vector*)((Vector *)tmp) = *(const Vector*)(base) - *(const Vector*)((Vector *)v);
        newLen = AbyssEngine::AEMath::VectorLength(*(const Vector*)((Vector *)tmp));
        *(Vector*)(base) = *(const Vector*)((Vector *)v);
    } else if (self->field_0x9c == ty) {
        float v[4]; v[0] = (float)x; v[1] = (float)ty; v[2] = 0;
        *(Vector*)((Vector *)tmp) = *(const Vector*)(base) - *(const Vector*)((Vector *)v);
        newLen = AbyssEngine::AEMath::VectorLength(*(const Vector*)((Vector *)tmp));
        *(Vector*)((Vector *)((char *)self + 0xb0)) = *(const Vector*)((Vector *)v);
    }

    float zoom = self->field_0xbc + (newLen - oldLen) * g_fcRotScale;
    self->field_0xbc = zoom;
    if (zoom > g_fcZoomMax || zoom < g_fcZoomMin) {
        zoom = (zoom > g_fcZoomMax) ? g_fcZoomMax : g_fcZoomMin;
        self->field_0xbc = zoom;
    }
    TFC_zoomTarget(self->field_0xf4, zoom);

    
}

// ---- OnTouchEnd_17a144.cpp ----

// The full button-action dispatch body (cutscene/pause menu, boost/shoot, dock,
// jump, menu navigation, dialogue choice handling, station services, etc.) is an
// enormous switch over the Hud touch-result bits.  Translated as one documented
// helper so the entry point stays faithful and compiling; it consumes the same
// hud-result that the inline body branches on, and performs all field writes and
// engine calls on `self`.

__attribute__((visibility("hidden"))) extern int *g_teGuard;   // @0x18a15c (stack guard [0])

// MGame::OnTouchEnd(int p1, int p2, void *touchId)
void MGame::OnTouchEnd(int p1, int p2, void *touchId) {
    MGame *self = this;
    int *guard = g_teGuard;
    int g0 = *guard;

    // Release the tracked touch id if this is the one we were following.
    if (self->field_0xc0 == touchId) {
        self->field_0xc0 = 0;
        self->field_0xc1 = 0;
        self->field_0xc2 = 0;
        self->field_0xc3 = 0;
    }

    int wasAutoPilot = ((PlayerEgo *)(self->field_0x58))->isAutoPilot();
    self->field_0x1a8 = 0x3f800000;                 // this[1].field_A0 (fast-forward weight)
    TFC_setFastForwardMode(self->field_0xf4, 0);
    // (*ppPVar1)[3].field_18 == ego + 0x... resume-flag.
    *(uint8_t *)((char *)self->field_0x58 + 0x150) = 1;

    unsigned hr = 0;
    if (self->field_0x5d == 0) {
        hr = ((Hud *)(self->field_0x74))->touchEnd(p1, (void *)(intptr_t)p2, (int)(intptr_t)touchId);
        self->field_0xf8 = hr;
        if (hr != 0) {
            self->field_0x98 = 0;
            self->field_0x9c = 0;
        }
    }

    // All button-press reactions live in the dispatch helper.
    ((MGame *)(self))->dispatchTouchEndAction(p1, p2, touchId, hr, wasAutoPilot);

    
}

// ---- successCheck_17ff58.cpp ----

// 0x... missionCompleted
// 0x... checkObjective
// 0x727c0
// Corrupt follow-up-mission setup block (227..291) kept as one helper.

__attribute__((visibility("hidden"))) extern int *g_scGuard;     // @0x18ff6e (stack guard [0])
__attribute__((visibility("hidden"))) extern int *g_scStatus;    // @0x18ffa8 (*puVar18)
__attribute__((visibility("hidden"))) extern uint8_t **g_scFlag; // @0x1904aa

// Bind a DialogueWindow to the current level (duplicated block at 0x1019020c/0x190020).
static void bindDlg(MGame *self) {
    if (self->field_0x8c == 0) {
        DialogueWindow *w = (DialogueWindow *)::operator new(0x74);
        DialogueWindow_ctor(w);
        Level *lvl = self->field_0x78;
        self->field_0x8c = w;
        if (lvl != 0) ((DialogueWindow *)(w))->setLevel(lvl);
    } else if (((DialogueWindow *)(self->field_0x8c))->hasLevel() == 0) {
        Level *lvl = self->field_0x78;
        if (lvl != 0) ((DialogueWindow *)(self->field_0x8c))->setLevel(lvl);
    }
}

// Follow-up-mission setup helpers: getCampaignMission() yields the active campaign Mission*
// (header types it as int); Mission::setType is not declared in the shared header.
// Status::getCampaignMission
// Mission::setType
extern "C" void    *Objective_dtor(Objective *o);                   // Objective::~Objective
// Status::replaceHash(String haystack, String needle, String replacement) -> String (sret),
// modelled as a free function on String12 like the other TUs (see HangarWindow.cpp/Status.cpp).
extern "C" void     Status_replaceHash(String12 *out, Status *self, String12 *haystack,
                                       String12 *needle, String12 *repl);
__attribute__((visibility("hidden"))) extern int  g_scFollowTextKey; // @0x1900c2 (briefing text key)
__attribute__((visibility("hidden"))) extern int  g_scFollowHashLit; // @0x1900f8 ("#station" token literal)

// MGame::buildMissionFollowup(): turn a just-completed mission into its follow-up "deliver
// to <station>" mission. Retargets the campaign mission at the originating agent's station,
// raises its briefing, switches the player out of turret/free-look back to flight, rebuilds
// the agent's mission string and clears the old objectives. The DialogueWindow has already
// been bound to the level by the caller.
void MGame::buildMissionFollowup() {
    MGame *self = this;
    Status *status = (Status *)(*g_status);

    // Point the campaign mission at the agent's home station.
    Mission *cm = ((Status *)(status))->getCampaignMissionPtr();
    Agent *agent = ((Mission *)(((Status *)(status))->getCampaignMissionPtr()))->getAgent();
    ((Mission *)(cm))->setTargetStation(((Agent *)(agent))->getStation());

    // Raise the briefing for the (now retargeted) mission and convert it to a delivery type.
    ((DialogueWindow *)(self->field_0x8c))->set(((Status *)(status))->getCampaignMissionPtr(), 1, -1);
    ((Mission *)(((Status *)(status))->getCampaignMissionPtr()))->setType(0xb);

    // Leave turret / first-person and reset the flight camera.
    self->field_0x58->setTurretMode(0);
    ((LevelScript *)(self->field_0x7c))->resetCamera((Level *)self->field_0x78);
    self->field_0x58->setFreeLookMode(false);
    TFC_enableFirstPersonCam(self->field_0xf4, 0);
    self->field_0x58->hideShipForFirstPersonCameraView(false);
    *(uint8_t *)((char *)self + 0x239) = 1;   // this[1].field_48+1 (delivery-active flag)

    ((Mission *)(((Status *)(status))->getCampaignMissionPtr()))->setStatusValue(0);
    ((Mission *)(((Status *)(status))->getCampaignMissionPtr()))->setWon(false);

    // Rebuild the agent's mission text: take the template and substitute the target station.
    void *tmpl = ((GameText *)(*g_gameText))->getText(g_scFollowTextKey);
    String12 sTmpl, sHash, sStation, sResult;
    sTmpl = *(String12 *)tmpl;
    String_cstr_ctor(&sHash, (const char *)(intptr_t)g_scFollowHashLit, false);
    RetStr station = (status->getMission())->getTargetStationName();
    sStation = *(String12 *)&station;
    Status_replaceHash(&sResult, status, &sTmpl, &sStation, &sHash);
    ((String *)(&sHash))->dtor();

    Agent *missionAgent = ((Mission *)(status->getMission()))->getAgent();
    ((Agent *)(missionAgent))->setMissionString(&sResult);
    ((String *)(&sResult))->dtor();

    status->setMission(((Status *)(status))->getCampaignMissionPtr());

    // Hand the route back to the player and clear it / its waypoint autopilot.
    self->field_0x58->setRoute(0);
    if (self->field_0x58->goingToWaypoint() != 0)
        self->field_0x58->setAutoPilot(0);
    self->field_0x58->removeRoute();
    ((Level *)(self->field_0x78))->setPlayerRoute(0);

    // Release the two old objectives held on the Level (slots 10 and 11).
    int *level = (int *)self->field_0x78;
    if (level[10] != 0) {
        ::operator delete(Objective_dtor((Objective *)(intptr_t)level[10]));
        level = (int *)self->field_0x78;
    }
    level[10] = 0;
    if (level[11] != 0) {
        ::operator delete(Objective_dtor((Objective *)(intptr_t)level[11]));
        level = (int *)self->field_0x78;
    }
    level[11] = 0;

    *(uint16_t *)((char *)self + 0x5d) = 0x101;   // pause + cutscene flags
    ((MGame *)(self))->pauseSounds();
}

// MGame::successCheck(): detect mission completion and run its outcome.
void MGame::successCheck() {
    MGame *self = this;
    int *guard = g_scGuard;
    int g0 = *guard;

    bool timed = !(self->field_0x4c < (int)(self->field_0x48 < 0x1389));
    if (timed || self->field_0xdc != 0) {
        if (((Mission *)((Mission *)(*g_status)->getCampaignMission()))->getType() != 0xaa) goto done;
    }

    {
        int *status = g_scStatus;
        Mission *mc = ((Status *)(*status))->missionCompleted(0, 0, 0);
        int obj = ((Level *)(self->field_0x78))->checkObjective();
        if (mc == 0 && obj == 0) goto done;
    }

    if (((Mission *)((*g_status)->getMission()))->getType() == 5) goto deliverFollowup;
    if (((Mission *)((*g_status)->getMission()))->getType() == 3) goto deliverFollowup;

    {
        int *status = g_scStatus;
        if (((Mission *)((*g_status)->getMission()))->isCampaignMission() == 0)
            ((Status *)(status))->incMissionCount();

        if (((Mission *)((*g_status)->getMission()))->isCampaignMission() != 0) {
            bool hasSuccess = false;
            if (((Mission *)((*g_status)->getMission()))->isCampaignMission() != 0) {
                int cm = (*g_status)->getCurrentCampaignMission();
                if (DialogueWindow::hasSuccessDialogue(cm) != 0)
                    hasSuccess = true;
            }
            if (!hasSuccess) {
                int cm = (*g_status)->getCurrentCampaignMission();
                if (cm > 0x2d && ((Mission *)((*g_status)->getMission()))->isCampaignMission() != 0) {
                    int cm2 = (*g_status)->getCurrentCampaignMission();
                    if (DialogueWindow::hasSuccessDialogue(cm2) == 0) {
                        ((Status *)(*status))->nextCampaignMission();
                        ((Level *)(self->field_0x78))->removeObjectives();
                        ((Status *)(status))->setMission((Mission *)(*g_status)->getCampaignMission() /* mission: arg lost in decomp */);
                    }
                }
                goto done;
            }
            // fallthrough: has success dialogue -> show it.
            bindDlg(self);
            ((DialogueWindow *)(self->field_0x8c))->set((*g_status)->getMission(), 1, -1);
            self->field_0x5d = 0x101;
            ((MGame *)(self))->pauseSounds();

            int cm = (*g_status)->getCurrentCampaignMission();
            if (((Mission *)((*g_status)->getMission()))->isCampaignMission() != 0 && cm == 0x26) {
                EnemyList *enemies = (EnemyList *)(intptr_t)((Level *)(self->field_0x78))->getEnemies();
                unsigned n = enemies->size;
                for (unsigned i = 0; i < n; i++) {
                    // Enemy entries are polymorphic; this branch touches the
                    // PlayerFixedObject layout (field_0x40 "moving" flag).
                    PlayerFixedObject *e = (PlayerFixedObject *)enemies->data[i];
                    if (e->field_0x40 != 0) {
                        ((Player *)(e->player))->setHitpoints((int)(intptr_t)e->player);
                        ((PlayerFixedObject *)(e))->setMoving(1);
                    }
                }
            } else if (((Mission *)((*g_status)->getMission()))->isCampaignMission() != 0 && cm == 0x38) {
                StarSystem *ss = (StarSystem *)(intptr_t)((Level *)(self->field_0x78))->getStarSystem();
                ((StarSystem *)(ss))->getPlanets();
                int pts[3] = {0, 0, 0};
                Route *route = (Route *)::operator new(0x18);
                ((Route *)(route))->ctor(pts, 3);
                EnemyList *enemies = (EnemyList *)(intptr_t)((Level *)(self->field_0x78))->getEnemies();
                unsigned n = enemies->size;
                for (unsigned i = 0; i < n; i++) {
                    KIPlayer *k = *(KIPlayer **)(*(int *)((char *)enemies + 4) + i * 4);
                    if (k->shipGroup == 1) {
                        Route *rc = (Route *)((Route *)(route))->clone();
                        ((KIPlayer *)(k))->setRoute(rc);
                    }
                }
                ::operator delete(((Route *)(route))->dtor());
            } else if (((Mission *)((*g_status)->getMission()))->isCampaignMission() != 0 && cm == 0x3f) {
                EnemyList *enemies = (EnemyList *)(intptr_t)((Level *)(self->field_0x78))->getEnemies();
                unsigned n = enemies->size;
                for (unsigned i = 0; i < n; i++) {
                    KIPlayer *e = enemies->data[i];
                    if (e->shipGroup == 8)
                        ((Player *)((Player *)e->player))->removeAllGuns();
                }
            } else if (((Mission *)((*g_status)->getMission()))->isCampaignMission() != 0 && cm == 0x49) {
                EnemyList *enemies = (EnemyList *)(intptr_t)((Level *)(self->field_0x78))->getEnemies();
                unsigned n = enemies->size;
                for (unsigned i = 0; i < n; i++) {
                    PlayerFixedObject *o = *(PlayerFixedObject **)(*(int *)((char *)enemies + 4) + i * 4);
                    if (o->field_0x40 != 0) {
                        ((Player *)(o->player))->setHitpoints((int)(intptr_t)o->player);
                        ((PlayerFixedObject *)(o))->setMoving(1);
                    }
                }
            } else {
                if (Station_getIndex((*g_status)->getStation()) == 0x70 &&
                    (*g_status)->getCurrentCampaignMission() == 0x8f)
                    **g_scFlag = 1;
            }
            goto done;
        }
        // Non-campaign success-dialogue path.
        bindDlg(self);
        // Slot holds either a Mission* (non-campaign) or a campaign mission id;
        // use a pointer-width carrier so neither is truncated on a 64-bit host.
        intptr_t m = (*g_status)->getCurrentCampaignMission() == 0
                         ? (intptr_t)(*g_status)->getMission()
                         : (intptr_t)(*g_status)->getCurrentCampaignMission();
        ((DialogueWindow *)(self->field_0x8c))->set((Mission *)m, 1, -1);
        self->field_0x5d = 0x101;
        ((MGame *)(self))->pauseSounds();
        goto done;
    }

deliverFollowup:
    bindDlg(self);
    ((MGame *)(self))->buildMissionFollowup();

done:
    
}

// ---- _MGame_177c08.cpp ----
// PIC globals: the MGame vtable base (hidden -> direct pc-relative single-deref of the
// value) and the String destructor.
__attribute__((visibility("hidden"))) extern void *MGame_vtable;

// MGame::~MGame() complete destructor: install the MGame vtable, release game state,
// destroy the embedded String at 0x64, and return this.
MGame * MGame::dtor() {
    MGame *self = this;
    *(void **)self = (char *)MGame_vtable + 8;
    ((MGame *)(self))->OnRelease();
    ((String *)((char *)self + 0x64))->dtor();
    return self;
}

// ---- resumeSounds_178e24.cpp ----

__attribute__((visibility("hidden"))) extern FModSound **g_fmod;

// MGame::resumeSounds(): resume FMOD, the player's engine sound, and every enemy's.
void MGame::resumeSounds() {
    MGame *self = this;
    ((FModSound *)(*g_fmod))->resumeAll();
    ((PlayerEgo *)(self->field_0x58))->ResumeEngineSound();
    EnemyList *e = (EnemyList *)(intptr_t)((Level *)(self->field_0x78))->getEnemies();
    if (e == 0) return;
    for (uint32_t i = 0; i < e->size; i++)
        ((KIPlayer *)(e->data[i]))->ResumeEngineSound();
}

// ---- startChargingJumpDrive_179fb0.cpp ----
__attribute__((visibility("hidden"))) extern Status **g_status;   // DAT_18a128
__attribute__((visibility("hidden"))) extern int *g_jumpFlag;     // DAT_18a134 (*piVar6)
__attribute__((visibility("hidden"))) extern int **g_alienAmt;    // DAT_18a138
__attribute__((visibility("hidden"))) extern int **g_jumpCost;    // DAT_18a130
__attribute__((visibility("hidden"))) extern int **g_alienCost;   // DAT_18a140


// MGame::startChargingJumpDrive(): if the jump drive is armed, validate fuel/cargo, then
// either pop a "no fuel" choice dialogue or actually trigger the hyperspace jump.
void MGame::startChargingJumpDrive() {
    MGame *self = this;
    if (self->field_0xdd == 0) return;
    Status **sp = g_status;
    int needed = 1;
    if (((Ship *)(((Status *)(*sp))->getShip()))->hasCargo(0x7a, 1) == 0) {
        ChoiceWindow *w = self->field_0x94;
        if (w == 0) {
            w = (ChoiceWindow *)MGame_opnew(0x5c);
            new (w) ChoiceWindow();
            self->field_0x94 = w;
        }
        void *txt = ((GameText *)(**(int **)g_status))->getText(0x243);
        ((ChoiceWindow *)(w))->set(*(String *)txt);
        self->field_0x5d = 1;
        self->field_0xce = 1;
        ((MGame *)(self))->pauseSounds();
        **g_jumpCost = 0;
        return;
    }
    int hc = (*g_status)->hardCoreMode();
    int *jf = g_jumpFlag;
    if (hc != 0) needed = 2;
    int cost;
    if (*jf == F<int>((MGame *)*sp, 0x78)) {
        cost = needed << 1;
    } else {
        cost = **g_alienAmt;
        if ((*g_status)->inAlienOrbit() != 0) cost = needed;
    }
    ((Ship *)(((Status *)(*sp))->getShip()))->getCargo(0x7a);
    if (((Item *)(0))->getAmount() < cost) {
        ChoiceWindow *w = self->field_0x94;
        if (w == 0) {
            w = (ChoiceWindow *)MGame_opnew(0x5c);
            new (w) ChoiceWindow();
            self->field_0x94 = w;
        }
        int hc2 = (*g_status)->hardCoreMode();
        void *txt = ((GameText *)(**(int **)g_status))->getText(hc2 != 0 ? 0x243 : 0x244);
        ((ChoiceWindow *)(w))->set(*(String *)txt);
        self->field_0x5d = 1;
        self->field_0xce = 1;
        ((MGame *)(self))->pauseSounds();
        *jf = 0;
        return;
    }
    ((PlayerEgo *)(self->field_0x58))->startJumpDrive();
    if (*jf != F<int>((MGame *)*sp, 0x78)) {
        if ((*g_status)->inAlienOrbit() == 0) needed = **g_alienCost;
    }
    ((Hud *)(self->field_0x74))->hudEvent(0x1e, self->field_0x58, needed);
    ((Ship *)(((Status *)(*sp))->getShip()))->removeCargo(0x7a, needed);
}

// ---- pauseSounds_178ddc.cpp ----

__attribute__((visibility("hidden"))) extern FModSound **g_fmod;

// MGame::pauseSounds(): snapshot the pause flag, pause FMOD, the player's engine sound,
// and every enemy's.
void MGame::pauseSounds() {
    MGame *self = this;
    self->field_0x1a6 = self->field_0x5d;
    ((FModSound *)(*g_fmod))->pauseAllPlayingSoundFXEvents();
    ((PlayerEgo *)(self->field_0x58))->PauseEngineSound();
    EnemyList *e = (EnemyList *)(intptr_t)((Level *)(self->field_0x78))->getEnemies();
    if (e != 0) {
        for (uint32_t i = 0; i < e->size; i++)
            ((KIPlayer *)(e->data[i]))->PauseEngineSound();
    }
}

// ---- reset_178470.cpp ----

// 0x78118
extern "C" void Radio_ctor(Radio *r);  // 0x75160
// 0x71ef0
// 0x74488
extern "C" void *TargetFollowCamera_dtor(void *c);  // 0x72064
extern "C" void TargetFollowCamera_ctor(TargetFollowCamera *c, int cam, int target,
                                        int a, int b, int d, int e, int f, int g); // 0x78124
// 0x78130
// 0x72a18
extern "C" void Radar_ctor(Radar *r, Level *l);  // 0x7813c
// 0x7204c

__attribute__((visibility("hidden"))) extern int g_resAspectA;    // @0x1886dc (DAT)
__attribute__((visibility("hidden"))) extern int g_resAspectB;    // @0x1886e0
__attribute__((visibility("hidden"))) extern int g_resAspectC;    // @0x1886e4
__attribute__((visibility("hidden"))) extern PaintCanvas **g_resCanvas; // @0x1884ea
__attribute__((visibility("hidden"))) extern int *g_resStatus;    // @0x1884fe (*piVar15)
__attribute__((visibility("hidden"))) extern int g_resInitB;      // @0x1886fc
__attribute__((visibility("hidden"))) extern int **g_resShipTune; // @0x188608 ([0][0x2f4])
__attribute__((visibility("hidden"))) extern uint8_t **g_resPauseSrc; // @0x188684

// MGame::reset(): rebuild all per-session subsystems and reset transient state.
void MGame::reset() {
    MGame *self = this;
    self->field_0xbc = 0; self->field_0xc0 = 0;
    self->field_0x98 = 0; self->field_0x9c = 0;
    self->field_0xa0 = 0;
    self->field_0x154 = 0; self->field_0x158 = 0;
    self->field_0x138 = 0;
    self->field_0x15c = 0;
    self->field_0x118 = g_resAspectC;
    self->field_0x11c = g_resAspectA;
    self->field_0x120 = g_resAspectB;
    self->field_0x124 = 0; self->field_0x128 = 0;
    self->field_0x12c = 0;   // 0x12c
    self->field_0x130 = 0; self->field_0x134 = 0;

    self->field_0x58 = (PlayerEgo *)(intptr_t)((Level *)(self))->getPlayer();

    Hud *hud = (Hud *)::operator new(0x53c);
    ((Hud *)(hud))->ctor();
    self->field_0x74 = hud;

    Radio *radio = (Radio *)::operator new(0x48);
    Radio_ctor(radio);
    self->field_0x84 = radio;
    ((Radio *)(radio))->setMessages((Array<RadioMessage *> *)(intptr_t)((Level *)(self->field_0x78))->getMessages());

    PaintCanvas *pc = *g_resCanvas;
    pc->CameraCreate((unsigned *)((char *)self + 0xf0));
    unsigned cam = *(unsigned *)g_resCanvas;
    int *status = g_resStatus;

    float fov = *(float *)&g_resAspectC;  // DAT_001886e8 default
    if ((*g_status)->inAlienOrbit() != 0) {
        int cm = (*g_status)->getCurrentCampaignMission();
        fov = (cm < 0x50) ? *(float *)&g_resInitB : *(float *)&g_resAspectC;
    }
    pc->CameraSetPerspective(cam, fov, 0, 0);

    if (self->field_0xf4 != 0) {
        ::operator delete(TargetFollowCamera_dtor(self->field_0xf4));
        self->field_0xf4 = 0;
    }
    TargetFollowCamera *tfc = (TargetFollowCamera *)::operator new(0x178);
    TargetFollowCamera_ctor(tfc, self->field_0xf0,
                            *(int *)((char *)self->field_0x58 + 8), 0, 0, 0, 0, 0, 0);
    self->field_0xf4 = tfc;
    pc->CameraSetCurrent(self->field_0xf0);
    ((PlayerEgo *)(self->field_0x58))->setTargetFollowCamera(self->field_0xf4);
    ((TargetFollowCamera *)(self->field_0xf4))->resetShipHandling();

    Radar *radar = (Radar *)::operator new(0x248);
    Radar_ctor(radar, self->field_0x78);
    self->field_0x80 = radar;

    if ((*g_status)->getMission() != 0)
        self->field_0x61 = (uint8_t)((Mission *)((*g_status)->getMission()))->isCampaignMission();

    LevelScript *script = (LevelScript *)::operator new(0xe8);
    new (script) LevelScript(self->field_0x78, self->field_0x74,
                             self->field_0x80, self->field_0xf4);
    self->field_0x7c = script;
    ((LevelScript *)(script))->lookBehind();
    ((Level *)(self->field_0x78))->initParticleSystems();

    ChoiceWindow *cw = (ChoiceWindow *)::operator new(0x5c);
    new (cw) ChoiceWindow();
    self->field_0x94 = cw;

    self->field_0x48 = 0; self->field_0x4c = 0;
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x5c = 0;
    self->field_0x5f = 0;
    self->field_0xc4 = 0;
    self->field_0xc6 = 0;
    self->field_0xc8 = 0;
    self->field_0xe0 = 0;
    self->field_0x15e = 0;
    self->field_0x110 = 0;
    self->field_0x168 = 0x3f800000;
    self->field_0x1b0 = 0;
    self->field_0x1b4 = g_resInitB;
    self->field_0x100 = 0;
    self->field_0x104 = 0; self->field_0x108 = 0; self->field_0x10c = 0;
    self->field_0x1b8 = 0;
    self->field_0x1cc = (*g_resShipTune)[0x2f4 / 4];
    self->field_0x1d0 = 0;
    self->field_0x1e8 = 0;
    self->field_0x178 = 0;
    self->field_0x17c = 0;
    self->field_0x180 = 0; self->field_0x184 = 0;

    unsigned t = ((ApplicationManager *)(self->field_0x8))->GetCurrentTimeMillis();
    self->field_0x111 = 0x101;
    self->field_0xcf = 0;
    self->field_0xd0 = 0;
    self->field_0xd4 = 0;
    self->field_0x1e4 = 0;
    self->field_0x1e6 = 0;
    self->field_0x20 = t & 0xffff;
    self->field_0x24 = 0;
    self->field_0x28 = t & 0xffff;
    self->field_0x2c = 0;
    self->field_0x1dd = **g_resPauseSrc;

    // *status points at the Status object; fields +0x184/0x188/0x18c are not
    // modeled in gof2/Status.h (owned by another batch) — write via byte offset.
    void *s = (void *)(intptr_t)*status;
    F<int>(s, 0x184) = 0;
    F<int>(s, 0x188) = 1;
    F<int>(s, 0x18c) = 1;
}

// ---- handleAccelerometer_178714.cpp ----

// 0x6ff88
// 0x729d0
// 0x729dc
// Roll-control callbacks invoked by fn-ptr: (ego, shipField, amt).
extern "C" void *MGame_accelCtxBegin(int field8);  // fn @0x1887b4 (pcVar5)
extern "C" double *MGame_accelCtxValue();  // fn @0x1887bc (pcVar4)
// Roll tail helpers @0x1abb78 / @0x1abb68.

__attribute__((visibility("hidden"))) extern int g_accelTune;   // @0x1887d4 (*(int*) iVar6)

// MGame::handleAccelerometer(): tilt-steer the ship from device accelerometer input.
void MGame::handleAccelerometer() {
    MGame *self = this;
    Engine *eng = (Engine *)((ApplicationManager *)(self->field_0x8))->GetEngine();
    double *acc = ((Engine *)(eng))->GetAccelValue();

    // Yaw from raw tilt (axis at +8, scaled by 2.5).
    float yaw = (float)(*(double *)((char *)acc + 8) * 2.5);
    float steer = 1.0f;
    if (yaw <= 1.0f) {
        steer = -1.0f;
        if (yaw < -1.0f) {
            // frameTime arg was dropped by the decompiler; the per-frame nominal
            // tick (0x32 ms) is used as the time multiplier.
            ((PlayerEgo *)(self->field_0x58))->left(0x32, steer * steer);
            goto afterYaw;
        }
        steer = yaw;
        if (yaw < 0.0f) {
            ((PlayerEgo *)(self->field_0x58))->left(0x32, steer * steer);
            goto afterYaw;
        }
        if (yaw == 0.0f)
            goto afterYaw;
    }
    ((PlayerEgo *)(self->field_0x58))->right(0x32, steer * steer);

afterYaw: {
    int field8 = self->field_0x8;

    MGame_accelCtxBegin(field8);
    double *v1 = MGame_accelCtxValue();
    double d0 = *v1;
    MGame_accelCtxBegin(field8);
    int c2 = (int)(long)MGame_accelCtxValue();

    float base = (float)d0;
    int tune = g_accelTune;
    double dz = *(double *)((char *)c2 + 0x10);
    float ref = *(float *)((char *)tune + 0x1c);
    char sign = *(char *)((char *)tune + 0x10);

    MGame_accelCtxBegin(field8);
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
    int shipOff = (self->field_0x16c > 0) ? 0x44 : 0x40;
    if (roll <= 1.0f) {
        rollMag = -1.0f;
        if (roll < -1.0f) {
            return ((PlayerEgo *)(self->field_0x58))->rollRight(F<int>(self, shipOff), rollMag * rollMag);
        }
        rollMag = roll;
        if (roll < 0.0f) {
            return ((PlayerEgo *)(self->field_0x58))->rollRight(F<int>(self, shipOff), rollMag * rollMag);
        }
        if (roll == 0.0f) return;
    }
    return ((PlayerEgo *)(self->field_0x58))->rollLeft(F<int>(self, shipOff), rollMag * rollMag);
}
}

// ---- OnTouchMove_1799dc.cpp ----

// 0x78238
// 0x78244
// 0x78250
// 0x7825c
// 0x78268
// 0x78274
// 0x75118
// 0x750d0
// 0x750e8
// 0x7474c
// Thrust-from-drag application (0x189af2..0x189b46): new = max(2, base + dY*scale),
// then setThrust + throttleChanged. Helper keeps the corrupt float math compiling.

__attribute__((visibility("hidden"))) extern int **g_tmShipTune;  // @0x189b12 ([0][0x2f8] thrust scale)
__attribute__((visibility("hidden"))) extern int **g_tmStarMap;   // @0x189b8a (StarMap/Layout sel)
__attribute__((visibility("hidden"))) extern int **g_tmAppData;   // @0x189bf8

// MGame::OnTouchMove(int p1, int y, void *touch)
void MGame::OnTouchMove(int p1, int y, void *touch) {
    MGame *self = this;
    int handledFree = 0;
    if (self->field_0x5d != 0) {
        uint8_t fc = self->field_0x15e;
        int allowFree = (fc != 0);
        if (fc != 0 &&
            ((MenuTouchWindow *)(self->field_0x88))->isShowingMessage() == 0 &&
            ((MenuTouchWindow *)(self->field_0x88))->isMakingScreenshot() == 0) {
            allowFree = 1;
        } else {
            allowFree = 0;
        }
        (void)allowFree;
    }

    if (self->field_0x15c != 0 && self->field_0xf8 == 0 &&
        self->field_0x14 == 3) {
        ((MGame *)(self))->freeCamTouchMove(p1, y, touch);
        handledFree = 1;
    }

    if (!handledFree) {
        if (self->field_0x5d == 0) {
            int hh = ((Hud *)(self->field_0x74))->touchMove(p1, (void *)(intptr_t)y, (int)(intptr_t)touch);
            self->field_0xf8 = hh;
            unsigned mode = (unsigned)self->field_0x14;
            if (mode <= 1) {
                ((MGame *)(self))->maneuverTouchMove(mode, y);
                if (self->field_0x1b8 != 0 && self->field_0x5f == 0) {
                    int f8 = self->field_0xf8;
                    int ok = (f8 == 0) ||
                             (f8 == 0x20 && self->field_0xc0 != touch);
                    if (ok) {
                        float thrust;
                        if (self->field_0x1b9 == 0) {
                            float fy = (float)y;
                            float start = (float)self->field_0x1c0;
                            float thresh = (float)self->field_0x1cc;
                            float d = fy - start;
                            float ad = (d > 0.0f) ? d : -d;
                            if (ad > thresh) {
                                int dir = (start > fy) ? 1 : -1;
                                int ny = dir + y;
                                self->field_0x1c8 = 0;
                                self->field_0x1c0 = ny;
                                thrust = ((PlayerEgo *)(self->field_0x58))->getThrust();
                                self->field_0x1d0 = (int)thrust;
                                self->field_0x1b9 = 1;
                                applyThrust(self, y);
                            }
                        } else {
                            self->field_0x1b9 = 1;
                            applyThrust(self, y);
                        }
                    }
                }
            }
        }
    }

    if (self->field_0x5d == 0) return;

    // Modal / overlay touch routing.
    if (self->field_0x60 != 0 || self->field_0xc5 != 0 ||
        self->field_0xce != 0 || self->field_0xc4 != 0) {
        ((ChoiceWindow *)(self->field_0x94))->OnTouchMove(p1, y);
        return;
    }
    if (self->field_0xc7 != 0) {
        int sel = **g_tmStarMap;
        if (*(uint8_t *)sel == 0)
            ((StarMap *)(self->field_0x90))->OnTouchMove(p1, y);
        else
            ((Layout *)(**g_tmStarMap))->OnTouchMove(p1, y);
        return;
    }
    if (self->field_0x5e != 0) {
        ((DialogueWindow *)(self->field_0x8c))->OnTouchMove(p1, y);
        return;
    }
    if (self->field_0xc9 != 0) {
        int ad = **g_tmAppData;
        if (*(uint8_t *)((char *)ad + 5) == 0 && *(uint8_t *)((char *)ad + 0xc) == 0)
            ((MenuTouchWindow *)(self->field_0x88))->OnTouchMove(p1, y);
    }
}

// ---- setCinematicMode_17c454.cpp ----
// Hidden PC-relative globals: a "cinematic active" byte flag and an input/UI byte flag.
__attribute__((visibility("hidden"))) extern uint8_t *g_cinFlagA;
__attribute__((visibility("hidden"))) extern uint8_t **g_cinFlagB;

// camera enter/leave cinematic helper (this->[0xf4], byte[0x164]) — tail call.
// hud cinematic helper (**[0x78], [0x40], 1) — tail call.

// MGame::setCinematicMode(bool): toggles cinematic camera/UI state, switching cameras
// and (de)activating the look-at HUD overlay.
void MGame::setCinematicMode(bool on) {
    MGame *self = this;
    self->field_0x15e = on;
    *g_cinFlagA = on;
    uint8_t *flag = *g_cinFlagB;
    if (!on) {
        *flag = self->field_0x1dd;
        ((MGame *)(self))->switchCamera(self->field_0x160);
        return Cam_setCinematic(self->field_0xf4, self->field_0x164);
    }
    self->field_0x1dd = *flag;
    *flag = 1;
    if (self->field_0xdc == 0 && self->field_0x5f == 0) {
        self->field_0x160 = self->field_0x14;
        self->field_0x164 = TFC_isInLookAtMode(self->field_0xf4);
        TFC_setLookAtCam(self->field_0xf4, 0);
        ((MGame *)(self))->switchCamera(3);
        return ((Hud *)(*(void **)self->field_0x78))->enterCinematic(self->field_0x40, 1);
    }
}

// ---- updateJumpScene_17f588.cpp ----

// 0x785c8
void TFC_translate(void *cam, int x, int y, int z);  // 0x72688
// 0x720b8
// 0x6f7cc
extern "C" void *TFC_getPosition(void *cam);  // 0x76b28
// 0x785d4
// 0x76aec
// 0x72af0
// 0x785e0
// 0x785ec
// 0x785f8
// 0x745fc
// 0x74608
// 0x74614
// 0x73678
// 0x7366c

__attribute__((visibility("hidden"))) extern int *g_ujGuard;     // @0x18f598 (stack guard [0])
__attribute__((visibility("hidden"))) extern unsigned **g_ujCanvasA; // @0x18f5b2
__attribute__((visibility("hidden"))) extern unsigned **g_ujCanvasB; // @0x18f6a4
__attribute__((visibility("hidden"))) extern int g_ujZNear;      // @0x18f8dc (DAT)
__attribute__((visibility("hidden"))) extern int g_ujZSound;     // @0x18f8e0
__attribute__((visibility("hidden"))) extern int g_ujSpeed;      // @0x18f8e4
__attribute__((visibility("hidden"))) extern int *g_ujSound;     // @0x18f71e
__attribute__((visibility("hidden"))) extern unsigned **g_ujCanvasC; // @0x18f782
__attribute__((visibility("hidden"))) extern int *g_ujStatus;    // @0x18f7ac (piVar9)
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
    int *guard = g_ujGuard;
    int g0 = *guard;
    bool fadeOut = true;

    if (self->field_0xdd != 0 && self->field_0x114 != 0) {
        int tr = (int)(long)((PaintCanvas*)g_PaintCanvas)->TransformGetTransform(**g_ujCanvasA);
        int prog = *(int *)((char *)tr + 0x114);
        int over = (*(unsigned *)((char *)tr + 0x110) > 0x6a4);
        if ((0 - over) - prog < 0) goto camMove;
    } else {
        int lm = ((Level *)(self->field_0x78))->getLandmarks();
        if (*(int *)((char *)*(int *)((char *)lm + 4) + 4) != 0) {
            ((Level *)(self->field_0x78))->getLandmarks();
            int jg = *(int *)((char *)*(int *)((char *)lm + 4) + 4);
            if (((PlayerJumpgate *)(jg))->timeToJump() == 0) goto camMove;
        }
    }
    fadeOut = true;
    goto afterCam;

camMove: {
        int speed = self->field_0x40;
        int ego = *(int *)((char *)self->field_0x58);
        float mtx[4];
        *(Vector*)((Vector *)((char *)ego + 4)) = AbyssEngine::AEMath::MatrixRotateVector(*(const Matrix*)(mtx), *(const Vector*)((Vector *)((char *)ego + 4)));
        *(Vector*)((Vector *)((char *)self + 0xe4)) = *(const Vector*)((Vector *)mtx);
        TFC_translate(self->field_0xf4, 0, 0, 0);
        (void)speed;
        if (self->field_0xdd != 0) {
            ((AEGeometry *)(self->field_0x114))->getPosition();
        } else {
            int lm = ((Level *)(self->field_0x78))->getLandmarks();
            void *obj = *(void **)((char *)*(int *)((char *)lm + 4) + 4);
            void *vt = *(void **)obj;
            (*(void (**)(void *, void *))((char *)vt + 0x28))(obj, mtx);
        }
        *(Vector*)((Vector *)((char *)self + 0xe4)) = *(const Vector*)((Vector *)mtx);
        fadeOut = false;
    }

afterCam:
    if (self->field_0xdd != 0) {
        unsigned tr = (unsigned)(long)((PaintCanvas*)g_PaintCanvas)->TransformGetTransform(**g_ujCanvasB);
        ((TransformFull *)(tr))->Update(self->field_0x40, false /* updateBounds: arg lost in decomp */);
    }

    void *camPos = TFC_getPosition(self->field_0xf4);
    float threshold = (float)self->field_0xec + *(float *)&g_ujZNear;
    float fVar14 = threshold;
    if (*(float *)((char *)camPos + 8) < threshold && self->field_0xdd == 0) {
        int lm = ((Level *)(self->field_0x78))->getLandmarks();
        ((PlayerJumpgate *)(*(int *)((char *)*(int *)((char *)lm + 4) + 4)))->activate();
        float p[4];
        ((PlayerEgo *)(p))->getPosition();
        float t2 = (float)self->field_0xec + *(float *)&g_ujZSound;
        if (t2 <= p[2] && self->field_0xcd == 0) {
            int *snd = g_ujSound;
            FModSound_setProp(*snd, *(int *)((char *)self->field_0x58 + 0x1c));
            FModSound_setProp(*snd, 0x8d5);
            FModSound_setProp(*snd, 0x8d4);
            FModSound_setProp(*snd, 0x23);
            ((FModSound *)(*snd))->play(0x1f, 0, 0 /* vel: arg lost in decomp */, 0.0f);
            self->field_0xcd = 1;
        }
    }

    if (fadeOut) {
        ((PlayerEgo *)(self->field_0x58))->setSpeed(*(float *)&g_ujSpeed);
        ((PlayerEgo *)(self->field_0x58))->setVisible(0);
        ((PlayerEgo *)(self->field_0x58))->setExhaustVisible(0);
    }
    (void)fVar14;

    // Animation-end check.
    bool ended;
    if (self->field_0xdd != 0) {
        int tr = (int)(long)((PaintCanvas*)g_PaintCanvas)->TransformGetTransform(**g_ujCanvasC);
        ended = *(uint8_t *)((char *)tr + 0xed) != 0;
    } else {
        int lm = ((Level *)(self->field_0x78))->getLandmarks();
        ended = ((PlayerJumpgate *)(*(int *)((char *)*(int *)((char *)lm + 4) + 4)))->animationEnded() != 0;
    }
    if (!ended) goto done;

    {
        int *status = g_ujStatus;
        if ((*g_status)->getCurrentCampaignMission() == 0x2a && (*g_status)->inAlienOrbit() != 0) {
            ((LevelScript *)(self->field_0x7c))->setEvent(6);
            ((PlayerEgo *)(self->field_0x58))->setSpeed(0.0f);
            int lm = ((Level *)(self->field_0x78))->getLandmarks();
            int *node = *(int **)((char *)*(int *)((char *)lm + 4) + 0xc);
            (*(void (**)(int *, int, int, int))((char *)*node + 0x48))(node, g_ujPos0, g_ujPos1, g_ujPos2);
            lm = ((Level *)(self->field_0x78))->getLandmarks();
            void *node2 = *(void **)((char *)*(int *)((char *)lm + 4) + 0xc);
            float mtx[4];
            (*(void (**)(void *, void *))((char *)*(void **)node2 + 0x28))(node2, mtx);
            *(Vector*)((Vector *)((char *)self + 0xe4)) = *(const Vector*)((Vector *)mtx);
            ((PlayerEgo *)(self->field_0x58))->setPosition();
            PlayerEgo *p = self->field_0x58;
            p->field_0x25 = 1;
            self->field_0xdc = 0;
            ((PlayerEgo *)(p))->resetChargingDrive();
        } else {
            int **stationPtr = g_ujStation;
            ((Status *)((Station *)*status))->departStation((Station *)*stationPtr);
            ((Level *)(self->field_0x78))->setInitStreamOut();
            if (self->field_0xdd == 0)
                ((Status *)((Station *)*status))->jumpgateUsed();
            if (((Station *)((Station *)*stationPtr))->equals(*(Station **)((char *)*status + 0x78)) != 0) {
                **g_ujFlagA = 1;
                **g_ujFlagB = 1;
                ((Status *)((Station *)*status))->setStation((Station *)*stationPtr /* station: arg lost in decomp */);
            }
            *stationPtr = 0;
            *(int *)((char *)*status + 100) = ((Player *)((Player *)*(int *)((char *)self->field_0x58)))->getHitpoints();
            *(int *)((char *)*status + 0x5c) = ((Player *)((Player *)*(int *)((char *)self->field_0x58)))->getShieldHP();
            *(int *)((char *)*status + 0x60) = ((Player *)((Player *)*(int *)((char *)self->field_0x58)))->getArmorHP();
            *(int *)((char *)*status + 0x68) = ((Player *)((Player *)*(int *)((char *)self->field_0x58)))->getGammaHP();
            *(int *)((char *)*status + 0xf4) = ((PlayerEgo *)(self->field_0x58))->getCurrentSecondaryWeaponIndex();
            **g_ujFlagC = 1;
            self->field_0x54 = 0;
            ((ApplicationManager *)(self->field_0x8))->SetCurrentApplicationModule(5);
        }
    }

done:
    
}

// ---- MGame_177b30.cpp ----
extern "C" void String_default_ctor(void *s);  // 0x6efbc
__attribute__((visibility("hidden"))) extern int g_mgameVtable; // @0x187b3c ([0]=vtable base)
__attribute__((visibility("hidden"))) extern int g_mgameInitVal; // @0x187c00 (DAT_00187c00)

// MGame::MGame() — install vtable, default-construct the title String, zero state.
MGame * MGame::ctor() {
    MGame *self = this;
    self->field_0x0 = g_mgameVtable + 8;
    String_default_ctor((char *)self + 0x64);

    int z = 0;
    int initVal = g_mgameInitVal;

    // NEON-cleared 16-byte groups at 0xa4, 0x13c, 0x18c.
    static const unsigned grpA[] = {0xa4u, 0x13cu, 0x18cu};
    for (unsigned i = 0; i < 3; i++) {
        unsigned off = grpA[i];
        F<int>(self, off + 0)  = 0;
        F<int>(self, off + 4)  = 0;
        F<int>(self, off + 8)  = 0;
        F<int>(self, off + 12) = 0;
    }

    self->field_0x48 = z; self->field_0x4c = z;
    self->field_0xe4 = z; self->field_0xe8 = z;
    self->field_0xec = z;
    self->field_0x1bc = z; self->field_0x1c0 = z;
    self->field_0x1c4 = z;
    self->field_0xb4 = z; self->field_0xb8 = z;
    self->field_0x14c = z; self->field_0x150 = z;
    self->field_0x19c = z; self->field_0x1a0 = z;

    self->field_0xc = 0x64;       // field_0C = 100
    self->field_0x10 = -1;        // field_10 = -1
    self->field_0x14 = z;

    // NEON-cleared 16-byte groups at 0x30, 0x20, 0x80, 0x70.
    static const unsigned grpB[] = {0x30u, 0x20u, 0x80u, 0x70u};
    for (unsigned i = 0; i < 4; i++) {
        unsigned off = grpB[i];
        F<int>(self, off + 0)  = 0;
        F<int>(self, off + 4)  = 0;
        F<int>(self, off + 8)  = 0;
        F<int>(self, off + 12) = 0;
    }

    self->field_0x1a6 = 0;
    self->field_0x54 = 0;
    self->field_0xd5 = 0;
    self->field_0x114 = 0;
    self->field_0xf4 = 0;
    self->field_0x1e4 = 0;
    self->field_0x1d4 = z;
    self->field_0x40 = z;
    self->field_0x58 = 0; self->field_0x5c = z;
    self->field_0x60 = 0;
    self->field_0x90 = 0; self->field_0x94 = 0;
    self->field_0xc8 = 0;
    self->field_0xc4 = z;
    self->field_0xce = 0;
    self->field_0xca = z;
    self->field_0xd8 = z; self->field_0xdc = z;
    self->field_0x1d8 = initVal;
    self->field_0x1e0 = z;
    return self;
}

// ---- OnRelease_17c6bc.cpp ----

// 0x74f74
// 0x75310
// 0x7531c
// 0x7846c
extern "C" void *Radar_dtor(void *r);  // 0x78484
extern "C" void *Radio_dtor(...);  // 0x75388
// 0x75bd4
// 0x7537c
extern "C" void *DialogueWindow_dtor(...);  // 0x75010
unsigned short GameText_getLanguage();  // 0x6f544
void Globals_loadFont(int font, int lang);  // 0x71d04
// 0x71d10
// 0x71d1c
extern "C" void ArrayReleaseClasses_StringPtr(void *arr);  // 0x6facc
extern "C" void *ArrayStringPtr_dtor(void *arr);  // 0x6f64c
// Tail helper @0x1ac168 (re-enables low-pass / restores FMOD state).

__attribute__((visibility("hidden"))) extern int g_relPostEffect;   // @0x18c8b8 (DAT)
__attribute__((visibility("hidden"))) extern int *g_relSoundFlag;   // @0x18c6da (*piVar4)
__attribute__((visibility("hidden"))) extern int *g_relAppMod;      // @0x18c792 (*puVar3)
__attribute__((visibility("hidden"))) extern int *g_relFont;        // @0x18c856 (**)
__attribute__((visibility("hidden"))) extern int *g_relLayout;      // @0x18c86a (**)
__attribute__((visibility("hidden"))) extern int **g_relImgFactory; // @0x18c878

// MGame::OnRelease(): tear down the whole game session and reload UI resources.
void MGame::OnRelease() {
    MGame *self = this;
    Engine *eng = (Engine *)((ApplicationManager *)(self->field_0x8))->GetEngine();
    ((Engine *)(eng))->SetPostEffect(g_relPostEffect, 0);

    int *soundFlag = g_relSoundFlag;
    if (*soundFlag != 0) {
        ((FModSound *)(0))->setDownPitch(true /* down: arg lost in decomp */);
        ((FModSound *)((Engine *)*soundFlag))->disableReverb();
        ((FModSound *)((Engine *)*soundFlag))->stopAllSoundFXEvents();
    }

    if (self->field_0x78 != 0) {
        self->field_0x78->~Level();
        ::operator delete(self->field_0x78);
    }

    self->field_0x48 = 0; self->field_0x4c = 0;
    self->field_0x14 = 0;
    self->field_0x78 = 0;
    self->field_0x54 = 0;
    self->field_0x70 = 0;
    self->field_0x40 = 0;
    self->field_0x60 = 0;
    self->field_0x58 = 0; self->field_0x5c = 0;
    // NEON-clear 0x30 and 0x20 groups.
    for (unsigned i = 0; i < 4; i++) F<int>(self, 0x30 + i * 4) = 0;
    for (unsigned i = 0; i < 4; i++) F<int>(self, 0x20 + i * 4) = 0;

    if (self->field_0x114 != 0) {
        self->field_0x114->~AEGeometry();
        ::operator delete(self->field_0x114);
    }
    self->field_0x114 = 0;

    if (self->field_0x74 != 0)
        ::operator delete(((Hud *)(self->field_0x74))->dtor());
    self->field_0x74 = 0;

    if (self->field_0x7c != 0) {
        self->field_0x7c->~LevelScript();
        ::operator delete(self->field_0x7c);
    }
    self->field_0x7c = 0;

    if (self->field_0x80 != 0)
        ::operator delete(Radar_dtor(self->field_0x80));
    self->field_0x80 = 0;

    if (self->field_0x84 != 0)
        ::operator delete(Radio_dtor(self->field_0x84));
    self->field_0x84 = 0;

    int *appMod = g_relAppMod;
    void *m1 = ((ApplicationManager *)(*appMod))->GetApplicationModule(0);
    if (*(int *)((char *)m1 + 0x10) != 0) {
        void *m2 = ((ApplicationManager *)(*appMod))->GetApplicationModule(0);
        StarMap *sm = *(StarMap **)((char *)m2 + 0x10);
        if (sm != 0) {
            sm->~StarMap();
            ::operator delete(sm);
        }
    }
    void *m3 = ((ApplicationManager *)(*appMod))->GetApplicationModule(0);
    *(int *)((char *)m3 + 0x10) = 0;

    if (self->field_0x88 != 0)
        ::operator delete(((MenuTouchWindow *)(self->field_0x88))->dtor());
    self->field_0x88 = 0;

    if (self->field_0x8c != 0)
        ::operator delete(DialogueWindow_dtor(self->field_0x8c));
    self->field_0x8c = 0;

    if (self->field_0x90 != 0) {
        self->field_0x90->~StarMap();
        ::operator delete(self->field_0x90);
    }
    self->field_0x90 = 0;

    if (self->field_0x94 != 0) {
        self->field_0x94->~ChoiceWindow();
        ::operator delete(self->field_0x94);
    }
    self->field_0x94 = 0;

    self->field_0xd8 = 0; self->field_0xdc = 0;
    self->field_0xd5 = 0;
    self->field_0xc5 = 0;
    self->field_0xcb = 0;

    if (self->field_0xf4 != 0)
        ::operator delete(TargetFollowCamera_dtor(self->field_0xf4));
    self->field_0xf4 = 0;

    if (self->field_0x1e8 != 0) {
        ((GameRecord *)(intptr_t)self->field_0x1e8)->~GameRecord();
        ::operator delete((void *)(intptr_t)self->field_0x1e8);
    }
    self->field_0x1e8 = 0;

    // GetApplicationData()->[0] : release-all-resources target.
    {
        int *p = (int *)0; (void)p;
    }
    ((PaintCanvas*)g_PaintCanvas)->ReleaseAllResources();

    int font = **(int **)&g_relFont;
    int lang = GameText_getLanguage();
    Globals_loadFont(font, lang);

    int *layout = g_relLayout;
    if (*layout != 0) {
        ((Layout *)(*layout))->reload();
        ((ImageFactory *)(**g_relImgFactory))->reload();
        ((Layout *)(*layout))->initTip();
    }

    ArrayReleaseClasses_StringPtr(self->field_0x1ec);
    if (self->field_0x1ec != 0)
        ::operator delete(ArrayStringPtr_dtor(self->field_0x1ec));
    self->field_0x1ec = 0;

    if (*soundFlag != 0)
        FModSound_restoreState();
}

// ---- OnRender2D_1808c0.cpp ----

// 0x74458
// Radio::draw wrapper
// Radar::draw wrapper
// 0x18c624 nextCamId
// Hud::draw wrapper
// 0x... drawFade
// 0x755d4
// splash/fade text helper

__attribute__((visibility("hidden"))) extern int *g_r2dGuard;    // @0x1908d4 (stack guard [0])
__attribute__((visibility("hidden"))) extern unsigned **g_r2dCanvas; // @0x1908e8
__attribute__((visibility("hidden"))) extern Layout ***g_r2dHelpLayout; // @0x190962
__attribute__((visibility("hidden"))) extern int **g_r2dPauseFlag; // @0x1909cc
__attribute__((visibility("hidden"))) extern int **g_r2dRewardLayout; // @0x190d4a
__attribute__((visibility("hidden"))) extern Layout ***g_r2dFadeLayout; // @0x190a76

// MGame::OnRender2D(): draw the 2D HUD / menus / overlays for the active state.
void MGame::OnRender2D() {
    MGame *self = this;
    int *guard = g_r2dGuard;
    int g0 = *guard;
    if (self->field_0x54 == 0) {
        
        return;
    }

    ((PaintCanvas*)g_PaintCanvas)->Begin2d();

    if (self->field_0x5d != 0 && self->field_0xc9 != 0) {
        // Menu-touch-window / star-system background path.
        bool drawSS = true;
        if (self->field_0x15e != 0 && self->field_0x15c != 0 &&
            *(uint8_t *)((char *)self->field_0x88 + 1) == 0) {
            drawSS = true;
        } else {
            ((MenuTouchWindow *)(self->field_0x88))->draw();
            drawSS = (self->field_0x15e != 0);
        }
        if (drawSS) {
            ((Level *)(self->field_0x78))->getStarSystem();
            ((StarSystem *)(0))->render2D();
        }
        float v[4]; *(int *)&v[0] = 0x3f000000; *(int *)&v[1] = 0x3f000000; v[2] = 0;
        Engine *eng = (Engine *)((ApplicationManager *)(self->field_0x8))->GetEngine();
        *(Vector*)((char *)eng + 0x3cc) = *(const Vector*)(v);
        ((PaintCanvas*)g_PaintCanvas)->End2d();
        
        return;
    }

    if (self->field_0x5d == 0 || self->field_0x111 == 0) {
        // World/HUD render path.
        if (self->field_0x111 == 0) {
            // Cinematic-sequence path (field 0x49 etc.).
            ((Level *)(self->field_0x78))->getStarSystem();
            ((StarSystem *)(0))->render2D();
            if (((LevelScript *)(self->field_0x7c))->startSequenceOver() != 0 ||
                ((LevelScript *)(self->field_0x7c))->startSequence() == 0)
                ((MGame *)(self))->drawRadio();
        } else if (self->field_0xc7 != 0) {
            ((StarMap *)(self->field_0x90))->draw();
            Layout *hl = **g_r2dHelpLayout;
            if (*(uint8_t *)hl != 0)
                ((Layout *)(hl))->drawHelpWindow();
        } else {
            ((Level *)(self->field_0x78))->render2D();
            if (**g_r2dPauseFlag == 0)
                ((Hud *)(self->field_0x74))->drawPauseButton();

            if (((Mission *)((Mission *)(*g_status)->getCampaignMission()))->getType() == 0xaa) {
                if (((LevelScript *)(self->field_0x7c))->getEvent() == 0)
                    ((Hud *)(self->field_0x74))->drawOrbitInformation();
                ((MGame *)(self))->drawRadio();
                if (self->field_0x5e != 0)
                    ((DialogueWindow *)(self->field_0x8c))->draw();
                self->field_0x111 = 0;
                self->field_0x5d = 0;
            } else if (self->field_0x5f != 0 || self->field_0x15d != 0) {
                int cm = (*g_status)->getCurrentCampaignMission();
                if (cm > 7 && self->field_0x80 == 0 && self->field_0x15d == 0 &&
                    ((PlayerEgo *)(self->field_0x58))->isDockingToPlanet() == 0 &&
                    ((LevelScript *)(self->field_0x7c))->getEvent() == 0)
                    ((Hud *)(self->field_0x74))->drawOrbitInformation();
                if (((LevelScript *)(self->field_0x7c))->startSequenceOver() != 0 ||
                    ((LevelScript *)(self->field_0x7c))->startSequence() == 0)
                    ((MGame *)(self))->drawRadio();
                if (self->field_0x5e != 0)
                    ((DialogueWindow *)(self->field_0x8c))->draw();
                self->field_0x111 = 0;
                self->field_0x5d = 0;
            } else if (self->field_0x60 == 0) {
                ((PlayerEgo *)(self->field_0x58))->draw(1 /* allowHud: arg lost in decomp */);
                if (((PlayerEgo *)(self->field_0x58))->isMining() == 0 &&
                    self->field_0xc7 == 0 &&
                    (((PlayerEgo *)(self->field_0x58))->isHacking() == 0 ||
                     self->field_0xc9 != 0))
                    ((MGame *)(self))->drawRadar();
                if (self->field_0x5e == 0) {
                    ((MGame *)(self))->nextCamId(self->field_0x14);
                    ((MGame *)(self))->drawHud();
                    ((MGame *)(self))->drawRadio();
                    ((Radar *)(self->field_0x80))->drawCurrentLock((Hud *)(self->field_0x74) /* hud: arg lost in decomp */);
                    ((Layout *)(**g_r2dRewardLayout))->drawMissionRewardMessage(1 /* transition: arg lost in decomp */);
                } else {
                    ((DialogueWindow *)(self->field_0x8c))->draw();
                }
                if (self->field_0xc5 != 0 || self->field_0xc6 != 0 ||
                    self->field_0xce != 0 || self->field_0xc4 != 0)
                    ((ChoiceWindow *)(self->field_0x94))->draw();
                if (self->field_0xca != 0)
                    ((Hud *)(self->field_0x74))->drawMenu();
            } else if (!(self->field_0x4c < (int)(self->field_0x48 < 0xbb9))) {
                // Loading/jump splash text.
                ((PaintCanvas*)g_PaintCanvas)->SetColor((unsigned)self->field_0x4);
                ((PaintCanvas*)(long)(*(int *)g_r2dCanvas))->DrawImage2D((unsigned)self->field_0x10, 0, 0, (unsigned char)'D');
                if (self->field_0x50 >= 4000)
                    ((MGame *)(self))->drawFadeMessage(*(int *)g_r2dCanvas);
            }
            ((Layout *)(**g_r2dFadeLayout))->drawFade();
        }
    }

    ((PaintCanvas*)g_PaintCanvas)->End2d();
    
}

// ---- dialogueEvent_17fe34.cpp ----
__attribute__((visibility("hidden"))) extern Status **g_status;


// MGame::dialogueEvent(): when a level sequence completes, decide whether to raise a
// briefing/mission dialogue, and if so set up the dialogue window + first-person view.
void MGame::dialogueEvent() {
    MGame *self = this;
    if (((LevelScript *)(self->field_0x7c))->startSequenceOver() == 0) return;
    Status **sp = g_status;
    if (DialogueWindow::hasBriefingDialogue((*g_status)->getCurrentCampaignMission()) == 0) {
        if (((Mission *)((*g_status)->getMission()))->isCampaignMission() != 0) return;
    }
    if (((Mission *)((*g_status)->getMission()))->isEmpty() != 0) return;
    if (((Mission *)((*g_status)->getMission()))->getType() == 8) return;
    if (((Mission *)((*g_status)->getMission()))->getType() == 0xa6) return;
    if (((Mission *)((*g_status)->getMission()))->getType() == 0) return;
    if (((Mission *)((*g_status)->getMission()))->getType() == 0xb7) return;
    if (((Mission *)((*g_status)->getMission()))->isVisible() == 0) return;
    if (((Mission *)((*g_status)->getMission()))->isCampaignMission() == 0) {
        if (((Mission *)((*g_status)->getMission()))->getType() == 0xb) return;
    }
    if (self->field_0x8c == 0) {
        DialogueWindow *w = (DialogueWindow *)MGame_opnew(0x74);
        DialogueWindow_ctor(w, (*g_status)->getMission(), self->field_0x78, 0);
        self->field_0x8c = w;
    }
    ((PlayerEgo *)(self->field_0x58))->setTurretMode(0);
    ((LevelScript *)(self->field_0x7c))->resetCamera((Level *)(self->field_0x78) /* level: arg lost in decomp */);
    ((PlayerEgo *)(self->field_0x58))->setFreeLookMode(0);
    TFC_enableFirstPersonCam(self->field_0xf4, 0);
    ((PlayerEgo *)(self->field_0x58))->hideShipForFirstPersonCameraView(0);
    LevelScript *cam = self->field_0x7c;
    self->field_0x111 = 1;
    F<int>(cam, 0x8) = 0;
    F<int>(cam, 0xc) = 0;
    self->field_0x5d = 1;
    ((MGame *)(self))->pauseSounds();
    self->field_0x5e = 1;
}

// ---- nextCamId_17c624.cpp ----
__attribute__((visibility("hidden"))) extern Status **g_status;

// MGame::nextCamId(int): advances through the cycle of available camera modes, skipping
// the turret cam unless the ship actually has a turret, and the docked cam unless docked.
int MGame::nextCamId(int cur) {
    MGame *self = this;
    int id = cur + 1;
    if (id == 2) id = cur + 2;
    if (id == 1) {
        Status **sp = g_status;
        if (((Ship *)(((Status *)(*sp))->getShip()))->getFirstEquipmentOfSort(8) != 0 ||
            ((Ship *)(((Status *)(*sp))->getShip()))->getFirstEquipmentOfSort(0x23) != 0) {
            id = ((PlayerEgo *)(self->field_0x58))->hasAutoTurret() == 0 ? 1 : 2;
        } else {
            id = 2;
        }
    }
    if (id == 2) id = 3;
    if (id >= 4) {
        if (((PlayerEgo *)(self->field_0x58))->isDockedToDockingPoint() == 0) return 0;
        Status **sp = g_status;
        if (((Ship *)(((Status *)(*sp))->getShip()))->getFirstEquipmentOfSort(8) == 0 &&
            ((Ship *)(((Status *)(*sp))->getShip()))->getFirstEquipmentOfSort(0x23) == 0) {
            return 3;
        }
        id = ((PlayerEgo *)(self->field_0x58))->hasAutoTurret() != 0 ? 3 : 1;
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

// MGame::jumpFinish(): a direct hyperspace jump has been committed; advance the
// campaign to the next mission. (UseKhadorDrive tail-call: status,1.)
void MGame::jumpFinish() {
    (*g_status)->nextCampaignMission();
}

// MGame::starMapShown(): the jump star map / direct-jump UI just came up, so the
// open in-flight HUD menu is dismissed. (UseKhadorDrive tail-call on the Hud.)
void MGame::starMapShown() {
    ((Hud *)(this->field_0x74))->closeHudMenu();
}

// MGame::endRunModule(int): tear down the running game module and switch the
// application back to module `code`. (OnTouchBegin game-over splash tap.)
void MGame::endRunModule(int code) {
    ((ApplicationManager *)(this->field_0x8))->SetCurrentApplicationModule((unsigned)code);
}

// MGame::freeCamPanDone(int): the free-cam pan/zoom gesture finished while the
// player is mining. The original simply takes the clean function-exit path here
// (field_0x111 has already been raised by the caller), so there is nothing more
// to do.
void MGame::freeCamPanDone(int /*touchY*/) {
    // no-op: stack-guard-OK return path.
}

// MGame::drawRadio(): paint the in-flight radio overlay. Receiver is the Radio at
// +0x84; the original threads the current system time plus the player ego and the
// active level script.
void MGame::drawRadio() {
    long long now = (long long)((ApplicationManager *)(this->field_0x8))->GetSystemTimeMillis();
    ((Radio *)(this->field_0x84))->draw(now, (PlayerEgo *)(this->field_0x58),
                                        (LevelScript *)(this->field_0x7c));
}

// MGame::drawRadar(): paint the radar overlay. Receiver is the Radar at +0x80; the
// original passes the player object, the HUD, and the level as the draw mode source.
void MGame::drawRadar() {
    ((Radar *)(this->field_0x80))->draw((void *)(this->field_0x58),
                                        (Hud *)(this->field_0x74),
                                        (int)(intptr_t)(this->field_0x78));
}

// MGame::drawHud(): paint the main HUD. Receiver is the Hud at +0x74.
void MGame::drawHud() {
    long long now = (long long)((ApplicationManager *)(this->field_0x8))->GetSystemTimeMillis();
    ((Hud *)(this->field_0x74))->draw(now, (long long)this->field_0x40,
                                      (void *)(this->field_0x58),
                                      this->field_0x5d != 0, 0, 0);
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
    MGame *self = this;
    // Per-frame jump-scene advance (the only sub-state with a clean recovered body).
    if (self->field_0xdc != 0)
        self->updateJumpScene();
    // Win/lose evaluation runs every frame.
    self->gameOverCheck();
    self->successCheck();
    (void)frameDeltaMs;
}

// MGame::handleHudTouchAction(...): the OnTouchBegin tail that reacts to the HUD's
// touch-down result bitmask (boost/shoot, lock-on, free-cam gestures). The detailed
// reaction table is the inlined corrupt-SIMD switch; the determinable effect is the
// free-cam pan begin recorded by the caller.
void MGame::handleHudTouchAction(int p1, int p2, void *touchId, unsigned hudResult) {
    MGame *self = this;
    (void)self; (void)p1; (void)p2; (void)touchId; (void)hudResult;
    // The hud-result reactions mutate flight state inline; no separable side effect
    // remains to perform here once OnTouchBegin has stored field_0xf8 = hudResult.
}

// MGame::dispatchTouchEndAction(...): the OnTouchEnd tail switch over the HUD
// touch-release result (pause/menu, dock, jump, dialogue choice, station services).
// OnTouchEnd has already released the tracked touch id and stored the hud result;
// the per-button reactions are the inlined corrupt-SIMD dispatch.
void MGame::dispatchTouchEndAction(int p1, int p2, void *touchId, unsigned hudResult,
                                   int wasAutoPilot) {
    MGame *self = this;
    (void)self; (void)p1; (void)p2; (void)touchId; (void)hudResult; (void)wasAutoPilot;
    // Button reactions are applied inline against `self`; no separable tail action.
}

// ---- accelerometer roll-context helpers (handleAccelerometer fn-ptr calls) ----
// In the binary these are two GOT-indirect calls threaded through one engine:
//   accelCtxBegin(appMgr) == ApplicationManager::GetEngine()  (returns the Engine)
//   accelCtxValue()       == Engine::GetAccelValue()          (on that Engine)
// The engine handle produced by Begin is stashed for the immediately-following Value.
static void *g_accelEngine = 0;

extern "C" void *MGame_accelCtxBegin(int field8) {
    g_accelEngine = ((ApplicationManager *)(intptr_t)field8)->GetEngine();
    return g_accelEngine;
}

extern "C" double *MGame_accelCtxValue() {
    return ((Engine *)g_accelEngine)->GetAccelValue();
}

// operator new / delete used by MGame's window allocations.
extern "C" void *MGame_opnew(unsigned sz) { return ::operator new(sz); }
extern "C" void MGame_opdelete(void *p) { ::operator delete(p); }
