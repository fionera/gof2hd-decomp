#include "class.h"

// PlayerFixedObject::update(int dt). Large state machine on this->0x88:
//   normal -> dying(3) -> exploding(4) -> dead(5). Heavily optimized in the target
//   (the FPSCR shuffles in the Ghidra output are just float comparisons). This is a
//   faithful structural translation; engine subsystems are reached via extern helpers.

extern "C" void Player_update(void *player, bool b);
extern "C" int  Status_getStanding();
extern "C" unsigned char Standing_isEnemy(void *st);
extern "C" unsigned char Standing_isFriend(void *st, int kind);
extern "C" int  Player_turnedEnemy();
extern "C" int  Player_isAlwaysFriend();

extern "C" float Player_getBombForce();
extern "C" float Player_getEmpForce(void *player);
extern "C" void  Player_setBombForce(float f);
extern "C" void  Player_setEmpForce(void *player, float f);

extern "C" void  PlayerFixedObject_moveForward(PlayerFixedObject *self, int amount);
extern "C" void  PlayerFixedObject_setExhaustVisible(PlayerFixedObject *self, bool v);

extern "C" int   Status_getCurrentCampaignMission();
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, int id);
extern "C" void  Transform_Update(void *t, bool b);
extern "C" void  Transform_SetAnimationState(void *t, int a, int b);

extern "C" int   Player_getHitpoints();
extern "C" void  Level_friendDied();
extern "C" void  Level_enemyDied(int lod, bool kind);
extern "C" void  Level_pirateStationAction(bool a);
extern "C" int   KIPlayer_cargoAvailable();
extern "C" void  KIPlayer_createCrate(PlayerFixedObject *self);

extern "C" void *AEGeometry_getMatrix(void *geom);
extern "C" void  AEGeometry_setMatrix(void *geom, void *m);
extern "C" void  AEGeometry_getPosition(Vector *out, void *geom);
extern "C" void  AEGeometry_addChild(void *geom, void *child);
extern "C" float AEGeometry_moveForward_ret(void *geom, float d);
extern "C" void  AEGeometry_rotate(void *geom, float x, float y, float z);
extern "C" void *Matrix_assign(void *dst, void *src);
extern "C" void *Vector_assign(Vector *dst, Vector *src);

extern "C" void *ParticleSystemManager_systemSetMatrix(void *mgr, int sys, void *m);
extern "C" void  ParticleSystemManager_enableSystemEmit(void *mgr, bool e);
extern "C" void  FModSound_play(void *snd, int id, Vector *pos, float v);

extern "C" void *Explosion_ctor(void *e, int a);
extern "C" void  Explosion_addFireStreaks(void *e);
extern "C" void  Explosion_start(void *e, Vector *pos);
extern "C" void  Explosion_update(void *e, int dt, void *cam);
extern "C" void  Explosion_reset(void *e);
extern "C" void  Explosion_setScaling(void *e, float s);
extern "C" int   Explosion_isPlaying(void *e);
extern "C" void *operator_new(uint32_t);

extern "C" void *Level_getPlayer();
extern "C" void *PlayerEgo_getTargetFollowCamera(void *ego);
extern "C" void  TargetFollowCamera_getPosition(Vector *out, void *cam);
extern "C" void  TargetFollowCamera_setRumblePercentage(float pct, void *cam);
extern "C" void  Vector_sub(Vector *a, Vector *b);
extern "C" float Vector_length(Vector *v);

extern "C" void *Player_getEnemies();
extern "C" int   Player_isActive();
extern "C" void  Player_getPosition(Vector *out);
extern "C" void *Player_getEnemy(void *player, unsigned int idx);
extern "C" void  Player_setActive(bool a);
extern "C" void  KIPlayer_setActive(bool a);

extern "C" void *Level_getEnemies();
extern "C" void  Player_damage(void *player, int dmg);
extern "C" void *PlayerEgo_getHUD(void *ego);
extern "C" int   Achievements_hasMedal(void *a, int id, int n);
extern "C" int   Achievements_getValue(void *a, int id, int n);
extern "C" void  Hud_hudEventMedal(void *hud, int id, int v);

extern "C" void  PaintCanvas_MaterialCreate(void *pc, unsigned short mat, void *out);
extern "C" void  PaintCanvas_MeshChangeMaterial(void *pc, unsigned int mesh, unsigned short mat);

// PC-relative singleton holders.
__attribute__((visibility("hidden"))) extern void **g_pfo_canvasU;   // PaintCanvas for Transform updates
__attribute__((visibility("hidden"))) extern void **g_pfo_fmod;      // FModSound holder
__attribute__((visibility("hidden"))) extern void **g_pfo_audioFlag; // *holder+0xf = positional flag
__attribute__((visibility("hidden"))) extern void **g_pfo_egoA;      // PlayerEgo holder (achievements path)
__attribute__((visibility("hidden"))) extern void **g_pfo_achievements;
__attribute__((visibility("hidden"))) extern void **g_pfo_pcMaterial;
__attribute__((visibility("hidden"))) extern const int g_pfo_dmgVal; // DAT_00164c90

static inline bool typeIsPirateOrE(PlayerFixedObject *self) {
    int k = self->f_ac;
    return k == 0x37a3 || k == 0xe;
}

extern "C" void PlayerFixedObject_update(PlayerFixedObject *self, int dt)
{
    self->f_130 = dt;
    bool bdt = (bool)(unsigned char)dt;

    // ship's KIPlayer "is active for tutorial" flag derived from 0xf8/0x134
    bool kiFlag = (self->f_f8 + 1 != 0) && (self->f_134 != 0);
    Player_update(self->f_4, kiFlag);

    void *player = self->f_4;
    unsigned char enemyFlag = 0;
    if ((self->f_28 & 0xfffffffe) == 8) {
        *(unsigned char *)((char *)player + 0x5c) = 1;
        enemyFlag = 0;
    } else {
        int st = Status_getStanding();
        unsigned char e = Standing_isEnemy((void *)(long)st);
        player = self->f_4;
        *(unsigned char *)((char *)player + 0x5c) = e;
        if ((self->f_28 & 0xfffffffe) == 8) {
            enemyFlag = 0;
        } else {
            void *st2 = (void *)(long)Status_getStanding();
            enemyFlag = Standing_isFriend(st2, self->f_28);
            player = self->f_4;
        }
    }
    *(unsigned char *)((char *)player + 0x5d) = enemyFlag;

    if (Player_turnedEnemy() != 0)
        *(unsigned short *)(*(char **)((char *)self + 0x4) + 0x5c) = 1;
    if (Player_isAlwaysFriend() != 0)
        *(unsigned short *)(*(char **)((char *)self + 0x4) + 0x5c) = 0x100;

    if (self->f_88 != 6) {
        float bomb = Player_getBombForce();
        float emp = Player_getEmpForce(self->f_4);
        if (bomb > 0.0f) {
            float nb = bomb * 0.95f; // DAT decay factor
            if (nb < 1.0f) nb = 0.0f;
            Player_setBombForce(nb);
        }
        if (emp > 0.0f) {
            float ne = emp - (float)dt;
            float clamped = ne;
            if (ne < 1.0f) clamped = 0.0f;
            self->f_24 = (ne >= 1.0f) ? 1 : 0;
            Player_setEmpForce(self->f_4, clamped);
        }
    }

    if (self->f_24 == 0 && (unsigned int)(self->f_88 - 3) >= 2) {
        int kind = self->f_ac;
        bool doMove = (kind != 0x37a3);
        if (doMove) doMove = (self->f_134 != 0);
        if (doMove) PlayerFixedObject_moveForward(self, dt);

        int cm = Status_getCurrentCampaignMission();
        int k2 = self->f_ac;
        bool skip = (cm == 0x5b && k2 == 0x494e);
        if (!skip) {
            if (k2 == 0x494a) {
                if (Status_getCurrentCampaignMission() == 0x91) goto afterMotion;
                k2 = self->f_ac;
            }
            if (k2 != 0x4220) {
                void *t = PaintCanvas_TransformGetTransform(*g_pfo_canvasU,
                            *(int *)((char *)self->f_8 + 0xc));
                Transform_Update(t, bdt);
            }
        }
    }
afterMotion:

    if (Player_getHitpoints() < 1 && (unsigned int)(self->f_88 - 3) >= 2) {
        // ---- death transition ----
        if (*(char *)(*(char **)((char *)self + 0x4) + 0x5c) == 0) {
            Level_friendDied();
        } else {
            Level_enemyDied(self->f_54, (bool)(unsigned char)self->f_ac);
        }
        if (self->f_ac == 0x37a3)
            Level_pirateStationAction((bool)(unsigned char)self->f_54);

        self->f_134 = 0;
        self->f_88 = 3;
        int cargo = KIPlayer_cargoAvailable();
        self->f_4c = (unsigned char)cargo;
        if (cargo != 0) KIPlayer_createCrate(self);
        PlayerFixedObject_setExhaustVisible(self, false);

        void *wreck = self->f_124;
        AEGeometry_setMatrix(wreck, AEGeometry_getMatrix(self->f_8));
        wreck = self->f_124;

        void *expl;
        if (wreck != 0) {
            AEGeometry_setMatrix(wreck, AEGeometry_getMatrix(self->f_8));
            void *t = PaintCanvas_TransformGetTransform(*g_pfo_canvasU,
                        *(int *)((char *)wreck + 0xc));
            Transform_SetAnimationState(t, 1, 0);
            if (self->f_28 == 3 && self->f_134 != 0 &&
                *(int *)(*(char **)((char *)self + 0x8) + 0x10) != -1) {
                AEGeometry_addChild(self->f_8, self->f_124);
            }
        }

        // particle + sound for the explosion (shared tail for both branches)
        void *lod = self->f_54;
        void *mgr = *(void **)((char *)lod + 0x74);
        int sysOff = typeIsPirateOrE(self) ? 0x54 : 0x50;
        int sys = *(int *)((char *)lod + sysOff);
        void *m = AEGeometry_getMatrix(self->f_8);
        void *sndHandle = ParticleSystemManager_systemSetMatrix(mgr, sys, m);
        Vector *pos = 0;
        if (*(char *)((char *)*g_pfo_audioFlag + 0xf) != 0)
            pos = (Vector *)((char *)self + 0x2c);
        FModSound_play(*g_pfo_fmod, 0x14, pos, (float)(long)sndHandle);
        lod = self->f_54;
        ParticleSystemManager_enableSystemEmit(*(void **)((char *)lod + 0x74),
            (bool)(unsigned char)*(int *)((char *)lod + (typeIsPirateOrE(self) ? 0x54 : 0x50)));

        expl = operator_new(0x68);
        Explosion_ctor(expl, 0);
        self->f_18c = expl;
        Explosion_addFireStreaks(expl);
        expl = self->f_18c;

        char posBuf[12];
        AEGeometry_getPosition((Vector *)posBuf, self->f_8);
        Explosion_start(expl, (Vector *)posBuf);

        if (self->f_ac == 0xe) {
            unsigned int *enemies = (unsigned int *)Level_getEnemies();
            for (unsigned int i = 0; i < enemies[0]; i++) {
                void *en = Level_getEnemies();
                void *obj = *(void **)(*(int *)((char *)en + 4) + i * 4);
                if (*(char *)((char *)obj + 0x3e) != 0) {
                    en = Level_getEnemies();
                    obj = *(void **)(*(int *)((char *)en + 4) + i * 4);
                    Player_damage(*(void **)((char *)obj + 4), g_pfo_dmgVal);
                }
                enemies = (unsigned int *)Level_getEnemies();
            }
            if (self->f_ac == 0xe &&
                *(char *)(*(char **)((char *)self + 0x4) + 0x44) == 0) {
                void *egoObj = *g_pfo_egoA;
                void *ach = *g_pfo_achievements;
                *(int *)((char *)egoObj + 0x118) = *(int *)((char *)egoObj + 0x118) + 1;
                if (Achievements_hasMedal(ach, 0x27, 1) == 0) {
                    float cur = (float)*(int *)((char *)egoObj + 0x118);
                    float val = (float)Achievements_getValue(ach, 0x27, 1);
                    if ((int)(cur / val) < 2) {
                        void *ego = Level_getPlayer();
                        void *hud = PlayerEgo_getHUD(ego);
                        cur = (float)*(int *)((char *)egoObj + 0x118);
                        val = (float)Achievements_getValue(ach, 0x27, 1);
                        Hud_hudEventMedal(hud, 0x27, (int)((cur / val) * 100.0f));
                    }
                }
            }
        }
    }

    int state = self->f_88;
    if (state == 3) {
        // dying: run explosion, drift, advance the wreck transform until done
        if (self->f_18c != 0)
            Explosion_update(self->f_18c, dt, 0);
        if (self->f_ac != 0x37a3) {
            if (self->f_134 != 0) {
                self->f_180 = self->f_180 + dt;
                float d = AEGeometry_moveForward_ret(self->f_124, (float)dt);
                if (self->f_78 != 0)
                    AEGeometry_moveForward_ret(self->f_78, d);
            }
            void *m = AEGeometry_getMatrix(self->f_124);
            Matrix_assign(*(char **)((char *)self + 0x4) + 0x4, m);
            char posBuf[12];
            AEGeometry_getPosition((Vector *)posBuf, self->f_124);
            Vector_assign((Vector *)((char *)self + 0x2c), (Vector *)posBuf);
            Array<void *> *bv = F<Array<void *> *>(self, 0x128);
            if (bv != 0) {
                for (unsigned int i = 0; i < bv->length; i++) {
                    void *o = bv->data[i];
                    typedef void (*BVFn)(void *, float, float, float);
                    BVFn fn = *(BVFn *)(*(char **)o + 0x4);
                    fn(o, self->f_2c, self->f_30, self->f_34);
                    bv = F<Array<void *> *>(self, 0x128);
                }
            }
        }
        self->f_101 = 0;
        void *t = PaintCanvas_TransformGetTransform(*g_pfo_canvasU,
                    *(int *)((char *)self->f_124 + 0xc));
        Transform_Update(t, bdt);
        t = PaintCanvas_TransformGetTransform(*g_pfo_canvasU,
                    *(int *)((char *)self->f_124 + 0xc));
        if (*(char *)((char *)t + 0xed) == 0) {
            void *lod = self->f_54;
            self->f_88 = 4;
            ParticleSystemManager_enableSystemEmit(*(void **)((char *)lod + 0x74),
                (bool)(unsigned char)*(int *)((char *)lod + (typeIsPirateOrE(self) ? 0x54 : 0x50)));
            Explosion_reset(self->f_18c);
            float scale = 6.0f;
            if (self->f_ac == 0x37e7) scale = 8.0f;
            if (self->f_ac == 0x37a3) scale = 8.0f;
            Explosion_setScaling(self->f_18c, scale);
            Explosion_start(self->f_18c, (Vector *)((char *)self + 0x2c));
            self->f_198 = 1;
            self->f_190 = 0;
            if (Level_getPlayer() != 0) {
                void *ego = Level_getPlayer();
                if (PlayerEgo_getTargetFollowCamera(ego) != 0) {
                    ego = Level_getPlayer();
                    void *cam = PlayerEgo_getTargetFollowCamera(ego);
                    char cp[12];
                    TargetFollowCamera_getPosition((Vector *)cp, cam);
                    Vector_sub((Vector *)cp, (Vector *)((char *)self + 0x2c));
                    float len = Vector_length((Vector *)cp);
                    float maxd = 50.0f;
                    float use = (len < maxd) ? len : maxd;
                    self->f_19c = 1.0f - use / maxd;
                    ego = Level_getPlayer();
                    cam = PlayerEgo_getTargetFollowCamera(ego);
                    TargetFollowCamera_setRumblePercentage(self->f_19c, cam);
                }
            }
        }
    } else if (state == 4) {
        // exploding
        self->f_190 = self->f_190 + dt;
        if (self->f_18c != 0)
            Explosion_update(self->f_18c, dt, 0);
        self->f_d8 = self->f_d8 + dt;

        bool spin = self->f_4c != 0 && Player_isActive() != 0 &&
                    self->f_78 != 0;
        if (spin) {
            float r = (float)(dt >> 1) * 0.001f; // DAT scalings
            r = (float)(int)(r * 1.0f);
            AEGeometry_rotate(self->f_78, r, r, r);
            if (self->f_d8 >= 0xea61) {
                KIPlayer_setActive((bool)(unsigned char)(long)self);
                self->f_101 = 1;
            }
        } else {
            if (self->f_18c != 0 && Explosion_isPlaying(self->f_18c) == 0) {
                if (self->f_d8 >= 0xea61)
                    KIPlayer_setActive((bool)(unsigned char)(long)self);
                self->f_101 = 1;
            }
        }

        if (self->f_194 >= 0) {
            if (self->f_12c != 0 && self->f_190 >= 0x8d &&
                (unsigned int)self->f_1a0 <= 0x7fffffff) {
                char posBuf[12];
                AEGeometry_getPosition((Vector *)posBuf, self->f_8);
                Vector_assign((Vector *)((char *)self + 0x2c), (Vector *)posBuf);
                Array<void *> *bv = F<Array<void *> *>(self, 0x12c);
                for (unsigned int i = 0; i < bv->length; i++) {
                    void *o = bv->data[i];
                    typedef void (*BVFn)(void *, float, float, float);
                    BVFn fn = *(BVFn *)(*(char **)o + 0x4);
                    fn(o, self->f_2c, self->f_30, self->f_34);
                }
                unsigned short mat;
                switch ((unsigned int)self->f_194) {
                    case 1: mat = 0x8248; break;
                    case 2: mat = 0x8249; break;
                    case 3: mat = 0x824a; break;
                    case 4: mat = 0x824b; break;
                    default: mat = 0x824d; break;
                }
                self->f_1a0 = mat;
                void *pc = *g_pfo_pcMaterial;
                char matOut[4];
                PaintCanvas_MaterialCreate(pc, mat, matOut);
                PaintCanvas_MeshChangeMaterial(pc,
                    *(unsigned int *)(*(char **)((char *)self + 0x124) + 0x1c),
                    *(unsigned short *)matOut);
            }
            // rumble ramp
            if (Level_getPlayer() != 0) {
                void *ego = Level_getPlayer();
                void *cam = PlayerEgo_getTargetFollowCamera(ego);
                if (cam != 0 && self->f_198 > 0) {
                    int v = self->f_198 + dt;
                    if (v >= 0x7d0) v = 0x7d0;
                    self->f_198 = v;
                    ego = Level_getPlayer();
                    cam = PlayerEgo_getTargetFollowCamera(ego);
                    TargetFollowCamera_setRumblePercentage(
                        self->f_19c * ((float)v / 50.0f + 1.0f), cam);
                    if (self->f_18c != 0 &&
                        Explosion_isPlaying(self->f_18c) == 0) {
                        ego = Level_getPlayer();
                        cam = PlayerEgo_getTargetFollowCamera(ego);
                        TargetFollowCamera_setRumblePercentage(0.0f, cam);
                        self->f_198 = 0;
                    }
                }
            }
        }
    } else if (state == 5) {
        // dead-but-selectable: search for a nearby active enemy to re-home on
        unsigned int *enemies = (unsigned int *)Player_getEnemies();
        if (enemies != 0) {
            self->f_168 = 0;
            for (unsigned int i = 0; i < enemies[0]; i++) {
                if (Player_isActive() != 0) {
                    char pb[12];
                    Player_getPosition((Vector *)pb);
                    Vector_assign((Vector *)((char *)self + 0x90), (Vector *)pb);
                    float dx = self->f_2c - self->f_90;
                    float dy = self->f_30 - self->f_94;
                    float dz = self->f_34 - self->f_98;
                    const float lo = 0.0f, hi = 50.0f; // DAT thresholds
                    if (dx < hi && dx > lo && dy < hi && dy > lo && dz < hi && dz > lo) {
                        self->f_168 = Player_getEnemy(self->f_4, i);
                        Player_getPosition((Vector *)pb);
                        Vector_assign((Vector *)((char *)self + 0x90), (Vector *)pb);
                        self->f_144 = self->f_90;
                        self->f_148 = self->f_94;
                        self->f_14c = self->f_98;
                        break;
                    }
                }
            }
        }
        float vx = self->f_144 - self->f_2c;
        float vy = self->f_148 - self->f_30;
        float vz = self->f_14c - self->f_34;
        self->f_150 = vx;
        self->f_154 = vy;
        self->f_158 = vz;
        const float lo = 0.0f, hi = 50.0f;
        if (vx < hi && vx > lo && vz > lo && vy < hi && vy > lo && vz < hi) {
            self->f_88 = 1;
            Player_setActive((bool)(unsigned char)(long)self->f_4);
        }
    }

    // mirror the integer position into the Player object
    void *p = self->f_4;
    *(int *)((char *)p + 0x48) = self->f_178;
    *(int *)((char *)p + 0x4c) = self->f_17c;
    *(int *)((char *)p + 0x50) = self->f_180;
}
