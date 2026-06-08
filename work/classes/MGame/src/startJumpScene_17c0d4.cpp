#include "class.h"

struct PlayerEgo;
struct Engine;
struct AEGeometry;
struct TargetFollowCamera;
struct Level;
struct PaintCanvas;
struct Vector;

extern "C" void Player_setVulnerable(void *player, int v);               // 0x7294c
extern "C" void Level_enableFog(Level *l, int on);                       // 0x7843c
extern "C" int PlayerEgo_isDockedToDockingPoint(PlayerEgo *p);           // 0x75928
extern "C" void PlayerEgo_dockToDockingPoint(PlayerEgo *p, int a, void *r); // 0x72838
extern "C" void TFC_setActive(TargetFollowCamera *c, int v);            // 0x72acc
extern "C" void TFC_setLookAtCam(TargetFollowCamera *c, int v);         // 0x72670
extern "C" float TFC_useTargetsUpVector(TargetFollowCamera *c, int v);  // 0x7267c
extern "C" void PlayerEgo_setSpeed(PlayerEgo *p, float s);              // 0x76b40
extern "C" void PlayerEgo_setDockingState(PlayerEgo *p, int v);        // 0x76ca8
extern "C" int PlayerEgo_isInTurretMode(PlayerEgo *p);                  // 0x7678c
extern "C" void PlayerEgo_setTurretMode(PlayerEgo *p, int v);          // 0x72b08
extern "C" void FModSound_stop(int snd, int id);                       // 0x724a8
extern "C" void MGame_switchCamera(MGame *self, int id);               // 0x78388
extern "C" void Hud_releaseAllKeys(void *hud);                         // 0x77a1c
extern "C" int Status_inAlienOrbit();                                  // 0x723d0
extern "C" int Status_getCurrentCampaignMission();                     // 0x71770
extern "C" void PaintCanvas_CameraSetPerspective(unsigned cam, float fov, float a, float b); // 0x72004
extern "C" void PlayerEgo_setAutoPilot(PlayerEgo *p, int v);           // 0x728d4
extern "C" void PlayerEgo_setCollide(PlayerEgo *p, int v);             // 0x76a80
extern "C" void PlayerEgo_stopBoost(PlayerEgo *p);                     // 0x72cac
extern "C" int ApplicationManager_GetEngine();                        // 0x7258c
extern "C" void Engine_SetPostEffect(Engine *e, int id, int on);      // 0x707f8
extern "C" int Level_getLandmarks(Level *l);                          // 0x725b0
extern "C" void Vector_assign(Vector *dst, Vector *src);              // 0x6eb3c
extern "C" void PlayerEgo_setPosition(PlayerEgo *p, int x, int y, float z); // 0x72cb8
extern "C" void PlayerEgo_setComputerControlled(PlayerEgo *p, int v);  // 0x76aa4
extern "C" void AEGeometry_setRotation3(void *g, int x, int y, int z); // 0x73054
extern "C" void PlayerEgo_resetMovement(PlayerEgo *p);                 // 0x78448
extern "C" void *operator_new(unsigned sz);                           // 0x6eb24
extern "C" void AEGeometry_ctor(AEGeometry *g, int id, PaintCanvas *pc, int b); // 0x7207c
extern "C" int PaintCanvas_TransformGetTransform(unsigned cam);       // 0x72088
extern "C" void Transform_SetAnimationState(int t, int a, int b);     // 0x6fd18
extern "C" void PlayerEgo_getPosition(void *out, PlayerEgo *p);       // 0x72574
extern "C" void Vector_copy(Vector *dst, Vector *src);                // fn @0x18c268 (pcVar8)
extern "C" void Vector_scale(Vector *v, float s);                     // 0x6ec74
extern "C" void Vector_addAssign(Vector *dst, Vector *src);           // 0x73534
extern "C" void AEGeometry_setPosition(AEGeometry *g, Vector *v);     // 0x72148
extern "C" void AEGeometry_setScaling(AEGeometry *g, int x, int y, int z); // 0x727b4
extern "C" void AEGeometry_setDirection(AEGeometry *g, Vector *v);    // 0x726ac
extern "C" void Matrix_rotateVector(void *mat, Vector *v);            // 0x6f694
extern "C" void FModSound_setProp(int snd, int id);                   // fn @0x18c30e (pcVar8)
extern "C" void FModSound_play(int snd, int a, int b, float c);       // 0x71548
extern "C" void TFC_setPosition(TargetFollowCamera *c, float x, float y, float z); // 0x76abc

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
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

// MGame::startJumpScene(): set up the hyperspace cinematic (docked or free variant).
extern "C" void MGame_startJumpScene(MGame *self) {
    int *guard = g_jsGuard;
    int g0 = *guard;

    Player_setVulnerable(*(void **)((char *)F<int>(self, 0x58)), 0);
    Level_enableFog(F<Level *>(self, 0x78), 0);

    if (PlayerEgo_isDockedToDockingPoint(F<PlayerEgo *>(self, 0x58)) != 0) {
        PlayerEgo_dockToDockingPoint(F<PlayerEgo *>(self, 0x58), 0, F<void *>(self, 0x80));
        TFC_setActive(F<TargetFollowCamera *>(self, 0xf4), 1);
        TFC_setLookAtCam(F<TargetFollowCamera *>(self, 0xf4), 0);
        float sp = TFC_useTargetsUpVector(F<TargetFollowCamera *>(self, 0xf4), 0);
        PlayerEgo_setSpeed(F<PlayerEgo *>(self, 0x58), sp);
        PlayerEgo_setDockingState(F<PlayerEgo *>(self, 0x58), 0);
    }
    if (PlayerEgo_isInTurretMode(F<PlayerEgo *>(self, 0x58)) != 0)
        PlayerEgo_setTurretMode(F<PlayerEgo *>(self, 0x58), 0);

    int *snd = g_jsSound;
    FModSound_stop(*snd, 0x23);
    MGame_switchCamera(self, 0);
    F<int>(self, 0x70) = g_jsHudFlag;
    Hud_releaseAllKeys(F<void *>(self, 0x74));
    F<uint8_t>(self, 0x110) = 0;
    F<uint8_t>(self, 0x5c) = 0;

    PaintCanvas *pc = *g_jsCanvas;
    unsigned cam = F<unsigned>(self, 0xf0);
    float fov = *(float *)&g_jsFovDefault;
    if (Status_inAlienOrbit() != 0) {
        int cm = Status_getCurrentCampaignMission();
        fov = (cm < 0x50) ? *(float *)&g_jsFovAlienB : *(float *)&g_jsFovAlienA;
    }
    PaintCanvas_CameraSetPerspective(cam, fov, *(float *)&g_jsHudFlag, 0);
    PlayerEgo_setAutoPilot(F<PlayerEgo *>(self, 0x58), 0);
    F<uint8_t>(self, 0x5d) = 0;
    F<uint8_t>(self, 0xd6) = 0;
    F<uint8_t>(self, 0xdc) = 1;
    F<uint8_t>(self, 0x5f) = 1;
    PlayerEgo_setCollide(F<PlayerEgo *>(self, 0x58), 0);
    TFC_setLookAtCam(F<TargetFollowCamera *>(self, 0xf4), 1);
    PlayerEgo_stopBoost(F<PlayerEgo *>(self, 0x58));
    Engine *eng = (Engine *)ApplicationManager_GetEngine();
    Engine_SetPostEffect(eng, g_jsPostEffect, 0);

    float camX, camY, camZ;
    if (F<uint8_t>(self, 0xdd) == 0) {
        // Free-space jump: position from a landmark.
        int lm = Level_getLandmarks(F<Level *>(self, 0x78));
        void *obj = *(void **)((char *)lm + 4);
        void *vt = *(void **)obj;
        float vtmp[4];
        (*(void (**)(void *, void *))((char *)vt + 0x28))(obj, vtmp);
        Vector_assign((Vector *)((char *)self + 0xe4), (Vector *)vtmp);
        float nz = (float)F<int>(self, 0xec) + *(float *)&g_jsOffsetZ;
        F<int>(self, 0xec) = (int)nz;
        PlayerEgo_setPosition(F<PlayerEgo *>(self, 0x58), F<int>(self, 0xe4),
                              F<int>(self, 0xe8), nz);
        PlayerEgo_setComputerControlled(F<PlayerEgo *>(self, 0x58), 1);
        AEGeometry_setRotation3(*(void **)((char *)F<int>(self, 0x58) + 8), 0, 0, 0);
        F<int>(self, 0xe4) = (int)((float)F<int>(self, 0xe4) + *(float *)&g_jsOffsetX);
        F<int>(self, 0xe8) = (int)((float)F<int>(self, 0xe8) + *(float *)&g_jsOffsetY);
        F<int>(self, 0xec) = (int)((float)F<int>(self, 0xec) + *(float *)&g_jsOffsetZ2);
        camX = (float)F<int>(self, 0xe4);
        camY = (float)F<int>(self, 0xe8);
        camZ = (float)F<int>(self, 0xec);
    } else {
        // Charged jump: spawn the warp-tunnel geometry.
        PlayerEgo_resetMovement(F<PlayerEgo *>(self, 0x58));
        PlayerEgo_setComputerControlled(F<PlayerEgo *>(self, 0x58), 1);
        AEGeometry *geo = (AEGeometry *)operator_new(0xc0);
        AEGeometry_ctor(geo, 0x3ab2, *g_jsCanvas, 0);
        F<AEGeometry *>(self, 0x114) = geo;
        int tr = PaintCanvas_TransformGetTransform(*(unsigned *)g_jsCanvas);
        Transform_SetAnimationState(tr, 1, 0);

        float pos[4];
        PlayerEgo_getPosition(pos, F<PlayerEgo *>(self, 0x58));
        Vector *dst = (Vector *)((char *)self + 0xe4);
        Vector_copy(dst, (Vector *)pos);

        float dir[4];
        PlayerEgo_getPosition(dir, F<PlayerEgo *>(self, 0x58));
        Vector_scale((Vector *)dir, *(float *)&g_jsOffsetX);
        Vector_addAssign(dst, (Vector *)dir);
        AEGeometry_setPosition(F<AEGeometry *>(self, 0x114), dst);

        AEGeometry_setScaling(F<AEGeometry *>(self, 0x114), 0x40000000, 0x40000000, 0x40000000);
        float zero[4]; zero[0] = 0; *(int *)&zero[1] = 0x3f800000; zero[2] = 0;
        AEGeometry_setDirection(F<AEGeometry *>(self, 0x114), (Vector *)zero);

        float off[4]; off[0] = (float)g_jsOffsetX; off[1] = (float)g_jsOffsetY; off[2] = (float)g_jsOffsetX;
        Vector_copy((Vector *)off, (Vector *)off);
        Matrix_rotateVector(off, (Vector *)((char *)*(int *)((char *)F<int>(self, 0x58)) + 4));
        Vector_copy((Vector *)off, (Vector *)off);
        Vector_addAssign(dst, (Vector *)off);

        FModSound_setProp(*snd, F<int>(self, 0x1c));
        FModSound_setProp(*snd, 0x23);
        FModSound_setProp(*snd, 0x8d5);
        FModSound_setProp(*snd, 0x8d4);
        FModSound_play(*snd, 0x20, 0, 0.0f);

        camX = (float)F<int>(self, 0xe4);
        camZ = (float)F<int>(self, 0xe8);
        camY = (float)F<int>(self, 0xec);
    }
    TFC_setPosition(F<TargetFollowCamera *>(self, 0xf4), camX, camY, camZ);

    if (*guard != g0)
        __stack_chk_fail();
}
