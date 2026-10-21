#include "game/ship/KIPlayer.h"
#include "engine/core/AERandom.h"
#include "game/mission/Item.h"
#include "game/world/Level.h"
#include "game/mission/Status.h"
#include "game/ui/Hud.h"
#include "game/ship/Player.h"
#include "game/world/Route.h"
#include "game/world/SpacePoint.h"
#include "game/world/Standing.h"
#include "game/core/String.h"
#include "engine/render/AEGeometry.h"
#include "game/ship/Ship.h"




// Engine math free-functions (defined in libgof2hd).
namespace AbyssEngine { namespace AEMath {
void   MatrixGetPosition(Vector* out, const float* matrix);
void   MatrixMultiply(const Matrix& lhs, const Matrix& rhs);
Vector MatrixRotateVector(const Matrix& matrix, const Vector& vector);
float  VectorLength(const Vector& value);
Vector operator+(const Vector& lhs, const Vector& rhs);
Vector operator-(const Vector& lhs, const Vector& rhs);
} }





// FMOD audio event shims.
extern "C" void FModSound_resumeEvent(void* player, int channel);
extern "C" void FModSound_pauseEvent(void* player);
extern "C" void FModSound_stopEvent(void* player);
extern "C" void FModSound_playEvent(void* player, int event, int flags);

// Game singletons and engine constants captured from PC-relative slots.
extern "C" void*  gCanvas;
extern "C" void** gCanvasPtr;
extern void* const gAERandom __attribute__((visibility("hidden")));
extern void* const gItemDb   __attribute__((visibility("hidden")));
extern "C" unsigned KIPlayer_initA;
extern "C" unsigned KIPlayer_initB;

KIPlayer::KIPlayer(int faction, int group, Player* player, AEGeometry* geom,
                   float x, float y, float z, bool active) {
    this->name.ctor();

    this->field_0x90 = 0;  this->field_0x94 = 0;
    this->field_0x98 = 0;  this->field_0x9c = 0;
    this->field_0x10c = 0; this->field_0x110 = 0;
    this->field_0x114 = 0; this->field_0x118 = 0;
    this->field_0x2c = 0;  this->field_0x30 = 0;  this->field_0x34 = 0;
    this->level = 0;  this->route = 0;  this->initialRoute = 0;
    this->field_0xa0 = 0;  this->field_0xa4 = 0;
    this->field_0x11c = 0; this->field_0x120 = 0;

    this->player = player;
    this->geometry = 0;
    this->parentGeometry = 0;
    this->spacePointIds = 0;
    this->field_0xc8 = 0;
    this->spacePoints = 0;

    // When a parent geometry is supplied (and it has a base transform) build a child group.
    bool haveChild = (geom != 0) && (geom->baseTransform != 0);
    if (geom != 0 && haveChild) {
        this->parentGeometry = geom;
        AEGeometry* child = new AEGeometry((PaintCanvas*)gCanvas);
        this->geometry = child;
        child->addChild(this->parentGeometry->transform);
        this->parentGeometry->altTransform = this->geometry->transform;
    } else {
        this->geometry = geom;
        this->parentGeometry = 0;
    }

    this->cargo = 0;
    this->shipGroup = group;
    this->field_0x72 = 0;
    this->field_0x25 = 0;
    this->field_0x75 = 0;
    this->field_0x42 = 0;
    this->field_0x44 = 0;
    this->field_0x48 = -1;
    this->field_0x8c = 1;
    this->stealFlag = 0;
    this->hasCargo = 0;
    this->carriesMissionCrate = 0;
    this->diedWithMissionCrate = 0;
    this->field_0x6a = 0;
    this->wingmanFlag = 0;
    this->jumperFlag = 0;
    this->jumpDone = 0;

    {
        String tmp;
        tmp.ctor_char("", false);
        this->name.assign(&tmp);
        tmp.clear();
    }

    this->field_0x24 = 0;
    this->visibleFlag = 1;
    this->wingmanTarget = 0;
    this->field_0x70 = 0;
    this->field_0x40 = 0;
    this->field_0xd8 = 0;
    this->field_0x80 = -1;
    this->field_0x84 = -1;
    this->field_0x104 = 0;

    this->player->setKIPlayer(this);

    this->state = 0;
    this->field_0x10 = 0;
    this->sleepFlag = 0x100;
    this->field_0xbc = 0;
    this->field_0xc0 = 0xff;
    this->crateGeometry = 0;
    this->rotationSpeed = KIPlayer_initB;
    this->type = faction;
    this->field_0x64 = KIPlayer_initA;

    if (geom != 0) {
        geom->setPosition(x, y, z);
        *(Matrix*)this->player->transform = geom->getMatrix();
        if (this->parentGeometry != 0)
            AbyssEngine::AEMath::MatrixMultiply(*(Matrix*)this->player->transform,
                                                this->parentGeometry->getMatrix());
    }

    this->posX = x;
    this->posY = y;
    this->posZ = z;
    this->field_0x14 = 0;
    this->field_0x73 = 0;
    this->field_0x76 = 0;
    this->engineSoundEvent = -1;
    this->field_0xfc = 0;
    this->field_0x100 = 0x100;
    (void)active;
}

// Releases every owned sub-object: the wrapped Player, the active Route (at +0x6c; the
// initial Route is not freed here), the crate/parent/child AEGeometry transforms, the
// jump-animation handle array, the cargo array and the navigation space-point array
// (whose elements are class instances and are released first), then the embedded name.
KIPlayer::~KIPlayer() {
    delete this->player;
    this->player = 0;

    delete this->route;
    this->route = 0;

    delete this->crateGeometry;
    this->crateGeometry = 0;

    delete this->spacePointIds;
    this->spacePointIds = 0;

    delete this->geometry;
    this->geometry = 0;

    delete this->parentGeometry;
    this->parentGeometry = 0;

    delete this->cargo;
    this->cargo = 0;

    if (this->spacePoints != 0) {
        for (SpacePoint* e : *this->spacePoints) delete e;
        this->spacePoints->clear();
        delete this->spacePoints;
        this->spacePoints = 0;
    }

    this->name.clear();
}

// A bare actor has no propulsion of its own; subclasses report their real speed.
int KIPlayer::getSpeed() {
    return 0;
}

int KIPlayer::getType() {
    return this->type;
}

bool KIPlayer::isDead() {
    return this->state == 4;
}

void KIPlayer::setJumpSphere(uint32_t sphere) {
    this->jumpSphere = sphere;
}

uint8_t KIPlayer::isWingMan() {
    return this->wingmanFlag;
}

void KIPlayer::ResumeEngineSound() {
    if (this->player != 0 && this->engineSoundEvent != -1)
        FModSound_resumeEvent(this->player, 0);
}

void KIPlayer::setWingmanCommand(int cmd, KIPlayer* target) {
    this->field_0xe4 = cmd;
    this->wingmanTarget = target;
}

void KIPlayer::PauseEngineSound() {
    if (this->player != 0 && this->engineSoundEvent != -1)
        FModSound_pauseEvent(this->player);
}

void KIPlayer::setRotationSpeed(float speed) {
    this->rotationSpeed = speed;
}

uint8_t KIPlayer::isEnemy() {
    return (uint8_t)this->player->enemyFlags;
}

bool KIPlayer::isDocked() {
    return this->state == 9;
}

bool KIPlayer::isDying() {
    return this->state == 3;
}

void KIPlayer::translate(const Vector& v) {
    this->geometry->translate(v);
    if (this->route != 0)
        this->route->translate(v);
}

// Returns the ship position by value.
Vector KIPlayer::getPosition() {
    Vector v;
    AbyssEngine::AEMath::MatrixGetPosition(&v, this->player->transform);
    return v;
}

void KIPlayer::setVisible(bool visible) {
    this->visibleFlag = visible;
    if (this->geometry != 0)
        this->geometry->setVisible(this->visibleFlag != 0);
}

void KIPlayer::StopEngineSound() {
    if (this->player != 0 && this->engineSoundEvent != -1)
        FModSound_stopEvent(this->player);
}

void KIPlayer::render() {
    this->geometry->render();
}

void KIPlayer::setToSleep() {
    this->state = 5;
    this->player->setActive(0);
    this->sleepFlag = 1;
}

void KIPlayer::setState(int state) {
    this->state = state;
}

void KIPlayer::jump() {
    this->jumpDone = 0;
    this->jumperFlag = 1;
}

void KIPlayer::setActive(bool active) {
    this->player->setActive(active);
}

void KIPlayer::PlayEngineSound() {
    if (this->player != 0 && this->engineSoundEvent != -1)
        FModSound_playEvent(this->player, this->engineSoundEvent, 0);
}

void KIPlayer::setRoute(Route* route) {
    delete this->route;
    this->route = 0;
    if (route != 0)
        this->initialRoute = route;
    this->route = route;
}

uint8_t KIPlayer::isVisible() {
    return this->visibleFlag;
}

void KIPlayer::awake() {
    this->state = 1;
    this->player->setActive(true);
}

void KIPlayer::setInitActive(bool active) {
    this->setActive(active);
    this->initActiveFlag = 0;
}

void KIPlayer::setWingman(bool b, int cmd) {
    this->wingmanFlag = b;
    this->wingmanCommand = cmd;
    this->field_0xe4 = 1;
}

void KIPlayer::setSpacePoints(Array<SpacePoint*>* pts) {
    this->spacePoints = pts;
}

uint8_t KIPlayer::isJumper() {
    return this->jumperFlag;
}

// slot +0x38: a bare actor has no collision shape of its own; subclasses override.
int KIPlayer::collide(float, float, float) {
    return 0;
}

// slot +0x34: a bare actor has no per-tick behaviour; subclasses override.
void KIPlayer::update(int) {
}

// slot +0x1c: cache the requested speed (binary stores it at +0x64).
void KIPlayer::setSpeed(float v) {
    *(float*)&this->field_0x64 = v;
}

// slot +0x18: base actor has no revive behaviour; subclasses override.
void KIPlayer::revive() {
}

// slot +0x30: base actor has no push behaviour; subclasses override.
void KIPlayer::push(int) {
}

// slot +0x2c: a bare actor takes no bomb/explosion impulse; subclasses override.
void KIPlayer::initPush(const Vector&, int) {
}

// slot +0x40: unpack the vector and dispatch to the polymorphic float form (slot +0x3c).
int KIPlayer::outerCollide(const Vector& v) {
    return this->outerCollide(v.x, v.y, v.z);
}

// slot +0x3c: a bare actor has no outer surface of its own; subclasses override.
int KIPlayer::outerCollide(float, float, float) {
    return 0;
}

void KIPlayer::setJumper(bool b) {
    this->jumperFlag = b;
}

int KIPlayer::cargoAvailable() {
    Array<int>* arr = this->cargo;
    if (arr != 0) {
        unsigned int len = arr->size();
        for (unsigned int i = 0; i < len; i += 2) {
            if ((*arr)[i + 1] > 0)
                return 1;
        }
    }
    return 0;
}

void KIPlayer::setDead() {
    this->state = 4;
    this->player->setActive(false);
}

// Returns the nearest space point of type 2 (docking) to the supplied direction vector,
// measured from the player's transformed position.
SpacePoint* KIPlayer::getNearestDockingPoint(const Vector& dir) {
    Array<SpacePoint*>* arr = this->spacePoints;
    if (arr == 0)
        return 0;

    Vector selfPos = this->getPosition();

    SpacePoint* best = 0;
    float bestLen = 1e30f;
    unsigned count = arr->size();
    for (unsigned i = 0; i < count; ++i) {
        SpacePoint* pt = (*arr)[i];
        if (pt->type != 2)
            continue;

        Vector rotated = MatrixRotateVector(this->geometry->getMatrix(),
                                            *(const Vector*)(*arr)[i]);
        Vector world = selfPos + rotated;
        Vector delta = world - dir;
        float len = VectorLength(delta);
        float alen = len < 0.0f ? -len : len;
        if (alen < bestLen) {
            best = (*this->spacePoints)[i];
            bestLen = alen;
        }
        count = this->spacePoints->size();
    }
    return best;
}

// Packs the three floats into a Vector and forwards to setPosition(Vector const&).
void KIPlayer::setPosition(float x, float y, float z) {
    Vector v;
    v.x = x;
    v.y = y;
    v.z = z;
    this->setPosition(v);
}

void KIPlayer::reset() {
    if (this->player != 0)
        this->player->reset();
    if (this->sleepFlag != 0 || this->initActiveFlag == 0)
        this->setToSleep();
    Route* r = this->initialRoute;
    if (r != 0) {
        this->route = r;
        r->reset();
    } else if (this->route != 0) {
        this->route->reset();
    }
    this->field_0xfc = 0;
    this->field_0x100 = 0;
}

// On a crate capture: rolls/limits the amount against ship free space, creates the item,
// applies steal/standing penalties, moves the cargo into the ship's hold or equipment, bumps
// the relevant statistics, and notifies the HUD.
void KIPlayer::captureCrate(Hud* hud) {
    if ((unsigned)(this->state - 3) < 2) {
        this->hasCargo = 0;
        if (this->field_0x101 != 0)
            this->setToSleep();
    }

    Array<int>* cargo = this->cargo;
    this->crateGeometry = 0;
    if (cargo == 0)
        return;

    unsigned count = cargo->size();
    for (unsigned i = 0; i < count; i += 2) {
        int amount = (*cargo)[i + 1];
        if (amount < 1)
            continue;

        // Randomise the captured amount unless we are in a "guaranteed" state.
        if ((unsigned)(this->state - 3) >= 2)
            amount = ((AbyssEngine::AERandom*)(*(void**)gAERandom))->nextInt();

        Status* status = gStatus;

        // Clamp to the ship's free space.
        int free1 = status->getShip()->getFreeSpace();
        int amt = amount;
        if (free1 <= amount)
            amt = status->getShip()->getFreeSpace();
        if (amt < 1) {
            amount = 1;
        } else {
            int free2 = status->getShip()->getFreeSpace();
            if (free2 <= amount)
                amount = status->getShip()->getFreeSpace();
        }

        // Resolve the item info and decrement the crate's remaining count.
        int itemId = (*this->cargo)[i];
        // RAWREAD: gItemDb internal table (untyped ItemDb handle; no modeled class/layout)
        int infoPtr = *(int*)(*(char**)(*(char**)gItemDb + 4) + itemId * 4);
        Item* item = ((Item*)(intptr_t)infoPtr)->makeItem();
        (*this->cargo)[i + 1] = (*this->cargo)[i + 1] - amount;
        if (item == 0)
            return;

        // RAWREAD: player+0x5d (high byte of Player::enemyFlags @ +0x5c; no member at exactly +0x5d)
        if (*((char*)this->player + 0x5d) != 0)
            this->level->stealFriendCargo();

        if (this->stealFlag == 0)
            ((Standing*)(intptr_t)status->getStanding())->applyStealCargo(this->shipGroup);

        // Determine whether this is a special (illegal) cargo item.
        bool special = false;
        if (this->carriesMissionCrate != 0) {
            int idx = item->getIndex();
            special = (idx == 0x74) || (idx == 0x75);
        }

        Ship* ship = status->getShip();
        int avail = ship->spaceAvailable(item->getAmount());
        int flagA = special ? 1 : 0;

        if (avail == 0) {
            if (special)
                this->diedWithMissionCrate = 1;
            hud->catchCargo(item->getIndex(), item->getAmount(), flagA, 0, 0, 1, 0);
            return;
        }

        status->crateCaptured(item->getAmount());
        if (special)
            item->setUnsaleable(1);

        bool merged = false;
        if (item->getType() == 1) {
            Array<Item*>* equip = status->getShip()->getEquipment();
            if (equip != 0) {
                unsigned ecount = equip->size();
                for (unsigned j = 0; j < ecount; ++j) {
                    Item* e = (*equip)[j];
                    if (e != 0 && e->getIndex() == item->getIndex()) {
                        e->changeAmount(item->getAmount());
                        merged = true;
                    }
                }
            }
        }
        if (!merged)
            status->getShip()->addCargo(item);

        this->level->field_1c = item->getAmount() + this->level->field_1c;

        if (special) {
            this->lostMissionCrateToEgo = 1;
        } else if (this->shipGroup == 9) {
            Status* st = gStatus;
            st->field_cc = item->getAmount() + st->field_cc;
        } else {
            int idx = item->getIndex();
            if (idx >= 0x84 && idx < 0x9a) {
                Status* st = gStatus;
                (*st->field_ac)[idx - 0x84] = true;
            }
        }

        hud->catchCargo(item->getIndex(), item->getAmount(), flagA, 0, 0, 0, 0);
        return;
    }
}

// Among the player's space points of type 1 (navigation), returns the nearest free one
// (or the explicitly requested `target`) measured against the player's transformed position.
SpacePoint* KIPlayer::getNearestNavigationPoint(const Vector& dir, SpacePoint* target) {
    Array<SpacePoint*>* arr = this->spacePoints;
    if (arr == 0)
        return 0;

    Vector selfPos = this->getPosition();

    SpacePoint* best = 0;
    float bestLen = 1e30f;
    unsigned count = arr->size();
    for (unsigned i = 0; i < count; ++i) {
        SpacePoint* pt = (*arr)[i];
        if (pt->type != 1)
            continue;

        Vector rotated = MatrixRotateVector(this->geometry->getMatrix(),
                                            *(const Vector*)(*arr)[i]);
        Vector world = selfPos + rotated;
        Vector delta = world - dir;
        float len = VectorLength(delta);
        float alen = len < 0.0f ? -len : len;
        if (alen < bestLen) {
            SpacePoint* cand = (*this->spacePoints)[i];
            if (cand->free != 0 || cand == target) {
                best = cand;
                bestLen = alen;
            } else {
                best = target;
                bestLen = alen;
            }
        }
        count = this->spacePoints->size();
    }
    return best;
}

void KIPlayer::setEnemies(Array<Player*>* enemies) {
    this->player->setEnemies(enemies);
}

void KIPlayer::setShipGroup(AEGeometry* geom, int group, bool flag) {
    this->shipGroupFlag = group;
    if (geom == 0 || !flag) {
        this->geometry = geom;
        delete this->parentGeometry;
        this->parentGeometry = 0;
    } else {
        AEGeometry* grp = this->geometry;
        this->parentGeometry = geom;
        if (grp == 0) {
            grp = new AEGeometry((PaintCanvas*)gCanvas);
            this->geometry = grp;
        }
        grp->addChild(this->parentGeometry->transform);
        this->parentGeometry->altTransform = this->geometry->transform;
    }
    this->geometry->setPosition(this->posX, this->posY, this->posZ);
    *(Matrix*)this->player->transform = this->geometry->getMatrix();
    if (this->parentGeometry != 0)
        AbyssEngine::AEMath::MatrixMultiply(*(Matrix*)this->player->transform,
                                            this->parentGeometry->getMatrix());
}

void KIPlayer::setPosition(const Vector& v) {
    if (this->geometry != 0) {
        this->geometry->setPosition(v);
        *(Matrix*)this->player->transform = this->geometry->getMatrix();
        if (this->parentGeometry != 0)
            AbyssEngine::AEMath::MatrixMultiply(*(Matrix*)this->player->transform,
                                                this->parentGeometry->getMatrix());
    }
}

void KIPlayer::createCrate(int type) {
    unsigned short id;
    if (type == 1) {
        id = 0x421e;
    } else if (type == 2) {
        id = 0x421f;
    } else if (type == 3) {
        id = 0x4218;
    } else {
        int st = this->shipGroup;
        if (st == 1) {
            id = 0x425f;
        } else if (st == 3) {
            id = 0x425e;
        } else if (st == 9) {
            id = 0x4214;
        } else {
            id = (st == 0) ? 0x4260 : 0x4261;
        }
    }
    AEGeometry* crate = new AEGeometry((uint16_t)id, (PaintCanvas*)gCanvasPtr[0], false);
    this->crateGeometry = crate;
    Vector pos = crate->getPosition();
    crate->setPosition(pos);
    *(Matrix*)this->player->transform = crate->getMatrix();
    this->player->setKIPlayer(this);
}

// Accessor for the AI route this player follows.
Route* KIPlayer::getRoute() {
    return this->route;
}

// Accessor for the patrol/space-point list.
Array<SpacePoint*>* KIPlayer::getSpacePoints() {
    return this->spacePoints;
}

// Binds this AI object to the active Level it lives in.
void KIPlayer::setLevel(Level* lvl) {
    this->level = lvl;
}

// A KIPlayer is not a solid bounding volume, so these overrides ignore the query point
// and hand back a zero vector.
Vector KIPlayer::getProjectionVector(const Vector& v) {
    (void)v;
    Vector out;
    out.x = 0.0f; out.y = 0.0f; out.z = 0.0f;
    return out;
}

Vector KIPlayer::getCollisionNormal(const Vector& position) {
    (void)position;
    Vector out;
    out.x = 0.0f; out.y = 0.0f; out.z = 0.0f;
    return out;
}

Vector KIPlayer::projectCollisionOnSurface(const Vector& position) {
    (void)position;
    Vector out;
    out.x = 0.0f; out.y = 0.0f; out.z = 0.0f;
    return out;
}

// Forward a single gun into the given weapon slot of the wrapped Player.
void KIPlayer::addGun(Gun* gun, int slot) {
    this->player->addGun(gun, slot);
}

// Forward a whole gun list into the given weapon slot of the wrapped Player.
void KIPlayer::addGun(Array<Gun*>* guns, int slot) {
    this->player->addGun(guns, slot);
}

// A bare actor has no collision surface to test against the level, so the query
// never produces a hit.
int KIPlayer::levelCollision(Vector* out, long long flags) {
    (void)out;
    (void)flags;
    return 0;
}

// Base actor has no explosion model; subclasses arm their own.
void KIPlayer::enableExplosion() {
}

// Base actor keeps no spawn-orientation state; subclasses that need it override.
void KIPlayer::setInitialRotation(Vector rotation) {
    (void)rotation;
}
