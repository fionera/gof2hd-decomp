#include "gof2/PlayerFixedObject.h"
#include "gof2/AEGeometry.h"

namespace AbyssEngine { namespace AEMath { float VectorLength(const Vector &value); } }
#include "gof2/FModSound.h"
#include "gof2/Generator.h"
#include "gof2/LODManager.h"
#include "gof2/ParticleSystemManager.h"
#include "gof2/Transform.h"
#include "gof2/Achievements.h"
#include "gof2/Explosion.h"
#include "gof2/Hud.h"
#include "gof2/KIPlayer.h"
#include "gof2/Level.h"
#include "gof2/Mission.h"
#include "gof2/PlayerEgo.h"
#include "gof2/Standing.h"
#include "gof2/Status.h"
#include "gof2/String.h"
#include "gof2/BoundingVolume.h"
#include "gof2/Player.h"
#include "gof2/PaintCanvasClass.h"

// V3 (3-float by-value vector return type) is provided by gof2/PlayerFixedObject.h
// as a typedef of AbyssEngine::AEMath::Vector.

// Byte-offset reader retained for the few foreign-class fields accessed from this
// translation unit by raw offset (engine singletons / Player internals not modelled
// in their out-of-batch headers).
template <class T> static inline T &F(void *p, int off) { return *(T *)((char *)p + off); }

extern "C" V3 BV_staticProjectCollisionOnSurface(void *vec, void *bvArray);
extern "C" void *Explosion_ctor(void *e, int a);
extern "C" void  Explosion_update(void *e, int dt, void *cam);
extern "C" void  TargetFollowCamera_getPosition(Vector *out, void *cam);
extern "C" void  TargetFollowCamera_setRumblePercentage(float pct, void *cam);
extern "C" void *Player_getEnemies();
extern "C" void  Player_getPosition(Vector *out);
extern "C" void Transform_setExhaustVisible(void *transform, bool v);
extern "C" void Array_BV_ctor(void *arr);
extern "C" void BoundingVolume_setArr(BoundingVolume *bv, void *arr);
void *Globals_getWreckCollision(void *globals, int kind, void *geom);
extern "C" V3 BV_getProjectionVector(void *bv);
extern "C" char PlayerFixedObject_vtable;
extern "C" void ArrayReleaseClasses_BV(void *arr);
extern "C" void *Array_BV_dtor(void *p);
extern "C" void *Explosion_dtor(void *p);
extern "C" void *PlayerFixedObject_baseDtor(PlayerFixedObject *self);
extern "C" void String_ctor_empty(void *s);
extern "C" void String_ctor_cstr(void *s, const char *cstr, bool b);
extern "C" void *String_op_assign(void *dst, void *src);
extern "C" void Generator_ctor(void *g);
extern "C" void *Generator_dtor(void *g);

// ---- getDockingType_154ef0.cpp ----
int PlayerFixedObject::getDockingType() {
    PlayerFixedObject *self = this;
    return self->dockingType;
}

// ---- setBV_154016.cpp ----
void PlayerFixedObject::setBV_arr(Array<BoundingVolume *> *bv) {
    PlayerFixedObject *self = this;
    self->boundingVolumes = bv;
}

// ---- hideShip_154f1a.cpp ----
void PlayerFixedObject::hideShip() {
    PlayerFixedObject *self = this;
    self->shipHidden = 1;
}

// ---- getTransportID_154efc.cpp ----
int PlayerFixedObject::getTransportID() {
    PlayerFixedObject *self = this;
    return self->transportID;
}

// ---- setDockingType_154eea.cpp ----
void PlayerFixedObject::setDockingType(int v) {
    PlayerFixedObject *self = this;
    self->dockingType = v;
}

// ---- setPosition_153fb4.cpp ----
typedef void (*SetPosFn)(PlayerFixedObject *, float, float, float);

void PlayerFixedObject::setPosition_vec(const Vector &v) {
    PlayerFixedObject *self = this;
    SetPosFn fn = *(SetPosFn *)(*(char **)self + 0x48);
    return fn(self, v.x, v.y, v.z);
}

// ---- translate_153fd0.cpp ----
typedef void (*SetPosFn)(PlayerFixedObject *, float, float, float);

// Reads ship pos (signed ints at +0x178/0x17c/0x180 -> float), adds the delta Vector,
// then forwards to vtable slot 0x48.
void PlayerFixedObject::translate(const Vector &d) {
    PlayerFixedObject *self = this;
    float x = (float)self->intPosX;
    float y = (float)self->intPosY;
    float z = (float)self->intPosZ;
    SetPosFn fn = *(SetPosFn *)(*(char **)self + 0x48);
    return fn(self, d.x + x, d.y + y, d.z + z);
}

// ---- getName_154f02.cpp ----
// AbyssEngine::String::String(String* out, const String* src, bool) -> void.
// Returns String by value (sret r0, this r1). Copy-ctor returns void, so the
// compiler keeps a frame + restores the sret pointer.
// `struct RetStr` is provided by gof2/Station.h via the class header.

RetStr PlayerFixedObject::getName() {
    PlayerFixedObject *self = this;
    RetStr r;
    ((String *)(&r))->ctor_copy((String *)((char *)self + 0x1ac), false);
    return r;
}

// ---- setName_154f12.cpp ----
// Tail-call into AbyssEngine::String::operator= (or move-assign): dst = this+0x1ac, src = r1 (the String arg).

void * PlayerFixedObject::setName(String *name) {
    PlayerFixedObject *self = this;
    return ((String *)((char *)self + 0x1ac))->assign(name);
}

// ---- _PlayerFixedObject_153e14.cpp ----
// Deleting destructor (D0): run the complete dtor (D1), then tail-call operator delete.
// Mangled names so the demangled symbol contains "~PlayerFixedObject".
void *_ZN17PlayerFixedObjectD1Ev(PlayerFixedObject *self); // complete object dtor

void _ZN17PlayerFixedObjectD0Ev(PlayerFixedObject *self)
{
    return ::operator delete(_ZN17PlayerFixedObjectD1Ev(self));
}

// ---- setMoving_153ee4.cpp ----
void PlayerFixedObject::setMoving(bool v) {
    PlayerFixedObject *self = this;
    self->moving = v;
}

// ---- projectCollisionOnSurface_154e16.cpp ----
// Returns a Vector by value (sret r0, this r1, collision vector r2). The callee returns
// the projected Vector into the same sret, so the compiler keeps a frame + call (not tail).
V3 PlayerFixedObject::projectCollisionOnSurface(void *vec) {
    PlayerFixedObject *self = this;
    void *bv = self->wreckCollision;
    if (bv != 0 && self->state == 4) {
        return BV_staticProjectCollisionOnSurface(vec, bv);
    }
    void *bv2 = self->boundingVolumes;
    if (bv2 != 0) {
        return BV_staticProjectCollisionOnSurface(vec, bv2);
    }
    V3 z = {0.0f, 0.0f, 0.0f};
    return z;
}

// ---- setTransportID_154ef6.cpp ----
void PlayerFixedObject::setTransportID(int v) {
    PlayerFixedObject *self = this;
    self->transportID = v;
}

// ---- outerCollide_154e50.cpp ----
// outerCollide(Vector) - Vector passed by value (r1,r2,r3). Pure tail-forward through
// vtable slot 0x3c: ldr r12,[r0]; ldr r12,[r12,#0x3c]; bx r12.
typedef void (*OuterCollideVecFn)(PlayerFixedObject *, Vector);

void PlayerFixedObject::outerCollide_vec(Vector v) {
    PlayerFixedObject *self = this;
    OuterCollideVecFn fn = *(OuterCollideVecFn *)(*(char **)self + 0x3c);
    return fn(self, v);
}

// ---- getPosition_154056.cpp ----
// sret in r0, this in r1. Reads three signed ints at +0x178/0x17c/0x180 and converts to float.
struct PosOut { float x, y, z; };

void PlayerFixedObject_getPosition(PosOut *out, PlayerFixedObject *self)
{
    float a = (float)self->intPosX;
    float b = (float)self->intPosY;
    float c = (float)self->intPosZ;
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
__attribute__((visibility("hidden"))) extern void **g_pfo_status;     // Status singleton holder

static inline bool typeIsPirateOrE(PlayerFixedObject *self) {
    int k = self->kind;
    return k == 0x37a3 || k == 0xe;
}

void PlayerFixedObject::update(int dt) {
    PlayerFixedObject *self = this;
    self->deltaTime = dt;
    bool bdt = (bool)(unsigned char)dt;

    // ship's KIPlayer "is active for tutorial" flag derived from 0xf8/0x134
    bool kiFlag = (self->field_0xf8 + 1 != 0) && (self->moving != 0);
    ((Player *)(self->player))->update(dt, kiFlag);

    // Player::field_0x5c/0x5d (enemy/friend flags) are not modelled in Player.h
    // (out-of-batch header) -> byte-offset accessed.
    void *player = self->player;
    unsigned char enemyFlag = 0;
    if ((self->faction & 0xfffffffe) == 8) {
        F<unsigned char>(player, 0x5c) = 1;
        enemyFlag = 0;
    } else {
        int st = ((Status *)(*g_pfo_status))->getStanding();
        unsigned char e = ((Standing *)((void *)(long)st))->isEnemy(self->faction);
        player = self->player;
        F<unsigned char>(player, 0x5c) = e;
        if ((self->faction & 0xfffffffe) == 8) {
            enemyFlag = 0;
        } else {
            void *st2 = (void *)(long)((Status *)(*g_pfo_status))->getStanding();
            enemyFlag = ((Standing *)(st2))->isFriend(self->faction);
            player = self->player;
        }
    }
    F<unsigned char>(player, 0x5d) = enemyFlag;

    if (Player_turnedEnemy((Player *)self->player) != 0)
        F<unsigned short>(self->player, 0x5c) = 1;
    if (((Player *)(self->player))->isAlwaysFriend() != 0)
        F<unsigned short>(self->player, 0x5c) = 0x100;

    if (self->state != 6) {
        float bomb = ((Player *)(self->player))->getBombForce();
        float emp = ((Player *)(self->player))->getEmpForce();
        if (bomb > 0.0f) {
            float nb = bomb * 0.95f; // DAT decay factor
            if (nb < 1.0f) nb = 0.0f;
            ((Player *)(self->player))->setBombForce(nb);
        }
        if (emp > 0.0f) {
            float ne = emp - (float)dt;
            float clamped = ne;
            if (ne < 1.0f) clamped = 0.0f;
            self->empActive = (ne >= 1.0f) ? 1 : 0;
            ((Player *)(self->player))->setEmpForce(clamped);
        }
    }

    if (self->empActive == 0 && (unsigned int)(self->state - 3) >= 2) {
        int kind = self->kind;
        bool doMove = (kind != 0x37a3);
        if (doMove) doMove = (self->moving != 0);
        if (doMove) ((PlayerFixedObject *)(self))->moveForward(dt);

        int cm = ((Status *)(*g_pfo_status))->getCurrentCampaignMission();
        int k2 = self->kind;
        bool skip = (cm == 0x5b && k2 == 0x494e);
        if (!skip) {
            if (k2 == 0x494a) {
                if (((Status *)(*g_pfo_status))->getCurrentCampaignMission() == 0x91) goto afterMotion;
                k2 = self->kind;
            }
            if (k2 != 0x4220) {
                void *t = ((PaintCanvas*)*g_pfo_canvasU)->TransformGetTransform(
                            *(int *)((char *)self->geometry + 0xc));
                ((AbyssEngine::Transform *)(t))->Update((long long)dt, true);
            }
        }
    }
afterMotion:

    if (((Player *)(self->player))->getHitpoints() < 1 && (unsigned int)(self->state - 3) >= 2) {
        // ---- death transition ----
        if (F<char>(self->player, 0x5c) == 0) {
            ((Level *)(self->level))->friendDied();
        } else {
            ((Level *)((int)(__INTPTR_TYPE__)self->level))->enemyDied(0, (bool)(unsigned char)self->kind);
        }
        if (self->kind == 0x37a3)
            ((Level *)self->level)->pirateStationAction((bool)(unsigned char)self->kind);

        self->moving = 0;
        self->state = 3;
        int cargo = ((KIPlayer *)(self))->cargoAvailable();
        self->hasCargo = (unsigned char)cargo;
        if (cargo != 0) ((KIPlayer *)(self))->createCrate(0);
        ((PlayerFixedObject *)(self))->setExhaustVisible(false);

        void *wreck = self->wreckGeometry;
        ((AEGeometry *)(wreck))->setMatrix(((AEGeometry *)(self->geometry))->getMatrix());
        wreck = self->wreckGeometry;

        void *expl;
        if (wreck != 0) {
            ((AEGeometry *)(wreck))->setMatrix(((AEGeometry *)(self->geometry))->getMatrix());
            void *t = ((PaintCanvas*)*g_pfo_canvasU)->TransformGetTransform(
                        *(int *)((char *)wreck + 0xc));
            ((AbyssEngine::Transform *)(t))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);
            if (self->faction == 3 && self->moving != 0 &&
                *(int *)((char *)self->geometry + 0x10) != -1) {
                ((AEGeometry *)(self->geometry))->addChild((uint32_t)(uintptr_t)self->wreckGeometry);
            }
        }

        // particle + sound for the explosion (shared tail for both branches)
        void *lod = self->level;
        void *mgr = *(void **)((char *)lod + 0x74);
        int sysOff = typeIsPirateOrE(self) ? 0x54 : 0x50;
        int sys = *(int *)((char *)lod + sysOff);
        void *m = (void *)&((AEGeometry *)(self->geometry))->getMatrix();
        ((ParticleSystemManager *)(mgr))->systemSetMatrix(sys, m);
        int sndHandle = sys;
        Vector *pos = 0;
        if (*(char *)((char *)*g_pfo_audioFlag + 0xf) != 0)
            pos = (Vector *)((char *)self + 0x2c);
        ((FModSound *)(*g_pfo_fmod))->play(0x14, pos, (Vector *)0, (float)sndHandle);
        lod = self->level;
        {
            int emitHandle = *(int *)((char *)lod + (typeIsPirateOrE(self) ? 0x54 : 0x50));
            ((ParticleSystemManager *)(*(void **)((char *)lod + 0x74)))->enableSystemEmit(emitHandle, true);
        }

        expl = ::operator new(0x68);
        Explosion_ctor(expl, 0);
        self->explosion = expl;
        ((Explosion *)(expl))->addFireStreaks();
        expl = self->explosion;

        char posBuf[12];
        ((AEGeometry *)((Vector *)posBuf))->getPosition();
        ((Explosion *)(expl))->start((Vector *)posBuf, (const Vector *)0);

        if (self->kind == 0xe) {
            unsigned int *enemies = (unsigned int *)(intptr_t)((Level *)self->level)->getEnemies();
            for (unsigned int i = 0; i < enemies[0]; i++) {
                void *en = (void *)(intptr_t)((Level *)self->level)->getEnemies();
                void *obj = *(void **)(*(int *)((char *)en + 4) + i * 4);
                if (*(char *)((char *)obj + 0x3e) != 0) {
                    en = (void *)(intptr_t)((Level *)self->level)->getEnemies();
                    obj = *(void **)(*(int *)((char *)en + 4) + i * 4);
                    ((Player *)(*(void **)((char *)obj + 4)))->damage(g_pfo_dmgVal);
                }
                enemies = (unsigned int *)(intptr_t)((Level *)self->level)->getEnemies();
            }
            if (self->kind == 0xe &&
                *(char *)((char *)self->player + 0x44) == 0) {
                void *egoObj = *g_pfo_egoA;
                void *ach = *g_pfo_achievements;
                *(int *)((char *)egoObj + 0x118) = *(int *)((char *)egoObj + 0x118) + 1;
                if (((Achievements *)(ach))->hasMedal(0x27, 1) == 0) {
                    float cur = (float)*(int *)((char *)egoObj + 0x118);
                    float val = (float)((Achievements *)(ach))->getValue(0x27, 1);
                    if ((int)(cur / val) < 2) {
                        void *ego = (void *)(intptr_t)((Level *)self->level)->getPlayer();
                        void *hud = (void *)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getHUD();
                        cur = (float)*(int *)((char *)egoObj + 0x118);
                        val = (float)((Achievements *)(ach))->getValue(0x27, 1);
                        ((Hud *)(hud))->hudEventMedal(0x27, (int)((cur / val) * 100.0f));
                    }
                }
            }
        }
    }

    int state = self->state;
    if (state == 3) {
        // dying: run explosion, drift, advance the wreck transform until done
        if (self->explosion != 0)
            Explosion_update(self->explosion, dt, 0);
        if (self->kind != 0x37a3) {
            if (self->moving != 0) {
                self->intPosZ = self->intPosZ + dt;
                // AEGeometry::moveForward returns void; the binary forwards the same
                // distance (dt, held live in r0/s0) to the secondary geometry.
                ((AEGeometry *)self->wreckGeometry)->moveForward((float)dt);
                if (self->secondaryGeometry != 0)
                    ((AEGeometry *)self->secondaryGeometry)->moveForward((float)dt);
            }
            Matrix &m = ((AEGeometry *)(self->wreckGeometry))->getMatrix();
            *(Matrix *)((char *)self->player + 0x4) = m;
            Vector pos = ((AEGeometry *)(self->wreckGeometry))->getPosition();
            *(Vector *)((char *)self + 0x2c) = pos;
            Array<void *> *bv = (Array<void *> *)self->boundingVolumes;
            if (bv != 0) {
                for (unsigned int i = 0; i < bv->size(); i++) {
                    void *o = bv->data()[i];
                    typedef void (*BVFn)(void *, float, float, float);
                    BVFn fn = *(BVFn *)(*(char **)o + 0x4);
                    fn(o, self->posX, self->posY, self->posZ);
                    bv = (Array<void *> *)self->boundingVolumes;
                }
            }
        }
        self->finished = 0;
        void *t = ((PaintCanvas*)*g_pfo_canvasU)->TransformGetTransform(
                    *(int *)((char *)self->wreckGeometry + 0xc));
        ((AbyssEngine::Transform *)(t))->Update((long long)dt, true);
        t = ((PaintCanvas*)*g_pfo_canvasU)->TransformGetTransform(
                    *(int *)((char *)self->wreckGeometry + 0xc));
        if (*(char *)((char *)t + 0xed) == 0) {
            void *lod = self->level;
            self->state = 4;
            {
                int emitHandle = *(int *)((char *)lod + (typeIsPirateOrE(self) ? 0x54 : 0x50));
                ((ParticleSystemManager *)(*(void **)((char *)lod + 0x74)))->enableSystemEmit(emitHandle, true);
            }
            ((Explosion *)(self->explosion))->reset();
            float scale = 6.0f;
            if (self->kind == 0x37e7) scale = 8.0f;
            if (self->kind == 0x37a3) scale = 8.0f;
            ((Explosion *)(self->explosion))->setScaling(scale);
            ((Explosion *)(self->explosion))->start((Vector *)((char *)self + 0x2c), (const Vector *)0);
            self->rumbleTimer = 1;
            self->explosionElapsed = 0;
            if (((Level *)self->level)->getPlayer() != 0) {
                void *ego = (void *)(intptr_t)((Level *)self->level)->getPlayer();
                if (((PlayerEgo *)(ego))->getTargetFollowCamera() != 0) {
                    ego = (void *)(intptr_t)((Level *)self->level)->getPlayer();
                    void *cam = (void *)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getTargetFollowCamera();
                    char cp[12];
                    TargetFollowCamera_getPosition((Vector *)cp, cam);
                    *(Vector *)cp -= *(const Vector *)((char *)self + 0x2c);
                    float len = AbyssEngine::AEMath::VectorLength(*(const Vector *)cp);
                    float maxd = 50.0f;
                    float use = (len < maxd) ? len : maxd;
                    self->rumblePercentage = 1.0f - use / maxd;
                    ego = (void *)(intptr_t)((Level *)self->level)->getPlayer();
                    cam = (void *)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getTargetFollowCamera();
                    TargetFollowCamera_setRumblePercentage(self->rumblePercentage, cam);
                }
            }
        }
    } else if (state == 4) {
        // exploding
        self->explosionElapsed = self->explosionElapsed + dt;
        if (self->explosion != 0)
            Explosion_update(self->explosion, dt, 0);
        self->explosionTimer = self->explosionTimer + dt;

        bool spin = self->hasCargo != 0 && ((Player *)(self->player))->isActive() != 0 &&
                    self->secondaryGeometry != 0;
        if (spin) {
            float r = (float)(dt >> 1) * 0.001f; // DAT scalings
            r = (float)(int)(r * 1.0f);
            ((AEGeometry *)(self->secondaryGeometry))->rotate(r, r, r);
            if (self->explosionTimer >= 0xea61) {
                self->finished = 1;
            }
        } else {
            if (self->explosion != 0 && ((Explosion *)(self->explosion))->isPlaying() == 0) {
                if (self->explosionTimer >= 0xea61)
                self->finished = 1;
            }
        }

        if (self->wreckType >= 0) {
            if (self->wreckCollision != 0 && self->explosionElapsed >= 0x8d &&
                (unsigned int)self->wreckMaterial <= 0x7fffffff) {
                char posBuf[12];
                ((AEGeometry *)((Vector *)posBuf))->getPosition();
                *(Vector *)((Vector *)((char *)self + 0x2c)) = *(const Vector *)((Vector *)posBuf);
                Array<void *> *bv = (Array<void *> *)self->wreckCollision;
                for (unsigned int i = 0; i < bv->size(); i++) {
                    void *o = bv->data()[i];
                    typedef void (*BVFn)(void *, float, float, float);
                    BVFn fn = *(BVFn *)(*(char **)o + 0x4);
                    fn(o, self->posX, self->posY, self->posZ);
                }
                unsigned short mat;
                switch ((unsigned int)self->wreckType) {
                    case 1: mat = 0x8248; break;
                    case 2: mat = 0x8249; break;
                    case 3: mat = 0x824a; break;
                    case 4: mat = 0x824b; break;
                    default: mat = 0x824d; break;
                }
                self->wreckMaterial = mat;
                void *pc = *g_pfo_pcMaterial;
                char matOut[4];
                ((PaintCanvas*)pc)->MaterialCreate(mat, (unsigned int *)matOut);
                ((PaintCanvas*)pc)->MeshChangeMaterial(
                    *(unsigned int *)((char *)self->wreckGeometry + 0x1c),
                    *(unsigned short *)matOut);
            }
            // rumble ramp
            if (((Level *)self->level)->getPlayer() != 0) {
                void *ego = (void *)(intptr_t)((Level *)self->level)->getPlayer();
                void *cam = (void *)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getTargetFollowCamera();
                if (cam != 0 && self->rumbleTimer > 0) {
                    int v = self->rumbleTimer + dt;
                    if (v >= 0x7d0) v = 0x7d0;
                    self->rumbleTimer = v;
                    ego = (void *)(intptr_t)((Level *)self->level)->getPlayer();
                    cam = (void *)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getTargetFollowCamera();
                    TargetFollowCamera_setRumblePercentage(
                        self->rumblePercentage * ((float)v / 50.0f + 1.0f), cam);
                    if (self->explosion != 0 &&
                        ((Explosion *)(self->explosion))->isPlaying() == 0) {
                        ego = (void *)(intptr_t)((Level *)self->level)->getPlayer();
                        cam = (void *)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getTargetFollowCamera();
                        TargetFollowCamera_setRumblePercentage(0.0f, cam);
                        self->rumbleTimer = 0;
                    }
                }
            }
        }
    } else if (state == 5) {
        // dead-but-selectable: search for a nearby active enemy to re-home on
        unsigned int *enemies = (unsigned int *)Player_getEnemies();
        if (enemies != 0) {
            self->targetEnemy = 0;
            for (unsigned int i = 0; i < enemies[0]; i++) {
                if (((Player *)(((Player *)(self->player))->getEnemy(i)))->isActive() != 0) {
                    char pb[12];
                    Player_getPosition((Vector *)pb);
                    *(Vector *)((Vector *)((char *)self + 0x90)) = *(const Vector *)((Vector *)pb);
                    float dx = self->posX - self->targetX;
                    float dy = self->posY - self->targetY;
                    float dz = self->posZ - self->targetZ;
                    const float lo = 0.0f, hi = 50.0f; // DAT thresholds
                    if (dx < hi && dx > lo && dy < hi && dy > lo && dz < hi && dz > lo) {
                        self->targetEnemy = (int32_t)(__INTPTR_TYPE__)((Player *)(self->player))->getEnemy(i);
                        Player_getPosition((Vector *)pb);
                        *(Vector *)((Vector *)((char *)self + 0x90)) = *(const Vector *)((Vector *)pb);
                        self->field_0x144 = self->targetX;
                        self->field_0x148 = self->targetY;
                        self->field_0x14c = self->targetZ;
                        break;
                    }
                }
            }
        }
        float vx = self->field_0x144 - self->posX;
        float vy = self->field_0x148 - self->posY;
        float vz = self->field_0x14c - self->posZ;
        self->field_0x150 = vx;
        self->field_0x154 = vy;
        self->field_0x158 = vz;
        const float lo = 0.0f, hi = 50.0f;
        if (vx < hi && vx > lo && vz > lo && vy < hi && vy > lo && vz < hi) {
            self->state = 1;
            ((Player *)(self->player))->setActive((bool)(unsigned char)(long)self->player);
        }
    }

    // mirror the integer position into the Player object
    void *p = self->player;
    *(int *)((char *)p + 0x48) = self->intPosX;
    *(int *)((char *)p + 0x4c) = self->intPosY;
    *(int *)((char *)p + 0x50) = self->intPosZ;
}

// ---- setExhaustVisible_154cb0.cpp ----
// PaintCanvas singleton holder (single pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_pfo_canvas;

// NEAR: target keeps the frame up-front (no shrink-wrap, bool saved to r4 before the
// field checks). clang shrink-wraps here (checks before push, bx lr early-exit).
void PlayerFixedObject::setExhaustVisible(bool v) {
    PlayerFixedObject *self = this;
    void *geom = self->geometry;
    if (geom != 0 && *(int *)((char *)geom + 0x14) != -1) {
        void **holder = g_pfo_canvas;
        return Transform_setExhaustVisible(
            ((PaintCanvas*)*holder)->TransformGetTransform(*(int *)((char *)geom + 0xc)), v);
    }
}

// ---- collide_154d4a.cpp ----
// collide(float,float,float): iterate the active bounding-volume array, calling each
// volume's vtable slot +8 with (bv, x, y, z); return 1 on first hit, else 0.
// NEAR: clang rotates the first loop as break-test (cbnz) while the target uses
// continue-test (cmp;beq) there; the two loops differ only in rotation.
typedef int (*CollideFn)(void *bv, float, float, float);

int PlayerFixedObject::collide(float x, float y, float z) {
    PlayerFixedObject *self = this;
    Array<void *> *a = (Array<void *> *)self->wreckCollision;
    if ((a != 0 || self->state != 4) && self->field_0x8c != 0) {
        if (a != 0 && self->state == 4) {
            for (uint32_t i = 0; i < a->size(); i++) {
                void *bv = a->data()[i];
                CollideFn fn = *(CollideFn *)(*(char **)bv + 0x8);
                if (fn(bv, x, y, z) != 0) return 1;
                a = (Array<void *> *)self->wreckCollision;
            }
        } else {
            Array<void *> *b = (Array<void *> *)self->boundingVolumes;
            if (b != 0) {
                for (uint32_t i = 0; i < b->size(); i++) {
                    void *bv = b->data()[i];
                    CollideFn fn = *(CollideFn *)(*(char **)bv + 0x8);
                    if (fn(bv, x, y, z) != 0) return 1;
                    b = (Array<void *> *)self->boundingVolumes;
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
void PlayerFixedObject::setBV(BoundingVolume *bv) {
    PlayerFixedObject *self = this;
    void *arr = ::operator new(0xc);
    Array_BV_ctor(arr);
    self->boundingVolumes = arr;
    return BoundingVolume_setArr(bv, arr);
}

// ---- reset_153e24.cpp ----
typedef unsigned long long uint64_t;


typedef void (*SetPosFn)(PlayerFixedObject *, float, float, float);

// PC-relative pointer-to-function-pointer; *holder is a Vector::operator= style routine
// invoked as fn(Vector*, Vector*).
typedef void (*VecAssignFn)(void *dst, void *src);
__attribute__((visibility("hidden"))) extern VecAssignFn *g_pfo_vecAssignZero;

void PlayerFixedObject::reset() {
    PlayerFixedObject *self = this;
    ((KIPlayer *)(self))->reset();

    // vtable slot 0x48 -> setPosition(this->0x58, 0x5c, 0x60)
    SetPosFn setPos = *(SetPosFn *)(*(char **)self + 0x48);
    setPos(self, self->spawnX, self->spawnY, self->spawnZ);

    self->targetEnemy = 0;

    // Vector copy: 0x58 -> 0x138
    {
        char buf[12];
        *(uint64_t *)buf = self->spawnX;
        *(uint32_t *)(buf + 8) = self->spawnZ;
        *(Vector *)((Vector *)((char *)self + 0x138)) = *(const Vector *)((Vector *)buf);
    }
    // Vector copy: 0x138 -> 0x2c
    {
        char buf[12];
        *(uint64_t *)buf = self->field_0x138;
        *(uint32_t *)(buf + 8) = self->field_0x140;
        *(Vector *)((Vector *)((char *)self + 0x2c)) = *(const Vector *)((Vector *)buf);
    }

    self->deltaTime = 0;
    if (self->state != 5)
        self->state = 0;

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

void PlayerFixedObject::setWreckedMeshId(int meshId) {
    PlayerFixedObject *self = this;
    self->wreckMeshId = (uint16_t)meshId;
    void *geom = ::operator new(0xc0);
    void **holder = g_pfo_canvas2;
    new ((void *)geom) AEGeometry((uint16_t)meshId, (PaintCanvas *)*holder, true);
    self->wreckGeometry = geom;
    void *t = ((PaintCanvas*)*holder)->TransformGetTransform(*(int *)((char *)geom + 0xc));
    *(int *)((char *)t + 0xe0) = 0x48f42400; // 500000.0f far-clip constant (raw bits)

    int kind = self->kind;
    int sel;
    if (kind == 0xd) {
        sel = 4;
    } else if (kind == 0xe) {
        sel = 0;
    } else if (kind == 0xf) {
        if (self->faction == 3) sel = 1;
        else sel = (self->faction == 2) ? 2 : 3;
    } else if (kind == 0x37a3) {
        sel = 5;
    } else {
        sel = self->wreckType;
        if (sel < 0) return;
        self->wreckCollision = Globals_getWreckCollision(**g_pfo_globals, sel, self->wreckGeometry);
        return;
    }
    self->wreckType = sel;
    self->wreckCollision = Globals_getWreckCollision(**g_pfo_globals, sel, self->wreckGeometry);
}

// ---- getProjectionVector_154dd6.cpp ----
// Returns a Vector by value (sret r0, this r1). Picks the active bounding-volume array,
// indexes it by the stored collision index (this+0x16c) and forwards the chosen BV.
// NEAR: target shares one index+call across both branches (explicit join) and keeps the
// first bv in a callee reg; clang lays the branches out separately here.
V3 PlayerFixedObject::getProjectionVector() {
    PlayerFixedObject *self = this;
    void *bv = self->wreckCollision;
    if (bv != 0 && self->state == 4) {
        int idx = self->collisionIndex;
        void **data = *(void ***)((char *)bv + 0x4);
        return BV_getProjectionVector(data[idx]);
    }
    void *bv2 = self->boundingVolumes;
    if (bv2 != 0) {
        int idx = self->collisionIndex;
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

void *_ZN17PlayerFixedObjectD1Ev(PlayerFixedObject *self)
{
    void *wreck = self->wreckGeometry;
    *(void **)self = &PlayerFixedObject_vtable + 8;
    if (wreck != self->geometry) {
        if (wreck != 0) { ((AEGeometry *)wreck)->~AEGeometry(); ::operator delete(wreck); }
        self->wreckGeometry = 0;
    }
    void *bvB = self->boundingVolumes;
    if (bvB != 0) {
        ArrayReleaseClasses_BV(bvB);
        void *b = self->boundingVolumes;
        if (b != 0) ::operator delete(Array_BV_dtor(b));
    }
    self->boundingVolumes = 0;
    void *bvA = self->wreckCollision;
    if (bvA != 0) {
        ArrayReleaseClasses_BV(bvA);
        void *a = self->wreckCollision;
        if (a != 0) ::operator delete(Array_BV_dtor(a));
    }
    self->wreckCollision = 0;
    void *expl = self->explosion;
    if (expl != 0) ::operator delete(Explosion_dtor(expl));
    self->explosion = 0;
    ((String *)((char *)self + 0x1ac))->dtor();
    return PlayerFixedObject_baseDtor(self);
}

// ---- render_154cdc.cpp ----
// Tail-call thunks selected by object state.
extern "C" void render_thunk_state5(void *geom);   // DAT_001abdd4 thunk, arg = this->0x8
extern "C" void render_thunk_other(void *expl);     // DAT_001ac2b4 thunk, arg = this->0x18c (Explosion*)

// NEAR: clang reorders the state comparisons (3 before 5) and duplicates the shared
// state5 block instead of sharing it via the original goto; structure differs.
void PlayerFixedObject::render() {
    PlayerFixedObject *self = this;
    void *g78 = self->secondaryGeometry;
    if (g78 != 0 && self->shipHidden == 0) {
        ((AEGeometry *)(g78))->render();
    }
    int state = self->state;
    void *expl;
    if (state == 5) {
LAB_538:
        if (self->shipHidden != 0) return;
        return render_thunk_state5(self->geometry);
    }
    if (state == 4) {
        if (self->shipHidden == 0) ((AEGeometry *)(self->wreckGeometry))->render();
        expl = self->explosion;
        if (expl == 0) return;
    } else {
        if (state != 3) {
            if (((Player *)(self->player))->isActive() == 0) return;
            goto LAB_538;
        }
        if (self->shipHidden == 0) ((AEGeometry *)(self->wreckGeometry))->render();
        expl = self->explosion;
    }
    return render_thunk_other(expl);
}

// ---- PlayerFixedObject_153a7c.cpp ----
typedef unsigned long long uint64_t;

// PlayerFixedObject::PlayerFixedObject(int kind, int param2, Player*, AEGeometry*, float, float, float, ...)
// (two extra stack floats in_stack_00000004/8/c make up the spawn position passed by value).




// PC-relative literal table holding the station-index match list (4 ints) and the
// loot-list parameter table (4 x 8-byte pairs).
__attribute__((visibility("hidden"))) extern const int g_pfo_stationIdx[4];
__attribute__((visibility("hidden"))) extern const int g_pfo_lootParams[8]; // pairs at +0 used; [idx*2+1]
// AERandom singleton holder (pc-rel -> holder; *holder is the AERandom object).
__attribute__((visibility("hidden"))) extern void **g_pfo_random;

void PlayerFixedObject::ctor(int kind, int param2, void *player, void *geom, float p5, float p6, float p7, float sx, float sy, float sz) {
    PlayerFixedObject *self = this;

    // Three zeroed Vector4 (16-byte) blocks.
    self->field_0x158 = 0; self->field_0x160 = 0;
    self->field_0x148 = 0; self->field_0x150 = 0;
    self->field_0x138 = 0; self->field_0x140 = 0;

    *(void **)self = &PlayerFixedObject_vtable + 8;

    String_ctor_empty((char *)self + 0x1ac);
    self->explosion = 0;
    self->wreckMeshId = 0;
    self->faction = param2;
    self->wreckGeometry = 0;
    self->boundingVolumes = 0;
    self->wreckCollision = 0;
    self->deltaTime = 0;
    self->targetEnemy = 0;
    self->collisionIndex = 0;
    self->field_0x170 = 0;
    self->shipHidden = 0;
    self->explosionElapsed = 0;
    self->field_0x174 = 0;
    self->intPosX = 0; self->intPosY = 0; self->intPosZ = 0;

    // Vector::operator=(this+0x2c, {sx,sy,sz})
    {
        char buf[12];
        *(float *)(buf + 0) = sx; *(float *)(buf + 4) = sy; *(float *)(buf + 8) = sz;
        *(Vector *)((Vector *)((char *)self + 0x2c)) = *(const Vector *)((Vector *)buf);
    }

    self->moving = 0;
    self->wreckType = -1;
    self->wreckMaterial = -1;
    self->dockingType = 0;
    self->intPosX = (int32_t)sx;
    self->intPosY = (int32_t)sy;
    self->intPosZ = (int32_t)sz;

    // Name string from a fixed literal.
    {
        char tmp[12];
        String_ctor_cstr(tmp, "", false);
        String_op_assign((char *)self + 0x1ac, tmp);
        ((String *)(tmp))->dtor();
    }

    void *mission = ((Status *)(*g_pfo_status))->getMission();
    int campaign = ((Mission *)(mission))->isCampaignMission();
    bool special = false;
    if (campaign != 0) {
        int cm = ((Status *)(*g_pfo_status))->getCurrentCampaignMission();
        if (cm == 0x28) special = true;
        else {
            cm = ((Status *)(*g_pfo_status))->getCurrentCampaignMission();
            if (cm == 0x29) special = true;
        }
    }

    if (special) {
        if (self->lootList != 0) {
            ::operator delete(Array_int_dtor((Array<int> *)self->lootList));
        }
        self->lootList = 0;
    } else {
        void *gen = ::operator new(1);
        Generator_ctor(gen);
        if (kind == 0x37a3) {
            self->field_0x41 = 1;
            void *station = ((Status *)(*g_pfo_status))->getStation();
            int sidx = Station_getIndex((Station *)station);
            for (uint32_t i = 0; i < 4; i++) {
                if (g_pfo_stationIdx[i] == sidx) {
                    void *loot = ((Generator *)(gen))->getLootList(g_pfo_lootParams[i * 2 + 1], 0);
                    self->lootList = loot;
                }
            }
        } else {
            uint32_t *loot = (uint32_t *)((Generator *)(gen))->getLootList(-1, -1);
            self->lootList = loot;
            if (loot != 0) {
                int second = (kind != 0x498e) ? 0x4a88 : 0x498e;
                if (kind != 0x498e && kind != second) {
                    void *rng = *g_pfo_random;
                    for (int idx = 1; (uint32_t)(idx - 1) < loot[0]; idx += 2) {
                        // NOTE: AERandom::nextInt(self, bound) had its `bound`
                        // argument dropped by the decompiler; the receiver `rng`
                        // is preserved. Exact bounds are unrecovered (Ghidra
                        // unavailable) — using the additive offsets as bounds.
                        if (kind == 0xe) {
                            int r = AERandom_nextInt((AbyssEngine::AERandom *)rng, 5);
                            loot = (uint32_t *)self->lootList;
                            int *cell = (int *)(loot[1] + idx * 4);
                            *cell = *cell * (r + 5);
                        } else {
                            int r = AERandom_nextInt((AbyssEngine::AERandom *)rng, 2);
                            int *base = (int *)(*(int *)((char *)self->lootList + 4) + idx * 4);
                            *base = *base * (r + 2);
                            int r2 = AERandom_nextInt((AbyssEngine::AERandom *)rng, 8);
                            loot = (uint32_t *)self->lootList;
                            int *cell = (int *)(loot[1] + idx * 4);
                            int v = *cell;
                            if (v < r2 + 8) v = r2 + 8;
                            *cell = v;
                        }
                    }
                }
            }
        }
        ::operator delete(Generator_dtor(gen));
    }

    *(uint8_t *)((char *)self->player + 0x45) = 1;
    if (kind != 0x37a3) {
        self->field_0xf8 = 0x2f;
        if (kind == 0xe) {
            self->field_0xf8 = -1;
            self->moving = 0;
        }
    }
}

// ---- setPosition_153eec.cpp ----
typedef void (*BVSetPosFn)(void *bv, float, float, float);

// PlayerFixedObject::setPosition(float, float, float)
void PlayerFixedObject::setPosition3(float x, float y, float z) {
    PlayerFixedObject *self = this;
    self->spawnX = x;
    self->spawnY = y;
    self->spawnZ = z;
    self->intPosX = (int32_t)x;
    self->intPosY = (int32_t)y;
    self->intPosZ = (int32_t)z;

    Vector posVec = {x, y, z};
    ((AEGeometry *)(self->geometry))->setPosition(posVec);
    void *m = (void *)&((AEGeometry *)(self->geometry))->getMatrix();
    *(Matrix *)((char *)self->player + 0x4) = *(const Matrix *)(m);

    char buf[12];
    ((AEGeometry *)((Vector *)buf))->getPosition();
    *(Vector *)((Vector *)((char *)self + 0x2c)) = *(const Vector *)((Vector *)buf);

    Array<void *> *bv = (Array<void *> *)self->boundingVolumes;
    if (bv != 0) {
        for (uint32_t i = 0; i < bv->size(); i++) {
            void *o = bv->data()[i];
            BVSetPosFn fn = *(BVSetPosFn *)(*(char **)o + 0x4);
            fn(o, self->posX, self->posY, self->posZ);
            bv = (Array<void *> *)self->boundingVolumes;
        }
    }

    if (self->wreckGeometry != 0) {
        ((AEGeometry *)(self->wreckGeometry))->setPosition(posVec);
    }
}

// ---- setDeadButSelectable_154f24.cpp ----
// NEAR (80.6%): structure matches, but the final Transform::SetAnimationRangeInTime call is
// emitted as a tail-branch here while the target keeps a frame and passes the 64-bit arg on the
// stack (push {r2,r3,...}; strd; call; pop). Calling-convention detail not reproducible from src.
// PaintCanvas singleton holder (pc-rel -> holder; *holder is the canvas).
__attribute__((visibility("hidden"))) extern void **g_pfo_canvas3;

void PlayerFixedObject::setDeadButSelectable() {
    PlayerFixedObject *self = this;
    void *player = self->player;
    self->moving = 0;
    ((Player *)(player))->setHitpoints(1);
    ((Player *)(self->player))->setVulnerable(false);
    ((LODManager *)(*(void **)self->level))->removeObject((AEGeometry *)self->geometry);
    void *geom = self->geometry;
    if (geom != 0) { ((AEGeometry *)geom)->~AEGeometry(); ::operator delete(geom); }
    void **holder = g_pfo_canvas3;
    void *newGeom = self->wreckGeometry;
    self->geometry = newGeom;
    void *t = ((PaintCanvas*)*holder)->TransformGetTransform(*(int *)((char *)newGeom + 0xc));
    // NOTE: the decompiler emitted a single 64-bit argument; SetAnimationRangeInTime
    // takes (start, end). Passing the recovered value as start and 0 as end.
    ((AbyssEngine::Transform *)(t))->SetAnimationRangeInTime(*(long long *)((char *)t + 0xf8), 0);
}

// ---- outerCollide_154e5a.cpp ----
// outerCollide(float,float,float): iterate the active bounding-volume array, calling each
// volume's vtable slot +0xc with (bv, x, y, z). On the first hit, store the hit index at
// this+0x16c and return 1; otherwise return 0.
typedef int (*CollideFn)(void *bv, float, float, float);

int PlayerFixedObject::outerCollide(float x, float y, float z) {
    PlayerFixedObject *self = this;
    Array<void *> *a = (Array<void *> *)self->wreckCollision;
    if ((a != 0 || self->state != 4) && self->field_0x8c != 0) {
        if (a != 0 && self->state == 4) {
            for (uint32_t i = 0; i < a->size(); i++) {
                void *bv = a->data()[i];
                CollideFn fn = *(CollideFn *)(*(char **)bv + 0xc);
                if (fn(bv, x, y, z) != 0) { self->collisionIndex = i; return 1; }
                a = (Array<void *> *)self->wreckCollision;
            }
        } else {
            Array<void *> *b = (Array<void *> *)self->boundingVolumes;
            if (b != 0) {
                for (uint32_t i = 0; i < b->size(); i++) {
                    void *bv = b->data()[i];
                    CollideFn fn = *(CollideFn *)(*(char **)bv + 0xc);
                    if (fn(bv, x, y, z) != 0) { self->collisionIndex = i; return 1; }
                    b = (Array<void *> *)self->boundingVolumes;
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

void PlayerFixedObject::moveForward(int amount) {
    PlayerFixedObject *self = this;
    float d = (float)amount;
    self->intPosZ = amount + self->intPosZ;
    ((AEGeometry *)(self->geometry))->moveForward(d);
    void *m = ((AEGeometry *)(self->geometry))->getMatrix();
    *(Matrix *)((char *)self->player + 0x4) = *(const Matrix *)(m);
    char buf[12];
    ((AEGeometry *)((Vector *)buf))->getPosition();
    *(Vector *)((Vector *)((char *)self + 0x2c)) = *(const Vector *)((Vector *)buf);
    if (self->wreckGeometry != 0) {
        ((AEGeometry *)(self->wreckGeometry))->moveForward(d);
    }
    Array<void *> *bv = (Array<void *> *)self->boundingVolumes;
    if (bv != 0) {
        for (uint32_t i = 0; i < bv->size(); i++) {
            void *o = bv->data()[i];
            BVMoveFn fn = *(BVMoveFn *)(*(char **)o + 0x4);
            Vector pos = { self->posX, self->posY, self->posZ };
            fn(o, pos);
            bv = (Array<void *> *)self->boundingVolumes;
        }
    }
}
