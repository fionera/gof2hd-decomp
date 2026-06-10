#include "PlayerFixedObject.h"


extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" void *String_assign(void *dst);
extern "C" void operator_delete(void *p);
extern "C" V3 BV_staticProjectCollisionOnSurface(void *vec, void *bvArray);
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
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas);
extern "C" void Transform_setExhaustVisible(void *transform, bool v);
extern "C" void Array_BV_ctor(void *arr);
extern "C" void BoundingVolume_setArr(BoundingVolume *bv, void *arr);
extern "C" void KIPlayer_reset(PlayerFixedObject *self);
extern "C" void AEGeometry_ctor(void *geom, uint16_t meshId, void *canvas, bool b);
extern "C" int Globals_getWreckCollision(void *globals, int kind, void *geom);
extern "C" V3 BV_getProjectionVector(void *bv);
extern "C" char PlayerFixedObject_vtable;
extern "C" void *AEGeometry_dtor(void *p);
extern "C" void ArrayReleaseClasses_BV(void *arr);
extern "C" void *Array_BV_dtor(void *p);
extern "C" void *Explosion_dtor(void *p);
extern "C" void String_dtor(void *p);
extern "C" void *PlayerFixedObject_baseDtor(PlayerFixedObject *self);
extern "C" void AEGeometry_render(void *geom);
extern "C" int Player_isActive(void *player);
extern "C" void String_ctor_empty(void *s);
extern "C" void String_ctor_cstr(void *s, const char *cstr, bool b);
extern "C" void *String_op_assign(void *dst, void *src);
extern "C" void *Status_getMission();
extern "C" int Mission_isCampaignMission(void *m);
extern "C" int Status_getCurrentCampaignMission();
extern "C" void *Status_getStation();
extern "C" int Station_getIndex(void *st);
extern "C" void Generator_ctor(void *g);
extern "C" void *Generator_dtor(void *g);
extern "C" void *Generator_getLootList(void *g, int a, int b);
extern "C" void *Array_int_dtor(void *p);
extern "C" int AERandom_nextInt(int n);
extern "C" void AEGeometry_setPosition(void *geom, float x, float y, float z);
extern "C" void AEGeometry_getPosition(Vector *out, void *geom);
extern "C" void Player_setHitpoints(void *player, int hp);
extern "C" void Player_setVulnerable(void *player, bool v);
extern "C" void LODManager_removeObject(void *lod, void *geom);
extern "C" void *AEGeometry_dtor(void *geom);
extern "C" void Transform_SetAnimationRangeInTime(void *transform, long long range);
extern "C" void AEGeometry_moveForward(void *geom, float d);

// ---- getDockingType_154ef0.cpp ----
extern "C" int PlayerFixedObject_getDockingType(PlayerFixedObject *self)
{
    return F<int>(self, 0x1a4);
}

// ---- setBV_154016.cpp ----
extern "C" void PlayerFixedObject_setBV_arr(PlayerFixedObject *self, Array<BoundingVolume *> *bv)
{
    F<Array<BoundingVolume *> *>(self, 0x128) = bv;
}

// ---- hideShip_154f1a.cpp ----
extern "C" void PlayerFixedObject_hideShip(PlayerFixedObject *self)
{
    F<uint8_t>(self, 0x1b8) = 1;
}

// ---- getTransportID_154efc.cpp ----
extern "C" int PlayerFixedObject_getTransportID(PlayerFixedObject *self)
{
    return F<int>(self, 0x1a8);
}

// ---- setDockingType_154eea.cpp ----
extern "C" void PlayerFixedObject_setDockingType(PlayerFixedObject *self, int v)
{
    F<int>(self, 0x1a4) = v;
}

// ---- setPosition_153fb4.cpp ----
typedef void (*SetPosFn)(PlayerFixedObject *, float, float, float);

extern "C" void PlayerFixedObject_setPosition_vec(PlayerFixedObject *self, const Vector &v)
{
    SetPosFn fn = *(SetPosFn *)(*(char **)self + 0x48);
    return fn(self, v.x, v.y, v.z);
}

// ---- translate_153fd0.cpp ----
typedef void (*SetPosFn)(PlayerFixedObject *, float, float, float);

// Reads ship pos (signed ints at +0x178/0x17c/0x180 -> float), adds the delta Vector,
// then forwards to vtable slot 0x48.
extern "C" void PlayerFixedObject_translate(PlayerFixedObject *self, const Vector &d)
{
    float x = (float)F<int32_t>(self, 0x178);
    float y = (float)F<int32_t>(self, 0x17c);
    float z = (float)F<int32_t>(self, 0x180);
    SetPosFn fn = *(SetPosFn *)(*(char **)self + 0x48);
    return fn(self, d.x + x, d.y + y, d.z + z);
}

// ---- getName_154f02.cpp ----
// AbyssEngine::String::String(String* out, const String* src, bool) -> void.
// Returns String by value (sret r0, this r1). Copy-ctor returns void, so the
// compiler keeps a frame + restores the sret pointer.
struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

extern "C" RetStr PlayerFixedObject_getName(PlayerFixedObject *self)
{
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0x1ac, false);
    return r;
}

// ---- setName_154f12.cpp ----
// Tail-call into AbyssEngine::String::operator= (or move-assign): dst = this+0x1ac, src = r1 (the String arg).

extern "C" void *PlayerFixedObject_setName(PlayerFixedObject *self)
{
    return String_assign((char *)self + 0x1ac);
}

// ---- _PlayerFixedObject_153e14.cpp ----
// Deleting destructor (D0): run the complete dtor (D1), then tail-call operator delete.
// Mangled names so the demangled symbol contains "~PlayerFixedObject".
extern "C" void *_ZN17PlayerFixedObjectD1Ev(PlayerFixedObject *self); // complete object dtor

extern "C" void _ZN17PlayerFixedObjectD0Ev(PlayerFixedObject *self)
{
    return operator_delete(_ZN17PlayerFixedObjectD1Ev(self));
}

// ---- setMoving_153ee4.cpp ----
extern "C" void PlayerFixedObject_setMoving(PlayerFixedObject *self, bool v)
{
    F<uint8_t>(self, 0x134) = v;
}

// ---- projectCollisionOnSurface_154e16.cpp ----
// Returns a Vector by value (sret r0, this r1, collision vector r2). The callee returns
// the projected Vector into the same sret, so the compiler keeps a frame + call (not tail).
struct V3 { float x, y, z; };

extern "C" V3 PlayerFixedObject_projectCollisionOnSurface(PlayerFixedObject *self, void *vec)
{
    void *bv = F<void *>(self, 0x12c);
    if (bv != 0 && F<int>(self, 0x88) == 4) {
        return BV_staticProjectCollisionOnSurface(vec, bv);
    }
    void *bv2 = F<void *>(self, 0x128);
    if (bv2 != 0) {
        return BV_staticProjectCollisionOnSurface(vec, bv2);
    }
    V3 z = {0.0f, 0.0f, 0.0f};
    return z;
}

// ---- setTransportID_154ef6.cpp ----
extern "C" void PlayerFixedObject_setTransportID(PlayerFixedObject *self, int v)
{
    F<int>(self, 0x1a8) = v;
}

// ---- outerCollide_154e50.cpp ----
// outerCollide(Vector) - Vector passed by value (r1,r2,r3). Pure tail-forward through
// vtable slot 0x3c: ldr r12,[r0]; ldr r12,[r12,#0x3c]; bx r12.
typedef void (*CollideFn)(PlayerFixedObject *, Vector);

extern "C" void PlayerFixedObject_outerCollide_vec(PlayerFixedObject *self, Vector v)
{
    CollideFn fn = *(CollideFn *)(*(char **)self + 0x3c);
    return fn(self, v);
}

// ---- getPosition_154056.cpp ----
// sret in r0, this in r1. Reads three signed ints at +0x178/0x17c/0x180 and converts to float.
struct PosOut { float x, y, z; };

extern "C" void PlayerFixedObject_getPosition(PosOut *out, PlayerFixedObject *self)
{
    float a = (float)F<int32_t>(self, 0x178);
    float b = (float)F<int32_t>(self, 0x17c);
    float c = (float)F<int32_t>(self, 0x180);
    out->x = a;
    out->y = b;
    out->z = c;
}

// ---- update_1541e0.cpp ----
// PlayerFixedObject::update(int dt). Large state machine on this->0x88:
//   normal -> dying(3) -> exploding(4) -> dead(5). Heavily optimized in the target
//   (the FPSCR shuffles in the Ghidra output are just float comparisons). This is a
//   faithful structural translation; engine subsystems are reached via extern helpers.













// PC-relative singleton holders.
__attribute__((visibility("hidden"))) extern void **g_pfo_canvasU;   // PaintCanvas for Transform updates
__attribute__((visibility("hidden"))) extern void **g_pfo_fmod;      // FModSound holder
__attribute__((visibility("hidden"))) extern void **g_pfo_audioFlag; // *holder+0xf = positional flag
__attribute__((visibility("hidden"))) extern void **g_pfo_egoA;      // PlayerEgo holder (achievements path)
__attribute__((visibility("hidden"))) extern void **g_pfo_achievements;
__attribute__((visibility("hidden"))) extern void **g_pfo_pcMaterial;
__attribute__((visibility("hidden"))) extern const int g_pfo_dmgVal; // DAT_00164c90

static inline bool typeIsPirateOrE(PlayerFixedObject *self) {
    int k = F<int>(self, 0xac);
    return k == 0x37a3 || k == 0xe;
}

extern "C" void PlayerFixedObject_update(PlayerFixedObject *self, int dt)
{
    F<int>(self, 0x130) = dt;
    bool bdt = (bool)(unsigned char)dt;

    // ship's KIPlayer "is active for tutorial" flag derived from 0xf8/0x134
    bool kiFlag = (F<int>(self, 0xf8) + 1 != 0) && (F<unsigned char>(self, 0x134) != 0);
    Player_update(F<void *>(self, 0x4), kiFlag);

    void *player = F<void *>(self, 0x4);
    unsigned char enemyFlag = 0;
    if ((F<unsigned int>(self, 0x28) & 0xfffffffe) == 8) {
        *(unsigned char *)((char *)player + 0x5c) = 1;
        enemyFlag = 0;
    } else {
        int st = Status_getStanding();
        unsigned char e = Standing_isEnemy((void *)(long)st);
        player = F<void *>(self, 0x4);
        *(unsigned char *)((char *)player + 0x5c) = e;
        if ((F<unsigned int>(self, 0x28) & 0xfffffffe) == 8) {
            enemyFlag = 0;
        } else {
            void *st2 = (void *)(long)Status_getStanding();
            enemyFlag = Standing_isFriend(st2, F<int>(self, 0x28));
            player = F<void *>(self, 0x4);
        }
    }
    *(unsigned char *)((char *)player + 0x5d) = enemyFlag;

    if (Player_turnedEnemy() != 0)
        *(unsigned short *)(*(char **)((char *)self + 0x4) + 0x5c) = 1;
    if (Player_isAlwaysFriend() != 0)
        *(unsigned short *)(*(char **)((char *)self + 0x4) + 0x5c) = 0x100;

    if (F<int>(self, 0x88) != 6) {
        float bomb = Player_getBombForce();
        float emp = Player_getEmpForce(F<void *>(self, 0x4));
        if (bomb > 0.0f) {
            float nb = bomb * 0.95f; // DAT decay factor
            if (nb < 1.0f) nb = 0.0f;
            Player_setBombForce(nb);
        }
        if (emp > 0.0f) {
            float ne = emp - (float)dt;
            float clamped = ne;
            if (ne < 1.0f) clamped = 0.0f;
            F<unsigned char>(self, 0x24) = (ne >= 1.0f) ? 1 : 0;
            Player_setEmpForce(F<void *>(self, 0x4), clamped);
        }
    }

    if (F<unsigned char>(self, 0x24) == 0 && (unsigned int)(F<int>(self, 0x88) - 3) >= 2) {
        int kind = F<int>(self, 0xac);
        bool doMove = (kind != 0x37a3);
        if (doMove) doMove = (F<unsigned char>(self, 0x134) != 0);
        if (doMove) PlayerFixedObject_moveForward(self, dt);

        int cm = Status_getCurrentCampaignMission();
        int k2 = F<int>(self, 0xac);
        bool skip = (cm == 0x5b && k2 == 0x494e);
        if (!skip) {
            if (k2 == 0x494a) {
                if (Status_getCurrentCampaignMission() == 0x91) goto afterMotion;
                k2 = F<int>(self, 0xac);
            }
            if (k2 != 0x4220) {
                void *t = PaintCanvas_TransformGetTransform(*g_pfo_canvasU,
                            *(int *)((char *)F<void *>(self, 0x8) + 0xc));
                Transform_Update(t, bdt);
            }
        }
    }
afterMotion:

    if (Player_getHitpoints() < 1 && (unsigned int)(F<int>(self, 0x88) - 3) >= 2) {
        // ---- death transition ----
        if (*(char *)(*(char **)((char *)self + 0x4) + 0x5c) == 0) {
            Level_friendDied();
        } else {
            Level_enemyDied(F<int>(self, 0x54), (bool)(unsigned char)F<int>(self, 0xac));
        }
        if (F<int>(self, 0xac) == 0x37a3)
            Level_pirateStationAction((bool)(unsigned char)F<int>(self, 0x54));

        F<unsigned char>(self, 0x134) = 0;
        F<int>(self, 0x88) = 3;
        int cargo = KIPlayer_cargoAvailable();
        F<unsigned char>(self, 0x4c) = (unsigned char)cargo;
        if (cargo != 0) KIPlayer_createCrate(self);
        PlayerFixedObject_setExhaustVisible(self, false);

        void *wreck = F<void *>(self, 0x124);
        AEGeometry_setMatrix(wreck, AEGeometry_getMatrix(F<void *>(self, 0x8)));
        wreck = F<void *>(self, 0x124);

        void *expl;
        if (wreck != 0) {
            AEGeometry_setMatrix(wreck, AEGeometry_getMatrix(F<void *>(self, 0x8)));
            void *t = PaintCanvas_TransformGetTransform(*g_pfo_canvasU,
                        *(int *)((char *)wreck + 0xc));
            Transform_SetAnimationState(t, 1, 0);
            if (F<int>(self, 0x28) == 3 && F<unsigned char>(self, 0x134) != 0 &&
                *(int *)(*(char **)((char *)self + 0x8) + 0x10) != -1) {
                AEGeometry_addChild(F<void *>(self, 0x8), F<void *>(self, 0x124));
            }
        }

        // particle + sound for the explosion (shared tail for both branches)
        void *lod = F<void *>(self, 0x54);
        void *mgr = *(void **)((char *)lod + 0x74);
        int sysOff = typeIsPirateOrE(self) ? 0x54 : 0x50;
        int sys = *(int *)((char *)lod + sysOff);
        void *m = AEGeometry_getMatrix(F<void *>(self, 0x8));
        void *sndHandle = ParticleSystemManager_systemSetMatrix(mgr, sys, m);
        Vector *pos = 0;
        if (*(char *)((char *)*g_pfo_audioFlag + 0xf) != 0)
            pos = (Vector *)((char *)self + 0x2c);
        FModSound_play(*g_pfo_fmod, 0x14, pos, (float)(long)sndHandle);
        lod = F<void *>(self, 0x54);
        ParticleSystemManager_enableSystemEmit(*(void **)((char *)lod + 0x74),
            (bool)(unsigned char)*(int *)((char *)lod + (typeIsPirateOrE(self) ? 0x54 : 0x50)));

        expl = operator_new(0x68);
        Explosion_ctor(expl, 0);
        F<void *>(self, 0x18c) = expl;
        Explosion_addFireStreaks(expl);
        expl = F<void *>(self, 0x18c);

        char posBuf[12];
        AEGeometry_getPosition((Vector *)posBuf, F<void *>(self, 0x8));
        Explosion_start(expl, (Vector *)posBuf);

        if (F<int>(self, 0xac) == 0xe) {
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
            if (F<int>(self, 0xac) == 0xe &&
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

    int state = F<int>(self, 0x88);
    if (state == 3) {
        // dying: run explosion, drift, advance the wreck transform until done
        if (F<void *>(self, 0x18c) != 0)
            Explosion_update(F<void *>(self, 0x18c), dt, 0);
        if (F<int>(self, 0xac) != 0x37a3) {
            if (F<unsigned char>(self, 0x134) != 0) {
                F<int>(self, 0x180) = F<int>(self, 0x180) + dt;
                float d = AEGeometry_moveForward_ret(F<void *>(self, 0x124), (float)dt);
                if (F<int>(self, 0x78) != 0)
                    AEGeometry_moveForward_ret(F<void *>(self, 0x78), d);
            }
            void *m = AEGeometry_getMatrix(F<void *>(self, 0x124));
            Matrix_assign(*(char **)((char *)self + 0x4) + 0x4, m);
            char posBuf[12];
            AEGeometry_getPosition((Vector *)posBuf, F<void *>(self, 0x124));
            Vector_assign((Vector *)((char *)self + 0x2c), (Vector *)posBuf);
            Array<void *> *bv = F<Array<void *> *>(self, 0x128);
            if (bv != 0) {
                for (unsigned int i = 0; i < bv->length; i++) {
                    void *o = bv->data[i];
                    typedef void (*BVFn)(void *, float, float, float);
                    BVFn fn = *(BVFn *)(*(char **)o + 0x4);
                    fn(o, F<float>(self, 0x2c), F<float>(self, 0x30), F<float>(self, 0x34));
                    bv = F<Array<void *> *>(self, 0x128);
                }
            }
        }
        F<unsigned char>(self, 0x101) = 0;
        void *t = PaintCanvas_TransformGetTransform(*g_pfo_canvasU,
                    *(int *)((char *)F<void *>(self, 0x124) + 0xc));
        Transform_Update(t, bdt);
        t = PaintCanvas_TransformGetTransform(*g_pfo_canvasU,
                    *(int *)((char *)F<void *>(self, 0x124) + 0xc));
        if (*(char *)((char *)t + 0xed) == 0) {
            void *lod = F<void *>(self, 0x54);
            F<int>(self, 0x88) = 4;
            ParticleSystemManager_enableSystemEmit(*(void **)((char *)lod + 0x74),
                (bool)(unsigned char)*(int *)((char *)lod + (typeIsPirateOrE(self) ? 0x54 : 0x50)));
            Explosion_reset(F<void *>(self, 0x18c));
            float scale = 6.0f;
            if (F<int>(self, 0xac) == 0x37e7) scale = 8.0f;
            if (F<int>(self, 0xac) == 0x37a3) scale = 8.0f;
            Explosion_setScaling(F<void *>(self, 0x18c), scale);
            Explosion_start(F<void *>(self, 0x18c), (Vector *)((char *)self + 0x2c));
            F<int>(self, 0x198) = 1;
            F<int>(self, 0x190) = 0;
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
                    F<float>(self, 0x19c) = 1.0f - use / maxd;
                    ego = Level_getPlayer();
                    cam = PlayerEgo_getTargetFollowCamera(ego);
                    TargetFollowCamera_setRumblePercentage(F<float>(self, 0x19c), cam);
                }
            }
        }
    } else if (state == 4) {
        // exploding
        F<int>(self, 0x190) = F<int>(self, 0x190) + dt;
        if (F<void *>(self, 0x18c) != 0)
            Explosion_update(F<void *>(self, 0x18c), dt, 0);
        F<int>(self, 0xd8) = F<int>(self, 0xd8) + dt;

        bool spin = F<unsigned char>(self, 0x4c) != 0 && Player_isActive() != 0 &&
                    F<int>(self, 0x78) != 0;
        if (spin) {
            float r = (float)(dt >> 1) * 0.001f; // DAT scalings
            r = (float)(int)(r * 1.0f);
            AEGeometry_rotate(F<void *>(self, 0x78), r, r, r);
            if (F<int>(self, 0xd8) >= 0xea61) {
                KIPlayer_setActive((bool)(unsigned char)(long)self);
                F<unsigned char>(self, 0x101) = 1;
            }
        } else {
            if (F<void *>(self, 0x18c) != 0 && Explosion_isPlaying(F<void *>(self, 0x18c)) == 0) {
                if (F<int>(self, 0xd8) >= 0xea61)
                    KIPlayer_setActive((bool)(unsigned char)(long)self);
                F<unsigned char>(self, 0x101) = 1;
            }
        }

        if (F<int>(self, 0x194) >= 0) {
            if (F<void *>(self, 0x12c) != 0 && F<int>(self, 0x190) >= 0x8d &&
                (unsigned int)F<int>(self, 0x1a0) <= 0x7fffffff) {
                char posBuf[12];
                AEGeometry_getPosition((Vector *)posBuf, F<void *>(self, 0x8));
                Vector_assign((Vector *)((char *)self + 0x2c), (Vector *)posBuf);
                Array<void *> *bv = F<Array<void *> *>(self, 0x12c);
                for (unsigned int i = 0; i < bv->length; i++) {
                    void *o = bv->data[i];
                    typedef void (*BVFn)(void *, float, float, float);
                    BVFn fn = *(BVFn *)(*(char **)o + 0x4);
                    fn(o, F<float>(self, 0x2c), F<float>(self, 0x30), F<float>(self, 0x34));
                }
                unsigned short mat;
                switch ((unsigned int)F<int>(self, 0x194)) {
                    case 1: mat = 0x8248; break;
                    case 2: mat = 0x8249; break;
                    case 3: mat = 0x824a; break;
                    case 4: mat = 0x824b; break;
                    default: mat = 0x824d; break;
                }
                F<int>(self, 0x1a0) = mat;
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
                if (cam != 0 && F<int>(self, 0x198) > 0) {
                    int v = F<int>(self, 0x198) + dt;
                    if (v >= 0x7d0) v = 0x7d0;
                    F<int>(self, 0x198) = v;
                    ego = Level_getPlayer();
                    cam = PlayerEgo_getTargetFollowCamera(ego);
                    TargetFollowCamera_setRumblePercentage(
                        F<float>(self, 0x19c) * ((float)v / 50.0f + 1.0f), cam);
                    if (F<void *>(self, 0x18c) != 0 &&
                        Explosion_isPlaying(F<void *>(self, 0x18c)) == 0) {
                        ego = Level_getPlayer();
                        cam = PlayerEgo_getTargetFollowCamera(ego);
                        TargetFollowCamera_setRumblePercentage(0.0f, cam);
                        F<int>(self, 0x198) = 0;
                    }
                }
            }
        }
    } else if (state == 5) {
        // dead-but-selectable: search for a nearby active enemy to re-home on
        unsigned int *enemies = (unsigned int *)Player_getEnemies();
        if (enemies != 0) {
            F<int>(self, 0x168) = 0;
            for (unsigned int i = 0; i < enemies[0]; i++) {
                if (Player_isActive() != 0) {
                    char pb[12];
                    Player_getPosition((Vector *)pb);
                    Vector_assign((Vector *)((char *)self + 0x90), (Vector *)pb);
                    float dx = F<float>(self, 0x2c) - F<float>(self, 0x90);
                    float dy = F<float>(self, 0x30) - F<float>(self, 0x94);
                    float dz = F<float>(self, 0x34) - F<float>(self, 0x98);
                    const float lo = 0.0f, hi = 50.0f; // DAT thresholds
                    if (dx < hi && dx > lo && dy < hi && dy > lo && dz < hi && dz > lo) {
                        F<void *>(self, 0x168) = Player_getEnemy(F<void *>(self, 0x4), i);
                        Player_getPosition((Vector *)pb);
                        Vector_assign((Vector *)((char *)self + 0x90), (Vector *)pb);
                        F<unsigned int>(self, 0x144) = F<unsigned int>(self, 0x90);
                        F<unsigned int>(self, 0x148) = F<unsigned int>(self, 0x94);
                        F<unsigned int>(self, 0x14c) = F<unsigned int>(self, 0x98);
                        break;
                    }
                }
            }
        }
        float vx = F<float>(self, 0x144) - F<float>(self, 0x2c);
        float vy = F<float>(self, 0x148) - F<float>(self, 0x30);
        float vz = F<float>(self, 0x14c) - F<float>(self, 0x34);
        F<float>(self, 0x150) = vx;
        F<float>(self, 0x154) = vy;
        F<float>(self, 0x158) = vz;
        const float lo = 0.0f, hi = 50.0f;
        if (vx < hi && vx > lo && vz > lo && vy < hi && vy > lo && vz < hi) {
            F<int>(self, 0x88) = 1;
            Player_setActive((bool)(unsigned char)(long)F<void *>(self, 0x4));
        }
    }

    // mirror the integer position into the Player object
    void *p = F<void *>(self, 0x4);
    *(int *)((char *)p + 0x48) = F<int>(self, 0x178);
    *(int *)((char *)p + 0x4c) = F<int>(self, 0x17c);
    *(int *)((char *)p + 0x50) = F<int>(self, 0x180);
}

// ---- setExhaustVisible_154cb0.cpp ----
// PaintCanvas singleton holder (single pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_pfo_canvas;

// NEAR: target keeps the frame up-front (no shrink-wrap, bool saved to r4 before the
// field checks). clang shrink-wraps here (checks before push, bx lr early-exit).
extern "C" void PlayerFixedObject_setExhaustVisible(PlayerFixedObject *self, bool v)
{
    void *geom = F<void *>(self, 0x8);
    if (geom != 0 && *(int *)((char *)geom + 0x14) != -1) {
        void **holder = g_pfo_canvas;
        return Transform_setExhaustVisible(PaintCanvas_TransformGetTransform(*holder), v);
    }
}

// ---- collide_154d4a.cpp ----
// collide(float,float,float): iterate the active bounding-volume array, calling each
// volume's vtable slot +8 with (bv, x, y, z); return 1 on first hit, else 0.
// NEAR: clang rotates the first loop as break-test (cbnz) while the target uses
// continue-test (cmp;beq) there; the two loops differ only in rotation.
typedef int (*CollideFn)(void *bv, float, float, float);

extern "C" int PlayerFixedObject_collide(PlayerFixedObject *self, float x, float y, float z)
{
    Array<void *> *a = F<Array<void *> *>(self, 0x12c);
    if ((a != 0 || F<int>(self, 0x88) != 4) && F<uint8_t>(self, 0x8c) != 0) {
        if (a != 0 && F<int>(self, 0x88) == 4) {
            for (uint32_t i = 0; i < a->length; i++) {
                void *bv = a->data[i];
                CollideFn fn = *(CollideFn *)(*(char **)bv + 0x8);
                if (fn(bv, x, y, z) != 0) return 1;
                a = F<Array<void *> *>(self, 0x12c);
            }
        } else {
            Array<void *> *b = F<Array<void *> *>(self, 0x128);
            if (b != 0) {
                for (uint32_t i = 0; i < b->length; i++) {
                    void *bv = b->data[i];
                    CollideFn fn = *(CollideFn *)(*(char **)bv + 0x8);
                    if (fn(bv, x, y, z) != 0) return 1;
                    b = F<Array<void *> *>(self, 0x128);
                }
            }
        }
    }
    return 0;
}

// ---- setBV_15401c.cpp ----
// Allocate a 12-byte Array<BoundingVolume*>, default-construct, store at +0x128,
// then forward (param_1, arr) to the bounding-volume registration thunk.
// NEAR: clang assigns arr->r5, this->r6 (we get them swapped); allocator tie-break, not source-driven.
extern "C" void PlayerFixedObject_setBV(PlayerFixedObject *self, BoundingVolume *bv)
{
    void *arr = operator_new(0xc);
    Array_BV_ctor(arr);
    F<void *>(self, 0x128) = arr;
    return BoundingVolume_setArr(bv, arr);
}

// ---- reset_153e24.cpp ----
typedef unsigned long long uint64_t;


typedef void (*SetPosFn)(PlayerFixedObject *, float, float, float);

// PC-relative pointer-to-function-pointer; *holder is a Vector::operator= style routine
// invoked as fn(Vector*, Vector*).
typedef void (*VecAssignFn)(void *dst, void *src);
__attribute__((visibility("hidden"))) extern VecAssignFn *g_pfo_vecAssignZero;

extern "C" void PlayerFixedObject_reset(PlayerFixedObject *self)
{
    KIPlayer_reset(self);

    // vtable slot 0x48 -> setPosition(this->0x58, 0x5c, 0x60)
    SetPosFn setPos = *(SetPosFn *)(*(char **)self + 0x48);
    setPos(self, F<float>(self, 0x58), F<float>(self, 0x5c), F<float>(self, 0x60));

    F<int32_t>(self, 0x168) = 0;

    // Vector copy: 0x58 -> 0x138
    {
        char buf[12];
        *(uint64_t *)buf = *(uint64_t *)((char *)self + 0x58);
        *(uint32_t *)(buf + 8) = F<uint32_t>(self, 0x60);
        Vector_assign((Vector *)((char *)self + 0x138), (Vector *)buf);
    }
    // Vector copy: 0x138 -> 0x2c
    {
        char buf[12];
        *(uint64_t *)buf = *(uint64_t *)((char *)self + 0x138);
        *(uint32_t *)(buf + 8) = F<uint32_t>(self, 0x140);
        Vector_assign((Vector *)((char *)self + 0x2c), (Vector *)buf);
    }

    F<int32_t>(self, 0x130) = 0;
    if (F<int32_t>(self, 0x88) != 5)
        F<int32_t>(self, 0x88) = 0;

    VecAssignFn fn = *g_pfo_vecAssignZero;
    char zero[12];
    *(uint32_t *)(zero + 0) = 0;
    *(uint32_t *)(zero + 4) = 0;
    *(uint32_t *)(zero + 8) = 0;
    fn((char *)self + 0x90, zero);
    *(uint32_t *)(zero + 0) = 0;
    *(uint32_t *)(zero + 4) = 0;
    *(uint32_t *)(zero + 8) = 0;
    fn((char *)self + 0x144, zero);
    *(uint32_t *)(zero + 0) = 0;
    *(uint32_t *)(zero + 4) = 0;
    *(uint32_t *)(zero + 8) = 0;
    fn((char *)self + 0x150, zero);
}

// ---- setWreckedMeshId_15407c.cpp ----
// PaintCanvas singleton holder (pc-rel deref -> holder; canvas object is *holder).
__attribute__((visibility("hidden"))) extern void **g_pfo_canvas2;
// Globals singleton: pc-rel deref -> holder, **holder is the Globals object.
__attribute__((visibility("hidden"))) extern void ***g_pfo_globals;

extern "C" void PlayerFixedObject_setWreckedMeshId(PlayerFixedObject *self, int meshId)
{
    F<uint16_t>(self, 0x184) = (uint16_t)meshId;
    void *geom = operator_new(0xc0);
    void **holder = g_pfo_canvas2;
    AEGeometry_ctor(geom, (uint16_t)meshId, *holder, true);
    F<void *>(self, 0x124) = geom;
    void *t = PaintCanvas_TransformGetTransform(*holder, *(int *)((char *)geom + 0xc));
    *(int *)((char *)t + 0xe0) = 0x48f42400; // 500000.0f far-clip constant (raw bits)

    int kind = F<int>(self, 0xac);
    int sel;
    if (kind == 0xd) {
        sel = 4;
    } else if (kind == 0xe) {
        sel = 0;
    } else if (kind == 0xf) {
        if (F<int>(self, 0x28) == 3) sel = 1;
        else sel = (F<int>(self, 0x28) == 2) ? 2 : 3;
    } else if (kind == 0x37a3) {
        sel = 5;
    } else {
        sel = F<int>(self, 0x194);
        if (sel < 0) return;
        F<int>(self, 0x12c) = Globals_getWreckCollision(**g_pfo_globals, sel, F<void*>(self, 0x124));
        return;
    }
    F<int>(self, 0x194) = sel;
    F<int>(self, 0x12c) = Globals_getWreckCollision(**g_pfo_globals, sel, F<void*>(self, 0x124));
}

// ---- getProjectionVector_154dd6.cpp ----
// Returns a Vector by value (sret r0, this r1). Picks the active bounding-volume array,
// indexes it by the stored collision index (this+0x16c) and forwards the chosen BV.
// NEAR: target shares one index+call across both branches (explicit join) and keeps the
// first bv in a callee reg; clang lays the branches out separately here.
struct V3 { float x, y, z; };

extern "C" V3 PlayerFixedObject_getProjectionVector(PlayerFixedObject *self)
{
    void *bv = F<void *>(self, 0x12c);
    if (bv != 0 && F<int>(self, 0x88) == 4) {
        int idx = F<int>(self, 0x16c);
        void **data = *(void ***)((char *)bv + 0x4);
        return BV_getProjectionVector(data[idx]);
    }
    void *bv2 = F<void *>(self, 0x128);
    if (bv2 != 0) {
        int idx = F<int>(self, 0x16c);
        void **data = *(void ***)((char *)bv2 + 0x4);
        return BV_getProjectionVector(data[idx]);
    }
    V3 z = {0.0f, 0.0f, 0.0f};
    return z;
}

// ---- _PlayerFixedObject_153d70.cpp ----
// Complete object destructor (D1). Returns this. Tears down the wrecked-mesh geometry,
// the two bounding-volume arrays, the explosion, and the name String, then tail-calls
// the base destructor.

extern "C" void *_ZN17PlayerFixedObjectD1Ev(PlayerFixedObject *self)
{
    void *wreck = F<void *>(self, 0x124);
    *(void **)self = &PlayerFixedObject_vtable + 8;
    if (wreck != F<void *>(self, 0x8)) {
        if (wreck != 0) operator_delete(AEGeometry_dtor(wreck));
        F<void *>(self, 0x124) = 0;
    }
    void *bvB = F<void *>(self, 0x128);
    if (bvB != 0) {
        ArrayReleaseClasses_BV(bvB);
        void *b = F<void *>(self, 0x128);
        if (b != 0) operator_delete(Array_BV_dtor(b));
    }
    F<void *>(self, 0x128) = 0;
    void *bvA = F<void *>(self, 0x12c);
    if (bvA != 0) {
        ArrayReleaseClasses_BV(bvA);
        void *a = F<void *>(self, 0x12c);
        if (a != 0) operator_delete(Array_BV_dtor(a));
    }
    F<void *>(self, 0x12c) = 0;
    void *expl = F<void *>(self, 0x18c);
    if (expl != 0) operator_delete(Explosion_dtor(expl));
    F<void *>(self, 0x18c) = 0;
    String_dtor((char *)self + 0x1ac);
    return PlayerFixedObject_baseDtor(self);
}

// ---- render_154cdc.cpp ----
// Tail-call thunks selected by object state.
extern "C" void render_thunk_state5(void *geom);   // DAT_001abdd4 thunk, arg = this->0x8
extern "C" void render_thunk_other(int meshId);     // DAT_001ac2b4 thunk, arg = this->0x18c

// NEAR: clang reorders the state comparisons (3 before 5) and duplicates the shared
// state5 block instead of sharing it via the original goto; structure differs.
extern "C" void PlayerFixedObject_render(PlayerFixedObject *self)
{
    void *g78 = F<void *>(self, 0x78);
    if (g78 != 0 && F<uint8_t>(self, 0x1b8) == 0) {
        AEGeometry_render(g78);
    }
    int iVar1 = F<int>(self, 0x88);
    if (iVar1 == 5) {
LAB_538:
        if (F<uint8_t>(self, 0x1b8) != 0) return;
        return render_thunk_state5(F<void *>(self, 0x8));
    }
    if (iVar1 == 4) {
        if (F<uint8_t>(self, 0x1b8) == 0) AEGeometry_render(F<void *>(self, 0x124));
        iVar1 = F<int>(self, 0x18c);
        if (iVar1 == 0) return;
    } else {
        if (iVar1 != 3) {
            if (Player_isActive(F<void *>(self, 0x4)) == 0) return;
            goto LAB_538;
        }
        if (F<uint8_t>(self, 0x1b8) == 0) AEGeometry_render(F<void *>(self, 0x124));
        iVar1 = F<int>(self, 0x18c);
    }
    return render_thunk_other(iVar1);
}

// ---- PlayerFixedObject_153a7c.cpp ----
typedef unsigned long long uint64_t;

// PlayerFixedObject::PlayerFixedObject(int kind, int param2, Player*, AEGeometry*, float, float, float, ...)
// (two extra stack floats in_stack_00000004/8/c make up the spawn position passed by value).

extern "C" void KIPlayer_ctor(PlayerFixedObject *self, int kind, int hp, void *player,
                              void *geom, float a, float b, float c, bool d);



// PC-relative literal table holding the station-index match list (4 ints) and the
// loot-list parameter table (4 x 8-byte pairs).
__attribute__((visibility("hidden"))) extern const int g_pfo_stationIdx[4];
__attribute__((visibility("hidden"))) extern const int g_pfo_lootParams[8]; // pairs at +0 used; [idx*2+1]
// AERandom singleton holder (pc-rel -> holder; *holder is the AERandom object).
__attribute__((visibility("hidden"))) extern void **g_pfo_random;

extern "C" void PlayerFixedObject_ctor(PlayerFixedObject *self, int kind, int param2,
                                       void *player, void *geom,
                                       float p5, float p6, float p7,
                                       float sx, float sy, float sz)
{
    KIPlayer_ctor(self, kind, -1, player, geom, p5, p6, p7, (bool)(int)sx);

    // Three zeroed Vector4 (16-byte) blocks.
    *(uint64_t *)((char *)self + 0x158) = 0; *(uint64_t *)((char *)self + 0x160) = 0;
    *(uint64_t *)((char *)self + 0x148) = 0; *(uint64_t *)((char *)self + 0x150) = 0;
    *(uint64_t *)((char *)self + 0x138) = 0; *(uint64_t *)((char *)self + 0x140) = 0;

    *(void **)self = &PlayerFixedObject_vtable + 8;

    String_ctor_empty((char *)self + 0x1ac);
    F<int32_t>(self, 0x18c) = 0;
    F<uint16_t>(self, 0x184) = 0;
    F<int32_t>(self, 0x28) = param2;
    F<int32_t>(self, 0x124) = 0;
    F<int32_t>(self, 0x128) = 0;
    F<int32_t>(self, 0x12c) = 0;
    F<int32_t>(self, 0x130) = 0;
    F<int32_t>(self, 0x168) = 0;
    F<int32_t>(self, 0x16c) = 0;
    F<uint8_t>(self, 0x170) = 0;
    F<uint8_t>(self, 0x1b8) = 0;
    F<int32_t>(self, 0x190) = 0;
    F<int32_t>(self, 0x174) = 0;
    F<int32_t>(self, 0x178) = 0; F<int32_t>(self, 0x17c) = 0; F<int32_t>(self, 0x180) = 0;

    // Vector::operator=(this+0x2c, {sx,sy,sz})
    {
        char buf[12];
        *(float *)(buf + 0) = sx; *(float *)(buf + 4) = sy; *(float *)(buf + 8) = sz;
        Vector_assign((Vector *)((char *)self + 0x2c), (Vector *)buf);
    }

    F<uint8_t>(self, 0x134) = 0;
    F<int32_t>(self, 0x194) = -1;
    F<int32_t>(self, 0x1a0) = -1;
    F<int32_t>(self, 0x1a4) = 0;
    F<int32_t>(self, 0x178) = (int32_t)sx;
    F<int32_t>(self, 0x17c) = (int32_t)sy;
    F<int32_t>(self, 0x180) = (int32_t)sz;

    // Name string from a fixed literal.
    {
        char tmp[12];
        String_ctor_cstr(tmp, "", false);
        String_op_assign((char *)self + 0x1ac, tmp);
        String_dtor(tmp);
    }

    void *mission = Status_getMission();
    int campaign = Mission_isCampaignMission(mission);
    bool special = false;
    if (campaign != 0) {
        int cm = Status_getCurrentCampaignMission();
        if (cm == 0x28) special = true;
        else {
            cm = Status_getCurrentCampaignMission();
            if (cm == 0x29) special = true;
        }
    }

    if (special) {
        if (F<void *>(self, 0x50) != 0) {
            operator_delete(Array_int_dtor(F<void *>(self, 0x50)));
        }
        F<int32_t>(self, 0x50) = 0;
    } else {
        void *gen = operator_new(1);
        Generator_ctor(gen);
        if (kind == 0x37a3) {
            F<uint8_t>(self, 0x41) = 1;
            void *station = Status_getStation();
            int sidx = Station_getIndex(station);
            for (uint32_t i = 0; i < 4; i++) {
                if (g_pfo_stationIdx[i] == sidx) {
                    void *loot = Generator_getLootList(gen, g_pfo_lootParams[i * 2 + 1], 0);
                    F<void *>(self, 0x50) = loot;
                }
            }
        } else {
            uint32_t *loot = (uint32_t *)Generator_getLootList(gen, -1, -1);
            F<void *>(self, 0x50) = loot;
            if (loot != 0) {
                int second = (kind != 0x498e) ? 0x4a88 : 0x498e;
                if (kind != 0x498e && kind != second) {
                    void *rng = *g_pfo_random;
                    for (int idx = 1; (uint32_t)(idx - 1) < loot[0]; idx += 2) {
                        if (kind == 0xe) {
                            int r = AERandom_nextInt((int)(long)rng);
                            loot = (uint32_t *)F<void *>(self, 0x50);
                            int *cell = (int *)(loot[1] + idx * 4);
                            *cell = *cell * (r + 5);
                        } else {
                            int r = AERandom_nextInt((int)(long)rng);
                            int *base = (int *)(*(int *)((char *)F<void *>(self, 0x50) + 4) + idx * 4);
                            *base = *base * (r + 2);
                            int r2 = AERandom_nextInt((int)(long)rng);
                            loot = (uint32_t *)F<void *>(self, 0x50);
                            int *cell = (int *)(loot[1] + idx * 4);
                            int v = *cell;
                            if (v < r2 + 8) v = r2 + 8;
                            *cell = v;
                        }
                    }
                }
            }
        }
        operator_delete(Generator_dtor(gen));
    }

    *(uint8_t *)(*(char **)((char *)self + 0x4) + 0x45) = 1;
    if (kind != 0x37a3) {
        F<int32_t>(self, 0xf8) = 0x2f;
        if (kind == 0xe) {
            F<int32_t>(self, 0xf8) = -1;
            F<uint8_t>(self, 0x134) = 0;
        }
    }
}

// ---- setPosition_153eec.cpp ----
typedef void (*BVSetPosFn)(void *bv, float, float, float);

// PlayerFixedObject::setPosition(float, float, float)
extern "C" void PlayerFixedObject_setPosition3(PlayerFixedObject *self, float x, float y, float z)
{
    F<float>(self, 0x58) = x;
    F<float>(self, 0x5c) = y;
    F<float>(self, 0x60) = z;
    F<int32_t>(self, 0x178) = (int32_t)x;
    F<int32_t>(self, 0x17c) = (int32_t)y;
    F<int32_t>(self, 0x180) = (int32_t)z;

    AEGeometry_setPosition(F<void *>(self, 0x8), x, y, z);
    void *m = AEGeometry_getMatrix(F<void *>(self, 0x8));
    Matrix_assign((char *)F<void *>(self, 0x4) + 0x4, m);

    char buf[12];
    AEGeometry_getPosition((Vector *)buf, F<void *>(self, 0x8));
    Vector_assign((Vector *)((char *)self + 0x2c), (Vector *)buf);

    Array<void *> *bv = F<Array<void *> *>(self, 0x128);
    if (bv != 0) {
        for (uint32_t i = 0; i < bv->length; i++) {
            void *o = bv->data[i];
            BVSetPosFn fn = *(BVSetPosFn *)(*(char **)o + 0x4);
            fn(o, F<float>(self, 0x2c), F<float>(self, 0x30), F<float>(self, 0x34));
            bv = F<Array<void *> *>(self, 0x128);
        }
    }

    if (F<void *>(self, 0x124) != 0) {
        AEGeometry_setPosition(F<void *>(self, 0x124), x, y, z);
    }
}

// ---- setDeadButSelectable_154f24.cpp ----
// NEAR (80.6%): structure matches, but the final Transform::SetAnimationRangeInTime call is
// emitted as a tail-branch here while the target keeps a frame and passes the 64-bit arg on the
// stack (push {r2,r3,...}; strd; call; pop). Calling-convention detail not reproducible from src.
// PaintCanvas singleton holder (pc-rel -> holder; *holder is the canvas).
__attribute__((visibility("hidden"))) extern void **g_pfo_canvas3;

extern "C" void PlayerFixedObject_setDeadButSelectable(PlayerFixedObject *self)
{
    void *player = F<void *>(self, 0x4);
    F<uint8_t>(self, 0x134) = 0;
    Player_setHitpoints(player, 1);
    Player_setVulnerable(F<void *>(self, 0x4), false);
    LODManager_removeObject(*(void **)F<void *>(self, 0x54), F<void *>(self, 0x8));
    void *geom = F<void *>(self, 0x8);
    if (geom != 0) operator_delete(AEGeometry_dtor(geom));
    void **holder = g_pfo_canvas3;
    void *newGeom = F<void *>(self, 0x124);
    F<void *>(self, 0x8) = newGeom;
    void *t = PaintCanvas_TransformGetTransform(*holder, *(int *)((char *)newGeom + 0xc));
    Transform_SetAnimationRangeInTime(t, *(long long *)((char *)t + 0xf8));
}

// ---- outerCollide_154e5a.cpp ----
// outerCollide(float,float,float): iterate the active bounding-volume array, calling each
// volume's vtable slot +0xc with (bv, x, y, z). On the first hit, store the hit index at
// this+0x16c and return 1; otherwise return 0.
typedef int (*CollideFn)(void *bv, float, float, float);

extern "C" int PlayerFixedObject_outerCollide(PlayerFixedObject *self, float x, float y, float z)
{
    Array<void *> *a = F<Array<void *> *>(self, 0x12c);
    if ((a != 0 || F<int>(self, 0x88) != 4) && F<uint8_t>(self, 0x8c) != 0) {
        if (a != 0 && F<int>(self, 0x88) == 4) {
            for (uint32_t i = 0; i < a->length; i++) {
                void *bv = a->data[i];
                CollideFn fn = *(CollideFn *)(*(char **)bv + 0xc);
                if (fn(bv, x, y, z) != 0) { F<uint32_t>(self, 0x16c) = i; return 1; }
                a = F<Array<void *> *>(self, 0x12c);
            }
        } else {
            Array<void *> *b = F<Array<void *> *>(self, 0x128);
            if (b != 0) {
                for (uint32_t i = 0; i < b->length; i++) {
                    void *bv = b->data[i];
                    CollideFn fn = *(CollideFn *)(*(char **)bv + 0xc);
                    if (fn(bv, x, y, z) != 0) { F<uint32_t>(self, 0x16c) = i; return 1; }
                    b = F<Array<void *> *>(self, 0x128);
                }
            }
        }
    }
    return 0;
}

// ---- moveForward_154134.cpp ----
// NEAR (~82%): logic matches but the target carries a stack canary (it was built with the
// guard for the on-stack Vector). Under the fixed -fstack-protector (basic, not -strong) flag,
// clang only guards char arrays accessed as bytes; this Vector buffer (read via Vector*) is not
// protected, so no canary is emitted. Matching it would require -fstack-protector-strong.

typedef void (*BVMoveFn)(void *bv, Vector);

extern "C" void PlayerFixedObject_moveForward(PlayerFixedObject *self, int amount)
{
    float d = (float)amount;
    F<int>(self, 0x180) = amount + F<int>(self, 0x180);
    AEGeometry_moveForward(F<void *>(self, 0x8), d);
    void *m = AEGeometry_getMatrix(F<void *>(self, 0x8));
    Matrix_assign((char *)F<void *>(self, 0x4) + 0x4, m);
    char buf[12];
    AEGeometry_getPosition((Vector *)buf, F<void *>(self, 0x8));
    Vector_assign((Vector *)((char *)self + 0x2c), (Vector *)buf);
    if (F<void *>(self, 0x124) != 0) {
        AEGeometry_moveForward(F<void *>(self, 0x124), d);
    }
    Array<void *> *bv = F<Array<void *> *>(self, 0x128);
    if (bv != 0) {
        for (uint32_t i = 0; i < bv->length; i++) {
            void *o = bv->data[i];
            BVMoveFn fn = *(BVMoveFn *)(*(char **)o + 0x4);
            fn(o, *(Vector *)((char *)self + 0x2c));
            bv = F<Array<void *> *>(self, 0x128);
        }
    }
}
