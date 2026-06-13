#include <new>
#include "gof2/game/ship/PlayerGasCloud.h"
#include "gof2/engine/core/AERandom.h"
#include "gof2/game/ship/Ship.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/mission/Status.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/game/ui/Hud.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/mission/Mission.h"
#include "gof2/game/ship/Player.h"
#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/game/core/PaintCanvasClass.h"

// AEMath free operators used below; declared minimally to avoid pulling in AEMath.h.
namespace AbyssEngine { namespace AEMath {
Vector operator-(const Vector &a, const Vector &b);
} }

// Status singleton: getShip/getCurrentCampaignMission are real Status methods
// that the decompiler emitted without their receiver. The receiver is the
// global Status singleton, reached via *gStatus (same pattern as Hud/Player).
__attribute__((visibility("hidden"))) extern Status **gStatus;

extern "C" char PlayerGasCloud_vtable;
extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);

void PlayerGasCloud::translate(Vector const &param_1)
{
    return ((AEGeometry *)this->geometry)->translate(param_1);
}

bool PlayerGasCloud::isSparkAlive(int param_1)
{
    Array<AEGeometry*> *arr = this->sparkGeometries;
    if (arr == 0)
        return false;
    if (arr->size() <= (unsigned int)param_1)
        return false;
    return -1500 < (*this->sparkTimers)[param_1];
}

void PlayerGasCloud::setSparkInSight(int param_1, bool param_2)
{
    Array<AEGeometry*> *arr = this->sparkGeometries;
    if (arr == 0)
        return;
    if ((unsigned int)param_1 < arr->size()) {
        (*this->sparkInSight)[param_1] = param_2;
    }
}

void PlayerGasCloud::setPosition(Vector const &param_1)
{
    return ((AEGeometry *)this->geometry)->setPosition(param_1);
}

void *PlayerGasCloud::getSparks()
{
    return this->sparkGeometries;
}

Vector PlayerGasCloud::getPosition()
{
    return ((AEGeometry *)this->geometry)->getPosition();
}

// Deleting destructor (D0): run the complete dtor (D1), then tail-call operator delete.
void *_ZN14PlayerGasCloudD1Ev(void *self); // complete object dtor

void _ZN14PlayerGasCloudD0Ev(void *self)
{
    return ::operator delete(_ZN14PlayerGasCloudD1Ev(self));
}

uint8_t PlayerGasCloud::hasExploded()
{
    return this->exploded;
}

__attribute__((visibility("hidden"))) extern void **g_pgc_canvas;

PlayerGasCloud::PlayerGasCloud(int param_1, ParticleSystemManager *param_2, AEGeometry *param_3,
                               Vector const &param_4)
{
    void *this_00 = ::operator new(0x114);
    ((Player *)(this_00))->ctor(0, 9999999, 0, 0, 0);

    *(void **)this = &PlayerGasCloud_vtable + 8;
    this->center.x = 0;
    this->center.y = 0;
    this->center.z = 0;
    ((Player *)(this->player))->setKIPlayer((KIPlayer *)this);
    ((Player *)(this->player))->setMaxHitpoints(1);

    void **vt = *(void ***)this;
    typedef void (*pfn)(void *, Vector const &);
    ((pfn)vt[0x11])(this, param_4);

    this->elapsedSinceExplosion = 0;
    this->field_0x78 = 0;
    this->field_0x25 = 0;

    int iVar1 = 0x4a35;
    if ((unsigned int)(param_1 - 0xc9) < 4)
        iVar1 = param_1 + 0x496c;
    int iVar2 = 0x4a39;
    if ((unsigned int)(param_1 - 0xc9) < 4)
        iVar2 = param_1 + 0x4970;
    this->itemId = param_1;
    this->sparkMeshId = iVar2;
    this->cloudMeshId = iVar1;

    // Two 16-byte vector stores clear the spark-array pointer block (0x138..0x154).
    this->sparkGeometries = 0;
    this->sparkVelocities = 0;
    this->sparkLife = 0;
    this->sparkLifeMin = 0;
    this->field_0x145 = 0;
    this->sparkScale = 0;
    this->sparkTimers = 0;
    this->sparkInSight = 0;
    this->exploded = 0;

    this->center = param_4;

    void *geom = ::operator new(0xc0);
    new (geom) AEGeometry(this->cloudMeshId, (PaintCanvas *)*g_pgc_canvas, false);
    this->modelGeometry = geom;
    ((AEGeometry *)geom)->setPosition(param_4);

    this->field_0x4c = 1;
    this->field_0x44 = 1;
    this->state = 0;
    this->active = 1;
    this->settled = 0;
}

// Complete object destructor (D1). Sets the vtable, tears down the spark arrays and the
// base geometry, then tail-calls the base destructor.

void *_ZN14PlayerGasCloudD1Ev(void *selfv)
{
    PlayerGasCloud *self = (PlayerGasCloud *)selfv;
    *(void **)self = &PlayerGasCloud_vtable + 8;

    if (self->sparkGeometries != 0) {
        for (AEGeometry *g : *self->sparkGeometries)
            delete g;
        self->sparkGeometries->clear();
        delete self->sparkGeometries;
        self->sparkGeometries = 0;
    }

    if (self->sparkVelocities != 0) {
        for (Vector *v : *self->sparkVelocities)
            delete v;
        self->sparkVelocities->clear();
        delete self->sparkVelocities;
        self->sparkVelocities = 0;
    }

    delete self->sparkLife;
    self->sparkLife = 0;

    delete self->sparkLifeMin;
    self->sparkLifeMin = 0;

    delete self->sparkScale;
    self->sparkScale = 0;

    delete self->sparkTimers;
    self->sparkTimers = 0;

    void *g = self->modelGeometry;
    if (g != 0) {
        ((AEGeometry *)g)->~AEGeometry();
        ::operator delete(g);
    }
    self->modelGeometry = 0;

    return ((PlayerGasCloud *)(self))->baseDtor();
}

// PlayerGasCloud::~PlayerGasCloud() -- real complete-object destructor.
//   Resets the vtable slot, releases every spark Array<T> (releasing the
//   contained class instances first where the elements are owning pointers),
//   destroys the cloud model geometry, then chains to the base-class
//   destructor (the gas cloud derives from a base game-object whose dtor is
//   reached through PlayerGasCloud_baseDtor).
PlayerGasCloud::~PlayerGasCloud()
{
    *(void **)this = &PlayerGasCloud_vtable + 8;

    if (this->sparkGeometries != 0) {
        for (AEGeometry *g : *this->sparkGeometries)
            delete g;
        this->sparkGeometries->clear();
        delete this->sparkGeometries;
        this->sparkGeometries = 0;
    }

    if (this->sparkVelocities != 0) {
        for (Vector *v : *this->sparkVelocities)
            delete v;
        this->sparkVelocities->clear();
        delete this->sparkVelocities;
        this->sparkVelocities = 0;
    }

    delete this->sparkLife;
    this->sparkLife = 0;

    delete this->sparkLifeMin;
    this->sparkLifeMin = 0;

    delete this->sparkScale;
    this->sparkScale = 0;

    delete this->sparkTimers;
    this->sparkTimers = 0;

    if (this->modelGeometry != 0) {
        ((AEGeometry *)this->modelGeometry)->~AEGeometry();
        ::operator delete(this->modelGeometry);
    }
    this->modelGeometry = 0;

    ((PlayerGasCloud *)(this))->baseDtor();
}

// ~PlayerGasCloud() tail (binary: tail-call to SpacePoint::~SpacePoint @0x732b4).
//   PlayerGasCloud derives from the SpacePoint base game-object; once the cloud's own
//   spark arrays and model geometry have been released, the base subobject is torn
//   down here. SpacePoint owns no heap members (its destructor only re-seats the
//   vtable, work already done by the derived dtor above), so the base teardown reduces
//   to yielding the object pointer back to the deleting destructor that frees it.
void *PlayerGasCloud::baseDtor()
{
    return this;
}

using AbyssEngine::AEMath::Vector;

struct PlayerGasCloud;

// --- engine callees ---------------------------------------------------------
extern "C" {

// Vector math helpers.
float VectorLength(const Vector *v);
void VectorNormalize(Vector *out, const Vector *v);

// Globals reached through GOT.
extern void *g_pgc_itemList;     // **(DAT_186954): root item table
extern void *g_pgc_canvasRoot;   // *(DAT_186958): paint canvas
extern void *g_pgc_rng;          // *(DAT_18695c): random generator

// Tuning constants (DAT_ floats).
extern float g_pgc_countScale;
extern float g_pgc_attrDiv;
extern float g_pgc_spread;
extern float g_pgc_lifeDiv;
}

// PlayerGasCloud::explode(int itemIndex, Vector src, float radius)
void PlayerGasCloud_explode(void *selfv, int itemIndex, Vector src, float radius)
{
    PlayerGasCloud *self = (PlayerGasCloud *)selfv;

    if (self->exploded == 0) {
        self->state = 3;
        self->exploded = 1;

        self->sparkGeometries = new Array<AEGeometry*>();
        self->sparkVelocities = new Array<Vector*>();
        self->sparkLife = new Array<float>();
        self->sparkLifeMin = new Array<float>();
        self->sparkTimers = new Array<int>();
        self->sparkInSight = new Array<bool>();
        self->sparkScale = new Array<float>();

        // Distance from the explosion source to the cloud centre.
        Vector delta;
        delta = src - self->center;
        float dist = VectorLength(&delta);
        float t = 1.5f - dist / radius;
        float countBase = t * g_pgc_countScale;

        // Item-defined intensity multiplier.
        void *itemTable = *(void **)((char *)*(void **)g_pgc_itemList);
        void *item = *(void **)((char *)*(void **)((char *)itemTable + 4) + itemIndex * 4);
        int attr = ((Item *)(item))->getAttribute(0);
        float attrF = (float)attr / g_pgc_attrDiv;
        float spread = t * g_pgc_spread;
        float lifeDiv = g_pgc_lifeDiv;

        int count = (int)((countBase / 1.5f + 10.0f) * attrF);
        for (int i = 0; i < count; i++) {
            void *shard = ::operator new(0xc0);
            new (shard) AEGeometry(self->sparkMeshId,
                            (PaintCanvas *)*(void **)g_pgc_canvasRoot, false);
            ((AEGeometry *)(shard))->setPosition(self->center);

            void *rng = *(void **)g_pgc_rng;
            float jx = (float)((AbyssEngine::AERandom *)(rng))->next(10000);
            float jy = (float)((AbyssEngine::AERandom *)(rng))->next(10000);
            float jz = (float)((AbyssEngine::AERandom *)(rng))->next(10000);

            Vector p;
            p.x = ((self->center.x + delta.x) - spread) + t * jx;
            p.y = ((self->center.y + delta.y) - spread) + t * jy;
            p.z = ((self->center.z + delta.z) - spread) + t * jz;

            // Direction = normalized (p - center).
            Vector d, dn;
            d = p - self->center;
            VectorNormalize(&dn, &d);

            float life = ((float)((AbyssEngine::AERandom *)(rng))->next(200) / lifeDiv) * 3.0f + 3.0f;
            int timer = ((AbyssEngine::AERandom *)(rng))->next(14000);

            self->sparkLife->push_back(life * 7.0f);
            self->sparkLifeMin->push_back(life);
            self->sparkTimers->push_back(timer + 8000);

            Vector *velCopy = (Vector *)::operator new(0xc);
            *velCopy = dn;
            self->sparkVelocities->push_back(velCopy);
            self->sparkGeometries->push_back((AEGeometry *)shard);
            self->sparkInSight->push_back(false);
            self->sparkScale->push_back(1.0f);
        }
    }

    
}

using AbyssEngine::AEMath::Vector;

// --- engine callees ---------------------------------------------------------
extern "C" {

void *PlayerEgo_getTurretPosition(void *ego, Vector *out);
int PlayerEgo_getCampaignProgress(void *ego);

// Vector math.
void Vector_sub(Vector *out, const Vector *a, const Vector *b);
void Vector_scale(float s, Vector *inout);      // operator*(float, Vector&)
void Vector_add(Vector *out, const Vector *b);  // operator+(out, b) -> out += b
float VectorLength(const Vector *v);
void VectorNormalize(Vector *out, const Vector *v);

// GOT globals.
extern void *g_pgcu_canvasRoot;   // *(DAT_186d78 / DAT_186e14): paint canvas
extern void *g_pgcu_itemDefs;     // *(DAT_186e08): item-definition table
extern void *g_pgcu_pickupSound;  // *(DAT_186e0c): pickup FModSound
extern void *g_pgcu_campaign;     // *(DAT_186e10): campaign state

// Tuning constants.
extern float g_pgcu_velScale;
extern float g_pgcu_catchDist;
extern int   g_pgcu_minTimer;
extern int   g_pgcu_resetTimer;
extern float g_pgcu_fadeLo;
extern float g_pgcu_fadeAdd;
extern float g_pgcu_fadeDiv;
extern float g_pgcu_growDiv;
}

// PlayerGasCloud::update(int dt)
void PlayerGasCloud_update(void *selfv, int dt)
{
    PlayerGasCloud *self = (PlayerGasCloud *)selfv;

    if (dt != 0) {
        Array<AEGeometry*> *arr = self->sparkGeometries;
        if (self->exploded == 0 || self->settled != 0 || arr == 0) {
            // Idle / pre-explosion: just advance the bound transform.
            void *t = ((PaintCanvas *)*(void **)g_pgcu_canvasRoot)->TransformGetTransform(
                          *(unsigned int *)((char *)self->modelGeometry + 0xc));
            ((AbyssEngine::Transform *)(t))->Update(1, (bool)dt);
        } else {
            float dtf = (float)dt;
            self->settled = 1;
            self->elapsedSinceExplosion += dt;
            float velStep = dtf * g_pgcu_velScale;

            unsigned int i = 0;
            while (i < arr->size()) {
                int *timer = &(*self->sparkTimers)[i];
                *timer -= dt;

                float *life = &(*self->sparkLife)[i];
                float newLife = *life - velStep;
                *life = newLife;
                float lifeMin = (*self->sparkLifeMin)[i];
                if (newLife < lifeMin)
                    *life = lifeMin;

                // Distance from this shard to the player's turret.
                void *player = (void *)(intptr_t)self->level->getPlayer();
                Vector turretPos, shardPos, delta;
                PlayerEgo_getTurretPosition(player, &turretPos);
                void *geom = (*self->sparkGeometries)[i];
                ((AEGeometry *)(geom))->getPosition();
                delta = turretPos - *(Vector *)(&shardPos);
                float dist = VectorLength(&delta);

                bool collected = false;
                if (dist < g_pgcu_catchDist && self->elapsedSinceExplosion >= 2000) {
                    void *p2 = (void *)(intptr_t)self->level->getPlayer();
                    if (((PlayerEgo *)(p2))->isInTurretMode() != 0 &&
                        (*self->sparkTimers)[i] >= g_pgcu_minTimer) {
                        (*self->sparkTimers)[i] = g_pgcu_resetTimer;
                        void *ship = ((Status *)(*gStatus))->getShip();
                        int itemId = self->itemId;
                        if (((Ship *)(ship))->getFreeSpace() < 1) {
                            if (self->level->getPlayer() != 0) {
                                ((FModSound *)(g_pgcu_pickupSound))->stop(0x8d0);
                                ((FModSound *)(g_pgcu_pickupSound))->play(0x8d0, 0, 0, 0.0f);
                                void *ego = (void *)(intptr_t)self->level->getPlayer();
                                int hud = ((PlayerEgo *)(ego))->getHUD();
                                ((Hud *)(hud))->catchCargo(self->itemId, false, true, false, true, false, 0, 0);
                            }
                        } else {
                            void *def = *(void **)(*(int *)(*(int *)g_pgcu_itemDefs + 4) + itemId * 4);
                            ((Item *)(def))->makeItem();
                            if (self->level->getPlayer() != 0) {
                                void *ego = (void *)(intptr_t)self->level->getPlayer();
                                int hud = ((PlayerEgo *)(ego))->getHUD();
                                ((Hud *)(hud))->catchCargo(self->itemId, true, false, false, false, false, 0, 0);

                                void *camp = *(void **)g_pgcu_campaign;
                                if (*(char *)((char *)camp + 0x2d) == 0 &&
                                    ((Status *)(*gStatus))->getCurrentCampaignMission() > 0x8e) {
                                    Vector missionBuf;
                                    ((Status *)(&missionBuf))->getMission();
                                    if (((Mission *)(&missionBuf))->isEmpty() != 0 &&
                                        self->itemId == 0xcc) {
                                        *(char *)((char *)camp + 0x2d) = 1;
                                        self->level->createRadioMessage(0x1a, 0);
                                    }
                                }
                            }
                            ((FModSound *)(g_pgcu_pickupSound))->stop(0x8d0);
                            ((FModSound *)(g_pgcu_pickupSound))->play(0x8d0, 0, 0, 0.0f);
                            void *ship2 = ((Status *)(*gStatus))->getShip();
                            ((Ship *)(ship2))->addCargo((Item *)def);
                        }
                        (*self->sparkScale)[i] = 0.0f;
                        collected = true;
                    }
                }

                if (!collected) {
                    // Fade / grow the shard's scale based on its remaining timer.
                    int tval = (*self->sparkTimers)[i];
                    float *scale = &(*self->sparkScale)[i];
                    if (tval <= 0) {
                        float sc = (float)tval / g_pgcu_growDiv + 1.0f;
                        *scale = sc;
                        if (sc < 0.0f)
                            *scale = 0.0f;
                    } else if (dist < g_pgcu_fadeLo) {
                        *scale = (dist + g_pgcu_fadeAdd) / g_pgcu_fadeDiv;
                    }
                    void *t = ((PaintCanvas *)*(void **)g_pgcu_canvasRoot)->TransformGetTransform(
                                  *(unsigned int *)((char *)geom + 0xc));
                    ((AbyssEngine::Transform *)(t))->Update(1, (bool)dt);
                }

                // Advance the shard's position.
                void *moveGeom;
                Vector moved;
                bool homing = (*self->sparkInSight)[i] != 0 &&
                              self->elapsedSinceExplosion >= 2000;
                if (homing) {
                    void *ship = ((Status *)(*gStatus))->getShip();
                    if (((Ship *)(ship))->getFirstEquipmentOfSort(0x23) != 0) {
                        // Steer toward the turret.
                        Vector dir, dn;
                        dir = turretPos - *(Vector *)(&shardPos);
                        VectorNormalize(&dn, &dir);
                        *(*self->sparkVelocities)[i] = dn;
                        moveGeom = (*self->sparkGeometries)[i];

                        void *ship2 = ((Status *)(*gStatus))->getShip();
                        Item *eq = ((Ship *)(ship2))->getFirstEquipmentOfSort(0x23);
                        int attr = ((Item *)(eq))->getAttribute(0);
                        float step = (float)(attr * dt);
                        Vector vel; vel = turretPos;
                        Vector_scale(step, &vel);
                        Vector_add(&vel, &shardPos);
                        moved = vel;
                        ((AEGeometry *)(moveGeom))->setPosition(moved);
                        goto advance;
                    }
                }
                {
                    moveGeom = (*self->sparkGeometries)[i];
                    Vector vel;
                    vel = *(*self->sparkVelocities)[i];
                    Vector_scale((*self->sparkLife)[i] * dtf, &vel);
                    Vector_add(&vel, &shardPos);
                    moved = vel;
                    ((AEGeometry *)(moveGeom))->setPosition(moved);
                }

            advance:
                if (g_pgcu_minTimer <= (*self->sparkTimers)[i])
                    self->settled = 0;
                i++;
                arr = self->sparkGeometries;
            }

            if (self->settled != 0)
                self->active = 0;
        }
    }

    
}

namespace AbyssEngine { namespace AEMath {
void MatrixGetDir(Vector *out, Matrix const *m);
void MatrixGetUp(Vector *out, Matrix const *m);
float operator-(Vector &a, Vector const &b);
} }

__attribute__((visibility("hidden"))) extern void **g_pgc_canvas2;

void PlayerGasCloud::render()
{
    Vector local_80;
    Vector dir;
    char cameraLocal[60];

    if (this->active == 0)
        return;
    int mode = this->state;
    if (mode != 3 && mode != 0)
        return;

    void *cam = *g_pgc_canvas2;
    int cur = ((PaintCanvas *)cam)->CameraGetCurrent();
    __aeabi_memcpy(cameraLocal, ((PaintCanvas *)cam)->CameraGetLocal(cur), 0x3c);

    AbyssEngine::AEMath::MatrixGetDir((Vector *)&local_80, (Matrix *)cameraLocal);
    float scale = AbyssEngine::AEMath::operator-(dir, *(Vector *)&local_80);

    Array<AEGeometry*> *arr;
    if (this->exploded == 0 || (arr = this->sparkGeometries) == 0) {
        void *geom = this->modelGeometry;
        AbyssEngine::AEMath::MatrixGetUp((Vector *)&local_80, (Matrix *)cameraLocal);
        ((AEGeometry *)(geom))->setDirection(dir, *(Vector *)&local_80);
        ((AEGeometry *)(geom))->render();
    } else {
        for (unsigned int i = 0; i < arr->size(); i++) {
            AEGeometry *g = (*this->sparkGeometries)[i];
            float si = (*this->sparkScale)[i];
            ((AEGeometry *)(g))->setScaling(si);
            AbyssEngine::AEMath::MatrixGetUp((Vector *)&local_80, (Matrix *)cameraLocal);
            ((AEGeometry *)(g))->setDirection(dir, *(Vector *)&local_80);
            ((AEGeometry *)(g))->render();
            arr = this->sparkGeometries;
        }
    }
}
