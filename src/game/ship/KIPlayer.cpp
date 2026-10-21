#include "game/ship/KIPlayer.h"
#include "game/core/Globals.h"
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

using AbyssEngine::Matrix;



KIPlayer::KIPlayer(int faction, int group, Player *player, AEGeometry *geom,
                   float x, float y, float z, bool grouped) {
    this->tempVector.x = 0;
    this->tempVector.y = 0;
    this->tempVector.z = 0;
    this->auxVector.x = 0;
    this->field_0x10c = 0;
    this->field_0x110 = 0;
    this->field_0x114 = 0;
    this->field_0x118 = 0;
    this->shipPosition.x = 0;
    this->shipPosition.y = 0;
    this->shipPosition.z = 0;
    this->level = 0;
    this->route = 0;
    this->initialRoute = 0;
    this->auxVector.y = 0;
    this->auxVector.z = 0;
    this->field_0x11c = 0;
    this->field_0x120 = 0;
    this->player = player;
    this->geometry = 0;
    this->parentGeometry = 0;
    this->spacePointIds = 0;
    this->field_0xc8 = 0;
    this->spacePoints = 0;

    if (geom != 0 && grouped) {
        this->parentGeometry = geom;
        this->geometry = new AEGeometry(Globals::Canvas);
        this->geometry->addChild(this->parentGeometry->transform);
        this->parentGeometry->altTransform = this->geometry->transform;
    } else {
        this->geometry = geom;
        this->parentGeometry = 0;
    }

    this->shipGroup = group;
    this->cargo = 0;
    this->field_0x72 = 0;
    this->field_0x25 = 0;
    this->field_0x75 = 0;
    this->field_0x42 = 0;
    this->reviveLockFlag = 0;
    this->field_0x44 = 0;
    this->field_0x48 = -1;
    this->field_0x8c = 1;
    reinterpret_cast<int &>(this->stealFlagByte) = 0;
    this->hasCargo = 0;
    this->carriesMissionCrate = 0;
    this->diedWithMissionCrate = 0;
    this->lostMissionCrateToEgo = 0;
    this->field_0x6a = 0;
    this->wingmanFlag = 0;
    this->jumperFlag = 0;
    this->jumpDone = 0;
    this->name = String("");
    this->visibleFlag = 1;
    this->field_0x24 = 0;
    this->wingmanTarget = 0;
    this->field_0x70 = 0;
    this->field_0x71 = 0;
    this->field_0x40 = 0;
    this->deadFlag = 0;
    this->field_0xd8 = 0;
    this->field_0x80 = -1;
    this->field_0x84 = -1;
    this->field_0x104 = 0;
    this->player->setKIPlayer(this);

    this->field_0x10 = 0;
    this->state = 0;
    this->sleepFlag = 0;
    this->initActiveFlag = 1;
    this->field_0xbc = 0;
    this->field_0xc0 = 255;
    this->crateGeometry = 0;
    this->rotationSpeed = 5.0f;
    this->type = faction;
    this->speed = 2.1f;

    if (geom != 0) {
        geom->setPosition(x, y, z);
        this->player->transform = geom->getMatrix();
        if (this->parentGeometry != 0)
            this->player->transform *= this->parentGeometry->getMatrix();
    }

    this->posX = x;
    this->posY = y;
    this->posZ = z;
    this->field_0x14 = 0;
    this->field_0x73 = 0;
    this->field_0x74 = 0;
    this->field_0x76 = 0;
    this->engineSoundEvent = -1;
    this->field_0xfc = 0;
    this->field_0x100 = 0;
    this->field_0x101 = 1;
}

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
        ArrayReleaseClasses(*this->spacePoints);
        delete this->spacePoints;
        this->spacePoints = 0;
    }
}

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

bool KIPlayer::isWingMan() {
    return this->wingmanFlag;
}

void KIPlayer::ResumeEngineSound() {
    if (this->player != 0 && this->engineSoundEvent != -1)
        this->player->ResumeEngineSound(false);
}

void KIPlayer::setWingmanCommand(int cmd, KIPlayer *target) {
    this->field_0xe4 = cmd;
    this->wingmanTarget = target;
}

void KIPlayer::PauseEngineSound() {
    if (this->player != 0 && this->engineSoundEvent != -1)
        this->player->PauseEngineSound();
}

void KIPlayer::setRotationSpeed(float speed) {
    this->rotationSpeed = speed;
}

uint8_t KIPlayer::isEnemy() {
    return (uint8_t) reinterpret_cast<uint16_t &>(this->player->enemyFlagsLo);
}

bool KIPlayer::isDocked() {
    return this->state == 9;
}

bool KIPlayer::isDying() {
    return this->state == 3;
}

void KIPlayer::translate(const Vector &v) {
    this->geometry->translate(v);
    if (this->route != 0)
        this->route->translate(v);
}

Vector KIPlayer::getPosition() {
    return AbyssEngine::AEMath::MatrixGetPosition(this->player->transform);
}

void KIPlayer::setVisible(bool visible) {
    this->visibleFlag = visible;
    if (this->geometry != 0)
        this->geometry->setVisible(this->visibleFlag != 0);
}

void KIPlayer::StopEngineSound() {
    if (this->player != 0 && this->engineSoundEvent != -1)
        this->player->StopEngineSound();
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
        this->player->PlayEngineSound(this->engineSoundEvent, nullptr);
}

void KIPlayer::setRoute(Route *route) {
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

void KIPlayer::setSpacePoints(Array<SpacePoint *> *pts) {
    this->spacePoints = pts;
}

uint8_t KIPlayer::isJumper() {
    return this->jumperFlag;
}

int KIPlayer::collide(float, float, float) {
    return 0;
}

void KIPlayer::update(int) {
}

void KIPlayer::setSpeed(float v) {
    this->speed = v;
}

void KIPlayer::revive() {
}

void KIPlayer::push(int) {
}

void KIPlayer::initPush(const Vector &, int) {
}

int KIPlayer::outerCollide(const Vector &v) {
    return this->outerCollide(v.x, v.y, v.z);
}

int KIPlayer::outerCollide(float, float, float) {
    return 0;
}

void KIPlayer::setJumper(bool b) {
    this->jumperFlag = b;
}

bool KIPlayer::cargoAvailable() {
    Array<int> *arr = this->cargo;
    if (arr != 0) {
        unsigned int len = arr->size();
        for (unsigned int i = 0; i < len; i += 2) {
            if ((*arr)[i + 1] > 0)
                return true;
        }
    }
    return false;
}

void KIPlayer::setDead() {
    this->state = 4;
    this->setActive(false);
}

SpacePoint *KIPlayer::getNearestDockingPoint(const Vector &position) {
    if (this->spacePoints == 0)
        return 0;

    Vector shipPosition = this->getPosition();
    SpacePoint *nearest = 0;
    float nearestDistance = 100000000.0f;
    for (unsigned int i = 0; i < this->spacePoints->size(); ++i) {
        if ((*this->spacePoints)[i]->type != 2)
            continue;
        float distance = AbyssEngine::AEMath::VectorLength(
                shipPosition + AbyssEngine::AEMath::MatrixRotateVector(this->geometry->getMatrix(),
                                                                       *(const Vector *) (*this->spacePoints)[i]) -
                position);
        distance = distance > 0.0f ? distance : -distance;
        if (distance < nearestDistance) {
            nearestDistance = distance;
            nearest = (*this->spacePoints)[i];
        }
    }
    return nearest;
}

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
    if (this->initialRoute != 0)
        this->route = this->initialRoute;
    if (this->route != 0)
        this->route->reset();
    this->field_0xfc = 0;
    this->field_0x100 = 0;
}

void KIPlayer::captureCrate(Hud *hud) {
    if (this->state == 3 || this->state == 4) {
        this->hasCargo = 0;
        if (this->field_0x101)
            this->setActive(false);
    }
    this->crateGeometry = 0;
    if (this->cargo == 0)
        return;

    // cargo holds (item index, amount) pairs; the first non-empty one is picked up.
    for (unsigned int i = 0; i < this->cargo->size(); i += 2) {
        int amount = (*this->cargo)[i + 1];
        if (amount < 1)
            continue;

        if (this->state != 3 && this->state != 4)
            amount = Globals::rnd->nextInt(amount);
        amount = (Globals::status->getShip()->getFreeSpace() > amount ? amount
                                                                       : Globals::status->getShip()->getFreeSpace()) < 1
                         ? 1
                         : (Globals::status->getShip()->getFreeSpace() > amount
                                    ? amount
                                    : Globals::status->getShip()->getFreeSpace());

        Item *item = (*Globals::items)[(*this->cargo)[i]]->makeItem(amount);
        (*this->cargo)[i + 1] -= amount;
        if (item == 0)
            return;

        if (this->player->carriesFriendCargoFlag)
            this->level->stealFriendCargo();
        if (!this->stealFlagByte)
            ((Standing *) (intptr_t) Globals::status->getStanding())->applyStealCargo(this->shipGroup);

        bool missionCrate = this->carriesMissionCrate && (item->getIndex() == 116 || item->getIndex() == 117);

        if (!Globals::status->getShip()->spaceAvailable(item->getAmount())) {
            if (missionCrate)
                this->diedWithMissionCrate = 1;
            hud->catchCargo(item->getIndex(), item->getAmount(), true, false, false, false, false);
            return;
        }

        Globals::status->crateCaptured(item->getAmount());
        if (missionCrate)
            item->setUnsaleable(true);

        bool stacked = false;
        if (item->getType() == 1) {
            Array<Item *> *equipment = Globals::status->getShip()->getEquipment();
            if (equipment != 0) {
                for (unsigned int j = 0; j < equipment->size(); ++j) {
                    if ((*equipment)[j] != 0 && (*equipment)[j]->getIndex() == item->getIndex()) {
                        (*equipment)[j]->changeAmount(item->getAmount());
                        stacked = true;
                    }
                }
            }
        }
        if (!stacked)
            Globals::status->getShip()->addCargo(item);

        this->level->field_1c += item->getAmount();
        if (missionCrate) {
            this->lostMissionCrateToEgo = 1;
        } else if (this->shipGroup == 9) {
            Globals::status->field_cc += item->getAmount();
        } else if (item->getIndex() >= 132 && item->getIndex() <= 153) {
            (*Globals::status->field_ac)[item->getIndex() - 132] = true;
        }
        hud->catchCargo(item->getIndex(), item->getAmount(), false, missionCrate, false, false, false);
        return;
    }
}

SpacePoint *KIPlayer::getNearestNavigationPoint(const Vector &position, SpacePoint *current) {
    if (this->spacePoints == 0)
        return 0;

    Vector shipPosition = this->getPosition();
    SpacePoint *nearest = 0;
    float nearestDistance = 100000000.0f;
    for (unsigned int i = 0; i < this->spacePoints->size(); ++i) {
        if ((*this->spacePoints)[i]->type != 1)
            continue;
        float distance = AbyssEngine::AEMath::VectorLength(
                shipPosition + AbyssEngine::AEMath::MatrixRotateVector(this->geometry->getMatrix(),
                                                                       *(const Vector *) (*this->spacePoints)[i]) -
                position);
        distance = distance > 0.0f ? distance : -distance;
        if (distance < nearestDistance &&
            ((*this->spacePoints)[i]->isFree() || (*this->spacePoints)[i] == current)) {
            nearestDistance = distance;
            nearest = (*this->spacePoints)[i];
        }
    }
    return nearest;
}

void KIPlayer::setEnemies(Array<Player *> *enemies) {
    this->player->setEnemies(enemies);
}

void KIPlayer::setShipGroup(AEGeometry *geom, int group, bool flag) {
    this->shipGroupFlag = group;
    if (geom == 0 || !flag) {
        this->geometry = geom;
        delete this->parentGeometry;
        this->parentGeometry = 0;
    } else {
        AEGeometry *grp = this->geometry;
        this->parentGeometry = geom;
        if (grp == 0) {
            grp = new AEGeometry((PaintCanvas *) Globals::Canvas);
            this->geometry = grp;
        }
        grp->addChild(this->parentGeometry->transform);
        this->parentGeometry->altTransform = this->geometry->transform;
    }
    this->geometry->setPosition(this->posX, this->posY, this->posZ);
    this->player->transform = this->geometry->getMatrix();
    if (this->parentGeometry != 0) {
        this->player->transform *= this->parentGeometry->getMatrix();
    }
}

void KIPlayer::setPosition(const Vector &v) {
    if (this->geometry != 0) {
        this->geometry->setPosition(v);
        this->player->transform = this->geometry->getMatrix();
        if (this->parentGeometry != 0)
            this->player->transform *= this->parentGeometry->getMatrix();
    }
}

// Mesh of the cargo crate a destroyed ship drops: special crate types first, otherwise
// one per faction.
static unsigned short crateMesh(int type, int faction) {
    if (type == 1)
        return 16926;
    if (type == 3)
        return 16920;
    if (type == 2)
        return 16927;
    if (faction == 1)
        return 16991;
    if (faction == 9)
        return 16916;
    if (faction == 3)
        return 16990;
    return faction == 0 ? 16992 : 16993;
}

void KIPlayer::createCrate(int type) {
    this->crateGeometry = new AEGeometry(crateMesh(type, this->shipGroup), Globals::Canvas, false);
    this->crateGeometry->setPosition(this->geometry->getPosition());
    this->player->transform = this->crateGeometry->getMatrix();
    this->player->setKIPlayer(this);
}

Route *KIPlayer::getRoute() {
    return this->route;
}

Array<SpacePoint *> *KIPlayer::getSpacePoints() {
    return this->spacePoints;
}

void KIPlayer::setLevel(Level *lvl) {
    this->level = lvl;
}

Vector KIPlayer::getProjectionVector(const Vector &v) {
    (void) v;
    Vector out;
    out.x = 0.0f;
    out.y = 0.0f;
    out.z = 0.0f;
    return out;
}

Vector KIPlayer::getCollisionNormal(const Vector &position) {
    (void) position;
    Vector out;
    out.x = 0.0f;
    out.y = 0.0f;
    out.z = 0.0f;
    return out;
}

Vector KIPlayer::projectCollisionOnSurface(const Vector &position) {
    (void) position;
    Vector out;
    out.x = 0.0f;
    out.y = 0.0f;
    out.z = 0.0f;
    return out;
}

void KIPlayer::addGun(Gun *gun, int slot) {
    this->player->addGun(gun, slot);
}

void KIPlayer::addGun(Array<Gun *> *guns, int slot) {
    this->player->addGun(guns, slot);
}

int KIPlayer::levelCollision(Vector *out, long long flags) {
    (void) out;
    (void) flags;
    return 0;
}

void KIPlayer::enableExplosion() {
}

void KIPlayer::setInitialRotation(Vector rotation) {
    (void) rotation;
}
