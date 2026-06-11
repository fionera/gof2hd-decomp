#include "gof2/Player.h"
#include "gof2/Achievements.h"
#include "gof2/KIPlayer.h"
#include "gof2/GameText.h"
#include "gof2/Hud.h"
#include "gof2/PlayerEgo.h"
#include "gof2/SolarSystem.h"
#include "gof2/Standing.h"
#include "gof2/Globals.h"
#include "gof2/Gun.h"


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
extern "C" void Player_setEnemies(Player *self, Array<Player *> *enemies);
extern "C" void *Array_Player_dtor(Array<Player *> *array);
extern "C" void Player_operator_delete_tail(void *p);
extern "C" void Player_setEnemies_tail(Player *self, Array<Player *> *enemies);
extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" void Matrix_ctor(Matrix *self);
extern "C" void Array_GunArrayArray_ctor(Array<Array<Gun *> *> *array);
extern "C" void Array_Gun_ctor(Array<Gun *> *array);
extern "C" void ArraySetLength_GunArrayArray(int len, Array<Array<Gun *> *> *array);
extern "C" void ArraySetLength_Gun(int len, Array<Gun *> *array);
void MatrixGetPosition(void *out, float *matrix);
extern "C" void Vector_assign(Vector *dst, Vector *src);
extern "C" int Status_getSystem();
extern "C" int SolarSystem_getRace();
extern "C" int __aeabi_idiv(int a, int b);
extern "C" void Level_friendTurnedEnemy(int level);
extern "C" void Level_attackWanted(void *level, int a);
extern "C" void Level_alarmAllFriends(void *level, int a, bool b);
extern "C" void *Status_getStanding();
extern "C" void *Level_getPlayer();
extern "C" void Player_damageEmp_tail(Player *self);
extern "C" int **g_damageEmp_progress;
extern "C" void **g_damageEmp_achievements;
extern "C" void ArrayAdd_Gun(Gun *gun, Array<Gun *> *array);
extern "C" void Player_shoot_tail(Player *self, int soundId);
extern "C" void Player_regenerateHull_tail();
extern "C" void ArrayReleaseClasses_GunArray(Array<Gun *> *array);
extern "C" void ArrayReleaseClasses_GunArrayArray(Array<Array<Gun *> *> *array);
extern "C" void *Array_GunArray_dtor(Array<Gun *> *array);
extern "C" void *Array_GunArrayArray_dtor(Array<Array<Gun *> *> *array);
extern "C" void Player_operator_delete(void *p);
extern "C" void Player_setGammaHP_tail();
extern "C" int gStopSoundIds[];
extern "C" void *gFModSound;
extern "C" void *gFModSoundAlt;
extern "C" void Player_stopShootSound_play_tail(void *sound, int id);
extern "C" int Item_getSinglePrice(void *item);
void Globals_addSoundResourceToList(int id);
extern "C" void Player_calcWeaponSounds_tail(int a, int b);
extern "C" unsigned char FModSound_pause(void *self);
extern "C" void *gAppManager;
extern "C" void **gFModSoundPtr;
// Two engine overloads share this name; declared as C++ overloads (not extern "C")
// so overload resolution by arity/argument types selects the right one.
void *FModSound_updateEvent3DAttributes(void *sound, void *event, void *vec, Vector *pos, Vector *vel, bool b);
extern "C" void ArrayAdd_PlayerArray(Array<Player *> *src, Array<Player *> *dst);
extern "C" void Gun_setEnemies(void *gun);
extern "C" void FModSound_stop(void *self, void *event);
extern "C" int gShootSoundsByType[];
extern "C" int gShootSoundsByIndex[];
extern "C" void *gAppManagerA;
extern "C" void *gAppManagerB;
extern "C" void *gAppManagerC;
extern "C" int FModSound_isPlaying(void *sound, int id);
void FModSound_updateEvent3DAttributes(void *sound, int id, Vector *pos, Vector *vel, bool b);
extern "C" void Player_playShootSound_play_tail(float vol, void *sound, int id, Vector *pos, int z);
extern "C" void Player_playShootSound(Player *self, int type, Vector *channel, float volume);
extern "C" int Status_hardCoreMode();
extern "C" void Level_almostKillWanted(int level);
extern "C" void Level_killWanted(int level);
extern "C" void *Level_getEnemies();
extern "C" void *Status_getShip();
extern "C" int Ship_getSignatureRace(void *self);
extern "C" void *Ship_getFirstEquipmentOfSort(int self);
extern "C" void Ship_removeEquipment(void *self, void *item);
extern "C" void Standing_setPlayerSignatureRace(void *self, int r);
extern "C" int Status_inBlackMarketSystem();
extern "C" int Status_getCampaignMission();
extern "C" int String_Compare(void *a, void *b);
extern "C" void Mission_getStatusValue();
extern "C" void Mission_setStatusValue(int v);
extern "C" void Player_damage2(Player *self, int amount);
extern "C" int **g_damage_globals;
extern "C" int **g_damage_text;
extern "C" void Player_setEnemy_tail(Player *self, Player *enemy);
extern "C" unsigned char FModSound_resume(void *self, void *event);
void Player_damage_full(Player *self, int amount, int a, int b);
extern "C" void Player_stopShootSound(Player *self, int a, int b);
extern "C" void Player_setMaxArmorHP_tail();
extern "C" void Player_StopEngineSound(Player *self);
extern "C" float Vector_subF(Vector *out, Vector *a, Vector *b);
extern "C" void Vector_scale(Vector *out, float s, Vector *v);
extern "C" void Vector_div(Vector *out, Vector *v, float s);
extern "C" void FloatVectorMax(void *out, float a, float b, int c, int d);

// ---- pitchAllPrimaryGuns_a433a.cpp ----
__attribute__((minsize)) extern "C" void Player_pitchAllPrimaryGuns(Player *self, float pitch)
{
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

// ---- isAlwaysEnemy_a2a2e.cpp ----
unsigned char Player::isAlwaysEnemy() {
    Player *self = this;
    return self->alwaysEnemy;
}

// ---- setKIPlayer_a2a64.cpp ----
void Player::setKIPlayer(KIPlayer *value) {
    Player *self = this;
    self->kiPlayer = value;
}

// ---- damageHull_a2ec6.cpp ----
__attribute__((minsize)) extern "C" void Player_damageHull(Player *self, int damage)
{
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

// ---- getEnemies_a2cd2.cpp ----
extern "C" Array<Player *> *Player_getEnemies(Player *self)
{
    return self->enemies;
}

// ---- getShieldDamageRate_a2e82.cpp ----
int Player::getShieldDamageRate() {
    Player *self = this;
    return self->shieldDamageRate;
}

// ---- replaceGuns_a4360.cpp ----
int Player_replaceGuns(int a, int b, int c, int d, int e, bool f)
{
    return a;
}

// ---- getShieldHP_a2da2.cpp ----
int Player::getShieldHP() {
    Player *self = this;
    return (int)self->shieldHP;
}

// ---- removeAllGuns_a3986.cpp ----
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

// ---- getArmorDamageRate_a2e88.cpp ----
int Player::getArmorDamageRate() {
    Player *self = this;
    return self->armorDamageRate;
}

// ---- getArmorHP_a2dd6.cpp ----
int Player::getArmorHP() {
    Player *self = this;
    return self->armorHP;
}

// ---- setRadius_a2a60.cpp ----
void Player::setRadius(int value) {
    Player *self = this;
    self->radius = value;
}

// ---- resetDamageDoneByPlayer_a2f96.cpp ----
void Player::resetDamageDoneByPlayer() {
    Player *self = this;
    self->field_dc = 0;
    self->damageDoneByPlayer = 0;
    self->turnedEnemy = 0;
}

// ---- getKIPlayer_a2a6a.cpp ----
KIPlayer * Player::getKIPlayer() {
    Player *self = this;
    return self->kiPlayer;
}

// ---- isDead_a36ce.cpp ----
bool Player::isDead() {
    Player *self = this;
    return self->hitpoints < 1;
}

// ---- getRadius_a2ce0.cpp ----
int Player::getRadius() {
    Player *self = this;
    return self->radius;
}

// ---- getEmpDamageRate_a2e7c.cpp ----
int Player::getEmpDamageRate() {
    Player *self = this;
    return self->empDamageRate;
}

// ---- isAlwaysFriend_a2a28.cpp ----
unsigned char Player::isAlwaysFriend() {
    Player *self = this;
    return self->alwaysFriend;
}

// ---- getHitpoints_a2e6c.cpp ----
int Player::getHitpoints() {
    Player *self = this;
    return self->hitpoints;
}

// ---- damageShield_a2f10.cpp ----
__attribute__((minsize)) extern "C" void Player_damageShield(Player *self, int damage)
{
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

// ---- regenerateArmor_a2e44.cpp ----
void Player::regenerateArmor() {
    Player *self = this;
    int v = self->armorHP + 2;
    if (v > self->maxArmorHP) {
        v = self->maxArmorHP;
    }
    self->armorHP = v;
    return Player_regenerateArmor_tail();
}

// ---- damageShip_a2f04.cpp ----
void Player::damageShip(int damage) {
    Player *self = this;
    int v = self->hitpoints - damage;
    v &= ~(v >> 31);
    self->hitpoints = v;
}

// ---- getMaxHitpoints_a2e70.cpp ----
int Player::getMaxHitpoints() {
    Player *self = this;
    return self->maxHitpoints;
}

// ---- getGammaHP_a2de2.cpp ----
int Player::getGammaHP() {
    Player *self = this;
    return (int)self->gammaHP;
}

// ---- getBombForce_a2ea6.cpp ----
float Player::getBombForce() {
    Player *self = this;
    return self->bombForce;
}

// ---- getMaxArmorHP_a2ddc.cpp ----
int Player::getMaxArmorHP() {
    Player *self = this;
    return self->maxArmorHP;
}

// ---- getEnemy_a2cd6.cpp ----
Player * Player::getEnemy(int index) {
    Player *self = this;
    return self->enemies->data()[index];
}

// ---- turnEnemy_a36a4.cpp ----
void Player::turnEnemy() {
    Player *self = this;
    self->turnedEnemy = 1;
}

// ---- setEmpData_a2a34.cpp ----
void Player::setEmpData(int points, int data) {
    Player *self = this;
    self->empPoints = points;
    if (self->maxEmpPoints < points) {
        self->maxEmpPoints = points;
    }
    ((Player *)(self))->updateDamageRate();
    self->empData = data;
}

// ---- setVulnerable_a2e8e.cpp ----
void Player::setVulnerable(bool value) {
    Player *self = this;
    self->vulnerable = value;
}

// ---- setActive_a36e0.cpp ----
void Player::setActive(bool value) {
    Player *self = this;
    self->active = value;
}

// ---- isAsteroid_a29cc.cpp ----
__attribute__((minsize)) extern "C" bool Player_isAsteroid(Player *self)
{
    KIPlayer *ki = self->kiPlayer;
    bool result = false;
    if (ki != 0) {
        result = *((char *)ki + 0x3c) != 0;
    }
    return result;
}

// ---- updateDamageRate_a2730.cpp ----
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

// ---- setBombForce_a2e9a.cpp ----
void Player::setBombForce(float value) {
    Player *self = this;
    self->bombForce = value;
}

// ---- setMaxHitpoints_a2d6c.cpp ----
void Player::setMaxHitpoints(int value) {
    Player *self = this;
    self->maxHitpoints = value;
    self->hitpoints = value;
    return Player_setMaxHitpoints_tail();
}

// ---- getGunRegenRate_a2ec2.cpp ----
int Player::getGunRegenRate() {
    Player *self = this;
    return 0;
}

// ---- getMaxEmpPoints_a2d96.cpp ----
int Player::getMaxEmpPoints() {
    Player *self = this;
    return self->maxEmpPoints;
}

// ---- regenerateShield_a2df0.cpp ----
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

// ---- doesNeverAttack_a29fe.cpp ----
unsigned char Player::doesNeverAttack() {
    Player *self = this;
    return self->neverAttack;
}

// ---- getMaxShieldHP_a2d9c.cpp ----
int Player::getMaxShieldHP() {
    Player *self = this;
    return self->maxShieldHP;
}

// ---- isDamaged_a36da.cpp ----
unsigned char Player::isDamaged() {
    Player *self = this;
    return self->damaged;
}

// ---- isActive_a36e6.cpp ----
unsigned char Player::isActive() {
    Player *self = this;
    return self->active;
}

// ---- shoot_a3f30.cpp ----
// Duplicate of Player::shoot(int,int,long long,bool); the canonical definition (using
// Player_shoot_full2 and the 'b' argument) lives further below. Removed to avoid a
// redefinition; the leftover here also dropped 'b' and wrongly returned a value.

// ---- getEmpPoints_a2d92.cpp ----
int Player::getEmpPoints() {
    Player *self = this;
    return self->empPoints;
}

// ---- GetEngineEvent_a4108.cpp ----
int Player::GetEngineEvent() {
    Player *self = this;
    return (int)(__INTPTR_TYPE__)self->engineEvent;
}

// ---- setEmpForce_a2ea0.cpp ----
void Player::setEmpForce(float value) {
    Player *self = this;
    self->empForce = value;
}

// ---- setShootingEnabled_a29c6.cpp ----
void Player::setShootingEnabled(bool value) {
    Player *self = this;
    self->shootingEnabled = value;
}

// ---- getDamageRate_a2e76.cpp ----
int Player::getDamageRate() {
    Player *self = this;
    return self->damageRate;
}

// ---- getEmpForce_a2eac.cpp ----
float Player::getEmpForce() {
    Player *self = this;
    return self->empForce;
}

// ---- turnedEnemy_a369e.cpp ----
// Accessor lives as the non-member inline Player_turnedEnemy() in Player.h, because a
// member of this name would collide with the 'turnedEnemy' data member (field 0xe0).

// ---- gunAvailable_a36ac.cpp ----
__attribute__((minsize)) extern "C" bool Player_gunAvailable(Player *self, unsigned int slot)
{
    if (slot < 4) {
        Array<Gun *> *slotArray = self->guns->data()[slot];
        if (slotArray != 0 && slotArray->size() != 0) {
            return *(int *)slotArray->data() != 0;
        }
    }
    return false;
}

// ---- getCombinedHP_a2db0.cpp ----
int Player::getCombinedHP() {
    Player *self = this;
    return (int)(self->shieldHP + (float)self->armorHP + (float)self->hitpoints);
}

// ---- isGasCloud_a29e2.cpp ----
__attribute__((minsize)) extern "C" bool Player_isGasCloud(Player *self)
{
    KIPlayer *ki = self->kiPlayer;
    bool result = false;
    if (ki != 0) {
        result = *((char *)ki + 0x44) != 0;
    }
    return result;
}

// ---- setArmorHP_a2d1c.cpp ----
void Player::setArmorHP(int value) {
    Player *self = this;
    if (self->maxArmorHP < value) {
        value = self->maxArmorHP;
    }
    self->armorHP = value;
    return Player_setArmorHP_tail();
}

// ---- setHitpoints_a2ce4.cpp ----
void Player::setHitpoints(int value) {
    Player *self = this;
    self->hitpoints = value;
    if (self->maxHitpoints < value) {
        self->maxHitpoints = value;
    }
    return Player_setHitpoints_tail();
}

// ---- setNeverAttack_a29f8.cpp ----
void Player::setNeverAttack(bool value) {
    Player *self = this;
    self->neverAttack = value;
}

// ---- setMaxShieldHP_a2d76.cpp ----
void Player::setMaxShieldHP(int value) {
    Player *self = this;
    self->maxShieldHP = value;
    self->shieldHP = (float)value;
    return Player_setMaxShieldHP_tail();
}

// ---- setMaxEmpPoints_a2d64.cpp ----
void Player::setMaxEmpPoints(int value) {
    Player *self = this;
    self->empPoints = value;
    self->maxEmpPoints = value;
    return Player_setMaxEmpPoints_tail();
}

// ---- getGunSlots_a39a6.cpp ----
int Player::getGunSlots() {
    Player *self = this;
    return 3;
}

// ---- getHitVector_a2eb2.cpp ----
// Local POD for the returned 3-float hit vector (packed as a double xy + float z).
// Named HitVec3 to avoid colliding with PlayerEgo.h's `typedef Vector Vec3`.
struct HitVec3 {
    double xy;
    float z;
};

void Player_getHitVector(HitVec3 *out, Player *self)
{
    double xy = *(double *)self->hitVector;
    out->z = self->hitVector[2];
    out->xy = xy;
}

// ---- setPlayShootSound_a2a56.cpp ----
void Player::setPlayShootSound(bool play, int id) {
    Player *self = this;
    self->playShootSound = play;
    self->playShootSoundId = id;
}

// ---- regenerateShield_a2e1a.cpp ----
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

// ---- heal_a42dc.cpp ----
__attribute__((minsize)) extern "C" void Player_heal(Player *self, float amount)
{
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

// ---- setShieldHP_a2cf6.cpp ----
__attribute__((minsize)) extern "C" void Player_setShieldHP(Player *self, int value)
{
    float maxF = (float)self->maxShieldHP;
    self->shieldHP = (float)value;
    if ((float)value > maxF) {
        self->shieldHP = maxF;
    }
    return Player_setShieldHP_tail();
}

// ---- refillGunDelay_a39dc.cpp ----
__attribute__((minsize)) extern "C" void Player_refillGunDelay(Player *self, int slot)
{
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

// ---- addEnemies_a2be2.cpp ----


__attribute__((minsize)) extern "C" void Player_addEnemies(Player *self, Array<Player *> *enemies)
{
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
    Player_setEnemies(self, tmp);
    Array_Player_dtor(tmp);
    return Player_operator_delete_tail(tmp);
}

// ---- Player_a2570.cpp ----



Player * Player::ctor(int radius, int hitpoints, int numPrimary, int numSecondary, int numTertiary) {
    Player *self = this;
    void * volatile cookie = __stack_chk_guard;
    Matrix_ctor((Matrix *)self->transform);
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
    Vector_assign((Vector *)self->position, (Vector *)tmp);
    self->field_f4 = (void *)(__INTPTR_TYPE__)-1;

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta == 0) {
        return self;
    }
    __stack_chk_fail(guardDelta);
}

// ---- getPosition_a2858.cpp ----
__attribute__((minsize)) extern "C" void Player_getPosition(void *out, Player *self)
{
    MatrixGetPosition(out, self->transform);
}

// ---- damageGamma_a2f62.cpp ----
__attribute__((minsize)) extern "C" float Player_damageGamma(Player *self, float amount)
{
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

// ---- damageEmp_a2fa4.cpp ----
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
            int sys = Status_getSystem();
            ki = (char *)self->kiPlayer;
            if (sys != 0 && *(char *)(ki + 0x42) != 0) {
                if (amount > 0) {
                    Level_attackWanted(*(void **)(ki + 0x54), *(int *)(ki + 0x48));
                }
                runLab = false;
            } else if (self->kiPlayer == 0) {
                runLab = false;
            }
        }
        // LAB_000b3016
        if (runLab && self->alwaysEnemy == 0 && ((KIPlayer *)self->kiPlayer)->isWingMan() == 0 &&
            (unsigned int)(*(int *)((char *)self->kiPlayer + 0x28) - 9) > 1 &&
            Status_getSystem() != 0) {
            int race = *(int *)((char *)self->kiPlayer + 0x28);
            Status_getSystem();
            if (race == SolarSystem_getRace()) {
                int prev = self->field_dc;
                self->field_dc = prev + amount;
                if (__aeabi_idiv(self->maxEmpPoints, 3) < prev + amount) {
                    self->turnedEnemy = 1;
                    Level_friendTurnedEnemy(*(int *)((char *)self->kiPlayer + 0x54));
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
            Status_getSystem() != 0) {
            int race = *(int *)((char *)self->kiPlayer + 0x28);
            Status_getSystem();
            if (race == SolarSystem_getRace()) {
                Level_alarmAllFriends(*(void **)((char *)self->kiPlayer + 0x54),
                                      *(int *)((char *)self->kiPlayer + 0x28), false);
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
            void *st = Status_getStanding();
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
                    void *ego = Level_getPlayer();
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

// ---- addGun_a3910.cpp ----


__attribute__((minsize)) void Player_addGun(Player *self, Array<Gun *> *guns, int slot)
{
    if (self->guns != 0) {
        if ((unsigned int)slot < 4) {
            Array<Gun *> *arr = static_cast<Array<Gun *> *>(operator new(sizeof(Array<Gun *>)));
            Array_Gun_ctor(arr);
            self->guns->data()[slot] = arr;
            for (unsigned int i = 0; i < guns->size(); i++) {
                ArrayAdd_Gun(guns->data()[i], self->guns->data()[slot]);
            }
        }
        if (self->playShootSound) {
            return Player_shoot_tail(self, self->playShootSoundId);
        }
    }
}

// ---- setAlwaysEnemy_a2a04.cpp ----
__attribute__((minsize)) extern "C" void Player_setAlwaysEnemy(Player *self, bool value)
{
    self->alwaysEnemy = value;
    self->enemyFlags = 1;
    self->turnedEnemy = 1;
}

// ---- regenerateHull_a2e58.cpp ----
void Player::regenerateHull() {
    Player *self = this;
    int v = self->maxHitpoints;
    if (self->hitpoints + 1 < self->maxHitpoints) {
        v = self->hitpoints + 1;
    }
    self->hitpoints = v;
    return Player_regenerateHull_tail();
}

// ---- _Player_a2864.cpp ----
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
                    Player_operator_delete(Array_GunArray_dtor(s2));
                    this->guns->data()[i] = 0;
                }
                guns = this->guns;
            }
        }
        ArrayReleaseClasses_GunArrayArray(guns);
        if (this->guns != 0) {
            Player_operator_delete(Array_GunArrayArray_dtor(this->guns));
        }
        this->guns = 0;
    }
    if (this->enemies != 0) {
        Player_operator_delete(Array_Player_dtor(this->enemies));
    }
    this->enemies = 0;
}

// ---- setGammaHP_a2d30.cpp ----
__attribute__((minsize)) extern "C" void Player_setGammaHP(Player *self, int value)
{
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

// ---- stopShootSound_a3a14.cpp ----
__attribute__((minsize)) extern "C" void Player_stopShootSound(Player *self, int index, int channel)
{
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

// ---- reset_a2a70.cpp ----
__attribute__((minsize)) extern "C" void Player_reset(Player *self)
{
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

// ---- addGun_a36ec.cpp ----


__attribute__((minsize)) void Player_addGun(Player *self, Gun *gun, int slot)
{
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

// ---- calcWeaponSounds_a3774.cpp ----


extern "C" int *g_cws_items;   // DAT_b37da  -> item table (GOT pointer)
extern "C" int *g_cws_sound;    // DAT_b3870  -> sound id holder (deref once)
extern "C" int *g_cws_sound2;   // DAT_b38d8 (deref once)
extern "C" int *g_cws_sound3;   // DAT_b38dc (array base)

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
                int a = Item_getSinglePrice(*(void **)&dataArr[order[i - 1]]);
                dataArr = *(int **)((char *)table + 4);
                int b = Item_getSinglePrice(*(void **)&dataArr[order[i]]);
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

// ---- PauseEngineSound_a4088.cpp ----
__attribute__((minsize)) extern "C" void Player_PauseEngineSound(Player *self)
{
    void *event = self->engineEvent;
    if (event != 0) {
        self->field_f8 = FModSound_pause(gFModSound);
    }
}

// ---- PlayEngineSound_a4014.cpp ----
struct Mat { float m[12]; };

__attribute__((minsize)) extern "C" void Player_PlayEngineSound(Player *self, Vector *vec)
{
    self->field_f4 = vec;
    if (*((char *)gAppManager + 0xf) != 0) {
        Mat pos;
        MatrixGetPosition(&pos, self->transform);
        void *ev = FModSound_updateEvent3DAttributes(
            gFModSoundPtr[0],
            self->engineEvent,
            self->field_f4,
            (Vector *)&pos, 0, true);
        self->engineEvent = ev;
        self->field_108 = 1;
    }
}

// ---- setEnemies_a2acc.cpp ----


__attribute__((minsize)) extern "C" void Player_setEnemies(Player *self, Array<Player *> *enemies)
{
    if (self->enemies != 0) {
        Player_operator_delete(Array_Player_dtor(self->enemies));
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

// ---- StopEngineSound_a40e0.cpp ----
__attribute__((minsize)) extern "C" void Player_StopEngineSound(Player *self)
{
    void *event = self->engineEvent;
    if (event != 0) {
        FModSound_stop(gFModSound, event);
        self->field_108 = 0;
        self->engineEvent = 0;
    }
}

// ---- playShootSound_a3a64.cpp ----
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
        if (FModSound_isPlaying(sound, soundId) != 0) {
            if (*((char *)gAppManagerA + 0xf) != 0) {
                FModSound_updateEvent3DAttributes(fmodPtr[0], soundId, pos, 0, false);
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

// ---- shoot_a3b20.cpp ----

extern "C" const float k_shootAt_inc;   // DAT_b3cb4

void Player::shoot1(unsigned int slot, int idLo, int idHi, int flag, int m0, int m1, int m2, int m3, int m4, int m5, int m6, int m7, int m8, int m9, int m10, int m11, int m12, int m13, int m14) {
    Player *self = this;
    void * volatile cookie = __stack_chk_guard;
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

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0) {
        __stack_chk_fail(guardDelta);
    }
}

// ---- damage_a31c0.cpp ----
extern "C" const float k_damage_full;   // DAT_b3518
extern "C" const float k_damage_hc;     // DAT_b351c
extern "C" const float k_damage_full2;  // DAT_b367c
extern "C" const float k_damage_hc2;    // DAT_b3680
extern "C" const float k_damage_regen;  // DAT_b3684

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
            Status_getSystem() != 0 &&
            ((self->enemyFlags == 0) || (self->turnedEnemy != 0))) {
            ki = (char *)self->kiPlayer;
            if (*(char *)(ki + 0x42) != 0) {
                if (amount > 0) {
                    self->damageDoneByPlayer = self->damageDoneByPlayer + amount;
                    Level_attackWanted(*(void **)(ki + 0x54), *(int *)(ki + 0x48));
                }
                goto LAB_3488;
            }
        } else {
            ki = (char *)self->kiPlayer;
        }

        if (ki != 0 && self->alwaysEnemy == 0 &&
            (unsigned int)(*(int *)(ki + 0x28) - 9) > 1 &&
            ((KIPlayer *)self->kiPlayer)->isWingMan() == 0 && Status_getSystem() != 0 &&
            ((self->enemyFlags == 0) || (self->turnedEnemy != 0))) {
            int race = *(int *)((char *)self->kiPlayer + 0x28);
            Status_getSystem();
            bool sameRace = (race == SolarSystem_getRace());
            if (!sameRace) {
                int race2 = *(int *)((char *)self->kiPlayer + 0x28);
                void *sys = (void *)(long)Status_getSystem();
                if (race2 != ((SolarSystem *)(sys))->getAttackRace()) {
                    goto LAB_342a;
                }
            }
            self->damageDoneByPlayer = self->damageDoneByPlayer + amount;
            int hc = Status_hardCoreMode();
            float thr1 = (hc != 0) ? k_damage_hc : k_damage_full;
            float f1 = thr1 * (float)self->maxHitpoints;
            float dmgF = (float)self->damageDoneByPlayer;
            float frac2 = (hc != 0) ? 0.25f : 0.5f;
            float thr3 = (hc != 0) ? k_damage_hc2 : k_damage_full2;

            if (f1 < dmgF) {
                Level_friendTurnedEnemy(*(int *)((char *)self->kiPlayer + 0x54));
                void *ship = Status_getShip();
                void *standing = Status_getStanding();
                if (Ship_getSignatureRace(ship) >= 0) {
                    bool match = ((unsigned int)Ship_getSignatureRace(ship) ==
                                  *(unsigned int *)((char *)self->kiPlayer + 0x28));
                    unsigned int dis = 0;
                    if (match) dis = *(unsigned char *)((char *)self->kiPlayer + 0x42);
                    if (match && dis == 0) {
                        void *item = Ship_getFirstEquipmentOfSort((int)(long)ship);
                        Ship_removeEquipment(ship, item);
                        void *st2 = Status_getStanding();
                        ((Standing *)(st2))->applyDelict(Ship_getSignatureRace(ship), 100);
                        Standing_setPlayerSignatureRace(standing, -1);
                        void *ego = Level_getPlayer();
                        int hud = (int)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getHUD();
                        int p = (int)(long)Level_getPlayer();
                        ((Hud *)(hud))->hudEvent(0x1f, (void *)(__INTPTR_TYPE__)p, 0);
                    }
                }
            }

            float f3 = (float)self->maxHitpoints;
            float dmgF3 = (float)self->damageDoneByPlayer;
            if (frac2 * f3 < dmgF3) {
                void *ship = Status_getShip();
                void *standing = Status_getStanding();
                if (Ship_getSignatureRace(ship) >= 0 &&
                    *(int *)((char *)self->kiPlayer + 0x28) < 4 &&
                    *(char *)((char *)self->kiPlayer + 0x42) == 0) {
                    void *item = Ship_getFirstEquipmentOfSort((int)(long)ship);
                    Ship_removeEquipment(ship, item);
                    void *st2 = Status_getStanding();
                    ((Standing *)(st2))->applyDelict(Ship_getSignatureRace(ship), 100);
                    Standing_setPlayerSignatureRace(standing, -1);
                    void *ego = Level_getPlayer();
                    int hud = (int)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getHUD();
                    int p = (int)(long)Level_getPlayer();
                    ((Hud *)(hud))->hudEvent(0x1f, (void *)(__INTPTR_TYPE__)p, 0);
                }
                self->turnedEnemy = 1;
            }

            float f4a = (float)self->maxHitpoints;
            float dmgF4 = (float)self->damageDoneByPlayer;
            if (thr3 * f4a < dmgF4) {
                Level_alarmAllFriends(*(void **)((char *)self->kiPlayer + 0x54),
                                      *(int *)((char *)self->kiPlayer + 0x28), true);
            }
            goto LAB_3488;
        }
    }

LAB_342a:
    {
        Globals *globals = (Globals *)*g_damage_globals;
        if (Status_inBlackMarketSystem() != 0) {
            char *ki = (char *)self->kiPlayer;
            if (ki != 0 && *(int *)(ki + 0x28) == 8) {
                self->turnedEnemy = 1;
                Level_alarmAllFriends(*(void **)(ki + 0x54), 8, true);
                int *enemies = (int *)Level_getEnemies();
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
                Level_almostKillWanted(*(int *)(ki + 0x54));
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
                    Status_inBlackMarketSystem() == 0) {
                    if (*(char *)((char *)self->kiPlayer + 0x42) == 0) {
                        void *st = Status_getStanding();
                        ((Standing *)(st))->applyKill(*(int *)((char *)self->kiPlayer + 0x28));
                    }
                    int mission = Status_getCampaignMission();
                    char *ki3 = (char *)self->kiPlayer;
                    // NOTE: original text-table key for this name lookup was dropped by the
                    // decompiler; using missionId (the only contextual id) to keep it compiling.
                    void *txt = ((GameText *)(*g_damage_text[0]))->getText(missionId);
                    int cmp = String_Compare((void *)(ki3 + 0x18), txt);
                    if (missionId == 0xb3 && cmp == 0) {
                        Mission_getStatusValue();
                        Mission_setStatusValue(mission);
                    }
                    if (*(char *)((char *)self->kiPlayer + 0x42) != 0) {
                        Level_killWanted(*(int *)((char *)self->kiPlayer + 0x54));
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
            Player_damage2(other, amount);
            *(char *)(*(int *)(*(int *)((char *)self->kiPlayer + 0x10) + 4) + 0xc2) = 0;
        }
    }
}

// ---- setEnemy_a2b76.cpp ----


__attribute__((minsize)) extern "C" void Player_setEnemy(Player *self, Player *enemy)
{
    Array<Player *> *tmp = static_cast<Array<Player *> *>(operator new(sizeof(Array<Player *>)));
    Array_Player_ctor(tmp);
    ArrayAdd_Player(enemy, tmp);
    Player_setEnemies(self, tmp);
    Array_Player_dtor(tmp);
    return Player_operator_delete_tail(tmp);
}

// ---- addEnemy_a2c6a.cpp ----


__attribute__((minsize)) extern "C" void Player_addEnemy(Player *self, Player *enemy)
{
    if (self->enemies == 0) {
        return Player_setEnemy_tail(self, enemy);
    }
    Array<Player *> *tmp = static_cast<Array<Player *> *>(operator new(sizeof(Array<Player *>)));
    Array_Player_ctor(tmp);
    if (self->enemies->size() != 0) {
        ArrayAdd_PlayerArray(self->enemies, tmp);
    }
    ArrayAdd_Player(enemy, tmp);
    Player_setEnemies(self, tmp);
    Array_Player_dtor(tmp);
    return Player_operator_delete_tail(tmp);
}

// ---- ResumeEngineSound_a40ac.cpp ----
__attribute__((minsize)) extern "C" void Player_ResumeEngineSound(Player *self, bool force)
{
    void *event = self->engineEvent;
    if (event != 0 && (self->field_f8 != 0 || force)) {
        self->field_f8 = FModSound_resume(gFModSound, event) ^ 1;
    }
}

// ---- damage_a3694.cpp ----
void Player::damage(int amount) {
    Player *self = this;
    return Player_damage_full(self, amount, 0, -1);
}

// ---- stopShooting_a3d08.cpp ----
__attribute__((minsize)) void Player_stopShooting(Player *self, int slot, int channel)
{
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
        Player_stopShootSound(self, gun->itemIndex, gun->weaponType);
        arr = self->guns->data()[slot];
    }
}

// ---- setAlwaysFriend_a2a14.cpp ----
__attribute__((minsize)) extern "C" void Player_setAlwaysFriend(Player *self, bool value)
{
    self->alwaysFriend = value;
    self->enemyFlags = 0x100;
    self->turnedEnemy = 0;
}

// ---- setMaxArmorHP_a2d8a.cpp ----
void Player::setMaxArmorHP(int value) {
    Player *self = this;
    self->armorHP = value;
    self->maxArmorHP = value;
    return Player_setMaxArmorHP_tail();
}

// ---- shoot_a3d5c.cpp ----

extern "C" unsigned int g_shoot_mask;   // DAT_b3f24
extern "C" const float k_shoot_inc;      // DAT_b3f28

int Player::shoot2(unsigned int slot, int gunId, int a4_00, int flag, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, int a20, int a21, int a22) {
    Player *self = this;
    void * volatile cookie = __stack_chk_guard;
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

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0) {
        __stack_chk_fail(guardDelta);
    }
    return retval;
}

// ---- update_a4110.cpp ----
extern "C" int **g_update_clock;       // DAT_b41a0
extern "C" char **g_update_flag;       // DAT_b41be
extern "C" float **g_update_speed;     // DAT_b41e0
extern "C" void (**g_update_transform)(void *, void *, int);  // DAT_b41e2 fn ptr table
extern "C" void **g_update_sound;      // DAT_b423c
extern "C" const float k_update_a;     // DAT_b42b8
extern "C" const float k_update_b;     // DAT_b42bc
extern "C" const float k_update_c;     // DAT_b42c0

Vector * Player::update(int dt, int doSound) {
    Player *self = this;
    void * volatile cookie = __stack_chk_guard;

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
        float d = (float)Vector_subF((Vector *)tmpA, (Vector *)self->position, (Vector *)local);
        Vector_scale((Vector *)tmpB, d, (Vector *)tmpA);
        Vector_div((Vector *)tmpC, (Vector *)tmpB, (float)dt);
        (void)spd;
        fn(tmpB, local, (int)(long)transform);
        void *ev = FModSound_updateEvent3DAttributes(*g_update_sound, self->engineEvent,
                                                   self->field_f4, (Vector *)tmpB, (Vector *)tmpC, false);
        self->engineEvent = ev;
        self->field_108 = 1;
        fn(tmpA, local, (int)(long)transform);
        Vector_assign((Vector *)self->position, (Vector *)tmpA);
    }

    float speed = (float)dt;
    float nf = *(float *)&self->field_60 + speed * k_update_a * k_update_b;
    *(float *)&self->field_60 = nf;
    FloatVectorMax(&result, nf, k_update_c, 2, 0x20);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0) {
        __stack_chk_fail(guardDelta);
    }
    return result;
}

// ---- shoot_a3f9e.cpp ----
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

// ---- stopShooting_a3cbc.cpp ----
__attribute__((minsize)) void Player_stopShooting(Player *self, int slot)
{
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
        Player_stopShootSound(self, gun->itemIndex, gun->weaponType);
        arr = self->guns->data()[slot];
    }
}

// ---- setHitVector_a2e94.cpp ----
float * Player::setHitVector(float x, float y, float z) {
    Player *self = this;
    float *p = self->hitVector;
    *p++ = x;
    *p++ = y;
    *p++ = z;
    return p;
}

// ---- resetGunDelay_a39aa.cpp ----
__attribute__((minsize)) extern "C" void Player_resetGunDelay(Player *self, int slot)
{
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
