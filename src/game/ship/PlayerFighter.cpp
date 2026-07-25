#include "game/ship/PlayerFighter.h"
#include "game/core/Globals.h"
#include "engine/core/AERandom.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/Mesh.h"
#include "engine/render/Material.h"
#include "game/mission/Mission.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/ParticleSystemManager.h"
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
#include "game/weapons/Radar.h"
#include "engine/audio/FModSound.h"
#include "game/world/SpacePoint.h"
#include "game/world/Wanted.h"
#include "game/ship/PlayerFixedObject.h"

int AERandom_nextInt_nobound(int rng);

static inline int PF_nextInt(int rng) { return AERandom_nextInt_nobound(rng); }

float VectorSignedToFloat(int v, int mode);

int *RH_op_new_arr(unsigned int n);

void RH_op_delete_arr(void *p); // lint: void_ptr (external symbol; generic deallocator, param type load-bearing)

void PF_update_dead(PlayerFighter * self);

namespace AbyssEngine {
    namespace AEMath {
        float VectorLength(const Vector &value);

        Vector operator-(const Vector &lhs, const Vector &rhs);

        Vector operator*(float lhs, const Vector &rhs);
    }
}

int AERandom_nextIntB(int rng, int bound);

uint8_t PlayerFighter::hasMissionCrateLost() {
    return this->missionCrateLost();
}

uint8_t PlayerFighter::hasMissionCrateCaptured() {
    return this->missionCrateCaptured();
}

void PlayerFighter::setShootError(int v) {
    this->shootError = (float) v;
}

void PlayerFighter::setAIDisabled(bool v) {
    this->aiDisabled = v;
}

void PlayerFighter::setShipGroup(AEGeometry *geom, int group, bool flag) {
    KIPlayer::setShipGroup(geom, group, flag);
}

void PlayerFighter::awake() {
    this->state = 1;
    ((Player *) (this->player()))->setActive(1);
    ((PlayerFighter *) (this))->setExhaustVisible(true);
    int geom = this->subGeometry();
    this->field_0xf5() = 1;
    if (geom == 0) {
        geom = this->geometry();
    }

    return ((AEGeometry *) (intptr_t) geom)->setVisible(true);
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

        return self->handleCloaking();
    }
}

void PlayerFighter::removeTrail() {
    delete this->trail;
    this->trail = 0;
}

PlayerFighter::~PlayerFighter() {
    delete this->route;
    this->route = 0;

    if (this->boundingVolumes != 0) {
        ArrayReleaseClasses(*this->boundingVolumes);
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
    return this->crateCaptured() == 0;
}

void PlayerFighter::setPosition(const Vector &v) {
    this->setPosition(v.x, v.y, v.z);
}

void PlayerFighter::setRotate(int v) {
    float f = (float) v;
    this->field_0x13d = 0;
    this->rotate = f;
    this->currentRotate = f;
}

uint8_t PlayerFighter::hasCrateLost() {
    return this->crateLost();
}




static inline int PF_getRefMatrix(int geom) {
    return (int) (intptr_t) &((AEGeometry *) (intptr_t) geom)->getReferenceMatrix();
}
static inline int PF_addSystem(int base, int matrix, int kind, int flag) {
    return ((ParticleSystemManager *) (intptr_t) base)
        ->addSystem((const AbyssEngine::AEMath::Matrix *) (intptr_t) matrix,
                    (ParticleSettings::ParticleSet) kind, flag);
}
static inline void PF_enableEmit(int base, int handle, int enable) {
    ((ParticleSystemManager *) (intptr_t) base)->enableSystemEmit(handle, enable);
}

void PlayerFighter::setLevel(Level *lvl) {
    ((KIPlayer *) (this))->setLevel(lvl);
    Level *lev = (Level *) (intptr_t) this->level();

    int v;
    v = PF_addSystem((int) (intptr_t) lev->field_74, PF_getRefMatrix(this->geometry()), 9, 0);
    this->engineTrailSystem = v;
    PF_enableEmit((int) (intptr_t) lev->field_74, v, 0);

    v = PF_addSystem((int) (intptr_t) lev->particleEmitBoolPtr, PF_getRefMatrix(this->geometry()), 0xf, 0);
    this->field_0x80 = v;
    PF_enableEmit((int) (intptr_t) lev->particleEmitBoolPtr, v, 0);

    v = PF_addSystem((int) (intptr_t) lev->particleRenderBoolPtr, PF_getRefMatrix(this->geometry()), 0x2a, 0);
    this->field_0x84 = v;
    PF_enableEmit((int) (intptr_t) lev->particleRenderBoolPtr, v, 0);

    v = PF_addSystem((int) (intptr_t) lev->field_8c, PF_getRefMatrix(this->geometry()), 0x11, 0);
    this->field_0x134 = v;
    PF_enableEmit((int) (intptr_t) lev->field_8c, v, 0);

    v = PF_addSystem((int) (intptr_t) lev->field_8c, PF_getRefMatrix(this->geometry()), 0x12, 0);
    this->field_0x138 = v;
    return PF_enableEmit((int) (intptr_t) lev->field_8c, v, 0);
}


PlayerFighter::PlayerFighter(int faction, int wingmanCmd, Player *player, AEGeometry *geom,
                             float x, float y, float z, bool flag)
    : KIPlayer(faction, wingmanCmd, player, geom, x, y, z, flag) {
    PlayerFighter * self = this;

    self->field_0x200 = 0;
    self->field_0x204 = 0;
    self->field_0x208 = 0;

    self->workingPosition = (Vector)
    {
        0, 0, 0
    };
    self->resetVecB = (Vector)
    {
        0, 0, 0
    };
    self->resetVecC = (Vector)
    {
        0, 0, 0
    };
    self->easeBaseMatrix = AbyssEngine::AEMath::Matrix();
    self->rollMatrix = AbyssEngine::AEMath::Matrix();

    int rng = (int) (intptr_t) Globals::rnd;

    float wp[12];
    int r;
    r = AERandom_nextIntB(rng, 25000);
    wp[0] = (float)(r - 30000);
    int r1 = AERandom_nextIntB(rng, 10000);
    int r2 = AERandom_nextIntB(rng, 25000);
    wp[2] = (float)(r2 + 20000);
    wp[1] = (float)(r1 - 10000);
    r = AERandom_nextIntB(rng, 25000);
    wp[3] = (float)(r + 5000);
    r1 = AERandom_nextIntB(rng, 10000);
    r2 = AERandom_nextIntB(rng, 25000);
    wp[5] = (float)(r2 + 20000);
    wp[4] = (float)(r1 - 10000);
    r = AERandom_nextIntB(rng, 25000);
    wp[6] = (float)(r + 5000);
    r1 = AERandom_nextIntB(rng, 10000);
    r2 = AERandom_nextIntB(rng, 25000);
    wp[8] = (float)(r2 + 55000);
    wp[7] = (float)(r1 - 10000);
    r = AERandom_nextIntB(rng, 25000);
    wp[9] = (float)(r - 30000);
    r1 = AERandom_nextIntB(rng, 10000);
    r2 = AERandom_nextIntB(rng, 25000);
    wp[10] = (float)(r1 - 10000);
    wp[11] = (float)(r2 + 55000);

    int count = AERandom_nextIntB(rng, 3) * 3 + 6;
    char used[4] = {0, 0, 0, 0};
    unsigned long long bytes = (unsigned long long) (unsigned) count * 4;
    unsigned sz = (int) (bytes >> 32) != 0 ? 0xffffffff : (unsigned) bytes;
    int *pts = RH_op_new_arr(sz);
    for (int i = 0; i < count; i += 3) {
        int idx;
        do {
            idx = AERandom_nextIntB(rng, 4);
        } while (used[idx] != 0);
        used[idx] = 1;
        pts[i] = (int) wp[idx * 3];
        pts[i + 1] = (int) wp[idx * 3 + 1];
        pts[i + 2] = (int) wp[idx * 3 + 2];
    }
    self->route = new Route(pts, (unsigned) count);
    RH_op_delete_arr(pts);

    // Block 3b: stationRouteAliens lazy init (dc576..dc5a0)
    if (PlayerFighter::stationRouteAliens == 0) {
        static const int stationWaypoints[12] = {
            40000, 0, 40000,
            40000, 0, -40000,
            -40000, 0, -40000,
            -40000, 0, 40000
        };
        int wpcopy[12];
        memcpy(wpcopy, stationWaypoints, sizeof(wpcopy));
        PlayerFighter::stationRouteAliens = (int)(intptr_t)new Route(wpcopy, 12);
    }

    // Block 4: field initialization (dc5a0..dc636)
    self->field_0x130 = -1;
    self->field_0x134 = -1;
    self->field_0x138 = -1;
    self->field_0x128 = 50000;
    self->rotate = 2.0f;
    self->shootError = 0x1.f3ffec0000000p-8f;
    self->speed = 2.0f;
    self->field_0x1b0 = 0x5dc;
    self->boostProb = 5;
    self->field_0x38 = 0;
    self->field_0x140 = 0;
    // Binary: strh zeros [+0x12d] which covers field_0x12d + field_0x12e
    self->field_0x12d = 0;
    self->field_0x12e = 0;
    // Binary: strh zeros [+0x13d] which covers field_0x13d + field_0x13e
    self->field_0x13d = 0;
    self->field_0x13e = 0;
    self->field_0x1f8 = 0;
    self->deathTimer() = 0;
    self->field_0x12f = 0;
    self->field_0x1fc = 0;
    self->field_0x1c8 = 0;
    self->field_0x1e0 = 0;
    self->field_0x1f4 = 0;
    self->wingmanCommand() = wingmanCmd;

    // Binary: vst1 q8=0 zeros +0x1b8..+0x1c7 (maneuverTimer, field_0x1bc, field_0x1c0, field_0x1c4)
    self->maneuverTimer = 0;
    self->field_0x1bc = 0;
    self->field_0x1c0 = 0;
    self->field_0x1c4 = 0;
    // Binary: vst1 q8=0 zeros +0x148..+0x157 (targetPlayer, commandRoute, boundingVolumes, trail)
    self->targetPlayer = nullptr;
    self->commandRoute = 0;
    self->boundingVolumes = 0;
    self->trail = 0;
    // Binary: vst1 q8=0 zeros +0x1d0..+0x1df (deltaTime, deltaTimeHi, hitpoints, field_0x1dc)
    self->deltaTime = 0;
    self->deltaTimeHi = 0;
    self->hitpoints = 0;
    self->field_0x1dc = 0;
    // Binary: vst1.32 q8=0 zeros +0x1e4..+0x1eb (field_0x1e4, currentSpeed)
    self->field_0x1e4 = 0;
    self->currentSpeed = 0;

    // Block 5: workingPosition from params, then route setup (dc622..dc692)
    self->workingPosition = Vector{x, y, z};
    self->field_0x13d = 1;
    self->crateCaptured() = 1;
    self->currentSpeed = self->speed;
    self->currentRotate = self->rotate;
    // Binary dc656: route->setLoop(true); dc65c: stationRouteAliens->setLoop(true)
    self->route->setLoop(1);
    ((Route *)(intptr_t)PlayerFighter::stationRouteAliens)->setLoop(1);
    self->routeClone() = 0;

    if (Globals::status->getCurrentCampaignMission() != 0x29) {
        if (wingmanCmd == 9) {
            // Binary: clones stationRouteAliens ([r5,#0])
            self->routeClone() = ((Route *)(intptr_t)PlayerFighter::stationRouteAliens)->clone();
        } else {
            self->routeClone() = self->route->clone();
        }
    }
    self->field_0x12d = 0;

    if (wingmanCmd == 9) {
        self->lootList() = 0;
    } else {
        Generator *g = new Generator();
        self->lootList() = g->getLootList(-1, -1);
        delete g;
    }

    self->field_0x128 = (Globals::status->inAlienOrbit() != 0) ? 100000 : 50000;

    Explosion *exp = new Explosion(0);
    self->explosion = exp;
    exp->addFireStreaks();
    self->field_0x13e = 1;
    self->hitpoints = ((Player *) (self->player()))->getHitpoints();
    self->field_0x1dc = 0;
    self->field_0x1e0 = 0;
    self->field_0xe4 = 1;
    self->field_0x25 = 1;
    self->engineTrailSystem = -1;

    int fov;
    if (Globals::status->getCurrentCampaignMission() == 1) {
        fov = -1;
    } else {
        fov = (self->field_0xdc() == 0) ? 0x2e : 0x30;
    }
    self->fov() = fov;

    // Binary: strd zeros targetRoll+smoothRoll together, then field_0x214
    self->targetRoll = 0;
    self->smoothRoll = 0;
    self->field_0x214 = 0;
    // Binary: strd stores int bit patterns for roll threshold and rate constants
    self->field_0x294 = 0x443b8000;  // 750.0f stored as int bits
    self->field_0x298 = 0x41723ace;  // ~15.14f stored as int bits
    // Binary: loop str zeros [r8+r1] for r1=0x29c..0x2af (5 words = rollSamples array)
    self->rollSamples[0] = 0;
    self->rollSamples[1] = 0;
    self->rollSamples[2] = 0;
    self->rollSamples[3] = 0;
    self->rollSamples[4] = 0;
    self->rollBufferFilled = 0;
    self->rollSampleIndex = 0;
    self->rollActive() = 0;
    // Binary: strh zeros [+0x254] covering field_0x254 + field_0x255 together
    self->field_0x254 = 0;
    self->field_0x255 = 0;
    self->easeMatrix = 0;
    // Binary: strd zeros spacePoint + cloakTimer; then overlapping stores zero cloakDuration + cloakActive
    self->field_0x2bc = 0;
    self->field_0x2c0 = 0;
    self->spacePoint = 0;
    self->cloakTimer = 0;
    self->cloakDuration = 0;
    self->cloakActive = 0;
    self->cloakCooldown = 0;
    self->cloakingPossible = 1;
    self->cloakMaterial = -1;
    self->aiDisabled = 0;
    self->gunSwitchTimer = 0;

    return;
}


void PlayerFighter::update(int dt) {
    // Chunk 1: Entry guard — dead state with explosion finished
    if (this->state == 4 && this->explosion->isPlaying() == 0 &&
        (this->crateCaptured() == 0 || 60000 < this->deathTimer())) {
        PF_update_dead(this);
        return;
    }

    // Chunk 2: Timer increment + position snapshot
    this->field_0x1c0 += dt;
    this->maneuverTimer += dt;
    if (this->wingmanCommand() == 1) {
        this->wingmanCommand() = 1;
    }
    this->deltaTime = dt;
    this->deltaTimeHi = dt >> 31;

    {
        Vector pos = ((AEGeometry *)(intptr_t)this->geometry())->getPosition();
        this->renderPosition() = pos;
    }

    // Chunk 3: Enemy/friend flag update
    if (this->field_0x43() == 0) {
        // Enemy flag
        unsigned char enemy;
        if ((this->wingmanCommand() & ~1) == 8) {
            enemy = 1;
        } else if (((KIPlayer *)(this))->isWingMan() != 0) {
            enemy = 0;
        } else {
            enemy = (unsigned char)((Standing *)(Globals::status->getStanding()))->isEnemy(this->wingmanCommand());
        }
        ((Player *)(intptr_t)this->player())->enemyFlagsLo = enemy;

        // Friend flag (symmetric pass)
        unsigned char friend_flag;
        if ((this->wingmanCommand() & ~1) == 8) {
            friend_flag = 0;
        } else if (((KIPlayer *)(this))->isWingMan() != 0) {
            friend_flag = 1;
        } else {
            friend_flag = (unsigned char)((Standing *)(Globals::status->getStanding()))->isFriend(this->wingmanCommand());
        }
        ((Player *)(intptr_t)this->player())->carriesFriendCargoFlag = friend_flag;
    } else {
        // field_0x43 != 0: HP-based enemy override (region D)
        int damage_dealt = ((Player *)(intptr_t)this->player())->damageDoneByPlayer;
        int hp_threshold = ((Player *)(intptr_t)this->player())->getMaxHitpoints() / 20;
        if (damage_dealt > hp_threshold) {
            ((Player *)(intptr_t)this->player())->enemyFlagsLo = 1;
        }
    }

    // Chunk 3b: Gun switch timer (region E — field_0x42 gate)
    if (this->KIPlayer::field_0x42 != 0) {
        this->gunSwitchTimer += dt;
        if (this->gunSwitchTimer > 20000) {
            this->gunSwitchTimer = 0;
            if (((Player *)(intptr_t)this->player())->gunAvailable(1)) {
                this->field_0x140 = (this->field_0x140 == 0) ? 1 : 0;
            }
        }
    }

    // Chunk 3c: isAlwaysEnemy / lockedEnemy radar check (dd154..dd178 → dd62a)
    if (((Player *)(intptr_t)this->player())->isAlwaysEnemy() == 0) {
        PlayerEgo *ego = ((Level *)(intptr_t)this->level())->getPlayer();
        if (ego != 0) {
            ego = ((Level *)(intptr_t)this->level())->getPlayer();
            if (ego->field_0x14 != 0 &&
                ego->field_0x14->lockedEnemy == (KIPlayer *)this &&
                this->field_0x43() == 0) {
                ((Player *)(intptr_t)this->player())->setAlwaysEnemy(1);
                ((Level *)(intptr_t)this->level())->uncoverWanted(this->KIPlayer::field_0x48);
            }
        }
    }

    // Chunk 4a: Black market / turnedEnemy / isAlwaysFriend flag overrides (dd17c..dd1c6)
    {
        Status *st = Globals::status;
        if (st->inBlackMarketSystem() != 0) {
            if (st->field_110 != 0) {
                if (this->wingmanCommand() == 8) {
                    reinterpret_cast<uint16_t &>(((Player *)(intptr_t)this->player())->enemyFlagsLo) = 0;
                }
            }
        }
        Player *pl = (Player *)(intptr_t)this->player();
        if (pl->turnedEnemy() != 0) {
            reinterpret_cast<uint16_t &>(pl->enemyFlagsLo) = 1;
        }
        if (pl->isAlwaysFriend() != 0) {
            reinterpret_cast<uint16_t &>(pl->enemyFlagsLo) = 0x100;
        }
    }

    // Chunk 4b: wingmanFlag — lazy route-clone init (dd1c6..dd1da)
    if (this->wingmanFlag == 0) {
        if (this->KIPlayer::route == 0) {
            this->KIPlayer::route = this->route->clone();
        }
    }

    // Chunk 4c: Trail tick update (dd1da..dd20a)
    {
        this->field_0x1c8 += dt;
        if (this->field_0x1c8 >= 201) {
            if (this->trail != 0) {
                this->trail->update(this->renderPosition(), this->workingPosition);
                this->workingPosition = this->renderPosition();
            }
            this->field_0x1c8 = 0;
        }
    }

    // Chunk 4d: Player::transform = geometry->getMatrix() (dd20a..dd22c)
    reinterpret_cast<AbyssEngine::AEMath::Matrix &>(
            ((Player *)(intptr_t)this->player())->transform[0]) =
            ((AEGeometry *)(intptr_t)this->geometry())->getMatrix();
    if (this->subGeometry() != 0) {
        reinterpret_cast<AbyssEngine::AEMath::Matrix &>(
                ((Player *)(intptr_t)this->player())->transform[0]) *=
                ((AEGeometry *)(intptr_t)this->subGeometry())->getMatrix();
    }

    // Hoisted to avoid goto-over-init: enemies is used from Chunk 5b onward
    Array<Player *> *enemies = nullptr;

    // Chunk 5: Wingman flag / state dispatch (dd22c..dd27e)
    {
        int s5 = this->state;
        Player *pl5 = (Player *)(intptr_t)this->player();
        bool stop5 = false;
        if (this->wingmanFlag != 0) {
            if (s5 == 4 || this->KIPlayer::route == 0) {
                stop5 = true;
            } else {
                // wingmanFlag && state!=4 && route!=0: fetch ego geometry pos → resetVecA
                PlayerEgo *ego2 = ((Level *)(intptr_t)this->level())->getPlayer();
                if (ego2 != 0) {
                    this->resetVecA() = ego2->geometry->getPosition();
                }
                // if field_0xe4 == 1: escort path (dd2e0, handled by PF_update_body)
                // if field_0xe4 != 1: jump back to dd240 = state 3/4 check below
                if (this->KIPlayer::field_0xe4 == 1) {
                    goto update_body;
                }
            }
        }
        if (!stop5 && (unsigned)(s5 - 3) < 2u) {
            stop5 = true;
        }
        if (stop5) {
            pl5->StopEngineSound();
            goto update_body;
        }
        // Normal path: call Player::update
        int fov_p1 = this->fov() + 1;
        bool boost = (fov_p1 != 0) && (s5 != 5);
        pl5->update(dt, boost);
    }

    // Chunk 5b: getEnemies + aiDisabled gate (dd26c..dd27e)
    enemies = ((Player *)(intptr_t)this->player())->getEnemies();
    if (this->aiDisabled != 0) {
        goto update_body;
    }

    // Chunk 6: target index validation + route follow when no enemies (dd27e..dd3c6)
    if (enemies == nullptr) {
        // Route follow path (dd35c..dd3c0)
        Route *kr = this->KIPlayer::route;
        if (kr == nullptr) {
            this->KIPlayer::state = 5;
            goto update_body;
        }
        {
            Vector pos = ((AEGeometry *)(intptr_t)this->geometry())->getPosition();
            kr->update(pos);
        }
        Waypoint *wp = kr->getWaypoint();
        if (wp == nullptr) {
            goto update_body;
        }
        this->resetVecB.x = (float)wp->x;
        this->resetVecB.y = (float)wp->y;
        this->resetVecB.z = (float)wp->z;
        this->field_0x12c = 1;
        goto update_body;
    } else {
        // Target index validation (dd27e..dd3c6)
        int tgt = this->KIPlayer::field_0x38;
        if ((unsigned)tgt >= enemies->count) {
            tgt = -1;
            this->KIPlayer::field_0x38 = -1;
        }
        if (this->field_0x12e == 0) {
            this->KIPlayer::field_0x38 = -1;
        } else if (tgt >= 0) {
            Player *ep = (*enemies)[tgt];
            if (ep->isActive() == 0) {
                this->field_0x12e = 0;
            }
        }
    }

    // Chunk 7: maneuver timer gate + target-scan RNG entry (dd3c6..dd436)
    {
        int mtimer = this->maneuverTimer;
        this->targetPlayer = nullptr;
        if (mtimer >= 5001) {
            // Timer expired: if flag was set, clear it; otherwise roll for new shoot-chance flag
            uint8_t new_flag;
            if (this->field_0x12d != 0) {
                new_flag = 0;
            } else {
                new_flag = (uint8_t)(Globals::rnd->nextInt(100) < 20 ? 1 : 0);
            }
            this->maneuverTimer = 0;
            this->field_0x12d = new_flag;
            // Second RNG roll: should we scan for a new target this frame?
            int r2 = Globals::rnd->nextInt(100);
            if (r2 <= 29 && enemies->count >= 2) {
                // Enter target scan loop (dd426..dd50a) — handled by PF_update_body
                this->field_0x12e = 0;
                goto update_body;
            }
            // No scan: clear target index
            this->KIPlayer::field_0x38 = 0;
        }
    }

    // Chunk 7b: post-timer dd50e wingman/death check (dd50e..dd542) handled by PF_update_body
    // when timer >= 5001; but timer < 5001 path jumps over to dd544 below.
    // Since PF_update_body handles dd50e-dd542 too, we fall through for the no-scan path.

    // dd544: if already have a locked target, go straight to PF_update_body (dd7ac path)
    if (this->field_0x12e != 0) {
        goto update_body;
    }

    // Chunk 8: linear enemy scan when no locked target (dd55a..dd620)
    {
        int tgt_idx = -1;
        unsigned int n = enemies->count;
        float R = (float)(int)this->field_0x128;
        for (unsigned int i = 0; i < n; i++) {
            Player *ep = enemies->data_[i];
            if (ep == nullptr) continue;
            if (!ep->isActive()) continue;
            if (ep->isDead()) continue;
            // Get enemy position and store in resetVecA
            Vector ep_pos = ep->getPosition();
            this->resetVecA() = ep_pos;
            // TurnedEnemy check (skip if wingmanCommand==8)
            if (this->KIPlayer::shipGroup != 8) {
                if (((Player *)(intptr_t)this->player())->turnedEnemy() != 0) {
                    // found via turned-enemy: store target
                    this->field_0x12e = 1;
                    this->KIPlayer::field_0x38 = (int)i;
                    goto update_body;
                }
            }
            // Range check: AABB within attack radius R
            float dx = this->renderPosition().x - this->resetVecA().x;
            float dy = this->renderPosition().y - this->resetVecA().y;
            float dz = this->renderPosition().z - this->resetVecA().z;
            if (dx < R && dx > -R && dy < R && dy > -R && dz < R && dz > -R) {
                tgt_idx = (int)i;
                break;
            }
        }
        if (tgt_idx >= 0) {
            this->field_0x12e = 1;
            this->KIPlayer::field_0x38 = tgt_idx;
        }
    }

    // ------------ inlined PF_update_body begins here -------------------------
    update_body:;
    {
    using namespace AbyssEngine;
    using namespace AbyssEngine::AEMath;

    AEGeometry *geom = (AEGeometry *)(intptr_t)this->geometry();

    // de1a8: entry gate — if aiDisabled, just spin the reference matrix and cloak
    if (this->aiDisabled != 0) {
        geom->updateReferenceMatrix();
        this->handleCloaking();
        return;
    }

    // de1bc: state dispatch (tbh, states 0-9)
    int curState = this->state;
    if (curState > 9) {
        return; // de1dc: out-of-range -> epilogue
    }

    switch (curState) {
    // -----------------------------------------------------------------------
    // State 0: initialise — set state to 1, return
    // -----------------------------------------------------------------------
    case 0:
        this->state = 1;
        return;

    // -----------------------------------------------------------------------
    // State 2: dead / no-op — just return
    // -----------------------------------------------------------------------
    case 2:
        return;

    // -----------------------------------------------------------------------
    // State 1 / State 7: active AI — handleCloaking + damage particle logic
    // -----------------------------------------------------------------------
    case 1:
    case 7: {
        this->handleCloaking();

        // de204: check field_0x13d (damageEscape flag)
        if (this->field_0x13d == 0) {
            break; // skip to post-state movement (de9c6)
        }

        // de20e: compute accumulated damage delta
        int newDmg = this->deltaTime;    // field_0x1d0
        int fp_val = dt;                  // fp = dt at entry
        if (newDmg <= fp_val) {
            // de254: boost timer kick: field_0x1c0 = 10000
            // de214: skip to boost section
            break; // goto boost section via post-switch fall-through
        }
        // de21c: delta damage
        int delta = this->deltaTimeHi;   // field_0x1d4
        int newHp  = newDmg - fp_val;    // field_0x1d0 - dt
        this->deltaTime   = 0;           // field_0x1d0 = 0
        this->deltaTimeHi = newHp + delta; // field_0x1d4 += delta

        // de230: get HP percentage
        int maxHp = ((Player *)(intptr_t)this->player())->getMaxHitpoints();
        float pct = ((float)(newHp + delta) / (float)maxHp) * 100.0f;

        // de24c: if pct > 40.0: boost kick + break
        if (pct > 40.0f) {
            this->field_0x1c0 = 10000;   // de258
            this->field_0x1dc = 0;       // de262
            this->field_0x1e0 = 1;       // de268 (field_0x1e0 used as boostFlag)
            break; // de26c: goto boost section
        }
        break;
    }

    // -----------------------------------------------------------------------
    // State 3: dying spin + explosion
    // de2c0-de47e
    // -----------------------------------------------------------------------
    case 3: {
        // Clear death-trail flags
        this->field_0x101 = 0;           // de2d0
        this->field_0x13e = 0;           // de2d4

        // Build identity rotation matrix on stack, then set rotation from field_0x188
        Matrix rotMat;
        rotMat.m[0]  = 1.0f; rotMat.m[5]  = 1.0f; rotMat.m[10] = 1.0f; // diagonal
        rotMat.m[1]  = 0.0f; rotMat.m[2]  = 0.0f; rotMat.m[3]  = 0.0f;
        rotMat.m[4]  = 0.0f; rotMat.m[6]  = 0.0f; rotMat.m[7]  = 0.0f;
        rotMat.m[8]  = 0.0f; rotMat.m[9]  = 0.0f; rotMat.m[11] = 0.0f;
        rotMat.m[12] = 0.0f; rotMat.m[13] = 0.0f; rotMat.m[14] = 0.0f;
        AEMath::MatrixSetRotation(rotMat,
                                  this->field_0x188.x,
                                  this->field_0x188.y,
                                  this->field_0x188.z);

        // Apply rotation to geometry matrix if dt >= 1
        if (dt >= 1) {
            Matrix geoMat = geom->getMatrix();
            Matrix newMat = geoMat * rotMat;
            geom->setMatrix(newMat);
        }

        // Translate: direction * float(dt) * currentSpeed
        Vector dir = this->field_0x194 * (float)dt;
        Vector dirScaled = dir * this->currentSpeed;
        geom->translate(dirScaled);
        geom->updateReferenceMatrix();

        // Decrement death countdown
        this->field_0x1f8 -= dt;
        if (this->field_0x1f8 > -1) {
            return;
        }

        // Countdown expired: trigger explosion
        {
            Vector zeroVec = {0.0f, 0.0f, 0.0f};
            this->explosion->start(zeroVec, zeroVec);
        }

        // Disable engine trail emit
        ((Level *)(intptr_t)this->level())->field_74->enableSystemEmit(
            this->engineTrailSystem, false);

        // Disable damage particles if options[0x28] > 0
        if (Globals::options[0x28] > 0) {
            ((Level *)(intptr_t)this->level())->particleEmitBoolPtr->enableSystemEmit(
                this->field_0x80, false);
            ((Level *)(intptr_t)this->level())->particleSystemMgr->enableSystemEmit(
                this->field_0x84, false);
        }

        // Random bomb force: 50.0 + rnd(50) * 0.01
        int rndBomb = Globals::rnd->nextInt(50);
        float bombForce = 50.0f + (float)rndBomb * 0.01f;
        ((Player *)(intptr_t)this->player())->setBombForce(bombForce);

        // Random spin direction: 3 * rnd(200)-100, normalize -> field_0x17c
        {
            int rx = Globals::rnd->nextInt(200) - 100;
            int ry = Globals::rnd->nextInt(200) - 100;
            int rz = Globals::rnd->nextInt(200) - 100;
            Vector spinDir = {(float)rx, (float)ry, (float)rz};
            Vector normDir = VectorNormalize(spinDir);
            this->field_0x17c = normDir;
        }

        // State -> 4, reset countdown, cleanup spacePoint
        this->field_0x1f8 = 0;
        this->state = 4;

        if (this->spacePoint != 0) {
            SpacePoint *sp = (SpacePoint *)(intptr_t)this->spacePoint;
            if (!sp->isFree()) {
                sp->giveFree();
                this->spacePoint = 0;
            }
        }

        // de47e: hasCrateCaptured check (reached only from state 3 path)
        if (!this->crateCaptured()) {
            return;
        }
        // de488: if carriesMissionCrate != 0 && missionCrateCaptured != 0 -> special exit
        if (this->carriesMissionCrate != 0 && this->missionCrateCaptured() != 0) {
            // de458: clear crateCaptured, set empDisabledByte=1
            this->crateCaptured() = 0;
            ((Player *)(intptr_t)this->player())->empDisabledByte = 1;
            return;
        }
        // de498: create crate — shipGroup == 9 ? flag=1 : flag=0
        {
            int flag = ((this->shipGroup - 9) == 0) ? 1 : 0;
            this->createCrate(flag);
        }
        return;
    }

    // -----------------------------------------------------------------------
    // State 4: crate float / wreck hover
    // de4ac-de4c2
    // -----------------------------------------------------------------------
    case 4: {
        // de4ac: field_0x1f8 += dt; if >= 1: clear field_0x73, fall to post-state
        this->field_0x1f8 += dt;
        if (this->field_0x1f8 < 1) {
            // still floating: stay in state 4 movement
            break;
        }
        this->field_0x73 = 0;
        // fall through to post-state movement
        break;
    }

    // -----------------------------------------------------------------------
    // State 5: asleep proximity check
    // de4c4-de590
    // -----------------------------------------------------------------------
    case 5: {
        Player *pl5b = (Player *)(intptr_t)this->player();
        // de4c6: if !pl5b->enemyFlagsLo: skip visibility
        if (pl5b->enemyFlagsLo != 0) {
            // de4cc: if status->getCurrentCampaignMission() >= 2 and geom != 0:
            if (Globals::status->getCurrentCampaignMission() >= 2) {
                AEGeometry *visGeom = (AEGeometry *)(intptr_t)this->subGeometry();
                if (visGeom == nullptr) {
                    visGeom = geom;
                }
                visGeom->setVisible(0);
            }
        }
        // de4e4..de4f4: no target, or target hidden -> stay asleep
        if (this->targetPlayer == nullptr) {
            return;
        }
        if (this->targetPlayer->field_5e != 0) {
            return;
        }
        // de4f8: proximity check: resetVecC components vs field_0x128 (radius)
        {
            float radius = (float)this->field_0x128;
            float rx = this->resetVecC.x;
            float ry = this->resetVecC.y;
            float rz = this->resetVecC.z;
            if (rx >= radius || rx <= -radius) { return; }
            if (ry >= radius || ry <= -radius) { return; }
            if (rz >= radius) { return; }
        }
        // In range: awaken
        this->state = 1;
        AEGeometry *visGeom2 = (AEGeometry *)(intptr_t)this->subGeometry();
        if (visGeom2 == nullptr) visGeom2 = geom;
        visGeom2->setVisible(1);
        ((Player *)(intptr_t)this->player())->setActive(1);
        if (this->field_0x12f != 0) {
            ((Level *)(intptr_t)this->level())->pirateStationAction(1);
        }
        return;
    }

    // -----------------------------------------------------------------------
    // State 6: jump — accelerate until > 100.0, then setDead
    // de590-de5d8
    // -----------------------------------------------------------------------
    case 6: {
        this->currentSpeed *= 1.1f;
        float move = this->currentSpeed * (float)dt;
        int moveInt = (int)move;
        float movef = (float)moveInt;
        geom->moveForward(movef);
        if (this->currentSpeed > 100.0f) {
            this->setDead();
        }
        return;
    }

    // -----------------------------------------------------------------------
    // State 8: docking approach
    // de5da-de8a2
    // -----------------------------------------------------------------------
    case 8: {
        // Get the docking-approach route position
        Route *dRoute = this->KIPlayer::route;

        // Get docking point: route->getPosition() via vtable[10]=getPosition
        Vector routePos;
        {
            KIPlayer *routeKI = (KIPlayer *)(intptr_t)0; // placeholder
            // Actual: call vtable[10] on self = getPosition()
            (void)routeKI;
        }
        // de5e0: r0 = [r4, 0x6c] = KIPlayer::route (navigation route)
        // de5e2-de5ea: call r6 = Route::getDockingTarget getter, result -> sl
        //              Then call r6 again -> r1 = docking route object
        //              vtable[10] with r0=sp+0x140 -> getPosition -> sp+0x140
        //              [r4+0x6c]->geometry->getMatrix -> rotate vector by it
        //              sp+0x198 = sp+0x140 + (rotated docking offset)
        //              self->getNearestDockingPoint(sp+0x198) -> r5 (dock slot idx)
        //              sp+0x140 = getPosition again
        //              sp+0xd8 = MatrixRotateVector([r4+0x6c]->geom->matrix, dock slot)
        //              sp+0x198 = sp+0x140 + sp+0xd8
        //              field_0x9c = sp+0x198
        //              sp+0x140 = getPosition(self)
        //              sp+0x198 = sp+0x140 - field_0x9c
        //              distance = VectorLength(sp+0x198) as int
        //              dockThreshold[type] = ...
        //              if distance > threshold: go to EaseInOutMatrix -> de8a4
        //              else: setExhaustVisible(false), clear field_0x2bc/2c0, state=9

        // Simplified: route via KIPlayer::route, compute approach
        // This is complex enough that I will stub it for now.
        // The approach: if within docking distance, go to state 9
        // Else: EaseInOutMatrix interpolation to approach position
        // For now: structured but incomplete stub that compiles
        if (dRoute != nullptr) {
            // Check if we've reached the docking point
            // (full logic elided — will be filled in a follow-up pass)
        }
        break;
    }

    // -----------------------------------------------------------------------
    // State 9: docked waiting (timer)
    // de714-de84f
    // -----------------------------------------------------------------------
    case 9: {
        // Increment docking timer
        this->field_0x2bc += dt;

        // Mission dock logic (type 0xb8 passenger transport)
        Mission *mission = Globals::status->getMission();
        if (!mission->isEmpty() && mission->getType() == 0xb8) {
            Route *r9 = this->KIPlayer::route;
            if (r9 != nullptr) {
                PlayerFixedObject *dockTarget =
                    (PlayerFixedObject *)r9->getDockingTarget();
                if (dockTarget != nullptr && dockTarget->getDockingType() == 1) {
                    // compute field_0x2c0 = int(float(field_0x2bc) / dockDiv)
                    // dockDiv comes from a pool constant (type-dependent)
                    // (exact constant elided — placeholder 3000.0f matches observed behaviour)
                    int oldC = this->field_0x2c0;
                    float timerF = (float)this->field_0x2bc;
                    float dockDiv = (this->wreckGeometry() != nullptr
                                     && /* type 0x33 */ false) ? 3000.0f : 3000.0f;
                    this->field_0x2c0 = (int)(timerF / dockDiv);

                    // passenger delivery logic
                    if (Globals::status->getCurrentCampaignMission() == 0x5e) {
                        int sv = mission->getStatusValue();
                        int maxPass = Globals::status->getShip()->getMaxPassengers();
                        if (sv >= maxPass) { goto state9_skip_delivery; }
                    }
                    if (this->field_0x2c0 > oldC) {
                        int lvlPass = ((Level *)(intptr_t)this->level())->getNumDeliveredPassengers();
                        int missionAmt = mission->getProductionGoodAmount();
                        if (lvlPass < missionAmt) {
                            int sv2 = mission->getStatusValue();
                            int newSv = sv2 + oldC;
                            int diff = newSv - this->field_0x2c0;
                            mission->setStatusValue(diff);
                            if (mission->getStatusValue() <= -1) {
                                mission->setStatusValue(0);
                            }
                        }
                    }
                }
            }
        }
        state9_skip_delivery:;

        // Check if docking time complete
        {
            Route *r9b = this->KIPlayer::route;
            int dockTime = 0;
            if (r9b != nullptr) {
                dockTime = r9b->getDockingTime();
            }
            if (this->field_0x2bc <= dockTime) {
                return;
            }
        }

        // Docking done: setExhaustVisible, advance route, state=1
        this->setExhaustVisible(true);
        Route *r9c = this->KIPlayer::route;
        if (r9c != nullptr) {
            int cur = r9c->getCurrent();
            r9c->reachWaypoint(cur);
        }
        this->state = 1;

        // Re-enable engine trail if field_0x1fc != 0
        if (this->field_0x1fc != 0) {
            ((Level *)(intptr_t)this->level())->particleEmitBoolPtr->enableSystemEmit(
                this->field_0x80, true);
            ((Level *)(intptr_t)this->level())->particleSystemMgr->enableSystemEmit(
                this->field_0x84, true);
        }

        // Clean up spacePoint
        if (this->spacePoint != 0) {
            SpacePoint *sp = (SpacePoint *)(intptr_t)this->spacePoint;
            sp->giveFree();
            this->spacePoint = 0;
        }
        return;
    }

    default:
        break;
    }

    // -----------------------------------------------------------------------
    // Post-state: boost timer (de840-de9c6)
    // de840: if field_0x1c0 > 5000 and boost not active: maybe start boost
    // de934: if boost active: update speed toward target
    // -----------------------------------------------------------------------
    // de840: only try to start a boost if not already boosting
    if (this->field_0x1c0 > 5000 && this->field_0x1f4 == 0) {
        bool skipBoost = false;
        if (this->field_0x1e0 == 0) {
            int r = Globals::rnd->nextInt(100);
            if (r < this->boostProb) {
                skipBoost = true;
            }
        }
        if (!skipBoost) {
            // de87a: start boost
            int dur = Globals::rnd->nextInt(3000) + 5000;
            this->field_0x1c4 = dur;
            this->field_0x1f4  = 1;
            // strd at de89e: field_0x1ec = 5.5 (speed target), currentRotate = 1.3
            this->field_0x1ec = 5.5f;
            this->currentRotate = 1.3f;
        } else {
            // de8c0: skipBoost path — reset timer
            this->field_0x1c0 = 0;
        }
    }

    // de934: if boost active: update currentSpeed toward target
    if (this->field_0x1f4 != 0) {
        if (this->field_0x1c0 > this->field_0x1c4) {
            // Boost expired: restore pre-boost speed
            this->field_0x1e0 = 0;
            this->field_0x1c0 = 0;
            this->currentSpeed = this->speed;
            this->field_0x1ec = this->speed;
            this->currentRotate = this->rotate;
            this->field_0x1f4  = 0;
        } else {
            // Still boosting: drive speed toward 5.5 target
            float spd    = this->currentSpeed;
            float target = this->field_0x1ec;
            spd *= (spd < target) ? 1.05f : 0.95f;
            if (spd > 5.5f)        { spd = 5.5f; }
            if (spd < this->speed) { spd = this->speed; }
            this->currentSpeed = spd;
            if (spd == target) {
                this->field_0x1ec = 0.0f;
                this->field_0x1f4 = 0;
            }
        }
    }

    // de9c6: post-state movement subsystem
    // Check if commandRoute is valid and not paused
    {
        Player *tgt_b = this->targetPlayer;
        if (tgt_b != nullptr && this->field_0x12c == 0) {
            // de9da: the target's byte at +0x69 widens the follow range when the ego is docked
            bool routeActive = tgt_b->pad_69 != 0;

            // Determine range threshold: 12000 if player is docked, else 8000
            int threshold;
            if (routeActive) {
                Level *lv = (Level *)(intptr_t)this->level();
                PlayerEgo *ego = lv->getPlayer();
                threshold = (ego != nullptr && ego->isDockedToDockingPoint()) ? 12000 : 8000;
            } else {
                threshold = 8000;
            }

            float threshF    = (float)threshold;
            float negThreshF = -threshF;

            // Check resetVecC components vs threshold
            bool inRange = (this->resetVecC.x < threshF && this->resetVecC.x > negThreshF &&
                            this->resetVecC.y < threshF && this->resetVecC.y > negThreshF &&
                            this->resetVecC.z < threshF);
            if (inRange) {
                // Within range: compute normalized heading from resetVecB
                Vector normDir = VectorNormalize(this->resetVecB);
                this->resetVecA() = normDir;
            }
        }
    }

    // -----------------------------------------------------------------------
    // Movement: MatrixSetRotation + translate (de2c0 / dea08 path)
    // -----------------------------------------------------------------------
    // (builds identity matrix, loads field_0x188 angles, rotates geom)
    {
        Matrix identMat;
        identMat.m[0]  = 1.0f; identMat.m[5]  = 1.0f; identMat.m[10] = 1.0f;
        identMat.m[1]  = 0.0f; identMat.m[2]  = 0.0f; identMat.m[3]  = 0.0f;
        identMat.m[4]  = 0.0f; identMat.m[6]  = 0.0f; identMat.m[7]  = 0.0f;
        identMat.m[8]  = 0.0f; identMat.m[9]  = 0.0f; identMat.m[11] = 0.0f;
        identMat.m[12] = 0.0f; identMat.m[13] = 0.0f; identMat.m[14] = 0.0f;
        AEMath::MatrixSetRotation(identMat,
                                  this->field_0x188.x,
                                  this->field_0x188.y,
                                  this->field_0x188.z);
        if (dt >= 1) {
            Matrix geoMat = geom->getMatrix();
            Matrix newMat = geoMat * identMat;
            geom->setMatrix(newMat);
        }

        Vector transl = this->field_0x194 * (float)dt;
        Vector translScaled = transl * this->currentSpeed;
        geom->translate(translScaled);
        geom->updateReferenceMatrix();
        geom->updateReferenceMatrix(); // called twice in asm (dea9e + deaa0)

        // Explosion update
        Level *lv = (Level *)(intptr_t)this->level();
        TargetFollowCamera *cam = nullptr;
        {
            PlayerEgo *ego = lv->getPlayer();
            if (ego != nullptr) {
                cam = (TargetFollowCamera *)(intptr_t)ego->getTargetFollowCamera();
            }
        }
        this->explosion->update(dt, cam);

        // Increment death timer
        this->deathTimer() += dt;

        // de9fc: crate-wreck bomb/rotation block
        if (this->crateCaptured()) {
            Player *deadPl = (Player *)(intptr_t)this->player();
            if (deadPl->isActive() && dt >= 1) {
                AEGeometry *wGeom = this->wreckGeometry();
                if (wGeom != nullptr) {
                    // getBombForce > 0: apply bomb push
                    float bombF = deadPl->getBombForce();
                    if (bombF > 0.0f) {
                        Vector bombDir = this->field_0x17c * bombF;
                        this->resetVecA() = bombDir;
                        wGeom->translate(this->resetVecA());
                        geom->translate(this->resetVecA());

                        // Decay bomb force: *= 0.98, clamp min to 0 if < 0.05
                        float newBF = bombF * 0.98f;
                        if (newBF < 0.05f) { newBF = 0.0f; }
                        deadPl->setBombForce(newBF);
                    }

                    // Wreck rotation: dt>>1 * (1/65536) * 2*pi
                    float angle = (float)(dt >> 1) * (1.0f / 65536.0f) * 6.2831855f;
                    float fangle = (float)(int)angle;
                    wGeom->rotate(fangle, fangle, fangle);

                    // Copy wreck matrix into Player::transform for camera tracking
                    Matrix wMat = wGeom->getMatrix();
                    reinterpret_cast<Matrix &>(deadPl->transform[0]) = wMat;
                }
            }
        }

        // debc4: if death timer < 60001, skip cleanup
        if (this->deathTimer() < 60001) {
            goto death_done;
        }
        // Explosion still playing?
        if (this->explosion->isPlaying()) { goto death_done_return; }

        // Camera target cleanup: if ego's radar target is this ship, clear it
        // debd0: [ego+0x14] = Radar* (field_0x14); [Radar+0x1c] = field_0x1c (KIPlayer*)
        {
            Level *lv2 = (Level *)(intptr_t)this->level();
            PlayerEgo *ego2 = lv2->getPlayer();
            if (ego2 != nullptr) {
                Radar *radar = ego2->field_0x14;
                if (radar != nullptr && radar->field_0x1c == this) {
                    radar->field_0x1c = nullptr;
                }
            }
        }

        // empDisabled check
        {
            Player *cleanPl = (Player *)(intptr_t)this->player();
            if (cleanPl->empDisabledByte != 0 && cleanPl->pad_69 == 0) {
                cleanPl->empDisabledByte = 1;
            }
        }

        // Delete wreck geometry
        delete this->wreckGeometry();
        this->deathTimer() = 0;
        this->wreckGeometry() = nullptr;
        this->setActive(false);
        this->field_0x101 = 1;

        death_done_return:
        return;
        death_done:;
    }

    // -----------------------------------------------------------------------
    // Shooting math + roll samples (ded30-df040)
    // -----------------------------------------------------------------------
    // (will be implemented in follow-up chunk)

    // -----------------------------------------------------------------------
    // Roll subsystem (df042-df110)
    // -----------------------------------------------------------------------
    {
        if (this->rollActive() != 0) {
            this->roll(dt);
        }

        // Smooth targetRoll -> smoothRoll
        float targetRoll = *(float *)&this->targetRoll;
        float smoothRoll = *(float *)&this->smoothRoll;
        if (smoothRoll != targetRoll) {
            float step = (float)dt * 1.25f / 3.9f;
            if (smoothRoll < targetRoll) {
                smoothRoll += step;
                if (smoothRoll > targetRoll) { smoothRoll = targetRoll; }
            } else {
                smoothRoll -= step;
                if (smoothRoll < targetRoll) { smoothRoll = targetRoll; }
            }
            *(float *)&this->smoothRoll = smoothRoll;

            // If still different: reset countdown to 750
            if (smoothRoll != targetRoll) {
                this->field_0xfc = 750;
            } else {
                // countdown roll timer
                if (this->field_0xfc >= 1) {
                    this->field_0xfc -= (uint32_t)dt;
                    if ((int32_t)this->field_0xfc <= 0) {
                        this->rollActive() = 1;
                    }
                }
            }
        }
    }

    // -----------------------------------------------------------------------
    // Camera matrix + particle trail (df110-df260)
    // -----------------------------------------------------------------------
    {
        // df110: compute roll angle = smoothRoll * (1/4096) * pi
        float rollAngle = *(float *)&this->smoothRoll;
        rollAngle *= 0.000244140625f;  // 1/4096
        rollAngle *= 3.14159265f;      // pi
        (void)rollAngle; // used below in MatrixSetRotation if implemented

        // Build identity matrix for roll
        Matrix rollMat;
        rollMat.m[0]  = 1.0f; rollMat.m[1]  = 0.0f; rollMat.m[2]  = 0.0f;
        rollMat.m[3]  = 0.0f; rollMat.m[4]  = 0.0f; rollMat.m[5]  = 1.0f;
        rollMat.m[6]  = 0.0f; rollMat.m[7]  = 0.0f; rollMat.m[8]  = 0.0f;
        rollMat.m[9]  = 0.0f; rollMat.m[10] = 1.0f; rollMat.m[11] = 0.0f;
        rollMat.m[12] = 0.0f; rollMat.m[13] = 0.0f; rollMat.m[14] = 0.0f;

        // Player transform = rollMat * this->rollMatrix (camera view)
        Player *camPl = (Player *)(intptr_t)this->player();
        Matrix &camMat = reinterpret_cast<Matrix &>(camPl->transform[0]);
        camMat = rollMat * this->rollMatrix;

        // if rollActive: apply roll matrix again
        if (this->rollActive()) {
            camMat *= this->rollMatrix;
        }

        // Set PaintCanvas local transform from camMat (via geom->transform node ID)
        {
            AEGeometry *playerGeom = (AEGeometry *)(intptr_t)this->geometry();
            Globals::Canvas->TransformSetLocal(playerGeom->transform, camMat);
        }

        // Copy geom matrix into camMat for camera tracking
        {
            Matrix gm = geom->getMatrix();
            camMat = gm;
        }

        // Enable/disable particle trail based on Player::empDisabledByte
        Level *lv3 = (Level *)(intptr_t)this->level();
        Player *trailPl = (Player *)(intptr_t)this->player();
        bool trailOn = (trailPl->empDisabledByte == 0);
        lv3->field_74->enableSystemEmit(this->field_0x130, trailOn);
        lv3->field_74->enableSystemEmit(this->field_0x134, trailOn);
        lv3->field_74->enableSystemEmit(this->field_0x138, trailOn);

        geom->updateReferenceMatrix();
    }

    // -----------------------------------------------------------------------
    // Landmark collision (df260-df358)
    // -----------------------------------------------------------------------
    {
        if (this->field_0x13e != 0) {
            Level *lv4 = (Level *)(intptr_t)this->level();
            Array<KIPlayer *> *landmarks = lv4->getLandmarks();
            if (landmarks != nullptr) {
                Vector workPos = this->workingPosition;
                unsigned int n_lm = landmarks->count;
                for (unsigned int i_lm = 0; i_lm < n_lm; ++i_lm) {
                    KIPlayer *lm = (*landmarks)[i_lm];
                    if (lm == nullptr) { continue; }
                    if (!lm->collide(workPos.x, workPos.y, workPos.z)) { continue; }
                    Vector normal = lm->getCollisionNormal(workPos);
                    if (normal.x == 0.0f && normal.y == 0.0f && normal.z == 0.0f) { continue; }
                    // Deflect: current dir + (workPos - dir)*speed*0.03, normalize, set
                    Vector dir_lm = geom->getDirection();
                    this->resetVecA() = dir_lm;
                    Vector diff = workPos - this->resetVecA();
                    diff *= this->currentSpeed * 0.03f;
                    Vector newDir = this->resetVecA() + diff;
                    // Store newDir in field_0x9c (reinterpreted as Vector at that offset)
                    reinterpret_cast<Vector &>(this->field_0x9c) = newDir;
                    this->resetVecA() = VectorNormalize(newDir);
                    {
                        Vector up = {0.0f, 1.0f, 0.0f};
                        geom->setDirection(this->resetVecA(), up);
                    }
                    geom->moveForward(this->currentSpeed * (float)dt);
                    break;
                }
            }
        }
    }

    // -----------------------------------------------------------------------
    // Enemy collision (df358-df446)
    // -----------------------------------------------------------------------
    {
        Level *lv5 = (Level *)(intptr_t)this->level();
        Array<KIPlayer *> *enemies_ki = lv5->getEnemies();
        if (enemies_ki != nullptr) {
            Vector workPos2 = this->workingPosition;
            unsigned int n2 = enemies_ki->count;
            for (unsigned int i2 = 0; i2 < n2; ++i2) {
                KIPlayer *en = (*enemies_ki)[i2];
                if (en == nullptr) { continue; }
                if (!en->collide(workPos2.x, workPos2.y, workPos2.z)) { continue; }
                Vector normal2 = en->getCollisionNormal(workPos2);
                if (normal2.x == 0.0f && normal2.y == 0.0f && normal2.z == 0.0f) { continue; }
                Vector dir2 = geom->getDirection();
                this->resetVecA() = dir2;
                Vector diff2 = workPos2 - this->resetVecA();
                diff2 *= this->currentSpeed * 0.03f;
                Vector newDir2 = this->resetVecA() + diff2;
                reinterpret_cast<Vector &>(this->field_0x9c) = newDir2;
                this->resetVecA() = VectorNormalize(newDir2);
                {
                    Vector up2 = {0.0f, 1.0f, 0.0f};
                    geom->setDirection(this->resetVecA(), up2);
                }
                geom->moveForward(this->currentSpeed * (float)dt);
                break;
            }
        }
    }

    // df446: push(dt) via virtual dispatch
    this->push(dt);
    } // end update_body block
}

void PlayerFighter::setPosition(float x, float y, float z) {
    this->posX = x;
    this->posY = y;
    this->posZ = z;

    ((AEGeometry *) (intptr_t) this->geometry())->setPosition(x, y, z);
    this->workingPosition = Vector{x, y, z};
    if (this->trail != 0) {
        this->trail->reset(this->workingPosition);
    }
    reinterpret_cast<AbyssEngine::AEMath::Matrix &>(
            ((Player *) (intptr_t) this->player())->transform[0]) =
            ((AEGeometry *) (intptr_t) this->geometry())->getMatrix();

    return;
}

static const float gRoll_threshold = 0;
static const float gRoll_f18 = 0;
static const float gRoll_f1c = 0;
static const float gRoll_f20 = 0;
static const float gRoll_f24 = 0;
static const float gRoll_f28 = 0;
static const float gRoll_f2c = 0;
static const float gRoll_f30 = 0;
static const float gRoll_f34 = 0;
static const float gRoll_f38 = 0;
static const float gRoll_f3c = 0;
static const float gRoll_f40 = 0;
static const float gRoll_f44 = 0;
static const float gRoll_f48 = 0;

void PlayerFighter::roll(int angle) {
    PlayerFighter * self = this;

    if (self->rollActive() == 0) {
        goto done;
    }

    {
        AbyssEngine::AEMath::Matrix &m =
                ((AEGeometry *) (intptr_t) self->geometry())->getMatrix();
        float fwdX = m.m[4];
        float fwdY = m.m[5];
        float absX = (0.0f < fwdX) ? fwdX : -fwdX;
        if (0x3b < angle) angle = 0x3c;

        if (fwdY >= 0.0f && absX < gRoll_threshold) {
            AbyssEngine::AEMath::MatrixIdentity(self->rollMatrix);
            self->rollActive() = 0;
            self->field_0x254 = 0;
            goto done;
        }

        float bank;
        float roll;
        if (self->field_0x254 != 0) {
            roll = (fwdY == fwdY) ? gRoll_f20 : gRoll_f1c;
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
        self->rollActive() = 1;
        AbyssEngine::AEMath::MatrixSetRotation(self->rollMatrix, bank * fa, 0.0f, roll);
    }

done:
    return;
}

void PlayerFighter::cloak(int dur, bool b) {
    unsigned v;
    if (dur > 0) {
        v = (unsigned) dur;
    } else {
        v = PF_nextInt((int) (intptr_t) Globals::rnd) + 5000;
    }
    this->cloakActive = 1;
    this->cloakDuration = v + 4000;
    this->field_0x2d9 = b;
}

void PlayerFighter::setMissionCrate(bool on) {
    this->isMissionCrate() = on;
    if (on) {
        this->lootList() = nullptr;
        this->lootList() = new Array<int>();
        int mission = (int) (intptr_t) Globals::status->getMission();
        int type = ((Mission *) (mission))->getType();
        int item = (type == 5) ? 0x74 : 0x75;
        ArrayAdd(item, *(this->lootList()));

        ArrayAdd<int>(1, *this->lootList());
    }
}

int PlayerFighter::collide(float x, float y, float z) {
    if ((unsigned) (this->state - 3) > 1) {
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

    ArrayAdd<BoundingVolume *>(bv, *a);
}

void PlayerFighter::setWingmanCommand(int cmd, KIPlayer *target) {
    int saved = this->field_0xe4;
    ((KIPlayer *) (this))->setWingmanCommand(cmd, target);
    if ((unsigned) (cmd - 2) < 2) {
        this->maneuverTimer = 0x1389;
        if (this->currentSpeed != 5.5f) {
            this->field_0x1c0 = 0x1389;
        }
        if (cmd == 2) {
            if (((Level *) (this->level()))->getPlayerRoute() != 0) {
                Route *r = ((Level *) (this->level()))->getPlayerRoute();
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
    if ((unsigned) (this->state - 3) > 1) {
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

static inline void AEMath_VectorScale(AbyssEngine::AEMath::Vector *out, float s,
                                      const AbyssEngine::AEMath::Vector *v) {
    *out = s * *v;
}


static const float gIP_strength = 5000.0f;

void PlayerFighter::initPush(const Vector &target, int radius) {

    AbyssEngine::AEMath::Vector gp = this->getPosition();
    float pos[3] = {gp.x, gp.y, gp.z};

    float diff[3];
    *reinterpret_cast<Vector *>(diff) = target - *reinterpret_cast<const Vector *>(pos);
    float len = AbyssEngine::AEMath::VectorLength(*reinterpret_cast<const AbyssEngine::AEMath::Vector *>(diff));
    float r = VectorSignedToFloat(radius, 0);
    float t = 1.0f;
    if (len / r < 1.0f) t = len / r;
    int strength = (int) ((1.0f - t) * gIP_strength);
    this->pushTimer() = strength;
    this->pushDuration() = strength;

    AbyssEngine::AEMath::Vector gp2 = this->getPosition();
    float pos2[3] = {gp2.x, gp2.y, gp2.z};
    float dir[3];
    *reinterpret_cast<Vector *>(dir) =
            *reinterpret_cast<const Vector *>(pos) - *reinterpret_cast<const Vector *>(pos2);
    float norm[3];
    AbyssEngine::AEMath::VectorNormalize((Vector *) norm, (Vector *) dir);
    this->pushNormal() = *(Vector *) norm;

    int rngObj = (int) (intptr_t) Globals::rnd;
    float rx = VectorSignedToFloat(AERandom_nextIntB(rngObj, 200) - 100, 0);
    float ry = VectorSignedToFloat(AERandom_nextIntB(rngObj, 200) - 100, 0);
    float rz = VectorSignedToFloat(AERandom_nextIntB(rngObj, 200) - 100, 0);
    float rvec[3] = {rx, ry, rz};
    float rnorm[3];
    AbyssEngine::AEMath::VectorNormalize((Vector *) rnorm, (Vector *) rvec);
    float scaled[3];
    AEMath_VectorScale((AbyssEngine::AEMath::Vector *) scaled, (float) strength,
                       (const AbyssEngine::AEMath::Vector *) rnorm);
    this->pushImpulse() = *(Vector *) scaled;

    return;
}


void PlayerFighter::setExhaustVisible(bool vis) {
    int geom = this->geometry();
    if (geom != 0) {
        int sub = this->subGeometry();
        int id;
        if (sub != 0)
            id = (int) ((AEGeometry *) (intptr_t) sub)->childTransform;
        else
            id = (int) ((AEGeometry *) (intptr_t) geom)->childTransform;
        if (id != -1) {
            unsigned t = (unsigned) (unsigned long) Globals::Canvas->TransformGetTransform(id);

            return ((AbyssEngine::Transform *) (unsigned long) t)->SetVisible(vis);
        }
    }
}

static unsigned **const gR_g3 = nullptr;
static unsigned **const gR_g4 = nullptr;
static unsigned **const gR_g5 = nullptr;

void PlayerFighter::render() {
    if (this->wreckGeometry() != 0) {
        ((AEGeometry *) (0))->render();
    }
    int active = ((Player *) (this->player()))->isActive();
    int st = this->state;
    unsigned *idp;
    if (active != 0 && (unsigned) (st - 3) < 2) {
        if (this->explosion != 0) {
            ((Explosion *) (0))->render();
            st = this->state;
        }
        if (st == 4) {
            if (this->field_0x1f8 >= 300) {
                goto done;
            }
            idp = *gR_g4;
        } else {
            if (st != 3) {
                goto done;
            }
            if (this->subGeometry() == 0) {
                return ((AEGeometry *) (intptr_t) this->geometry())->render();
            }
            idp = *gR_g3;
        }
    } else {
        if (active != 0 || st != 5) {
            goto done;
        }
        if (this->subGeometry() == 0) {
            ((AEGeometry *) (0))->render();
        } else {
            idp = *gR_g5;
            unsigned char tmp[60];
            unsigned id = *idp;
            unsigned char *src = (unsigned char *) Globals::Canvas->TransformGetLocal(id);
            memcpy(tmp, src, 0x3c);
            Globals::Canvas->TransformSetLocal(
                *idp, *(const AbyssEngine::AEMath::Matrix *) (intptr_t)((AEGeometry *) (intptr_t) this->subGeometry())->
                transform);
            ((AEGeometry *) (0))->render();
            Globals::Canvas->TransformSetLocal(
                *idp, *(const AbyssEngine::AEMath::Matrix *) (intptr_t)((AEGeometry *) (intptr_t) this->subGeometry())->
                transform);
        }
        if (this->trail != 0) {
            ((AbyssEngine::Trail *) (0))->render();
        }
        goto done;
    }
    {
        unsigned char tmp[60];
        unsigned id = *idp;
        unsigned char *src = (unsigned char *) Globals::Canvas->TransformGetLocal(id);
        memcpy(tmp, src, 0x3c);
        Globals::Canvas->TransformSetLocal(
            *idp, *(const AbyssEngine::AEMath::Matrix *) (intptr_t)((AEGeometry *) (intptr_t) this->subGeometry())->
            transform);
        ((AEGeometry *) (0))->render();
        Globals::Canvas->TransformSetLocal(
            *idp, *(const AbyssEngine::AEMath::Matrix *) (intptr_t)((AEGeometry *) (intptr_t) this->subGeometry())->
            transform);
    }
done:
    ;
}

static const float gPush_div = 5000.0f;

void PlayerFighter::push(int dt) {

    if (0 < this->pushTimer()) {
        int remaining = this->pushTimer() - dt;
        float fr = VectorSignedToFloat(remaining, 0);
        float ftotal = VectorSignedToFloat(this->pushDuration(), 0);
        this->pushTimer() = remaining;
        float frac = fr / ftotal;

        Matrix rot;
        AbyssEngine::AEMath::MatrixSetRotation(rot, frac * this->pushImpulse().z, 0.0f,
                                               frac * this->pushImpulse().y);

        int lo = this->deltaTime;
        int hi = this->deltaTimeHi;
        if ((int) (unsigned) (lo == 0) <= hi) {
            AEGeometry *geom = (AEGeometry *) (intptr_t) this->geometry();
            AbyssEngine::AEMath::Matrix *m = &geom->getMatrix();
            Matrix result = *m * rot;

            geom->setMatrix(result);
            lo = this->deltaTime;
            hi = this->deltaTimeHi;
        }
        float speed = (float) (((long long) hi << 32) | (unsigned) lo);
        AEGeometry *geom = (AEGeometry *) (intptr_t) this->geometry();
        float ftotal2 = VectorSignedToFloat(this->pushDuration(), 0);

        Vector a = this->pushNormal() * speed;
        Vector b = a * this->currentSpeed;
        Vector c = b * ((2.0f - frac) * 3.0f * (ftotal2 / gPush_div));
        geom->translate(c);
    }

    return;
}



void PlayerFighter::reset() {
    ((KIPlayer *) (this))->reset();
    this->crateCaptured() = 1;

    Vector spawn = {this->posX, this->posY, this->posZ};
    this->workingPosition = spawn;
    this->renderPosition() = spawn;

    this->deltaTime = 0;
    this->deltaTimeHi = 0;
    this->field_0x38 = 0;
    this->field_0x12e = 0;
    this->targetPlayer = nullptr;
    this->field_0x12c = 0;
    this->maneuverTimer = 0;
    this->field_0x1c0 = 0;
    this->field_0x1c4 = 0;
    this->field_0x1c8 = 0;
    if (this->state != 5) {
        this->state = 0;
    }

    this->resetVecA() = (Vector){0, 0, 0};
    this->resetVecB = (Vector){0, 0, 0};
    this->resetVecC = (Vector){0, 0, 0};

    this->isMissionCrate() = 0;
    this->missionCrateLost() = 0;
    this->crateLost() = 0;
    this->crateCaptured() = 1;
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

static const float gHC_divIn = 2000.0f;
static const float gHC_divOut = -2000.0f;

void PlayerFighter::handleCloaking() {
    if (this->wingmanCommand() != 10) return;
    if (this->geometry() == 0) return;
    if (((KIPlayer *) (this))->isDead() != 0) return;
    if (((Player *) (intptr_t) this->player())->empDisabledByte != 0) return;
    if (this->cloakingPossible == 0) return;

    if (this->cloakActive != 0) {
        int delta = this->cloakTimer;
        if (delta == 0) {
            unsigned matId = this->cloakMaterial;
            this->field_0x13c = 1;
            if (matId == 0xffffffff) {
                Globals::Canvas->MeshCloneMaterial(
                    ((AEGeometry *) (intptr_t) this->subGeometry())->meshId,
                    this->cloakMaterial);
                AbyssEngine::Mesh *mp = (AbyssEngine::Mesh *) Globals::Canvas->MeshGetPointer(
                    ((AEGeometry *) (intptr_t) this->subGeometry())->meshId);
                matId = this->cloakMaterial;
                this->cloakSavedMode =
                        ((AbyssEngine::Material *) mp->material)->blendMode;
            }
            AbyssEngine::Material *mat =
                    (AbyssEngine::Material *) Globals::Canvas->MaterialGetMaterial(matId);
            mat->blendMode = 0xe;
            Globals::Canvas->MeshChangeMaterial(
                ((AEGeometry *) (intptr_t) this->subGeometry())->meshId,
                this->cloakMaterial);
            if (this->field_0x2d9 == 0) {
                delta = this->cloakTimer;
            } else {
                this->field_0x2d9 = 0;
                delta = 2000;
                this->cloakTimer = 2000;
            }
        }
        long long acc = (long long) delta +
                        (((long long) this->deltaTimeHi << 32) | (unsigned) this->deltaTime);
        int total = (int) acc;
        this->cloakTimer = total;

        if (total - this->deltaTime <= 2000) {
            if (1999 < total) {
                ((PlayerFighter *) (this))->setExhaustVisible(false);
                this->exhaustHidden() = 1;
            }
            int mp = (int) (long) Globals::Canvas->
                    MeshGetPointer(((AEGeometry *) (intptr_t) this->subGeometry())->meshId);
            float a = VectorSignedToFloat(this->cloakTimer, 0) / gHC_divIn;
            Globals::Canvas->MeshChangeShaderAnimValue((AbyssEngine::Mesh *) (intptr_t) mp, a, 1u);
            return;
        } else {
            if (this->cloakDuration < total) {
                int restore = this->cloakSavedMode;
                this->cloakTimer = 0;
                this->cloakActive = 0;
                this->field_0x13c = 0;
                AbyssEngine::Material *mat =
                        (AbyssEngine::Material *) Globals::Canvas->MaterialGetMaterial(this->cloakMaterial);
                mat->blendMode = restore;
                this->setExhaustVisible(1);
                return;
            }
            if (total <= this->cloakDuration - 2000) {
                return;
            }
            this->exhaustHidden() = 0;
            int mp = (int) (long) Globals::Canvas->
                    MeshGetPointer(((AEGeometry *) (intptr_t) this->subGeometry())->meshId);
            float a = VectorSignedToFloat(this->cloakTimer, 0);
            float b = VectorSignedToFloat(this->cloakDuration - 2000, 0);
            float alpha = (a - b) / gHC_divOut + 1.0f;
            Globals::Canvas->MeshChangeShaderAnimValue((AbyssEngine::Mesh *) (intptr_t) mp, alpha, 1u);
            return;
        }
    }

    if (this->field_0x1e0 != 0 && PF_nextInt((int) (intptr_t) Globals::rnd) < 0x32) {
        this->cloak(-1, false);
        return;
    }
    int acc = this->deltaTime + this->cloakCooldown;
    this->cloakCooldown = acc;
    if (8000 < acc) {
        this->cloakCooldown = 0;
        if (PF_nextInt((int) (intptr_t) Globals::rnd) < 0x1e) {
            this->cloak(-1, false);
        }
    }
}

void PlayerFighter::revive() {
    int enemy = ((Player *) (intptr_t) this->player())->turnedEnemyFlag;
    ((Player *) (this->player()))->reset();
    if (enemy != 0) {
        ((Player *) (this->player()))->turnEnemy();
    }
    this->wreckGeometry() = 0;
    this->state = 1;
    this->field_0x12e = 0;
    this->field_0x38 = -1;
    this->routeClone()->reset();
    this->hitpoints = ((Player *) (this->player()))->getHitpoints();
    this->field_0x1dc = 0;
    this->field_0x1e0 = 0;
    this->deathTimer() = 0;
    this->field_0xf0 = 0;
    this->currentSpeed = this->speed;
    this->explosion->reset();
    this->pushTimer() = 0;
    this->field_0x24 = 0;
    this->setExhaustVisible(true);

    int geom = this->subGeometry();
    this->field_0xf5() = 1;
    if (geom == 0) {
        geom = this->geometry();
    }
    ((AEGeometry *) (geom))->setVisible(1);

    if ((unsigned) (this->wingmanCommand() - 9) < 2) {
        delete this->lootList();
        this->lootList() = 0;
    } else {
        Generator *g = new Generator();
        delete this->lootList();
        this->lootList() = 0;
        this->lootList() = g->getLootList(-1, -1);
        delete g;
    }
}

int PlayerFighter::stationRouteAliens;

