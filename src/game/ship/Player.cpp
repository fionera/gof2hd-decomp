#include "game/ship/Player.h"
#include "engine/core/ArrayReleaseClasses.h"
#include "game/ship/Ship.h"
#include "game/mission/Mission.h"
#include "engine/audio/FModSound.h"
#include "game/mission/Item.h"
#include "game/world/Level.h"
#include "game/mission/Achievements.h"
#include "game/ship/KIPlayer.h"
#include "engine/core/GameText.h"
#include "game/ui/Hud.h"
#include "game/world/SolarSystem.h"
#include "game/world/Standing.h"
#include "game/core/Globals.h"
#include "game/weapons/Gun.h"
#include "game/mission/Status.h"
#include "engine/render/Engine.h"
#include "engine/core/ApplicationManager.h"
#include <new>

namespace AbyssEngine {
    namespace AEMath {
        Vector operator-(const Vector &, const Vector &);

        Vector operator*(float, const Vector &);

        float VectorLength(const Vector &);
    }
}

// Weapon sound event per item index (rodata 0x1fbb50 in the original).
static const int kWeaponSounds[233] = {
    52, 53, 54, 56, 55, 57, 58, 59, 60, 50, 49, 50,
    78, 75, 79, 77, 72, 73, 74, 76, 80, 81, 66, 67,
    63, 64, 68, 65, 69, 70, 71, 82, 83, 84, 85, 86,
    87, 88, 89, 90, 91, 6, 7, 8, 9, 10, 11, 92,
    93, 94, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    1098, 1100, 1099, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, 1111, 1113, 1112, 1117,
    1095, 1096, 1097, 1101, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, 1118, -1, -1, -1, 2254, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, 2271, 2272, -1, -1, 2263, 2263, 2263, 2265, 2265,
    2265, -1, -1, -1, -1, 2254, 2267, 2268, 2252, -1, 2269, -1,
    2275, 2276, 2278, 2277, 2280,
};

struct SlaveLink {
    int field_0x0;
    Player *player;
};
#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(SlaveLink, player) == 4, "SlaveLink.player must be at +4");
#endif

struct AudioStateView {
    char pad_0[0xf];
    char soundEnabledFlag;
};
#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(AudioStateView, soundEnabledFlag) == 0xf,
              "AudioStateView.soundEnabledFlag must be at +0xf");
#endif



// lint: void_ptr (external symbol; retyping the param breaks symbol parity)

void Player::pitchAllPrimaryGuns(float pitch) {
    if (this->guns != 0) {
        Array<Gun *> *prim = (*this->guns)[0];
        if (prim != 0) {
            int n = prim->size();
            for (int i = 0; n != i; i++) {
                prim->data()[i]->field_0xb0 = pitch;
            }
        }
    }
}

unsigned char Player::isAlwaysEnemy() {
    return this->alwaysEnemy;
}

void Player::setKIPlayer(KIPlayer *value) {
    this->kiPlayer = value;
}

void Player::damageHull(int damage) {
    if (!this->vulnerable) {
        return;
    }
    if (!this->active) {
        return;
    }
    int armor = this->armorHP;
    if (armor <= 0) {
        int h = this->hitpoints - damage;
        h &= ~(h >> 31);
        this->hitpoints = h;
    } else {
        armor -= damage;
        this->armorHP = armor;
    }
    if (armor <= -1) {
        this->armorHP = 0;
    }
    this->damaged = 1;
    this->updateDamageRate();
}

int Player::getShieldDamageRate() {
    return this->shieldDamageRate;
}

void Player::replaceGuns(int a, int b, int c, int d, int e, bool f) {}

int Player::getShieldHP() {
    return (int) this->shieldHP;
}

void Player::removeAllGuns() {
    if (this->guns != 0) {
        ArrayReleaseClasses(*this->guns);
        delete this->guns;
    }
    this->guns = 0;
}

int Player::getArmorDamageRate() {
    return this->armorDamageRate;
}

int Player::getArmorHP() {
    return this->armorHP;
}

void Player::setRadius(int value) {
    this->radius = value;
}

void Player::resetDamageDoneByPlayer() {
    this->field_dc = 0;
    this->damageDoneByPlayer = 0;
    this->turnedEnemyFlag = 0;
}

KIPlayer *Player::getKIPlayer() {
    return this->kiPlayer;
}

bool Player::isDead() {
    return this->hitpoints < 1;
}

int Player::getRadius() {
    return this->radius;
}

int Player::getEmpDamageRate() {
    return this->empDamageRate;
}

unsigned char Player::isAlwaysFriend() {
    return this->alwaysFriend;
}

int Player::getHitpoints() {
    return this->hitpoints;
}

void Player::damageShield(int damage) {
    if (!this->vulnerable) {
        return;
    }
    if (!this->active) {
        return;
    }
    float s = this->shieldHP;
    if (s <= 0.0f) {
        int h = this->hitpoints - damage;
        h &= ~(h >> 31);
        this->hitpoints = h;
    } else {
        s = s - (float) damage;
        this->shieldHP = s;
    }
    if (s < 0.0f) {
        this->shieldHP = 0;
    }
    this->damaged = 1;
    this->updateDamageRate();
}

void Player::regenerateArmor() {
    int v = this->armorHP + 2;
    if (v > this->maxArmorHP) {
        v = this->maxArmorHP;
    }
    this->armorHP = v;
    this->updateDamageRate();
}

void Player::damageShip(int damage) {
    int v = this->hitpoints - damage;
    v &= ~(v >> 31);
    this->hitpoints = v;
}

int Player::getMaxHitpoints() {
    return this->maxHitpoints;
}

int Player::getGammaHP() {
    return (int) this->gammaHP;
}

float Player::getBombForce() {
    return this->bombForce;
}

int Player::getMaxArmorHP() {
    return this->maxArmorHP;
}

Player *Player::getEnemy(int index) {
    return this->enemies->data()[index];
}

void Player::turnEnemy() {
    this->turnedEnemyFlag = 1;
}

void Player::setEmpData(int points, int data) {
    this->empPoints = points;
    if (this->maxEmpPoints < points) {
        this->maxEmpPoints = points;
    }
    ((Player *) (this))->updateDamageRate();
    this->empData = data;
}

void Player::setVulnerable(bool value) {
    this->vulnerable = value;
}

void Player::setActive(bool value) {
    this->active = value;
}

bool Player::isAsteroid() {
    KIPlayer *ki = this->kiPlayer;
    bool result = false;
    if (ki != 0) {
        result = (uint8_t) ki->stealFlagByte != 0;
    }
    return result;
}

void Player::updateDamageRate() {
    float maxHp = (float) this->maxHitpoints;
    float hp = (float) this->hitpoints;
    float maxArmor = (float) this->maxArmorHP;
    float armor = (float) this->armorHP;
    float maxEmp = (float) this->maxEmpPoints;
    float maxShield = (float) this->maxShieldHP;
    float emp = (float) this->empPoints;

    float shieldRate = (this->shieldHP / maxShield) * 100.0f;
    float armorRate = (armor / maxArmor) * 100.0f;
    float empRate = (emp / maxEmp) * 100.0f;

    this->damageRate = (int) ((hp / maxHp) * 100.0f);
    this->shieldDamageRate = (int) shieldRate;
    this->armorDamageRate = (int) armorRate;
    this->empDamageRate = (int) empRate;
}

void Player::setBombForce(float value) {
    this->bombForce = value;
}

void Player::setMaxHitpoints(int value) {
    this->maxHitpoints = value;
    this->hitpoints = value;
    this->updateDamageRate();
}

int Player::getGunRegenRate(int slot) {
    (void) slot;
    return 0;
}

int Player::getMaxEmpPoints() {
    return this->maxEmpPoints;
}

void Player::regenerateShield(float amount) {
    float f = this->shieldHP + amount;
    float maxF = (float) this->maxShieldHP;
    if (f < maxF) {
        maxF = f;
    }
    this->shieldHP = maxF;
    this->updateDamageRate();
}

unsigned char Player::doesNeverAttack() {
    return this->neverAttack;
}

int Player::getMaxShieldHP() {
    return this->maxShieldHP;
}

unsigned char Player::isDamaged() {
    return this->damaged;
}

unsigned char Player::isActive() {
    return this->active;
}

int Player::getEmpPoints() {
    return this->empPoints;
}

FMOD::Event *Player::GetEngineEvent() {
    return this->engineEvent;
}

void Player::setEmpForce(float value) {
    this->empForce = value;
}

void Player::setShootingEnabled(bool value) {
    this->shootingEnabled = value;
}

int Player::getDamageRate() {
    return this->damageRate;
}

float Player::getEmpForce() {
    return this->empForce;
}

unsigned char Player::turnedEnemy() {
    return this->turnedEnemyFlag;
}

bool Player::gunAvailable(int slot) {
    if ((unsigned) slot < 4) { // ASM: original compares slot unsigned (bhi), not signed (bgt)
        Array<Gun *> *slotArray = this->guns->data()[slot];
        if (slotArray != 0 && slotArray->size() != 0) {
            return *(int *) slotArray->data() != 0;
        }
    }
    return false;
}

int Player::getCombinedHP() {
    return (int) (this->shieldHP + (float) this->armorHP + (float) this->hitpoints);
}

bool Player::isGasCloud() {
    KIPlayer *ki = this->kiPlayer;
    bool result = false;
    if (ki != 0) {
        result = (uint8_t) ki->field_0x44 != 0;
    }
    return result;
}

void Player::setArmorHP(int value) {
    if (this->maxArmorHP < value) {
        value = this->maxArmorHP;
    }
    this->armorHP = value;
    this->updateDamageRate();
}

void Player::setHitpoints(int value) {
    this->hitpoints = value;
    if (this->maxHitpoints < value) {
        this->maxHitpoints = value;
    }
    this->updateDamageRate();
}

void Player::setNeverAttack(bool value) {
    this->neverAttack = value;
}

void Player::setMaxShieldHP(int value) {
    this->maxShieldHP = value;
    this->shieldHP = (float) value;
    this->updateDamageRate();
}

void Player::setMaxEmpPoints(int value) {
    this->empPoints = value;
    this->maxEmpPoints = value;
    this->updateDamageRate();
}

int Player::getGunSlots() {
    return 3;
}

struct HitVec3 {
    double xy;
    float z;
};

Vector Player::getHitVector() {
    Vector out;
    double xy = *(double *) this->hitVector;
    out.z = this->hitVector[2];
    *(double *) &out.x = xy;
    return out;
}

void Player::setPlayShootSound(bool play, int id) {
    this->playShootSoundFlag = play;
    this->playShootSoundId = id;
}

void Player::regenerateShield() {
    float f = this->shieldHP + 1.0f;
    float maxF = (float) this->maxShieldHP;
    if (f < maxF) {
        maxF = f;
    }
    this->shieldHP = maxF;
    this->updateDamageRate();
}

void Player::heal(float amount) {
    float f = this->healAccumulator + amount;
    this->healAccumulator = f;
    if (f > 1.0f) {
        int count = (int) f;
        for (int i = 0; i < count; i++) {
            ((Player *) (this))->regenerateHull();
        }
        this->healAccumulator = this->healAccumulator - (float) count;
    }
}

void Player::setShieldHP(int value) {
    this->shieldHP = (float) value;
    if (this->shieldHP > (float) this->maxShieldHP)
        this->shieldHP = (float) this->maxShieldHP;
    this->updateDamageRate();
}

void Player::refillGunDelay(int slot) {
    if (this->guns != nullptr && (unsigned int) slot < this->guns->size() && slot >= 0
        && (*this->guns)[slot] != nullptr) {
        for (unsigned int i = 0; i < (*this->guns)[slot]->size(); i++) {
            Gun *gun = (*(*this->guns)[slot])[i];
            gun->timer = gun->fireDelay;
        }
    }
}

void Player::addEnemies(Array<Player *> *enemies) {
    if (this->enemies == 0) {
        this->setEnemies(enemies);
        return;
    }
    Array<Player *> *merged = new Array<Player *>();
    for (unsigned int i = 0; i < this->enemies->size(); i++)
        ArrayAdd((*this->enemies)[i], *merged);
    for (unsigned int i = 0; i < enemies->size(); i++)
        ArrayAdd((*enemies)[i], *merged);
    this->setEnemies(merged);
    delete merged;
}

Player::Player(int radius, int hitpoints, int numPrimary, int numSecondary, int numTertiary) {
    Player * self = this;

    self->shieldHP = 0.0f;
    self->armorHP = 0;
    self->maxArmorHP = 0;
    self->maxShieldHP = 0;
    self->hitVector[0] = 0.0f;
    self->hitVector[1] = 0.0f;
    self->hitVector[2] = 0.0f;
    self->lastPosition.y = 0.0f;
    self->lastPosition.z = 0.0f;
    self->radius = radius;
    self->numPrimaryGuns = numPrimary;
    self->numSecondaryGuns = numSecondary;
    self->numTertiaryGuns = numTertiary;
    self->gammaHP = 100.0f;
    self->hitpoints = hitpoints;
    self->empPoints = 0;
    self->maxEmpPoints = 0;
    self->maxHitpoints = hitpoints;
    self->damageDoneByPlayer = 0;
    self->field_5e = 0;
    self->shootingEnabled = 1;
    self->lastPosition.x = 0.0f;
    self->flShake = 0.0f;
    self->bombForce = 0.0f;
    self->empForce = 0.0f;
    ((Player *) (self))->updateDamageRate();
    self->field_58 = -1;

    Array<Array<Gun *> *> *gunArr = new Array<Array<Gun *> *>();
    self->guns = gunArr;
    ArraySetLength(3, *gunArr);

    if (numPrimary >= 1) {
        (*self->guns)[0] = new Array<Gun *>();
        ArraySetLength(numPrimary, *(*self->guns)[0]);
    } else {
        (*self->guns)[0] = nullptr;
    }
    if (numSecondary >= 1) {
        (*self->guns)[1] = new Array<Gun *>();
        ArraySetLength(numSecondary, *(*self->guns)[1]);
    } else {
        (*self->guns)[1] = nullptr;
    }
    if (numTertiary >= 1) {
        (*self->guns)[2] = new Array<Gun *>();
        ArraySetLength(numTertiary, *(*self->guns)[2]);
    } else {
        (*self->guns)[2] = nullptr;
    }

    self->playShootSoundId = 1;
    self->playShootSoundFlag = 1;
    reinterpret_cast<uint16_t &>(self->destroyedByte) = 0;
    self->active = 1;
    self->damaged = 0;
    self->damageTimer = 0;
    self->vulnerable = 1;
    self->kiPlayer = 0;
    self->bombForce = 0.0f;
    reinterpret_cast<uint16_t &>(self->enemyFlagsLo) = 0;
    self->turnedEnemyFlag = 0;
    self->alwaysEnemy = 0;
    self->alwaysFriend = 0;
    self->enemies = 0;
    self->field_54 = 0;
    reinterpret_cast<uint16_t &>(self->empDisabledByte) = 0;
    self->neverAttack = 0;
    self->healAccumulator = 0;
    self->engineEvent = 0;
    self->engineSoundPlaying = 0;
    self->enginePaused = 0;
    self->shieldHit = 0;
    self->armorHit = 0;
    self->hullHit = 0;
    self->gammaHit = 0;

    Vector tmp = AbyssEngine::AEMath::MatrixGetPosition(
        self->transform);
    self->lastPosition = tmp;
    self->engineSoundId = -1;
}

Vector Player::getPosition() {
    Vector out = AbyssEngine::AEMath::MatrixGetPosition(
        this->transform);
    return out;
}

void Player::damageGamma(float amount) {
    if (this->vulnerable && this->active) {
        this->gammaHP = this->gammaHP - amount;
        this->gammaHit = 1;
        if (this->gammaHP <= 0.0f)
            this->gammaHP = 0;
    }
}

void Player::damageEmp(int amount, bool ownFire) {
    if (!this->vulnerable || !this->active || this->empPoints < 1 || this->hitpoints < 1)
        return;

    if (!ownFire) {
        if (this->kiPlayer && !this->alwaysEnemy && (unsigned) (this->kiPlayer->shipGroup - 9) >= 2
            && Globals::status->getSystem() && this->kiPlayer->field_0x42) {
            if (amount > 0)
                this->kiPlayer->level->attackWanted(this->kiPlayer->field_0x48);
        } else if (this->kiPlayer && !this->alwaysEnemy && !this->kiPlayer->isWingMan()
                   && (unsigned) (this->kiPlayer->shipGroup - 9) >= 2 && Globals::status->getSystem()
                   && this->kiPlayer->shipGroup
                          == ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace()) {
            this->field_dc += amount;
            if (this->field_dc > this->maxEmpPoints / 3) {
                this->turnedEnemyFlag = 1;
                this->kiPlayer->level->friendTurnedEnemy(this->kiPlayer->shipGroup);
            }
        }
    }

    this->empPoints -= amount;
    if (this->empPoints <= 0) {
        if (!ownFire && this->kiPlayer) {
            if (!this->alwaysEnemy && (unsigned) (this->kiPlayer->shipGroup - 9) >= 2 && Globals::status->getSystem()
                && this->kiPlayer->shipGroup
                       == ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace())
                this->kiPlayer->level->alarmAllFriends(this->kiPlayer->shipGroup, false);
            if (!this->kiPlayer->stealFlagByte && !this->kiPlayer->countsAsEnemyExcludeFlag
                && !this->kiPlayer->field_0x42)
                ((Standing *) (intptr_t) Globals::status->getStanding())->applyDisable(this->kiPlayer->shipGroup);
            if (this->kiPlayer && !this->empDisabledByte) {
                if (Globals::status->field_134 < 0)
                    Globals::status->field_134 = 0;
                if (!Globals::achievements->hasMedal(42, 1)) {
                    if (++Globals::status->field_134 >= Globals::achievements->getValue(42, 1)) {
                        this->kiPlayer->level->getPlayer()->getHUD()->hudEventMedal(42, 100);
                        Globals::status->field_138 = 1;
                    }
                }
            }
        }
        this->empPoints = 0;
        this->empDisabledByte = 1;
        this->empTimer = 0;
        this->empForce = (float) this->empData;
    }
    this->updateDamageRate();
}

void Player::addGun(Array<Gun *> *gunsIn, int slot) {
    if (this->guns != 0) {
        if ((unsigned int) slot < 4) {
            Array<Gun *> *arr = new Array<Gun *>();
            this->guns->data()[slot] = arr;
            for (unsigned int i = 0; i < gunsIn->size(); i++) {
                ArrayAdd(gunsIn->data()[i], *this->guns->data()[slot]);
            }
        }
        if (this->playShootSoundFlag) {
            this->calcWeaponSounds(this->playShootSoundId);
            return;
        }
    }
}

void Player::setAlwaysEnemy(bool value) {
    reinterpret_cast<uint16_t &>(this->enemyFlagsLo) = 1;
    this->alwaysEnemy = value;
    this->turnedEnemyFlag = 1;
}

void Player::regenerateHull() {
    int v = this->maxHitpoints;
    if (this->hitpoints + 1 < this->maxHitpoints) {
        v = this->hitpoints + 1;
    }
    this->hitpoints = v;
    this->updateDamageRate();
}

Player::~Player() {
    Array<Array<Gun *> *> *guns = this->guns;
    if (guns != 0) {
        for (unsigned int i = 0; i < guns->size(); i++) {
            Array<Gun *> *slot = guns->data()[i];
            if (slot != 0) {
                ArrayReleaseClasses(*slot);
                Array<Gun *> *s2 = this->guns->data()[i];
                if (s2 == 0) {
                    this->guns->data()[i] = 0;
                } else {
                    delete s2;
                    this->guns->data()[i] = 0;
                }
                guns = this->guns;
            }
        }
        ArrayReleaseClasses(*guns);
        delete this->guns;
        this->guns = 0;
    }
    if (this->enemies != 0) {
        delete this->enemies;
    }
    this->enemies = 0;
}

void Player::setGammaHP(int value) {
    float hp = (float) value;
    if (value > 100 && value != 9999999)
        hp = 100.0f;
    this->gammaHP = hp;
    this->updateDamageRate();
}

void Player::stopShootSound(int index, int channel) {
    if (channel == 2 || channel == 3 || channel == 8) {
        if (this->kiPlayer != nullptr && this->kiPlayer->shipGroup == 9)
            Globals::sound->stop(62);
        else
            Globals::sound->stop(kWeaponSounds[index]);
    }
}

void Player::reset() {
    this->gammaHP = 100.0f;
    this->hitpoints = this->maxHitpoints;
    this->empPoints = this->maxEmpPoints;
    this->shieldHP = (float) this->maxShieldHP;
    this->updateDamageRate();
    this->vulnerable = 1;
    this->active = 1;
    this->damaged = 0;
    this->field_54 = 0;
    this->destroyedByte = 0;
    this->damageDoneByPlayer = 0;
    this->field_5e = 0;
    this->damageTimer = 0;
    this->empDisabledByte = 0;
    this->shieldHit = 0;
    this->armorHit = 0;
    this->hullHit = 0;
    this->gammaHit = 0;
    this->bombForce = 0.0f;
    this->empForce = 0.0f;
    this->field_dc = 0;
    this->turnedEnemyFlag = 0;
}

void Player::addGun(Gun *gun, int slot) {
    if (this->guns != 0) {
        if ((unsigned int) slot < 4) {
            Array<Gun *> *arr = new Array<Gun *>();
            this->guns->data()[slot] = arr;
            ArrayAdd(gun, *this->guns->data()[slot]);
        }
        if (this->playShootSoundFlag) {
            this->calcWeaponSounds(this->playShootSoundId);
            return;
        }
    }
}

void Player::calcWeaponSounds(int count) {
    Array<Array<Gun *> *> *guns = this->guns;
    if (guns == 0) {
        return;
    }
    if (guns->data()[0] != 0) {
        unsigned int n = ((Array<Gun *> *) guns->data()[0])->size();
        int *order = new int[n];
        for (int i = 0; i < (int) n; i++) {
            order[i] = this->guns->data()[0]->data()[i]->itemIndex;
        }

        bool sorted = true;
        int i = 1;
        Array<Item *> **itemTablePtr = reinterpret_cast<Array<Item *> **>(&Globals::items);
        do {
            for (; i < (int) n; i++) {
                Item **dataArr = (*itemTablePtr)->data_;
                int a = dataArr[order[i - 1]]->getSinglePrice();
                dataArr = (*itemTablePtr)->data_;
                int b = dataArr[order[i]]->getSinglePrice();
                if (a < b) {
                    sorted = false;
                    int t = order[i - 1];
                    order[i - 1] = order[i];
                    order[i] = t;
                }
            }
            bool again = !sorted;
            sorted = true;
            i = 1;
            if (!again) break;
        } while (true);

        for (unsigned int x = 0; x < n; x++) {
            for (unsigned int y = 0; y != n; y++) {
                if (x != y && order[x] == order[y]) {
                    order[y] = -1;
                }
            }
        }

        int idx = 0;
        do {
            if ((int) n <= idx) break;
            if (order[idx] >= 0) {
                Gun *gun = this->guns->data()[0]->data()[idx];
                gun->field_0x89 = 1;
                Globals::globals->addSoundResourceToList(kWeaponSounds[gun->itemIndex]);
                count--;
            }
            idx++;
        } while (count != 0);

        delete[] order;
        guns = this->guns;
    }

    if (guns->size() > 2) {
        Array<Gun *> *slot2 = guns->data()[2];
        if (slot2 != 0 && slot2->size() != 0) {
            Gun *g = slot2->data()[0];
            if (g != 0) {
                g->field_0x89 = 1;
                Globals::globals->addSoundResourceToList(kWeaponSounds[g->itemIndex]);
                return;
            }
        }
    }
}

void Player::setEnemies(Array<Player *> *enemies) {
    if (this->enemies != 0) {
        delete this->enemies;
    }
    this->enemies = 0;
    if (enemies != 0) {
        Array<Player *> *copy = new Array<Player *>();
        this->enemies = copy;
        ArrayAdd(*enemies, *copy);
    }
    Array<Array<Gun *> *> *guns = this->guns;
    if (guns != 0) {
        for (unsigned int i = 0; i < guns->size(); i++) {
            Array<Gun *> *slot = guns->data()[i];
            if (slot != 0) {
                for (unsigned int j = 0; j < slot->size(); j++) {
                    Gun *gun = slot->data()[j];
                    if (gun != 0) {
                        gun->setEnemies(this->enemies);
                        guns = this->guns;
                        slot = guns->data()[i];
                    }
                }
            }
        }
    }
}

static const int kRaceShootSounds[11] = {52, 55, 54, 53, 61, 61, 61, 61, 61, 62, 2276};

void Player::playShootSound(int type, int channel, Vector *pos, float volume) {
    int soundId = 61;
    if (this->kiPlayer == nullptr)
        soundId = kWeaponSounds[type];
    else if ((unsigned int) this->kiPlayer->shipGroup <= 10)
        soundId = kRaceShootSounds[this->kiPlayer->shipGroup];
    if ((channel == 2 || channel == 3 || channel == 8) && Globals::sound->isPlaying(soundId)) {
        if (Globals::options[15])
            Globals::sound->updateEvent3DAttributes(soundId, pos, nullptr, false);
        return;
    }
    Globals::sound->play(soundId, Globals::options[15] ? pos : nullptr, nullptr, volume);
}

void Player::damage(int amount, bool ownFire, int weaponId) {
    if (!this->vulnerable || !this->active || this->hitpoints < 1)
        return;

    if (!ownFire) {
        if (this->kiPlayer && !this->alwaysEnemy && (unsigned) (this->kiPlayer->shipGroup - 9) >= 2
            && Globals::status->getSystem() && (!this->enemyFlagsLo || this->turnedEnemyFlag)
            && this->kiPlayer->field_0x42) {
            if (amount > 0) {
                this->damageDoneByPlayer += amount;
                this->kiPlayer->level->attackWanted(this->kiPlayer->field_0x48);
            }
        } else if (this->kiPlayer && !this->alwaysEnemy && (unsigned) (this->kiPlayer->shipGroup - 9) >= 2
                   && !this->kiPlayer->isWingMan() && Globals::status->getSystem()
                   && (!this->enemyFlagsLo || this->turnedEnemyFlag)
                   && (this->kiPlayer->shipGroup == ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace()
                       || this->kiPlayer->shipGroup == ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getAttackRace())) {
            this->damageDoneByPlayer += amount;
            bool hardCore = Globals::status->hardCoreMode();
            float turnLimit = hardCore ? 0.1f : 0.33f;
            float hostileLimit = hardCore ? 0.25f : 0.5f;
            float alarmLimit = hardCore ? 0.4f : 0.66f;
            if (turnLimit * (float) this->maxHitpoints < (float) this->damageDoneByPlayer) {
                this->kiPlayer->level->friendTurnedEnemy(this->kiPlayer->shipGroup);
                Ship *ship = Globals::status->getShip();
                Standing *standing = ((Standing *) (intptr_t) Globals::status->getStanding());
                if (ship->getSignatureRace() >= 0 && ship->getSignatureRace() == this->kiPlayer->shipGroup
                    && !this->kiPlayer->field_0x42) {
                    ship->removeEquipment(ship->getFirstEquipmentOfSort(29));
                    ((Standing *) (intptr_t) Globals::status->getStanding())->applyDelict(ship->getSignatureRace(), 100);
                    standing->setPlayerSignatureRace(-1);
                    this->kiPlayer->level->getPlayer()->getHUD()->hudEvent(31, this->kiPlayer->level->getPlayer(), 0);
                }
            }
            if ((float) this->damageDoneByPlayer > hostileLimit * (float) this->maxHitpoints) {
                Ship *ship = Globals::status->getShip();
                Standing *standing = ((Standing *) (intptr_t) Globals::status->getStanding());
                if (ship->getSignatureRace() >= 0 && this->kiPlayer->shipGroup <= 3 && !this->kiPlayer->field_0x42) {
                    ship->removeEquipment(ship->getFirstEquipmentOfSort(29));
                    ((Standing *) (intptr_t) Globals::status->getStanding())->applyDelict(ship->getSignatureRace(), 100);
                    standing->setPlayerSignatureRace(-1);
                    this->kiPlayer->level->getPlayer()->getHUD()->hudEvent(31, this->kiPlayer->level->getPlayer(), 0);
                }
                this->turnedEnemyFlag = 1;
            }
            if ((float) this->damageDoneByPlayer > alarmLimit * (float) this->maxHitpoints)
                this->kiPlayer->level->alarmAllFriends(this->kiPlayer->shipGroup, true);
        } else if (Globals::status->inBlackMarketSystem() && this->kiPlayer && this->kiPlayer->shipGroup == 8) {
            this->turnedEnemyFlag = 1;
            this->kiPlayer->level->alarmAllFriends(8, true);
            Array<KIPlayer *> *enemies = this->kiPlayer->level->getEnemies();
            if (enemies) {
                for (unsigned i = 0; i < enemies->size(); i++) {
                    KIPlayer *enemy = (*enemies)[i];
                    if (enemy->shipGroup == 8)
                        enemy->field_0x25 = 1;
                }
            }
            Globals::status->field_110 = false;
            Globals::status->field_0x111 = true;
        }
    }

    int remaining = (int) this->shieldHP - amount;
    if (remaining < 0) {
        this->shieldHP = 0.0f;
        remaining += this->armorHP;
        if (remaining < 0) {
            this->hullHit = 1;
            this->armorHP = 0;
            this->hitpoints += remaining;
        } else {
            this->armorHP = remaining;
            this->armorHit = 1;
        }
    } else {
        this->shieldHit = 1;
        this->shieldHP = (float) remaining;
    }

    KIPlayer *ki = this->kiPlayer;
    if (ki && !ki->stealFlagByte && !ki->countsAsEnemyExcludeFlag && ki->field_0x42
        && this->hitpoints < this->maxHitpoints / 3)
        ki->level->almostKillWanted(ki->field_0x48);

    if (this->hitpoints <= 0) {
        this->hitpoints = 0;
        if (ownFire) {
            this->destroyedByte = 1;
        } else if (this->kiPlayer && !this->kiPlayer->stealFlagByte && !this->kiPlayer->countsAsEnemyExcludeFlag
                   && !Globals::status->inBlackMarketSystem()) {
            if (!this->kiPlayer->field_0x42)
                ((Standing *) (intptr_t) Globals::status->getStanding())->applyKill(this->kiPlayer->shipGroup);
            Mission *mission = (Mission *) (intptr_t) Globals::status->getCampaignMission();
            int other = this->kiPlayer->name.Compare(*Globals::gameText->getText(1664));
            if (weaponId == 179 && other == 0)
                mission->setStatusValue(mission->getStatusValue() + 1);
            if (this->kiPlayer->field_0x42)
                this->kiPlayer->level->killWanted(this->kiPlayer->field_0x48);
        }
    }

    this->damaged = 1;
    this->flShake += 0.065f;
    this->updateDamageRate();
    if (this->kiPlayer && this->kiPlayer->field_0x10) {
        KIPlayer *linked = (KIPlayer *) (intptr_t) this->kiPlayer->field_0x10;
        linked->player->vulnerable = 1;
        linked->player->damage(amount);
        ((KIPlayer *) (intptr_t) this->kiPlayer->field_0x10)->player->vulnerable = 0;
    }
}


void Player::setEnemy(Player *enemy) {
    Array<Player *> *tmp = new Array<Player *>();
    ArrayAdd(enemy, *tmp);
    ((Player *) (this))->setEnemies(tmp);
    delete tmp;
}

void Player::addEnemy(Player *enemy) {
    if (this->enemies == 0) {
        this->setEnemy(enemy);
        return;
    }
    Array<Player *> *merged = new Array<Player *>();
    if (this->enemies->size() != 0)
        ArrayAdd(*this->enemies, *merged);
    ArrayAdd(enemy, *merged);
    this->setEnemies(merged);
    delete merged;
}

void Player::damage(int amount) {
    this->damage(amount, false, -1);
}

void Player::stopShooting(int slot, int channel) {
    if ((unsigned int) (channel - 22) < 9 && this->guns && (unsigned int) slot < this->guns->size() && slot >= 0 && (*this->guns)[slot]) {
        for (unsigned int i = 0; i < (*this->guns)[slot]->size(); i++) {
            Gun *gun = (*(*this->guns)[slot])[i];
            this->stopShootSound(gun->itemIndex, gun->weaponType);
        }
    }
}

void Player::setAlwaysFriend(bool value) {
    this->alwaysFriend = value;
    this->enemyFlagsLo = 0;
    this->carriesFriendCargoFlag = 1;
    this->turnedEnemyFlag = 0;
}

void Player::setMaxArmorHP(int value) {
    this->armorHP = value;
    this->maxArmorHP = value;
    this->updateDamageRate();
}




void Player::update(int dt, bool doSound) {
    this->damageTimer += dt;
    if (this->damageTimer > 3000) {
        this->damageTimer = 0;
        this->damaged = 0;
    }

    if (this->empDisabledByte) {
        this->empTimer += dt;
        this->empPoints = (int) ((float) this->empTimer / (float) this->empData * (float) this->maxEmpPoints);
        if (this->empPoints > this->maxEmpPoints) {
            this->empPoints = this->maxEmpPoints;
            this->empDisabledByte = 0;
            Globals::status->field_134--;
            this->empTimer = 0;
        }
        this->updateDamageRate();
    }

    if (Globals::options[15] && doSound && this->engineSoundId != -1) {
        const Matrix &transform = reinterpret_cast<const Matrix &>(this->transform[0]);
        Vector velocity = (float) Player::velocity
                          * (AbyssEngine::AEMath::MatrixGetPosition(transform) - this->lastPosition)
                          / (float) dt;
        Vector position = AbyssEngine::AEMath::MatrixGetPosition(transform);
        this->engineEvent = Globals::sound->updateEvent3DAttributes(this->engineEvent, this->engineSoundId,
                                                                   &position, &velocity, false);
        this->engineSoundPlaying = 1;
        this->lastPosition = AbyssEngine::AEMath::MatrixGetPosition(transform);
    } else if (this->engineSoundPlaying) {
        this->StopEngineSound();
    }

    float shake = this->flShake + (float) dt * -0.001f * 0.025f;
    this->flShake = shake < 0.0f ? 0.0f : shake;
}

bool Player::shoot(int slot, int itemIndex, long long target, bool flag) {
    return this->shoot(slot, itemIndex, target, flag, reinterpret_cast<const Matrix &>(this->transform[0]));
}

bool Player::shoot(int slot, int itemIndex, long long target, bool flag, Matrix mat) {
    Matrix transform = mat;
    if (this->guns == nullptr || this->shootingEnabled == 0)
        return true;
    if (!((unsigned int) slot < this->guns->size() && slot >= 0 && (*this->guns)[slot] != nullptr))
        return true;
    int ready = 1;
    for (unsigned int i = 0; i < (*this->guns)[slot]->size(); i++) {
        Gun *gun = (*(*this->guns)[slot])[i];
        if ((gun->weaponType == 6 || gun->weaponType == 7 || gun->weaponType == 34) && gun->lifetimes[0] >= 0) {
            gun->ignite();
        } else if (gun->itemIndex == itemIndex && gun->timer > gun->fireDelay) {
            if (gun->weaponType == 6 || gun->weaponType == 7 || gun->weaponType == 34)
                gun->level->field_69 = 1;
            if (gun->shoot(mat, (int) target, flag)) {
                this->flShake += 0.008f;
                if (this->playShootSoundFlag) {
                    Vector position = AbyssEngine::AEMath::MatrixGetPosition(transform);
                    Gun *fired = (*(*this->guns)[slot])[i];
                    this->playShootSound(fired->itemIndex, fired->weaponType, &position, fired->field_0xb0);
                }
                gun->timer = 0;
                return true;
            }
            if (gun->ammoCount < 1)
                ready = 0;
        }
    }
    return ready;
}

bool Player::shoot(int slot, long long target, bool flag) {
    this->shoot(slot, target, flag, reinterpret_cast<const Matrix &>(this->transform[0]));
    return true;
}

bool Player::shoot(int slot, long long target, bool flag, Matrix mat) {
    Matrix transform = mat;
    if (this->guns != nullptr && this->shootingEnabled != 0 && slot >= 0
        && (unsigned int) slot < this->guns->size() && (*this->guns)[slot] != nullptr) {
        for (unsigned int i = 0; i < (*this->guns)[slot]->size(); i++) {
            Gun *gun = (*(*this->guns)[slot])[i];
            if (gun->timer > gun->fireDelay && gun->shootAt(transform, (int) target, this, flag)) {
                this->flShake += 0.008f;
                Gun *fired = (*(*this->guns)[slot])[i];
                fired->timer = 0;
                if (this->playShootSoundFlag && fired->field_0x89) {
                    Vector position = AbyssEngine::AEMath::MatrixGetPosition(transform);
                    Gun *sounding = (*(*this->guns)[slot])[i];
                    this->playShootSound(sounding->itemIndex, sounding->weaponType, &position,
                                         sounding->field_0xb0);
                }
            }
        }
    }
    return true;
}

void Player::stopShooting(int slot) {
    if (this->guns && (unsigned int) slot < this->guns->size() && slot >= 0 && (*this->guns)[slot]) {
        for (unsigned int i = 0; i < (*this->guns)[slot]->size(); i++) {
            Gun *gun = (*(*this->guns)[slot])[i];
            this->stopShootSound(gun->itemIndex, gun->weaponType);
        }
    }
}

void Player::setHitVector(float x, float y, float z) {
    this->hitVector[0] = x;
    this->hitVector[1] = y;
    this->hitVector[2] = z;
}

void Player::resetGunDelay(int slot) {
    if (this->guns != nullptr && (unsigned int) slot < this->guns->size() && slot >= 0
        && (*this->guns)[slot] != nullptr) {
        for (unsigned int i = 0; i < (*this->guns)[slot]->size(); i++)
            (*(*this->guns)[slot])[i]->timer = 0;
    }
}

Array<Player *> *Player::getEnemies() {
    return this->enemies;
}

void Player::PlayEngineSound(int soundId, Vector *position) {
    this->engineSoundId = soundId;
    if (Globals::options[15]) {
        Vector pos = AbyssEngine::AEMath::MatrixGetPosition(reinterpret_cast<const Matrix &>(this->transform[0]));
        this->engineEvent = Globals::sound->updateEvent3DAttributes(this->engineEvent, this->engineSoundId, &pos,
                                                                   nullptr, true);
        this->engineSoundPlaying = 1;
    }
}

void Player::PauseEngineSound() {
    FMOD::Event *event = this->engineEvent;
    if (event != 0) {
        this->enginePaused = ((FModSound *) Globals::sound)->pause(event);
    }
}

void Player::ResumeEngineSound(bool force) {
    FMOD::Event *event = this->engineEvent;
    if (event != 0 && (this->enginePaused != 0 || force)) {
        this->enginePaused = ((FModSound *) Globals::sound)->resume(event) ^ 1;
    }
}

void Player::StopEngineSound() {
    FMOD::Event *event = this->engineEvent;
    if (event != 0) {
        ((FModSound *) Globals::sound)->stop(event);
        this->engineEvent = 0;
        this->engineSoundPlaying = 0;
    }
}

int Player::velocity;
