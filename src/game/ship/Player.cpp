#include "game/ship/Player.h"
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
#include <new>

// Free math operators/functions (defined in libgof2hd; not declared in math.h).
namespace AbyssEngine { namespace AEMath {
    Vector operator-(const Vector&, const Vector&);
    Vector operator*(float, const Vector&);
    float VectorLength(const Vector&);
} }

// Engine/game globals (defined elsewhere in the runtime).
extern int * g_cws_items;
extern int * g_cws_sound;
extern int * g_cws_sound2;
extern int * g_cws_sound3;
extern int ** g_damage_text;
extern unsigned int g_shoot_mask;
extern void ** g_update_sound;
extern float ** g_update_speed;

void MatrixGetPosition(void *out, float *matrix);
extern "C" int __aeabi_idiv(int a, int b);
extern "C" int gStopSoundIds[];
extern "C" void *gFModSound;
extern "C" void *gFModSoundAlt;
extern "C" void *gAppManager;
extern "C" void **gFModSoundPtr;
extern "C" void Gun_setEnemies(void *gun);
extern "C" int gShootSoundsByType[];
extern "C" int gShootSoundsByIndex[];
extern "C" void *gAppManagerA;
extern "C" void *gAppManagerB;
extern "C" void *gAppManagerC;
extern "C" void Player_StopEngineSound(Player *self);
extern "C" void FloatVectorMax(void *out, float a, float b, int c, int d);

void Player::pitchAllPrimaryGuns(float pitch) {
    if (this->guns != 0) {
        Array<Gun *> *prim = this->guns->data()[0];
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

int Player::replaceGuns(int a, int b, int c, int d, int e, bool f) {
    return a;
}

int Player::getShieldHP() {
    return (int)this->shieldHP;
}

void Player::removeAllGuns() {
    if (this->guns != 0) {
        for (Array<Gun *> *slot : *this->guns) delete slot;
        this->guns->clear();
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

KIPlayer * Player::getKIPlayer() {
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
        s = s - (float)damage;
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
    return (int)this->gammaHP;
}

float Player::getBombForce() {
    return this->bombForce;
}

int Player::getMaxArmorHP() {
    return this->maxArmorHP;
}

Player * Player::getEnemy(int index) {
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
    ((Player *)(this))->updateDamageRate();
    this->empData = data;
}

void Player::setVulnerable(bool value) {
    this->vulnerable = value;
}

void Player::setActive(bool value) {
    this->active = value;
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
    KIPlayer *ki = this->kiPlayer;
    bool result = false;
    if (ki != 0) {
        result = ki->stealFlag != 0;
    }
    return result;
}

void Player::updateDamageRate() {
    float maxHp = (float)this->maxHitpoints;
    float hp = (float)this->hitpoints;
    float maxArmor = (float)this->maxArmorHP;
    float armor = (float)this->armorHP;
    float maxEmp = (float)this->maxEmpPoints;
    float maxShield = (float)this->maxShieldHP;
    float emp = (float)this->empPoints;

    float shieldRate = (this->shieldHP / maxShield) * 100.0f;
    float armorRate = (armor / maxArmor) * 100.0f;
    float empRate = (emp / maxEmp) * 100.0f;

    this->damageRate = (int)((hp / maxHp) * 100.0f);
    this->shieldDamageRate = (int)shieldRate;
    this->armorDamageRate = (int)armorRate;
    this->empDamageRate = (int)empRate;
}

void Player::setBombForce(float value) {
    this->bombForce = value;
}

void Player::setMaxHitpoints(int value) {
    this->maxHitpoints = value;
    this->hitpoints = value;
    this->updateDamageRate();
}

int Player::getGunRegenRate() {
    return 0;
}

int Player::getGunRegenRate(int slot) {
    (void)slot;
    return 0;
}

int Player::getMaxEmpPoints() {
    return this->maxEmpPoints;
}

void Player::regenerateShield(float amount) {
    float f = this->shieldHP + amount;
    float maxF = (float)this->maxShieldHP;
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

// Duplicate of Player::shoot(int,int,long long,bool); the canonical definition (using
// Player_shoot_full2 and the 'b' argument) lives further below. Removed to avoid a
// redefinition; the leftover here also dropped 'b' and wrongly returned a value.

int Player::getEmpPoints() {
    return this->empPoints;
}

int Player::GetEngineEvent() {
    return (int)(__INTPTR_TYPE__)this->engineEvent;
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

// Accessor for the "turned hostile this encounter" flag (field 0xe0).
unsigned char Player::turnedEnemy() {
    return this->turnedEnemyFlag;
}

bool Player::gunAvailable(int slot) {
    if (slot < 4) {
        Array<Gun *> *slotArray = this->guns->data()[slot];
        if (slotArray != 0 && slotArray->size() != 0) {
            return *(int *)slotArray->data() != 0;
        }
    }
    return false;
}

int Player::getCombinedHP() {
    return (int)(this->shieldHP + (float)this->armorHP + (float)this->hitpoints);
}

bool Player::isGasCloud() {
    KIPlayer *ki = this->kiPlayer;
    bool result = false;
    if (ki != 0) {
        result = ki->field_0x44 != 0;
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
    this->shieldHP = (float)value;
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

// Local POD for the returned 3-float hit vector (packed as a double xy + float z).
// Named HitVec3 to avoid colliding with PlayerEgo.h's `typedef Vector Vec3`.
struct HitVec3 {
    double xy;
    float z;
};

void Player::getHitVector(Vector *out) {
    double xy = *(double *)this->hitVector;
    out->z = this->hitVector[2];
    *(double *)&out->x = xy;
}

Vector Player::getHitVector() {
    Vector out;
    double xy = *(double *)this->hitVector;
    out.z = this->hitVector[2];
    *(double *)&out.x = xy;
    return out;
}

void Player::setPlayShootSound(bool play, int id) {
    this->playShootSoundFlag = play;
    this->playShootSoundId = id;
}

void Player::regenerateShield() {
    float f = this->shieldHP + 1.0f;
    float maxF = (float)this->maxShieldHP;
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
        int count = (int)f;
        for (int i = 0; i < count; i++) {
            ((Player *)(this))->regenerateHull();
        }
        this->healAccumulator = this->healAccumulator - (float)count;
    }
}

void Player::setShieldHP(int value) {
    float maxF = (float)this->maxShieldHP;
    this->shieldHP = (float)value;
    if ((float)value > maxF) {
        this->shieldHP = maxF;
    }
    this->updateDamageRate();
}

void Player::refillGunDelay(int slot) {
    Array<Array<Gun *> *> *guns = this->guns;
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
    if (this->enemies == 0) {
        this->setEnemies(enemies);
        return;
    }
    Array<Player *> *tmp = new Array<Player *>();
    for (unsigned int i = 0; i < this->enemies->size(); i++) {
        tmp->push_back(this->enemies->data()[i]);
    }
    for (unsigned int i = 0; i < enemies->size(); i++) {
        tmp->push_back(enemies->data()[i]);
    }
    ((Player *)(this))->setEnemies(tmp);
    delete tmp;
}

Player::Player(int radius, int hitpoints, int numPrimary, int numSecondary, int numTertiary) {
    Player *self = this;
    // transform[] is the raw storage of the 3x4 matrix at +0x04; default-init it in place.
    *reinterpret_cast<AbyssEngine::AEMath::Matrix *>(self->transform) = AbyssEngine::AEMath::Matrix();
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
    self->flShake = 0.0f;
    self->bombForce = 0.0f;
    self->empForce = 0.0f;
    ((Player *)(self))->updateDamageRate();
    self->field_58 = -1;

    Array<Array<Gun *> *> *gunArr = new Array<Array<Gun *> *>();
    self->guns = gunArr;
    gunArr->resize(3);

    if (numPrimary < 1) {
        self->guns->data()[0] = 0;
    } else {
        Array<Gun *> *a = new Array<Gun *>();
        self->guns->data()[0] = a;
        self->guns->data()[0]->resize(numPrimary);
    }
    if (numSecondary < 1) {
        self->guns->data()[1] = 0;
    } else {
        Array<Gun *> *a = new Array<Gun *>();
        self->guns->data()[1] = a;
        self->guns->data()[1]->resize(numSecondary);
    }
    if (numTertiary < 1) {
        self->guns->data()[2] = 0;
    } else {
        Array<Gun *> *a = new Array<Gun *>();
        self->guns->data()[2] = a;
        self->guns->data()[2]->resize(numTertiary);
    }

    self->playShootSoundFlag = 1;
    self->playShootSoundId = 1;
    self->destroyed = 0;
    self->active = 1;
    self->damageTimer = 0;
    self->vulnerable = 1;
    self->kiPlayer = 0;
    self->bombForce = 0.0f;
    self->enemyFlags = 0;
    self->turnedEnemyFlag = 0;
    self->alwaysEnemy = 0;
    self->alwaysFriend = 0;
    self->enemies = 0;
    self->field_54 = 0;
    self->empDisabled = 0;
    self->neverAttack = 0;
    self->healAccumulator = 0;
    self->engineEvent = 0;
    self->engineSoundPlaying = 0;
    self->enginePaused = 0;
    self->shieldHit = 0;
    self->armorHit = 0;
    self->hullHit = 0;
    self->gammaHit = 0;

    float tmp[3];
    MatrixGetPosition(tmp, self->transform);
    *(Vector *)self->position = *(Vector *)tmp;
    self->enginePositionVec = (void *)(__INTPTR_TYPE__)-1;
}

void Player::getPosition(Vector *out) {
    MatrixGetPosition(out, this->transform);
}

Vector Player::getPosition() {
    Vector out;
    MatrixGetPosition(&out, this->transform);
    return out;
}

float Player::damageGamma(float amount) {
    if (this->vulnerable) {
        if (this->active) {
            amount = this->gammaHP - amount;
            this->gammaHit = 1;
            this->gammaHP = amount;
            if (!(amount > 0.0f)) {
                this->gammaHP = 0;
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
        KIPlayer *ki = self->kiPlayer;
        bool runLab = true;
        if ((unsigned int)(ki->shipGroup - 9) >= 2) {
            int sys = gStatus->getSystem();
            ki = self->kiPlayer;
            if (sys != 0 && ki->field_0x42 != 0) {
                if (amount > 0) {
                    ((Level *)(ki->level))->attackWanted(ki->field_0x48);
                }
                runLab = false;
            } else if (self->kiPlayer == 0) {
                runLab = false;
            }
        }
        // LAB_000b3016
        if (runLab && self->alwaysEnemy == 0 && self->kiPlayer->isWingMan() == 0 &&
            (unsigned int)(self->kiPlayer->shipGroup - 9) > 1 &&
            gStatus->getSystem() != 0) {
            int race = self->kiPlayer->shipGroup;
            gStatus->getSystem();
            if (race == ((SolarSystem *)(intptr_t)gStatus->getSystem())->getRace()) {
                int prev = self->field_dc;
                self->field_dc = prev + amount;
                if (__aeabi_idiv(self->maxEmpPoints, 3) < prev + amount) {
                    self->turnedEnemyFlag = 1;
                    ((Level *)(self->kiPlayer->level))->friendTurnedEnemy(0);
                }
            }
        }
    }

    {
        int ep = self->empPoints - amount;
        self->empPoints = ep;
        if (ep > 0) {
            self->updateDamageRate();
            return;
        }
    }
    if (!flag && self->kiPlayer != 0) {
        if (self->alwaysEnemy == 0 &&
            (unsigned int)(self->kiPlayer->shipGroup - 9) > 1 &&
            gStatus->getSystem() != 0) {
            int race = self->kiPlayer->shipGroup;
            gStatus->getSystem();
            if (race == ((SolarSystem *)(intptr_t)gStatus->getSystem())->getRace()) {
                ((Level *)(self->kiPlayer->level))->alarmAllFriends(self->kiPlayer->shipGroup, false);
            }
        }
        KIPlayer *ki = self->kiPlayer;
        if (ki->stealFlag != 0) {
            goto lab_30e2;
        }
        if (*(char *)((char *)ki + 0x3d) != 0) {  // RAWREAD: ki+0x3d (no KIPlayer member at +0x3d; byte inside stealFlag int at 0x3c)
            goto lab_30f4;
        }
        if (ki->field_0x42 != 0) {
            goto lab_30f4;
        }
        {
            void *st = (void *)(long)gStatus->getStanding();
            ((Standing *)(st))->applyDisable(self->kiPlayer->shipGroup);
            ki = self->kiPlayer;
        }
    lab_30e2:
        if (ki == 0) {
            goto lab_3164;
        }
    lab_30f4:
        if (self->empDisabled != 0) {
            goto lab_3164;
        }
        {
            if ((unsigned int)gStatus->field_134 > 0x7fffffff) {
                gStatus->field_134 = 0;
            }
            if (gAchievements->hasMedal(0x2a, 1) == 0) {
                int cnt = gStatus->field_134 + 1;
                gStatus->field_134 = cnt;
                if (gAchievements->getValue(0x2a, 1) <= cnt) {
                    void *ego = (void *)(__INTPTR_TYPE__)((Level *)(self->kiPlayer->level))->getPlayer();
                    void *hud = (void *)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getHUD();
                    ((Hud *)(hud))->hudEventMedal(0x2a, 100);
                    gStatus->field_138 = 1;
                }
            }
        }
    }

lab_3164:
    // LAB_000b3164
    float f = (float)self->empData;
    self->empDisabled = 1;
    self->empPoints = 0;
    (self->pad_e8[0]) = 0;
    self->empForce = f;
    self->updateDamageRate();
        return;
}

void Player::addGun(Array<Gun *> *gunsIn, int slot) {
    if (this->guns != 0) {
        if ((unsigned int)slot < 4) {
            Array<Gun *> *arr = new Array<Gun *>();
            this->guns->data()[slot] = arr;
            for (unsigned int i = 0; i < gunsIn->size(); i++) {
                this->guns->data()[slot]->push_back(gunsIn->data()[i]);
            }
        }
        if (this->playShootSoundFlag) {
            this->calcWeaponSounds(this->playShootSoundId);
            return;
        }
    }
}

void Player::setAlwaysEnemy(bool value) {
    this->alwaysEnemy = value;
    this->enemyFlags = 1;
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
                for (Gun *gun : *slot) delete gun;
                slot->clear();
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
        for (Array<Gun *> *slot : *guns) delete slot;
        guns->clear();
        delete this->guns;
        this->guns = 0;
    }
    if (this->enemies != 0) {
        delete this->enemies;
    }
    this->enemies = 0;
}

void Player::setGammaHP(int value) {
    float f = (float)value;
    float sel = f;
    if (value != 9999999) {
        sel = 100.0f;
    }
    if (value > 100) {
        f = sel;
    }
    this->gammaHP = f;
    this->updateDamageRate();
}

void Player::stopShootSound(int index, int channel) {
    if ((unsigned int)channel > 8) {
        return;
    }
    if (((1 << channel) & 0x10c) != 0) {
        void *sound;
        int id;
        if (this->kiPlayer != 0 && this->kiPlayer->shipGroup == 9) {
            id = 0x3e;
            sound = gFModSoundAlt;
        } else {
            id = gStopSoundIds[index];
            sound = gFModSound;
        }
        ((FModSound *)sound)->stop(id);
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
    self->destroyed = 0;
    self->damageDoneByPlayer = 0;
    self->field_5e = 0;
    self->damageTimer = 0;
    self->empDisabled = 0;
    self->shieldHit = 0;
    self->armorHit = 0;
    self->hullHit = 0;
    self->gammaHit = 0;
    self->bombForce = 0;
    self->empForce = 0;
    (self->pad_dd[0]) = 0;
    (((uint8_t*)&self->empForce)[1]) = 0;  // RAWREAD: byte at +0xd9 punned through empForce (no modeled field)
}

void Player::addGun(Gun *gun, int slot) {
    if (this->guns != 0) {
        if ((unsigned int)slot < 4) {
            Array<Gun *> *arr = new Array<Gun *>();
            this->guns->data()[slot] = arr;
            this->guns->data()[slot]->push_back(gun);
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
        unsigned int n = ((Array<Gun *> *)guns->data()[0])->size();
        int *order = new int[n];
        for (int i = 0; i < (int)n; i++) {
            order[i] = this->guns->data()[0]->data()[i]->itemIndex;
        }

        bool sorted = true;
        int i = 1;
        int *table = g_cws_items;
        do {
            for (; i < (int)n; i++) {
                // RAWREAD: g_cws_items is an int* into an opaque engine container; +4 = its data ptr
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
                this->guns->data()[0]->data()[idx]->field_0x89 = 1;
                gGlobals->addSoundResourceToList(*sound);
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
                int sid = g_cws_sound3[g->itemIndex];
                g->field_0x89 = 1;
                (void)sid;
                gGlobals->addSoundResourceToList(*g_cws_sound2);
                return;
            }
        }
    }
}

__attribute__((minsize)) extern "C" void Player_PauseEngineSound(Player *self)
{
    FMOD::Event *event = self->engineEvent;
    if (event != 0) {
        self->enginePaused = ((FModSound *)(gFModSound))->pause(event);
    }
}

struct Mat { float m[12]; };

__attribute__((minsize)) extern "C" void Player_PlayEngineSound(Player *self, Vector *vec)
{
    self->enginePositionVec = vec;
    if (*((char *)gAppManager + 0xf) != 0) {
        Mat pos;
        MatrixGetPosition(&pos, self->transform);
        FMOD::Event *ev = ((FModSound *)(gFModSoundPtr[0]))->updateEvent3DAttributes(self->engineEvent, 0, (Vector *)self->enginePositionVec, (Vector *)&pos, false);
        self->engineEvent = ev;
        self->engineSoundPlaying = 1;
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
        for (unsigned int i = 0; i < enemies->size(); i++) {
            copy->push_back(enemies->data()[i]);
        }
    }
    Array<Array<Gun *> *> *guns = this->guns;
    if (guns != 0) {
        for (unsigned int i = 0; i < guns->size(); i++) {
            Array<Gun *> *slot = guns->data()[i];
            if (slot != 0) {
                for (unsigned int j = 0; j < slot->size(); j++) {
                    Gun *gun = slot->data()[j];
                    if (gun != 0) {
                        Gun_setEnemies(gun);
                        guns = this->guns;
                        slot = guns->data()[i];
                    }
                }
            }
        }
    }
}

__attribute__((minsize)) extern "C" void Player_StopEngineSound(Player *self)
{
    FMOD::Event *event = self->engineEvent;
    if (event != 0) {
        ((FModSound *)(gFModSound))->stop(event);
        self->engineSoundPlaying = 0;
        self->engineEvent = 0;
    }
}

// Player::playShootSound(int type, int channel, Vector *pos, float volume) — fire a
// weapon-shot sound. Picks the sound id from the per-shipgroup table when this ship is
// AI-controlled, otherwise from the per-weapon table. On a "looping" channel (one of the
// bits in 0x10c) an already-playing sound is just re-positioned instead of restarted.
void Player::playShootSound(int type, int channel, Vector *pos, float volume) {
    int soundId;
    if (this->kiPlayer == 0) {
        soundId = gShootSoundsByIndex[type];
    } else {
        unsigned int kind = static_cast<unsigned int>(this->kiPlayer->shipGroup);
        if (kind < 0xb) {
            soundId = gShootSoundsByType[kind];
        } else {
            soundId = 0x3d;
        }
    }

    FModSound *sound = reinterpret_cast<FModSound *>(gFModSoundPtr[0]);
    if (static_cast<unsigned int>(channel) < 9 && ((1 << (channel & 0xff)) & 0x10c) != 0) {
        if (sound->isPlaying(soundId) != 0) {
            if (reinterpret_cast<char *>(gAppManagerA)[0xf] != 0) {
                sound->updateEvent3DAttributes(soundId, pos, 0, false);
            }
            return;
        }
        if (reinterpret_cast<char *>(gAppManagerB)[0xf] == 0) {
            pos = 0;
        }
    } else if (reinterpret_cast<char *>(gAppManagerC)[0xf] == 0) {
        pos = 0;
    }
    sound->play(soundId, pos, 0, volume);
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
                        self->flShake = self->flShake + k_shootAt_inc;
                        Gun *g2 = self->guns->data()[slot]->data()[i];
                        g2->timer = 0;
                        if (self->playShootSoundFlag != 0 && g2->field_0x89 != 0) {
                            float tmp[3];
                            MatrixGetPosition(tmp, self->transform);
                            Gun *g3 = self->guns->data()[slot]->data()[i];
                            self->playShootSound(g3->itemIndex, g3->weaponType,
                                                 reinterpret_cast<Vector *>(tmp),
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

// Full damage implementation. Player::damage(int) forwards here with flag=false,
// missionId=-1.
void Player::damage(int amount, bool flag, int missionId) {
    Player *self = this;
    if (self->vulnerable == 0) return;
    if (self->active == 0) return;
    if (self->hitpoints < 1) return;

    if (flag == 0 && self->kiPlayer != 0) {
        KIPlayer *ki = self->kiPlayer;
        if (self->alwaysEnemy == 0 &&
            (unsigned int)(ki->shipGroup - 9) > 1 &&
            gStatus->getSystem() != 0 &&
            ((self->enemyFlags == 0) || (self->turnedEnemyFlag != 0))) {
            ki = self->kiPlayer;
            if (ki->field_0x42 != 0) {
                if (amount > 0) {
                    self->damageDoneByPlayer = self->damageDoneByPlayer + amount;
                    ((Level *)(ki->level))->attackWanted(ki->field_0x48);
                }
                goto LAB_3488;
            }
        } else {
            ki = self->kiPlayer;
        }

        if (ki != 0 && self->alwaysEnemy == 0 &&
            (unsigned int)(ki->shipGroup - 9) > 1 &&
            self->kiPlayer->isWingMan() == 0 && gStatus->getSystem() != 0 &&
            ((self->enemyFlags == 0) || (self->turnedEnemyFlag != 0))) {
            int race = self->kiPlayer->shipGroup;
            gStatus->getSystem();
            bool sameRace = (race == ((SolarSystem *)(intptr_t)gStatus->getSystem())->getRace());
            if (!sameRace) {
                int race2 = self->kiPlayer->shipGroup;
                void *sys = (void *)(long)gStatus->getSystem();
                if (race2 != ((SolarSystem *)(sys))->getAttackRace()) {
                    goto LAB_342a;
                }
            }
            self->damageDoneByPlayer = self->damageDoneByPlayer + amount;
            int hc = gStatus->hardCoreMode();
            float thr1 = (hc != 0) ? k_damage_hc : k_damage_full;
            float f1 = thr1 * (float)self->maxHitpoints;
            float dmgF = (float)self->damageDoneByPlayer;
            float frac2 = (hc != 0) ? 0.25f : 0.5f;
            float thr3 = (hc != 0) ? k_damage_hc2 : k_damage_full2;

            if (f1 < dmgF) {
                ((Level *)(self->kiPlayer->level))->friendTurnedEnemy(0);
                void *ship = (void *)gStatus->getShip();
                void *standing = (void *)(long)gStatus->getStanding();
                if (((Ship *)(ship))->getSignatureRace() >= 0) {
                    bool match = ((unsigned int)((Ship *)(ship))->getSignatureRace() ==
                                  (unsigned int)self->kiPlayer->shipGroup);
                    unsigned int dis = 0;
                    if (match) dis = (unsigned char)self->kiPlayer->field_0x42;
                    if (match && dis == 0) {
                        Item *item = ((Ship *)(ship))->getFirstEquipmentOfSort(0x1d);
                        ((Ship *)(ship))->removeEquipment(item);
                        void *st2 = (void *)(long)gStatus->getStanding();
                        ((Standing *)(st2))->applyDelict(((Ship *)(ship))->getSignatureRace(), 100);
                        ((Standing *)(standing))->setPlayerSignatureRace(-1);
                        void *ego = (void *)(__INTPTR_TYPE__)((Level *)(self->kiPlayer->level))->getPlayer();
                        int hud = (int)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getHUD();
                        PlayerEgo *p = ((Level *)(self->kiPlayer->level))->getPlayer();
                        ((Hud *)(hud))->hudEvent(0x1f, p, 0);
                    }
                }
            }

            float f3 = (float)self->maxHitpoints;
            float dmgF3 = (float)self->damageDoneByPlayer;
            if (frac2 * f3 < dmgF3) {
                void *ship = (void *)gStatus->getShip();
                void *standing = (void *)(long)gStatus->getStanding();
                if (((Ship *)(ship))->getSignatureRace() >= 0 &&
                    self->kiPlayer->shipGroup < 4 &&
                    self->kiPlayer->field_0x42 == 0) {
                    Item *item = ((Ship *)(ship))->getFirstEquipmentOfSort(0x1d);
                    ((Ship *)(ship))->removeEquipment(item);
                    void *st2 = (void *)(long)gStatus->getStanding();
                    ((Standing *)(st2))->applyDelict(((Ship *)(ship))->getSignatureRace(), 100);
                    ((Standing *)(standing))->setPlayerSignatureRace(-1);
                    void *ego = (void *)(__INTPTR_TYPE__)((Level *)(self->kiPlayer->level))->getPlayer();
                    int hud = (int)(__INTPTR_TYPE__)((PlayerEgo *)(ego))->getHUD();
                    PlayerEgo *p = ((Level *)(self->kiPlayer->level))->getPlayer();
                    ((Hud *)(hud))->hudEvent(0x1f, p, 0);
                }
                self->turnedEnemyFlag = 1;
            }

            float f4a = (float)self->maxHitpoints;
            float dmgF4 = (float)self->damageDoneByPlayer;
            if (thr3 * f4a < dmgF4) {
                ((Level *)(self->kiPlayer->level))->alarmAllFriends(self->kiPlayer->shipGroup, true);
            }
            goto LAB_3488;
        }
    }

LAB_342a:
    {
        // NOTE: the "g_damage_globals" holder actually resolves (GOT 0x22015c) to the
        // Status singleton, not Globals (prior-session label was wrong). The write below
        // targets a Status field at offset 0x110 as a 16-bit store.
        if (gStatus->inBlackMarketSystem() != 0) {
            KIPlayer *ki = self->kiPlayer;
            if (ki != 0 && ki->shipGroup == 8) {
                self->turnedEnemyFlag = 1;
                ((Level *)(ki->level))->alarmAllFriends(8, true);
                Array<KIPlayer *> *enemies = ((Level *)(ki->level))->getEnemies();
                if (enemies != nullptr) {
                    unsigned count = enemies->size();
                    unsigned i = 0;
                    while (count != i) {
                        KIPlayer *e = (*enemies)[i];
                        i++;
                        if (e->shipGroup == 8) {
                            e->field_0x25 = 1;
                        }
                    }
                }
                gStatus->field_110 = 0x100;
            }
        }
    }

LAB_3488:
    {
        int shieldI = (int)self->shieldHP - amount;
        if (shieldI < 0) {
            self->shieldHP = 0.0f;
            shieldI = shieldI + self->armorHP;
            if (shieldI < 0) {
                self->armorHP = 0;
                self->hullHit = 1;
                self->hitpoints = shieldI + self->hitpoints;
            } else {
                self->armorHit = 1;
                self->armorHP = shieldI;
            }
        } else {
            self->shieldHit = 1;
            self->shieldHP = (float)shieldI;
        }
    }

    {
        int hp;
        KIPlayer *ki = self->kiPlayer;
        if (ki != 0 && ki->stealFlag == 0 && *(char *)((char *)ki + 0x3d) == 0 &&  // RAWREAD: ki+0x3d (no KIPlayer member at +0x3d; byte inside stealFlag int at 0x3c)
            ki->field_0x42 != 0) {
            hp = self->hitpoints;
            if (__aeabi_idiv(self->maxHitpoints, 3) > hp) {
                ((Level *)(ki->level))->almostKillWanted(ki->field_0x48);
            } else {
                goto LAB_34f8_hp;
            }
        }
        hp = self->hitpoints;
    LAB_34f8_hp:
        if (hp < 1) {
            self->hitpoints = 0;
            if (flag != 0) {
                self->destroyed = 1;
            } else {
                KIPlayer *ki2 = self->kiPlayer;
                if (ki2 != 0 && ki2->stealFlag == 0 && *(char *)((char *)ki2 + 0x3d) == 0 &&  // RAWREAD: ki2+0x3d (no KIPlayer member at +0x3d; byte inside stealFlag int at 0x3c)
                    gStatus->inBlackMarketSystem() == 0) {
                    if (self->kiPlayer->field_0x42 == 0) {
                        void *st = (void *)(long)gStatus->getStanding();
                        ((Standing *)(st))->applyKill(self->kiPlayer->shipGroup);
                    }
                    int mission = gStatus->getCampaignMission();
                    KIPlayer *ki3 = self->kiPlayer;
                    // NOTE: original text-table key for this name lookup was dropped by the
                    // decompiler; using missionId (the only contextual id) to keep it compiling.
                    void *txt = ((GameText *)(*g_damage_text[0]))->getText(missionId);
                    int cmp = (&ki3->name)->Compare_str((String *)txt);
                    if (missionId == 0xb3 && cmp == 0) {
                        // Ghidra: mission->setStatusValue(mission->getStatusValue() + 1)
                        ((Mission *)(intptr_t)mission)->setStatusValue(
                            ((Mission *)(intptr_t)mission)->getStatusValue() + 1);
                    }
                    if (self->kiPlayer->field_0x42 != 0) {
                        ((Level *)(self->kiPlayer->level))->killWanted(0);
                    }
                }
            }
        }
    }

    self->damaged = 1;
    self->flShake = self->flShake + k_damage_regen;
    ((Player *)(self))->updateDamageRate();
    if (self->kiPlayer != 0) {
        int slave = self->kiPlayer->field_0x10;
        if (slave != 0) {
            Player *other = *(Player **)(slave + 4);     // RAWREAD: slave+4 (slave is an untyped int handle, not a modeled class)
            other->vulnerable = 1;
            other->damage(amount);
            *(char *)(*(int *)(self->kiPlayer->field_0x10 + 4) + 0xc2) = 0;  // RAWREAD: slave+4, +0xc2 (untyped int handle)
        }
    }
}

void Player::setEnemy(Player *enemy) {
    Array<Player *> *tmp = new Array<Player *>();
    tmp->push_back(enemy);
    ((Player *)(this))->setEnemies(tmp);
    delete tmp;
}

void Player::addEnemy(Player *enemy) {
    if (this->enemies == 0) {
        this->setEnemy(enemy);
        return;
    }
    Array<Player *> *tmp = new Array<Player *>();
    if (this->enemies->size() != 0) {
        for (unsigned int i = 0; i < this->enemies->size(); i++) {
            tmp->push_back(this->enemies->data()[i]);
        }
    }
    tmp->push_back(enemy);
    ((Player *)(this))->setEnemies(tmp);
    delete tmp;
}

__attribute__((minsize)) extern "C" void Player_ResumeEngineSound(Player *self, bool force)
{
    FMOD::Event *event = self->engineEvent;
    if (event != 0 && (self->enginePaused != 0 || force)) {
        self->enginePaused = ((FModSound *)(gFModSound))->resume(event) ^ 1;
    }
}

void Player::damage(int amount) {
    this->damage(amount, false, -1);
}

void Player::stopShooting(int slot, int channel) {
    if ((unsigned int)(channel - 0x16) >= 9) {
        return;
    }
    Array<Array<Gun *> *> *guns = this->guns;
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
        ((Player *)(this))->stopShootSound(gun->itemIndex, gun->weaponType);
        arr = this->guns->data()[slot];
    }
}

void Player::setAlwaysFriend(bool value) {
    this->alwaysFriend = value;
    this->enemyFlags = 0x100;
    this->turnedEnemyFlag = 0;
}

void Player::setMaxArmorHP(int value) {
    this->armorHP = value;
    this->maxArmorHP = value;
    this->updateDamageRate();
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
                        // RAWREAD: byte at +0x69 of g->lifetimes (an int* projectile buffer, not a modeled class)
                        *(char *)((intptr_t)g->lifetimes + 0x69) = 1;
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
                        self->flShake = self->flShake + k_shoot_inc;
                        if (self->playShootSoundFlag != 0) {
                            float tmp[3];
                            MatrixGetPosition(tmp, self->transform);
                            Gun *g2 = self->guns->data()[slot]->data()[i];
                            self->playShootSound(g2->itemIndex, g2->weaponType,
                                                 reinterpret_cast<Vector *>(tmp),
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

Vector * Player::update(int dt, bool doSound) {
    Player *self = this;

    int b4 = self->damageTimer + dt;
    self->damageTimer = b4;
    if (b4 > 3000) {
        self->damaged = 0;
        self->damageTimer = 0;
    }

    Vector *result = 0;

    if (self->empDisabled != 0) {
        int e = self->empData + dt;
        float ef = (float)self->empData;
        float ef2 = (float)e;
        int maxEmp = self->maxEmpPoints;
        float mf = (float)maxEmp;
        float v = (ef2 / ef) * mf;
        self->empData = e;
        self->empPoints = (int)v;
        if (maxEmp < (int)v) {
            self->empDisabled = 0;
            self->empPoints = maxEmp;
            gStatus->field_134 = gStatus->field_134 - 1;
            self->empData = 0;
        }
        ((Player *)(self))->updateDamageRate();
    }

    char *flagObj = (char *)gEngine;
    if (*(char *)(flagObj + 0xf) == 0 || doSound == 0 || self->enginePositionVec == (void *)(__INTPTR_TYPE__)-1) {
        if (self->engineSoundPlaying != 0) {
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
        FMOD::Event *ev = ((FModSound *)(*g_update_sound))->updateEvent3DAttributes(self->engineEvent, 0, (Vector *)tmpB, (Vector *)tmpC, false);
        self->engineEvent = ev;
        self->engineSoundPlaying = 1;
        fn(tmpA, local, (int)(long)transform);
        *(Vector *)self->position = *(Vector *)tmpA;
    }

    float speed = (float)dt;
    float nf = self->flShake + speed * k_update_a * k_update_b;
    self->flShake = nf;
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
    int *m = (int *)this->transform;
    Player_shoot_full2(this, a, b, (int)pos, (int)((unsigned long long)pos >> 32),
                       m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7],
                       m[8], m[9], m[10], m[11], m[12], m[13], m[14]);
}

// shoot(int,int,long long,bool,Matrix): primary/turret fire worker. For slot `b`,
// fires every gun whose itemIndex matches the requested id, using the supplied
// firing matrix. (Body recovered from 0xb3d5c.)
void Player::shoot(int a, int b, long long pos, bool flag, Matrix mat) {
    Player *self = this;
    unsigned int mask = g_shoot_mask;

    Array<Array<Gun *> *> *guns = self->guns;
    if (guns != 0 && self->shootingEnabled != 0 && b >= 0 &&
        (unsigned int)b < guns->size()) {
        Array<Gun *> *arr = guns->data()[b];
        if (arr != 0) {
            for (unsigned int i = 0; i < arr->size(); i++) {
                Gun *g = self->guns->data()[b]->data()[i];
                unsigned int sortIdx = g->weaponType - 6;
                if (sortIdx < 0x1d && ((1u << (sortIdx & 0xff)) & mask) != 0 &&
                    *(int *)g->lifetimes >= 0) {
                    ((Gun *)(g))->ignite();
                } else if (g->itemIndex == (int)pos && g->fireDelay < g->timer) {
                    if (sortIdx < 0x1d && ((1u << (sortIdx & 0xff)) & mask) != 0) {
                        // RAWREAD: byte at +0x69 of g->lifetimes (an int* projectile buffer, not a modeled class)
                        *(char *)((intptr_t)g->lifetimes + 0x69) = 1;
                    }
                    ((Gun *)(g))->shoot(mat, flag, false);
                    self->flShake = self->flShake + k_shoot_inc;
                    if (self->playShootSoundFlag != 0) {
                        float tmp[3];
                        MatrixGetPosition(tmp, self->transform);
                        Gun *g2 = self->guns->data()[b]->data()[i];
                        self->playShootSound(g2->itemIndex, g2->weaponType,
                                             reinterpret_cast<Vector *>(tmp),
                                             g2->field_0xb0);
                    }
                    g->timer = 0;
                    break;
                }
            }
        }
    }
    (void)a;
}

// shoot(int,long long,bool): secondary launch through the hull transform; expands
// fields 0x04..0x3c into the firing matrix and forwards to the Matrix-taking
// overload, then returns success. (Body recovered from 0xb3f30.)
void Player::shoot(int a, long long pos, bool flag) {
    Matrix mat;
    float *src = this->transform;
    for (int k = 0; k < 15; ++k) mat.m[k] = src[k];
    mat.m[15] = 1.0f;
    this->shoot(a, pos, flag, mat);
}

// shoot(int,long long,bool,Matrix): secondary-launch worker. For slot `a`, every
// ready gun fires via Gun::shootAt with the supplied matrix. (Body recovered from
// 0xb3b20.)
void Player::shoot(int a, long long pos, bool flag, Matrix mat) {
    Player *self = this;

    Array<Array<Gun *> *> *guns = self->guns;
    if (guns != 0 && self->shootingEnabled != 0 && a >= 0 &&
        (unsigned int)a < guns->size()) {
        Array<Gun *> *arr = guns->data()[a];
        if (arr != 0) {
            for (unsigned int i = 0; i < arr->size(); i++) {
                Gun *g = self->guns->data()[a]->data()[i];
                if (g->fireDelay < g->timer) {
                    ((Gun *)(g))->shootAt(mat, (int)pos, self, flag);
                    self->flShake = self->flShake + k_shootAt_inc;
                    Gun *g2 = self->guns->data()[a]->data()[i];
                    g2->timer = 0;
                    if (self->playShootSoundFlag != 0 && g2->field_0x89 != 0) {
                        float tmp[3];
                        MatrixGetPosition(tmp, self->transform);
                        Gun *g3 = self->guns->data()[a]->data()[i];
                        self->playShootSound(g3->itemIndex, g3->weaponType,
                                             reinterpret_cast<Vector *>(tmp),
                                             g3->field_0xb0);
                    }
                }
            }
        }
    }
}

// Player::stopShooting(int) — single-arg overload (no channel filter).
void Player::stopShooting(int slot) {
    Array<Array<Gun *> *> *guns = this->guns;
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
        ((Player *)(this))->stopShootSound(gun->itemIndex, gun->weaponType);
        arr = this->guns->data()[slot];
    }
}

float * Player::setHitVector(float x, float y, float z) {
    float *p = this->hitVector;
    *p++ = x;
    *p++ = y;
    *p++ = z;
    return p;
}

void Player::resetGunDelay(int slot) {
    Array<Array<Gun *> *> *guns = this->guns;
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
            arr->data()[i]->timer = 0;
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


// ---- PlayEngineSound_a4014 ----
void Player::PlayEngineSound(Vector *vec) {
    this->enginePositionVec = vec;
    if (*((char *)gAppManager + 0xf) != 0) {
        float pos[12];
        MatrixGetPosition(pos, this->transform);
        FMOD::Event *ev = ((FModSound *)gFModSoundPtr[0])->updateEvent3DAttributes(
            this->engineEvent, 0, (Vector *)this->enginePositionVec, (Vector *)pos, false);
        this->engineEvent = ev;
        this->engineSoundPlaying = 1;
    }
}

void Player::PlayEngineSound(int unused, Vector *vec) {
    (void)unused;
    this->enginePositionVec = vec;
    if (*((char *)gAppManager + 0xf) != 0) {
        float pos[12];
        MatrixGetPosition(pos, this->transform);
        FMOD::Event *ev = ((FModSound *)gFModSoundPtr[0])->updateEvent3DAttributes(
            this->engineEvent, 0, (Vector *)this->enginePositionVec, (Vector *)pos, false);
        this->engineEvent = ev;
        this->engineSoundPlaying = 1;
    }
}

// ---- PauseEngineSound_a4088 ----
void Player::PauseEngineSound() {
    FMOD::Event *event = this->engineEvent;
    if (event != 0) {
        this->enginePaused = ((FModSound *)gFModSound)->pause(event);
    }
}

// ---- ResumeEngineSound_a40ac ----
void Player::ResumeEngineSound(bool force) {
    FMOD::Event *event = this->engineEvent;
    if (event != 0 && (this->enginePaused != 0 || force)) {
        this->enginePaused = ((FModSound *)gFModSound)->resume(event) ^ 1;
    }
}

// ---- StopEngineSound_a40e0 ----
void Player::StopEngineSound() {
    FMOD::Event *event = this->engineEvent;
    if (event != 0) {
        ((FModSound *)gFModSound)->stop(event);
        this->engineSoundPlaying = 0;
        this->engineEvent = 0;
    }
}

// =====================================================================
// Resolved tail-branch targets (wiring pass complete).
//
// The decompiler split several Player methods at their final tail-branch
// (a `b.w` through an ARM/Thumb interworking veneer) into a separate
// `Player_<method>_tail` symbol. Each veneer was followed to its real
// target and the call sites were rewritten to call that target directly,
// so the standalone `_tail` shims no longer exist:
//   - all HP/armor/shield/emp setters + regen/damage helpers + damageEmp
//     -> Player::updateDamageRate()                  (veneer @ 0x72df0)
//   - addGun(Gun*/Array<Gun*>*)                      -> this->calcWeaponSounds()  (0x72fac)
//   - calcWeaponSounds secondary-slot tail           -> Globals_addSoundResourceToList()  (0x72298)
//   - stopShootSound tail                            -> FModSound::stop(int)  (0x724a8)
//   - playShootSound tail                            -> FModSound::play(int,Vector*,Vector*,float)  (0x71548)
//   - addEnemies/addEnemy empty-list short-circuit   -> this->setEnemies()/setEnemy()  (0x72eb0 / 0x72ebc)
// =====================================================================

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

