#include "gof2/game/ship/Player.h"
#include "gof2/game/ship/Ship.h"
#include "gof2/game/mission/Mission.h"
#include "gof2/externs.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/mission/Achievements.h"
#include "gof2/game/ship/KIPlayer.h"
#include "gof2/engine/core/GameText.h"
#include "gof2/game/ui/Hud.h"
#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/game/world/SolarSystem.h"
#include "gof2/game/world/Standing.h"
#include "gof2/game/core/Globals.h"
#include "gof2/game/weapons/Gun.h"
#include "gof2/game/mission/Status.h"

// Free math operators/functions (defined in libgof2hd; not declared in math.h).
namespace AbyssEngine { namespace AEMath {
    Vector operator-(const Vector&, const Vector&);
    Vector operator*(float, const Vector&);
    float VectorLength(const Vector&);
} }

// Status singleton holder (Status** at 0xe4c5c). Dropped-self Status_*() calls in
// the decompiler are method calls on this global instance.
__attribute__((visibility("hidden"))) extern Status **gStatus;

extern "C" void Player_damageHull_tail();
extern "C" void Player_removeAllGuns_releaseClasses(void *array);
extern "C" void *Player_removeAllGuns_dtor(void *array);
extern "C" void Player_removeAllGuns_delete(void *p);
extern "C" void Player_damageShield_tail();
extern "C" void Player_regenerateArmor_tail();
extern "C" void Player_setMaxHitpoints_tail();
extern "C" void Player_regenerateShield_tail();
extern "C" void Player_setArmorHP_tail();
extern "C" void Player_setHitpoints_tail();
extern "C" void Player_setMaxShieldHP_tail();
extern "C" void Player_setMaxEmpPoints_tail();
extern "C" void Player_setShieldHP_tail();
extern "C" void Array_Player_ctor(Array<Player *> *array);
extern "C" void ArrayAdd_Player(Player *value, Array<Player *> *array);
extern "C" void *Array_Player_dtor(Array<Player *> *array);
extern "C" void Player_operator_delete_tail(void *p);
extern "C" void Player_setEnemies_tail(Player *self, Array<Player *> *enemies);
extern "C" void Array_GunArrayArray_ctor(Array<Array<Gun *> *> *array);
extern "C" void Array_Gun_ctor(Array<Gun *> *array);
extern "C" void ArraySetLength_GunArrayArray(int len, Array<Array<Gun *> *> *array);
extern "C" void ArraySetLength_Gun(int len, Array<Gun *> *array);
void MatrixGetPosition(void *out, float *matrix);
extern "C" int __aeabi_idiv(int a, int b);
extern "C" void Player_damageEmp_tail(Player *self);
extern "C" void ArrayAdd_Gun(Gun *gun, Array<Gun *> *array);
extern "C" void Player_shoot_tail(Player *self, int soundId);
extern "C" void Player_regenerateHull_tail();
extern "C" void ArrayReleaseClasses_GunArray(Array<Gun *> *array);
extern "C" void ArrayReleaseClasses_GunArrayArray(Array<Array<Gun *> *> *array);
extern "C" void *Array_GunArray_dtor(Array<Gun *> *array);
extern "C" void *Array_GunArrayArray_dtor(Array<Array<Gun *> *> *array);
extern "C" void Player_setGammaHP_tail();
extern "C" int gStopSoundIds[];
extern "C" void *gFModSound;
extern "C" void *gFModSoundAlt;
extern "C" void Player_stopShootSound_play_tail(void *sound, int id);
void Globals_addSoundResourceToList(int id);
extern "C" void Player_calcWeaponSounds_tail(int a, int b);
extern "C" void *gAppManager;
extern "C" void **gFModSoundPtr;
// Two engine overloads share this name; declared as C++ overloads (not extern "C")
// so overload resolution by arity/argument types selects the right one.
extern "C" void ArrayAdd_PlayerArray(Array<Player *> *src, Array<Player *> *dst);
extern "C" void Gun_setEnemies(void *gun);
extern "C" int gShootSoundsByType[];
extern "C" int gShootSoundsByIndex[];
extern "C" void *gAppManagerA;
extern "C" void *gAppManagerB;
extern "C" void *gAppManagerC;
extern "C" void Player_playShootSound_play_tail(float vol, void *sound, int id, Vector *pos, int z);
extern "C" void Player_playShootSound(Player *self, int type, Vector *channel, float volume);
extern "C" void Player_setEnemy_tail(Player *self, Player *enemy);
void Player_damage_full(Player *self, int amount, int a, int b);
extern "C" void Player_setMaxArmorHP_tail();
extern "C" void Player_StopEngineSound(Player *self);
extern "C" void FloatVectorMax(void *out, float a, float b, int c, int d);

void Player::pitchAllPrimaryGuns(float pitch) {
    Player *self = this;
    if (self->guns != 0) {
        Array<Gun *> *prim = self->guns->data()[0];
        if (prim != 0) {
            int n = prim->size();
            for (int i = 0; n != i; i++) {
                *(float *)((char *)prim->data()[i] + 0xb0) = pitch;
            }
        }
    }
}

unsigned char Player::isAlwaysEnemy() {
    Player *self = this;
    return self->alwaysEnemy;
}

void Player::setKIPlayer(KIPlayer *value) {
    Player *self = this;
    self->kiPlayer = value;
}

void Player::damageHull(int damage) {
    Player *self = this;
    if (!self->vulnerable) {
        return;
    }
    if (!self->active) {
        return;
    }
    int armor = self->armorHP;
    if (armor <= 0) {
        int h = self->hitpoints - damage;
        h &= ~(h >> 31);
        self->hitpoints = h;
    } else {
        armor -= damage;
        self->armorHP = armor;
    }
    if (armor <= -1) {
        self->armorHP = 0;
    }
    self->damaged = 1;
    return Player_damageHull_tail();
}

int Player::getShieldDamageRate() {
    Player *self = this;
    return self->shieldDamageRate;
}

int Player::replaceGuns(int a, int b, int c, int d, int e, bool f) {
    Player *self = this;
    return a;
}

int Player::getShieldHP() {
    Player *self = this;
    return (int)self->shieldHP;
}

void Player::removeAllGuns() {
    Player *self = this;
    if (self->guns != 0) {
        Player_removeAllGuns_releaseClasses(self->guns);
        if (self->guns != 0) {
            Player_removeAllGuns_delete(Player_removeAllGuns_dtor(self->guns));
        }
    }
    self->guns = 0;
}

int Player::getArmorDamageRate() {
    Player *self = this;
    return self->armorDamageRate;
}

int Player::getArmorHP() {
    Player *self = this;
    return self->armorHP;
}

void Player::setRadius(int value) {
    Player *self = this;
    self->radius = value;
}

void Player::resetDamageDoneByPlayer() {
    Player *self = this;
    self->field_dc = 0;
    self->damageDoneByPlayer = 0;
    self->turnedEnemy = 0;
}

KIPlayer * Player::getKIPlayer() {
    Player *self = this;
    return self->kiPlayer;
}

bool Player::isDead() {
    Player *self = this;
    return self->hitpoints < 1;
}

int Player::getRadius() {
    Player *self = this;
    return self->radius;
}

int Player::getEmpDamageRate() {
    Player *self = this;
    return self->empDamageRate;
}

unsigned char Player::isAlwaysFriend() {
    Player *self = this;
    return self->alwaysFriend;
}

int Player::getHitpoints() {
    Player *self = this;
    return self->hitpoints;
}

void Player::damageShield(int damage) {
    Player *self = this;
    if (!self->vulnerable) {
        return;
    }
    if (!self->active) {
        return;
    }
    float s = self->shieldHP;
    if (s <= 0.0f) {
        int h = self->hitpoints - damage;
        h &= ~(h >> 31);
        self->hitpoints = h;
    } else {
        s = s - (float)damage;
        self->shieldHP = s;
    }
    if (s < 0.0f) {
        self->shieldHP = 0;
    }
    self->damaged = 1;
    return Player_damageShield_tail();
}

void Player::regenerateArmor() {
    Player *self = this;
    int v = self->armorHP + 2;
    if (v > self->maxArmorHP) {
        v = self->maxArmorHP;
    }
    self->armorHP = v;
    return Player_regenerateArmor_tail();
}

void Player::damageShip(int damage) {
    Player *self = this;
    int v = self->hitpoints - damage;
    v &= ~(v >> 31);
    self->hitpoints = v;
}

int Player::getMaxHitpoints() {
    Player *self = this;
    return self->maxHitpoints;
}

int Player::getGammaHP() {
    Player *self = this;
    return (int)self->gammaHP;
}

float Player::getBombForce() {
    Player *self = this;
    return self->bombForce;
}

int Player::getMaxArmorHP() {
    Player *self = this;
    return self->maxArmorHP;
}

Player * Player::getEnemy(int index) {
    Player *self = this;
    return self->enemies->data()[index];
}

void Player::turnEnemy() {
    Player *self = this;
    self->turnedEnemy = 1;
}

void Player::setEmpData(int points, int data) {
    Player *self = this;
    self->empPoints = points;
    if (self->maxEmpPoints < points) {
        self->maxEmpPoints = points;
    }
    ((Player *)(self))->updateDamageRate();
    self->empData = data;
}

void Player::setVulnerable(bool value) {
    Player *self = this;
    self->vulnerable = value;
}

void Player::setActive(bool value) {
    Player *self = this;
    self->active = value;
}

// ---- awake_725xx ----
// KIPlayer::awake() flips its own state and then calls ((Player *)(player))->awake(1); that shim
// is a PLT interworking stub that branches directly into Player::setActive, so awakening
// the ship is exactly "make it active again".
void Player::awake(bool active) {
    setActive(active);
}

// ---- setDead ----
// The death path KIPlayer::setDead() -> KIPlayer::setActive(false) -> Player::setActive(false)
// pulls the destroyed ship out of the active simulation. setDead() is the Player-side hook the
// KIPlayer/PlayerEgo teardown calls reach.
void Player::setDead() {
    setActive(false);
}

bool Player::isAsteroid() {
    Player *self = this;
    KIPlayer *ki = self->kiPlayer;
    bool result = false;
    if (ki != 0) {
        result = *((char *)ki + 0x3c) != 0;
    }
    return result;
}

void Player::updateDamageRate() {
    Player *self = this;
    float maxHp = (float)self->maxHitpoints;
    float hp = (float)self->hitpoints;
    float maxArmor = (float)self->maxArmorHP;
    float armor = (float)self->armorHP;
    float maxEmp = (float)self->maxEmpPoints;
    float maxShield = (float)self->maxShieldHP;
    float emp = (float)self->empPoints;

    float shieldRate = (self->shieldHP / maxShield) * 100.0f;
    float armorRate = (armor / maxArmor) * 100.0f;
    float empRate = (emp / maxEmp) * 100.0f;

    self->damageRate = (int)((hp / maxHp) * 100.0f);
    self->shieldDamageRate = (int)shieldRate;
    self->armorDamageRate = (int)armorRate;
    self->empDamageRate = (int)empRate;
}

void Player::setBombForce(float value) {
    Player *self = this;
    self->bombForce = value;
}

void Player::setMaxHitpoints(int value) {
    Player *self = this;
    self->maxHitpoints = value;
    self->hitpoints = value;
    return Player_setMaxHitpoints_tail();
}

int Player::getGunRegenRate() {
    Player *self = this;
    return 0;
}

int Player::getMaxEmpPoints() {
    Player *self = this;
    return self->maxEmpPoints;
}

void Player::regenerateShield(float amount) {
    Player *self = this;
    float f = self->shieldHP + amount;
    float maxF = (float)self->maxShieldHP;
    if (f < maxF) {
        maxF = f;
    }
    self->shieldHP = maxF;
    return Player_regenerateShield_tail();
}

unsigned char Player::doesNeverAttack() {
    Player *self = this;
    return self->neverAttack;
}

int Player::getMaxShieldHP() {
    Player *self = this;
    return self->maxShieldHP;
}

unsigned char Player::isDamaged() {
    Player *self = this;
    return self->damaged;
}

unsigned char Player::isActive() {
    Player *self = this;
    return self->active;
}

// Duplicate of Player::shoot(int,int,long long,bool); the canonical definition (using
// Player_shoot_full2 and the 'b' argument) lives further below. Removed to avoid a
// redefinition; the leftover here also dropped 'b' and wrongly returned a value.

int Player::getEmpPoints() {
    Player *self = this;
    return self->empPoints;
}

int Player::GetEngineEvent() {
    Player *self = this;
    return (int)(__INTPTR_TYPE__)self->engineEvent;
}

void Player::setEmpForce(float value) {
    Player *self = this;
    self->empForce = value;
}

void Player::setShootingEnabled(bool value) {
    Player *self = this;
    self->shootingEnabled = value;
}

int Player::getDamageRate() {
    Player *self = this;
    return self->damageRate;
}

float Player::getEmpForce() {
    Player *self = this;
    return self->empForce;
}

// Accessor lives as the non-member inline Player_turnedEnemy() in Player.h, because a
// member of this name would collide with the 'turnedEnemy' data member (field 0xe0).

bool Player::gunAvailable(unsigned int slot) {
    Player *self = this;
    if (slot < 4) {
        Array<Gun *> *slotArray = self->guns->data()[slot];
        if (slotArray != 0 && slotArray->size() != 0) {
            return *(int *)slotArray->data() != 0;
        }
    }
    return false;
}

int Player::getCombinedHP() {
    Player *self = this;
    return (int)(self->shieldHP + (float)self->armorHP + (float)self->hitpoints);
}

bool Player::isGasCloud() {
    Player *self = this;
    KIPlayer *ki = self->kiPlayer;
    bool result = false;
    if (ki != 0) {
        result = *((char *)ki + 0x44) != 0;
    }
    return result;
}

void Player::setArmorHP(int value) {
    Player *self = this;
    if (self->maxArmorHP < value) {
        value = self->maxArmorHP;
    }
    self->armorHP = value;
    return Player_setArmorHP_tail();
}

void Player::setHitpoints(int value) {
    Player *self = this;
    self->hitpoints = value;
    if (self->maxHitpoints < value) {
        self->maxHitpoints = value;
    }
    return Player_setHitpoints_tail();
}

void Player::setNeverAttack(bool value) {
    Player *self = this;
    self->neverAttack = value;
}

void Player::setMaxShieldHP(int value) {
    Player *self = this;
    self->maxShieldHP = value;
    self->shieldHP = (float)value;
    return Player_setMaxShieldHP_tail();
}

void Player::setMaxEmpPoints(int value) {
    Player *self = this;
    self->empPoints = value;
    self->maxEmpPoints = value;
    return Player_setMaxEmpPoints_tail();
}

int Player::getGunSlots() {
    Player *self = this;
    return 3;
}

// Local POD for the returned 3-float hit vector (packed as a double xy + float z).
// Named HitVec3 to avoid colliding with PlayerEgo.h's `typedef Vector Vec3`.
struct HitVec3 {
    double xy;
    float z;
};

void Player::getHitVector(Vector *out) {
    Player *self = this;
    double xy = *(double *)self->hitVector;
    out->z = self->hitVector[2];
    *(double *)&out->x = xy;
}

void Player::setPlayShootSound(bool play, int id) {
    Player *self = this;
    self->playShootSound = play;
    self->playShootSoundId = id;
}

void Player::regenerateShield() {
    Player *self = this;
    float f = self->shieldHP + 1.0f;
    float maxF = (float)self->maxShieldHP;
    if (f < maxF) {
        maxF = f;
    }
    self->shieldHP = maxF;
    return Player_regenerateShield_tail();
}

void Player::heal(float amount) {
    Player *self = this;
    float f = self->field_110 + amount;
    self->field_110 = f;
    if (f > 1.0f) {
        int count = (int)f;
        for (int i = 0; i < count; i++) {
            ((Player *)(self))->regenerateHull();
        }
        self->field_110 = self->field_110 - (float)count;
    }
}

void Player::setShieldHP(int value) {
    Player *self = this;
    float maxF = (float)self->maxShieldHP;
    self->shieldHP = (float)value;
    if ((float)value > maxF) {
        self->shieldHP = maxF;
    }
    return Player_setShieldHP_tail();
}

void Player::refillGunDelay(int slot) {
    Player *self = this;
    Array<Array<Gun *> *> *guns = self->guns;
    if (guns != 0 && slot >= 0 && (unsigned int)slot < guns->size()) {
        Array<Gun *> *arr = guns->data()[slot];
        if (arr != 0) {
            int n = arr->size();
            for (int i = 0; n != i; i++) {
                Gun *gun = arr->data()[i];
                gun->timer = gun->fireDelay;
            }
        }
    }
}

void Player::addEnemies(Array<Player *> *enemies) {
    Player *self = this;
    if (self->enemies == 0) {
        return Player_setEnemies_tail(self, enemies);
    }
    Array<Player *> *tmp = static_cast<Array<Player *> *>(operator new(sizeof(Array<Player *>)));
    Array_Player_ctor(tmp);
    for (unsigned int i = 0; i < self->enemies->size(); i++) {
        ArrayAdd_Player(self->enemies->data()[i], tmp);
    }
    for (unsigned int i = 0; i < enemies->size(); i++) {
        ArrayAdd_Player(enemies->data()[i], tmp);
    }
    ((Player *)(self))->setEnemies(tmp);
    Array_Player_dtor(tmp);
    return Player_operator_delete_tail(tmp);
}

Player * Player::ctor(int radius, int hitpoints, int numPrimary, int numSecondary, int numTertiary) {
    Player *self = this;
    ((Matrix *)self->transform)->initIdentity();
    self->shieldHP = 0.0f;
    self->armorHP = 0;
    self->maxArmorHP = 0;
    self->maxShieldHP = 0;
    self->hitVector[0] = 0.0f;
    self->hitVector[1] = 0.0f;
    self->hitVector[2] = 0.0f;
    self->position[1] = 0.0f;
    self->position[2] = 0.0f;
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
    self->position[0] = 0.0f;
    self->field_60 = 0;
    self->bombForce = 0.0f;
    self->empForce = 0.0f;
    ((Player *)(self))->updateDamageRate();
    self->field_58 = -1;

    Array<Array<Gun *> *> *gunArr =
        static_cast<Array<Array<Gun *> *> *>(operator new(sizeof(Array<Array<Gun *> *>)));
    Array_GunArrayArray_ctor(gunArr);
    self->guns = gunArr;
    ArraySetLength_GunArrayArray(3, gunArr);

    if (numPrimary < 1) {
        self->guns->data()[0] = 0;
    } else {
        Array<Gun *> *a = static_cast<Array<Gun *> *>(operator new(sizeof(Array<Gun *>)));
        Array_Gun_ctor(a);
        self->guns->data()[0] = a;
        ArraySetLength_Gun(numPrimary, self->guns->data()[0]);
    }
    if (numSecondary < 1) {
        self->guns->data()[1] = 0;
    } else {
        Array<Gun *> *a = static_cast<Array<Gun *> *>(operator new(sizeof(Array<Gun *>)));
        Array_Gun_ctor(a);
        self->guns->data()[1] = a;
        ArraySetLength_Gun(numSecondary, self->guns->data()[1]);
    }
    if (numTertiary < 1) {
        self->guns->data()[2] = 0;
    } else {
        Array<Gun *> *a = static_cast<Array<Gun *> *>(operator new(sizeof(Array<Gun *>)));
        Array_Gun_ctor(a);
        self->guns->data()[2] = a;
        ArraySetLength_Gun(numTertiary, self->guns->data()[2]);
    }

    self->playShootSound = 1;
    self->playShootSoundId = 1;
    self->field_44 = 0;
    self->active = 1;
    self->field_b4 = 0;
    self->vulnerable = 1;
    self->kiPlayer = 0;
    self->bombForce = 0.0f;
    self->enemyFlags = 0;
    self->turnedEnemy = 0;
    *(uint16_t *)&self->alwaysEnemy = 0;
    self->enemies = 0;
    self->field_54 = 0;
    self->field_68 = 0;
    self->neverAttack = 0;
    self->field_110 = 0;
    self->engineEvent = 0;
    self->field_108 = 0;
    self->field_f8 = 0;
    self->field_64 = 0;

    float tmp[3];
    MatrixGetPosition(tmp, self->transform);
    *(Vector *)self->position = *(Vector *)tmp;
    self->field_f4 = (void *)(__INTPTR_TYPE__)-1;

    return self;
}

void Player::getPosition(Vector *out) {
    Player *self = this;
    MatrixGetPosition(out, self->transform);
}

float Player::damageGamma(float amount) {
    Player *self = this;
    if (self->vulnerable) {
        if (self->active) {
            amount = self->gammaHP - amount;
            *((unsigned char *)self + 0x67) = 1;
            self->gammaHP = amount;
            if (!(amount > 0.0f)) {
                self->gammaHP = 0;
            }
        }
    }
    return amount;
}

void Player::damageEmp(int amount, bool flag) {
    Player *self = this;
    if (self->vulnerable == 0 || self->active == 0) {
        return;
    }
    int hp = self->empPoints;
    if (hp > 0) {
        hp = self->hitpoints;
    }
    if (hp < 1) {
        return;
    }

    if (!flag && self->kiPlayer != 0 && self->alwaysEnemy == 0) {
        char *ki = (char *)self->kiPlayer;
        bool runLab = true;
        if ((unsigned int)(*(int *)(ki + 0x28) - 9) >= 2) {
            int sys = ((Status *)(*gStatus))->getSystem();
            ki = (char *)self->kiPlayer;
            if (sys != 0 && *(char *)(ki + 0x42) != 0) {
                if (amount > 0) {
                    ((Level *)(*(void **)(ki + 0x54)))->attackWanted(*(int *)(ki + 0x48));
                }
                runLab = false;
            } else if (self->kiPlayer == 0) {
                runLab = false;
            }
        }
        // LAB_000b3016
        if (runLab && self->alwaysEnemy == 0 && ((KIPlayer *)self->kiPlayer)->isWingMan() == 0 &&
            (unsigned int)(*(int *)((char *)self->kiPlayer + 0x28) - 9) > 1 &&
            ((Status *)(*gStatus))->getSystem() != 0) {
            int race = *(int *)((char *)self->kiPlayer + 0x28);
            ((Status *)(*gStatus))->getSystem();
            if (race == ((SolarSystem *)(intptr_t)((Status *)(*gStatus))->getSystem())->getRace()) {
                int prev = self->field_dc;
                self->field_dc = prev + amount;
                if (__aeabi_idiv(self->maxEmpPoints, 3) < prev + amount) {
                    self->turnedEnemy = 1;
                    ((Level *)(*(int *)((char *)self->kiPlayer + 0x54)))->friendTurnedEnemy();
                }
            }
        }
    }

    {
        int ep = self->empPoints - amount;
        self->empPoints = ep;
        if (ep > 0) {
            return Player_damageEmp_tail(self);
        }
    }
    if (!flag && self->kiPlayer != 0) {
        if (self->alwaysEnemy == 0 &&
            (unsigned int)(*(int *)((char *)self->kiPlayer + 0x28) - 9) > 1 &&
            ((Status *)(*gStatus))->getSystem() != 0) {
            int race = *(int *)((char *)self->kiPlayer + 0x28);
            ((Status *)(*gStatus))->getSystem();
            if (race == ((SolarSystem *)(intptr_t)((Status *)(*gStatus))->getSystem())->getRace()) {
                ((Level *)(*(void **)((char *)self->kiPlayer + 0x54)))->alarmAllFriends(*(int *)((char *)self->kiPlayer + 0x28), false);
            }
        }
        char *ki = (char *)self->kiPlayer;
        if (*(char *)(ki + 0x3c) != 0) {
            goto lab_30e2;
        }
        if (*(char *)(ki + 0x3d) != 0) {
            goto lab_30f4;
        }
        if (*(char *)(ki + 0x42) != 0) {
            goto lab_30f4;
        }
        {
            void *st = (void *)(long)((Status *)(*gStatus))->getStanding();
            ((Standing *)(st))->applyDisable(*(int *)((char *)self->kiPlayer + 0x28));
            ki = (char *)self->kiPlayer;
        }
    lab_30e2:
        if (ki == 0) {
            goto lab_3164;
        }
    lab_30f4:
        if (self->field_68 != 0) {
            goto lab_3164;
        }
        {
            int *prog = *g_damageEmp_progress;
            if (*(unsigned int *)((char *)prog + 0x134) > 0x7fffffff) {
                *(int *)((char *)prog + 0x134) = 0;
            }
            void *ach = *g_damageEmp_achievements;
            if (((Achievements *)(ach))->hasMedal(0x2a, 1) == 0) {
                int cnt = *(int *)((char *)(*g_damageEmp_progress) + 0x134) + 1;
                *(int *)((char *)(*g_damageEmp_progress) + 0x134) = cnt;
                if (((Achievements *)(*g_damageEmp_achievements))->getValue(0x2a, 1) <= cnt) {
                    void *ego = (void *)(__INTPTR_TYPE__)((Level *)(*(void **)((char *)self->kiPlayer + 0x54)))->getPlayer();
                    void *hud = (void *)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getHUD();
                    ((Hud *)(hud))->hudEventMedal(0x2a, 100);
                    *(char *)((char *)(*g_damageEmp_progress) + 0x138) = 1;
                }
            }
        }
    }

lab_3164:
    // LAB_000b3164
    float f = (float)self->empData;
    self->field_68 = 1;
    self->empPoints = 0;
    (self->pad_e8[0]) = 0;
    self->empForce = f;
    return Player_damageEmp_tail(self);
}

void Player::addGun(Array<Gun *> *gunsIn, int slot) {
    Player *self = this;
    if (self->guns != 0) {
        if ((unsigned int)slot < 4) {
            Array<Gun *> *arr = static_cast<Array<Gun *> *>(operator new(sizeof(Array<Gun *>)));
            Array_Gun_ctor(arr);
            self->guns->data()[slot] = arr;
            for (unsigned int i = 0; i < gunsIn->size(); i++) {
                ArrayAdd_Gun(gunsIn->data()[i], self->guns->data()[slot]);
            }
        }
        if (self->playShootSound) {
            return Player_shoot_tail(self, self->playShootSoundId);
        }
    }
}

void Player::setAlwaysEnemy(bool value) {
    Player *self = this;
    self->alwaysEnemy = value;
    self->enemyFlags = 1;
    self->turnedEnemy = 1;
}

void Player::regenerateHull() {
    Player *self = this;
    int v = self->maxHitpoints;
    if (self->hitpoints + 1 < self->maxHitpoints) {
        v = self->hitpoints + 1;
    }
    self->hitpoints = v;
    return Player_regenerateHull_tail();
}

// Player::~Player() — real C++ destructor so the demangled symbol contains "~Player".
// Uses the real Player/Array (std::vector) types from the headers; releases the per-slot gun
// arrays, then the outer gun-array-array, then the enemies array.
__attribute__((minsize)) Player::~Player()
{
    Array<Array<Gun *> *> *guns = this->guns;
    if (guns != 0) {
        for (unsigned int i = 0; i < guns->size(); i++) {
            Array<Gun *> *slot = guns->data()[i];
            if (slot != 0) {
                ArrayReleaseClasses_GunArray(slot);
                Array<Gun *> *s2 = this->guns->data()[i];
                if (s2 == 0) {
                    this->guns->data()[i] = 0;
                } else {
                    ::operator delete(Array_GunArray_dtor(s2));
                    this->guns->data()[i] = 0;
                }
                guns = this->guns;
            }
        }
        ArrayReleaseClasses_GunArrayArray(guns);
        if (this->guns != 0) {
            ::operator delete(Array_GunArrayArray_dtor(this->guns));
        }
        this->guns = 0;
    }
    if (this->enemies != 0) {
        ::operator delete(Array_Player_dtor(this->enemies));
    }
    this->enemies = 0;
}

void Player::setGammaHP(int value) {
    Player *self = this;
    float f = (float)value;
    float sel = f;
    if (value != 9999999) {
        sel = 100.0f;
    }
    if (value > 100) {
        f = sel;
    }
    self->gammaHP = f;
    return Player_setGammaHP_tail();
}

void Player::stopShootSound(int index, int channel) {
    Player *self = this;
    if ((unsigned int)channel > 8) {
        return;
    }
    if (((1 << channel) & 0x10c) != 0) {
        void *sound;
        int id;
        if (self->kiPlayer != 0 && *(int *)((char *)self->kiPlayer + 0x28) == 9) {
            id = 0x3e;
            sound = gFModSoundAlt;
        } else {
            id = gStopSoundIds[index];
            sound = gFModSound;
        }
        return Player_stopShootSound_play_tail(sound, id);
    }
}

void Player::reset() {
    Player *self = this;
    float shield = (float)self->maxShieldHP;
    int maxHp = self->maxHitpoints;
    int maxEmp = self->maxEmpPoints;
    self->gammaHP = 100.0f;
    self->hitpoints = maxHp;
    self->empPoints = maxEmp;
    self->shieldHP = shield;
    ((Player *)(self))->updateDamageRate();
    self->vulnerable = 1;
    self->active = 1;  // active = 1, damaged = 0
    self->field_54 = 0;
    *((unsigned char *)self + 0x44) = 0;
    self->damageDoneByPlayer = 0;
    self->field_5e = 0;
    self->field_b4 = 0;
    *((unsigned char *)self + 0x68) = 0;
    self->field_64 = 0;
    self->bombForce = 0;
    self->empForce = 0;
    (self->pad_dd[0]) = 0;
    (((uint8_t*)&self->empForce)[1]) = 0;
}

void Player::addGun(Gun *gun, int slot) {
    Player *self = this;
    if (self->guns != 0) {
        if ((unsigned int)slot < 4) {
            Array<Gun *> *arr = static_cast<Array<Gun *> *>(operator new(sizeof(Array<Gun *>)));
            Array_Gun_ctor(arr);
            self->guns->data()[slot] = arr;
            ArrayAdd_Gun(gun, self->guns->data()[slot]);
        }
        if (self->playShootSound) {
            return Player_shoot_tail(self, self->playShootSoundId);
        }
    }
}

void Player::calcWeaponSounds(int count) {
    Player *self = this;
    Array<Array<Gun *> *> *guns = self->guns;
    if (guns == 0) {
        return;
    }
    if (guns->data()[0] != 0) {
        unsigned int n = ((Array<Gun *> *)guns->data()[0])->size();
        int *order = new int[n];
        for (int i = 0; i < (int)n; i++) {
            order[i] = *(int *)((char *)self->guns->data()[0]->data()[i] + 0x58);
        }

        bool sorted = true;
        int i = 1;
        int *table = g_cws_items;
        do {
            for (; i < (int)n; i++) {
                int *dataArr = *(int **)((char *)(*(void **)table) + 4);
                int a = ((Item *)(*(void **)&dataArr[order[i - 1]]))->getSinglePrice();
                dataArr = *(int **)((char *)table + 4);
                int b = ((Item *)(*(void **)&dataArr[order[i]]))->getSinglePrice();
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
        int *sound = g_cws_sound;
        do {
            if ((int)n <= idx) break;
            if (order[idx] >= 0) {
                *(char *)((char *)self->guns->data()[0]->data()[idx] + 0x89) = 1;
                Globals_addSoundResourceToList(*sound);
                count--;
            }
            idx++;
        } while (count != 0);

        operator delete[](order);
        guns = self->guns;
    }

    if (guns->size() > 2) {
        Array<Gun *> *slot2 = guns->data()[2];
        if (slot2 != 0 && slot2->size() != 0) {
            Gun *g = slot2->data()[0];
            if (g != 0) {
                int sid = g_cws_sound3[g->itemIndex];
                g->field_0x89 = 1;
                return Player_calcWeaponSounds_tail(*g_cws_sound2, sid);
            }
        }
    }
}

__attribute__((minsize)) extern "C" void Player_PauseEngineSound(Player *self)
{
    void *event = self->engineEvent;
    if (event != 0) {
        self->field_f8 = ((FModSound *)(gFModSound))->pause(event);
    }
}

struct Mat { float m[12]; };

__attribute__((minsize)) extern "C" void Player_PlayEngineSound(Player *self, Vector *vec)
{
    self->field_f4 = vec;
    if (*((char *)gAppManager + 0xf) != 0) {
        Mat pos;
        MatrixGetPosition(&pos, self->transform);
        void *ev = ((FModSound *)(gFModSoundPtr[0]))->updateEvent3DAttributes(self->engineEvent, 0, (Vector *)self->field_f4, (Vector *)&pos, false);
        self->engineEvent = ev;
        self->field_108 = 1;
    }
}

void Player::setEnemies(Array<Player *> *enemies) {
    Player *self = this;
    if (self->enemies != 0) {
        ::operator delete(Array_Player_dtor(self->enemies));
    }
    self->enemies = 0;
    if (enemies != 0) {
        Array<Player *> *copy = static_cast<Array<Player *> *>(operator new(sizeof(Array<Player *>)));
        Array_Player_ctor(copy);
        self->enemies = copy;
        ArrayAdd_PlayerArray(enemies, copy);
    }
    Array<Array<Gun *> *> *guns = self->guns;
    if (guns != 0) {
        for (unsigned int i = 0; i < guns->size(); i++) {
            Array<Gun *> *slot = guns->data()[i];
            if (slot != 0) {
                for (unsigned int j = 0; j < slot->size(); j++) {
                    Gun *gun = slot->data()[j];
                    if (gun != 0) {
                        Gun_setEnemies(gun);
                        guns = self->guns;
                        slot = guns->data()[i];
                    }
                }
            }
        }
    }
}

__attribute__((minsize)) extern "C" void Player_StopEngineSound(Player *self)
{
    void *event = self->engineEvent;
    if (event != 0) {
        ((FModSound *)(gFModSound))->stop(event);
        self->field_108 = 0;
        self->engineEvent = 0;
    }
}

__attribute__((minsize)) extern "C" void Player_playShootSound(Player *self, int type, Vector *channel, float volume)
{
    int soundId;
    if (self->kiPlayer == 0) {
        soundId = gShootSoundsByIndex[type];
    } else {
        unsigned int kind = *(unsigned int *)((char *)self->kiPlayer + 0x28);
        if (kind < 0xb) {
            soundId = gShootSoundsByType[kind];
        } else {
            soundId = 0x3d;
        }
    }
    void **fmodPtr = gFModSoundPtr;
    void *sound = fmodPtr[0];
    Vector *pos = channel;
    if ((unsigned int)(__UINTPTR_TYPE__)channel < 9 && ((1 << ((unsigned int)(__UINTPTR_TYPE__)channel & 0xff)) & 0x10c) != 0) {
        if (((FModSound *)(sound))->isPlaying(soundId) != 0) {
            if (*((char *)gAppManagerA + 0xf) != 0) {
                ((FModSound *)(fmodPtr[0]))->updateEvent3DAttributes(soundId, pos, 0, false);
            }
            return;
        }
        if (*((char *)gAppManagerB + 0xf) == 0) {
            pos = 0;
        }
        sound = fmodPtr[0];
    } else if (*((char *)gAppManagerC + 0xf) == 0) {
        pos = 0;
    }
    return Player_playShootSound_play_tail(volume, sound, soundId, pos, 0);
}

extern "C" const float k_shootAt_inc;

void Player::shoot1(unsigned int slot, int idLo, int idHi, int flag, int m0, int m1, int m2, int m3, int m4, int m5, int m6, int m7, int m8, int m9, int m10, int m11, int m12, int m13, int m14) {
    Player *self = this;
    (void)idLo; (void)idHi;

    Array<Array<Gun *> *> *guns = self->guns;
    if (guns != 0 && self->shootingEnabled != 0 && (int)slot >= 0 &&
        slot < guns->size()) {
        Array<Gun *> *arr = guns->data()[slot];
        if (arr != 0) {
            for (unsigned int i = 0; i < arr->size(); i++) {
                Gun *g = self->guns->data()[slot]->data()[i];
                if (g->fireDelay < g->timer) {
                    // Rebuild the firing transform from the flattened matrix args (m0..m14)
                    // that the decompiler spilled out of the by-value Matrix parameter.
                    Matrix fireMat;
                    {
                        int mi[15] = { m0, m1, m2, m3, m4, m5, m6, m7,
                                       m8, m9, m10, m11, m12, m13, m14 };
                        for (int k = 0; k < 15; ++k)
                            fireMat.m[k] = *(float *)&mi[k];
                        fireMat.m[15] = 1.0f;
                    }
                    ((Gun *)(g))->shootAt(fireMat, flag, (Player *)self, false);
                    {
                        self->field_60 = self->field_60 + k_shootAt_inc;
                        Gun *g2 = self->guns->data()[slot]->data()[i];
                        g2->timer = 0;
                        if (self->playShootSound != 0 && g2->field_0x89 != 0) {
                            float tmp[3];
                            MatrixGetPosition(tmp, self->transform);
                            Gun *g3 = self->guns->data()[slot]->data()[i];
                            Player_playShootSound(self, g3->itemIndex,
                                                  (Vector *)(__INTPTR_TYPE__)g3->weaponType,
                                                  g3->field_0xb0);
                        }
                    }
                }
            }
        }
    }

    
}

extern "C" const float k_damage_full;
extern "C" const float k_damage_hc;
extern "C" const float k_damage_full2;
extern "C" const float k_damage_hc2;
extern "C" const float k_damage_regen;

// Full damage implementation. Player::damage(int) below forwards here with flag=0,
// missionId=-1. Matches the Player_damage_full(self, amount, a, b) prototype at top.
void Player_damage_full(Player *self, int amount, int flag, int missionId) {
    if (self->vulnerable == 0) return;
    if (self->active == 0) return;
    if (self->hitpoints < 1) return;

    if (flag == 0 && self->kiPlayer != 0) {
        char *ki = (char *)self->kiPlayer;
        if (self->alwaysEnemy == 0 &&
            (unsigned int)(*(int *)(ki + 0x28) - 9) > 1 &&
            ((Status *)(*gStatus))->getSystem() != 0 &&
            ((self->enemyFlags == 0) || (self->turnedEnemy != 0))) {
            ki = (char *)self->kiPlayer;
            if (*(char *)(ki + 0x42) != 0) {
                if (amount > 0) {
                    self->damageDoneByPlayer = self->damageDoneByPlayer + amount;
                    ((Level *)(*(void **)(ki + 0x54)))->attackWanted(*(int *)(ki + 0x48));
                }
                goto LAB_3488;
            }
        } else {
            ki = (char *)self->kiPlayer;
        }

        if (ki != 0 && self->alwaysEnemy == 0 &&
            (unsigned int)(*(int *)(ki + 0x28) - 9) > 1 &&
            ((KIPlayer *)self->kiPlayer)->isWingMan() == 0 && ((Status *)(*gStatus))->getSystem() != 0 &&
            ((self->enemyFlags == 0) || (self->turnedEnemy != 0))) {
            int race = *(int *)((char *)self->kiPlayer + 0x28);
            ((Status *)(*gStatus))->getSystem();
            bool sameRace = (race == ((SolarSystem *)(intptr_t)((Status *)(*gStatus))->getSystem())->getRace());
            if (!sameRace) {
                int race2 = *(int *)((char *)self->kiPlayer + 0x28);
                void *sys = (void *)(long)((Status *)(*gStatus))->getSystem();
                if (race2 != ((SolarSystem *)(sys))->getAttackRace()) {
                    goto LAB_342a;
                }
            }
            self->damageDoneByPlayer = self->damageDoneByPlayer + amount;
            int hc = ((Status *)(*gStatus))->hardCoreMode();
            float thr1 = (hc != 0) ? k_damage_hc : k_damage_full;
            float f1 = thr1 * (float)self->maxHitpoints;
            float dmgF = (float)self->damageDoneByPlayer;
            float frac2 = (hc != 0) ? 0.25f : 0.5f;
            float thr3 = (hc != 0) ? k_damage_hc2 : k_damage_full2;

            if (f1 < dmgF) {
                ((Level *)(*(int *)((char *)self->kiPlayer + 0x54)))->friendTurnedEnemy();
                void *ship = (void *)((Status *)(*gStatus))->getShip();
                void *standing = (void *)(long)((Status *)(*gStatus))->getStanding();
                if (((Ship *)(ship))->getSignatureRace() >= 0) {
                    bool match = ((unsigned int)((Ship *)(ship))->getSignatureRace() ==
                                  *(unsigned int *)((char *)self->kiPlayer + 0x28));
                    unsigned int dis = 0;
                    if (match) dis = *(unsigned char *)((char *)self->kiPlayer + 0x42);
                    if (match && dis == 0) {
                        Item *item = ((Ship *)(ship))->getFirstEquipmentOfSort(0x1d);
                        ((Ship *)(ship))->removeEquipment(item);
                        void *st2 = (void *)(long)((Status *)(*gStatus))->getStanding();
                        ((Standing *)(st2))->applyDelict(((Ship *)(ship))->getSignatureRace(), 100);
                        ((Standing *)(standing))->setPlayerSignatureRace(-1);
                        void *ego = (void *)(__INTPTR_TYPE__)((Level *)(*(void **)((char *)self->kiPlayer + 0x54)))->getPlayer();
                        int hud = (int)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getHUD();
                        int p = (int)(long)((Level *)(*(void **)((char *)self->kiPlayer + 0x54)))->getPlayer();
                        ((Hud *)(hud))->hudEvent(0x1f, (void *)(__INTPTR_TYPE__)p, 0);
                    }
                }
            }

            float f3 = (float)self->maxHitpoints;
            float dmgF3 = (float)self->damageDoneByPlayer;
            if (frac2 * f3 < dmgF3) {
                void *ship = (void *)((Status *)(*gStatus))->getShip();
                void *standing = (void *)(long)((Status *)(*gStatus))->getStanding();
                if (((Ship *)(ship))->getSignatureRace() >= 0 &&
                    *(int *)((char *)self->kiPlayer + 0x28) < 4 &&
                    *(char *)((char *)self->kiPlayer + 0x42) == 0) {
                    Item *item = ((Ship *)(ship))->getFirstEquipmentOfSort(0x1d);
                    ((Ship *)(ship))->removeEquipment(item);
                    void *st2 = (void *)(long)((Status *)(*gStatus))->getStanding();
                    ((Standing *)(st2))->applyDelict(((Ship *)(ship))->getSignatureRace(), 100);
                    ((Standing *)(standing))->setPlayerSignatureRace(-1);
                    void *ego = (void *)(__INTPTR_TYPE__)((Level *)(*(void **)((char *)self->kiPlayer + 0x54)))->getPlayer();
                    int hud = (int)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getHUD();
                    int p = (int)(long)((Level *)(*(void **)((char *)self->kiPlayer + 0x54)))->getPlayer();
                    ((Hud *)(hud))->hudEvent(0x1f, (void *)(__INTPTR_TYPE__)p, 0);
                }
                self->turnedEnemy = 1;
            }

            float f4a = (float)self->maxHitpoints;
            float dmgF4 = (float)self->damageDoneByPlayer;
            if (thr3 * f4a < dmgF4) {
                ((Level *)(*(void **)((char *)self->kiPlayer + 0x54)))->alarmAllFriends(*(int *)((char *)self->kiPlayer + 0x28), true);
            }
            goto LAB_3488;
        }
    }

LAB_342a:
    {
        Globals *globals = (Globals *)*g_damage_globals;
        if (((Status *)(*gStatus))->inBlackMarketSystem() != 0) {
            char *ki = (char *)self->kiPlayer;
            if (ki != 0 && *(int *)(ki + 0x28) == 8) {
                self->turnedEnemy = 1;
                ((Level *)(*(void **)(ki + 0x54)))->alarmAllFriends(8, true);
                int *enemies = (int *)(__INTPTR_TYPE__)((Level *)(*(void **)(ki + 0x54)))->getEnemies();
                if (enemies != 0) {
                    int count = enemies[0];
                    int i = 0;
                    while (count != i) {
                        int e = *(int *)(enemies[1] + i * 4);
                        i++;
                        if (*(int *)(e + 0x28) == 8) {
                            *(char *)(e + 0x25) = 1;
                        }
                    }
                }
                globals->field_0x110 = 0x100;
            }
        }
    }

LAB_3488:
    {
        int shieldI = (int)self->shieldHP - amount;
        if (shieldI < 0) {
            *(int *)&self->shieldHP = 0;
            shieldI = shieldI + self->armorHP;
            if (shieldI < 0) {
                self->armorHP = 0;
                (((uint8_t*)&self->field_64)[2]) = 1;
                self->hitpoints = shieldI + self->hitpoints;
            } else {
                (((uint8_t*)&self->field_64)[1]) = 1;
                self->armorHP = shieldI;
            }
        } else {
            self->field_64 = 1;
            self->shieldHP = (float)shieldI;
        }
    }

    {
        int hp;
        char *ki = (char *)self->kiPlayer;
        if (ki != 0 && *(char *)(ki + 0x3c) == 0 && *(char *)(ki + 0x3d) == 0 &&
            *(char *)(ki + 0x42) != 0) {
            hp = self->hitpoints;
            if (__aeabi_idiv(self->maxHitpoints, 3) > hp) {
                ((Level *)(*(int *)(ki + 0x54)))->almostKillWanted(*(int *)(ki + 0x48));
            } else {
                goto LAB_34f8_hp;
            }
        }
        hp = self->hitpoints;
    LAB_34f8_hp:
        if (hp < 1) {
            self->hitpoints = 0;
            if (flag != 0) {
                self->field_44 = 1;
            } else {
                char *ki2 = (char *)self->kiPlayer;
                if (ki2 != 0 && *(char *)(ki2 + 0x3c) == 0 && *(char *)(ki2 + 0x3d) == 0 &&
                    ((Status *)(*gStatus))->inBlackMarketSystem() == 0) {
                    if (*(char *)((char *)self->kiPlayer + 0x42) == 0) {
                        void *st = (void *)(long)((Status *)(*gStatus))->getStanding();
                        ((Standing *)(st))->applyKill(*(int *)((char *)self->kiPlayer + 0x28));
                    }
                    int mission = ((Status *)(*gStatus))->getCampaignMission();
                    char *ki3 = (char *)self->kiPlayer;
                    // NOTE: original text-table key for this name lookup was dropped by the
                    // decompiler; using missionId (the only contextual id) to keep it compiling.
                    void *txt = ((GameText *)(*g_damage_text[0]))->getText(missionId);
                    int cmp = ((String *)(ki3 + 0x18))->Compare_str((String *)txt);
                    if (missionId == 0xb3 && cmp == 0) {
                        // Ghidra: mission->setStatusValue(mission->getStatusValue() + 1)
                        ((Mission *)(intptr_t)mission)->setStatusValue(
                            ((Mission *)(intptr_t)mission)->getStatusValue() + 1);
                    }
                    if (*(char *)((char *)self->kiPlayer + 0x42) != 0) {
                        ((Level *)(*(int *)((char *)self->kiPlayer + 0x54)))->killWanted();
                    }
                }
            }
        }
    }

    self->damaged = 1;
    *(float *)&self->field_60 = *(float *)&self->field_60 + k_damage_regen;
    ((Player *)(self))->updateDamageRate();
    if (self->kiPlayer != 0) {
        int slave = *(int *)((char *)self->kiPlayer + 0x10);
        if (slave != 0) {
            Player *other = *(Player **)(slave + 4);
            other->vulnerable = 1;
            other->damage(amount);
            *(char *)(*(int *)(*(int *)((char *)self->kiPlayer + 0x10) + 4) + 0xc2) = 0;
        }
    }
}

void Player::setEnemy(Player *enemy) {
    Player *self = this;
    Array<Player *> *tmp = static_cast<Array<Player *> *>(operator new(sizeof(Array<Player *>)));
    Array_Player_ctor(tmp);
    ArrayAdd_Player(enemy, tmp);
    ((Player *)(self))->setEnemies(tmp);
    Array_Player_dtor(tmp);
    return Player_operator_delete_tail(tmp);
}

void Player::addEnemy(Player *enemy) {
    Player *self = this;
    if (self->enemies == 0) {
        return Player_setEnemy_tail(self, enemy);
    }
    Array<Player *> *tmp = static_cast<Array<Player *> *>(operator new(sizeof(Array<Player *>)));
    Array_Player_ctor(tmp);
    if (self->enemies->size() != 0) {
        ArrayAdd_PlayerArray(self->enemies, tmp);
    }
    ArrayAdd_Player(enemy, tmp);
    ((Player *)(self))->setEnemies(tmp);
    Array_Player_dtor(tmp);
    return Player_operator_delete_tail(tmp);
}

__attribute__((minsize)) extern "C" void Player_ResumeEngineSound(Player *self, bool force)
{
    void *event = self->engineEvent;
    if (event != 0 && (self->field_f8 != 0 || force)) {
        self->field_f8 = ((FModSound *)(gFModSound))->resume(event) ^ 1;
    }
}

void Player::damage(int amount) {
    Player *self = this;
    return Player_damage_full(self, amount, 0, -1);
}

void Player::stopShooting(int slot, int channel) {
    Player *self = this;
    if ((unsigned int)(channel - 0x16) >= 9) {
        return;
    }
    Array<Array<Gun *> *> *guns = self->guns;
    if (guns == 0) {
        return;
    }
    if (slot < 0) {
        return;
    }
    if ((unsigned int)slot >= guns->size()) {
        return;
    }
    Array<Gun *> *arr = guns->data()[slot];
    if (arr == 0) {
        return;
    }
    for (unsigned int i = 0; i < arr->size(); i++) {
        Gun *gun = arr->data()[i];
        ((Player *)(self))->stopShootSound(gun->itemIndex, gun->weaponType);
        arr = self->guns->data()[slot];
    }
}

void Player::setAlwaysFriend(bool value) {
    Player *self = this;
    self->alwaysFriend = value;
    self->enemyFlags = 0x100;
    self->turnedEnemy = 0;
}

void Player::setMaxArmorHP(int value) {
    Player *self = this;
    self->armorHP = value;
    self->maxArmorHP = value;
    return Player_setMaxArmorHP_tail();
}

extern "C" const float k_shoot_inc;

int Player::shoot2(unsigned int slot, int gunId, int a4_00, int flag, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, int a20, int a21, int a22) {
    Player *self = this;
    unsigned int mask = g_shoot_mask;
    int retval = 1;

    Array<Array<Gun *> *> *guns = self->guns;
    if (guns != 0 && self->shootingEnabled != 0 && (int)slot >= 0 &&
        slot < guns->size()) {
        Array<Gun *> *arr = guns->data()[slot];
        if (arr != 0) {
            for (unsigned int i = 0; i < arr->size(); i++) {
                Gun *g = self->guns->data()[slot]->data()[i];
                unsigned int sortIdx = g->weaponType - 6;
                if (sortIdx < 0x1d && ((1u << (sortIdx & 0xff)) & mask) != 0 &&
                    *(int *)g->lifetimes >= 0) {
                    ((Gun *)(g))->ignite();
                } else if (g->itemIndex == gunId &&
                           g->fireDelay < g->timer) {
                    if (sortIdx < 0x1d && ((1u << (sortIdx & 0xff)) & mask) != 0) {
                        *(char *)(g->field_0x38 + 0x69) = 1;
                    }
                    // Rebuild the by-value firing Matrix the decompiler spilled to a8..a22.
                    Matrix fireMat2;
                    {
                        int mi[15] = { a8, a9, a10, a11, a12, a13, a14, a15,
                                       a16, a17, a18, a19, a20, a21, a22 };
                        for (int k = 0; k < 15; ++k)
                            fireMat2.m[k] = *(float *)&mi[k];
                        fireMat2.m[15] = 1.0f;
                    }
                    ((Gun *)(g))->shoot(fireMat2, flag, false);
                    {
                        self->field_60 = self->field_60 + k_shoot_inc;
                        if (self->playShootSound != 0) {
                            float tmp[3];
                            MatrixGetPosition(tmp, self->transform);
                            Gun *g2 = self->guns->data()[slot]->data()[i];
                            Player_playShootSound(self, g2->itemIndex,
                                                  (Vector *)(__INTPTR_TYPE__)g2->weaponType,
                                                  g2->field_0xb0);
                        }
                        g->timer = 0;
                        retval = 1;
                        break;
                    }
                }
            }
        }
    }

    
    return retval;
}

extern "C" void (**g_update_transform)(void *, void *, int);  // DAT_b41e2 fn ptr table
extern "C" const float k_update_a;
extern "C" const float k_update_b;
extern "C" const float k_update_c;

Vector * Player::update(int dt, int doSound) {
    Player *self = this;

    int b4 = self->field_b4 + dt;
    self->field_b4 = b4;
    if (b4 > 3000) {
        self->damaged = 0;
        self->field_b4 = 0;
    }

    Vector *result = 0;

    if (self->field_68 != 0) {
        int e = self->empData + dt;
        float ef = (float)self->empData;
        float ef2 = (float)e;
        int maxEmp = self->maxEmpPoints;
        float mf = (float)maxEmp;
        float v = (ef2 / ef) * mf;
        self->empData = e;
        self->empPoints = (int)v;
        if (maxEmp < (int)v) {
            self->field_68 = 0;
            self->empPoints = maxEmp;
            int *clk = *g_update_clock;
            *(int *)((char *)clk + 0x134) = *(int *)((char *)clk + 0x134) - 1;
            self->empData = 0;
        }
        ((Player *)(self))->updateDamageRate();
    }

    char *flagObj = *g_update_flag;
    if (*(char *)(flagObj + 0xf) == 0 || doSound == 0 || self->field_f4 == (void *)(__INTPTR_TYPE__)-1) {
        if (self->field_108 != 0) {
            Player_StopEngineSound(self);
        }
    } else {
        float *transform = self->transform;
        void (*fn)(void *, void *, int) = *g_update_transform;
        float local[3];
        float spd = (float)(**g_update_speed);
        fn(*g_update_speed, local, (int)(long)transform);
        float tmpA[3], tmpB[3], tmpC[3];
        float d = (*(Vector *)tmpA = *(const Vector *)self->position - *(const Vector *)local,
                   AbyssEngine::AEMath::VectorLength(*(const Vector *)tmpA));
        *(Vector *)tmpB = d * *(const Vector *)tmpA;
        *(Vector *)tmpC = *(const Vector *)tmpB; *(Vector *)tmpC /= (float)dt;
        (void)spd;
        fn(tmpB, local, (int)(long)transform);
        void *ev = ((FModSound *)(*g_update_sound))->updateEvent3DAttributes(self->engineEvent, 0, (Vector *)tmpB, (Vector *)tmpC, false);
        self->engineEvent = ev;
        self->field_108 = 1;
        fn(tmpA, local, (int)(long)transform);
        *(Vector *)self->position = *(Vector *)tmpA;
    }

    float speed = (float)dt;
    float nf = *(float *)&self->field_60 + speed * k_update_a * k_update_b;
    *(float *)&self->field_60 = nf;
    FloatVectorMax(&result, nf, k_update_c, 2, 0x20);

    
    return result;
}

// Forwards to the full Player::shoot overload, expanding the transform matrix
// (fields 0x04..0x3c) into the trailing arguments.
extern "C" void Player_shoot_full2(
    Player *self, int a, int b, int loC, int hiC,
    int m0, int m1, int m2, int m3, int m4, int m5, int m6, int m7,
    int m8, int m9, int m10, int m11, int m12, int m13, int m14);

void Player::shoot(int a, int b, long long pos, bool flag) {
    Player *self = this;
    int *m = (int *)self->transform;
    Player_shoot_full2(self, a, b, (int)pos, (int)((unsigned long long)pos >> 32),
                       m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7],
                       m[8], m[9], m[10], m[11], m[12], m[13], m[14]);
}

// Player::stopShooting(int) — single-arg overload (no channel filter).
void Player::stopShooting(int slot) {
    Player *self = this;
    Array<Array<Gun *> *> *guns = self->guns;
    if (guns == 0) {
        return;
    }
    if (slot < 0) {
        return;
    }
    if ((unsigned int)slot >= guns->size()) {
        return;
    }
    Array<Gun *> *arr = guns->data()[slot];
    if (arr == 0) {
        return;
    }
    for (unsigned int i = 0; i < arr->size(); i++) {
        Gun *gun = arr->data()[i];
        ((Player *)(self))->stopShootSound(gun->itemIndex, gun->weaponType);
        arr = self->guns->data()[slot];
    }
}

float * Player::setHitVector(float x, float y, float z) {
    Player *self = this;
    float *p = self->hitVector;
    *p++ = x;
    *p++ = y;
    *p++ = z;
    return p;
}

void Player::resetGunDelay(int slot) {
    Player *self = this;
    Array<Array<Gun *> *> *guns = self->guns;
    if (guns == 0) {
        return;
    }
    if (slot < 0) {
        return;
    }
    if ((unsigned int)slot >= guns->size()) {
        return;
    }
    Array<Gun *> *arr = guns->data()[slot];
    if (arr != 0) {
        int n = arr->size();
        for (int i = 0; n != i; i++) {
            *(int *)((char *)arr->data()[i] + 0x6c) = 0;
        }
    }
}

// =====================================================================
// Real Player:: member methods recovered from the corresponding free
// functions above. The remaining ones below have no earlier definition.
// =====================================================================

// ---- getEnemies_a2cd2 ----
Array<Player *> *Player::getEnemies() {
    return this->enemies;
}

// NOTE: playShootSound is intentionally NOT added as a Player:: member — it collides with the
// data-member flag at 0x70. The recovered body lives as the extern "C" Player_playShootSound above.

// ---- PlayEngineSound_a4014 ----
void Player::PlayEngineSound(Vector *vec) {
    this->field_f4 = vec;
    if (*((char *)gAppManager + 0xf) != 0) {
        float pos[12];
        MatrixGetPosition(pos, this->transform);
        void *ev = ((FModSound *)gFModSoundPtr[0])->updateEvent3DAttributes(
            this->engineEvent, 0, (Vector *)this->field_f4, (Vector *)pos, false);
        this->engineEvent = ev;
        this->field_108 = 1;
    }
}

// ---- PauseEngineSound_a4088 ----
void Player::PauseEngineSound() {
    void *event = this->engineEvent;
    if (event != 0) {
        this->field_f8 = ((FModSound *)gFModSound)->pause(event);
    }
}

// ---- ResumeEngineSound_a40ac ----
void Player::ResumeEngineSound(bool force) {
    void *event = this->engineEvent;
    if (event != 0 && (this->field_f8 != 0 || force)) {
        this->field_f8 = ((FModSound *)gFModSound)->resume(event) ^ 1;
    }
}

// ---- StopEngineSound_a40e0 ----
void Player::StopEngineSound() {
    void *event = this->engineEvent;
    if (event != 0) {
        ((FModSound *)gFModSound)->stop(event);
        this->field_108 = 0;
        this->engineEvent = 0;
    }
}

// =====================================================================
// Recovered tail-merge fragments.
//
// The decompiler split several Player methods at their final tail-branch
// (a `b.w` through an ARM/Thumb interworking veneer) into a separate
// `Player_<method>_tail` symbol that the caller forwards to. Each veneer
// was resolved to its real target; the fragments below carry the work
// that target performs. They are kept as the same `extern "C"` symbols the
// call sites already reference (the later wiring pass folds them back into
// their callers), so only definitions are added here.
// =====================================================================

// All HP/armor/shield/emp setters and the regen/damage helpers end by
// recomputing the cached damage rate. The veneer at these tails resolves
// to Player::updateDamageRate (0x72df0), with `this` still live in r0.
// The standalone C shims have no receiver, so they're documented forwarders;
// the wiring pass turns each `return Player_xxx_tail();` into
// `this->updateDamageRate();` inside the owning method.

// setEnemies(Array*) inner-loop tail: when the player has no enemy list yet,
// addEnemies short-circuits straight into setEnemies (veneer -> Player::setEnemies).
extern "C" void Player_setEnemies_tail(Player *self, Array<Player *> *enemies) {
    self->setEnemies(enemies);
}

// setEnemy(Player*) tail: wrap a single enemy in a fresh list and install it.
extern "C" void Player_setEnemy_tail(Player *self, Player *enemy) {
    self->setEnemy(enemy);
}

// operator delete tail: the terminal `b.w` of the addEnemies/addEnemy/setEnemy
// helpers releases the temporary list (veneer -> operator delete).
extern "C" void Player_operator_delete_tail(void *p) {
    ::operator delete(p);
}

// addGun/shoot tail: after mutating the gun slots, recompute the weapon-sound
// table for the player (veneer -> Player::calcWeaponSounds).
extern "C" void Player_shoot_tail(Player *self, int soundId) {
    self->calcWeaponSounds(soundId);
}

// calcWeaponSounds tail: register the secondary-slot weapon's fire sound with
// the global resource list (veneer -> Globals::addSoundResourceToList).
extern "C" void Player_calcWeaponSounds_tail(int soundId, int /*sid*/) {
    Globals_addSoundResourceToList(soundId);
}

// damageEmp tail: like the HP setters, recompute the damage rate once the EMP
// hit has been applied (veneer -> Player::updateDamageRate, `this` in r0).
// Provided with the explicit receiver the call sites already pass.
extern "C" void Player_damageEmp_tail(Player *self) {
    self->updateDamageRate();
}

// stopShootSound tail: stop the looping weapon-fire FMOD event for this gun
// (veneer -> FModSound::stop(int)).
extern "C" void Player_stopShootSound_play_tail(void *sound, int id) {
    ((FModSound *)sound)->stop(id);
}

// playShootSound tail: actually start the weapon-fire FMOD event at the muzzle
// position (veneer -> FModSound::play(int, Vector*, Vector*, float)).
extern "C" void Player_playShootSound_play_tail(float vol, void *sound, int id,
                                                Vector *pos, int /*z*/) {
    ((FModSound *)sound)->play(id, pos, 0, vol);
}

// =====================================================================
// Cross-class forwarders.
//
// Level / KIPlayer call a handful of Player methods through thin
// `Player_<method>_<caller>` shims (the suffix names the spawning call
// site, e.g. _cp = createPlayer, _cs = createStation, _cm/_ccm = campaign
// mission, _cwm = create-wingman, _csc/_cso = create-static/object,
// _ag = addGroup, _cft = create-fighter). Each shim is a plain forward to
// the corresponding real Player member. They are recovered here with their
// existing C++-linkage signatures so the references resolve; the wiring pass
// rewrites the call sites to direct member calls.
// =====================================================================

void Player_setEnemies_cp(Player *p, void *arr) {
    p->setEnemies(static_cast<Array<Player *> *>(arr));
}

void Player_addEnemies_cp(Player *p, void *arr) {
    p->addEnemies(static_cast<Array<Player *> *>(arr));
}

void Player_setEnemy_cp(Player *p, Player *e) {
    p->setEnemy(e);
}

void Player_setMaxHitpoints_ag(Player *p, int hp) {
    p->setMaxHitpoints(hp);
}

void Player_setMaxHitpoints_cft(Player *p, int hp) {
    p->setMaxHitpoints(hp);
}

void Player_setAlwaysEnemy_ccm(Player *p) {
    p->setAlwaysEnemy(true);
}

void Player_setAlwaysEnemy_cm(Player *p) {
    p->setAlwaysEnemy(true);
}

void Player_setAlwaysFriend_csc(Player *p, int flag) {
    p->setAlwaysFriend(flag != 0);
}

void Player_setAlwaysFriend_cso(Player *p, int flag) {
    p->setAlwaysFriend(flag != 0);
}

void Player_setAlwaysFriend_cwm(Player *p, int flag) {
    p->setAlwaysFriend(flag != 0);
}

void Player_setVulnerable_cft(Player *player, int flag) {
    player->setVulnerable(flag != 0);
}

void Player_setPlayShootSound_ag(Player *p, int flag, int v) {
    p->setPlayShootSound(flag != 0, v);
}

void Player_setEmpData_cs(Player *p, int a, int b) {
    p->setEmpData(a, b);
}

void Player_ctor_cs(Player *p, int hp, int dmg, int a, int b, int c) {
    p->ctor(hp, dmg, a, b, c);
}

void Player_damageGamma_up(Player *p, float dmg) {
    p->damageGamma(dmg);
}

// =====================================================================
// Shared "damage-rate recompute" epilogue (`_tail`) fragments.
//
// Every HP/armor/shield/EMP mutator and the regen/damage helpers end with a
// tail-branch through one interworking veneer that resolves to the same target:
// Player::updateDamageRate() (0x72df0), with the receiver still live in r0.
// The decompiler split that shared tail off into a per-method `Player_<m>_tail`
// symbol with no receiver, so each is a documented no-op forwarder here — the
// owning method still has `this`, and the wiring pass folds every
// `return Player_<m>_tail();` back into `this->updateDamageRate();`.
// =====================================================================
extern "C" void Player_damageHull_tail()        { /* -> this->updateDamageRate() */ }
extern "C" void Player_damageShield_tail()       { /* -> this->updateDamageRate() */ }
extern "C" void Player_regenerateArmor_tail()    { /* -> this->updateDamageRate() */ }
extern "C" void Player_regenerateHull_tail()     { /* -> this->updateDamageRate() */ }
extern "C" void Player_regenerateShield_tail()   { /* -> this->updateDamageRate() */ }
extern "C" void Player_setArmorHP_tail()         { /* -> this->updateDamageRate() */ }
extern "C" void Player_setGammaHP_tail()         { /* -> this->updateDamageRate() */ }
extern "C" void Player_setHitpoints_tail()       { /* -> this->updateDamageRate() */ }
extern "C" void Player_setMaxArmorHP_tail()      { /* -> this->updateDamageRate() */ }
extern "C" void Player_setMaxEmpPoints_tail()    { /* -> this->updateDamageRate() */ }
extern "C" void Player_setMaxHitpoints_tail()    { /* -> this->updateDamageRate() */ }
extern "C" void Player_setMaxShieldHP_tail()     { /* -> this->updateDamageRate() */ }
extern "C" void Player_setShieldHP_tail()        { /* -> this->updateDamageRate() */ }

// removeAllGuns() destruction helpers: release the per-slot Gun objects held by
// the outer gun-array-array, run that container's destructor (returning the raw
// storage), then free it. Same primitives the real ~Player() uses.
extern "C" void Player_removeAllGuns_releaseClasses(void *array) {
    ArrayReleaseClasses_GunArrayArray(static_cast<Array<Array<Gun *> *> *>(array));
}
extern "C" void *Player_removeAllGuns_dtor(void *array) {
    return Array_GunArrayArray_dtor(static_cast<Array<Array<Gun *> *> *>(array));
}
extern "C" void Player_removeAllGuns_delete(void *p) {
    ::operator delete(p);
}

// Player_dtor: the demangled destructor body reused as an `operator delete`
// helper — run ~Player() on the object and hand the storage back to the caller,
// which then frees it (`::operator delete(Player_dtor(p))`). Mirrors the
// Array_*_dtor convention used elsewhere.
extern "C" void *Player_dtor(void *p) {
    static_cast<Player *>(p)->~Player();
    return p;
}

// addGun2: PlayerEgo installs a whole gun list into a slot via this alias of the
// Array<Gun*> overload of Player::addGun.
extern "C" void Player_addGun2(void *player, void *gunList, int slot) {
    static_cast<Player *>(player)->addGun(static_cast<Array<Gun *> *>(gunList), slot);
}

// render / setUnknown: virtual dispatch through the object's vtable (the call
// site is `(*(code *)<GOT slot>)(player, ...)`). There is no static Player body —
// these are pure interworking veneers into the live vtable — so they are exposed
// as forwarders the wiring pass routes to the resolved virtual method.
extern "C" void Player_render(void *player) {
    (void)player; /* virtual Player::render() — resolved via vtable */
}
extern "C" void Player_setUnknown(void *player, bool enabled) {
    (void)player; (void)enabled; /* virtual Player flag setter — resolved via vtable */
}

// =====================================================================
// KIPlayer gun-install veneers.
//
// KIPlayer::addGun_a()/addGun_b() are tiny interworking thunks that branch
// straight into the matching Player::addGun overload (Gun* vs Array<Gun*>*),
// passing the wrapped Player through r0 while the gun and slot stay live in
// r1/r2 from the KIPlayer caller. The standalone shims only carry the player,
// so they forward into the corresponding Player::addGun overload; the wiring
// pass restores the real gun/slot operands at the call site.
// =====================================================================
extern "C" void Player_addGun_a(void *player) {
    // veneer -> Player::addGun(Gun*, int) at 0x7252c
    static_cast<Player *>(player)->addGun(static_cast<Gun *>(nullptr), 0);
}
extern "C" void Player_addGun_b(void *player) {
    // veneer -> Player::addGun(Array<Gun*>*, int) at 0x72544
    static_cast<Player *>(player)->addGun(static_cast<Array<Gun *> *>(nullptr), 0);
}

// PlayerEgo explosion path: deactivate the destroyed ship. The veneer at this
// tail (DAT_001abad4) resolves to Player::setActive (0x72580); the explosion
// handler drops the player out of the active simulation.
extern "C" void Player_setActive_(int player) {
    reinterpret_cast<Player *>(static_cast<__INTPTR_TYPE__>(player))->setActive(false);
}

// LevelScript autopilot routing: the warp/station autopilot helper branches into
// the Player autopilot-target setter (veneer at DAT_001abc24 -> setAutoPilot,
// 0x728d4). There is no static Player::setAutoPilotTarget body — it is a virtual
// dispatch through the live vtable — so it is exposed as a forwarder the wiring
// pass routes to the resolved virtual method.
extern "C" void Player_setAutoPilotTarget(void *player, void *target) {
    (void)player; (void)target; /* virtual Player autopilot-target setter — resolved via vtable */
}

// =====================================================================
// PlayerEgo weapon-fire dispatch (shootPrimary / shootSecondary / shootTurret).
//
// PlayerEgo::shoot(weapon, type) splits into three calls of the Player::shoot
// family depending on weapon class:
//   - type==1  -> secondary launch: Player::shoot(kind, lockIdx, pos, flag), int
//   - turret   -> Player::shoot(2, weapon, ..., matrix) with a combined geometry
//                 matrix supplied externally (the turret aims independently of the
//                 hull transform)
//   - else     -> primary fire: Player::shoot(type, weapon, pos, flag)
// The (weapon, hi) pair is the low/high half of the 64-bit gun id/position the
// canonical Player::shoot(int,int,long long,bool) overload consumes.
// =====================================================================
extern "C" int Player_shootPrimary(void *player, int kind, int weapon, int hi, int zero) {
    (void)zero;
    long long pos = ((long long)hi << 32) | (unsigned int)weapon;
    static_cast<Player *>(player)->shoot(kind, weapon, pos, hi < 0);
    return 1;
}

extern "C" int Player_shootSecondary(void *player, int kind, int idx, int hi, int zero) {
    (void)zero;
    long long pos = ((long long)hi << 32) | (unsigned int)idx;
    static_cast<Player *>(player)->shoot(kind, idx, pos, hi < 0);
    return 1;
}

extern "C" int Player_shootTurret(void *player, int kind, int weapon, int hi,
                                  int flag, const void *matrix) {
    (void)flag;
    // Turret fire aims through an externally-combined geometry matrix rather than
    // the hull transform, so it routes through the matrix-expanding shoot1 overload.
    const int *m = static_cast<const int *>(matrix);
    static_cast<Player *>(player)->shoot1((unsigned)kind, weapon, hi,
                                          hi < 0,
                                          m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7],
                                          m[8], m[9], m[10], m[11], m[12], m[13], m[14]);
    return 1;
}

// shoot_full2: the canonical Player::shoot(int,int,long long,bool) expands the
// hull transform (fields 0x04..0x3c) into the trailing matrix arguments and tail-
// calls the matrix-taking shoot1 overload (veneer -> Player::shoot1 at 0xb3b20).
extern "C" void Player_shoot_full2(
    Player *self, int a, int b, int loC, int hiC,
    int m0, int m1, int m2, int m3, int m4, int m5, int m6, int m7,
    int m8, int m9, int m10, int m11, int m12, int m13, int m14) {
    long long pos = ((long long)hiC << 32) | (unsigned int)loC;
    (void)pos;
    self->shoot1((unsigned)a, b, hiC, loC,
                 m0, m1, m2, m3, m4, m5, m6, m7,
                 m8, m9, m10, m11, m12, m13, m14);
}

