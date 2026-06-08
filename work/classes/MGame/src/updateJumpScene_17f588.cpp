#include "class.h"

struct PlayerEgo;
struct Station;
struct Player;
struct Vector;
struct LevelScript;
struct Level;

extern "C" int Level_getLandmarks(Level *l);                             // 0x725b0
extern "C" int PlayerJumpgate_timeToJump(int jg);                        // 0x785c8
extern "C" int PaintCanvas_TransformGetTransform(unsigned cam);          // 0x72088
extern "C" void Matrix_rotateVector(void *out, Vector *v);              // 0x6f694
extern "C" void Vector_assign(Vector *dst, Vector *src);               // 0x6eb3c
extern "C" void TFC_translate(void *cam, int x, int y, int z);         // 0x72688
extern "C" void AEGeometry_getPosition(int geo, void *out);            // 0x720b8
extern "C" void Transform_Update(unsigned t, int speedFlag);           // 0x6f7cc
extern "C" void *TFC_getPosition(void *cam);                           // 0x76b28
extern "C" void PlayerJumpgate_activate(int jg);                       // 0x785d4
extern "C" void PlayerEgo_getPosition(void *out, PlayerEgo *p);        // 0x7264c
extern "C" void FModSound_setProp(int snd, int id);                    // fn @0x18f724
extern "C" void FModSound_play(int snd, int a, int b, float c);        // 0x71548
extern "C" void PlayerEgo_setSpeed(PlayerEgo *p, float s);             // 0x76b40
extern "C" void PlayerEgo_setVisible(PlayerEgo *p, int v);             // 0x76aec
extern "C" void PlayerEgo_setExhaustVisible(PlayerEgo *p, int v);      // 0x72af0
extern "C" int PlayerJumpgate_animationEnded(int jg);                  // 0x785e0
extern "C" int Status_getCurrentCampaignMission();                     // 0x71770
extern "C" int Status_inAlienOrbit();                                  // 0x723d0
extern "C" void LevelScript_setEvent(LevelScript *s, int ev);          // 0x785ec
extern "C" void PlayerEgo_setPosition(PlayerEgo *p, int x, int y, int z); // 0x72cb8
extern "C" void PlayerEgo_resetChargingDrive(PlayerEgo *p);            // 0x785f8
extern "C" void Status_departStation(Station *s);                     // 0x745fc
extern "C" void Level_setInitStreamOut(int level);                    // 0x74608
extern "C" void Status_jumpgateUsed(Station *s);                      // 0x74614
extern "C" int Station_equals(Station *a, Station *b);                // 0x73678
extern "C" void Status_setStation(Station *s);                        // 0x7366c
extern "C" int Player_getHitpoints(Player *p);                        // 0x724f0
extern "C" int Player_getShieldHP(Player *p);                         // 0x726c4
extern "C" int Player_getArmorHP(Player *p);                          // 0x72928
extern "C" int Player_getGammaHP(Player *p);                          // 0x72814
extern "C" int PlayerEgo_getCurrentSecondaryWeaponIndex(PlayerEgo *p); // 0x76d20
extern "C" void ApplicationManager_SetCurrentApplicationModule(unsigned m); // 0x71d64

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
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

// MGame::updateJumpScene(): advance the hyperspace cinematic each frame.
extern "C" void MGame_updateJumpScene(MGame *self) {
    int *guard = g_ujGuard;
    int g0 = *guard;
    bool fadeOut = true;

    if (F<uint8_t>(self, 0xdd) != 0 && F<int>(self, 0x114) != 0) {
        int tr = PaintCanvas_TransformGetTransform(**g_ujCanvasA);
        int prog = *(int *)((char *)tr + 0x114);
        int over = (*(unsigned *)((char *)tr + 0x110) > 0x6a4);
        if ((0 - over) - prog < 0) goto camMove;
    } else {
        int lm = Level_getLandmarks(F<Level *>(self, 0x78));
        if (*(int *)((char *)*(int *)((char *)lm + 4) + 4) != 0) {
            Level_getLandmarks(F<Level *>(self, 0x78));
            int jg = *(int *)((char *)*(int *)((char *)lm + 4) + 4);
            if (PlayerJumpgate_timeToJump(jg) == 0) goto camMove;
        }
    }
    fadeOut = true;
    goto afterCam;

camMove: {
        int speed = F<int>(self, 0x40);
        int ego = *(int *)((char *)F<int>(self, 0x58));
        float mtx[4];
        Matrix_rotateVector(mtx, (Vector *)((char *)ego + 4));
        Vector_assign((Vector *)((char *)self + 0xe4), (Vector *)mtx);
        TFC_translate(F<void *>(self, 0xf4), 0, 0, 0);
        (void)speed;
        if (F<uint8_t>(self, 0xdd) != 0) {
            AEGeometry_getPosition(F<int>(self, 0x114), mtx);
        } else {
            int lm = Level_getLandmarks(F<Level *>(self, 0x78));
            void *obj = *(void **)((char *)*(int *)((char *)lm + 4) + 4);
            void *vt = *(void **)obj;
            (*(void (**)(void *, void *))((char *)vt + 0x28))(obj, mtx);
        }
        Vector_assign((Vector *)((char *)self + 0xe4), (Vector *)mtx);
        fadeOut = false;
    }

afterCam:
    if (F<uint8_t>(self, 0xdd) != 0) {
        unsigned tr = PaintCanvas_TransformGetTransform(**g_ujCanvasB);
        Transform_Update(tr, F<int>(self, 0x40));
    }

    void *camPos = TFC_getPosition(F<void *>(self, 0xf4));
    float threshold = (float)F<int>(self, 0xec) + *(float *)&g_ujZNear;
    float fVar14 = threshold;
    if (*(float *)((char *)camPos + 8) < threshold && F<uint8_t>(self, 0xdd) == 0) {
        int lm = Level_getLandmarks(F<Level *>(self, 0x78));
        PlayerJumpgate_activate(*(int *)((char *)*(int *)((char *)lm + 4) + 4));
        float p[4];
        PlayerEgo_getPosition(p, F<PlayerEgo *>(self, 0x58));
        float t2 = (float)F<int>(self, 0xec) + *(float *)&g_ujZSound;
        if (t2 <= p[2] && F<uint8_t>(self, 0xcd) == 0) {
            int *snd = g_ujSound;
            FModSound_setProp(*snd, *(int *)((char *)F<int>(self, 0x58) + 0x1c));
            FModSound_setProp(*snd, 0x8d5);
            FModSound_setProp(*snd, 0x8d4);
            FModSound_setProp(*snd, 0x23);
            FModSound_play(*snd, 0x1f, 0, 0.0f);
            F<uint8_t>(self, 0xcd) = 1;
        }
    }

    if (fadeOut) {
        PlayerEgo_setSpeed(F<PlayerEgo *>(self, 0x58), *(float *)&g_ujSpeed);
        PlayerEgo_setVisible(F<PlayerEgo *>(self, 0x58), 0);
        PlayerEgo_setExhaustVisible(F<PlayerEgo *>(self, 0x58), 0);
    }
    (void)fVar14;

    // Animation-end check.
    bool ended;
    if (F<uint8_t>(self, 0xdd) != 0) {
        int tr = PaintCanvas_TransformGetTransform(**g_ujCanvasC);
        ended = *(uint8_t *)((char *)tr + 0xed) != 0;
    } else {
        int lm = Level_getLandmarks(F<Level *>(self, 0x78));
        ended = PlayerJumpgate_animationEnded(*(int *)((char *)*(int *)((char *)lm + 4) + 4)) != 0;
    }
    if (!ended) goto done;

    {
        int *status = g_ujStatus;
        if (Status_getCurrentCampaignMission() == 0x2a && Status_inAlienOrbit() != 0) {
            LevelScript_setEvent(F<LevelScript *>(self, 0x7c), 6);
            PlayerEgo_setSpeed(F<PlayerEgo *>(self, 0x58), 0.0f);
            int lm = Level_getLandmarks(F<Level *>(self, 0x78));
            int *node = *(int **)((char *)*(int *)((char *)lm + 4) + 0xc);
            (*(void (**)(int *, int, int, int))((char *)*node + 0x48))(node, g_ujPos0, g_ujPos1, g_ujPos2);
            lm = Level_getLandmarks(F<Level *>(self, 0x78));
            void *node2 = *(void **)((char *)*(int *)((char *)lm + 4) + 0xc);
            float mtx[4];
            (*(void (**)(void *, void *))((char *)*(void **)node2 + 0x28))(node2, mtx);
            Vector_assign((Vector *)((char *)self + 0xe4), (Vector *)mtx);
            PlayerEgo_setPosition(F<PlayerEgo *>(self, 0x58), F<int>(self, 0xe4),
                                  F<int>(self, 0xe8), F<int>(self, 0xec));
            PlayerEgo *p = F<PlayerEgo *>(self, 0x58);
            *(uint8_t *)((char *)p + 0x25) = 1;
            F<uint8_t>(self, 0xdc) = 0;
            PlayerEgo_resetChargingDrive(p);
        } else {
            int **stationPtr = g_ujStation;
            Status_departStation((Station *)*status);
            Level_setInitStreamOut(F<int>(self, 0x78));
            if (F<uint8_t>(self, 0xdd) == 0)
                Status_jumpgateUsed((Station *)*status);
            if (Station_equals((Station *)*stationPtr,
                               *(Station **)((char *)*status + 0x78)) != 0) {
                **g_ujFlagA = 1;
                **g_ujFlagB = 1;
                Status_setStation((Station *)*status);
            }
            *stationPtr = 0;
            *(int *)((char *)*status + 100) = Player_getHitpoints((Player *)*(int *)((char *)F<int>(self, 0x58)));
            *(int *)((char *)*status + 0x5c) = Player_getShieldHP((Player *)*(int *)((char *)F<int>(self, 0x58)));
            *(int *)((char *)*status + 0x60) = Player_getArmorHP((Player *)*(int *)((char *)F<int>(self, 0x58)));
            *(int *)((char *)*status + 0x68) = Player_getGammaHP((Player *)*(int *)((char *)F<int>(self, 0x58)));
            *(int *)((char *)*status + 0xf4) = PlayerEgo_getCurrentSecondaryWeaponIndex(F<PlayerEgo *>(self, 0x58));
            **g_ujFlagC = 1;
            F<uint8_t>(self, 0x54) = 0;
            ApplicationManager_SetCurrentApplicationModule(F<unsigned>(self, 0x8));
        }
    }

done:
    if (*guard != g0)
        __stack_chk_fail();
}
