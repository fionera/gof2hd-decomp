#include "game/ship/PlayerFighter.h"
#include "game/mission/Mission.h"
#include "engine/render/AEGeometry.h"
#include "game/mission/Generator.h"
#include "game/world/Level.h"
#include "game/mission/Status.h"
#include "engine/render/Trail.h"
#include "game/mission/Explosion.h"
#include "game/ship/KIPlayer.h"
#include "game/ship/Player.h"
#include "game/world/Route.h"
#include "game/world/Standing.h"
#include "engine/math/Transform.h"
#include "engine/math/EaseInOutMatrix.h"
#include "platform/libc.h"
#include "engine/core/AERandom.h"
#include "engine/math/AEMath.h"
#include "game/world/SpacePoint.h"

// Canonical singletons reachable from this TU. gCanvas is declared locally
// because the full PaintCanvas.h headers clash with the AbyssEngine stubs
// already pulled in by Trail.h. gRandom is provided by AERandom.h above.

// Local minimal view of AbyssEngine::PaintCanvas (the full headers
// gof2/PaintCanvas.h / PaintCanvasClass.h cannot be included here: Trail.h
// already declares a distinct AbyssEngine::PaintCanvas stub that conflicts).
// Only the methods called in this TU are declared; signatures/scope match the
// real PaintCanvas definitions in src/PaintCanvas.cpp so the mangled
// _ZN11AbyssEngine11PaintCanvas... symbols resolve at link time.
namespace AbyssEngine {
class PaintCanvas {
public:
    void *TransformGetTransform(unsigned int index);
    void *TransformGetLocal(unsigned int index);
    void TransformSetLocal(unsigned int index, const AbyssEngine::AEMath::Matrix &matrix);
    void MeshCloneMaterial(unsigned int index, unsigned int *out);
    void *MeshGetPointer(unsigned int index);
    void *MaterialGetMaterial(unsigned int index);
    void MeshChangeMaterial(unsigned int meshIndex, unsigned int matIndex);
};
} // namespace AbyssEngine


// Canonical render-canvas singleton (binary .bss 0x2281b8), typed against the
// local ::PaintCanvas view above to avoid the Trail.h type clash.
extern PaintCanvas* gCanvas;

extern "C" void AEMath_Matrix_ctor(void *m);
// No-bound AERandom::nextInt() variant: the decompiler dropped the bound arg and
// Generator.h declares the (self,bound) overload, so this distinct shim avoids the
// conflict. Real first arg is the AERandom* receiver (held here as an int).
extern "C" int AERandom_nextInt_nobound(int rng);
static inline int PF_nextInt(int rng) { return AERandom_nextInt_nobound(rng); }
extern "C" float VectorSignedToFloat(int v, int mode);
extern "C" int *RH_op_new_arr(unsigned int n);
extern "C" void RH_op_delete_arr(void *p);
extern "C" void PF_update_dead(PlayerFighter *self);
extern "C" void PF_update_body(PlayerFighter *self, int dt);
extern "C" void AEMath_MatrixAssign(void *dst, void *src);
extern "C" void AEMath_MatrixIdentity(void *out, void *m);
extern "C" void AEMath_MatrixSetRotation(void *m, float rx, float ry, float rz);
static inline float AEMath_VectorLength(void *v) {
    return AbyssEngine::AEMath::VectorLength(*(const AbyssEngine::AEMath::Vector *)v);
}
extern "C" int AERandom_nextIntB(int rng, int bound);
extern "C" void PF_vscale(void *out, void *vec, float scalar);

uint8_t PlayerFighter::hasMissionCrateLost() {
    return this->missionCrateLost;
}

uint8_t PlayerFighter::hasMissionCrateCaptured() {
    return this->missionCrateCaptured;
}

void PlayerFighter::setShootError(int v) {
    this->shootError = (float)v;
}

void PlayerFighter::setAIDisabled(bool v) {
    this->aiDisabled = v;
}



// Forwards to the inherited KIPlayer::setShipGroup(AEGeometry*, int, bool).
void PlayerFighter::setShipGroup(AEGeometry *geom, int group, bool flag)
{
    KIPlayer::setShipGroup(geom, group, flag);
}

void PlayerFighter::awake() {
    this->state = 1;
    ((Player *)(this->player))->setActive(1);
    ((PlayerFighter *)(this))->setExhaustVisible(true);
    int geom = this->subGeometry;
    this->field_0xf5 = 1;
    if (geom == 0) {
        geom = this->geometry;
    }
    // awake() tail: make the (sub)geometry visible. Inherited AEGeometry::setVisible.
    return ((AEGeometry *)(intptr_t)geom)->setVisible(true);
}


void PlayerFighter::setBV(Array<BoundingVolume *> *v) {
    this->boundingVolumes = v;
}

void PlayerFighter::setBoostProb(int v) {
    this->boostProb = v;
}

void PlayerFighter::setCloakingPossible(bool v) {
    PlayerFighter *self = this;
    self->cloakingPossible = v;
    if (!v && self->field_0x13c != 0) {
        self->cloakTimer = self->cloakDuration + 1;
        // Forcing cloakTimer past the duration and re-entering handleCloaking()
        // runs the cloak-deactivation path (restores the material, hides exhaust).
        return self->handleCloaking();
    }
}


void PlayerFighter::removeTrail() {
    delete this->trail;
    this->trail = 0;
}

// Releases the owned route, bounding-volume array, trail, explosion and ease-in/out
// matrix. The KIPlayer base destructor is chained automatically.
PlayerFighter::~PlayerFighter()
{
    delete this->route;
    this->route = 0;

    if (this->boundingVolumes != 0) {
        for (auto *e : *this->boundingVolumes) delete e;
        this->boundingVolumes->clear();
        delete this->boundingVolumes;
    }
    this->boundingVolumes = 0;

    delete this->trail;
    this->trail = 0;

    delete this->explosion;
    this->explosion = 0;

    delete this->easeMatrix;
    this->easeMatrix = 0;
}

uint8_t PlayerFighter::hasCrateCaptured() {
    return this->crateCaptured == 0;
}

// setPosition(Vector const&) — unpacks the vector and dispatches the float setPosition
// override through the actor vtable (slot 0x48).
void PlayerFighter::setPosition(const Vector &v) {
    this->setPosition(v.x, v.y, v.z);
}

void PlayerFighter::setRotate(int v) {
    float f = (float)v;
    this->field_0x13d = 0;
    this->rotate = f;
    this->currentRotate = f;
}

uint8_t PlayerFighter::hasCrateLost() {
    return this->crateLost;
}

// Three function pointers fetched once from hidden PC-relative globals (each a ptr-to-fnptr).
extern void *const gSL_f1 __attribute__((visibility("hidden")));
extern void *const gSL_f2 __attribute__((visibility("hidden")));
extern void *const gSL_f3 __attribute__((visibility("hidden")));

typedef int (*F1)(int geom);
typedef int (*F2)(int base, int v, int kind, int z);
typedef void (*F3)(int base, int v, int z);

void PlayerFighter::setLevel(Level *lvl) {
    ((KIPlayer *)(this))->setLevel(lvl);
    F1 f1 = (F1)gSL_f1;
    F2 f2 = (F2)gSL_f2;
    F3 f3 = (F3)gSL_f3;
    int base = this->level;

    int v;
    v = f2(*(int *)(base + 0x74), f1(this->geometry), 9, 0);
    this->engineTrailSystem = v;
    f3(*(int *)(this->level + 0x74), v, 0);

    v = f2(*(int *)(this->level + 0x78), f1(this->geometry), 0xf, 0);
    this->field_0x80 = v;
    f3(*(int *)(this->level + 0x78), v, 0);

    v = f2(*(int *)(this->level + 0x84), f1(this->geometry), 0x2a, 0);
    this->field_0x84 = v;
    f3(*(int *)(this->level + 0x84), v, 0);

    v = f2(*(int *)(this->level + 0x8c), f1(this->geometry), 0x11, 0);
    this->field_0x134 = v;
    f3(*(int *)(this->level + 0x8c), v, 0);

    v = f2(*(int *)(this->level + 0x8c), f1(this->geometry), 0x12, 0);
    this->field_0x138 = v;
    return f3(*(int *)(this->level + 0x8c), v, 0);
}

extern void *const gPFC_guard __attribute__((visibility("hidden")));
extern int *const gPFC_sharedRoute __attribute__((visibility("hidden")));
extern const int gPFC_defaultRoute __attribute__((visibility("hidden"))); // DAT_000ec81c (0x30 bytes)

PlayerFighter::PlayerFighter(int faction, int wingmanCmd, Player *player, AEGeometry *geom,
                             float x, float y, float z, bool flag)
    : KIPlayer(faction, wingmanCmd, player, geom, x, y, z, flag) {
    PlayerFighter *self = this;
    int *guardP = *(int **)gPFC_guard;
    volatile int saved = *guardP;

    self->field_0x200 = 0;
    self->field_0x204 = 0;
    self->field_0x208 = 0;
    // Original cleared 0x48 bytes from 0x158; the modeled members in that span are
    // the working position and the two transient reset vectors (trailing bytes hold
    // no named members in the packed layout).
    self->workingPosition = (Vector){0, 0, 0};
    self->resetVecB = (Vector){0, 0, 0};
    self->resetVecC = (Vector){0, 0, 0};
    AEMath_Matrix_ctor(&self->easeBaseMatrix);
    AEMath_Matrix_ctor(&self->rollMatrix);

    int rng = (int)(intptr_t)gRandom;
    // 9 candidate waypoints (3 floats each) drawn from the RNG.
    float wp[27];
    int r;
    r = PF_nextInt(rng); wp[0] = VectorSignedToFloat(r - 30000, 0);
    int r1 = PF_nextInt(rng);
    int r2 = PF_nextInt(rng);
    wp[2] = VectorSignedToFloat(r2 + 20000, 0);
    wp[1] = VectorSignedToFloat(r1 - 10000, 0);
    r = PF_nextInt(rng); wp[3] = VectorSignedToFloat(r + 5000, 0);
    r1 = PF_nextInt(rng);
    r2 = PF_nextInt(rng);
    wp[5] = VectorSignedToFloat(r2 + 20000, 0);
    wp[4] = VectorSignedToFloat(r1 - 10000, 0);
    r = PF_nextInt(rng); wp[6] = VectorSignedToFloat(r + 5000, 0);
    r1 = PF_nextInt(rng);
    r2 = PF_nextInt(rng);
    wp[8] = VectorSignedToFloat(r2 + 55000, 0);
    wp[7] = VectorSignedToFloat(r1 - 10000, 0);
    r = PF_nextInt(rng); wp[9] = VectorSignedToFloat(r - 30000, 0);
    r1 = PF_nextInt(rng);
    r2 = PF_nextInt(rng);
    wp[10] = VectorSignedToFloat(r1 - 10000, 0);
    wp[11] = VectorSignedToFloat(r2 + 55000, 0);

    int count = PF_nextInt(rng) * 3 + 6;
    char used[16];
    for (int i = 0; i < 13; i++) used[i] = 0;
    unsigned long long bytes = (unsigned long long)(unsigned)count * 4;
    unsigned sz = (int)(bytes >> 32) != 0 ? 0xffffffff : (unsigned)bytes;
    int *pts = RH_op_new_arr(sz);
    for (int i = 0; i < count; i += 3) {
        int idx;
        do {
            idx = PF_nextInt(rng);
        } while (used[idx] != 0);
        used[idx] = 1;
        pts[i] = (int)wp[idx * 3];
        pts[i + 1] = (int)wp[idx * 3 + 1];
        pts[i + 2] = (int)wp[idx * 3 + 2];
    }
    self->route = new Route(pts, (unsigned)count);
    RH_op_delete_arr(pts);

    int *shared = gPFC_sharedRoute;
    if (*shared == 0) {
        int defPts[12];
        memcpy(defPts, &gPFC_defaultRoute, 0x30);
        *shared = (int)(intptr_t)(new Route(defPts, 0xc));
    }

    self->field_0x130 = -1;
    self->field_0x134 = -1;
    self->field_0x138 = -1;
    self->field_0x128 = 50000;
    self->rotate = 2.0f;
    self->shootError = 0;
    self->speed = 2.0f;
    self->field_0x1b0 = 0x5dc;
    self->boostProb = 5;
    self->field_0x38 = 0;
    self->field_0x140 = 0;
    self->field_0x12d = 0;
    self->field_0x13d = 0;
    self->field_0x1f8 = 0;
    self->deathTimer = 0;
    self->field_0x12f = 0;
    self->field_0x1fc = 0;
    self->field_0x1c8 = 0;
    self->field_0x1e0 = 0;
    self->field_0x1f4 = 0;
    self->wingmanCommand = wingmanCmd;

    // Four 16-byte zero blocks (an int followed by a 3-field vector each).
    self->maneuverTimer = 0;
    self->field_0x1c0 = 0;
    self->field_0x1c4 = 0;
    self->field_0x1c8 = 0;
    self->field_0x148 = 0;
    self->commandRoute = 0;
    self->boundingVolumes = 0;
    self->trail = 0;
    self->deltaTime = 0;
    self->deltaTimeHi = 0;
    self->hitpoints = 0;
    self->field_0x1dc = 0;
    self->field_0x1e4 = 0;
    self->currentSpeed = 0;
    self->field_0x1ec = 0;
    self->currentRotate = 0;

    // Original stored the raw `flag` bit-pattern as workingPosition.x.
    self->workingPosition.x = *(float *)&flag;
    self->workingPosition.y = 0;
    self->workingPosition.z = 0;
    self->field_0x13d = 1;
    self->crateCaptured = 1;
    self->currentSpeed = self->speed;
    self->currentRotate = self->rotate;
    self->route->setLoop(0);
    ((Route *)((void *)(long)*shared))->setLoop(0);
    self->routeClone = 0;

    if (gStatus->getCurrentCampaignMission() != 0x29) {
        if (wingmanCmd == 9) {
            self->routeClone = ((Route *)((void *)(long)*shared))->clone();
        } else {
            self->routeClone = self->route->clone();
        }
    }
    self->field_0x12d = 0;

    if (wingmanCmd == 9) {
        self->lootList = 0;
    } else {
        Generator *g = new Generator();
        self->lootList = g->getLootList(-1, -1);
        delete g;
    }

    self->field_0x128 = (gStatus->inAlienOrbit() != 0) ? 100000 : 50000;

    Explosion *exp = new Explosion(0);
    self->explosion = exp;
    exp->addFireStreaks();
    self->field_0x13e = 1;
    self->hitpoints = ((Player *)(self->player))->getHitpoints();
    self->field_0x1dc = 0;
    self->field_0x1e0 = 0;
    self->field_0xe4 = 1;
    self->field_0x25 = 1;
    self->engineTrailSystem = -1;

    int fov;
    if (gStatus->getCurrentCampaignMission() == 1) {
        fov = -1;
    } else {
        fov = (self->field_0xdc == 0) ? 0x2e : 0x30;
    }
    self->fov = fov;

    self->targetRoll = 0;
    self->smoothRoll = 0;
    self->field_0x214 = 0;
    self->field_0x294 = 0;
    self->field_0x298 = 0;
    self->rollSamples = 0;
    self->field_0x2a0 = 0;
    self->field_0x2a4 = 0;
    self->field_0x2a8 = 0;
    self->field_0x2ac = 0;
    self->rollBufferFilled = 0;
    self->rollSampleIndex = 0;
    self->rollActive = 0;
    self->field_0x254 = 0;
    self->easeMatrix = 0;
    self->field_0x13c = 0;
    self->spacePoint = 0;
    self->cloakTimer = 0;
    self->cloakCooldown = 0;
    self->field_0x2cd = 0;
    self->field_0x2c9 = 0;
    self->cloakingPossible = 1;
    self->cloakMaterial = -1;
    self->aiDisabled = 0;
    self->gunSwitchTimer = 0;

    return;
}

// Tail veneer taken on the dead+exploded early-out path (-> 0x1abe18).
// The remaining per-frame update: AI steering, weapons, cloaking, trail, camera, route
// following. A ~10KB state machine; delegated to a helper so this translation keeps the
// recoverable entry flow (early-out, timers, position, enemy flag) while staying tractable.

extern void *const gUpd_guard __attribute__((visibility("hidden")));

// PlayerFighter::update(int dt) — self in r0, dt in r1.
void PlayerFighter::update(int dt) {
    int *guardP = *(int **)gUpd_guard;
    volatile int saved = *guardP;
    // Hoisted to outer scope to remain visible across goto-target blocks.
    Array<Player *> *enemies = nullptr;
    PlayerEgo *ego = nullptr;

    // Dead-and-explosion-finished early-out: tear down via the dead veneer.
    if (this->state == 4 && this->explosion->isPlaying() == 0 &&
        (this->crateCaptured == 0 || 60000 < this->deathTimer)) {
        
        PF_update_dead(this);
        return;
    }

    // Advance per-frame timers.
    this->field_0x1c0 += dt;
    this->maneuverTimer += dt;
    if (this->wingmanCommand == 1) {
        this->wingmanCommand = 1;
    }
    this->deltaTime = dt;
    this->deltaTimeHi = dt >> 31;

    // Sync world position from the geometry.
    float pos[3];
    ((AEGeometry *)(pos))->getPosition();
    this->renderPosition = *(Vector *)pos;

    // Recompute the "is enemy" flag unless the ship is in a non-combat mode.
    if (this->field_0x43 == 0) {
        unsigned char enemy;
        if ((this->wingmanCommand & 0xfffffffe) == 8) {
            enemy = 1;
        } else if (((KIPlayer *)(this))->isWingMan() != 0) {
            enemy = 0;
        } else {
            enemy = (unsigned char)((Standing *)(gStatus->getStanding()))->isEnemy(this->wingmanCommand);
        }
        *(unsigned char *)(this->player + 0x5c) = enemy;
    }

    // Friend flag (player[0x5d]) — symmetric to enemy flag above.
    {
        unsigned char friendFlag;
        if ((this->wingmanCommand & ~1u) == 8) {
            friendFlag = 0;
        } else if (((KIPlayer *)(this))->isWingMan() != 0) {
            friendFlag = 1;
        } else {
            friendFlag = (unsigned char)((Standing *)(gStatus->getStanding()))->isFriend(this->wingmanCommand);
        }
        Player *playerP = (Player *)(intptr_t)this->player;
        *(uint8_t *)((char *)playerP + 0x5d) = friendFlag;

        // Damage-based forced-enemy override (non-combat mode only).
        if (this->field_0x43 != 0) {
            int dmgDone = playerP->damageDoneByPlayer;
            if (dmgDone > playerP->getMaxHitpoints() / 20) {
                *(uint8_t *)((char *)playerP + 0x5c) = 1;
            }
        }
    }

    // Gun-switch timer: if the combat flag (field_0x42) is set, advance the
    // gun-switch timer and toggle between secondary weapons once it fires.
    if (static_cast<KIPlayer *>(this)->field_0x42 != 0) {
        this->gunSwitchTimer += dt;
        if (this->gunSwitchTimer > 20000) {
            Player *playerP = (Player *)(intptr_t)this->player;
            this->gunSwitchTimer = 0;
            if (playerP->gunAvailable(1)) {
                this->field_0x140 = (this->field_0x140 == 0) ? 1 : 0;
            }
            // Ego check: if this fighter's owning Level has a player whose squad
            // leader target matches 'this', uncover it as a wanted NPC.
            if (!playerP->isAlwaysEnemy()) {
                PlayerEgo *ego = ((Level *)(intptr_t)this->level)->getPlayer();
                if (ego != nullptr) {
                    ego = ((Level *)(intptr_t)this->level)->getPlayer();
                    void *f14 = ego->field_0x14;
                    if (f14 != nullptr &&
                        *(PlayerFighter **)((char *)f14 + 4) == this &&
                        this->field_0x43 == 0) {
                        playerP->setAlwaysEnemy(true);
                        ((Level *)(intptr_t)this->level)->uncoverWanted(F<int>(this, 0x48));
                    }
                }
            }
        }
    }

    // Black-market system: clear enemy flag when wingmanCommand==8 and the
    // status module says a relevant condition is active.
    if (gStatus->inBlackMarketSystem()) {
        if ((uint8_t)gStatus->field_110 != 0) {
            if (this->wingmanCommand == 8) {
                *(uint16_t *)((char *)(intptr_t)this->player + 0x5c) = 0;
            }
        }
    }

    // Turned-enemy and always-friend overrides write the combined 16-bit flag.
    {
        Player *playerP = (Player *)(intptr_t)this->player;
        if (playerP->turnedEnemy()) {
            *(uint16_t *)((char *)playerP + 0x5c) = 1;
        }
        if (playerP->isAlwaysFriend()) {
            *(uint16_t *)((char *)playerP + 0x5c) = 0x100;
        }
    }

    // Clone the player's route if this NPC has no route yet (and is not a wingman).
    if (!this->field_0xdc && !this->routeClone) {
        this->routeClone = this->route->clone();
    }

    // Periodic trail / working-position update (every ~200 ms).
    this->field_0x1c8 += dt;
    if (this->field_0x1c8 >= 201) {
        if (this->trail != nullptr) {
            this->trail->update(this->workingPosition, this->renderPosition);
            this->workingPosition = this->renderPosition;
        }
        this->field_0x1c8 = 0;
    }

    // Mirror the geometry matrix onto the wrapped Player transform.
    {
        AEGeometry *geom = (AEGeometry *)(intptr_t)this->geometry;
        Player *playerP = (Player *)(intptr_t)this->player;
        { Matrix &src = geom->getMatrix(); *(Matrix *)((char *)playerP + 4) = src; }
        AEGeometry *parentGeom = (AEGeometry *)(intptr_t)static_cast<KIPlayer *>(this)->parentGeometry;
        if (parentGeom != nullptr) {
            Matrix &par = parentGeom->getMatrix();
            *(Matrix *)((char *)playerP + 4) *= par;
        }
    }

    // Wingman with an active routeClone and alive state: dispatch to the wingman
    // route-update sub-path instead of the normal per-frame enemy/Player::update path.
    if (this->field_0xdc != 0 && this->state != 4 && this->routeClone != nullptr) {
        // Get the ego (player) position to aim toward.
        ego = ((Level *)(intptr_t)this->level)->getPlayer();
        {
            Vector egoPos = ((AEGeometry *)ego->geometry)->getPosition();
            this->resetVecA = egoPos;
        }
        // Only proceed with wingman route update when sub-state is 1.
        if (this->field_0xe4 != 1) {
            goto lbl_dd240;
        }
        int kiWingCmd = static_cast<KIPlayer *>(this)->wingmanCommand;
        if (kiWingCmd == 0) {
            goto lbl_dd66c;
        }
        if (kiWingCmd == 1) {
            goto lbl_dd63c;
        }
        if (kiWingCmd != 2) {
            goto lbl_dd6ca;
        }
        // KI wingman command == 2: get ego up-vector scaled by 2000 and add to resetVecA.
        {
            Vector upVec = ((AEGeometry *)ego->geometry)->getUpVector();
            F<Vector>(this, 0x9c) = upVec;
            F<Vector>(this, 0x9c) *= 2000.0f;
            this->resetVecA += F<Vector>(this, 0x9c);
            Vector dirVec = ((AEGeometry *)(intptr_t)this->geometry)->getDirection();
            Vector tmpVec = dirVec;
            tmpVec *= 2000.0f;
            // fall through to lbl_dd6bc / target position update
            goto lbl_dd6bc_cmd2;
        }
    }

lbl_dd240:
    {
        // Standard non-wingman (or wingman sub-state != 1) per-frame tick:
        // Advance the Player simulation and fetch the current enemy list.
        int s = this->state;
        Player *playerP = (Player *)(intptr_t)this->player;
        if (s == 3 || s == 4) {
            playerP->StopEngineSound();
            goto lbl_ddb66;
        }
        bool doSound = (s != 5) && (F<int>(this, 0xf8) != -1);
        playerP->update(dt, doSound);
        enemies = playerP->getEnemies();
        if (this->aiDisabled != 0) {
            goto lbl_ddabe;
        }
        if (enemies == nullptr) {
            goto lbl_dd35c;
        }
        // enemies != null: target-validation / selection block.
        {
            int tgt = this->field_0x38;
            if ((unsigned int)tgt >= (unsigned int)enemies->size()) {
                tgt = -1;
                this->field_0x38 = tgt;
            }
            if (this->field_0x12e == 0) {
                goto lbl_dd3c0;
            }
            if (tgt < 0) {
                goto lbl_dd3c6;
            }
            // Check current target is still active.
            Player *curTgt = (Player *)enemies->data()[tgt];
            if (!curTgt->isActive()) {
                this->field_0x12e = 0;
            }
            goto lbl_dd3c6;
        }
    }

lbl_dd35c:
    {
        // No enemies — follow the routeClone toward the next waypoint.
        Route *rc = this->routeClone;
        if (rc == nullptr) {
            goto lbl_dd622;
        }
        {
            Vector myPos = ((AEGeometry *)(intptr_t)this->geometry)->getPosition();
            rc->update(myPos);
        }
        {
            Waypoint *wp = (Waypoint *)rc->getWaypoint();
            if (wp == nullptr) {
                goto lbl_dda88;
            }
            // Load integer waypoint coordinates and convert to float.
            // (Waypoint inherits KIPlayer; its own x/y/z fields sit at KIPlayer+0x124.)
            float wx = (float)F<int>(wp, 0x124);
            float wy = (float)F<int>(wp, 0x128);
            float wz = (float)F<int>(wp, 0x12c);
            this->resetVecB.x = wx;
            this->resetVecB.y = wy;
            this->resetVecB.z = wz;
            this->field_0x12c = 1;
            goto lbl_dda88;
        }
    }

lbl_dd3c0:
    this->field_0x38 = -1;

lbl_dd3c6:
    {
        // Enemy list exists.  Decide whether to re-scan or use current target.
        this->field_0x148 = 0;
        if (this->maneuverTimer < 0x1389) {
            goto lbl_dd544;
        }
        // Maneuver timer expired: pick a new target candidate.
        bool stray = (this->field_0x12d != 0);
        if (!stray) {
            // Randomised stray decision.
            int roll = AERandom_nextIntB((int)(intptr_t)gRandom, 100);
            stray = (roll < 20) ? 1 : 0;
        }
        this->field_0x12d = (short)stray;
        this->maneuverTimer = 0;

        // How many enemies are there?
        int numEnemies = (int)enemies->size();
        int roll2 = AERandom_nextIntB((int)(intptr_t)gRandom, 100);
        // Declare before goto to avoid jump-over-initialization.
        bool foundTarget = false;
        int sightRange = this->field_0x128;
        if (roll2 > 29 || numEnemies < 2) {
            goto lbl_dd50a_check;
        }
        // Scan for a valid target.
        {
            this->field_0x12e = 0;
            F<int>(this, 0x90) = 0;  // resetVecA.x as scratch
            // sp+0xc0 holds &resetVecA in the original asm; we'll track with a bool.
            for (int si = 0; si <= 4; ) {
                if ((uint8_t)(si)) {
                    Player *cand = (Player *)enemies->data()[si];
                    if (cand == nullptr) { si++; continue; }
                    if (!cand->isActive()) { si++; continue; }
                    if (cand->isDead()) { si++; continue; }
                    // Position check against sight range.
                    Vector candPos = cand->getPosition();
                    this->resetVecA = candPos;
                    // Range check for each axis:
                    float dx = this->renderPosition.x - this->resetVecA.x;
                    float dy = this->renderPosition.y - this->resetVecA.y;
                    float dz = this->renderPosition.z - this->resetVecA.z;
                    float sr = (float)sightRange;
                    float nsr = -(float)sightRange;
                    if (dx >= sr || dx <= nsr) { si++; continue; }
                    if (dy >= sr || dy <= nsr) { si++; continue; }
                    if (dz >= sr || dz <= nsr) { si++; continue; }
                    // Check faction / wing matching at dd474:
                    if (this->wingmanCommand != 8) {
                        Player *playerP = (Player *)(intptr_t)this->player;
                        if (!playerP->turnedEnemy()) {
                            goto lbl_dd480_inner;
                        }
                    }
                    goto lbl_df770_inner;
lbl_dd480_inner:
                    {
                        // Sight-range check passed and faction ok → record as candidate.
                        this->field_0x38 = si;
                        foundTarget = true;
                        si++;
                        continue;
lbl_df770_inner:
                        this->field_0x38 = si;
                        foundTarget = true;
                        goto lbl_dd50a_check;
                    }
                }
                si++;
            }
            if (foundTarget) {
                goto lbl_dd50a_check;
            }
lbl_dd50a_check:
            ;
        }
        if (!(uint8_t)this->field_0x12e) {
            this->field_0x38 = 0;
        }
        goto lbl_dd50e;
    }

lbl_dd544:
    {
        // maneuverTimer < 0x1389: scan enemy list for best live target.
        if (this->field_0x12e != 0) {
            goto lbl_dd7ac;
        }
        int sightRange = this->field_0x128;
        bool foundTarget = false;
        for (int si = 0; (unsigned int)si < enemies->size(); si++) {
            Player *cand = (Player *)enemies->data()[si];
            if (cand == nullptr) continue;
            if (!cand->isActive()) continue;
            if (cand->isDead()) continue;
            // Axis-distance check.
            Vector candPos = cand->getPosition();
            this->resetVecA = candPos;
            float dx = this->renderPosition.x - this->resetVecA.x;
            float dy = this->renderPosition.y - this->resetVecA.y;
            float dz = this->renderPosition.z - this->resetVecA.z;
            float sr = (float)sightRange;
            float nsr = -(float)sightRange;
            if (dx >= sr || dx <= nsr) continue;
            if (dy >= sr || dy <= nsr) continue;
            if (dz >= sr || dz <= nsr) continue;
            if (this->wingmanCommand != 8) {
                Player *playerP = (Player *)(intptr_t)this->player;
                if (!playerP->turnedEnemy()) {
                    this->field_0x38 = si;
                    this->field_0x12e = 1;
                    foundTarget = true;
                    break;
                }
            }
            this->field_0x38 = si;
            this->field_0x12e = 1;
            foundTarget = true;
            goto lbl_dd7ac;
        }
        goto lbl_dd7ac;
    }

lbl_dd50e:
    {
        // After target selection: check for wingman kill delegation.
        if (!this->field_0xdc) {
            goto lbl_dd780;
        }
        if (this->field_0xe4 != 3) {
            goto lbl_dd780;
        }
        KIPlayer *wt = static_cast<KIPlayer *>(this)->wingmanTarget;
        if (wt == nullptr) {
            goto lbl_dd780;
        }
        if (!wt->isDead()) {
            // Wingman target is alive — call setWingmanCommand(1, null) through vtable.
            this->setWingmanCommand(1, nullptr);
            goto lbl_dd780;
        }
        // Wingman target is dead: find the enemy whose geometry matches wingmanTarget.
        for (int si = 0; (unsigned int)si < enemies->size(); si++) {
            Player *cand = (Player *)enemies->data()[si];
            KIPlayer *candKI = cand->getKIPlayer();
            if (candKI == nullptr) continue;
            if ((Player *)candKI->player != (Player *)(intptr_t)wt->geometry) {
                // Not our target. Check if EMP-disabled.
                if (!cand->isAlwaysFriend()) {
                    si++;
                    // continue loop
                    continue;
                }
                goto lbl_df7b6;
            }
            if (!cand->isAlwaysFriend()) {
                si++;
                continue;
            }
lbl_df7b6:
            this->field_0x38 = si;
            this->field_0x12e = 1;
            goto lbl_dd780;
        }
        goto lbl_dd780;
    }

lbl_dd780:
    {
        // Validate current target index.
        if (this->field_0x38 >= 0 && (unsigned int)this->field_0x38 < enemies->size()) {
            Player *tgtP = (Player *)enemies->data()[this->field_0x38];
            if (!tgtP->isActive() || tgtP->isDead()) {
                this->field_0x12e = 0;
                this->field_0x38 = -1;
            }
        }
        // Ensure enemy flag is set when we have an active enemy.
        {
            Player *playerP = (Player *)(intptr_t)this->player;
            if (!*(uint8_t *)((char *)playerP + 0x5c)) {
                if (this->field_0x38 <= 0) {
                    this->field_0x12e = 0;
                    this->field_0x38 = 1;
                }
            }
        }
    }

lbl_dd7ac:
    {
        // Navigate toward the current target (or fall back to a route/position).
        bool hasWingman = (this->field_0xdc != 0);
        if (hasWingman && this->field_0xe4 == 3) {
            goto lbl_dd8b2;
        }
        // Main enemy scanning loop: find the 'best' enemy by faction, EMP, etc.
        {
            int tgtIdx = this->field_0x38;
            if (tgtIdx < 1) {
                goto lbl_dd8b0;
            }
            this->field_0x38 = -1;
            for (int si = 1; (unsigned int)si < enemies->size(); si++) {
                Player *cand = (Player *)enemies->data()[si];
                if (!cand->isActive()) { si++; continue; }
                if (cand->isDead()) { continue; }
                // EMP/always-friend/faction compatibility check.
                bool isEMP = (cand->isAlwaysFriend() != 0);
                KIPlayer *candKI = cand->getKIPlayer();
                uint8_t pad69 = (candKI != nullptr) ? *(uint8_t *)((char *)candKI + 0x69) : 0;
                int candWing;
                if (isEMP) {
                    candWing = (candKI != nullptr) ? candKI->shipGroup : 0;
                } else {
                    if (pad69 == 0) { continue; }
                    candWing = 0;
                }
                if (hasWingman) {
                    // Wingman mode: accept only if same faction as enemy flag.
                    uint8_t candEF = *(uint8_t *)((char *)cand + 0x5c);
                    if (!candEF) { continue; }
                    KIPlayer *wt = static_cast<KIPlayer *>(this)->wingmanTarget;
                    if (wt == nullptr) { continue; }
                    if (cand != (Player *)(intptr_t)wt->geometry) { continue; }
                    // fall through: candidate accepted.
                } else {
                    // Non-wingman: faction-pair acceptance table.
                    int myWing = this->wingmanCommand;
                    if (candWing == 8) {
                        if (myWing == 8) continue;
                    } else {
                        bool accept = false;
                        if (candWing == 8)      accept = true;
                        else if (myWing == 8)   accept = true;
                        else if (candWing == 9 && myWing == 9)  continue;
                        else if (candWing == 9) accept = true;
                        else if (myWing == 9)   accept = true;
                        else if (candWing == 0 && myWing == 1)  accept = true;
                        else if (candWing == 1 && myWing == 0)  accept = true;
                        else if (candWing == 3 && myWing == 2)  accept = true;
                        else if (candWing == 2 && myWing == 3)  accept = true;
                        else if (candWing == 10) {
                            if (myWing == 10) continue;
                        } else if (myWing == 10) accept = true;
                        if (!accept) continue;
                    }
                }
                // Candidate accepted.
                this->field_0x12e = 1;
                this->field_0x38 = si;
                goto lbl_dd8b0;
            }
        }

lbl_dd8b0:
        if (!hasWingman) {
            goto lbl_dd8d2;
        }
lbl_dd8b2:
        // Wingman mode: if no valid target found, clear following-route flag.
        if (!this->field_0x12e) {
            if (this->field_0xe4 == 1) {
                this->field_0x12c = 0;
                this->field_0x38 = -1;
                goto lbl_dd8e2;
            }
        }
lbl_dd8d2:
        this->field_0x12c = 0;
        if (this->field_0x38 != -1) {
            goto lbl_dd8ea;
        }
lbl_dd8e2:
        // No target: try routeClone navigation.
        {
            Route *rc = this->routeClone;
            if (rc != nullptr) {
                goto lbl_dd8fc;
            }
        }
lbl_dd8ea:
        {
            Player *playerP = (Player *)(intptr_t)this->player;
            int nearTgt = playerP->getEnemy(0) != nullptr ? (int)(intptr_t)playerP->getEnemy(0) : 0;
            // Actually getEnemy(0) but we need the full variant from dd8ec:
            // blx 0x6585c = Player::doesNeverAttack
            if (playerP->doesNeverAttack()) {
                goto lbl_dda38;
            }
        }
        {
            Route *rc = this->routeClone;
            if (rc != nullptr) {
                goto lbl_dd8fc;
            }
        }
lbl_dd8fc:
        {
            Route *rc = this->routeClone;
            if (rc == nullptr) {
                goto lbl_dda20;
            }
            if (!rc->getWaypoint()) {
                goto lbl_dda20;
            }
            {
                Player *playerP = (Player *)(intptr_t)this->player;
                if (!playerP->isActive()) {
                    goto lbl_dda20;
                }
            }
            // Navigate along routeClone.
            {
                Vector myPos = ((AEGeometry *)(intptr_t)this->geometry)->getPosition();
                rc->update(myPos);
            }
            if (!rc->getWaypoint()) {
                goto lbl_dda88;
            }
            // Get waypoint and navigate.
            {
                Waypoint *wp = (Waypoint *)rc->getWaypoint();
                if (!wp) { goto lbl_dda88; }
                int stateNow = this->state;
                if (stateNow != 7 && stateNow != 1) {
                    goto lbl_df766;
                }
                if (stateNow == 1) {
                    this->state = 7;
                }
                // Get ego position via vtable getPosition slot.
                Vector egoP;
                {
                    const KIPlayer *ki = static_cast<const KIPlayer *>(this);
                    egoP = const_cast<KIPlayer *>(ki)->getPosition();
                }
                SpacePoint *sp = this->getNearestNavigationPoint(egoP,
                    (SpacePoint *)(intptr_t)F<int>(this, 0x2c4));
                if (sp == nullptr) {
                    goto lbl_df766;
                }
                if ((SpacePoint *)(intptr_t)F<int>(this, 0x2c4) != sp) {
                    SpacePoint *old = (SpacePoint *)(intptr_t)F<int>(this, 0x2c4);
                    if (old != nullptr) { old->giveFree(); }
                    F<int>(this, 0x2c4) = (int)(intptr_t)sp;
                    sp->take();
                }
                // Build target position from waypoint geometry.
                Waypoint *wp2 = (Waypoint *)rc->getWaypoint();
                Vector wpPos = ((KIPlayer *)wp2)->getPosition();
                // Route wp matrix & direction.
                AEGeometry *wpGeom = (AEGeometry *)(intptr_t)((KIPlayer *)wp2)->geometry;
                Matrix wpMat = wpGeom->getMatrix();
                // Compute navigation direction.
                Vector egoPos2;
                {
                    const KIPlayer *ki = static_cast<const KIPlayer *>(this);
                    egoPos2 = const_cast<KIPlayer *>(ki)->getPosition();
                }
                Vector navDir = AbyssEngine::AEMath::operator-(egoPos2, wpPos);
                navDir = AbyssEngine::AEMath::MatrixRotateVector(wpMat, navDir);
                F<Vector>(this, 0x9c) = navDir;
                // Call getPosition through vtable.
                Vector egoPos3;
                {
                    const KIPlayer *ki = static_cast<const KIPlayer *>(this);
                    egoPos3 = const_cast<KIPlayer *>(ki)->getPosition();
                }
                Vector pos3 = AbyssEngine::AEMath::operator+(egoPos3, navDir);
                this->resetVecA = pos3;
                // Update target position.
                {
                    Vector egoPos4;
                    const KIPlayer *ki = static_cast<const KIPlayer *>(this);
                    egoPos4 = const_cast<KIPlayer *>(ki)->getPosition();
                }
                {
                    Vector navFull = AbyssEngine::AEMath::operator-(egoPos3, wpPos);
                    float dist = AbyssEngine::AEMath::VectorLength(navFull);
                    if ((int)dist < 0x7d0) {
                        goto lbl_df55a;
                    }
                    this->resetVecB = this->resetVecA;
                    goto lbl_df762;
                }
            }
        }
    }

lbl_dda20:
    {
        // No route/waypoint: find nearest enemy and navigate to it.
        Player *playerP = (Player *)(intptr_t)this->player;
        this->field_0x38 = 0;
        this->field_0x148 = (int)(intptr_t)playerP->getEnemy(0);
        if (this->field_0x148 == 0) {
            goto lbl_dda88;
        }
        {
            // Get enemy position as target.
            Player *nearEnemy = (Player *)(intptr_t)this->field_0x148;
            Vector enemyPos = nearEnemy->getPosition();
            this->resetVecA = enemyPos;
            this->resetVecB = this->resetVecA;
            goto lbl_dda88;
        }
    }

lbl_dda38:
    {
        // doesNeverAttack path: preserve current target index or clear.
        int tgt = this->field_0x38;
        if (tgt >= 0 && (unsigned int)tgt < enemies->size()) {
            this->field_0x148 = (int)(intptr_t)((Player *)enemies->data()[tgt]);
        } else {
            this->field_0x38 = 0;
        }
        {
            Player *playerP = (Player *)(intptr_t)this->player;
            Player *nearEnemy = (tgt >= 0 && (unsigned int)tgt < enemies->size())
                ? (Player *)(intptr_t)this->field_0x148
                : playerP->getEnemy(0);
            this->field_0x148 = (int)(intptr_t)nearEnemy;
            Vector enemyPos = nearEnemy->getPosition();
            this->resetVecA = enemyPos;
            this->resetVecB = this->resetVecA;
            goto lbl_dda88;
        }
    }

lbl_dda88:
    // Jumper route timer: if this is a jumper NPC following a route, advance the
    // timer and trigger a jump after 20 s.
    if (static_cast<KIPlayer *>(this)->jumperFlag && this->field_0x12c) {
        this->field_0xf0 += dt;
        if (this->field_0xf0 >= 20000) {
            this->state = 6;
            this->field_0xf0 = 0;
            this->field_0x12c = 0;
        }
    } else {
        this->field_0xf0 = 0;
    }

lbl_ddabe:
    // Wingman state-2 commandRoute dispatch: if this is a wingman in sub-state 2
    // with a commandRoute, update it and extract the next waypoint target.
    if (this->field_0xdc && this->field_0xe4 == 2) {
        Route *cr = this->commandRoute;
        if (cr != nullptr) {
            // Position-update the commandRoute.
            {
                Vector myPos = ((AEGeometry *)(intptr_t)this->geometry)->getPosition();
                cr->update(myPos);
            }
            int waypointCount = cr->getWaypoint() ? 1 : 0;  // just existence check
            int initialWp = this->field_0x1e4;
            if (cr->length() > initialWp) {
                // Route still has waypoints past the initial one: keep following.
                if (this->commandRoute != nullptr) {
                    delete this->commandRoute;
                    this->commandRoute = nullptr;
                }
                // Dispatch setWingmanCommand(1, null) via vtable.
                this->setWingmanCommand(1, nullptr);
                goto lbl_ddb66;
            }
            // More waypoints remain: read the current waypoint coordinates.
            if (!cr->getWaypoint()) {
                goto lbl_ddb66;
            }
            {
                Waypoint *wp = (Waypoint *)cr->getWaypoint();
                float wx = (float)F<int>(wp, 0x124);
                float wy = (float)F<int>(wp, 0x128);
                float wz = (float)F<int>(wp, 0x12c);
                this->field_0x12c = 1;
                this->resetVecB.x = wx;
                this->resetVecB.y = wy;
                this->resetVecB.z = wz;
            }
        } else {
            // No commandRoute: dispatch setWingmanCommand(1, null).
            this->setWingmanCommand(1, nullptr);
        }
    }

lbl_ddb66:
    // Compute distanceToTarget: resetVecC = resetVecB - renderPosition.
    this->resetVecC.x = this->resetVecB.x - this->renderPosition.x;
    this->resetVecC.y = this->resetVecB.y - this->renderPosition.y;
    this->resetVecC.z = this->resetVecB.z - this->renderPosition.z;

    // Remaining per-frame state machine (hitpoints, death, movement, shooting).
    PF_update_body(this, dt);
    return;

    // ---- Sub-path labels (unreachable from fall-through but reachable via goto) ----

lbl_dd622:
    // routeClone is null: transition to sleep state.
    this->state = 5;
    goto lbl_dda88;

lbl_dd63c:
    // KI wingman command == 1: get ego right-vector.
    {
        Vector rightVec = ((AEGeometry *)ego->geometry)->getRightVector();
        F<Vector>(this, 0x9c) = rightVec;
        F<Vector>(this, 0x9c) *= 4000.0f;
        this->resetVecA += F<Vector>(this, 0x9c);
        Vector dirVec = ((AEGeometry *)(intptr_t)this->geometry)->getDirection();
        Vector tmpVec = dirVec;
        tmpVec *= 4000.0f;
        goto lbl_dd6bc;
    }

lbl_dd66c:
    // KI wingman command == 0: get ego right-vector (different scale).
    {
        Vector rightVec = ((AEGeometry *)ego->geometry)->getRightVector();
        F<Vector>(this, 0x9c) = rightVec;
        F<Vector>(this, 0x9c) *= 4000.0f;
        this->resetVecA += F<Vector>(this, 0x9c);
        // Subtract from the routeClone (navigate away).
        this->routeClone->setNewCoords(this->resetVecA);
        this->routeClone->setLoop(false);
        goto lbl_dd69a;
    }

lbl_dd6bc_cmd2:
lbl_dd6bc:
    {
        // shared: build direction vector and update route.
        Vector geomDir = ((AEGeometry *)(intptr_t)this->geometry)->getDirection();
        Vector tmpDir = geomDir;
        tmpDir *= 3000.0f;
        F<Vector>(this, 0x9c) = AbyssEngine::AEMath::VectorNormalize(this->resetVecA);
        F<Vector>(this, 0x9c) *= 3000.0f;
        // Subtract from wing nav vector.
        this->resetVecA -= F<Vector>(this, 0x9c);
        this->routeClone->setNewCoords(this->resetVecA);
        this->routeClone->setLoop(false);
    }

lbl_dd69a:
    {
        // Update the routeClone for current position, then set looping.
        int routeLen = this->routeClone->length();
        if (routeLen >= 2) {
            // Convert resetVecA to int coordinates.
            Vector navA = this->resetVecA;
            int ix = (int)navA.x;
            int iy = (int)navA.y;
            int iz = (int)navA.z;
            // Free existing routeClone and create a new 3-point route.
            Route *oldRoute = this->routeClone;
            if (oldRoute != nullptr) {
                delete oldRoute;
                this->routeClone = nullptr;
            }
            int coords[3] = { ix, iy, iz };
            Route *newRoute = new Route(coords, 3);
            this->routeClone = newRoute;
        } else {
            // Route too short: update coords in place.
            int coords[3] = {
                (int)this->resetVecA.x,
                (int)this->resetVecA.y,
                (int)this->resetVecA.z
            };
            this->routeClone->setNewCoords(this->resetVecA);
        }
        this->routeClone->setLoop(false);
        goto lbl_dd240;
    }

lbl_dd6ca:
    {
        // KI wingman command != 0,1,2: navigate relative to route.
        int routeLen = this->routeClone->length();
        if (routeLen >= 2) {
            // Enough waypoints: compute and store new position.
            Vector navA = this->resetVecA;
            int ix = (int)navA.x;
            int iy = (int)navA.y;
            int iz = (int)navA.z;
            Route *oldRoute = this->routeClone;
            if (oldRoute != nullptr) {
                delete oldRoute;
                this->routeClone = nullptr;
            }
            int coords[3] = { ix, iy, iz };
            Route *newRoute = new Route(coords, 3);
            this->routeClone = newRoute;
        } else {
            this->routeClone->setNewCoords(this->resetVecA);
        }
        this->routeClone->setLoop(false);
        goto lbl_dd240;
    }

lbl_df766:
lbl_df762:
lbl_df55a:
    // These addresses are in the "docked / space-point navigation" tail which is
    // beyond the ddc56 cutoff — keep as stub.
    PF_update_body(this, dt);
    return;

lbl_df770:
lbl_df77a:
    // These addresses are in the "high-speed intercept" tail — keep as stub.
    PF_update_body(this, dt);
    return;
}

// PlayerFighter::setPosition(float, float, float). r0=self, r1..r3 = x,y,z (raw bits).
void PlayerFighter::setPosition(float x, float y, float z) {

    this->posX = x;
    this->posY = y;
    this->posZ = z;

    ((AEGeometry *)(intptr_t)this->geometry)->setPosition(x, y, z);
    this->workingPosition = Vector{ x, y, z };
    if (this->trail != 0) {
        this->trail->reset(this->workingPosition);
    }
    int m = (int)(intptr_t)&((AEGeometry *)(intptr_t)this->geometry)->getMatrix();
    AEMath_MatrixAssign((char *)this->player + 4, (void *)m);

    return;
}

extern void *const gRoll_guard __attribute__((visibility("hidden")));
extern const float gRoll_threshold __attribute__((visibility("hidden")));
extern const float gRoll_f18 __attribute__((visibility("hidden")));
extern const float gRoll_f1c __attribute__((visibility("hidden")));
extern const float gRoll_f20 __attribute__((visibility("hidden")));
extern const float gRoll_f24 __attribute__((visibility("hidden")));
extern const float gRoll_f28 __attribute__((visibility("hidden")));
extern const float gRoll_f2c __attribute__((visibility("hidden")));
extern const float gRoll_f30 __attribute__((visibility("hidden")));
extern const float gRoll_f34 __attribute__((visibility("hidden")));
extern const float gRoll_f38 __attribute__((visibility("hidden")));
extern const float gRoll_f3c __attribute__((visibility("hidden")));
extern const float gRoll_f40 __attribute__((visibility("hidden")));
extern const float gRoll_f44 __attribute__((visibility("hidden")));
extern const float gRoll_f48 __attribute__((visibility("hidden")));

// PlayerFighter::roll(int angle) — self in r0, angle in r1.
void PlayerFighter::roll(int angle) {
    PlayerFighter *self = this;
    int *guardP = *(int **)gRoll_guard;
    volatile int saved = *guardP;

    if (self->rollActive == 0) {
        goto done;
    }

    {
        int m = (int)(intptr_t)&((AEGeometry *)(self->geometry))->getMatrix();
        float fwdX = *(float *)(m + 0x10);
        float fwdY = *(float *)(m + 0x14);
        float absX = (0.0f < fwdX) ? fwdX : -fwdX;
        if (0x3b < angle) angle = 0x3c;

        // Snap-back to identity when banked back near level.
        if (fwdY >= 0.0f && absX < gRoll_threshold) {
            unsigned char tmp[60];
            AEMath_MatrixIdentity(tmp, &self->rollMatrix);
            AEMath_MatrixAssign(&self->rollMatrix, tmp);
            self->rollActive = 0;
            self->field_0x254 = 0;
            goto done;
        }

        float bank;    // fVar11
        float roll;    // fVar10 (z rotation)
        if (self->field_0x254 != 0) {
            roll = (fwdY == fwdY) ? gRoll_f20 : gRoll_f1c;   // NaN(fVar10) selection
            bank = (fwdX < 0.0f) ? roll : gRoll_f18;
        } else {
            bank = 0.0f;
            roll = 0.0f;
            if (fwdX > 0.0f && self->field_0x255 == 1) {
                self->field_0x254 = 1;
                bank = gRoll_f48;
            } else if (fwdX < 0.0f && self->field_0x255 == 2) {
                self->field_0x254 = 1;
                bank = gRoll_f44;
            } else if (fwdX >= 0.0f) {
                if (fwdY >= 0.0f) {
                    bank = absX;
                    const float *p = (gRoll_f38 < fwdX) ? &gRoll_f40 : &gRoll_f3c;
                    bank = *p;
                } else {
                    bank = gRoll_f24;
                }
            } else {
                bank = gRoll_f28;
                if (fwdY >= 0.0f) {
                    const float *p = &gRoll_f30;
                    if (fwdX != gRoll_f2c && fwdX < gRoll_f2c) p = &gRoll_f34;
                    bank = *p;
                }
            }
        }

        if (fwdX != 0.0f) {
            self->field_0x255 = (fwdX < 0.0f) ? 1 : 2;
        }
        float fa = VectorSignedToFloat(angle, 0);
        self->rollActive = 1;
        unsigned char tmp[60];
        AEMath_MatrixSetRotation(tmp, bank * fa, 0.0f, roll);
        AEMath_MatrixAssign(&self->rollMatrix, tmp);
    }

done:
    return;
}

void PlayerFighter::cloak(int dur, bool b) {
    unsigned v;
    if (dur > 0) {
        v = (unsigned)dur;
    } else {
        v = PF_nextInt((int)(intptr_t)gRandom) + 5000;
    }
    this->cloakActive = 1;
    this->cloakDuration = v + 4000;
    this->field_0x2d9 = b;
}

void PlayerFighter::setMissionCrate(bool on) {
    this->isMissionCrate = on;
    if (on) {
        this->lootList = new Array<int>();
        int mission = (int)(intptr_t)gStatus->getMission();
        int type = ((Mission *)(mission))->getType();
        int item = (type == 5) ? 0x74 : 0x75;
        this->lootList->push_back(item);
        // setMissionCrate() tail: append the slot marker (1) to the loot list.
        ArrayAdd<int>(1, *this->lootList);
    }
}

int PlayerFighter::collide(float x, float y, float z) {
    if ((unsigned)(this->state - 3) > 1) {
        Array<BoundingVolume *> *a = this->boundingVolumes;
        if (a != 0) {
            for (unsigned i = 0; i < a->size(); i++) {
                BoundingVolume *e = a->data()[i];
                if (e->collide(x, y, z) != 0) {
                    return 1;
                }
                a = this->boundingVolumes;
            }
        }
    }
    return 0;
}


void PlayerFighter::setBV(BoundingVolume *bv) {
    Array<BoundingVolume *> *a = new Array<BoundingVolume *>();
    this->boundingVolumes = a;
    // setBV() tail: append the bounding volume to the newly created array.
    ArrayAdd<BoundingVolume *>(bv, *a);
}


void PlayerFighter::setWingmanCommand(int cmd, KIPlayer *target) {
    int saved = this->field_0xe4;
    ((KIPlayer *)(this))->setWingmanCommand(cmd, target);
    if ((unsigned)(cmd - 2) < 2) {
        this->maneuverTimer = 0x1389;
        if (this->currentSpeed != 5.5f) {
            this->field_0x1c0 = 0x1389;
        }
        if (cmd == 2) {
            if (((Level *)(this->level))->getPlayerRoute() != 0) {
                Route *r = ((Level *)(this->level))->getPlayerRoute();
                this->commandRoute = r->getExactClone();
                this->field_0x1e4 = this->commandRoute->getCurrent();
                goto done;
            }
        } else if (target != 0) {
            goto done;
        }
        this->field_0xe4 = saved;
    } else if (cmd == 0) {
        this->field_0xe4 = saved;
        this->field_0x140 = (this->field_0x140 == 0);
    } else if (cmd == 1) {
        this->field_0x13d = 0;
        this->field_0x1e0 = 0;
    }
done:
    this->currentSpeed = this->speed;
    this->currentRotate = this->rotate;
}

void PlayerFighter::setSpeed(float v) {
    this->speed = v;
    this->field_0x13d = 0;
    this->currentSpeed = v;
}

int PlayerFighter::outerCollide(float x, float y, float z) {
    if ((unsigned)(this->state - 3) > 1) {
        Array<BoundingVolume *> *a = this->boundingVolumes;
        if (a != 0) {
            for (unsigned i = 0; i < a->size(); i++) {
                BoundingVolume *e = a->data()[i];
                if (e->outerCollide(x, y, z) != 0) {
                    return 1;
                }
                a = this->boundingVolumes;
            }
        }
    }
    return 0;
}

static inline void AEMath_VectorScale(void *out, float s, void *v) {  // out = s * v
    *(AbyssEngine::AEMath::Vector *)out =
        s * *(const AbyssEngine::AEMath::Vector *)v;
}

extern void *const gIP_guard __attribute__((visibility("hidden")));
extern const float gIP_strength __attribute__((visibility("hidden")));
extern void *const gIP_rngFn __attribute__((visibility("hidden")));     // DAT_000efdc0 (fn ptr)

typedef int (*RngFn)(int rng, int bound);

// PlayerFighter::initPush(Vector const& target, int radius) — target in r1, radius in r2.
void PlayerFighter::initPush(const Vector &target, int radius) {
    int *guardP = *(int **)gIP_guard;
    volatile int saved = *guardP;

    // pos = this->getPosition() — PlayerFighter inherits KIPlayer::getPosition
    // (binary vtable slot +0x28 resolves to KIPlayer::getPosition for a PlayerFighter).
    AbyssEngine::AEMath::Vector gp = this->getPosition();
    float pos[3] = { gp.x, gp.y, gp.z };

    float diff[3];
    *reinterpret_cast<Vector *>(diff) = target - *reinterpret_cast<const Vector *>(pos);
    float len = AEMath_VectorLength(diff);
    float r = VectorSignedToFloat(radius, 0);
    float t = 1.0f;
    if (len / r < 1.0f) t = len / r;
    int strength = (int)((1.0f - t) * gIP_strength);
    this->pushTimer = strength;
    this->pushDuration = strength;

    AbyssEngine::AEMath::Vector gp2 = this->getPosition();
    float pos2[3] = { gp2.x, gp2.y, gp2.z };
    float dir[3];
    *reinterpret_cast<Vector *>(dir) =
        *reinterpret_cast<const Vector *>(pos) - *reinterpret_cast<const Vector *>(pos2);
    float norm[3];
    AbyssEngine::AEMath::VectorNormalize((Vector *)norm, (Vector *)dir);
    this->pushNormal = *(Vector *)norm;

    RngFn rng = (RngFn)gIP_rngFn;
    int rngObj = (int)(intptr_t)gRandom;
    float rx = VectorSignedToFloat(rng(rngObj, 200) - 100, 0);
    float ry = VectorSignedToFloat(rng(rngObj, 200) - 100, 0);
    float rz = VectorSignedToFloat(rng(rngObj, 200) - 100, 0);
    float rvec[3] = {rx, ry, rz};
    float rnorm[3];
    AbyssEngine::AEMath::VectorNormalize((Vector *)rnorm, (Vector *)rvec);
    float scaled[3];
    AEMath_VectorScale(scaled, (float)strength, rnorm);
    this->pushImpulse = *(Vector *)scaled;

    return;
}

// hidden PC-relative pointer-to-pointer global (deref'd twice).
extern void *const gExhaustCanvas __attribute__((visibility("hidden")));

void PlayerFighter::setExhaustVisible(bool vis) {
    int geom = this->geometry;
    if (geom != 0) {
        int sub = this->subGeometry;
        int id = (sub != 0) ? *(int *)(sub + 0x14) : *(int *)(geom + 0x14);
        if (id != -1) {
            unsigned t = (unsigned)(unsigned long)gCanvas->TransformGetTransform(*(unsigned *)gExhaustCanvas);
            // setExhaustVisible() tail: toggle the exhaust transform's visibility.
            return ((AbyssEngine::Transform *)(unsigned long)t)->SetVisible(vis);
        }
    }
}

extern void *const gR_g3 __attribute__((visibility("hidden")));   // case 3 transform-id global
extern void *const gR_g4 __attribute__((visibility("hidden")));   // case 4 transform-id global
extern void *const gR_g5 __attribute__((visibility("hidden")));   // default-case transform-id global

void PlayerFighter::render() {
    if (this->wreckGeometry != 0) {
        ((AEGeometry *)(0))->render();
    }
    int active = ((Player *)(this->player))->isActive();
    int st = this->state;
    unsigned *idp;
    if (active != 0 && (unsigned)(st - 3) < 2) {
        if (this->explosion != 0) {
            ((Explosion *)(0))->render();
            st = this->state;
        }
        if (st == 4) {
            if (this->field_0x1f8 >= 300) {
                goto done;
            }
            idp = *(unsigned **)gR_g4;
        } else {
            if (st != 3) {
                goto done;
            }
            if (this->subGeometry == 0) {
                // render() tail: render the bare geometry (no sub-geometry override).
                return ((AEGeometry *)(intptr_t)this->geometry)->render();
            }
            idp = *(unsigned **)gR_g3;
        }
    } else {
        if (active != 0 || st != 5) {
            goto done;
        }
        if (this->subGeometry == 0) {
            ((AEGeometry *)(0))->render();
        } else {
            idp = *(unsigned **)gR_g5;
            unsigned char tmp[60];
            unsigned id = *idp;
            void *src = gCanvas->TransformGetLocal(id);
            memcpy(tmp, src, 0x3c);
            gCanvas->TransformSetLocal(*idp, *(const AbyssEngine::AEMath::Matrix *)(*(void **)(this->subGeometry + 0xc)));
            ((AEGeometry *)(0))->render();
            gCanvas->TransformSetLocal(*idp, *(const AbyssEngine::AEMath::Matrix *)(*(void **)(this->subGeometry + 0xc)));
        }
        if (this->trail != 0) {
            ((AbyssEngine::Trail *)(0))->render();
        }
        goto done;
    }
    {
        unsigned char tmp[60];
        unsigned id = *idp;
        void *src = gCanvas->TransformGetLocal(id);
        memcpy(tmp, src, 0x3c);
        gCanvas->TransformSetLocal(*idp, *(const AbyssEngine::AEMath::Matrix *)(*(void **)(this->subGeometry + 0xc)));
        ((AEGeometry *)(0))->render();
        gCanvas->TransformSetLocal(*idp, *(const AbyssEngine::AEMath::Matrix *)(*(void **)(this->subGeometry + 0xc)));
    }
done:
    ;
}

extern "C" void AEMath_MatrixMul(void *out, void *m);          // operator*(out, m)
// Vector scale helper used 3x: out = vec * scalar.

extern void *const gPush_guard __attribute__((visibility("hidden")));
extern const float gPush_div __attribute__((visibility("hidden")));

// PlayerFighter::push(int dt) — self in r0, dt in r1.
void PlayerFighter::push(int dt) {
    int *guardP = *(int **)gPush_guard;
    volatile int saved = *guardP;

    if (0 < this->pushTimer) {
        int remaining = this->pushTimer - dt;
        float fr = VectorSignedToFloat(remaining, 0);
        float ftotal = VectorSignedToFloat(this->pushDuration, 0);
        this->pushTimer = remaining;
        float frac = fr / ftotal;

        unsigned char rot[60];
        AEMath_MatrixSetRotation(rot, frac * this->pushImpulse.z, 0.0f,
                                 frac * this->pushImpulse.y);

        int lo = this->deltaTime;
        int hi = this->deltaTimeHi;
        if ((int)(unsigned)(lo == 0) <= hi) {
            void *geom = (void *)(intptr_t)this->geometry;
            void *m = (void *)&((AEGeometry *)((int)(long)geom))->getMatrix();
            AEMath_MatrixMul(rot, m);
            // The recovered argument is the rot buffer just multiplied above; the
            // decompiler had dropped the second (matrix) operand of setMatrix.
            ((AEGeometry *)((int)(long)geom))->setMatrix(*(const Matrix *)rot);
            lo = this->deltaTime;
            hi = this->deltaTimeHi;
        }
        float speed = __aeabi_l2f(((long long)hi << 32) | (unsigned)lo);
        void *geom = (void *)(intptr_t)this->geometry;
        float ftotal2 = VectorSignedToFloat(this->pushDuration, 0);

        unsigned char a[12], b[12], c[60];
        PF_vscale(a, &this->pushNormal, speed);
        PF_vscale(b, a, this->currentSpeed);
        PF_vscale(c, b, (2.0f - frac) * 3.0f * (ftotal2 / gPush_div));
        ((AEGeometry *)(geom))->translate(*(Vector *)c);
    }

    return;
}

// hidden PC-relative function-pointer global (Vector zero-set helper), used 3x.
extern void *const gReset_vfn __attribute__((visibility("hidden")));

typedef void (*VFn)(void *dst, void *zeroVec);

// PlayerFighter::reset() -- re-initialise the fighter's transient state in place:
// reset the KIPlayer base, snap the working/render position back to the spawn
// position, clear timers, cloaking and crate flags, and re-arm cloaking.
void PlayerFighter::reset()
{

    ((KIPlayer *)(this))->reset();
    this->crateCaptured = 1;

    // Snap the working and render positions back to the spawn point.
    Vector spawn = { this->posX, this->posY, this->posZ };
    this->workingPosition = spawn;
    this->renderPosition  = spawn;

    this->deltaTime = 0;
    this->deltaTimeHi = 0;
    this->field_0x38 = 0;
    this->field_0x12e = 0;
    this->field_0x148 = 0;
    this->field_0x12c = 0;
    this->maneuverTimer = 0;
    this->field_0x1c0 = 0;
    this->field_0x1c4 = 0;
    this->field_0x1c8 = 0;
    if (this->state != 5) {
        this->state = 0;
    }

    VFn vfn = (VFn)gReset_vfn;
    int z[3] = {0, 0, 0};
    vfn(&this->resetVecA, z);
    vfn(&this->resetVecB, z);
    vfn(&this->resetVecC, z);

    this->isMissionCrate = 0;
    this->missionCrateLost = 0;
    this->crateLost = 0;
    this->crateCaptured = 1;
    this->field_0x140 = 0;
    this->field_0x1fc = 0;
    this->field_0x13c = 0;
    this->cloakActive = 0;
    this->cloakTimer = 0;
    this->cloakDuration = 0;
    this->cloakCooldown = 0;
    this->cloakingPossible = 1;
    this->aiDisabled = 0;
}

// Tail-call veneers (cloak start / stop / material-fade-apply).
extern "C" void PF_cloakStart(PlayerFighter *self);                       // -> 0x1ac1e8
extern "C" void PF_cloakStop(PlayerFighter *self, int on);               // -> 0x1ac1f8
extern "C" void PF_cloakApply(void *meshPtr, int arg, float alpha, int flag); // -> 0x1ac208

extern const float gHC_divIn __attribute__((visibility("hidden")));
extern const float gHC_divOut __attribute__((visibility("hidden")));

// PlayerFighter::handleCloaking() — self in r0.
void PlayerFighter::handleCloaking() {
    if (this->wingmanCommand != 10) return;
    if (this->geometry == 0) return;
    if (((KIPlayer *)(this))->isDead() != 0) return;
    if (*(char *)(this->player + 0x68) != 0) return;
    if (this->cloakingPossible == 0) return;

    if (this->cloakActive != 0) {
        int delta = this->cloakTimer;
        if (delta == 0) {
            unsigned matId = this->cloakMaterial;
            this->field_0x13c = 1;
            if (matId == 0xffffffff) {
                gCanvas->MeshCloneMaterial(*(unsigned *)(this->subGeometry + 0x1c),
                                              &this->cloakMaterial);
                int mp = (int)(long)gCanvas->MeshGetPointer(
                                                    *(unsigned *)(this->subGeometry + 0x1c));
                matId = this->cloakMaterial;
                this->cloakSavedMode = *(int *)(*(int *)(mp + 0x30) + 0x20);
            }
            int mat = (int)(long)gCanvas->MaterialGetMaterial(matId);
            *(int *)(mat + 0x20) = 0xe;
            gCanvas->MeshChangeMaterial(*(unsigned *)(this->subGeometry + 0x1c),
                                           this->cloakMaterial);
            if (this->field_0x2d9 == 0) {
                delta = this->cloakTimer;
            } else {
                this->field_0x2d9 = 0;
                delta = 2000;
                this->cloakTimer = 2000;
            }
        }
        long long acc = (long long)delta +
                        (((long long)this->deltaTimeHi << 32) | (unsigned)this->deltaTime);
        int total = (int)acc;
        this->cloakTimer = total;

        if (total - this->deltaTime <= 2000) {
            if (1999 < total) {
                ((PlayerFighter *)(this))->setExhaustVisible(false);
                this->exhaustHidden = 1;
            }
            void *cv = (void *)gCanvas;
            int mp = (int)(long)gCanvas->MeshGetPointer(*(unsigned *)(this->subGeometry + 0x1c));
            float a = VectorSignedToFloat(this->cloakTimer, 0) / gHC_divIn;
            PF_cloakApply((void *)(long)mp, (int)(long)cv, a, 1);
            return;
        } else {
            if (this->cloakDuration < total) {
                int restore = this->cloakSavedMode;
                this->cloakTimer = 0;
                this->cloakActive = 0;
                this->field_0x13c = 0;
                int mat = (int)(long)gCanvas->MaterialGetMaterial(this->cloakMaterial);
                *(int *)(mat + 0x20) = restore;
                PF_cloakStop(this, 1);
                return;
            }
            if (total <= this->cloakDuration - 2000) {
                return;
            }
            this->exhaustHidden = 0;
            void *cv = (void *)gCanvas;
            int mp = (int)(long)gCanvas->MeshGetPointer(*(unsigned *)(this->subGeometry + 0x1c));
            float a = VectorSignedToFloat(this->cloakTimer, 0);
            float b = VectorSignedToFloat(this->cloakDuration - 2000, 0);
            float alpha = (a - b) / gHC_divOut + 1.0f;
            PF_cloakApply((void *)(long)mp, (int)(long)cv, alpha, 1);
            return;
        }
    }

    if (this->field_0x1e0 != 0 && PF_nextInt((int)(intptr_t)gRandom) < 0x32) {
        PF_cloakStart(this);
        return;
    }
    int acc = this->deltaTime + this->cloakCooldown;
    this->cloakCooldown = acc;
    if (8000 < acc) {
        this->cloakCooldown = 0;
        if (PF_nextInt((int)(intptr_t)gRandom) < 0x1e) {
            PF_cloakStart(this);
        }
    }
}

// PlayerFighter::revive() -- bring a (re-)spawning fighter back to life: reset the
// owning Player, restore enemy alignment, re-arm route/explosion/exhaust, refill
// hitpoints and speed, make the ship geometry visible again, and rebuild its loot
// list (cleared for wingman commands 9/10, regenerated otherwise).
void PlayerFighter::revive()
{
    int enemy = *(uint8_t *)((char *)(intptr_t)this->player + 0xe0);
    ((Player *)(this->player))->reset();
    if (enemy != 0) {
        ((Player *)(this->player))->turnEnemy();
    }
    this->wreckGeometry = 0;
    this->state = 1;
    this->field_0x12e = 0;
    this->field_0x38 = -1;
    this->routeClone->reset();
    this->hitpoints = ((Player *)(this->player))->getHitpoints();
    this->field_0x1dc = 0;
    this->field_0x1e0 = 0;
    this->deathTimer = 0;
    this->field_0xf0 = 0;
    this->currentSpeed = this->speed;
    this->explosion->reset();
    this->pushTimer = 0;
    this->field_0x24 = 0;
    this->setExhaustVisible(true);

    int geom = this->subGeometry;
    this->field_0xf5 = 1;
    if (geom == 0) {
        geom = this->geometry;
    }
    ((AEGeometry *)(geom))->setVisible(1);

    if ((unsigned)(this->wingmanCommand - 9) < 2) {
        delete this->lootList;
        this->lootList = 0;
    } else {
        Generator *g = new Generator();
        delete this->lootList;
        this->lootList = 0;
        this->lootList = g->getLootList(-1, -1);
        delete g;
    }
}

// All terminal tail-call veneers in this TU have been resolved to their real
// inherited / engine base targets and modeled directly at the call sites above
// (see the table at the top of the file). No forwarding shims remain.
