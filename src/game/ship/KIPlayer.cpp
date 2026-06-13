#include "gof2/game/ship/KIPlayer.h"
#include "gof2/engine/core/AERandom.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/ui/Hud.h"
#include "gof2/game/ship/Player.h"
#include "gof2/game/world/Route.h"
#include "gof2/game/world/Standing.h"
#include "gof2/game/core/String.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/game/ship/Ship.h"

extern "C" void FModSound_resumeEvent(void *a, int b);
extern "C" void FModSound_pauseEvent(void *player);
void MatrixGetPosition(Vector *out, float *matrix);
extern "C" void Object_setVisible(void *obj);
extern "C" void FModSound_stopEvent(void *player);
extern "C" void Player_render(void *player);
extern "C" void FModSound_playEvent(void *a, int b, int c);
extern "C" void *Player_dtor(void *p);
extern "C" void Player_addGun_b(void *player);
extern "C" void *gCanvas;
extern "C" void Player_addGun_a(void *player);

// Free math function (defined in libgof2hd; not declared in math.h): lhs *= rhs.
namespace AbyssEngine { namespace AEMath {
    void MatrixMultiply(Matrix&, const Matrix&);
} }
extern "C" void **gCanvasPtr;

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
    if (this->player != 0 && this->engineSoundEvent != -1) {
        return FModSound_resumeEvent(this->player, 0);
    }
}

void KIPlayer::setWingmanCommand(int cmd, KIPlayer *target) {
    this->field_0xe4 = cmd;
    this->wingmanTarget = target;
}

void KIPlayer::PauseEngineSound() {
    void *player = this->player;
    if (player != 0 && this->engineSoundEvent != -1) {
        return FModSound_pauseEvent(player);
    }
}

void KIPlayer::setRotationSpeed(float speed) {
    this->rotationSpeed = speed;
}

uint8_t KIPlayer::isEnemy() {
    return *(uint8_t *)((char *)this->player + 0x5c);
}

bool KIPlayer::isDocked() {
    return this->state == 9;
}

bool KIPlayer::isDying() {
    return this->state == 3;
}

void KIPlayer::translate(const Vector &v) {
    ((AEGeometry *)(this->geometry))->translate(v);
    Route *route = this->route;
    if (route == 0) {
        return;
    }
    return ((Route *)(route))->translate(v);
}

// Returns the ship position by value (sret in r0, this in r1).
Vector KIPlayer::getPosition() {
    Vector v;
    MatrixGetPosition(&v, (float *)((char *)this->player + 0x4));
    return v;
}

void KIPlayer::setVisible(bool visible) {
    void *obj = this->geometry;
    this->visibleFlag = visible;
    if (obj != 0) {
        return Object_setVisible(obj);
    }
}

// Deleting destructor (D0): run the complete dtor, then tail-call operator delete.
void *_ZN8KIPlayerD1Ev(KIPlayer *self); // complete object dtor

void _ZN8KIPlayerD0Ev(KIPlayer *self)
{
    return ::operator delete(_ZN8KIPlayerD1Ev(self));
}

void KIPlayer::StopEngineSound() {
    void *player = this->player;
    if (player != 0 && this->engineSoundEvent != -1) {
        return FModSound_stopEvent(player);
    }
}

void KIPlayer::render() {
    return Player_render(this->geometry);
}

void KIPlayer::setToSleep() {
    void *player = this->player;
    this->state = 5;
    ((Player *)(player))->setActive(0);
    this->sleepFlag = 1;
}

void KIPlayer::setState(int state) {
    this->state = state;
}

void KIPlayer::jump() {
    this->jumpDone = 0;
    this->jumperFlag = 1;
}

void KIPlayer::setActive() {
    return ((Player *)(this->player))->setActive(1);
}

void KIPlayer::PlayEngineSound() {
    if (this->player != 0 && this->engineSoundEvent != -1) {
        return FModSound_playEvent(this->player,
                                   this->engineSoundEvent, 0);
    }
}

void KIPlayer::setRoute(Route *route) {
    Route *old = this->route;
    if (old != 0) {
        ::operator delete(((Route *)(old))->dtor());
    }
    this->route = 0;
    if (route != 0) {
        this->initialRoute = route;
    }
    this->route = route;
}

uint8_t KIPlayer::isVisible() {
    return this->visibleFlag;
}

extern "C" char KIPlayer_vtable[];     // vtable symbol base

// Complete object destructor (D1). Returns this.
void *_ZN8KIPlayerD1Ev(KIPlayer *self)
{
    *(void **)self = KIPlayer_vtable + 8;

    void *p = self->player;
    if (p != 0) ::operator delete(Player_dtor(p));
    self->player = 0;

    Route *r = self->route;
    if (r != 0) ::operator delete(((Route *)(r))->dtor());
    self->route = 0;

    void *g1 = self->crateGeometry;
    if (g1 != 0) { ((AEGeometry *)g1)->~AEGeometry(); ::operator delete(g1); }
    self->crateGeometry = 0;

    delete self->field_0xc4;
    self->field_0xc4 = 0;

    void *g2 = self->geometry;
    if (g2 != 0) { ((AEGeometry *)g2)->~AEGeometry(); ::operator delete(g2); }
    self->geometry = 0;

    void *g3 = self->parentGeometry;
    if (g3 != 0) { ((AEGeometry *)g3)->~AEGeometry(); ::operator delete(g3); }
    self->parentGeometry = 0;

    delete self->cargo;
    self->cargo = 0;

    if (self->spacePoints != 0) {
        for (SpacePoint *e : *self->spacePoints) delete e;
        self->spacePoints->clear();
        delete self->spacePoints;
        self->spacePoints = 0;
    }

    ((String *)((char *)self + 0x18))->dtor();
    return self;
}

void KIPlayer::awake() {
    void *player = this->player;
    this->state = 1;
    return ((Player *)(player))->awake(1);
}

void KIPlayer::setInitActive() {
    this->initActiveFlag = 0;
}

void KIPlayer::setWingman(bool b, int cmd) {
    this->wingmanFlag = b;
    this->wingmanCommand = cmd;
    this->field_0xe4 = 1;
}

void KIPlayer::setSpacePoints(Array<SpacePoint *> *pts) {
    this->spacePoints = pts;
}

uint8_t KIPlayer::isJumper() {
    return this->jumperFlag;
}

typedef void (*CollideFn)(KIPlayer *, float, float, float);
// vtable getPosition(out)/setPosition(Vector const&) thunk signature.
typedef void (*fn_t)(KIPlayer *, Vector *);

void KIPlayer::outerCollide(const Vector &v) {
    CollideFn fn = *(CollideFn *)(*(char **)this + 0x3c);
    return fn(this, v.x, v.y, v.z);
}

void KIPlayer::setJumper(bool b) {
    this->jumperFlag = b;
}

int KIPlayer::cargoAvailable() {
    Array<int> *arr = this->cargo;
    if (arr != 0) {
        unsigned int len = arr->size();
        unsigned int i = 0;
        while (i < len) {
            if ((*arr)[i + 1] > 0) {
                return 1;
            }
            i += 2;
        }
    }
    return 0;
}

void KIPlayer::setDead() {
    this->state = 4;
    return ((Player *)(this))->setDead();
}

// String12 is provided by gof2/common.h.

extern "C" {
extern char KIPlayer_vtable[];                                   // KIPlayer vtable

void String_ctor_default(void *s);                              // 0x6efbc  String::String()
// 0x6ee18  String(char*,bool)
// 0x6f2b0  operator=

// Two engine constants captured from PC-relative slots (init colour/flags).
extern unsigned KIPlayer_initA;
extern unsigned KIPlayer_initB;
}

// KIPlayer::KIPlayer(int faction, int group, Player* player, AEGeometry* geom,
//                    float x, float y, float z, bool active)
void KIPlayer::ctor(int faction, int group, void *player, void *geom, float x, float y, float z, bool active) {
    *(int *)this = (int)(intptr_t)(KIPlayer_vtable + 8);
    String_ctor_default((char *)this + 0x18);

    // zero a wide block of fields (the engine writes zero-vectors via NEON stores).
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
    this->field_0xc4 = 0;
    this->field_0xc8 = 0;
    this->spacePoints = 0;

    // geometry: when a parent geometry is supplied (and it has children) build a child group.
    bool haveChild = (geom != 0) && (*(void **)((char *)geom + 0x18) != 0);
    if (geom != 0 && haveChild) {
        this->parentGeometry = geom;
        void *child = ::operator new(0xc0);
        new ((void *)child) AEGeometry((PaintCanvas *)*(void **)gCanvas);
        this->geometry = child;
        ((AEGeometry *)(child))->addChild(*(int *)((char *)this->parentGeometry + 0xc));
        *(int *)((char *)this->parentGeometry + 0x24) = *(int *)((char *)this->geometry + 0xc);
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
    this->field_0x4c = 0;
    this->field_0xd0 = 0;
    this->field_0x68 = 0;
    this->field_0x6a = 0;
    this->wingmanFlag = 0;
    this->jumperFlag = 0;
    this->jumpDone = 0;

    {
        String12 tmp;
        ((String *)(&tmp))->ctor_char("", false);
        ((String *)((char *)this + 0x18))->assign((String *)&tmp);
        ((String *)(&tmp))->dtor();
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

    ((Player *)(this->player))->setKIPlayer(this);

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
        ((AEGeometry *)(geom))->setPosition((float)x, (float)y, (float)z);
        *(Matrix *)((char *)this->player + 4) = ((AEGeometry *)(geom))->getMatrix();
        if (this->parentGeometry != 0)
            AbyssEngine::AEMath::MatrixMultiply(*(Matrix *)((char *)this->player + 4), ((AEGeometry *)(this->parentGeometry))->getMatrix());
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

// KIPlayer::~KIPlayer() (D2) -- non-deleting destructor. Resets the vtable to the
// KIPlayer base, then releases every owned sub-object: the wrapped Player, the
// active and initial Routes are not freed here (only the active Route at +0x6c),
// the crate/parent/child AEGeometry transforms, the jump-animation handle array
// (+0xc4), the cargo array (+0x50) and the navigation space-point array (+0xcc,
// whose elements are class instances and so are released first). Finally tears
// down the embedded name String at +0x18.
KIPlayer::~KIPlayer() {
    *(int *)this = (int)(intptr_t)(KIPlayer_vtable + 8);

    if (this->player != 0)
        ::operator delete(Player_dtor(this->player));
    this->player = 0;

    if (this->route != 0)
        ::operator delete(((Route *)(this->route))->dtor());
    this->route = 0;

    if (this->crateGeometry != 0) {
        ((AEGeometry *)(this->crateGeometry))->~AEGeometry();
        ::operator delete(this->crateGeometry);
    }
    this->crateGeometry = 0;

    delete this->field_0xc4;
    this->field_0xc4 = 0;

    if (this->geometry != 0) {
        ((AEGeometry *)(this->geometry))->~AEGeometry();
        ::operator delete(this->geometry);
    }
    this->geometry = 0;

    if (this->parentGeometry != 0) {
        ((AEGeometry *)(this->parentGeometry))->~AEGeometry();
        ::operator delete(this->parentGeometry);
    }
    this->parentGeometry = 0;

    delete this->cargo;
    this->cargo = 0;

    if (this->spacePoints != 0) {
        for (SpacePoint *e : *this->spacePoints) delete e;
        this->spacePoints->clear();
        delete this->spacePoints;
        this->spacePoints = 0;
    }

    ((String *)((char *)this + 0x18))->dtor();
}

namespace AbyssEngine { namespace AEMath {
Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector);
float VectorLength(const Vector &value);
Vector operator+(const Vector &lhs, const Vector &rhs);
Vector operator-(const Vector &lhs, const Vector &rhs);
} }

// KIPlayer::getNearestDockingPoint(AbyssEngine::AEMath::Vector const& dir)
//   Returns the nearest space point of type 2 (docking) to the supplied direction vector,
//   measured from the player's transformed position.
void * KIPlayer::getNearestDockingPoint(Vector *dir) {
    Array<SpacePoint *> *arr = this->spacePoints;
    if (arr == 0)
        return 0;

    Vector selfPos;
    {
        void **vtbl = *(void ***)this;
        ((fn_t)vtbl[0x28 / 4])(this, &selfPos);
    }

    void *best = 0;
    float bestLen = 1e30f;
    unsigned count = arr->size();
    for (unsigned i = 0; i < count; i = i + 1) {
        void *pt = (*arr)[i];
        if (*(int *)((char *)pt + 0x18) != 2)
            continue;

        void *mat = (void *)&((AEGeometry *)(this->geometry))->getMatrix();
        Vector rotated = AEMath::MatrixRotateVector(
            *(const AEMath::Matrix *)mat,
            *(const Vector *)(*arr)[i]);
        Vector world = selfPos + rotated;
        Vector delta = world - *dir;
        float len = AEMath::VectorLength(delta);
        float alen = len < 0.0f ? -len : len;
        if (alen < bestLen) {
            best = (*this->spacePoints)[i];
            bestLen = alen;
        }
        count = this->spacePoints->size();
    }
    return best;
}

// KIPlayer::setPosition(float, float, float)
//   Packs the three floats into a Vector and dispatches through vtable slot +0x44
//   (the virtual setPosition(Vector const&)).
void KIPlayer::setPosition3(float x, float y, float z) {
    Vector v;
    v.x = x;
    v.y = y;
    v.z = z;
    void **vtbl = *(void ***)this;
    ((fn_t)vtbl[0x44 / 4])(this, &v);
}

void KIPlayer::addGun_b() {
    return Player_addGun_b(this->player);
}

void KIPlayer::reset() {
    if (this->player != 0) {
        ((Player *)(this->player))->reset();
    }
    if (this->sleepFlag != 0 || this->initActiveFlag == 0) {
        ((KIPlayer *)(this))->setToSleep();
    }
    Route *r = this->initialRoute;
    if (r != 0) {
        this->route = r;
        ((Route *)(r))->reset();
    } else if (this->route != 0) {
        ((Route *)(this->route))->reset();
    }
    this->field_0xfc = 0;
    this->field_0x100 = 0;
}

// PC-relative singleton holders.
extern void *const gAERandom __attribute__((visibility("hidden")));
extern void *const gStatus __attribute__((visibility("hidden")));
extern void *const gItemDb __attribute__((visibility("hidden")));

extern "C" {
void KIPlayer_setActive_732f4(KIPlayer *self, int v);
// 0x72ce8 / 0x72cdc area
}

// KIPlayer::captureCrate(Hud* hud)
//   On a crate capture: rolls/limits the amount against ship free space, creates the item,
//   applies steal/standing penalties, moves the cargo into the ship's hold or equipment, bumps
//   the relevant statistics, and notifies the HUD.
void KIPlayer::captureCrate(void *hud) {
    if ((unsigned)(this->state - 3) < 2) {
        this->field_0x4c = 0;
        if (this->field_0x101 != 0)
            KIPlayer_setActive_732f4(this, 0);
    }

    Array<int> *cargo = this->cargo;
    this->crateGeometry = 0;
    if (cargo == 0)
        return;

    unsigned count = cargo->size();
    for (unsigned i = 0; i < count; i = i + 2) {
        int amount = (*cargo)[i + 1];
        if (amount < 1)
            continue;

        // randomise the captured amount unless we are in a "guaranteed" state.
        if ((unsigned)(this->state - 3) >= 2)
            amount = ((AbyssEngine::AERandom *)(*(void **)gAERandom))->nextInt();

        void *status = *(void **)gStatus;

        // clamp to the ship's free space.
        ((Status *)status)->getShip();
        int free1 = ((Ship *)(((Status *)status)->getShip()))->getFreeSpace();
        int amt = amount;
        if (free1 <= amount)
            amt = ((Ship *)(((Status *)status)->getShip()))->getFreeSpace();
        if (amt < 1) {
            amount = 1;
        } else {
            int free2 = ((Ship *)(((Status *)status)->getShip()))->getFreeSpace();
            if (free2 <= amount)
                amount = ((Ship *)(((Status *)status)->getShip()))->getFreeSpace();
        }

        // resolve the item info and decrement the crate's remaining count.
        int itemId = (*this->cargo)[i];
        int infoPtr = *(int *)(*(char **)(*(char **)gItemDb + 4) + itemId * 4);
        void *item = ((Item *)(infoPtr))->makeItem();
        (*this->cargo)[i + 1] = (*this->cargo)[i + 1] - amount;
        if (item == 0)
            return;

        if (*(char *)((char *)this->player + 0x5d) != 0)
            ((Level *)(this->level))->stealFriendCargo();

        if (this->stealFlag == 0)
            ((Standing *)(intptr_t)(((Status *)status)->getStanding()))->applyStealCargo(this->shipGroup);

        // determine whether this is a special (illegal) cargo item.
        bool special = false;
        if (this->field_0xd0 != 0) {
            int idx = ((Item *)(item))->getIndex();
            if (idx == 0x74)
                special = true;
            else
                special = (((Item *)(item))->getIndex() == 0x75);
        }

        void *ship = ((Status *)status)->getShip();
        int avail = ((Ship *)(ship))->spaceAvailable(((Item *)(item))->getAmount());
        int hudIndex;
        int hudAmount;
        int flagA = special ? 1 : 0;
        int flagB;

        if (avail == 0) {
            if (special)
                this->field_0x68 = 1;
            hudIndex = ((Item *)(item))->getIndex();
            hudAmount = ((Item *)(item))->getAmount();
            flagB = 1;
            ((Hud *)(hud))->catchCargo(hudIndex, hudAmount, flagA, 0, 0, flagB, 0, 0);
            return;
        }

        ((Status *)(status))->crateCaptured(((Item *)(item))->getAmount());
        if (special)
            ((Item *)(item))->setUnsaleable(1);

        bool merged = false;
        if (((Item *)(item))->getType() == 1) {
            void *equip = ((Ship*)(((Status *)status)->getShip()))->getEquipment();
            if (equip != 0) {
                unsigned ecount = *(unsigned *)equip;
                for (unsigned j = 0; j < ecount; j = j + 1) {
                    void *e = ((void **)(*(char **)((char *)equip + 4)))[j];
                    if (e != 0 && ((Item *)(e))->getIndex() == ((Item *)(item))->getIndex()) {
                        void *e2 = ((void **)(*(char **)((char *)equip + 4)))[j];
                        ((Item *)(e2))->changeAmount(((Item *)(item))->getAmount());
                        merged = true;
                    }
                }
            }
        }
        if (!merged)
            ((Ship *)(((Status *)status)->getShip()))->addCargo((Item *)item);

        *(int *)((char *)this->level + 0x1c) =
            ((Item *)(item))->getAmount() + *(int *)((char *)this->level + 0x1c);

        if (special) {
            this->field_0x69 = 1;
        } else if (this->shipGroup == 9) {
            void *st = *(void **)gStatus;
            *(int *)((char *)st + 0xcc) = ((Item *)(item))->getAmount() + *(int *)((char *)st + 0xcc);
        } else {
            int idx = ((Item *)(item))->getIndex();
            if (idx >= 0x84 && ((Item *)(item))->getIndex() < 0x9a) {
                void *st = *(void **)gStatus;
                char *tbl = *(char **)((char *)st + 0xac);
                *(char *)(*(char **)(tbl + 4) + ((Item *)(item))->getIndex() - 0x84) = 1;
            }
        }

        hudIndex = ((Item *)(item))->getIndex();
        hudAmount = ((Item *)(item))->getAmount();
        ((Hud *)(hud))->catchCargo(hudIndex, hudAmount, flagA, 0, 0, 0, 0, 0);
        return;
    }
}

extern "C" {
int SpacePoint_isFree(void *sp);
// virtual getPosition(out) at vtable slot +0x28.
}

// KIPlayer::getNearestNavigationPoint(AbyssEngine::AEMath::Vector const& dir, SpacePoint* target)
//   Among the player's space points of type 1 (navigation), returns the nearest free one
//   (or the explicitly requested `target`) measured against the player's transformed position.
void * KIPlayer::getNearestNavigationPoint(Vector *dir, void *target) {
    Array<SpacePoint *> *arr = this->spacePoints;
    if (arr == 0)
        return 0;

    // this->getPosition(&selfPos) via vtable +0x28.
    Vector selfPos;
    {
        void **vtbl = *(void ***)this;
        ((fn_t)vtbl[0x28 / 4])(this, &selfPos);
    }

    void *best = 0;
    float bestLen = 1e30f;
    unsigned count = arr->size();
    for (unsigned i = 0; i < count; i = i + 1) {
        void *pt = (*arr)[i];
        if (*(int *)((char *)pt + 0x18) != 1)
            continue;

        void *mat = (void *)&((AEGeometry *)(this->geometry))->getMatrix();
        Vector rotated = AEMath::MatrixRotateVector(
            *(const AEMath::Matrix *)mat,
            *(const Vector *)(*arr)[i]);
        Vector world = selfPos + rotated;
        Vector delta = world - *dir;
        float len = AEMath::VectorLength(delta);
        float alen = len < 0.0f ? -len : len;
        if (alen < bestLen) {
            void *cand = (*this->spacePoints)[i];
            if (SpacePoint_isFree(cand) != 0 || cand == target) {
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

void KIPlayer::setEnemies(Array<Player *> *enemies) {
    return ((Player *)(this->player))->setEnemies(enemies);
}

void KIPlayer::setShipGroup(int param2, int flag, int cond) {
    this->shipGroupFlag = flag;
    if (param2 == 0 || cond == 0) {
        this->geometry = (void *)(intptr_t)param2;
        void *g = this->parentGeometry;
        if (g != 0) { ((AEGeometry *)g)->~AEGeometry(); ::operator delete(g); }
        this->parentGeometry = 0;
    } else {
        void *grp = this->geometry;
        this->parentGeometry = (void *)(intptr_t)param2;
        if (grp == 0) {
            grp = ::operator new(0xc0);
            new ((void *)grp) AEGeometry((PaintCanvas *)gCanvas);
            this->geometry = grp;
        }
        ((AEGeometry *)(grp))->addChild(*(int *)((char *)this->parentGeometry + 0xc));
        *(int *)((char *)this->parentGeometry + 0x24) = *(int *)((char *)this->geometry + 0xc);
    }
    ((AEGeometry *)(this->geometry))->setPosition((float)this->posX,
                            (float)this->posY,
                            (float)this->posZ);
    *(Matrix *)((char *)this->player + 0x4) = ((AEGeometry *)(this->geometry))->getMatrix();
    if (this->parentGeometry != 0) {
        AbyssEngine::AEMath::MatrixMultiply(*(Matrix *)((char *)this->player + 0x4), ((AEGeometry *)(this->parentGeometry))->getMatrix());
    }
}

void KIPlayer::setPosition_vec(const Vector &v) {
    void *geom = this->geometry;
    if (geom != 0) {
        ((AEGeometry *)(geom))->setPosition(v);
        void *m = ((AEGeometry *)(this->geometry))->getMatrix();
        *(Matrix *)((char *)this->player + 0x4) = *(const Matrix *)m;
        void *cond = this->parentGeometry;
        if (cond != 0) {
            void *m2 = ((AEGeometry *)(cond))->getMatrix();
            AbyssEngine::AEMath::MatrixMultiply(*(Matrix *)((char *)this->player + 0x4), *(const Matrix *)m2);
        }
    }
}

void KIPlayer::addGun_a() {
    return Player_addGun_a(this->player);
}

void KIPlayer::createCrate(int type) {
    void *crate = ::operator new(0xc0);
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
            id = 0x4261;
            if (st == 0) {
                id = 0x4260;
            }
        }
    }
    new ((void *)crate) AEGeometry((uint16_t)id, (PaintCanvas *)gCanvasPtr[0], false);
    this->crateGeometry = crate;
    Vector pos;
    ((AEGeometry *)(&pos))->getPosition();
    ((AEGeometry *)(crate))->setPosition(*(const AbyssEngine::AEMath::Vector *)&pos);
    *(Matrix *)((char *)this->player + 0x4) = ((AEGeometry *)(crate))->getMatrix();
    ((Player *)(this->player))->setKIPlayer(this);
}

// ---- getRoute (a5b4c) ----
// Accessor for the AI route this player follows.
Route *KIPlayer::getRoute() {
    return this->route;
}

// ---- getSpacePoints (a5b44) ----
// Accessor for the patrol/space-point list (field +0xcc).
Array<SpacePoint *> *KIPlayer::getSpacePoints() {
    return this->spacePoints;
}

// ---- setLevel (base, called by PlayerFighter/PlayerTurret) ----
// Binds this AI object to the active Level it lives in.
void KIPlayer::setLevel(Level *lvl) {
    this->level = lvl;
}

// ---- collision-query virtuals (getProjectionVector b63ca / getCollisionNormal b5244
//      / projectCollisionOnSurface b63d4) ----
// A KIPlayer is not a solid bounding volume, so these overrides ignore the query point
// and hand back a zero vector. (The free KIPlayer_* helpers above fill an out-param;
// these method forms return the same zero vector by value.)
Vector KIPlayer::getProjectionVector(const Vector &v) {
    (void)v;
    Vector out;
    out.x = 0.0f; out.y = 0.0f; out.z = 0.0f;
    return out;
}

Vector KIPlayer::getCollisionNormal(const Vector &position) {
    (void)position;
    Vector out;
    out.x = 0.0f; out.y = 0.0f; out.z = 0.0f;
    return out;
}

Vector KIPlayer::projectCollisionOnSurface(const Vector &position) {
    (void)position;
    Vector out;
    out.x = 0.0f; out.y = 0.0f; out.z = 0.0f;
    return out;
}

// KIPlayer::addGun(Gun*) — delegate the new weapon to the wrapped Player object.
// The target body simply re-dispatches to Player::addGun on this->player; the index
// argument is supplied by the engine-side overload, so we forward through the existing
// Player_addGun_a entry point (the no-extra-argument default slot).
// AbyssEngine::ApplicationManager::GetEngine() — singleton engine accessor.
extern "C" void *AppManager_GetEngine();

void KIPlayer::addGun(Gun *gun) {
    Player_addGun_a(this->player);
}

// KIPlayer::setAutoPilot(KIPlayer *target) — engage/disengage the auto-pilot lock.
// This override lives on the PlayerEgo subclass, so the touched fields sit past the
// KIPlayer base layout; they are addressed by byte offset on the player-ego tail:
//   +0x158 autoPilotActive flag   +0x15c autoPilotTarget   +0x160 followingHostile
//   +0x14  controller (its +0x2c slot is the steering command)
//   +0x2a4/+0x2a8 lock-on highlight pair   +0xbc thrust scale (1.0 when engaged)
void KIPlayer::setAutoPilot(KIPlayer *target) {
    char *ego = reinterpret_cast<char *>(this);

    *(uint8_t *)(ego + 0x160) = 0;
    *(KIPlayer **)(ego + 0x15c) = target;

    bool wasActive = *(uint8_t *)(ego + 0x158) != 0;
    *(uint8_t *)(ego + 0x158) = (target != 0);

    if (target == 0) {
        // Disengage: clear the steering command and the lock-on highlight.
        *(void **)(*(char **)(ego + 0x14) + 0x2c) = 0;
        if (wasActive) {
            *(uint8_t *)(ego + 0x2a8) = 0;
            *(void **)(ego + 0x2a4) = 0;
        }
        return;
    }

    // Engage: mark "following hostile" when the target itself is hostile (+0x72),
    // reset the engine's frame accumulator and apply full forward thrust.
    if (*(char *)(reinterpret_cast<char *>(target) + 0x72) != 0) {
        *(uint8_t *)(ego + 0x160) = 1;
    }
    void *engine = AppManager_GetEngine();
    *(uint32_t *)((char *)engine + 0x360) = 0;
    *(uint32_t *)(ego + 0xbc) = 0x3f800000;   // 1.0f
}
